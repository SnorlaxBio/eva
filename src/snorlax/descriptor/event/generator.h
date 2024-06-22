/**
 * @file        snorlax/descriptor/event/generator.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 22, 2024
 */

#ifndef   __SNORLAX__DESCRIPTOR_EVENT_GENERATOR__H__
#define   __SNORLAX__DESCRIPTOR_EVENT_GENERATOR__H__

#include <snorlax/descriptor/event.h>

struct descriptor_event_generator {
    descriptor_event_generator_func_t * func;
    sync_t * sync;
    uint64_t size;
    descriptor_event_subscription_t * head;
    descriptor_event_subscription_t * tail;
    event_generator_set_t * set;
    uint32_t status;
};

struct descriptor_event_generator_func {
    descriptor_event_generator_t * (*rem)(___notnull descriptor_event_generator_t *);
    ___sync int32_t (*on)(___notnull descriptor_event_generator_t *);
    ___sync int32_t (*off)(___notnull descriptor_event_generator_t *);
    ___sync int32_t (*pub)(___notnull descriptor_event_generator_t *, event_queue_t *);
    ___sync int32_t (*add)(___notnull descriptor_event_generator_t *, ___notnull descriptor_event_subscription_t *);
    ___sync int32_t (*del)(___notnull descriptor_event_generator_t *, ___notnull descriptor_event_subscription_t *);
    ___sync void (*clear)(___notnull descriptor_event_generator_t *);
};

extern descriptor_event_generator_t * descriptor_event_generator_gen(___notnull event_generator_set_t * set);

#define descriptor_event_generator_rem(generator)                   ((generator)->func->rem(generator))
#define descriptor_event_generator_on(generator)                    ((generator)->func->on(generator))
#define descriptor_event_generator_off(generator)                   ((generator)->func->off(generator))
#define descriptor_event_generator_pub(generator, queue)            ((generator)->func->pub(generator, queue))
#define descriptor_event_generator_add(generator, subscription)     ((generator)->func->add(generator, subscription))
#define descriptor_event_generator_del(generator, subscription)     ((generator)->func->del(generator, subscription))
#define descriptor_event_generator_clear(generator)                 ((generator)->func->clear(generator))

#endif // __SNORLAX__DESCRIPTOR_EVENT_GENERATOR__H__
