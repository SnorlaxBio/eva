/**
 * @file        snorlax/descriptor/event/subscription.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 19, 2024
 */

#include "subscription.h"

static descriptor_event_subscription_func_t func = {
    descriptor_event_subscription_func_rem,
    descriptor_event_subscription_func_on
};

extern descriptor_event_subscription_t * descriptor_event_subscription_gen(descriptor_t * descriptor, uint32_t status) {
    descriptor_event_subscription_t * subscription = (descriptor_event_subscription_t *) calloc(1, sizeof(descriptor_event_subscription_t));

    subscription->func = &func;

    subscription->queue = event_subscription_event_queue_gen();

    subscription->status = status;
    subscription->descriptor = descriptor;

    return subscription;
}

extern descriptor_event_subscription_t * descriptor_event_subscription_func_rem(___notnull descriptor_event_subscription_t * subscription) {
    object_lock(subscription);
    subscription->queue = event_subscription_event_queue_rem(subscription->queue);
    object_unlock(subscription);

    subscription->sync = sync_rem(subscription->sync);

    free(subscription);
    
    return nil;
}

extern void descriptor_event_subscription_func_on(descriptor_event_subscription_t * subscription, uint32_t type, uint64_t ret) {
    // TODO: IMPLEMENT THIS
    snorlaxdbg(true, "implement", "");
}

extern void descriptor_event_subscription_descriptor_exception_set(___notnull descriptor_event_subscription_t * subscription, uint32_t type, void * func, uint32_t no) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, "critical", "");
    snorlaxdbg(subscription->descriptor == nil, "critical", "");
#endif // RELEASE

    descriptor_exception_set(address_of(subscription->descriptor->exception), type, func, no);

    descriptor_event_subscription_func_on(subscription, descriptor_event_type_exception, 0);
}
