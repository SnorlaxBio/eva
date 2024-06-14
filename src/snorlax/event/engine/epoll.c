/**
 * @file        snorlax/event/engine/epoll.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 14, 2024
 */

#include <unistd.h>
#include <stdlib.h>
#include <sys/epoll.h>

#include "epoll.h"

#include "../engine.h"
#include "../object.h"

extern event_engine_epoll_t * event_engine_epoll_gen(void) {
    event_engine_epoll_t * engine = (event_engine_epoll_t *) calloc(1, sizeof(event_engine_epoll_t));

    engine->max = 1024;
    engine->events = malloc(sizeof(struct epoll_event) * engine->max);
    engine->descriptor = epoll_create(engine->max);
    engine->timeout = 1;

    return engine;
}

extern event_engine_epoll_t * event_engine_epoll_func_rem(event_engine_epoll_t * engine) {
    while(engine->head) {
        event_object_t * o = event_engine_epoll_func_del(engine, engine->head);
        o = event_object_rem(o);
    }

    if(engine->events) {
        free(engine->events);
        engine->events = nil;
    }

    if(engine->descriptor > invalid) {
        close(engine->descriptor);
        engine->descriptor = invalid;
    }

    free(engine);

    return nil;
}

extern event_object_t * event_engine_epoll_func_add(event_engine_epoll_t * engine, event_object_t * o) {
    event_engine_func_add((event_engine_t *) engine, o);
    // EVENT FUNC CHECK UP ... 
    // CHECK: DESCRIPTOR EVENT OBJECT ... 
}

extern event_object_t * event_engine_epoll_func_del(event_engine_epoll_t * engine, event_object_t * o) {
    event_engine_func_del((event_engine_t *) engine, o);
    // CHECK: DESCRIPTOR EVENT OBJECT ... 
}
