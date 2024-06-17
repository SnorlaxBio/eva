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
    event_queue_t * queue;
    event_t * prev;
    event_t * next;

    event_subscription_t * subscription;
    uint32_t type;
    event_handler_t on;
};

struct event_func {
    event_t * (*rem)(event_t *, event_callback_t);
    int32_t (*on)(event_t *);
};

extern event_t * event_gen(event_subscription_t * subscription, uint32_t type, event_handler_t on, event_t * event);

#define event_rem(event, func)      (event ? event->func->rem(event, func) : nil)
#define event_on(event)             (event ? event->func->on(event) : fail)

#endif // __SNORLAX__EVENT__H__