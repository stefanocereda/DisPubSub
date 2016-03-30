/*
 * broker.cc
 *
 *  Created on: 21/mar/2016
 *      Author: home
 */
#include "broker_init_m.h"
#include "message_m.h"
#include "subscribe_m.h"
#include <algorithm>
#include <omnetpp.h>
#include <string.h>
#include "parameters.h"

using namespace omnetpp;

class broker: public cSimpleModule {
private:
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

    for (int i = 0; i < n; i++) {
        Broker_init_msg *msg = new Broker_init_msg("broker");
        msg->setSrcId(this->getId());
        send(msg, "gate$o", i);
    }
}

void broker::handleMessage(cMessage *msg) {
    if (strcmp("subscribe", msg->getFullName()) == 0) {
        handleSubscribeMessage(dynamic_cast<Subscribe_msg*>(msg));
    } else if (strcmp("broker", msg->getFullName()) == 0) {
        handleBrokerInitMessage(dynamic_cast<Broker_init_msg*>(msg));
    } else if (strcmp("message", msg->getFullName()) == 0) {
        handleMessageMessage(dynamic_cast<Message_msg*>(msg));
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

