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
static int32_t event_processor_pool_func_on(event_processor_pool_t * pool);
static int32_t event_processor_pool_func_off(event_processor_pool_t * pool, event_processor_cancel_t cancel);

static event_processor_pool_func_t func = {
    event_processor_pool_func_rem,
    event_processor_pool_func_on,
    event_processor_pool_func_off
};

extern event_processor_pool_t * event_processor_pool_gen(event_engine_t * engine, uint32_t size) {
    event_processor_pool_t * pool = (event_processor_pool_t *) calloc(1, sizeof(event_processor_pool_t));

    pool->func = &func;

    pool->engine = engine;
    pool->sync = sync_gen();

    for(uint32_t i = 0; i < size; i++) {
        event_processor_pool_add(pool, event_processor_gen(pool));
    }

    return pool;
}

extern void event_processor_pool_add(event_processor_pool_t * pool, event_processor_t * processor) {
    object_lock(pool);

    if(pool->tail) {
        pool->tail->next = processor;
        processor->prev = pool->tail;
    } else {
        pool->head = processor;
    }
    pool->tail = processor;
    pool->size = pool->size + 1;

    processor->pool = pool;

    object_unlock(pool);
}

extern void event_processor_pool_del(event_processor_pool_t * pool, event_processor_t * processor) {
    object_lock(pool);

    event_processor_t * prev = processor->prev;
    event_processor_t * next = processor->next;

    if(prev) {
        prev->next = next;
    } else {
        pool->head = next;
    }

    if(next) {
        next->prev = prev;
    } else {
        pool->tail = prev;
    }

    pool->size = pool->size - 1;

    processor->prev = nil;
    processor->next = nil;
    processor->pool = nil;

    object_unlock(pool);
}



static event_processor_pool_t * event_processor_pool_func_rem(event_processor_pool_t * pool) {
    object_lock(pool);
    event_processor_t * processor = pool->head;
    while(processor) {
        object_lock(processor);
        event_processor_cancel_set(processor, nil);
        object_unlock(processor);

        processor = processor->next;
    }

    object_wakeup(pool->engine->queue, true);

    while(pool->head) {
        event_processor_rem(pool->head);
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

static int32_t event_processor_pool_func_off(event_processor_pool_t * pool, event_processor_cancel_t cancel) {
    object_lock(pool);
    event_processor_t * processor = pool->head;
    while(processor) {
        event_processor_off(processor, nil);
        processor = processor->next;
    }
    object_unlock(pool);
    return success;
}
