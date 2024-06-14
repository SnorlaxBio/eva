/**
 * @file        snorlax/event/object/queue.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 13, 2024
 */

#ifndef   __SNORLAX__EVENT_OBJECT_QUEUE__H__
#define   __SNORLAX__EVENT_OBJECT_QUEUE__H__

#include <snorlax.h>

struct event;
struct event_object;
struct event_object_queue;
struct event_object_queue_node;

typedef struct event event_t;
typedef struct event_object event_object_t;
typedef struct event_object_queue event_object_queue_t;
typedef struct event_object_queue_node event_object_queue_node_t;

typedef void (*event_get_t)(event_t *);

struct event_object_queue {
    uint64_t size;
    event_t * head;
    event_t * tail;
};

extern event_t * event_object_queue_add(event_object_t * o, event_t * e);
extern event_t * event_object_queue_del(event_object_t * o, event_t * e);

#endif // __SNORLAX__EVENT_OBJECT_QUEUE__H__
