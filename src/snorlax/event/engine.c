/**
 * @file        snorlax/event/engine.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 14, 2024
 */

#include "engine.h"
#include "object.h"

static event_engine_t * singleton = nil;

static event_engine_t * event_engine_gen(void);

extern int event_engine_run(void) {
    if(singleton == nil) singleton = event_engine_gen();

    return singleton->func->run(singleton);
}

extern void event_engine_cancel(event_engine_cancel_t cancel) {
    // TODO: ASSERT
    singleton->cancel = cancel;
}

extern event_object_t * event_engine_object_add(event_object_t * o) {
    // TODO: ASSERT
    return singleton->func->add(singleton, o);
}

extern event_object_t * event_engine_object_del(event_object_t * o) {
    // TODO: ASSERT
    return singleton->func->del(singleton, o);
}

static event_engine_t * event_engine_gen(void) {
    event_engine_t * o = (event_engine_t *) event_engine_epoll_gen();

    return o;
}

extern event_object_t * event_engine_func_add(event_engine_t * engine, event_object_t * o) {
    if(engine->tail) {
        engine->tail->next = o;
        o->prev = engine->tail;
    } else {
        engine->head = o;
    }
    engine->tail = o;
    engine->size = engine->size + 1;
    return o;
}

extern event_object_t * event_engine_func_del(event_engine_t * engine, event_object_t * o) {
    if(o->engine) {
        event_object_t * prev = o->prev;
        event_object_t * next = o->next;

        if(prev) {
            prev->next = next;
        } else {
            engine->head = next;
        }

        if(next) {
            next->prev = prev;
        } else {
            engine->tail = prev;
        }

        engine->size = engine->size - 1;

        o->engine = nil;
    }
    
    return o;
}

