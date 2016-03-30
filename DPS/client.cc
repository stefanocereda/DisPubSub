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
#include "broker_init_m.h"

using namespace omnetpp;

class client: public cSimpleModule {
private:
    std::vector<int> ts_vec;
    std::vector<bool> my_subs;

    void sendMsg(int topic);
    void sendSub(int topic);

    void handleMessageMessage(Message_msg *m);
    void displayMessage(Message_msg *m);
    void handleMessageBroker(Broker_init_msg *msg);

protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

public:
    client() :
            ts_vec(NTOPIC), my_subs(NTOPIC, false) {
    }
};

// The module class needs to be registered with OMNeT++
Define_Module(client);

void client::initialize() {

    EV << this->getFullName() << " con id: " << this->getId() << "\n";
}

//Subscribe to the given topic
void client::sendSub(int topic) {
    //save our interest
    my_subs[topic] = true;

    //and send
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

    if (strcmp("broker", msg->getFullName()) == 0) {
        handleMessageBroker(dynamic_cast<Broker_init_msg*>(msg));
        }

}
void client::handleMessageBroker(Broker_init_msg *msg) {

        //Send a random subscription
        sendSub(intuniform(0, NTOPIC));

        //and send a random message
        sendMsg(intuniform(0, NTOPIC));
}

void client::handleMessageMessage(Message_msg *m){

    int topic = m->getTopic();

    //if I am not interested exit
    if (!my_subs[topic])
        return;

    //check the timestamp, if possible show the message, otherwise resend it as self message
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

