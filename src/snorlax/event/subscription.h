/**
 * @file        snorlax/event/subscription.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 15, 2024
 */

#ifndef   __SNORLAX__EVENT_SUBSCRIPTION__H__
#define   __SNORLAX__EVENT_SUBSCRIPTION__H__

#include <snorlax/eva.h>

struct event_subscription_func;

typedef struct event_subscription_func event_subscription_func_t;

struct event_subscription {
    event_subscription_func_t * func;
    event_generator_t * generator;
    event_subscription_t * prev;
    event_subscription_t * next;
};

struct event_subscription_func {
    event_subscription_t * (*rem)(event_subscription_t *);
};

#endif // __SNORLAX__EVENT_SUBSCRIPTION__H__
