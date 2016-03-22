/*
 * broker.cc
 *
 *  Created on: 21/mar/2016
 *      Author: home
 */
#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class broker : public cSimpleModule
{
private:
    bool gateIdb;

  protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

// The module class needs to be registered with OMNeT++
Define_Module(broker);

void broker::initialize()
{
    if (strcmp("client1", getName()) == 0 || strcmp("client3", getName()) == 0) {
        // create and send first message on gate "out". "tictocMsg" is an
        // arbitrary string which will be the name of the message object.
        cMessage *msg = new cMessage("subscribe");
        send(msg, "out");
    }
}

void broker::handleMessage(cMessage *msg)
{
    // The handleMessage() method is called whenever a message arrives
    // at the module. Here, we just send it to the other module, through
    // gate `out'. Because both `tic' and `toc' does the same, the message
    // will bounce between the two.
    if(msg->arrivedOn("ic1")){
        cMessage *msg = new cMessage("ok");
        send(msg, "oc1"); // send out the message
        this->gateIdb = msg->arrivedOn("ic2");

    }

    if(msg->arrivedOn("ic2")){
        cMessage *msg = new cMessage("ok");
                send(msg, "oc2"); // send out the message
    }



}


