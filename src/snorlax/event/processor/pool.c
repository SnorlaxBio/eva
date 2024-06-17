/**
 * @file        snorlax/event/processor/pool.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 18, 2024
 */

#include <stdlib.h>

#include "pool.h"

#include "../engine.h"
#include "../processor.h"

static event_processor_pool_t * event_processor_pool_func_rem(event_processor_pool_t * pool);

static event_processor_pool_func_t func = {
    event_processor_pool_func_rem
};

extern event_processor_pool_t * event_processor_pool_gen(event_engine_t * engine, uint32_t size) {
    event_processor_pool_t * pool = (event_processor_pool_t *) calloc(1, sizeof(event_processor_pool_t));

    pool->func = &func;

    return pool;
}

static event_processor_pool_t * event_processor_pool_func_rem(event_processor_pool_t * pool) {
    object_lock(pool);
    event_processor_t * processor = pool->head;
    while(processor) {
        object_lock(processor);
        event_processor_cancel(processor, nil);
        object_unlock(processor);

        processor = processor->next;
    }

    object_wakeup(pool->engine->queue, true);

    processor = pool->head;
    while(processor) {
        pool->head = pool->head->next;
        processor = event_processor_rem(processor);
        processor = pool->head;
    }
    pool->head = nil;
    pool->tail = nil;
    pool->size = 0LU;
    object_unlock(pool);

    pool->sync = sync_rem(pool->sync);
    free(pool);

    return nil;
}