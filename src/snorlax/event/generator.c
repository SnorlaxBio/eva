/**
 * @file        snorlax/event/generator.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 21, 2024
 */

#include "generator.h"

#include "generator/control/type.h"

#include "type.h"

#include "subscription.h"
#include "subscription/type.h"
#include "subscription/event.h"
#include "subscription/event/queue.h"

extern int32_t event_generator_func_add(___notnull event_generator_t * generator, ___notnull event_subscription_t * subscription) {
#ifndef   RELEASE
    snorlaxdbg(generator == nil, false, "critical", "");
    snorlaxdbg(subscription == nil, false, "critical", "");
    snorlaxdbg(subscription->generator, false, "critical", "");
#endif // RELEASE

    if(generator->tail) {
        generator->tail->next = subscription;
        subscription->prev = generator->tail;
    } else {
        generator->head = subscription;
    }

    subscription->generator = generator;

    generator->size = generator->size + 1;

    return success;
}

extern int32_t event_generator_func_del(___notnull event_generator_t * generator, ___notnull event_subscription_t * subscription) {
#ifndef   RELEASE
    snorlaxdbg(generator == nil, false, "critical", "");
    snorlaxdbg(subscription == nil, false, "critical", "");
    snorlaxdbg(subscription->generator != generator, false, "critical", "");
#endif // RELEASE

    event_subscription_t * prev = subscription->prev;
    event_subscription_t * next = subscription->next;

    if(prev) {
        prev->next = next;
        subscription->prev = nil;
    } else {
        generator->head = next;
    }

    if(next) {
        next->prev = prev;
        subscription->next = nil;
    } else {
        generator->tail = prev;
    }

    subscription->generator = nil;
    
    generator->size = generator->size - 1;

    return success;
}

extern void event_generator_func_clear(___notnull event_generator_t * generator) {
#ifndef   RELEASE
    snorlaxdbg(generator == nil, false, "critical", "");
#endif // RELEASE

    object_lock(generator);
    event_subscription_t * subscription = generator->head;
    while(subscription) {
        object_lock(subscription);
        event_generator_del(generator, subscription);
        event_subscription_event_queue_t * queue = subscription->queue;
        event_subscription_event_t * node = queue->head;
        while(node) {
            queue->head = node->next;
            if(queue->head == nil) {
                queue->tail = nil;
            } else {
                node->next = nil;
            }
            node->queue = nil;
            queue->size = queue->size - 1;

            event_generator_control(generator, subscription, event_generator_control_type_del, node);

            node = queue->head;
        }

        event_subscription_on(subscription, event_type_subscription, (event_subscription_event_t *) event_subscription_type_del);

        object_unlock(subscription);

        subscription = generator->head;
    }
    object_unlock(generator);
}
