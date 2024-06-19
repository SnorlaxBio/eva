/**
 * @file        snorlax/event/subscription.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 18, 2024
 */

#include <stdlib.h>

#include "subscription.h"

#include "../../event/subscription/event/queue.h"

command_event_subscription_func_t func = {
    command_event_subscription_func_rem,
    command_event_subscription_func_on
};

extern command_event_subscription_t * command_event_subscription_gen(command_t * command, uint32_t status) {
    command_event_subscription_t * subscription = (command_event_subscription_t *) calloc(1, sizeof(command_event_subscription_t));

    subscription->func = &func;

    subscription->queue = event_subscription_event_queue_gen();

    subscription->status = status;
    subscription->command = command;

    return subscription;
}

extern command_event_subscription_t * command_event_subscription_func_rem(command_event_subscription_t * subscription) {
    if(subscription) {
        object_lock(subscription);
        subscription->queue = event_subscription_event_queue_rem(subscription->queue);
        object_unlock(subscription);

        subscription->sync = sync_rem(subscription->sync);

        free(subscription);
    }
    
    return nil;
}

extern void command_event_subscription_func_on(command_event_subscription_t * subscription, uint32_t type, uint64_t ret) {
    // TODO: IMPLEMENT THIS
}

extern command_t * command_event_subscription_command_get(command_event_subscription_t * subscription) {
    return subscription ? subscription->command : nil;
}