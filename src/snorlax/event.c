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

extern event_t * event_func_rem(event_t * event) {
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
