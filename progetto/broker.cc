/*
 * broker.cc
 *
 *  Created on: 21/mar/2016
 *      Author: home
 */
#include <string.h>
#include <omnetpp.h>
#include <Subscribe_msg_m.h>

using namespace omnetpp;

class broker : public cSimpleModule
{
  protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

// The module class needs to be registered with OMNeT++
Define_Module(broker);

void broker::initialize(){

    int n = gateSize("gate");


       for(int i = 0; i < n ; i++){
        cMessage *msg = new cMessage("broker");
        EV << "Sending message " << msg << " on gate[" << i << "]\n";
        send(msg, "gate$o", i);
    }

//    if (strcmp("client1", getName()) == 0 || strcmp("client3", getName()) == 0) {
        // create and send first message on gate "out". "tictocMsg" is an
        // arbitrary string which will be the name of the message object.
  //      cMessage *msg = new cMessage("subscribe");
    //    send(msg, "gate$o");
    //}
}

void broker::handleMessage(cMessage *msg)
{
    // The handleMessage() method is called whenever a message arrives
    // at the module. Here, we just send it to the other module, through
    // gate `out'. Because both `tic' and `toc' does the same, the message
    // will bounce between the two.


    if(strcmp("broker", msg->getFullName()) == 0){
        int i = msg->getArrivalGate()->getIndex();

        cMessage *m = new cMessage("Router Ok!");

        send(m, "gate$o",i);
    }

    if(strcmp("subscribe", msg->getFullName()) == 0){
        if(Subscribe_msg *m = dynamic_cast<Subscribe_msg*>(msg))
        EV << "Subscription" << m->getTopic();
    }

}


