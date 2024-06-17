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

#include "event/queue.h"
#include "event/subscription.h"
#include "event/subscription/event.h"
#include "event/subscription/event/queue.h"

static event_t * event_func_rem(event_t * event);
static int32_t event_func_on(event_t * event);

static event_func_t func = {
    event_func_rem,
    event_func_on
};

extern event_t * event_gen(event_t * event, event_subscription_t * subscription, uint32_t type) {
    if(event == nil) {
        event = (event_t *) calloc(1, sizeof(event_func_t));

        event->func = &func;
    }

    event->subscription =  subscription;
    event->type = type;

    event->node = event_subscription_event_gen(event);

    return event;
}

static event_t * event_func_rem(event_t * event) {
    if(event->node) {
        event_subscription_event_queue_t * queue = event->subscription->queue;

        object_lock(queue);
        event_subscription_event_queue_del(queue, event->node);
        object_unlock(queue);

        event->node = event_subscription_event_rem(event->node);
    }

    free(event);

    return nil;
}

static int32_t event_func_on(event_t * event) {
    return event->subscription->func->on(event->subscription, event->type, event);
}
