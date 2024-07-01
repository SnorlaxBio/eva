/**
 * @file        snorlax/event/subscription/event/queue.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 21, 2024
 * @version     0.0.1
 */

#include <stdlib.h>

#include "queue.h"
#include "../event.h"

static event_subscription_event_queue_t * event_subscription_event_queue_func_rem(___notnull event_subscription_event_queue_t * queue);
static event_subscription_event_t * event_subscription_event_queue_func_add(___notnull event_subscription_event_queue_t * queue, ___notnull event_subscription_event_t * event);
static event_subscription_event_t * event_subscription_event_queue_func_del(___notnull event_subscription_event_queue_t * queue, ___notnull event_subscription_event_t * event);
static void event_subscription_event_queue_func_clear(___notnull event_subscription_event_queue_t * queue);

static event_subscription_event_queue_func_t func = {
    event_subscription_event_queue_func_rem,
    event_subscription_event_queue_func_add,
    event_subscription_event_queue_func_del,
    event_subscription_event_queue_func_clear
};

extern event_subscription_event_queue_t * event_subscription_event_queue_gen(void) {
    event_subscription_event_queue_t * queue = (event_subscription_event_queue_t *) calloc(1, sizeof(event_subscription_event_queue_t));

    queue->func = address_of(func);

    return queue;
}

static event_subscription_event_queue_t * event_subscription_event_queue_func_rem(___notnull event_subscription_event_queue_t * queue) {
#ifndef   RELEASE
    snorlaxdbg(queue == nil, false, "critical", "");
    snorlaxdbg(queue->size > 0, false, "critical", "%lu", queue->size);
    snorlaxdbg(queue->head, false, "critical", "");
    snorlaxdbg(queue->tail, false, "critical", "");
#endif // RELEASE

    queue->sync = sync_rem(queue->sync);

    free(queue);

    return nil;
}

static event_subscription_event_t * event_subscription_event_queue_func_add(___notnull event_subscription_event_queue_t * queue, ___notnull event_subscription_event_t * event) {
#ifndef   RELEASE
    snorlaxdbg(queue == nil, false, "critical", "");
    snorlaxdbg(event == nil, false, "critical", "");
    snorlaxdbg(event->queue, false, "critical", "");
#endif // RELEASE

    if(queue->tail) {
        queue->tail->next = event;
        event->prev = queue->tail;
    } else {
        queue->head = event;
    }

    queue->tail = event;
    queue->size = queue->size + 1;

    event->queue = queue;

    return event;
}

static event_subscription_event_t * event_subscription_event_queue_func_del(___notnull event_subscription_event_queue_t * queue, ___notnull event_subscription_event_t * event) {
#ifndef   RELEASE
    snorlaxdbg(queue == nil, false, "critical", "");
    snorlaxdbg(event == nil, false, "critical", "");
    snorlaxdbg(event->queue != queue, false, "critical", "");
#endif // RELEASE

    event_subscription_event_t * prev = event->prev;
    event_subscription_event_t * next = event->next;

    if(prev) {
        prev->next = next;
        event->prev = nil;
    } else {
        queue->head = next;
    }

    if(next) {
        next->prev = prev;
        event->next = nil;
    } else {
        queue->tail = prev;
    }

    queue->size = queue->size - 1;

    event->queue = nil;

    return event;
}

static void event_subscription_event_queue_func_clear(___notnull event_subscription_event_queue_t * queue) {
#ifndef   RELEASE
    snorlaxdbg(queue == nil, false, "critical", "");
#endif // RELEASE

    while(queue->head) {
        event_subscription_event_t * event = event_subscription_event_queue_func_del(queue, queue->head);
        event_subscription_event_rem(event);
    }
}
