/*
 * broker.cc
 *
 *  Created on: 21/mar/2016
 *      Author: home
 */
#include <string.h>
#include <omnetpp.h>
#include "subscribe_m.h"
#include <algorithm>

class broker : public cSimpleModule
{
private:
    //The broker needs to know on which channels it should send the various topics, mapping is topic -> list of interested channels
    typedef std::map<int, std::list<int>> SubscriptionTable;
    SubscriptionTable subs_table;

  protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

// The module class needs to be registered with OMNeT++
Define_Module(broker);


void broker::initialize(){

}

void broker::handleMessage(cMessage *msg)
{
    //TODO: fare un check + carino del tipo
    Subscribe_msg *m = check_and_cast<Subscribe_msg *> (msg);

    int topic = m->getTopic();
    int channel = m->getArrivalGate()->getIndex();

    //check whether it is a new topic
    SubscriptionTable::iterator it = subs_table.find(topic);
    if (it == subs_table.end()){
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
}

