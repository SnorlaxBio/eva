/**
 * @file        snorlax/event/generator.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 18, 2024
 */

#include "generator.h"
#include "subscription.h"

extern int32_t event_generator_func_add(event_generator_t * generator, event_subscription_t * subscription) {
    if(subscription) {
        // TODO: ASSERT
        if(generator->tail) {
            generator->tail->next = subscription;
            subscription->prev = generator->tail;
        } else {
            generator->head = subscription;
        }
        generator->tail = subscription;
        generator->size = generator->size + 1;

        return success;
    }
    return fail;
}

extern int32_t event_generator_func_del(event_generator_t * generator, event_subscription_t * subscription) {
    // TODO: ASSERT
    if(subscription) {
        event_subscription_t * prev = subscription->prev;
        event_subscription_t * next = subscription->next;

        if(prev) {
            prev->next = next;
        } else {
            generator->head = next;
        }

        if(next) {
            next->prev = prev;
        } else {
            generator->tail = prev;
        }

        generator->size = generator->size - 1;

        subscription->prev = nil;
        subscription->next = nil;
        subscription->generator = nil;

        return success;
    }

    return fail;
}