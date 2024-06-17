/**
 * @file        snorlax/event/engine.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 17, 2024
 */

#ifndef   __SNORLAX__EVENT_ENGINE__H__
#define   __SNORLAX__EVENT_ENGINE__H__

#include <snorlax/eva.h>

struct event_engine {
    event_engine_func_t * func;
    sync_t * sync;

    event_queue_t * queue;
    event_processor_pool_t * pool;

    event_engine_cancel_t cancel;
};

struct event_engine_func {
    event_engine_t * (*rem)(event_engine_t *);
};

#define event_engine_rem(engine)        (engine ? engine->func->rem(engine) : nil)

extern void event_engine_queue_push(event_engine_t * engine, event_t * event);

#endif // __SNORLAX__EVENT_ENGINE__H__
