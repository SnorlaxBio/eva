/**
 * @file        snorlax/descriptor/event/subscription.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 19, 2024
 */

#include "subscription.h"

#include "../event.h"

#include "../../event/engine.h"
#include "../../event/queue.h"

static descriptor_event_subscription_func_t func = {
    descriptor_event_subscription_func_rem,
    descriptor_event_subscription_func_on
};

/**
 * @fn          extern descriptor_event_subscription_t * descriptor_event_subscription_gen(___notnull descriptor_t * descriptor, descriptor_event_handler_t * handler)
 */
extern descriptor_event_subscription_t * descriptor_event_subscription_gen(___notnull descriptor_t * descriptor, descriptor_event_handler_t * handler) {
#ifndef   RELEASE
    snorlaxdbg(descriptor == nil, "critical", "");
#endif // RELEASE

    descriptor_event_subscription_t * subscription = (descriptor_event_subscription_t *) calloc(1, sizeof(descriptor_event_subscription_t));

    subscription->func = &func;

    subscription->queue = event_subscription_event_queue_gen();

    subscription->descriptor = descriptor;

    subscription->descriptor->status = subscription->descriptor->status | descriptor_state_subscription_gen;

    subscription->handler = calloc(descriptor_action_max, sizeof(descriptor_event_handler_t));

    if(handler) {
        for(uint32_t i = 0; i < descriptor_action_max; i++) {
            subscription->handler[i] = handler[i];
            if(handler[i]) {
                subscription->interest = subscription->interest | descriptor_event_subscription_state_get(i);
            }
        }
    }

    return subscription;
}

/**
 * @fn          extern descriptor_event_subscription_t * descriptor_event_subscription_func_rem(___notnull descriptor_event_subscription_t * subscription)
 */
extern descriptor_event_subscription_t * descriptor_event_subscription_func_rem(___notnull descriptor_event_subscription_t * subscription) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, "critical", "");
#endif // RELEASE

    object_lock(subscription);
    subscription->queue = event_subscription_event_queue_rem(subscription->queue);

    free(subscription->handler);

    if(subscription->descriptor) {
        subscription->descriptor->status = subscription->descriptor->status | (~descriptor_state_subscription_all);
    }
    object_unlock(subscription);

    subscription->sync = sync_rem(subscription->sync);

    free(subscription);
    
    return nil;
}

extern void descriptor_event_subscription_func_on(descriptor_event_subscription_t * subscription, uint32_t type, uint64_t ret) {
    descriptor_event_handler_t handler = subscription->handler[type];
    if(handler) {
        handler(subscription, type, ret);
    } else {
        handler = descriptor_event_handler_default_get(type);
        if(handler) handler(subscription, type, ret);
    }
}

extern void descriptor_event_subscription_descriptor_exception_set(___notnull descriptor_event_subscription_t * subscription, uint32_t type, void * func, uint32_t no) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, "critical", "");
    snorlaxdbg(subscription->descriptor == nil, "critical", "");
#endif // RELEASE

    descriptor_exception_set(address_of(subscription->descriptor->exception), type, func, no);

    descriptor_event_subscription_func_on(subscription, descriptor_event_type_exception, 0);
}

extern int32_t descriptor_event_subscription_dispatch(___notnull descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param, event_queue_t * queue) {
    if(event_engine_pool_size() == 0) {
        descriptor_event_handler_t on = descriptor_event_handler_get(type);

        on(subscription, type, param);
    } else {
        if(queue) {
            object_lock(queue);
            event_queue_push(queue, (event_t *) descriptor_event_gen(subscription, type, param));
            object_unlock(queue);

            object_wakeup(queue, false);
        } else {
            event_engine_push((event_t *) descriptor_event_gen(subscription, type, param));
        }
    }

    return success;
}
