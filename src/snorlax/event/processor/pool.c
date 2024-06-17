/**
 * @file        snorlax/event/processor/pool.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 17, 2024
 */

#include "pool.h"

#include "../processor.h"

static event_processor_pool_t * event_processor_pool_func_rem(event_processor_pool_t * pool);
static int32_t event_processor_pool_func_on(event_processor_pool_t * pool);
static int32_t event_processor_pool_func_off(event_processor_pool_t * pool);

static event_processor_pool_func_t func = {
    event_processor_pool_func_rem,
    event_processor_pool_func_on,
    event_processor_pool_func_off
};

extern event_processor_pool_t * event_processor_pool_gen(event_engine_t * engine, uint32_t n) {
    event_processor_pool_t * pool = (event_processor_pool_t *) calloc(1, sizeof(event_processor_pool_t));

    pool->func = &func;
    pool->engine = engine;
    pool->sync = sync_gen();
    
    for(uint32_t i = 0; i < n; i++) {
        event_processor_gen(pool);
    }

    return pool;
}

static event_processor_pool_t * event_processor_pool_func_rem(event_processor_pool_t * pool) {
    object_lock(pool);
    while(pool->head) {
        event_processor_rem(pool->head, nil);
    }
    object_unlock(pool);

    pool->sync = sync_rem(pool->sync);
    
    free(pool);
    
    return nil;
}

static int32_t event_processor_pool_func_on(event_processor_pool_t * pool) {
    object_lock(pool);
    event_processor_t * processor = pool->head;
    while(processor) {
        event_processor_on(processor);
        processor = processor->next;
    }
    object_unlock(pool);
    return success;
}

static int32_t event_processor_pool_func_off(event_processor_pool_t * pool) {
    object_lock(pool);
    event_processor_t * processor = pool->head;
    while(processor) {
        event_processor_off(processor, event_processor_func_cancel);
        processor = processor->next;
    }
    object_unlock(pool);
    return success;
}
