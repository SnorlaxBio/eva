/**
 * @file        snorlax/descriptor/event/generator.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 19, 2024
 */

#ifndef   __SNORLAX__DESCRIPTOR_EVENT_GENERATOR__H__
#define   __SNORLAX__DESCRIPTOR_EVENT_GENERATOR__H__

#include <snorlax/descriptor.h>

#include <snorlax/eva.h>
#include <snorlax/event/generator.h>

struct descriptor_event_generator {
    descriptor_event_generator_func_t * func;
    sync_t * sync;

    uint64_t size;
    descriptor_event_subscription_t * head;
    descriptor_event_subscription_t * tail;
};

struct descriptor_event_generator_func {
    descriptor_event_generator_t * (*rem)(descriptor_event_generator_t *);
    int32_t (*on)(descriptor_event_generator_t *);
    int32_t (*off)(descriptor_event_generator_t *);
    int32_t (*pub)(descriptor_event_generator_t *, event_queue_t *);
    int32_t (*add)(descriptor_event_generator_t *, descriptor_event_subscription_t *);
    int32_t (*del)(descriptor_event_generator_t *, descriptor_event_subscription_t *);
};

#define descriptor_event_generator_rem(generator)                       ((generator)->func->rem(generator))
#define descriptor_event_generator_on(generator)                        ((generator)->func->on(generator))
#define descriptor_event_generator_off(generator)                       ((generator)->func->off(generator))
#define descriptor_event_generator_pub(generator, queue)                ((generator)->func->pub(generator, queue))
#define descriptor_event_generator_add(generator, subscription)         ((generator)->func->add(generator, subscription))
#define descriptor_event_generator_del(generator, subscription)         ((generator)->func->del(generator, subscription))

#endif // __SNORLAX__DESCRIPTOR_EVENT_GENERATOR__H__
