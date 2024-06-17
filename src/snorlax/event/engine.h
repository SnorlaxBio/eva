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
    event_processor_pool_t * processors;

    event_engine_cancel_t cancel;
};

struct event_engine_func {
    event_engine_t * (*rem)(event_engine_t *);
};

#define event_engine_rem(engine)        (engine ? engine->func->rem(engine) : nil)

#endif // __SNORLAX__EVENT_ENGINE__H__
