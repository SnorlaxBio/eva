/**
 * @file        snorlax/event.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 17, 2024
 */

#include <stdlib.h>

#include "event.h"

static event_t * event_func_rem(event_t * event, event_callback_t f);
static int32_t event_func_on(event_t * event);

static event_func_t func = {
    event_func_rem,
    event_func_on
};

extern event_t * event_gen(event_subscription_t * subscription, uint32_t type, event_handler_t on, event_t * event) {
    if(event == nil) {
        event = (event_t *) calloc(1, sizeof(event_t));

        event->func = &func;
    }

    event->subscription = subscription;
    event->type = type;
    event->on = on;

    return event;
}

static event_t * event_func_rem(event_t * event, event_callback_t f) {
    if(f) f(event);

    free(event);

    return nil;
}

static int32_t event_func_on(event_t * event) {
    event->on(event->subscription, event->type, nil);
    return success;
}