/**
 * @file        snorlax/event.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 21, 2024
 */

#include <stdlib.h>

#include "event.h"
#include "event/queue.h"
#include "event/subscription.h"
#include "event/subscription/event.h"

static event_t * event_func_rem(___notnull event_t * event);

static event_func_t func = {
    event_func_rem,
    event_func_on
};

extern event_t * event_gen(___notnull event_subscription_t * subscription, uint32_t type, ___notnull event_subscription_event_t * node) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
    snorlaxdbg(node == nil, false, "critical", "");
#endif // RELEASE

    event_t * event = (event_t *) calloc(1, sizeof(event_t));

    event->func = address_of(func);

    event->subscription = subscription;
    event->type = type;
    event->node = node;

    node->origin = event;

    return event;
}

static event_t * event_func_rem(___notnull event_t * event) {
#ifndef   RELEASE
    snorlaxdbg(event == nil, false, "critical", "");
#endif // RELEASE

    if(event->queue) event_queue_del(event->queue, event);

    if(event->node){
        event->node->origin = nil;

        event->node = event_subscription_event_rem(event->node);
    }
    
    event->sync = sync_rem(event->sync);

    free(event);

    return nil;
}

extern void event_func_on(___notnull event_t * event) {
#ifndef   RELEASE
    snorlaxdbg(event == nil, false, "critical", "");
    snorlaxdbg(event->subscription == nil, false, "critical", "");
    snorlaxdbg(event->node == nil, false, "critical", "");
#endif // RELEASE

    object_lock(event->subscription);
    event_subscription_on(event->subscription, event->type, (uint64_t) event->node);
    object_unlock(event->subscription);

    event_rem(event);
}
