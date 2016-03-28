/*
 * broker.cc
 *
 *  Created on: 21/mar/2016
 *      Author: home
 */
#include <string.h>
#include <omnetpp.h>
#include "subscribe_m.h"
#include "broker_init_m.h"
#include <algorithm>

using namespace omnetpp;

class broker : public cSimpleModule
{
private:
    //The broker needs to know on which channels it should send the various topics, mapping is topic -> list of interested channels
    typedef std::map<int, std::list<int>> SubscriptionTable;
    SubscriptionTable subs_table;

    //and where are the other brokers
    std::list<int> broker_gate_table;

    void handleSubscribeMessage(Subscribe_msg *m);
    void handleBrokerInitMessage(Broker_init_msg *m);

protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

// The module class needs to be registered with OMNeT++
Define_Module(broker);

//Tell everybody that we are up and running :)
void broker::initialize(){
    int n = gateSize("gate");

    for(int i = 0; i < n ; i++){
        Broker_init_msg *msg = new Broker_init_msg("broker");
        msg->setSrcId(this->getId());
        send(msg, "gate$o", i);
      }
}

void broker::handleMessage(cMessage *msg)
{
    if(strcmp("subscribe", msg->getFullName()) == 0){
        handleSubscribeMessage(dynamic_cast<Subscribe_msg*>(msg));
    }
    else if (strcmp("broker", msg->getFullName()) == 0){
        handleBrokerInitMessage(dynamic_cast<Broker_init_msg*>(msg));
    }
}

void broker::handleBrokerInitMessage(Broker_init_msg *m)
{
    //TODO: quando un nuovo broker si connette dovremmo mandargli tutte le nostre subscription
    int channel = m->getArrivalGate()->getIndex();
    broker_gate_table.push_back(channel);
}

void broker::handleSubscribeMessage(Subscribe_msg *m)
{
    int topic = m->getTopic();
    int channel = m->getArrivalGate()->getIndex();

    //check whether it is a new topic
    bool newTopic = false;
    SubscriptionTable::iterator it = subs_table.find(topic);
    if (it == subs_table.end()){
        newTopic = true;
        //build a new list
        std::list<int> toAdd;
        toAdd.push_back(channel);
        //and add to the table
        subs_table.insert(std::pair<int, std::list<int>>(topic, toAdd) );
    }
    else{
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
    if (newTopic){
        for (std::list<int>::const_iterator iterator = broker_gate_table.begin(), end = broker_gate_table.end(); iterator != end; ++iterator) {
            if (*iterator != channel){
                // Duplicate message and send the copy.
                Subscribe_msg *copy = (Subscribe_msg *) m->dup();
                send(copy, "gate$o", *iterator);
            }
       }
    }
}

