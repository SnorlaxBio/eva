/**
 * @file        snorlax/event/queue.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 21, 2024
 * @version     0.0.1
 */

#include "queue.h"

static ___sync event_queue_t * event_queue_func_rem(___notnull event_queue_t * queue);
static ___sync void event_queue_func_push(___notnull event_queue_t * queue, ___notnull event_t * event);
static event_t * event_queue_func_pop(___notnull event_queue_t * queue);

static event_queue_func_t func = {
    event_queue_func_rem,
    event_queue_func_push,
    event_queue_func_pop
};

extern event_queue_t * event_queue_gen(sync_t * sync) {
    event_queue_t * queue = (event_queue_t *) calloc(1, sizeof(event_queue_t));

    queue->func = address_of(func);
    queue->sync = sync;

    return queue;
}

static ___sync event_queue_t * event_queue_func_rem(___notnull event_queue_t * queue) {
#ifndef   RELEASE
    snorlaxdbg(queue == nil, "critical", "");
    snorlaxdbg(queue->size > 0, "critical", "");
    snorlaxdbg(queue->head, "critical", "");
    snorlaxdbg(queue->tail, "critical", "");
#endif // RELEASE

    queue->sync = sync_rem(queue->sync);

    free(queue);

    return nil;
}

static ___sync void event_queue_func_push(___notnull event_queue_t * queue, ___notnull event_t * event) {
#ifndef   RELEASE
    snorlaxdbg(queue == nil, "critical", "");
    snorlaxdbg(event == nil, "critical", "");
    snorlaxdbg(event->queue, "critical", "");
#endif // RELEASE

    object_lock(queue);

    if(queue->tail) {
        queue->tail->next = event;
        event->prev = queue->tail;
    } else {
        queue->head = event;
    }

    queue->tail = event;
    queue->size = queue->size + 1;

    event->queue = queue;

    object_unlock(queue);

    object_wakeup(queue, false);
}

static event_t * event_queue_func_pop(___notnull event_queue_t * queue) {
    event_t * event = queue->head;

    if(event) {
        queue->head = event->next;
        if(queue->head){
            queue->head->prev = nil;
            event->next = nil;
        } else {
            queue->tail = nil;
        }

        event->queue = nil;
        queue->size = queue->size - 1;
    }

    return event;
}
