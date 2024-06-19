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

#include <snorlax/eva.h>

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
    void (*on)(command_event_subscription_t *, uint32_t, uint64_t);
};

extern command_event_subscription_t * command_event_subscription_gen(command_t * command, uint32_t status);

extern command_event_subscription_t * command_event_subscription_func_rem(command_event_subscription_t * subscription);
extern void command_event_subscription_func_on(command_event_subscription_t * subscription, uint32_t type, uint64_t ret);

#define command_event_subscription_rem(subscription)            ((subscription)->func->rem(subscription))
#define command_event_subscription_on(subscription, type, ret)  ((subscription)->func->on(subscription, type, ret))

#endif // __SNORLAX__COMMAND_EVENT_SUBSCRIPTION__H__
