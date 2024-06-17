/**
 * @file        snorlax/event/processor.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 17, 2024
 */

#include "processor.h"

#include "engine.h"
#include "processor/pool.h"

static int32_t event_processor_routine_func(event_processor_t * processor);

static event_processor_t * event_processor_func_rem(event_processor_t * processor, event_processor_cancel_t cancel);

static event_processor_func_t func = {
    event_processor_func_rem
};

extern event_processor_t * event_processor_gen(event_processor_pool_t * pool) {
    event_processor_t * processor = (event_processor_t *) calloc(1, sizeof(event_processor_t));

    processor->func = &func;
    processor->sync = sync_gen();

    processor->queue = &(pool->engine->queue);
    processor->thread = thread_gen((thread_routine_t) event_processor_routine_func);

    if(pool->tail) {
        pool->tail->next =  processor;
        processor->prev = pool->tail;
    } else {
        pool->head = processor;
    }
    processor->pool = pool;
    pool->size = pool->size + 1;

    return processor;
}

extern void event_processor_func_cancel(event_processor_t * processor) {

}

static event_processor_t * event_processor_func_rem(event_processor_t * processor, event_processor_cancel_t cancel) {
    processor->thread = thread_rem(processor->thread, (thread_cancel_t) (cancel ? cancel : event_processor_func_cancel));

    processor->queue = nil;

    event_processor_t * prev = processor->prev;
    event_processor_t * next = processor->next;

    if(prev) {
        prev->next = next;
    } else {
        processor->pool->head = next;
    }

    if(next) {
        next->prev = prev;
    } else {
        processor->pool->tail = prev;
    }

    processor->pool = processor->pool - 1;

    processor->prev = nil;
    processor->next = nil;
    processor->pool = nil;

    processor->sync = sync_rem(processor->sync);

    free(processor);

    return nil;
}
