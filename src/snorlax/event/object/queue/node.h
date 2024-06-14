/**
 * @file        snorlax/event/object/queue/node.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 13, 2024
 */

#ifndef   __SNORLAX__EVENT_OBJECT_QUEUE_NODE__H__
#define   __SNORLAX__EVENT_OBJECT_QUEUE_NODE__H__

struct event;
struct event_object;
struct event_object_queue_node;

typedef struct event event_t;
typedef struct event_object event_object_t;
typedef struct event_object_queue_node event_object_queue_node_t;

struct event_object_queue_node {
    event_object_t * o;
    event_t * prev;
    event_t * next;
};

#endif // __SNORLAX__EVENT_OBJECT_QUEUE_NODE__H__
