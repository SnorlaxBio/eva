/**
 * @file        snorlax/event/subscription/event.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 17, 2024
 */

#include "event.h"

#include "../../event.h"
#include "../subscription.h"

#include "event/queue.h"

static event_subscription_event_t * event_subscription_event_func_rem(event_subscription_event_t * o);

static event_subscription_event_func_t func = {
    event_subscription_event_func_rem
};

extern event_subscription_event_t * event_subscription_event_gen(event_t * event) {
    event_subscription_event_t * o = (event_subscription_event_t *) calloc(1, sizeof(event_subscription_event_t));

    o->func = &func;
    o->event = event;
    o->event->node = o;

    object_lock(event->subscription->queue);
    event_subscription_event_queue_push(event->subscription->queue, o);
    object_unlock(event->subscription->queue);

    return o;
}

static event_subscription_event_t * event_subscription_event_func_rem(event_subscription_event_t * o) {
    // TODO: CHECK CLEAR?
    free(o);

    return nil;
}