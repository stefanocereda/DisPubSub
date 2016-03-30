/*
 * parameters.h
 *
 *  Created on: Mar 28, 2016
 *      Author: stefano
 */

#include <omnetpp.h>

const int NTOPIC = 5; //how many topics
const const_simtime_t RESEND_TIMEOUT = 10.0; //time to re send a self message waiting for the correct timestamp
const int N_SEND = 10; //how many messages a client should send to a new broker
const float SUBS_RATIO = 0.1; //the percentage of messages that should be a subscription
const int MAX_DELAY = 10; //the maximum delay to send a sub/pub msg

