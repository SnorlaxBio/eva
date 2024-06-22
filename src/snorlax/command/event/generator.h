/**
 * @file        snorlax/command/event/generator.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 22, 2024
 */

#ifndef   __SNORLAX__COMMAND_EVENT_GENERATOR__H__
#define   __SNORLAX__COMMAND_EVENT_GENERATOR__H__

#include <snorlax/command/event.h>

struct command_event_generator {
    command_event_generator_func_t * func;
    sync_t * sync;
    uint64_t size;
    command_event_subscription_t * head;
    command_event_subscription_t * tail;
    event_generator_set_t * set;
    uint32_t status;
};

struct command_event_generator_func {
    command_event_generator_t * (*rem)(___notnull command_event_generator_t *);
    ___sync int32_t (*on)(___notnull command_event_generator_t *);
    ___sync int32_t (*off)(___notnull command_event_generator_t *);
    ___sync int32_t (*pub)(___notnull command_event_generator_t *, event_queue_t *);
    ___sync int32_t (*add)(___notnull command_event_generator_t *, ___notnull command_event_subscription_t *);
    ___sync int32_t (*del)(___notnull command_event_generator_t *, ___notnull command_event_subscription_t *);
    ___sync void (*clear)(___notnull command_event_generator_t *);
};

extern command_event_generator_t * command_event_generator_gen(___notnull event_generator_set_t * set);

#define command_event_generator_rem(generator)                  ((generator)->func->rem(generator))
#define command_event_generator_on(generator)                   ((generator)->func->on(generator))
#define command_event_generator_off(generator)                  ((generator)->func->off(generator))
#define command_event_generator_pub(generator, queue)           ((generator)->func->pub(generator, queue))
#define command_event_generator_add(generator, subscription)    ((generator)->func->add(generator, subscription))
#define command_event_generator_del(generator, subscription)    ((generator)->func->del(generator, subscription))
#define command_event_generator_clear(generator)                ((generator)->func->clear(generator))

#endif // __SNORLAX__COMMAND_EVENT_GENERATOR__H__
