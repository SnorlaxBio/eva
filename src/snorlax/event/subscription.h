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
    void (*on)(event_subscription_t *, uint32_t, uint64_t);
};

#define event_subscription_rem(subscription)            ((subscription)->func->rem(subscription))
#define event_subscription_on(subscription, type, ret)  ((subscription)->func->on(subscription, type, ret))

#endif // __SNORLAX__EVENT_SUBSCRIPTION__H__
