/**
 * @file        snorlax/event/generator.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 21, 2024
 */

#ifndef   __SNORLAX__EVENT_GENERATOR__H__
#define   __SNORLAX__EVENT_GENERATOR__H__

#include <snorlax/event.h>

struct event_generator {
    event_generator_func_t * func;
    sync_t * sync;
    uint64_t size;
    event_subscription_t * head;
    event_subscription_t * tail;

    event_generator_set_t * set;
};

struct event_generator_func {
    event_generator_t * (*rem)(___notnull event_generator_t *);
    int32_t (*on)(___notnull event_generator_t *);
    int32_t (*off)(___notnull event_generator_t *);
    int32_t (*pub)(___notnull event_generator_t *, event_queue_t *);
    int32_t (*add)(___notnull event_generator_t *, ___notnull event_subscription_t *);
    int32_t (*del)(___notnull event_generator_t *, ___notnull event_subscription_t *);
};

extern int32_t event_generator_func_add(___notnull event_generator_t * generator, ___notnull event_subscription_t * subscription);
extern int32_t event_generator_func_del(___notnull event_generator_t * generator, ___notnull event_subscription_t * subscription);

#endif // __SNORLAX__EVENT_GENERATOR__H__
