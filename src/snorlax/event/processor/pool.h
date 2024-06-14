/**
 * @file        snorlax/event/processor/pool.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 13, 2024
 */

#ifndef   __SNORLAX__EVENT_PROCESSOR_POOL__H__
#define   __SNORLAX__EVENT_PROCESSOR_POOL__H__

#include <snorlax.h>

struct event_processor;
struct event_processor_pool;
struct event_processor_pool_func;

typedef struct event_processor event_processor_t;
typedef struct event_processor_pool event_processor_pool_t;
typedef struct event_processor_pool_func event_processor_pool_func_t;

struct event_processor_pool {
    event_processor_pool_func_t * func;
    uint64_t size;
    event_processor_t * head;
    event_processor_t * tail;
};

struct event_processor_pool_func {
    event_processor_pool_t * (*rem)(event_processor_pool_t *);

    uint64_t (*size)(event_processor_pool_t *);
};

extern event_processor_pool_t * event_processor_pool_gen(void);

#define event_processor_pool_size(pool)         (pool->func->size(pool))

#endif // __SNORLAX__EVENT_PROCESSOR_POOL__H__
