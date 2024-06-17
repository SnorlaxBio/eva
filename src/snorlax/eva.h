/**
 * @file        snorlax/eva.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 17, 2024
 */

#ifndef   __SNORLAX__EVA__H__
#define   __SNORLAX__EVA__H__

#include <snorlax.h>

struct event;
struct event_func;
struct event_queue;
struct event_queue_func;
struct event_queue_node;
struct event_queue_node_func;
struct event_subscription;
struct event_subscription_func;

struct event_subscription_event;
struct event_subscription_event_func;
struct event_subscription_event_queue;
struct event_subscription_event_queue_func;


typedef struct event event_t;
typedef struct event_func event_func_t;
typedef struct event_queue event_queue_t;
typedef struct event_queue_func event_queue_func_t;
typedef struct event_queue_node event_queue_node_t;
typedef struct event_queue_node_func event_queue_node_func_t;
typedef struct event_subscription event_subscription_t;
typedef struct event_subscription_func event_subscription_func_t;

typedef struct event_subscription_event event_subscription_event_t;
typedef struct event_subscription_event_func event_subscription_event_func_t;
typedef struct event_subscription_event_queue event_subscription_event_queue_t;
typedef struct event_subscription_event_queue_func event_subscription_event_queue_func_t;


#endif // __SNORLAX__EVA__H__
