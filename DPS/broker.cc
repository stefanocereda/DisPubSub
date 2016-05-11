/*
 * broker.cc
 *
 *  Created on: 21/mar/2016
 *      Author: home
 */
#include "broker_init_m.h"
#include "message_m.h"
#include "unsubscribe_m.h"
#include "subscribe_m.h"
#include "leave_m.h"
#include "join_m.h"
#include <algorithm>
#include <omnetpp.h>
#include <string.h>
#include "parameters.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <future>
#include <functional>
#include <ack_join_m.h>
#include <assert.h>

using namespace omnetpp;

// Broker working modalities
#define HUB_MODE 1
#define NORMAL_EXE 0

// Broadcast function modalities
#define ONLY_BROKERS 0
#define ALL_GATES 1

class broker: public cSimpleModule {
private:
    //States if the broker is up (0) or not (1)
    int broker_hub_mode;

    //The broker needs to know on which channels it should send the various topics, mapping is topic -> list of interested channels
    typedef std::map<int, std::list<int>> SubscriptionTable;
    SubscriptionTable subs_table;

    //Per each position-topic how many subscriptions
    std::vector<int> subs_counter;

    //and where are the other brokers
    std::list<int> broker_gate_table;

    int recSubs = 0, sentSubs = 0, recPubs = 0, sentPubs = 0;

    void handleSubscribeMessage(Subscribe_msg *m);
    void handleBrokerInitMessage(Broker_init_msg *m);
    void handleMessageMessage(Message_msg *m);
    void updateStatusLeave(Leave_msg *m);
    void handleClientLeaveMessage(Leave_msg *m);
    void handleClientJoinMessage(Join_msg *m);
    void handleBrokerLeaveMessage(Leave_msg *m);
    void handleBrokerJoinMessage(Join_msg *m);
    void handleUnsubscribeMessage(Unsubscribe_msg *m);
    void handleAckJoinMessage(Ack_join_msg *m);
    void sendBrokerLeaveMessage();
    void sendJoinMessage();
    void sendJoinMessage(int delay);
    void broadcast(cMessage *m, int except_channel, int mode);
    void broadcast(cMessage *m, int except_channel, int mode, int delay);
    void bundleCycle();

protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

public:
    broker() :
            subs_counter(NTOPIC) {
    }
};

// The module class needs to be registered with OMNeT++
Define_Module(broker);

//Tell everybody that we are up and running :)
void broker::initialize() {
    broker_hub_mode = NORMAL_EXE;

    int n = gateSize("gate");

    for (int i = 0; i < n; i++) { // Broadcast
        Broker_init_msg *msg = new Broker_init_msg("broker");
        msg->setSrcId(this->getId());
        send(msg, "gate$o", i);
    }

    //will this broker leave?
    if (rand() % 100 <= BROKER_LEAVE_PROBABILITY * 100) {
        sendBrokerLeaveMessage();
    }

    /*just for testing*/
#if MODE == BROKER_LEAVE
    if (this->getId() == 2) {
        sendBrokerLeaveMessage();
    }
#endif
}

void broker::handleMessage(cMessage *msg) {
    // In the first case I'm operative as a broker
    if (broker_hub_mode == 0) {
        if (strcmp("subscribe", msg->getFullName()) == 0) {
            handleSubscribeMessage(dynamic_cast<Subscribe_msg*>(msg));
        } else if (strcmp("broker", msg->getFullName()) == 0) {
            handleBrokerInitMessage(dynamic_cast<Broker_init_msg*>(msg));
        } else if (strcmp("message", msg->getFullName()) == 0) {
            handleMessageMessage(dynamic_cast<Message_msg*>(msg));
        } else if (strcmp("client_leave", msg->getFullName()) == 0) {
            handleClientLeaveMessage(dynamic_cast<Leave_msg*>(msg));
        } else if (strcmp("broker_leave", msg->getFullName()) == 0) {
            handleBrokerLeaveMessage(dynamic_cast<Leave_msg*>(msg));
        } else if (strcmp("broker_join", msg->getFullName()) == 0) {
            handleBrokerJoinMessage(dynamic_cast<Join_msg*>(msg));
        } else if (strcmp("unsubscribe", msg->getFullName()) == 0) {
            handleUnsubscribeMessage(dynamic_cast<Unsubscribe_msg*>(msg));
        } else if (strcmp("ack_join", msg->getFullName()) == 0) {
            return;
        } else
            EV << "unknown message" << endl;
    } else { // In this case I work as hub
             // I have to send the message to all the connected brokers and clients except for the receiver
        if (strcmp("ack_join", msg->getFullName()) == 0) {
            handleAckJoinMessage(dynamic_cast<Ack_join_msg*>(msg));
        } else if (strcmp("unsubscribe", msg->getFullName()) == 0) {
            return;
        } else {
            broadcast(msg, msg->getArrivalGate()->getIndex(), ALL_GATES);
        }
    }
}

void broker::handleBrokerInitMessage(Broker_init_msg *m) {
    //add the broker to our list
    int channel = m->getArrivalGate()->getIndex();
    broker_gate_table.push_back(channel);

    //and send it our subscription list
    for (SubscriptionTable::const_iterator subs_it = subs_table.begin(), end =
            subs_table.end(); subs_it != end; ++subs_it) {
        int topic = subs_it->first;
        Subscribe_msg *msg = new Subscribe_msg("subscribe");
        msg->setSrcId(this->getId());
        msg->setTopic(topic);
        send(msg, "gate$o", channel);
    }
}

void broker::handleSubscribeMessage(Subscribe_msg *m) {
    recSubs++;
    int topic = m->getTopic();
    int channel = m->getArrivalGate()->getIndex();
    bool isNew = false;

    SubscriptionTable::iterator it = subs_table.find(topic);


    if (it == subs_table.end()) { // New topic
        isNew = true;
        subs_counter[topic]++;

        //build a new list
        std::list<int> toAdd;
        toAdd.push_back(channel);
        //and add to the table
        subs_table.insert(std::pair<int, std::list<int>>(topic, toAdd));

    } else {
        //otherwise get the current list of interested channels
        std::list<int> *old = &(it->second);
        //check if the current channel is not already there
        if (std::find(old->begin(), old->end(), channel) == old->end()) {
            //and add it
            old->push_back(channel);
            isNew = true;
            subs_counter[topic]++;
        }
    }

    //OK, now we should send the subscription to all the channels except the one where we have received it
    if (isNew) {
        for (std::list<int>::const_iterator iterator =
                broker_gate_table.begin(), end = broker_gate_table.end();
                iterator != end; ++iterator) {
            if (*iterator != channel) {
                // Duplicate message and send the copy.
                Subscribe_msg *copy = (Subscribe_msg *) m->dup();
                send(copy, "gate$o", *iterator);
                sentSubs++;
            }
        }
    }
}

void broker::handleMessageMessage(Message_msg *m) {
    int topic = m->getTopic();
    int in_chan = m->getArrivalGate()->getIndex();
    recPubs++;

    //search all the channels interested in the topic
    SubscriptionTable::iterator topic_it = subs_table.find(topic);
    if (topic_it != subs_table.end()) {
        std::list<int> chans_list = topic_it->second;
        for (std::list<int>::const_iterator chans_it = chans_list.begin(), end =
                chans_list.end(); chans_it != end; ++chans_it) {
            if (*chans_it != in_chan) {
                Message_msg *copy = (Message_msg *) m->dup();
                send(copy, "gate$o", *chans_it);
                sentPubs++;
            }
        }
    }

}

void broker::handleClientLeaveMessage(Leave_msg *m) {
    // Check consistency
    // Gate from which I receive the leave message
    int gate_index = m->getArrivalGate()->getIndex();

    if (std::find(std::begin(broker_gate_table), std::end(broker_gate_table),
            gate_index) == std::end(broker_gate_table)) {
        // Is received from a real client
        updateStatusLeave(m);
    } else {
        // Is received from a broker-hub
        return;
    }

}

void broker::updateStatusLeave(Leave_msg *m) {
    // After a leave_client,leave_broker update all the lists and maybe start an unsubscribe event.

    // Understand who is the leaver
    int in_chan = m->getArrivalGate()->getIndex();

    std::map<int, std::list<int>>::iterator topics_it, end_topic;
    std::list<int>::const_iterator chans_it, end_chan, chans_unsub_it,
            end_unsub_end;

    // Iterate on topics
    for (topics_it = subs_table.begin(), end_topic = subs_table.end();
            topics_it != end_topic; ++topics_it) {
        std::list<int> *chans_list = &(topics_it->second);

        // For each channel referred to the current topic
        for (chans_it = chans_list->begin(), end_chan = chans_list->end();
                chans_it != end_chan; ++chans_it) {

            // If the current channel is the leaver I have to remove it
            if (*chans_it == in_chan) {
                chans_it = chans_list->erase(chans_it);
                subs_counter[topics_it->first]--;

                // If I have no more follower I start a new unsubscribe chain
                if (subs_counter[topics_it->first] == 0) {

                    // Create a unsubscribe message referred to the current topic
                    Unsubscribe_msg *unsubscribe = new Unsubscribe_msg(
                            "unsubscribe");
                    unsubscribe->setTopic(topics_it->first);

                    /*EV << "\n Broker with id " << this->getId()
                     << " unsubscribe to the topic "
                     << topics_it->first;

                     EV << "\n current chan_list values: ";*/
                    for (std::list<int>::iterator iter = chans_list->begin();
                            iter != chans_list->end(); iter++) {
                        /*EV << *iter << " , ";*/
                    }

                    /*EV << "\n Broker with id " << this->getId()
                     << " BROADCAST the UNSUBSCRIBE";
                     EV << "\n Except-Channel " << *chans_it;*/
                    broadcast(unsubscribe, in_chan, ONLY_BROKERS);

                    //and also remove it from the map
                    subs_table.erase(topics_it);

                }
            }
        }
    }

}

void broker::handleAckJoinMessage(Ack_join_msg *m) {

    if (broker_hub_mode != NORMAL_EXE) {
        //The broker goes in normal execution
        broker_hub_mode = NORMAL_EXE;

    }

}

//We are leaving, send a self message for timing
void broker::sendBrokerLeaveMessage() {
    // I send in broadcast to all the connected brokers and clients that I'm leaving and then I pass to the hub_mode
    Leave_msg *leave = new Leave_msg("broker_leave");
    leave->setSrcId(this->getId());

    int delay = intuniform(MIN_BLEAVE_DELAY, MAX_BLEAVE_DELAY);
    scheduleAt(simTime() + delay, leave);
}

void broker::sendJoinMessage() {
    sendJoinMessage(0);
}

// I send in broadcast to all the connected brokers and clients that I'm joining and then I pass to the hub_mode
void broker::sendJoinMessage(int delay) {
    Join_msg *join = new Join_msg("broker_join");

    broadcast(join, -1, ALL_GATES, delay);
}

//Go in hub mode, reset everything and schedule the re join
void broker::handleBrokerLeaveMessage(Leave_msg *m) {
    assert(m->isSelfMessage());
    assert(broker_hub_mode == NORMAL_EXE);

    //The broker becomes an hub
    broker_hub_mode = HUB_MODE;

    //Reset everything
    subs_table.clear();

    for (int i = 0; i < NTOPIC; ++i)
        subs_counter[i] = 0;

    int awakeningDelay = intuniform(MIN_HUB_TIME, MAX_HUB_TIME);

    sendJoinMessage(awakeningDelay);
}

void broker::handleBrokerJoinMessage(Join_msg *m) {
    Ack_join_msg *msg = new Ack_join_msg("ack_join");
    int channel = m->getArrivalGate()->getIndex();
    send(msg, "gate$o", channel);

//add the broker to our list
    broker_gate_table.push_back(channel);

//and send it our subscription list
    for (SubscriptionTable::const_iterator subs_it = subs_table.begin(), end =
            subs_table.end(); subs_it != end; ++subs_it) {
        int topic = subs_it->first;

        //if the only subscriber is the same broker we should not send it the subscription
        if (subs_it->second.size() == 1
                && std::find(subs_it->second.begin(), subs_it->second.end(),
                        channel) != subs_it->second.end()) {
            continue;
        }

        Subscribe_msg *m = new Subscribe_msg("subscribe");
        m->setSrcId(this->getId());
        m->setTopic(topic);
        send(m, "gate$o", channel);
    }
}

void broker::handleUnsubscribeMessage(Unsubscribe_msg *m) {
// Handle the unsubscribe message as a leave on a specific topic

// Get the topic and the unsubscriber_channel from the message
    int topic = m->getTopic();
    int in_chan = m->getArrivalGate()->getIndex();

    /*EV << " \n handling the unsubscribe-chain " << " for topic: " << topic
     << " in_chan " << in_chan;*/

// Get the iterator referred on the topic of the message
    SubscriptionTable::iterator topic_it = subs_table.find(topic);

// If is not empty
    if (topic_it != subs_table.end()) {
        // I get the lists of subscribers to such a topic
        std::list<int> *chans_list = &(topic_it->second);

        // I basically iterate on such a lists of subscribers in order to find the one that unsubscribe and update the status
        for (std::list<int>::const_iterator chans_it = chans_list->begin(),
                end = chans_list->end(); chans_it != end; ++chans_it) {

            // If the current channel is the unsubscriber I have to remove it from the subscribers of this topic and decrement the subs_counter
            if (*chans_it == in_chan) {
                chans_it = chans_list->erase(chans_it);
                subs_counter[topic_it->first]--;

                // If I have no more follower I continue the already started unsubscribe chain
                if (subs_counter[topic_it->first] == 0) {
                    // Create a unsubscribe message referred to the unsubscribe topic
                    Unsubscribe_msg *unsubscribe = new Unsubscribe_msg(
                            "unsubscribe");
                    unsubscribe->setTopic(topic);

                    /*EV << "\nBroker with id " << this->getId()
                     << " continue the unsubscribe chain for "
                     << topic_it->first;*/

                    // Send it in broadcast to only the connected brokers
                    broadcast(unsubscribe, in_chan, ONLY_BROKERS);

                    //and also remove it from the map
                    subs_table.erase(topic_it);
                }
            }
        }
    }
}

void broker::broadcast(cMessage *m, int except_channel, int mode) {
    broadcast(m, except_channel, mode, 0);
}

void broker::broadcast(cMessage *m, int except_channel, int mode, int delay) {
// Method that sends a message in broadcasts to all the brokers channels except from the one from which has receives it

    if (mode == ONLY_BROKERS) {

        for (std::list<int>::const_iterator chans_it =
                broker_gate_table.begin(), end = broker_gate_table.end();
                chans_it != end; ++chans_it) {

            if (*chans_it != except_channel) {
                cMessage *copy = m->dup();
                sendDelayed(copy, delay, "gate$o", *chans_it);
            }
        }
    } else { // RealBroadcast that is to all the gates/channels
// get the number of gates
        int n = gateSize("gate");

        for (int i = 0; i < n; i++) {
            if (i != except_channel) {
                cMessage *copy = (cMessage *) m->dup();
                sendDelayed(copy, delay, "gate$o", i);
            }
        }
    }
}

void broker::finish() {
    EV << "b," << this->getId() << "," << recSubs << "," << sentSubs << ","
              << recPubs << "," << sentPubs << endl;
}
