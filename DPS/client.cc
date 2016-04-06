/*
 * client.cc
 *
 *  Created on: 21/mar/2016
 *      Author: home
 */
#include <string.h>
#include <omnetpp.h>
#include <vector>
#include <boost/format.hpp>
#include "subscribe_m.h"
#include "parameters.h"
#include "message_m.h"
#include "broker_init_m.h"
#include "leave_m.h"
#include "join_m.h"

#define LEAVE_PROBABILITY 0.1
#define LEAVE_DELAY 10
#define JOIN_PROBABILITY 0.1
#define JOIN_DELAY 10

#define ON 1
#define OFF 0

using namespace omnetpp;

class client: public cSimpleModule {
private:
    int working_modality;

    std::vector<int> ts_vec;
    std::vector<bool> my_subs;

    int numSubs = 0,
        numPubs = 0,
        wrongDispatch = 0,
        displayed = 0,
        rescheduled = 0,
        skipped = 0;

    void sendMsg(int topic, int delay);
    void sendSub(int topic, int delay);

    void handleMessageMessage(Message_msg *m);
    void displayMessage(Message_msg *m);
    void handleMessageBroker(Broker_init_msg *msg);
    void handleBrokerLeaveMessage(Leave_msg *m);
    void handleBrokerJoinMessage(Join_msg *m);

    void sendJoin();
    void sendLeave();
    void sendSubs();

    void bundleCycle();

protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

public:
    client() :
            ts_vec(NTOPIC, 0), my_subs(NTOPIC, false) {
    }
};

// The module class needs to be registered with OMNeT++
Define_Module(client);

void client::initialize() {
    EV << this->getFullName() << " has id: " << this->getId() << endl;
    working_modality = ON;
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
    EV << "The client with id: " << this->getId()
              << " sent a subscribe for the topic: " << topic << endl;
    numSubs++;
}

//Send a message for the given topic
void client::sendMsg(int topic, int delay) {
    Message_msg *msg = new Message_msg("message");
    msg->setTopic(topic);
    msg->setTimestamp(++ts_vec[topic]);

    sendDelayed(msg, delay, "gate$o", 0);
    EV << "The client with id: " << this->getId()
              << " sent a publish for the topic: " << topic << endl;
    numPubs++;
}

void client::sendJoin(){
    Join_msg *join = new Join_msg("client_join");
    join->setSrcId(this->getId());

    sendDelayed(join, JOIN_DELAY , "gate$o" , 0);
    EV << "\n"  << "The client with id: " << this->getId() << " has JOIN the connected broker again!";

    working_modality = ON;
}

void client::sendLeave() {
    Leave_msg *leave = new Leave_msg("client_leave");
    leave->setSrcId(this->getId());

    sendDelayed(leave, LEAVE_DELAY , "gate$o", 0);
    EV  << "\n"  << "The client with id: " << this->getId() << " has LEFT this network!";

    working_modality = OFF;

    bundleCycle();
}

void client::bundleCycle(){
    while(-1){
        if( rand() % 100 <= JOIN_PROBABILITY * 100){
            sendJoin();

            return;
        }
    }
}

void client::handleMessage(cMessage *msg) {
    if( working_modality == ON ){
        if (strcmp("message", msg->getFullName()) == 0) {
            handleMessageMessage(dynamic_cast<Message_msg*>(msg));
        }

        if (strcmp("broker", msg->getFullName()) == 0) {
            handleMessageBroker(dynamic_cast<Broker_init_msg*>(msg));
        }

        if (strcmp("broker_join", msg->getFullName()) == 0) {
            handleBrokerJoinMessage(dynamic_cast<Join_msg*>(msg));
        }

        if (strcmp("broker_leave", msg->getFullName()) == 0) {
            handleBrokerLeaveMessage(dynamic_cast<Leave_msg*>(msg));
        }

    }
    else{
        // It may happen when comunicating with a hub
        EV  << "\n" << "The client with id: " << this->getId() << " is OFF doesn't care of messages";
    }
}

void client::handleMessageBroker(Broker_init_msg *msg) {
    //We are attached to a new broker, send some subscriptions and some messages with random delays
    for (int i = 0; i < N_SEND; i++)
        if (rand() % 100 <= SUBS_RATIO * 100)
            //send a sub
            sendSub(intuniform(0, NTOPIC - 1), intuniform(0, 1));
        else
            //send a publish
            sendMsg(intuniform(0, NTOPIC - 1), intuniform(5, 30));

    if( rand() % 100 <= LEAVE_PROBABILITY * 100){
        sendLeave();
    }
}

void client::handleMessageMessage(Message_msg *m) {
    int topic = m->getTopic();

    //if I am not interested exit
    if (!my_subs[topic]) {
        wrongDispatch++;
        EV << "wrong";
        return;
    }

    //check the timestamp, if possible show the message, otherwise resend it as self message
    int ts = m->getTimestamp();
    int my_ts = ts_vec[topic];

    if (!(my_ts + 1 < ts)) {
        displayMessage(m);

        if (my_ts < ts) {
            //Merge vector
            ts_vec[topic] = ts;
            EV  << "\n" << "The client with id: " << this->getId()
                      << " now has updated his timestamp to: " << ts_vec[topic];
        }
    } else if (m->isSelfMessage()) {
        //it is a resent message and still we did not receive the missing messages, treat them as lost and go on
        EV << "The client with id: " << this->getId() << " lost "
                  << ts - ts_vec[topic] << " messages";
        displayMessage(m);
        skipped += ts - ts_vec[topic];
        ts_vec[topic] = ts;
    } else //try to wait
    {
        scheduleAt(simTime() + RESEND_TIMEOUT, m->dup());
        EV  << "\n"  << "The client with id: " << this->getId() << " and with timestamp: "
                  << my_ts
                  << " will delay the shipment of a message about topic: "
                  << topic << " with timestamp: " << ts << " at time: "
                  << simTime() + RESEND_TIMEOUT << endl;
        rescheduled++;
    }
}

void client::handleBrokerLeaveMessage(Leave_msg *m){
    // It only update the status after the receiving of a leave by a broker

    //updateStatusLeave(m);

    //I ack a broker leave
    Message_msg *msg = new Message_msg("ack_leave");
    int channel = m->getArrivalGate()->getIndex();
    send(msg, "gate$o", channel);

}

void client::sendSubs() {

    for (int i = 0; i < my_subs.size(); i++){
        if(my_subs[i] == true){
            Subscribe_msg *msg = new Subscribe_msg("subscribe");
                msg->setSrcId(this->getId());
                msg->setTopic(i);
        }
    }
}

void client::handleBrokerJoinMessage(Join_msg *m){

    Message_msg *msg = new Message_msg("ack_join");
    int channel = m->getArrivalGate()->getIndex();
    send(msg, "gate$o", channel);

    sendSubs();
}

void client::displayMessage(Message_msg *m) {
    EV  << "\n" << "The client with id: " << this->getId() << " and with timestamp: "
              << ts_vec[m->getTopic()]
              << " will display a message about topic: " << m->getTopic()
              << " with timestamp: " << m->getTimestamp() << endl;
    displayed++;
}

void client::finish()
{
    EV <<  boost::str(boost::format("c,%d,%d,%d,%d,%d,%d,%d")
            % this->getId() % numSubs % numPubs % wrongDispatch % displayed % rescheduled % skipped) << endl;
}

