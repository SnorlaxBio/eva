/**
 * @file        snorlax/event/queue.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 12, 2024
 */

#ifndef   __SNORLAX__EVENT_QUEUE__H__
#define   __SNORLAX__EVENT_QUEUE__H__

#include <snorlax.h>

struct event;
struct event_object;
struct event_param;
struct event_queue;
struct event_queue_func;

typedef struct event event_t;
typedef struct event_param event_param_t;
typedef struct event_object event_object_t;

typedef struct event_queue event_queue_t;
typedef struct event_queue_func event_queue_func_t;

typedef void (event_get_t)(event_t *);

struct event_queue {
    event_queue_func_t * func;
    uint64_t size;
    event_t * head;
    event_t * tail;
};

struct event_queue_func {
    event_queue_t * (*rem)(event_queue_t *, event_get_t);
    void (*clear)(event_queue_t *, event_get_t);
    uint64_t (*size)(event_queue_t *);
    event_t * (*push)(event_queue_t *, event_t *);
    event_t * (*pop)(event_queue_t *);
};

extern event_queue_t * event_queue_gen(void);

#define event_queue_rem(collection, get)        (collection->func->rem(collection, get))
#define event_queue_clear(collection, get)      (collection->func->clear(collection, get))
#define event_queue_size(collection)            (collection->func->size(collection))
#define event_queue_push(collection, e)         (collection->func->push(collection, e))
#define event_queue_pop(collection)             (collection->func->pop(collection))

#endif // __SNORLAX__EVENT_QUEUE__H__
