/**
 * @file        snorlax/event/engine.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 21, 2024
 */

#ifndef   __SNORLAX__EVENT_ENGINE__H__
#define   __SNORLAX__EVENT_ENGINE__H__

#include <snorlax/event.h>

#include <snorlax/event/engine/state.h>

struct event_engine {
    event_engine_func_t * func;
    sync_t * sync;
    uint32_t status;
    event_queue_t * queue;
    event_processor_pool_t * pool;
    event_generator_set_t * set;
    event_engine_cancel_t cancel;
};

struct event_engine_func {
    event_engine_t * (*rem)(___notnull event_engine_t *);
    int32_t (*on)(___notnull event_engine_t *);
    int32_t (*off)(___notnull event_engine_t *, event_engine_cancel_t);
    int32_t (*run)(___notnull event_engine_t *);
};

extern event_engine_t * event_engine_gen(void);



#define event_engine_rem(engine)            ((engine)->func->rem(engine))
#define event_engine_on(engine)             ((engine)->func->on(engine))
#define event_engine_off(engine, cancel)    ((engine)->func->off(engine, cancel))
#define event_engine_run(engine)            ((engine)->func->run(engine))

#endif // __SNORLAX__EVENT_ENGINE__H__
