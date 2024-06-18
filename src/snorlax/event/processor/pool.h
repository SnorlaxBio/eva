/**
 * @file        snorlax/event/processor/pool.h
 * @brief
 * @details 
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 18, 2024
 */

#ifndef   __SNORLAX__EVENT_PROCESSOR_POOL__H__
#define   __SNORLAX__EVENT_PROCESSOR_POOL__H__

#include <snorlax/eva.h>

struct event_processor_pool {
    event_processor_pool_func_t * func;
    sync_t * sync;

    uint32_t size;
    event_processor_t * head;
    event_processor_t * tail;

    event_engine_t * engine;
};

struct event_processor_pool_func {
    event_processor_pool_t * (*rem)(event_processor_pool_t *);
    int32_t (*on)(event_processor_pool_t *);
    int32_t (*off)(event_processor_pool_t *, event_processor_cancel_t);
};

extern event_processor_pool_t * event_processor_pool_gen(event_engine_t * engine, uint32_t size);

extern void event_processor_pool_add(event_processor_pool_t * pool, event_processor_t * processor);
extern void event_processor_pool_del(event_processor_pool_t * pool, event_processor_t * processor);

#define event_processor_pool_rem(pool)              ((pool) ? (pool)->func->rem(pool) : nil)
#define event_processor_pool_on(pool)               ((pool) ? (pool)->func->on(pool) : nil)
#define event_processor_pool_off(pool, cancel)      ((pool) ? (pool)->func->off(pool, cancel) : nil)

#endif // __SNORLAX__EVENT_PROCESSOR_POOL__H__
