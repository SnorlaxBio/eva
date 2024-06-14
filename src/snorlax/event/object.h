/**
 * @file        snorlax/event/object.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 13, 2024
 */

#ifndef   __SNORLAX__EVENT_OBJECT__H__
#define   __SNORLAX__EVENT_OBJECT__H__

#include <snorlax.h>
#include <snorlax/event/object/queue.h>

struct event_object;
struct event_engine;
struct event_queue;
struct event_object_func;
struct event_handler_array_t;
struct event_object_queue;
struct event_object_queue_node;

typedef struct event_engine event_engine_t;
typedef struct event_object event_object_t;
typedef struct event_queue event_queue_t;
typedef struct event_object_func event_object_func_t;
typedef struct event_handler_array event_handler_array_t;
typedef struct event_object_queue event_object_queue_t;
typedef struct event_object_queue_node event_object_queue_node_t;

struct event_object {
    event_object_func_t * func;
    event_engine_t * engine;
    event_object_t * prev;
    event_object_t * next;

    event_object_queue_t queue;

    uint32_t status;
    event_handler_array_t * handler;
};

struct event_object_func {
    event_object_t * (*rem)(event_object_t *);
};



#endif // __SNORLAX__EVENT_OBJECT__H__
