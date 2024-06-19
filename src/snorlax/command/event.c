/**
 * @file        snorlax/command/event.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 18, 2024
 */

#include <stdlib.h>

#include "event.h"

#include "../event.h"
#include "../event/subscription/event.h"

#include "event/subscription.h"
#include "event/generator.h"

static int32_t command_event_func_on(___notnull command_event_t * event);

typedef command_event_t * (*command_event_func_rem_t)(command_event_t *);

static command_event_func_t func = {
    (command_event_func_rem_t) event_func_rem,
    command_event_func_on
};

extern command_event_t * command_event_gen(command_event_subscription_t * subscription, uint32_t type) {
    command_event_t * event = (command_event_t *) calloc(1, sizeof(command_event_t));

    event->func = &func;

    event->subscription = subscription;
    event->type = type;

    event->node = event_subscription_event_gen((event_t *) event);

    return event;
}

static int32_t command_event_func_on(___notnull command_event_t * event) {
    if(event->type == command_event_type_exec) {
        int32_t ret = command_exec(event->subscription->command);

        command_event_subscription_on(event->subscription, command_event_type_exec, ret);

        if(event->subscription->status & command_event_subscription_state_once) {
            command_event_generator_t * generator = event->subscription->generator;

            object_lock(generator);
            command_event_generator_del(generator, event->subscription);
            object_unlock(generator);
        }
    } else {
        // TODO: ASSERTION
    }

    event = command_event_rem(event);

    return success;
}