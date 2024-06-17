/**
 * @file        snorlax/event/engine.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 18, 2024
 */

#include "engine.h"

#include "processor/pool.h"
#include "generator/set.h"
#include "queue.h"

#include "../event.h"

static event_engine_t * engine = nil;

extern uint32_t event_engine_pool_size(void) {
    if(engine) {
        object_lock(engine->pool);

        uint32_t n = engine->pool->size;

        object_unlock(engine->pool);

        return n;
    }

    return 0;
}

extern void event_engine_push(event_t * event) {
    if(engine && event) {
        object_lock(engine->queue);
        event_queue_push(engine->queue, event);
        object_unlock(engine->queue);

        object_wakeup(engine->queue, false);
    }
}

extern int32_t event_engine_run(void) {
    object_lock(engine);

    while(engine->cancel == nil) {
        object_unlock(engine);

        if(event_engine_pool_size() == 0) {
            event_generator_set_pub(engine->set, engine->queue);

            object_lock(engine->queue);
            uint64_t limit = engine->queue->size;
            for(uint64_t i = 0; i < limit; i++) {
                event_t * event = event_queue_pop(engine->queue);
                if(event) {
                    object_unlock(engine->queue);

                    event_on(event);
                    // TODO: FREE EVENT

                    object_lock(engine->queue);
                    continue;
                }
                break;
            }
            object_unlock(engine->queue);
        }

        object_lock(engine);
    }

    engine->cancel(engine);
    engine->cancel = nil;

    object_unlock(engine);

    engine = event_engine_rem(engine);

    return success;
}