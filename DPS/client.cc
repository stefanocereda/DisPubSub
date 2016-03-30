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

    void sendMsg(int topic);
    void sendSub(int topic);

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
    msg->setTopic(topic);

    send(msg, "gate$o", 0);
}

//Send a message for the given topic
void client::sendMsg(int topic) {
    Message_msg *msg = new Message_msg("message");
    msg->setTopic(topic);
    msg->setTimestamp(++ts_vec[topic]);

    sendDelayed(msg, 10, "gate$o", 0); //TODO spararli fuori a caso
}

void client::sendLeave(){
    Leave_msg *leave = new Leave_msg("client_leave");
    leave->setSrcId(this->getId());

    send(leave , "gate$o" , 0);
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
        int random1 = intuniform(0, NTOPIC);
        sendSub(random1);

        EV << "The client with id: " << this->getId() << " sent a subscribe for the topic: " << random1 << "\n";

        //and send a random message
        int random2 = intuniform(0, NTOPIC);
        sendMsg(random2);
        EV << "The client with id: " << this->getId() << " sent a publish for the topic: " << random2 << "\n";
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

        EV << "The client with id: " << this->getId() << " and with timestamp: " << my_ts
                << " will display a message about topic: "<< topic
                << " with timestamp: " << ts << "\n";

        displayMessage(m);

        if(my_ts < ts){
            //Merge vector
            ts_vec[topic] = ts;
        }


        EV << "The client with id: " << this->getId() << " now has updated his timestamp to: " << ts_vec[topic];
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
}

