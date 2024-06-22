/**
 * @file        snorlax/command/event/subscription.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 22, 2024
 */

#ifndef   __SNORLAX__COMMAND_EVENT_SUBSCRITON__H__
#define   __SNORLAX__COMMAND_EVENT_SUBSCRITON__H__

#include <snorlax/command.h>
#include <snorlax/command/event.h>

struct command_event_subscription {
    command_event_subscription_func_t * func;
    sync_t * sync;
    command_event_generator_t * generator;
    command_event_subscription_t * prev;
    command_event_subscription_t * next;
    event_subscription_event_queue_t * queue;
    command_event_subscription_handler_t * handler;

    int32_t retry;
    command_t * command;
};

struct command_event_subscription_func {
    command_event_subscription_t * (*rem)(___notnull command_event_subscription_t *);
    void (*on)(___notnull command_event_subscription_t *, uint32_t, uint64_t);
};

extern command_event_subscription_t * command_event_subscription_gen(___notnull command_t * command, int32_t retry, command_event_subscription_handler_t * handler);

#define command_event_subscription_rem(subscription)                ((subscription)->func->rem(subscription))
#define command_event_subscription_on(subscription, type, param)    ((subscription)->func->on(subscription, type, param))

#endif // __SNORLAX__COMMAND_EVENT_SUBSCRITON__H__
