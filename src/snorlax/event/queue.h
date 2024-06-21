/**
 * @file        snorlax/event/queue.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 21, 2024
 * @version     0.0.1
 */

#ifndef   __SNORLAX__EVENT_QUEUE__H__
#define   __SNORLAX__EVENT_QUEUE__H__

#include <snorlax/event.h>

struct event_queue {
    event_queue_func_t * func;
    sync_t * sync;
    uint64_t size;
    event_t * head;
    event_t * tail;
};

struct event_queue_func {
    ___sync event_queue_t * (*rem)(___notnull event_queue_t *);
    ___sync void (*push)(___notnull event_queue_t *, ___notnull event_t *);
    event_t * (*pop)(___notnull event_queue_t *);
    ___sync void (*del)(___notnull event_queue_t *, ___notnull event_t *);
};

extern event_queue_t * event_queue_gen(sync_t * sync);

#define event_queue_rem(queue)              ((queue)->func->rem(queue))
#define event_queue_push(queue, event)      ((queue)->func->push(queue, event))
#define event_queue_pop(queue)              ((queue)->func->pop(queue))
#define event_queue_del(queue, event)       ((queue)->func->del(queue, event))
#define event_queue_size(queue)             ((queue)->size)

#endif // __SNORLAX__EVENT_QUEUE__H__
