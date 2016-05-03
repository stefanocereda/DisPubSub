/*
 * parameters.h
 *
 *  Created on: Mar 28, 2016
 *      Author: stefano
 */

#include <omnetpp.h>

//CLIENT_LEAVE per le client leave con il tree
#define CLIENT_LEAVE 0
//RANDOM per le statistiche
#define RANDOM 1
//BROKER_LEAVE per le broker leave con la star
#define BROKER_LEAVE 2
//CONSISTENCY per vedere la consistency con Network
#define CONSISTENCY 3



#define MODE CONSISTENCY

#if (MODE == CLIENT_LEAVE)||(MODE == BROKER_LEAVE)||(MODE == CONSISTENCY)
    const int NTOPIC = 5; //how many topics
    const int N_SEND = 0; //how many messages a client should send to a new broker
    const float SUBS_RATIO = 0.1; //the percentage of messages that should be a subscription

    const const_simtime_t MIN_SUB_DELAY = 0.5; //when to send the subscriptions
    const const_simtime_t MAX_SUB_DELAY = 10.0;
    const const_simtime_t MIN_PUB_DELAY = 5.0; //when to send the first publish
    const const_simtime_t MAX_PUB_DELAY = 15.0;

    const const_simtime_t RESEND_TIMEOUT = 100.0; //time to re send a self message waiting for the correct timestamp

    const float REPLY_PROB = 0.0; //the probability that a client answer to a message
    const const_simtime_t MIN_REPLY_DELAY = 1.0; //and the time to answer
    const const_simtime_t MAX_REPLY_DELAY = 5.0;

    const float CLIENT_LEAVE_PROBABILITY = 0.0; //prob for client to leave
    const const_simtime_t MIN_LEAVE_DELAY = 3.0; //when to leave
    const const_simtime_t MAX_LEAVE_DELAY = 6.0;
    const const_simtime_t MIN_REJOIN_DELAY = 3.0; //and the additional time to come back
    const const_simtime_t MAX_REJOIN_DELAY = 6.0;

    const float BROKER_LEAVE_PROBABILITY = 0.0;
    const const_simtime_t MIN_HUB_TIME = 6.0;
    const const_simtime_t MAX_HUB_TIME = 6.0;
    const const_simtime_t MIN_BLEAVE_DELAY = 15.0;
    const const_simtime_t MAX_BLEAVE_DELAY = 15.0;

#elif MODE == RANDOM
    const int NTOPIC = 5; //how many topics
    const int N_SEND = 10; //how many messages a client should send to a new broker
    const float SUBS_RATIO = 0.2; //the percentage of messages that should be a subscription

    const const_simtime_t MIN_SUB_DELAY = 0.5; //when to send the subscriptions
    const const_simtime_t MAX_SUB_DELAY = 10.0;
    const const_simtime_t MIN_PUB_DELAY = 5.0; //when to send the first publish
    const const_simtime_t MAX_PUB_DELAY = 15.0;

    const const_simtime_t RESEND_TIMEOUT = 10.0; //time to re send a self message waiting for the correct timestamp

    const float REPLY_PROB = 0.95; //the probability that a client answer to a message
    const const_simtime_t MIN_REPLY_DELAY = 1.0; //and the time to answer
    const const_simtime_t MAX_REPLY_DELAY = 5.0;

    const float CLIENT_LEAVE_PROBABILITY = 0.2; //prob for client to leave
    const const_simtime_t MIN_LEAVE_DELAY = 3.0; //when to leave
    const const_simtime_t MAX_LEAVE_DELAY = 6.0;
    const const_simtime_t MIN_REJOIN_DELAY = 3.0; //and the additional time to come back
    const const_simtime_t MAX_REJOIN_DELAY = 6.0;

    const float BROKER_LEAVE_PROBABILITY = 0.2;
    const const_simtime_t MIN_HUB_TIME = 3.0;
    const const_simtime_t MAX_HUB_TIME = 6.0;
    const const_simtime_t MIN_BLEAVE_DELAY = 3.0;
    const const_simtime_t MAX_BLEAVE_DELAY = 6.0;

#endif
