/**
 * @file        snorlax/event/generator/set.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 21, 2024
 */

#ifndef   __SNORLAX__EVENT_GENERATOR_SET__H__
#define   __SNORLAX__EVENT_GENERATOR_SET__H__

#include <snorlax/event.h>

struct event_generator_set {
    event_generator_set_func_t * func;
    sync_t * sync;
    event_engine_t * engine;
    event_generator_t * command;
    event_generator_t * descriptor;
    event_generator_t * time;
    event_generator_t * signal;
};

struct event_generator_set_func {
    ___sync event_generator_set_t * (*rem)(___notnull event_generator_set_t *);
    ___sync int32_t (*on)(___notnull event_generator_set_t *);
    ___sync int32_t (*off)(___notnull event_generator_set_t *);
    ___sync int32_t (*pub)(___notnull event_generator_set_t *, event_queue_t *);
};

extern event_generator_set_t * event_generator_set_gen(___notnull event_engine_t * engine);

#define event_generator_set_rem(set)            ((set)->func->rem(set))
#define event_generator_set_on(set)             ((set)->func->on(set))
#define event_generator_set_off(set)            ((set)->func->off(set))
#define event_generator_set_pub(set, queue)     ((set)->func->pub(set, queue))

#endif // __SNORLAX__EVENT_GENERATOR_SET__H__
