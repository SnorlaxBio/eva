/**
 * @file        snorlax/descriptor/event/subscription.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 19, 2024
 */

#ifndef   __SNORLAX__DESCRIPTOR_EVENT_SUBSCRIPTION__H__
#define   __SNORLAX__DESCRIPTOR_EVENT_SUBSCRIPTION__H__

#include <snorlax/descriptor.h>

struct descriptor_event_subscription {
    descriptor_event_subscription_func_t * func;
    sync_t * sync;

    descriptor_event_generator_t * generator;
    descriptor_event_subscription_t * prev;
    descriptor_event_subscription_t * next;

    event_subscription_event_queue_t * queue;

    uint32_t status;
    uint32_t interest;

    descriptor_t * descriptor;
};

struct descriptor_event_subscription_func {
    descriptor_event_subscription_t * (*rem)(descriptor_event_subscription_t *);
    void (*on)(descriptor_event_subscription_t *, uint32_t, uint64_t);
};

extern descriptor_event_subscription_t * descriptor_event_subscription_gen(descriptor_t * descriptor, uint32_t status);

extern descriptor_event_subscription_t * descriptor_event_subscription_func_rem(___notnull descriptor_event_subscription_t * subscription);
extern void descriptor_event_subscription_func_on(descriptor_event_subscription_t * subscription, uint32_t type, uint64_t ret);

extern void descriptor_event_subscription_descriptor_exception_set(___notnull descriptor_event_subscription_t * subscription, uint32_t type, void * func, uint32_t no);

#define descriptor_event_subscription_rem(subscription)             ((subscription)->func->rem(subscription))
#define descriptor_event_subscription_on(subscription, type, ret)   ((subscription)->func->on(subscription, type, ret))

#endif // __SNORLAX__DESCRIPTOR_EVENT_SUBSCRIPTION__H__
