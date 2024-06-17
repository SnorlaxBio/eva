/**
 * @file        snorlax/command/event.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 18, 2024
 */

#ifndef   __SNORLAX__COMMAND_EVENT__H__
#define   __SNORLAX__COMMAND_EVENT__H__

#include <snorlax/eva.h>
#include <snorlax/command.h>

struct command_event {
    command_event_func_t * func;
    sync_t * sync;

    event_queue_t * queue;
    event_t * prev;
    event_t * next;

    event_subscription_event_t * node;

    command_event_subscription_t * subscription;
    uint32_t type;
};

struct command_event_func {
    command_event_t * (*rem)(command_event_t *);
    int32_t (*on)(command_event_t *);
};

extern command_event_t * command_event_gen(command_event_subscription_t * subscription, uint32_t type);

#define command_event_rem(event)        (event ? event->func->rem(event) : nil)

#endif // __SNORLAX__COMMAND_EVENT__H__
