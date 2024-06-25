/**
 * @file        snorlax/descriptor/event/subscription.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 24, 2024
 */

#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include "subscription.h"

#include "generator.h"
#include "subscription/type.h"

#include "type.h"

#include "../../descriptor.h"

#include "../../event/engine.h"
#include "../../event/generator/set.h"
#include "../../event/subscription/event/queue.h"




static descriptor_event_subscription_t * descriptor_event_subscription_func_rem(___notnull descriptor_event_subscription_t * subscription);
static void descriptor_event_subscription_func_on(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node);

// #define descriptor_event_type_subscription      0
// #define descriptor_event_type_open              1
// #define descriptor_event_type_in                2
// #define descriptor_event_type_out               3
// #define descriptor_event_type_close             4
// #define descriptor_event_type_exception         5
// #define descriptor_event_type_max               6

static event_subscription_event_t * descriptor_event_subscription_processor_func_subscription(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node);
static event_subscription_event_t * descriptor_event_subscription_processor_func_open(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node);
static event_subscription_event_t * descriptor_event_subscription_processor_func_in(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node);
static event_subscription_event_t * descriptor_event_subscription_processor_func_out(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node);
static event_subscription_event_t * descriptor_event_subscription_processor_func_close(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node);
static event_subscription_event_t * descriptor_event_subscription_processor_func_exception(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node);

static descriptor_event_subscription_process_t processor[descriptor_event_type_max] = {
    descriptor_event_subscription_processor_func_subscription,
    descriptor_event_subscription_processor_func_open,
    descriptor_event_subscription_processor_func_in,
    descriptor_event_subscription_processor_func_out,
    descriptor_event_subscription_processor_func_close,
    descriptor_event_subscription_processor_func_exception
};

static void descriptor_event_subscription_subscriber_func_subscription(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node);
static void descriptor_event_subscription_subscriber_func_open(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node);
static void descriptor_event_subscription_subscriber_func_in(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node);
static void descriptor_event_subscription_subscriber_func_out(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node);
static void descriptor_event_subscription_subscriber_func_close(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node);
static void descriptor_event_subscription_subscriber_func_exception(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node);

static descriptor_event_subscription_handler_t subscriber[descriptor_event_type_max] = {
    descriptor_event_subscription_processor_func_subscription,
    descriptor_event_subscription_subscriber_func_open,
    descriptor_event_subscription_subscriber_func_in,
    descriptor_event_subscription_subscriber_func_out,
    descriptor_event_subscription_subscriber_func_close,
    descriptor_event_subscription_subscriber_func_exception
};

static descriptor_event_subscription_func_t func = {
    descriptor_event_subscription_func_rem,
    descriptor_event_subscription_func_on
};

extern descriptor_event_subscription_t * descriptor_event_subscription_gen(descriptor_t * descriptor, descriptor_event_subscription_handler_t * handler) {
    descriptor_event_subscription_t * subscription = (descriptor_event_subscription_t *) calloc(1, sizeof(descriptor_event_subscription_t));

    subscription->func = address_of(func);

    subscription->descriptor = descriptor;
    subscription->queue = event_subscription_event_queue_gen();
    subscription->handler = (descriptor_event_subscription_handler_t *) calloc(descriptor_event_type_max, sizeof(descriptor_event_subscription_handler_t));

    if(handler) {
        for(int32_t i = 0; i < descriptor_event_type_max; i++) {
            subscription->handler[i] = handler[i];
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

    object_lock(subscription);

    // TODO: CHECK THIS LOGIC
    event_subscription_event_queue_clear(subscription->queue);

    descriptor_event_subscription_on(subscription, descriptor_event_type_subscription, (event_subscription_event_t *) descriptor_event_subscription_type_rem);

    subscription->queue = event_subscription_event_queue_rem(subscription->queue);

    object_unlock(subscription);

    subscription->sync = sync_rem(subscription->sync);
    free(subscription->handler);
    free(subscription);

    return nil;
}

static void descriptor_event_subscription_func_on(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
    snorlaxdbg(descriptor_event_type_max <= type, false, "critical", "");
#endif // RELEASE

    uint32_t status = subscription->descriptor->status;
    descriptor_event_subscription_process_t process = processor[type];
    process(subscription, type, node);

    // TODO: UPGRADE
    descriptor_t * descriptor = subscription->descriptor;

    if(type == descriptor_event_type_exception) {
        descriptor_event_subscription_on(subscription, descriptor_event_type_close, node);
    } else {
        if(descriptor_exception_get(descriptor)) {
            descriptor_event_subscription_on(subscription, descriptor_event_type_exception, node);
        } else if(type == descriptor_event_type_close) {
            descriptor_event_subscription_on(subscription, descriptor_event_type_subscription, descriptor_event_subscription_type_del);
        } else {
            if(status != descriptor->status) {
                descriptor_event_subscription_on(subscription, descriptor_event_type_subscription, descriptor_event_subscription_type_mod);
            } else {
                // 이 부분은 별로 마음에 들지 않는다. 참조를 너무 많이 겪는다.
                event_t * e = event_gen(subscription, descriptor_event_type_in, node);
                event_queue_t * queue = subscription->generator->set->engine->queue;
                event_queue_push(queue, e);
            }
        }
    }
    // TODO: UPGRADE
}

/** 
 * 
 * 
 * 내부적으로 호출되는것인가?
 */
static event_subscription_event_t * descriptor_event_subscription_processor_func_subscription(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
#endif // RELEASE


    return node;
}

static event_subscription_event_t * descriptor_event_subscription_processor_func_open(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
#endif // RELEASE

    object_lock(subscription);

    descriptor_t * descriptor = subscription->descriptor;
    int32_t ret = fail;

    if(descriptor->value <= invalid) {
        ret = descriptor_open(descriptor);
        if(ret == fail) {
#ifndef   RELEASE
            snorlaxdbg(descriptor_exception_get(descriptor)==nil, false, "critical", "");
#endif // RELEASE

            object_unlock(subscription);

            return node;
        } else {
            if(descriptor->status & descriptor_state_open) {
                descriptor_event_subscription_handler_t on = subscription->handler[type];
                on = on ? on : subscriber[type];

                on(subscription, type, node);

                object_unlock(subscription);

                return node;
            }
        }
    } else if((descriptor->status & descriptor_state_open) == 0) {
        if(descriptor_check(descriptor, descriptor_state_open) == true) {
            descriptor->status = descriptor->status | descriptor_state_open;

            descriptor_event_subscription_handler_t on = subscription->handler[type];
            on = on ? on : subscriber[type];

            on(subscription, type, node);
        }
    } else {
#ifndef   RELEASE
        snorlaxdbg(false, true, "notice", "already open");
#endif // RELEASE
    }

    object_unlock(subscription);

    return node;
}

static event_subscription_event_t * descriptor_event_subscription_processor_func_in(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
#endif // RELEASE

    int32_t retry = 2;
    descriptor_t * descriptor = subscription->descriptor;

    for(int32_t i = 0; i < retry; i++) {
        int64_t n = descriptor_read(descriptor);
        if(n > 0) {
            descriptor_event_subscription_handler_t on = subscription->handler[type];
            on = on ? on : subscriber[type];

            on(subscription, type, node);
            continue;
        }
        break;
    }

    if(descriptor_exception_get(descriptor) == nil) {
        buffer_t * out = descriptor->buffer.out;

    }

    return node;
}

static event_subscription_event_t * descriptor_event_subscription_processor_func_out(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
#endif // RELEASE

    return node;
}

static event_subscription_event_t * descriptor_event_subscription_processor_func_close(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
#endif // RELEASE

    return node;
}

static event_subscription_event_t * descriptor_event_subscription_processor_func_exception(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
#endif // RELEASE

    return node;
}

static void descriptor_event_subscription_subscriber_func_subscription(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
#endif // RELEASE
}

static void descriptor_event_subscription_subscriber_func_open(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
#endif // RELEASE
}

static void descriptor_event_subscription_subscriber_func_in(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
#endif // RELEASE
}

static void descriptor_event_subscription_subscriber_func_out(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
#endif // RELEASE
}

static void descriptor_event_subscription_subscriber_func_close(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
#endif // RELEASE
}

static void descriptor_event_subscription_subscriber_func_exception(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
#endif // RELEASE
}
