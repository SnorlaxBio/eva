/**
 * @file        snorlax/descriptor/event/subscription.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 25, 2024
 */

#include "subscription.h"

#include "generator.h"
#include "subscription/type.h"

#include "type.h"

#include "../../event/subscription/event/queue.h"

static descriptor_event_subscription_t * descriptor_event_subscription_func_rem(___notnull descriptor_event_subscription_t * subscription);
static void descriptor_event_subscription_func_on(___notnull descriptor_event_subscription_t * subscription, descriptor_event_subscription_process_t process, uint32_t type, event_subscription_event_t * node);
static void descriptor_event_subscription_func_notify(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node);

static descriptor_event_subscription_func_t func = {
    descriptor_event_subscription_func_rem,
    descriptor_event_subscription_func_on,
    descriptor_event_subscription_func_notify
};

extern descriptor_event_subscription_t * descriptor_event_subscription_gen(___notnull descriptor_t * descriptor, descriptor_event_subscription_handler_t * handler) {
#ifndef   RELEASE
    snorlaxdbg(descriptor == nil, false, "critical", "");
#endif // RELEASE

    descriptor_event_subscription_t * subscription = (descriptor_event_subscription_t *) calloc(1, sizeof(descriptor_event_subscription_t));

    subscription->func = address_of(func);
    subscription->queue = event_subscription_event_queue_gen();
    subscription->handler = (descriptor_event_subscription_handler_t *) calloc(descriptor_event_type_max, sizeof(descriptor_event_subscription_handler_t));
    subscription->descriptor = descriptor;
    subscription->type = event_subscription_type_descriptor;

    if(handler) {
        for(uint32_t i = 0; i < descriptor_event_type_max; i++) {
            subscription->handler[i] = handler[i];
            printf("%p\n", subscription->handler[i]);
        }
    }

    return subscription;
}

static descriptor_event_subscription_t * descriptor_event_subscription_func_rem(___notnull descriptor_event_subscription_t * subscription) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
#endif // RELEASE

    descriptor_event_generator_t * generator = subscription->generator;

    if(generator) descriptor_event_generator_del(generator, subscription);

    event_subscription_event_queue_clear(subscription->queue);

    descriptor_event_subscription_notify(subscription, descriptor_event_type_subscription, (event_subscription_event_t *) descriptor_event_subscription_type_rem);

    subscription->sync = sync_rem(subscription->sync);

    subscription->handler = memory_rem(subscription->handler);
    subscription->queue = event_subscription_event_queue_rem(subscription->queue);

    free(subscription);

    return nil;
}

static void descriptor_event_subscription_func_on(___notnull descriptor_event_subscription_t * subscription, descriptor_event_subscription_process_t process, uint32_t type, event_subscription_event_t * node) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
    snorlaxdbg(process == nil, false, "critical", "");
#endif // RELEASE

    process(subscription, type, node);
}

static void descriptor_event_subscription_func_notify(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
#endif // RELEASE

    descriptor_event_subscription_handler_t on = subscription->handler[type];

    if(on) {
        on(subscription, type, node);
    }
}
