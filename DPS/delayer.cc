#include <omnetpp.h>
#include "message_m.h"

using namespace omnetpp;

class delayer: public cSimpleModule {
protected:
    virtual void handleMessage(cMessage *msg) override;

};

Define_Module(delayer);

void delayer::handleMessage(cMessage *msg) {
    //20 22 24 da girare
    int delay = 0;

    if (!strcmp(msg->getFullName(), "message")) {
        Message_msg *m = dynamic_cast<Message_msg*>(msg);

        if (m->getContent() == '3')
            delay = 0;
        else if (m->getContent() == '2')
            delay = 25;
        else if (m->getContent() == '1')
            delay = 50;
    }
    if (msg->getArrivalGate()->getIndex() == 0)
        sendDelayed(msg, simTime() + delay, "gate$o", 1);
    else
        sendDelayed(msg, simTime() + delay, "gate$o", 0);
}
