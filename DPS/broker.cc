/*
 * broker.cc
 *
 *  Created on: 21/mar/2016
 *      Author: home
 */
#include <string.h>
#include <omnetpp.h>

class broker : public cSimpleModule
{
private:
    //the broker needs to know the nearby brokers, mappping is broker_id -> channel:
    //TODO: DOES THE BROKER REALLY NEED TO KNOW THIS?
    //typedef std::map<int, int> RoutingTable; //Serve per sapere dove sono i broker vicini (quale gate index)
    //RoutingTable broker_gate_table;

    //and on which channels it should send the various topics, mapping is topic -> list of interested channels
    typedef std::map<int, std::list<int>> SubscriptionTable;
    SubscriptionTable subscription_gate_table;

  protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

// The module class needs to be registered with OMNeT++
Define_Module(broker);


void broker::initialize(){
    //When the broker starts up it tells everybody
    //int n = gateSize("gate");

    //   for(int i = 0; i < n ; i++){
    //    Broker_init_msg *msg = new Broker_init_msg("broker");
    //    msg->setSrcId(this->getId());
      //  EV <<  this->getId() << "\n"; //the id is unique in all the program, and its duration.
    //    EV << "Sending message " << msg << " on gate[" << i << "]\n";
    //    send(msg, "gate$o", i);
    //}
}

void broker::handleMessage(SubscribeMessage *msg)
{
    /*if(strcmp("broker", msg->getFullName()) == 0){
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
    }*/

    int topic = msg->getTopic;
    int channel = msg->getArrivalGate()->getIndex();

    //check wheter it is a new topic
    SubscriptionTable::iterator it = subscription_gate_table.find(topic);
    if (it == subscription_gate_table.end()){
        //build a new list
        std::list<int> toAdd;
        toAdd.push_back(channel);
        //and add to the table
        subscription_gate_table.insert(it, toAdd);
    }
    else{
        //otherwise get the current list of interested channels
      //  it.reference
    }
}

