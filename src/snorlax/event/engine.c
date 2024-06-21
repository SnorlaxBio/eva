/**
 * @file        snorlax/event/engine.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 22, 2024
 */

#include "engine.h"

#include "queue.h"
#include "processor/pool.h"
#include "generator/set.h"

static void event_engine_func_default_cancel(event_engine_t * engine);

static event_engine_t * event_engine_func_rem(___notnull event_engine_t * engine);
static int32_t event_engine_func_on(___notnull event_engine_t * engine);
static int32_t event_engine_func_off(___notnull event_engine_t * engine, event_engine_cancel_t cancel);
static int32_t event_engine_func_run(___notnull event_engine_t * engine);

static event_engine_func_t func = {
    event_engine_func_rem,
    event_engine_func_on,
    event_engine_func_off,
    event_engine_func_run
};

extern event_engine_t * event_engine_gen(void) {
    event_engine_t * engine = (event_engine_t *) calloc(1, sizeof(event_engine_t));

    engine->func = address_of(func);

    engine->pool = event_processor_pool_gen(0, engine);
    engine->set = event_generator_set_gen(engine);
    engine->queue = event_queue_gen(engine->pool->size > 0 ? sync_gen() : nil);

    return engine;
}

static event_engine_t * event_engine_func_rem(___notnull event_engine_t * engine) {
#ifndef   RELEASE
    snorlaxdbg(engine == nil, "critical", "");
#endif // RELEASE

    engine->set = event_generator_set_rem(engine->set);
    engine->pool = event_processor_pool_rem(engine->pool);
    engine->queue = event_queue_rem(engine->queue);

    engine->sync = sync_rem(engine->sync);

    free(engine);

    return nil;
}

static int32_t event_engine_func_on(___notnull event_engine_t * engine) {
#ifndef   RELEASE
    snorlaxdbg(engine == nil, "critical", "");
#endif // RELEASE

    if((engine->status & event_engine_state_on) == 0) {
        event_processor_pool_on(engine->pool);
        event_generator_set_on(engine->set);
    } else {
#ifndef   RELEASE
        snorlaxdbg(false, "notice", "event engine already on");
#endif // RELEASE
    }

    return success;
}

static int32_t event_engine_func_off(___notnull event_engine_t * engine, event_engine_cancel_t cancel) {
#ifndef   RELEASE
    snorlaxdbg(engine == nil, "critical", "");
#endif // RELEASE

    if(engine->status & event_engine_state_on) {
        engine->status = engine->status & (~event_engine_state_on);
        if(engine->cancel == nil) {
            engine->cancel = cancel ? cancel : event_engine_func_default_cancel;
        } else {
#ifndef   RELEASE
            snorlaxdbg(false, "notice", "event engine cancel already set");
#endif // RELEASE
        }
    } else {
#ifndef   RELEASE
        snorlaxdbg(false, "notice", "event engine already off");
#endif // RELEASE
    }

    return success;
}

static int32_t event_engine_func_run(___notnull event_engine_t * engine) {
#ifndef   RELEASE
    snorlaxdbg(engine == nil, "critical", "");
#endif // RELEASE
    event_processor_pool_t * pool = engine->pool;

    while(engine->cancel == nil) {
        event_queue_t * queue = pool->size > 0 ? engine->queue : nil;

        event_generator_set_pub(engine->set, queue);

        queue = engine->queue;

        if(queue == nil) {
            queue = engine->queue;
            object_lock(queue);
            uint64_t limit = queue->size;
            for(uint64_t i = 0; i < limit; i++) {
                event_t * event = event_queue_pop(queue);
                if(event) {
                    object_unlock(queue);
                    event_on(event);
                    object_lock(queue);
                    continue;;
                }
                break;
            }
            object_unlock(queue);
        }
    }
#ifndef   RELEASE
    snorlaxdbg(engine->status & event_engine_state_on, "critical", "");
#endif // RELEASE
    engine->cancel(engine);
    engine->cancel = nil;

    return success;
}
