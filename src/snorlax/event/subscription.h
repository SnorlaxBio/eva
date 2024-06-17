/**
 * @file        snorlax/event/subscription.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 17, 2024
 */

#ifndef   __SNORLAX__EVENT_SUBSCRIPTION__H__
#define   __SNORLAX__EVENT_SUBSCRIPTION__H__

#include <snorlax/eva.h>

struct event_subscription {
    event_subscription_func_t * func;
    sync_t * sync;

    event_generator_t * generator;
    event_subscription_t * prev;
    event_subscription_t * next;

    event_subscription_event_queue_t * queue;
};

struct event_subscription_func {
    event_subscription_t * (*rem)(event_subscription_t *);
    int32_t (*on)(event_subscription_t *, uint32_t, event_t *);
};

#endif // __SNORLAX__EVENT_SUBSCRIPTION__H__
