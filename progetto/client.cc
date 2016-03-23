/*
 * client.cc
 *
 *  Created on: 21/mar/2016
 *      Author: home
 */
#include <string.h>
#include <omnetpp.h>
#include <Subscribe_msg_m.h>

using namespace omnetpp;

class client : public cSimpleModule
{
  protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

// The module class needs to be registered with OMNeT++
Define_Module(client);

void client::initialize()
{
      EV << this->getFullName() << " con id: " << this->getId() << "\n";

      Subscribe_msg *msg = new Subscribe_msg("subscribe");

      //Setup messaggio subscribe
      msg->setSrcId(this->getId());
      msg->setTopic(intuniform(0, 5));

      //Invio
      send(msg, "gate$o",0);
}




void client::handleMessage(cMessage *msg)
{
    // The handleMessage() method is called whenever a message arrives
    // at the module. Here, we just send it to the other module, through
    // gate `out'. Because both `tic' and `toc' does the same, the message
    // will bounce between the two.
//    send(msg, "out"); // send out the message
}



