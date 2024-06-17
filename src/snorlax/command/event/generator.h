/**
 * @file        snorlax/command/event/generator.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 18, 2024
 */

#ifndef   __SNORLAX__COMMAND_EVENT_GENERATOR__H__
#define   __SNORLAX__COMMAND_EVENT_GENERATOR__H__

#include <snorlax/eva.h>
#include <snorlax/command.h>

struct command_event_generator {
    command_event_generator_func_t * func;
    sync_t * sync;

    uint64_t size;
    command_event_subscription_t * head;
    command_event_subscription_t * tail;
};

struct command_event_generator_func {
    command_event_generator_t * (*rem)(command_event_generator_t *);
    int32_t (*on)(command_event_generator_t *);
    int32_t (*off)(command_event_generator_t *);
    int32_t (*pub)(command_event_generator_t *, event_queue_t *);
    int32_t (*add)(command_event_generator_t *, command_event_subscription_t *);
    int32_t (*del)(command_event_generator_t *, command_event_subscription_t *);
};

extern command_event_generator_t * command_event_generator_gen(void);


#endif // __SNORLAX__COMMAND_EVENT_GENERATOR__H__
