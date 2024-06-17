/**
 * @file        snorlax/event/processor/pool.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 17, 2024
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
    int32_t (*off)(event_processor_pool_t *);
};

extern event_processor_pool_t * event_processor_pool_gen(event_engine_t * engine, uint32_t n);

#endif // __SNORLAX__EVENT_PROCESSOR_POOL__H__
