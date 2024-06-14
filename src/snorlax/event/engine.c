/**
 * @file        snorlax/event/engine.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 13, 2024
 */

#include "engine.h"
#include "object.h"
#include "processor/pool.h"

#include "../descriptor.h"
#include "../descriptor/state.h"
#include "../descriptor/event/type.h"
#include "../event.h"
#include "../event/queue.h"

static event_engine_t * __engine = nil;

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
    o->prev = nil;
    o->next = nil;
    o->engine = nil;
    engine->size = engine->size - 1;
    return o;
}
