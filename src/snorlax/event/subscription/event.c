/**
 * @file        snorlax/event/subscription/event.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 21, 2024
 * @version     0.0.1
 */

#include <stdlib.h>

#include "event.h"

#include "event/queue.h"

#include "../subscription.h"

#include "../queue.h"

static event_subscription_event_func_t func = {
    event_subscription_event_func_rem
};

extern event_subscription_event_t * event_subscription_event_gen(event_subscription_t * subscription) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
#endif // RELEASE

    event_subscription_event_t * event = (event_subscription_event_t *) calloc(1, sizeof(event_subscription_event_t));

    event->func = address_of(func);

    event_subscription_event_queue_add(subscription->queue, event);
    
    return event;
}

extern event_subscription_event_t * event_subscription_event_func_rem(___notnull event_subscription_event_t * event) {
#ifndef   RELEASE
    snorlaxdbg(event == nil, false, "critical", "");
#endif // RELEASE

    if(event->queue) event_subscription_event_queue_del(event->queue, event);

    if(event->origin) {
        event->origin->node = nil;

        event_on(event->origin);
    }

    event->sync = sync_rem(event->sync);

    free(event);

    return nil;
}
