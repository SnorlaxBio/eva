/**
 * @file        snorlax/command/event/subscription.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 18, 2024
 */

#ifndef   __SNORLAX__COMMAND_EVENT_SUBSCRIPTION__H__
#define   __SNORLAX__COMMAND_EVENT_SUBSCRIPTION__H__

#include <snorlax/command.h>

struct command_event_subscription {
    command_event_subscription_func_t * func;
    sync_t * sync;

    command_event_generator_t * generator;
    command_event_subscription_t * prev;
    command_event_subscription_t * next;

    event_subscription_event_queue_t * queue;

    uint32_t status;

    command_t * command;
};

struct command_event_subscription_func {
    command_event_subscription_t * (*rem)(command_event_subscription_t *);
};

#endif // __SNORLAX__COMMAND_EVENT_SUBSCRIPTION__H__
