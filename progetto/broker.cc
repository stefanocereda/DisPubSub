/*
 * broker.cc
 *
 *  Created on: 21/mar/2016
 *      Author: home
 */
#include <string.h>
#include <omnetpp.h>
#include <Subscribe_msg_m.h>
#include <Broker_init_msg_m.h>

using namespace omnetpp;

class broker : public cSimpleModule
{
private:

    typedef std::map<int, int> RoutingTable; //Serve per sapere dove sono i broker vicini (quale gate index)

    RoutingTable broker_gate_table;

    RoutingTable subscribe_gate_table;

    RoutingTable client_topic_table;

    //TODO
    typedef std::map<int, int[]> RoutingTableTopic;

    RoutingTableTopic topic_gates_table;

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
        Broker_init_msg *msg = new Broker_init_msg("broker");
        msg->setSrcId(this->getId());
      //  EV <<  this->getId() << "\n"; //the id is unique in all the program, and its duration.
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
       if(Broker_init_msg *msg_b = dynamic_cast<Broker_init_msg*>(msg)){

            int srcId = msg_b->getSrcId();
            int i = msg->getArrivalGate()->getIndex();

           RoutingTable::iterator it = broker_gate_table.find(srcId);

           //
           if (it == broker_gate_table.end()) {
                broker_gate_table[srcId] = i;
                EV << this->getFullName() << " Con id: "<< this->getId()
                   << " Ho aggiunto alla mia broker_gate_table: " << srcId << "\n";
           }

            cMessage *m = new cMessage("Router Ok!");

            send(m, "gate$o",i);
       }
    }

    if(strcmp("subscribe", msg->getFullName()) == 0){
        if(Subscribe_msg *m = dynamic_cast<Subscribe_msg*>(msg)){
            int srcId = m->getSrcId();
            int i = msg->getArrivalGate()->getIndex();

            RoutingTable::iterator it = subscribe_gate_table.find(srcId);

            if (it == subscribe_gate_table.end()) {
                subscribe_gate_table[srcId] = i;

                RoutingTable::iterator it = client_topic_table.find(srcId);

                if (it == client_topic_table.end()) {
                    client_topic_table[srcId] = m->getTopic();
                    EV << this->getFullName() << " Con id: "<< this->getId()
                           << " Ho aggiunto alla mia client_topic_table che: "
                           << srcId << " è interessato al topic numero: "
                           << m->getTopic() << "\n";
                }else{
                    EV << "Errore... Ammettiamo che un client abbia più subscription?\n";
                }


                EV << this->getFullName() << " Con id: "<< this->getId()
                   << " Ho aggiunto alla mia subscribe_gate_table che: "
                   << srcId << " è associato al gate con indice: "
                   << i << "\n";
            }

                EV << "Subscription" << m->getTopic() << "\n";
        }
    }

}


