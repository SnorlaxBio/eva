/**
 * @file        snorlax/descriptor/event.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 19, 2024
 */

#include "event.h"

#include "event/handler.h"
#include "event/subscription.h"

#include "../event.h"
#include "../event/subscription/event.h"

typedef descriptor_event_t * (*descriptor_event_func_rem_t)(descriptor_event_t);

static descriptor_event_func_t func = {
    (descriptor_event_func_rem_t) event_func_rem,
    descriptor_event_func_on
};

extern descriptor_event_t * descriptor_event_gen(descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param) {
    descriptor_event_t * event = (descriptor_event_t *) calloc(1, sizeof(descriptor_event_t));

    event->func = &func;

    event->subscription = subscription;
    event->type = type;
    event->param = param;

    event->node = event_subscription_event_gen((event_t *) event);

    return event;
}

extern int32_t descriptor_event_func_on(___notnull descriptor_event_t * event) {
    uint32_t type = event->type;

    descriptor_event_handler_t on = descriptor_event_handler_get(type);

    if(on) {
        on(event->subscription, type, event->param);
    } else {
        descriptor_exception_set(address_of(event->subscription->descriptor->exception), descriptor_action_event_on, descriptor_exception_type_parameter_invalid);
        on = descriptor_event_handler_get(descriptor_event_type_exception);
        on(event->subscription, descriptor_event_type_exception, 0LU);
    }

    event = event_rem(event);

    return success;
}
