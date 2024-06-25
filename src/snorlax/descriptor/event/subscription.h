/**
 * @file        snorlax/descriptor/event/subscription.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 22, 2024
 */

#ifndef   __SNORLAX__DESCRIPTOR_EVENT_SUBSCRIPTION__H__
#define   __SNORLAX__DESCRIPTOR_EVENT_SUBSCRIPTION__H__

#include <snorlax/descriptor/event.h>

struct descriptor_event_subscription {
    descriptor_event_subscription_func_t * func;
    sync_t * sync;
    descriptor_event_generator_t * generator;
    descriptor_event_subscription_t * prev;
    descriptor_event_subscription_t * next;
    event_subscription_event_queue_t * queue;
    descriptor_event_subscription_handler_t * handler;

    descriptor_t * descriptor;
};

struct descriptor_event_subscription_func {
    descriptor_event_subscription_t * (*rem)(___notnull descriptor_event_subscription_t *);
    void (*on)(___notnull descriptor_event_subscription_t *, uint32_t, event_subscription_event_t *);
};

extern descriptor_event_subscription_t * descriptor_event_subscription_gen(descriptor_t * descriptor, descriptor_event_subscription_handler_t * handler);

#define descriptor_event_subscription_rem(subscription)                 ((subscription)->func->rem(subscription))
#define descriptor_event_subscription_on(subscription, type, event)     ((subscription)->func->on(subscription, type, event))

#endif // __SNORLAX__DESCRIPTOR_EVENT_SUBSCRIPTION__H__
