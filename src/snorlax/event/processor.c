/**
 * @file        snorlax/event/processor.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 21, 2024
 */

#include <stdlib.h>

#include "processor.h"
#include "processor/pool.h"
#include "engine.h"
#include "queue.h"

static int32_t event_processor_func_thread_routine(___notnull event_processor_t * processor);
static void event_processor_func_thread_cancel(event_processor_t * processor);

static event_processor_t * event_processor_func_rem(___notnull event_processor_t * processor);
static int32_t event_processor_func_on(___notnull event_processor_t * processor);
static int32_t event_processor_func_off(___notnull event_processor_t * processor, event_processor_cancel_t cancel);

static event_processor_func_t func = {
    event_processor_func_rem,
    event_processor_func_on,
    event_processor_func_off
};

extern event_processor_t * event_processor_gen(void) {
    event_processor_t * processor = (event_processor_t *) calloc(1, sizeof(event_processor_t));

    processor->func = address_of(func);
    processor->sync = sync_gen();
    processor->thread = thread_gen((thread_routine_t) event_processor_func_thread_routine);

    return processor;
}

static event_processor_t * event_processor_func_rem(___notnull event_processor_t * processor) {
#ifndef   RELEASE
    snorlaxdbg(processor == nil, false, "critical", "");
#endif // RELEASE

    event_processor_pool_t * pool = processor->pool;

    if(pool) {
#ifndef   RELEASE
        snorlaxdbg(false, true, "notice", "processor is in pool");
#endif // RELEASE

        object_lock(pool);
        event_processor_pool_del(pool, processor);
        object_unlock(pool);
    }

    event_processor_off(processor, nil);

    processor->thread = thread_rem(processor->thread);
    processor->sync = sync_rem(processor->sync);

    free(processor);

    return nil;
}

static int32_t event_processor_func_on(___notnull event_processor_t * processor) {
#ifndef   RELEASE
    snorlaxdbg(processor == nil, false, "critical", "");
    snorlaxdbg(processor->thread == nil, false, "critical", "");
    snorlaxdbg(processor->pool == nil, false, "critical", "");
#endif // RELEASE
    
    thread_t * thread = processor->thread;

    if(!thread_alive(thread)) {
        thread_on(thread);
    }

    return success;
}

static int32_t event_processor_func_off(___notnull event_processor_t * processor, event_processor_cancel_t cancel) {
#ifndef   RELEASE
    snorlaxdbg(processor == nil, false, "critical", "");
    snorlaxdbg(processor->thread == nil, false, "critical", "");

    snorlaxdbg(processor->pool == nil, false, "critical", "");
    snorlaxdbg(processor->pool->engine == nil, false, "critical", "");
    snorlaxdbg(processor->pool->engine->queue == nil, false, "critical", "");
#endif // RELEASE

    thread_t * thread = processor->thread;

    if(thread_alive(thread)) {
        thread_cancel_set(thread, (thread_cancel_t) (cancel ? cancel : event_processor_func_thread_cancel));
        object_wakeup(processor->pool->engine->queue, true);
        thread_off(thread, nil);
    }

    return success;
}

static int32_t event_processor_func_thread_routine(___notnull event_processor_t * processor) {
#ifndef   RELEASE
    snorlaxdbg(processor == nil, false, "critical", "");
    snorlaxdbg(processor->pool == nil, false, "critical", "");
    snorlaxdbg(processor->pool->engine == nil, false, "critical", "");
    snorlaxdbg(processor->pool->engine->queue == nil, false, "critical", "");
    snorlaxdbg(processor->thread == nil, false, "critical", "");
#endif // RELEASE

    event_queue_t * queue = processor->pool->engine->queue;
    thread_t * thread = processor->thread;

    while(thread_cancel_get(thread) == nil) {
        object_lock(queue);
        event_t * event = event_queue_pop(queue);

        if(event) {
            object_unlock(queue);

            event_on(event);
        } else {
            object_wait(queue, 0, 0);
            object_unlock(queue);
        }
    }

}

static void event_processor_func_thread_cancel(event_processor_t * processor) {

}
