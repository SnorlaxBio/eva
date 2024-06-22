/**
 * @file        snorlax/event/subscription/event/queue.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 21, 2024
 * @version     0.0.1
 */

#ifndef   __SNORLAX__EVENT_SUBSCRIPTION_EVENT_QUEUE__H__
#define   __SNORLAX__EVENT_SUBSCRIPTION_EVENT_QUEUE__H__

#include <snorlax/event.h>

struct event_subscription_event_queue {
    event_subscription_event_queue_func_t * func;
    sync_t * sync;
    uint64_t size;
    event_subscription_event_t * head;
    event_subscription_event_t * tail;
};

struct event_subscription_event_queue_func {
    event_subscription_event_queue_t * (*rem)(___notnull event_subscription_event_queue_t *);
    event_subscription_event_t * (*add)(___notnull event_subscription_event_queue_t *, ___notnull event_subscription_event_t *);
    event_subscription_event_t * (*del)(___notnull event_subscription_event_queue_t *, ___notnull event_subscription_event_t *);
    void (*clear)(___notnull event_subscription_event_queue_t *);
};

extern event_subscription_event_queue_t * event_subscription_event_queue_gen(void);

#define event_subscription_event_queue_rem(queue)           ((queue)->func->rem(queue))
#define event_subscription_event_queue_add(queue, event)    ((queue)->func->add(queue, event))
#define event_subscription_event_queue_del(queue, event)    ((queue)->func->del(queue, event))
#define event_subscription_event_queue_clear(queue)         ((queue)->func->clear(queue))

#endif // __SNORLAX__EVENT_SUBSCRIPTION_EVENT_QUEUE__H__
