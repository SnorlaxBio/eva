/**
 * @file        snorlax/event/subscription/event/queue.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 17, 2024
 */

#include "queue.h"

#include "../event.h"

typedef event_subscription_event_t * (*event_subscription_event_queue_func_push_t)(event_subscription_event_queue_t *, event_subscription_event_t *);
typedef event_subscription_event_t * (*event_subscription_event_queue_func_pop_t)(event_subscription_event_queue_t *);
typedef event_subscription_event_t * (*event_subscription_event_queue_func_del_t)(event_subscription_event_queue_t *, event_subscription_event_t *);

static event_subscription_event_queue_t * event_subscription_event_queue_func_rem(event_subscription_event_queue_t * queue);

static event_subscription_event_queue_func_t func = {
    event_subscription_event_queue_func_rem,
    (event_subscription_event_queue_func_push_t) queue_func_push,
    (event_subscription_event_queue_func_pop_t) queue_func_pop,
    (event_subscription_event_queue_func_del_t) queue_func_del,
};

extern event_subscription_event_queue_t * event_subscription_event_queue_gen(void) {
    event_subscription_event_queue_t * queue = (event_subscription_event_queue_t *) calloc(1, sizeof(event_subscription_event_queue_t));

    queue->func = &func;

    return queue;
}

static event_subscription_event_queue_t * event_subscription_event_queue_func_rem(event_subscription_event_queue_t * queue) {
    object_lock(queue);
    
    event_subscription_event_t * event = queue->head;
    while(event) {
        queue->head = queue->head->next;
        event = event_subscription_event_rem(event);
        event = queue->head;
    }

    object_unlock(queue);

    queue->sync = sync_rem(queue->sync);

    free(queue);
    
    return nil;
}