/*
 * parameters.h
 *
 *  Created on: Mar 28, 2016
 *      Author: stefano
 */

#include <omnetpp.h>

const int NTOPIC = 5; //how many topics
const int N_SEND = 15; //how many messages a client should send to a new broker
const float SUBS_RATIO = 0.05; //the percentage of messages that should be a subscription

const const_simtime_t MIN_SUB_DELAY = 0.5;//when to send the subscriptions
const const_simtime_t MAX_SUB_DELAY = 10.0;
const const_simtime_t MIN_PUB_DELAY = 5.0;//when to send the first publish
const const_simtime_t MAX_PUB_DELAY = 10.0;

const const_simtime_t RESEND_TIMEOUT = 10.0; //time to re send a self message waiting for the correct timestamp

const float REPLY_PROB = 0.5; //the probability that a client answer to a message
const const_simtime_t MIN_REPLY_DELAY = 1.0;//and the time to answer
const const_simtime_t MAX_REPLY_DELAY = 5.0;

const float CLIENT_LEAVE_PROBABILITY = 0.3; //prob for client to leave
const const_simtime_t  CLIENT_LEAVE_DELAY = 7.5; //when to leave
const const_simtime_t  CLIENT_JOIN_DRIFT = 2.5; //time to come back

const float BROKER_LEAVE_PROBABILITY = 0.3;
const const_simtime_t MIN_HUB_TIME = 3.0;
const const_simtime_t MAX_HUB_TIME = 6.0;
const const_simtime_t MIN_BLEAVE_DELAY = 3.0;
const const_simtime_t MAX_BLEAVE_DELAY = 6.0;
