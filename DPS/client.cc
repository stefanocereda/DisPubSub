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
#include "leave_m.h"

using namespace omnetpp;

class client: public cSimpleModule {
private:
    std::vector<int> ts_vec;
    std::vector<bool> my_subs;

    void sendMsg(int topic, int delay);
    void sendSub(int topic, int delay);

    void handleMessageMessage(Message_msg *m);
    void displayMessage(Message_msg *m);
    void handleMessageBroker(Broker_init_msg *msg);

    void sendLeave();

protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

public:
    client() :
            ts_vec(NTOPIC, 0), my_subs(NTOPIC, false) {
    }
};

// The module class needs to be registered with OMNeT++
Define_Module(client);

void client::initialize() {
    EV << this->getFullName() << " con id: " << this->getId() << "\n";
}

//Subscribe to the given topic
void client::sendSub(int topic, int delay) {
    //save our interest
    my_subs[topic] = true;

    //and send
    Subscribe_msg *msg = new Subscribe_msg("subscribe");
    msg->setSrcId(this->getId());
    msg->setTopic(topic);

    sendDelayed(msg, delay, "gate$o", 0);
    EV << "The client with id: " << this->getId() << " sent a subscribe for the topic: " << topic << "\n";
}

//Send a message for the given topic
void client::sendMsg(int topic, int delay) {
    Message_msg *msg = new Message_msg("message");
    msg->setTopic(topic);
    msg->setTimestamp(++ts_vec[topic]);

    sendDelayed(msg, delay, "gate$o", 0); //TODO spararli fuori a caso
    EV << "The client with id: " << this->getId() << " sent a publish for the topic: " << topic << "\n";
}

void client::sendLeave(){
    Leave_msg *leave = new Leave_msg("client_leave");
    leave->setSrcId(this->getId());

    send(leave , "gate$o" , 0);
    EV << "The client with id: " << this->getId() << " left\n";
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
    //We are attached to a new broker, send some subscriptions and some messages with random delays
    for (int i = 0; i < N_SEND; i++)
        if (rand()%100 <= SUBS_RATIO*100)
            //send a sub
            sendSub(intuniform(0, NTOPIC-1), intuniform(2, MAX_DELAY));
        else
            //send a publish
            sendMsg(intuniform(0, NTOPIC-1), intuniform(1, MAX_DELAY));
}

void client::handleMessageMessage(Message_msg *m){

    int topic = m->getTopic();

    //if I am not interested exit
    if (!my_subs[topic])
        return;

    //check the timestamp, if possible show the message, otherwise resend it as self message
    int ts = m->getTimestamp();
    int my_ts = ts_vec[topic];

    if (!(my_ts + 1 < ts)){
        displayMessage(m);

        if(my_ts < ts){
            //Merge vector
            ts_vec[topic] = ts;
            EV << "The client with id: " << this->getId() << " now has updated his timestamp to: " << ts_vec[topic];
        }
    }
    else
    {
        scheduleAt(simTime()+RESEND_TIMEOUT, m->dup());
        EV << "The client with id: " << this->getId() << " and with timestamp: " << my_ts
                       << " will delay the shipment of a message about topic: "<< topic
                       << " with timestamp: " << ts << " at time: " << simTime() + RESEND_TIMEOUT << "\n";
    }
}

//TODO
void client::displayMessage(Message_msg *m){
    EV << "The client with id: " << this->getId() << " and with timestamp: " << ts_vec[m->getTopic()]
            << " will display a message about topic: "<< m->getTopic()
            << " with timestamp: " << m->getTimestamp() << "\n";
}

