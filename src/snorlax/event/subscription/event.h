/**
 * @file        snorlax/event/subscription/event.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 17, 2024
 */

#ifndef   __SNORLAX__EVENT_SUBSCRIPTION_EVENT__H__
#define   __SNORLAX__EVENT_SUBSCRIPTION_EVENT__H__

#include <snorlax/eva.h>

struct event_subscription_event {
    event_subscription_event_func_t * func;

    sync_t * sync;

    event_subscription_event_queue_t * queue;
    event_subscription_event_t * prev;
    event_subscription_event_t * next;

    event_t * event;
};

struct event_subscription_event_func {
    event_subscription_event_t * (*rem)(event_subscription_event_t *);
};

extern event_subscription_event_t * event_subscription_event_gen(event_t * event);

#define event_subscription_event_rem(event)     (event->func->rem(event))

#endif // __SNORLAX__EVENT_SUBSCRIPTION_EVENT__H__
