#include <omnetpp.h>
#include "message_m.h"
#include "parameters.h"

using namespace omnetpp;

class delayer: public cSimpleModule {
protected:
    virtual void handleMessage(cMessage *msg) override;

};

Define_Module(delayer);

void delayer::handleMessage(cMessage *msg) {
    const_simtime_t delay = intuniform(MIN_DELAY * 100, MAX_DELAY * 100) / 100;

    //for test
#if MODE == CONSISTENCY
    //20 22 24 da girare
    delay = 0.0;

    if (!strcmp(msg->getFullName(), "message")) {
        Message_msg *m = dynamic_cast<Message_msg*>(msg);

        if (m->getContent() == '3')
            delay = 0.0;
        else if (m->getContent() == '2')
            delay = 25.0;
        else if (m->getContent() == '1')
            delay = 50.0;
    }
#endif

    if (msg->getArrivalGate()->getIndex() == 0)
        sendDelayed(msg, simTime() + delay, "gate$o", 1);
    else
        sendDelayed(msg, simTime() + delay, "gate$o", 0);
}
