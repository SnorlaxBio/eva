/**
 * @file        snorlax/event/processor.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 18, 2024
 */

#include <stdlib.h>

#include "engine.h"
#include "processor.h"
#include "processor/pool.h"

static event_processor_t * event_processor_func_rem(event_processor_t * processor);
static int32_t event_processor_func_run(event_processor_t * processor);

static event_processor_func_t func = {
    event_processor_func_rem
};

extern event_processor_t * event_processor_gen(event_processor_pool_t * pool) {
    event_processor_t * processor = (event_processor_t *) calloc(1, sizeof(event_processor_t));

    processor->func = &func;

    processor->thread = thread_gen((thread_routine_t) event_processor_func_run);

    return processor;
}

extern void event_processor_func_cancel(event_processor_t * processor) {

}

static event_processor_t * event_processor_func_rem(event_processor_t * processor) {
    event_processor_pool_t * pool = processor->pool;

    object_lock(pool);
    thread_cancel(processor->thread, (thread_cancel_t) event_processor_func_cancel);

    object_wakeup(pool->engine->queue, true);


    thread_off(processor->thread, (thread_cancel_t) event_processor_func_cancel);

    object_unlock(pool);


    processor->sync = sync_rem(processor->sync);
    free(processor);
    return nil;
}

extern void event_processor_cancel(event_processor_t * processor, event_processor_cancel_t cancel) {
    
}