/**
 * @file        snorlax/event/generator.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 21, 2024
 */

#include "generator.h"
#include "subscription.h"

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
