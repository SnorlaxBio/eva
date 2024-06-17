/**
 * @file        snorlax/event/queue.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 17, 2024
 */

#include "queue.h"

#include "../event.h"

static event_queue_t * event_queue_func_rem(event_queue_t * queue, event_callback_t func);

static event_queue_func_t func = {
    event_queue_func_rem
};

extern event_queue_t * event_queue_gen(void) {
    event_queue_t * queue = (event_queue_t *) calloc(1, sizeof(event_queue_t));

    queue->func = &func;

    return queue;
}

extern event_t * event_queue_func_push(event_queue_t * queue, event_t * event) {
    object_lock(queue);

    if(queue->tail) {
        queue->tail->next = event;
        event->prev = queue->tail;
    } else {
        queue->head = event;
    }

    queue->tail = event;
    queue->size = queue->size + 1;

    object_unlock(queue);

    object_wakeup(queue, false);

    return event;
}

extern event_t * event_queue_func_pop(event_queue_t * queue) {    
    event_t * event = queue->head;

    if(event) {
        queue->head = event->next;
        if(queue->head == nil) queue->tail = nil;
        queue->size = queue->size - 1;

        event->next = nil;
        event->queue = nil;
    }

    return event;
}

static event_queue_t * event_queue_func_rem(event_queue_t * queue, event_callback_t func) {
    object_lock(queue);

    event_t * event = queue->head;
    while(event) {
        queue->head = queue->head->next;

        event->next = nil;
        event->queue = nil;
        event = event_rem(event, func);
        event = queue->head;
    }

    object_unlock(queue);

    queue->sync = sync_rem(queue->sync);

    free(queue);

    return nil;
}
