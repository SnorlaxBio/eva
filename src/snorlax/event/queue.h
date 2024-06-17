/**
 * @file        snorlax/event/queue.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 17, 2024
 */

#ifndef   __SNORLAX__EVENT_QUEUE__H__
#define   __SNORLAX__EVENT_QUEUE__H__

#include <snorlax/eva.h>

#include <snorlax/queue.h>

struct event_queue {
    event_queue_func_t * func;

    sync_t * sync;

    uint64_t size;
    event_t * head;
    event_t * tail;
};

struct event_queue_func {
    event_queue_t * (*rem)(event_queue_t *);

    event_t * (*push)(event_queue_t *, event_t *);
    event_t * (*pop)(event_queue_t *);
    event_t * (*del)(event_queue_t *, event_t *);
};

extern event_queue_t * event_queue_gen(void);

#define event_queue_rem(queue)          (queue ? queue->func->rem(queue) : nil)
#define event_queue_push(queue, event)  (queue ? queue->func->push(queue, event) : nil)
#define event_queue_pop(queue)          (queue ? queue->func->pop(queue) : nil)
#define event_queue_del(queue, event)   (queue ? queue->func->del(queue, event) : nil)

#endif // __SNORLAX__EVENT_QUEUE__H__
