/**
 * @file        snorlax/event/generator.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 18, 2024
 */

#ifndef   __SNORLAX__EVENT_GENERATOR__H__
#define   __SNORLAX__EVENT_GENERATOR__H__

#include <snorlax/eva.h>

struct event_generator {
    event_generator_func_t * func;
    sync_t * sync;

    uint64_t size;
    event_subscription_t * head;
    event_subscription_t * tail;
};

struct event_generator_func {
    event_generator_t * (*rem)(event_generator_t *);
    int32_t (*on)(event_generator_t *);
    int32_t (*off)(event_generator_t *);
    int32_t (*pub)(event_generator_t *, event_queue_t *);
    int32_t (*add)(event_generator_t *, event_subscription_t *);
    int32_t (*del)(event_generator_t *, event_subscription_t *);
};

#define event_generator_rem(generator)                      (generator ? generator->func->rem(generator) : nil)
#define event_generator_on(generator)                       (generator ? generator->func->on(generator) : fail)
#define event_generator_off(generator)                      (generator ? generator->func->off(generator) : fail)
#define event_generator_pub(generator, queue)               (generator ? generator->func->pub(generator, queue) : false)
#define event_generator_add(generator, subscription)        (generator ? generator->func->add(generator, subscription) : fail)
#define event_generator_del(generator, subscription)        (generator ? generator->func->del(generatgor, subscription) : fail)

extern int32_t event_generator_func_add(event_generator_t * generator, event_subscription_t * subscription);
extern int32_t event_generator_func_del(event_generator_t * generator, event_subscription_t * subscription);

#endif // __SNORLAX__EVENT_GENERATOR__H__
