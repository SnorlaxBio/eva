/**
 * @file        snorlax/command/event/subscription.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 22, 2024
 */

#ifndef   __SNORLAX__COMMAND_EVENT_SUBSCRIPTION__H__
#define   __SNORLAX__COMMAND_EVENT_SUBSCRIPTION__H__

#include <snorlax/command/event.h>

struct command_event_subscription {
    command_event_subscription_func_t * func;
    sync_t * sync;
    command_event_generator_t * generator;
    command_event_subscription_t * prev;
    command_event_subscription_t * next;
    command_event_subscription_event_queue_t * queue;

    int32_t retry;
    int32_t count;
    command_t * command;
};

struct command_event_subscription_func {
    command_event_subscription_t * (*rem)(command_event_subscription_t *);
};

#endif // __SNORLAX__COMMAND_EVENT_SUBSCRIPTION__H__
