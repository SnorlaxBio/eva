/**
 * @file        snorlax/event/engine.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 14, 2024
 */

#ifndef   __SNORLAX__EVENT_ENGINE__H__
#define   __SNORLAX__EVENT_ENGINE__H__

#include <snorlax/eva.h>

struct event_engine_func;

typedef struct event_engine_func event_engine_func_t;



struct event_engine {
    event_engine_func_t * func;
    uint64_t size;
    event_object_t * head;
    event_object_t * tail;

    event_engine_cancel_t cancel;
};

struct event_engine_func {
    event_engine_t * (*rem)(event_engine_t *);

    event_object_t * (*add)(event_engine_t *, event_object_t *);
    event_object_t * (*del)(event_engine_t *, event_object_t *);

    int (*run)(event_engine_t *);
};

extern event_object_t * event_engine_func_add(event_engine_t * engine, event_object_t * o);
extern event_object_t * event_engine_func_del(event_engine_t * engine, event_object_t * o);

#endif // __SNORLAX__EVENT_ENGINE__H__
