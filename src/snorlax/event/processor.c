/**
 * @file        snorlax/event/processor.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 18, 2024
 */

#include <stdlib.h>

#include "processor.h"

#include "processor/pool.h"
#include "queue.h"
#include "engine.h"

#include "../event.h"

static int32_t event_processor_func_routine(event_processor_t * processor);

static event_processor_t * event_processor_func_rem(event_processor_t * processor);
static int32_t event_processor_func_on(event_processor_t * processor);
static int32_t event_processor_func_off(event_processor_t * processor, event_processor_cancel_t cancel);

static event_processor_func_t func = {
    event_processor_func_rem,
    event_processor_func_on,
    event_processor_func_off
};

extern event_processor_t * event_processor_gen(event_processor_pool_t * pool) {
    event_processor_t * processor = (event_processor_t *) calloc(1, sizeof(event_processor_t));

    processor->func = &func;
    processor->thread = thread_gen((thread_routine_t) event_processor_func_routine);
    processor->sync = sync_gen();

    event_processor_pool_add(pool, processor);

    return processor;
}

extern void event_processor_cancel_set(event_processor_t * processor, event_processor_cancel_t cancel) {
    object_lock(processor);

    processor->thread->cancel = (thread_cancel_t) (cancel ? cancel : event_processor_func_cancel);

    object_unlock(processor);
}

extern void event_processor_func_cancel(event_processor_t * processor) {

}

static int32_t event_processor_func_routine(event_processor_t * processor) {
    object_lock(processor);
    event_queue_t * queue = processor->pool->engine->queue;
    while(processor->thread->cancel == nil) {
        object_unlock(processor);

        object_lock(queue);
        event_t * event = event_queue_pop(queue);

        if(event) {
            object_unlock(queue);

            event_on(event);

            event = event_rem(event);
        } else {
            object_wait(queue, 0LU, 0LU);
            object_lock(queue);
        }

        object_lock(processor);
    }

    processor->thread->cancel((thread_t *) processor);
    processor->thread->cancel = nil;

    object_unlock(processor);

    return success;
}

static event_processor_t * event_processor_func_rem(event_processor_t * processor) {
    // PROCESSOR OFF
    processor->thread = thread_rem(processor->thread);

    event_processor_pool_del(processor->pool, processor);

    processor->sync = sync_rem(processor->sync);
    free(processor);

    return nil;
}

static int32_t event_processor_func_on(event_processor_t * processor) {
    return thread_on(processor->thread);
}

static int32_t event_processor_func_off(event_processor_t * processor, event_processor_cancel_t cancel) {
    return thread_off(processor->thread, (thread_cancel_t) cancel);
}

// #include "engine.h"
// #include "processor/pool.h"

// static event_processor_t * event_processor_func_rem(event_processor_t * processor);
// static int32_t event_processor_func_run(event_processor_t * processor);

// static event_processor_func_t func = {
//     event_processor_func_rem
// };

// extern event_processor_t * event_processor_gen(event_processor_pool_t * pool) {
//     event_processor_t * processor = (event_processor_t *) calloc(1, sizeof(event_processor_t));

//     processor->func = &func;

//     processor->thread = thread_gen((thread_routine_t) event_processor_func_run);

//     return processor;
// }

// extern void event_processor_func_cancel(event_processor_t * processor) {

// }

// static event_processor_t * event_processor_func_rem(event_processor_t * processor) {
//     event_processor_pool_t * pool = processor->pool;

//     object_lock(pool);
//     thread_cancel(processor->thread, (thread_cancel_t) event_processor_func_cancel);

//     object_wakeup(pool->engine->queue, true);


//     thread_off(processor->thread, (thread_cancel_t) event_processor_func_cancel);

//     object_unlock(pool);


//     processor->sync = sync_rem(processor->sync);
//     free(processor);
//     return nil;
// }

// extern void event_processor_cancel(event_processor_t * processor, event_processor_cancel_t cancel) {
    
// }