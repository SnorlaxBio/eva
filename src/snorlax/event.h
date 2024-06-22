/**
 * @file        snorlax/event.h
 * @brief       
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 21, 2024
 * @version     0.0.1
 */

#ifndef   __SNORLAX__EVENT__H__
#define   __SNORLAX__EVENT__H__

#include <snorlax.h>

struct event;
struct event_func;
struct event_queue;
struct event_queue_func;
struct event_subscription;
struct event_subscription_func;
struct event_subscription_event;
struct event_subscription_event_func;
struct event_subscription_event_queue;
struct event_subscription_event_queue_func;
struct event_engine;
struct event_engine_func;
struct event_generator;
struct event_generator_func;
struct event_generator_set;
struct event_generator_set_func;
struct event_processor;
struct event_processor_func;
struct event_processor_pool;
struct event_processor_pool_func;

typedef struct event event_t;
typedef struct event_func event_func_t;
typedef struct event_queue event_queue_t;
typedef struct event_queue_func event_queue_func_t;
typedef struct event_subscription event_subscription_t;
typedef struct event_subscription_func event_subscription_func_t;
typedef struct event_subscription_event event_subscription_event_t;
typedef struct event_subscription_event_func event_subscription_event_func_t;
typedef struct event_subscription_event_queue event_subscription_event_queue_t;
typedef struct event_subscription_event_queue_func event_subscription_event_queue_func_t;
typedef struct event_engine event_engine_t;
typedef struct event_engine_func event_engine_func_t;
typedef struct event_generator event_generator_t;
typedef struct event_generator_func event_generator_func_t;
typedef struct event_generator_set event_generator_set_t;
typedef struct event_generator_set_func event_generator_set_func_t;
typedef struct event_processor event_processor_t;
typedef struct event_processor_func event_processor_func_t;
typedef struct event_processor_pool event_processor_pool_t;
typedef struct event_processor_pool_func event_processor_pool_func_t;

typedef void (*event_processor_cancel_t)(___notnull event_processor_t *);
typedef void (*event_engine_cancel_t)(___notnull const event_engine_t *);
typedef void (*event_subscription_handler_t)(___notnull event_subscription_t *, int32_t, uint64_t);

struct event {
    event_func_t * func;
    sync_t * sync;
    event_queue_t * queue;
    event_t * prev;
    event_t * next;
    event_subscription_event_t * node;
    event_subscription_t * subscription;
    uint32_t type;
};

struct event_func {
    event_t * (*rem)(___notnull event_t *);
    void (*on)(___notnull event_t *);
};

extern event_t * event_gen(___notnull event_subscription_t * subscription, uint32_t type, ___notnull event_subscription_event_t * node);

extern void event_func_on(___notnull event_t * event);

#define event_rem(event)        ((event)->func->rem(event))
#define event_on(event)         ((event)->func->on(event))

#endif // __SNORLAX__EVENT__H__
