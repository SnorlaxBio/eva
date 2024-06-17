/**
 * @file        snorlax/event/generator/set.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 18, 2024
 */

#ifndef   __SNORLAX__EVENT_GENERATOR_SET__H__
#define   __SNORLAX__EVENT_GENERATOR_SET__H__

#include <snorlax/eva.h>

struct event_generator_set {
    event_generator_set_func_t * func;
    sync_t * sync;

    event_generator_t * command;
    event_generator_t * descriptor;
    event_generator_t * time;
    event_generator_t * signal;
    event_generator_t * user;
};

struct event_generator_set_func {
    event_generator_set_t * (*rem)(event_generator_set_t *);
};

extern event_generator_set_t * event_generator_set_gen(void);

extern int32_t event_generator_set_func_pub(event_generator_set_t * set, event_queue_t * queue);
extern int32_t event_generator_set_func_on(event_generator_set_t * set);
extern int32_t event_generator_set_func_off(event_generator_set_t * set);

#define event_generator_set_rem(set)            (set ? set->func->rem(set) : nil)
#define event_generator_set_pub(set, queue)     event_generator_set_func_pub(set, queue);
#define event_generator_set_on(set)             event_generator_set_func_on(set)
#define event_generator_set_off(set)            event_generator_set_func_off(set)

#endif // __SNORLAX__EVENT_GENERATOR_SET__H__
