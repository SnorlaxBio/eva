/**
 * @file        snorlax/event/engine.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 17, 2024
 */

#include <stdlib.h>

#include "engine.h"

static event_engine_t * event_engine_func_rem(event_engine_t * o);

static event_engine_func_t func = {
    event_engine_func_rem
};

static event_engine_t * engine = nil;

static event_engine_t * event_engine_gen(void);

extern int32_t event_engine_on(void) {
    if(engine == nil) {
        engine = event_engine_gen();
    }

    return success;
}

extern int32_t event_engine_off(event_engine_cancel_t cancel) {
    if(engine) {
        object_lock(engine);
        if(engine->cancel) {
            // TODO: DEBUG ALREADY CANCELLED ...
            object_unlock(engine);
            return success;
        }

        engine->cancel = cancel;
        object_unlock(engine);
    }

    return success;
}

extern int32_t event_engine_run(void) {
    if(engine) {
        object_lock(engine);
        while(engine->cancel == nil) {
            object_unlock(engine);
            // TODO: IMPLEMENT
            object_lock(engine);
        }
        engine->cancel(engine);
        object_unlock(engine);
        engine = event_engine_rem(engine);
        return success;
    }
    return fail;
}

static event_engine_t * event_engine_gen(void) {
    if(engine == nil) {
        engine = (event_engine_t *) calloc(1, sizeof(event_engine_t));

        engine->func = &func;
    }
    return engine;
}

static event_engine_t * event_engine_func_rem(event_engine_t * o) {
    if(o) {
        object_lock(o);
        // TODO: IMPLEMENT THIS
        object_unlock(o);

        o->sync = sync_rem(o->sync);
        free(o);
    }
    return nil;
}