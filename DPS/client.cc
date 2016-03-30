/*
 * client.cc
 *
 *  Created on: 21/mar/2016
 *      Author: home
 */
#include <string.h>
#include <omnetpp.h>
#include <vector>
#include "subscribe_m.h"
#include "parameters.h"
#include "message_m.h"

using namespace omnetpp;

class client: public cSimpleModule {
private:
    std::vector<int> ts_vec;

    void sendMsg(int topic);
    void sendSub(int topic);

    void handleMessageMessage(Message_msg *m);
    void displayMessage(Message_msg *m);

protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

public:
    client() :
            ts_vec(NTOPIC) {
    }
};

// The module class needs to be registered with OMNeT++
Define_Module(client);

void client::initialize() {
    EV << this->getFullName() << " con id: " << this->getId() << "\n";

    //Send a random subscription
    sendSub(intuniform(0, NTOPIC));

    //and send a random message
    sendMsg(intuniform(0, NTOPIC));
}

//Subscribe to the given topic
void client::sendSub(int topic) {
    Subscribe_msg *msg = new Subscribe_msg("subscribe");
    msg->setSrcId(this->getId());
    msg->setTopic(intuniform(0, NTOPIC));

    send(msg, "gate$o", 0);
}

//Send a message for the given topic
void client::sendMsg(int topic) {
    Message_msg *msg = new Message_msg("message");
    msg->setTopic(topic);
    msg->setTimestamp(++ts_vec[topic]);

    sendDelayed(msg, 10, "gate$o", 0); //TODO spararli fuori a caso
}

void client::handleMessage(cMessage *msg) {
    if (strcmp("message", msg->getFullName()) == 0) {
        handleMessageMessage(dynamic_cast<Message_msg*>(msg));
    }
}

//TODO CHECK + KEEP MSG
void client::handleMessageMessage(Message_msg *m){

    int topic = m->getTopic();
    int ts = m->getTimestamp();

    int my_ts = ts_vec[topic];

    if (!(my_ts+1 < ts)){
        displayMessage(m);
        ts_vec[topic]++;
    }
    else
    {
        scheduleAt(simTime()+RESEND_TIMEOUT, m->dup());
    }
}

//TODO
void client::displayMessage(Message_msg *m){
}

