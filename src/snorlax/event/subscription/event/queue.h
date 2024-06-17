/**
 * @file        snorlax/event/subscription/queue.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 17, 2024
 */

#ifndef   __SNORLAX__EVENT_SUBSCRIPTION_EVENT_QUEUE__H__
#define   __SNORLAX__EVENT_SUBSCRIPTION_EVENT_QUEUE__H__

#include <snorlax/eva.h>

#include <snorlax/queue.h>

struct event_subscription_event_queue {
    event_subscription_event_queue_func_t * func;

    sync_t * sync;

    uint64_t size;
    event_subscription_event_t * head;
    event_subscription_event_t * tail;
};

struct event_subscription_event_queue_func {
    event_subscription_event_queue_t * (*rem)(event_subscription_event_queue_t *);
    event_subscription_event_t * (*push)(event_subscription_event_queue_t *, event_subscription_event_t *);
    event_subscription_event_t * (*pop)(event_subscription_event_queue_t *);
    event_subscription_event_t * (*del)(event_subscription_event_queue_t *, event_subscription_event_t *);
};

extern event_subscription_event_queue_t * event_subscription_event_queue_gen(void);

#define event_subscription_event_queue_rem(queue)               (queue ? queue->func->rem(queue) : nil)
#define event_subscription_event_queue_push(queue, event)       (queue ? queue->func->push(queue, event) : nil)
#define event_subscription_event_queue_pop(queue)               (queue ? queue->func->pop(queue) : nil)
#define event_subscription_event_queue_del(queue, event)        (queue ? queue->func->del(queue, event) : nil)


#endif // __SNORLAX__EVENT_SUBSCRIPTION_EVENT_QUEUE__H__
