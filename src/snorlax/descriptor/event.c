/**
 * @file        snorlax/descriptor/event.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 20, 2024
 */

#include <stdlib.h>

#include "event.h"

#include "../event/subscription/event.h"

static descriptor_event_func_t func = {
    descriptor_event_func_rem,
    descriptor_event_func_on
};

extern descriptor_event_t * descriptor_event_gen(___notnull descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, "critical", "");
#endif // RELEASE

    descriptor_event_t * event = (descriptor_event_t *) calloc(1, sizeof(descriptor_event_t));

    event->func = &func;

    event->subscription = subscription;
    event->type = type;
    event->param = param;

    event->node = event_subscription_event_gen((event_t *) event);

    return event;
}

extern descriptor_event_t * descriptor_event_func_rem(___notnull descriptor_event_t * event) {

}

extern void descriptor_event_func_on(___notnull descriptor_event_t * event) {
#ifndef   RELEASE
    snorlaxdbg(event == nil, "critical", "");
#endif // RELEASE
    
    descriptor_event_subscription_t * subscription = event->subscription;
    uint32_t type = event->type;
    uint64_t param = event->param;

    descriptor_event_subscription_action_t func = descriptor_event_subscription_action_get(type);

#ifndef   RELEASE
    snorlaxdbg(func == nil, "critical", "");
#endif // RELEASE

    func(subscription, param);

    event = descriptor_event_rem(event);
}
