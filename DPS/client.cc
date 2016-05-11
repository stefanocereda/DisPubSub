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
#include "join_m.h"
#include "ack_join_m.h"

#define ON 1
#define OFF 0

using namespace omnetpp;

class client: public cSimpleModule {
private:
    int working_modality;

    typedef std::map<int, int> ts_map; //client->ts
    typedef std::vector<ts_map> ts_map_vect; //topic -> client -> ts
    ts_map_vect ts_struct;
    std::vector<bool> my_subs;

    int numSubs = 0, numPubs = 0, wrongDispatch = 0, displayed = 0,
            rescheduled = 0, skipped = 0;

    void sendMsg(int topic, const_simtime_t delay);
    void sendMsg(int topic, const_simtime_t delay, char content);
    void sendSub(int topic, const_simtime_t delay);
    void sendLeave(const_simtime_t delay, const_simtime_t join_drift);
    void reSendSubs(const_simtime_t delay);
    void sendSubs();
    void sendSubs(const_simtime_t delay);

    void handleMessageMessage(Message_msg *m);
    void displayMessage(Message_msg *m);
    void handleMessageBroker(Broker_init_msg *msg);
    void handleBrokerLeaveMessage(Leave_msg *m);
    void handleBrokerJoinMessage(Join_msg *m);

    void bundleCycle();
    bool isCasualConsistent(ts_map our_map, ts_map other_map, int sender);
    void mergeAllTs(ts_map other_map, int topic);

    void print(ts_map_vect s);

protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

public:
    client() :
            my_subs(NTOPIC, false) {
    }
};

// The module class needs to be registered with OMNeT++
Define_Module(client);

void client::initialize() {
    working_modality = ON;
    for (int t = 0; t < NTOPIC; t++) {
        ts_map foo = ts_map();
        foo[this->getId()] = 0;
        ts_struct.push_back(foo);
    }

    /*just for testing*/
#if MODE == CLIENT_LEAVE
    if (this->getId() == 18) {
        sendSub(1, 5.0);
        sendLeave(10.0, 5.0);
    }
    else {
        sendMsg(1, 6.0);
        sendMsg(1, 12.0);
        sendMsg(0, 13.0);
        sendMsg(1, 20.0);
        sendMsg(0, 22.0);
    }

#elif MODE == BROKER_LEAVE
    if (this->getId() == 17) {
        sendSub(1, 5.0);
    } else if (this->getId() == 18) {
        sendMsg(1, 10.0);
        sendMsg(1, 20.0);
        sendMsg(1, 30.0);
    } else if (this->getId() == 15) {
        sendMsg(1, 20.5);
    }

#elif MODE == CONSISTENCY
    if (this->getId() == 3) {
        sendMsg(1, 5.0, '1'); //msg 1 [1 - -]
    }

    else if (this->getId() == 4) {
        sendSub(1, 1.0);

        sendMsg(1, 5.0, '2'); //concurrent 2 [- 1 -]
    }

    else if (this->getId() == 5) {
        sendSub(1, 1.0);
        //we must display 1/2, 3
        //we must receive 3,2,1

        //receive 3 -> [1 2 -] => [- 1 0]
    }

#endif
}

//Subscribe to the given topic
void client::sendSub(int topic, const_simtime_t delay) {
    //save our interest
    my_subs[topic] = true;

    //and send
    Subscribe_msg *msg = new Subscribe_msg("subscribe");
    msg->setSrcId(this->getId());
    msg->setTopic(topic);

    sendDelayed(msg, delay, "gate$o", 0);
    numSubs++;
}

//Send a message for the given topic
void client::sendMsg(int topic, const_simtime_t delay) {
    sendMsg(topic, delay, 'a');
}

void client::sendMsg(int topic, const_simtime_t delay, char content) {
    Message_msg *msg = new Message_msg("message");
    msg->setTopic(topic);
    msg->setSenderId(this->getId());
    msg->setContent(content);

    ts_struct[topic][this->getId()]++;
    msg->setTs_struct(ts_struct);

    sendDelayed(msg, delay, "gate$o", 0);
    numPubs++;

    EV << this->getFullName() << " sent a message about topic " << topic
              << " with content " << content << endl;
}

void client::sendLeave(const_simtime_t delay, const_simtime_t join_drift) {
    Leave_msg *leave = new Leave_msg("client_leave");
    leave->setSrcId(this->getId());

    sendDelayed(leave, delay, "gate$o", 0);

    reSendSubs(delay + join_drift);
}

void client::reSendSubs(const_simtime_t delay) {
    // The join consists only in resend the subscription for the topic to which I'm interested in
    sendSubs(delay);
}

void client::handleMessage(cMessage *msg) {
    if (working_modality == ON) {
        if (strcmp("message", msg->getFullName()) == 0) {
            handleMessageMessage(dynamic_cast<Message_msg*>(msg));
        }

        else if (strcmp("broker", msg->getFullName()) == 0) {
            handleMessageBroker(dynamic_cast<Broker_init_msg*>(msg));
        }

        else if (strcmp("broker_join", msg->getFullName()) == 0) {
            handleBrokerJoinMessage(dynamic_cast<Join_msg*>(msg));
        }

        else
            EV << "client received an unknown message" << endl;

    } else {
        EV
                  << "client received a message while it was dead, neighbor broker should be dead"
                  << endl;
    }
}

void client::handleMessageBroker(Broker_init_msg *msg) {
    //We are attached to a new broker, send some subscriptions with random delays
    for (int i = 0; i < N_SEND; i++)
        if (rand() % 100 < SUBS_RATIO * 100) {
            //send a sub
            sendSub(intuniform(0, NTOPIC - 1),
                    (const_simtime_t) (intuniform(MIN_SUB_DELAY * 100,
                            MAX_SUB_DELAY * 100)) / 100);
        } else {
            //send a publish
            sendMsg(intuniform(0, NTOPIC - 1), MAX_SUB_DELAY+1.0);
        }

    //and maybe a leave
    if (rand() % 100 < CLIENT_LEAVE_PROBABILITY * 100) {
        sendLeave(
                (const_simtime_t) (intuniform(MIN_LEAVE_DELAY * 100,
                        MAX_LEAVE_DELAY * 100)) / 100,
                (const_simtime_t) (intuniform(MIN_REJOIN_DELAY * 100,
                        MAX_REJOIN_DELAY * 100)) / 100);
    }
}

void client::print(ts_map_vect s) {
    for (int t = 0; t < NTOPIC; t++) {
        for (ts_map::iterator it = s[t].begin(); it != s[t].end(); ++it) {
            EV << it->first << "->" << s[t][it->first] << "\t";
        }
        EV << "topic " << t << endl;
    }
}

void client::handleMessageMessage(Message_msg *m) {
    int topic = m->getTopic();
    int sender = m->getSenderId();

    //if I am not interested exit
    if (!my_subs[topic]) {
        wrongDispatch++;
        EV << this->getFullName() << " received a mesagge about topic " << topic
                  << " but is not interested" << endl;
        return;
    }

    //check the timestamp, if possible show the message, otherwise resend it as self message
    ts_map my_ts = ts_struct[topic];
    ts_map msg_ts = m->getTs_struct()[topic];

    /*EV << "msg" << endl;
     print(m->getTs_struct());

     EV << "old" << endl;
     print(ts_struct);*/

    //compare the two vectors
    if (isCasualConsistent(ts_struct[topic], msg_ts, sender)) {
        ts_struct[topic][sender] = msg_ts[sender]; //merge the only relevant
        displayMessage(m);

    } else if (!m->isSelfMessage()) {
        //try to wait for the missing messages
        EV << this->getFullName() << " rescheduled a message" << endl;
        rescheduled++;
        scheduleAt(simTime() + RESEND_TIMEOUT, m->dup());

    } else {
        //it is a resent message and still we did not receive the missing messages, treat them as lost and go on
        EV << this->getFullName() << " gave up waiting" << endl;
        //maybe the problem was not the sender, so we should merge everything
        mergeAllTs(msg_ts, topic);
        displayMessage(m);
        skipped++;
    }

    /*EV << "new" << endl;
     print(ts_struct);*/
}

void client::sendSubs() {
    sendSubs(0);
}

void client::sendSubs(const_simtime_t delay) {
    for (unsigned int i = 0; i < my_subs.size(); i++) {
        if (my_subs[i] == true) {
            Subscribe_msg *msg = new Subscribe_msg("subscribe");
            msg->setSrcId(this->getId());
            msg->setTopic(i);
            sendSub(i, delay);
        }
    }
}

void client::handleBrokerJoinMessage(Join_msg *m) {

    Ack_join_msg *msg = new Ack_join_msg("ack_join");
    int channel = m->getArrivalGate()->getIndex();
    send(msg, "gate$o", channel);

    sendSubs();
}

void client::displayMessage(Message_msg *m) {
    EV << this->getFullName() << " shows a message with content "
              << m->getContent() << endl;
    displayed++;

    //Moreover, we have a probability to reply
    double X=((double)rand()/(double)RAND_MAX);
    if (X <= REPLY_PROB){
        sendMsg(m->getTopic(),
                (const_simtime_t) (intuniform(MIN_REPLY_DELAY * 100,
                        MAX_REPLY_DELAY * 100)) / 100);
        EV << "reply" << endl;
    }

    //testing
#if MODE==CONSISTENCY
    if (this->getId() == 4)
    sendMsg(1, 0.0, '3'); //answer to msg 1 [1 2 -]
#endif

}

void client::finish() {
    EV << "c," << this->getId() << "," << numSubs << "," << numPubs << ","
              << wrongDispatch << "," << displayed << "," << rescheduled << ","
              << skipped << endl;
}

bool client::isCasualConsistent(ts_map our_map, ts_map other_map, int sender) {
    //the received vector is casual consistent with out own if
    //ts(r)[j] = V_k[j] + 1
    //ts(r)[i] <= V_k[i] for all i != j

    //if we know this client we check consistency
    if ((our_map.find(sender) != our_map.end())
            && (other_map[sender] > our_map[sender] + 1)) {
        EV << this->getFullName() << " received a message from client with id "
                  << sender << " but waits to show because the sender ts is"
                  << other_map[sender] << " and our is " << our_map[sender]
                  << endl;
        return false;
    }

    //now we check for the other clients.
    //if we know about them and the other client does not, it is consistent (the message cannot be a reply)
    //if the client knows about someone and we do not, they are not consistent and we should wait
    for (ts_map::iterator it = other_map.begin(); it != other_map.end(); ++it) {
        if (it->first == sender)
            continue;

        if (our_map.find(it->first) == our_map.end()) { //we do not know this client
            EV << this->getFullName()
                      << " received a message from client with id " << sender
                      << " but waits to show because the sender ts contains client with id"
                      << it->first << " and we do not." << endl;
            return false;
        }

        if (it->second > our_map[it->first] + 1) {
            EV << this->getFullName()
                      << " received a message from client with id" << sender
                      << " but waits to show because the sender ts contains client with id "
                      << it->first << " with ts " << it->second
                      << " but our is " << our_map[it->first] << endl;
            return false;
        }
    }

    return true;
}

void client::mergeAllTs(ts_map other_map, int topic) {
    //scan the other_map, for every found client merge in our map.
    for (ts_map::iterator it = other_map.begin(); it != other_map.end(); ++it) {

        if (ts_struct[topic].find(it->first) == ts_struct[topic].end()) {
            //this is an unknown client
            ts_struct[topic][it->first] = it->second;
        }

        else {
            //take the max
            if (ts_struct[topic][it->first] < it->second)
                ts_struct[topic][it->first] = it->second;
        }
    }
}
