/**
 * @file        snorlax/event/generator.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 15, 2024
 */

#ifndef   __SNORLAX__EVENT_GENERATOR__H__
#define   __SNORLAX__EVENT_GENERATOR__H__

#include <snorlax/eva.h>

struct event_generator_func;

typedef struct event_generator_func event_generator_func_t;

struct event_generator {
    event_generator_func_t * func;
    uint64_t size;
    event_subscription_t * head;
    event_subscription_t * tail;
};

struct event_generator_func {
    event_generator_t * (*rem)(event_generator_t *);

    int (*add)(event_generator_t *, event_subscription_t *);
    int (*del)(event_generator_t *, event_subscription_t *);
    int (*mod)(event_generator_t *, event_subscription_t *);

    int (*pub)(event_generator_t *, event_queue_t *);

};

#endif // __SNORLAX__EVENT_GENERATOR__H__
