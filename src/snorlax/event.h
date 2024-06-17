/**
 * @file        snorlax/event.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 17, 2024
 */

#ifndef   __SNORLAX__EVENT__H__
#define   __SNORLAX__EVENT__H__

#include <snorlax/eva.h>

struct event {
    event_func_t * func;
    sync_t * sync;

    event_queue_t * queue;
    event_t * prev;
    event_t * next;

    event_subscription_event_t * node;

    event_subscription_t * subscription;
    uint32_t type;
};

struct event_func {
    event_t * (*rem)(event_t *);

    int32_t (*on)(event_t *);
};

extern event_t * event_gen(event_t * event, event_subscription_t * subscription, uint32_t type);

#define event_rem(event)        (event ? event->func->rem(event) : nil)
#define event_on(event)         (event ? event->func->on(event) : fail)

#endif // __SNORLAX__EVENT__H__
