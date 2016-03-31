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
#include <algorithm>
#include <omnetpp.h>
#include <string.h>
#include "parameters.h"
using namespace omnetpp;

#define HUB_MODE 1
#define NORMAL_EXE 0

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

    void handleSubscribeMessage(Subscribe_msg *m);
    void handleBrokerInitMessage(Broker_init_msg *m);
    void handleMessageMessage(Message_msg *m);
    void updateStatusLeave(Leave_msg *m);
    void handleBrokerLeaveMessage(Leave_msg *m);
    void handleUnsubscribeMessage(Unsubscribe_msg *m);
    void sendBrokerLeaveMessage();
    void broadcast(cMessage *m);

protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

public:
    broker() :
            subs_counter(NTOPIC) {
    }
};

// The module class needs to be registered with OMNeT++
Define_Module(broker);

//Tell everybody that we are up and running :)
void broker::initialize() {
    int n = gateSize("gate");

    for (int i = 0; i < n; i++) {// Broadcast
        Broker_init_msg *msg = new Broker_init_msg("broker");
        msg->setSrcId(this->getId());
        send(msg, "gate$o", i);
    }

    broker_hub_mode = NORMAL_EXE;
}

void broker::handleMessage(cMessage *msg) {
    // In the first case I'm operative as a broker
    if(broker_hub_mode == 0){
        if (strcmp("subscribe", msg->getFullName()) == 0) {
            handleSubscribeMessage(dynamic_cast<Subscribe_msg*>(msg));
        } else if (strcmp("broker", msg->getFullName()) == 0) {
            handleBrokerInitMessage(dynamic_cast<Broker_init_msg*>(msg));
        } else if (strcmp("message", msg->getFullName()) == 0) {
            handleMessageMessage(dynamic_cast<Message_msg*>(msg));
        } else if(strcmp("client_leave", msg->getFullName()) == 0){
            updateStatusLeave(dynamic_cast<Leave_msg*>(msg));
        } else if(strcmp("broker_leave", msg->getFullName()) == 0){
            handleBrokerLeaveMessage(dynamic_cast<Leave_msg*>(msg));
        } else if(strcmp("unsubscribe", msg->getFullName()) == 0){
            handleUnsubscribeMessage(dynamic_cast<Unsubscribe_msg*>(msg));
        }
    }
    else{ // In this case I work as hub
        // I have to send the message to all the connected brokers except for the receiver

        // TODO: Should I work in different manner based on the type of message???
        // TODO: May I need a list of all the gates ? in order to broadcast messages also to clients.... and not only to brokers.
        broadcast(msg , msg->getArrivalGate()->getIndex());
    }
}

void broker::handleBrokerInitMessage(Broker_init_msg *m) {
    //add the broker to our list
    int channel = m->getArrivalGate()->getIndex();
    broker_gate_table.push_back(channel);

    //and send it our subscription list
    for (SubscriptionTable::const_iterator subs_it = subs_table.begin(), end = subs_table.end();
            subs_it != end; ++subs_it)
    {
        int topic = subs_it -> first;
        Subscribe_msg *m = new Subscribe_msg("subscribe");
        m->setSrcId(this->getId());
        m->setTopic(topic);
        send(m, "gate$o", channel);
    }
}

void broker::handleSubscribeMessage(Subscribe_msg *m) {
    int topic = m->getTopic();
    int channel = m->getArrivalGate()->getIndex();

    //check whether it is a new topic
    bool newTopic = false;
    SubscriptionTable::iterator it = subs_table.find(topic);

    EV << "The broker with id: " << this->getId() << " received a subscribe for the topic: " << topic
            << " from client with id: " << m->getSrcId() << "\n";

    subs_counter[topic]++;
    if (it == subs_table.end()) { // New topic
        newTopic = true;
        //build a new list
        std::list<int> toAdd;
        toAdd.push_back(channel);
        //and add to the table
        subs_table.insert(std::pair<int, std::list<int>>(topic, toAdd));


    } else {
        //otherwise get the current list of interested channels
        std::list<int> old = it->second;
        //check if the current channel is not already there
        if (std::find(old.begin(), old.end(), channel) != old.end())
            //and add it
            old.push_back(channel);
    }

    //OK, now we should send the subscription to all the channels except the one where we have received it
    //TODO broker centrale, gli arriva subscription arg1 da tutti e 3 i canali, come faccio a capire che non devo mandarla in giro?
    //no ma forse è giusto che la mandi, ci devo pensare
    if (newTopic) {
        for (std::list<int>::const_iterator iterator =
                broker_gate_table.begin(), end = broker_gate_table.end();
                iterator != end; ++iterator) {
            if (*iterator != channel) {
                // Duplicate message and send the copy.
                Subscribe_msg *copy = (Subscribe_msg *) m->dup();
                send(copy, "gate$o", *iterator);
            }
        }
    }
}

void broker::handleMessageMessage(Message_msg *m) {
    int topic = m->getTopic();
    int in_chan = m->getArrivalGate()->getIndex();


    //search all the channels interested in the topic
    SubscriptionTable::iterator topic_it = subs_table.find(topic);
    if (topic_it != subs_table.end()){
        std::list<int> chans_list = topic_it -> second;
        for (std::list<int>::const_iterator chans_it = chans_list.begin(), end = chans_list.end();
               chans_it != end; ++chans_it) {
                if(*chans_it != in_chan){
                    Message_msg *copy = (Message_msg *)m->dup();
                    send(copy, "gate$o", *chans_it);
                }
        }
    }

}

void broker::updateStatusLeave(Leave_msg *m){
    // After a leave_client,leave_broker update all the lists and maybe start an unsubscribe event.

    // Understand who is the leaver
    int in_chan = m->getArrivalGate()->getIndex();

    std::map<int, std::list<int>>::const_iterator topics_it,end_topic;
    std::list<int>::const_iterator chans_it,end_chan,chans_unsub_it,end_unsub_end;

    // Iterate on topics
    for (topics_it = subs_table.begin(), end_topic = subs_table.end();  topics_it != end_topic; ++topics_it) {
        std::list<int> chans_list = topics_it->second;

        // For each channel referred to the current topic
        for ( chans_it = chans_list.begin(), end_chan = chans_list.end();
                       chans_it != end_chan; ++chans_it) {

            // If the current channel is the leaver I have to remove it
            if(*chans_it == in_chan){
                chans_it = chans_list.erase(chans_it);
                subs_counter[topics_it->first]--;

                // If I have no more follower I start a new unsubscribe chain
                if(subs_counter[topics_it->first] == 0){
                    // Create a unsubscribe message referred to the current topic
                    Unsubscribe_msg *unsubscribe = new Unsubscribe_msg("unsubscribe");
                    unsubscribe->setTopic(topics_it->first);

                    // I have to broadcast the unsubscribe to all the channel except for the leaver that is chans_it
                    // non dovrebbe mandarle anceh ai nodi client ? altrimenti questi stanno fregati e non riceveranno più nulla
                    broadcast(unsubscribe , chans_it);
                }
            }
        }
    }
}

void broker::sendBrokerLeaveMessage(){
    // I send in broadcast to all the connected brokers that I'm leaving and then I pass to the hub_mode
    Leave_msg *leave = new Leave_msg("broker_leave");

    // The inverse may cause problem by still being in normal_exe even after the leave
    broker_hub_mode = HUB_MODE;
    broadcast(leave,null);

}


void broker::handleBrokerLeaveMessage(Leave_msg *m){
    // It only update the status after the receiving of a leave by a broker

    // TODO What if we merge it with the Client method that is updateStatusLeave ?
    updateStatusLeave(m);

}

void broker::handleUnsubscribeMessage(Unsubscribe_msg *m){
    // Handle the unsubscribe message as a leave on a specific topic

    // Get the topic and the unsubscriber_channel from the message
    int topic = m->getTopic();
    int in_chan = m->getArrivalGate()->getIndex();

    // Get the iterator referred on the topic of the message
    SubscriptionTable::iterator topic_it = subs_table.find(topic);
        // If is not empty
        if (topic_it != subs_table.end()){
            // I get the lists of subscribers to such a topic
            std::list<int> chans_list = topic_it -> second;

            // I basically iterate on such a lists of subscribers in order to find the one that unsubscribe and update the status
            for (std::list<int>::const_iterator chans_it = chans_list.begin(), end = chans_list.end();
                   chans_it != end; ++chans_it) {

                // If the current channel is the unsubscriber I have to remove it from the subscribers of this topic and decrement the subs_counter
                if(*chans_it == in_chan){
                    chans_it = chans_list.erase(chans_it);
                    subs_counter[topics_it->first]--;

                    // If I have no more follower I continue the already started unsubscribe chain
                    if(subs_counter[topics_it->first] == 0){
                        // Create a unsubscribe message referred to the unsubscribe topic
                        Unsubscribe_msg *unsubscribe = new Unsubscribe_msg("unsubscribe");
                        unsubscribe->setTopic(topic);

                        broadcast(unsubscribe,in_chan);
                    }
                }
            }
        }

}

void broker::broadcast(cMessage *m , int except_channel){
    // Method that sends a message in broadcasts to all the brokers channels except from the one from which has receives it

    for (std::list<int>::const_iterator chans_it = broker_gate_table.begin(), end = broker_gate_table.end();
           chans_it != end; ++chans_it) {
            if(*chans_it != except_channel){
                Message_msg *copy = (Message_msg *)m->dup();
                send(copy, "gate$o", *chans_it);
            }
    }
}
