/**
 * @file        snorlax/command/event.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 18, 2024
 */

#include "event.h"

#include "../event.h"

#include "event/subscription.h"

static command_event_t * commad_event_func_rem(command_event_t * event);
static int32_t command_event_func_on(command_event_t * event);

static command_event_func_t func = {
    commad_event_func_rem,
    command_event_func_on
};

extern command_event_t * command_event_gen(command_event_t * event, command_event_subscription_t * subscription, uint32_t type) {
    if(event == nil) {
        event = (command_event_t *) calloc(1, sizeof(command_event_t));

        event->func = &func;
    }

    return (command_event_t *) event_gen((event_t *) event, (event_subscription_t *) subscription, type);
}

static command_event_t * commad_event_func_rem(command_event_t * event) {


    return event_rem(event);
}

static int32_t command_event_func_on(command_event_t * event) {
    if(event) {
        if(event->type == command_event_type_exec) {
            command_exec(event->subscription->command);

            if(event->subscription->status & command_event_subscription_state_once) {
                command_event_generator_t * generator = event->subscription->generator;

                object_lock(generator);
                command_event_generator_func_del(generator, event->subscription);
                object_unlock(generator);
            }
        } else {
            // TODO: ASSERTION
        }

        event = command_event_rem(event);

        return success;
    }

    return fail;
}