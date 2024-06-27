/**
 * @file        snorlax/descriptor/event/generator/epoll/subscription/process.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 26, 2024
 */

#include "process.h"

#include "../control/type.h"
#include "../../epoll.h"

#include "../../../generator.h"

#include "../../../../../event/subscription/event.h"
#include "../../../../../event/generator/set.h"
#include "../../../../../event/engine.h"
#include "../../../../../event/queue.h"
#include "../../../type.h"
#include "../../../subscription.h"

static descriptor_event_subscription_process_t process[descriptor_event_type_max] = {
    descriptor_event_subscription_process_subscription,
    descriptor_event_subscription_process_open,
    descriptor_event_subscription_process_read,
    descriptor_event_subscription_process_write,
    descriptor_event_subscription_process_close,
    descriptor_event_subscription_process_exception
};

extern event_subscription_process_t descriptor_event_generator_epoll_subscription_process_get(uint32_t type) {
#ifndef   RELEASE
    snorlaxdbg(type == descriptor_event_type_subscription, false, "critical", "");
    snorlaxdbg(descriptor_event_type_max <= type, false, "critical", "");
#endif // RELEASE

    return (event_subscription_process_t) process[type];
}

extern void descriptor_event_subscription_process_subscription(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_node_t * node) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
#endif // RELEASE

    // TODO: CHECK THIS METHOD ... 구현할 필요 없어 보인다....
}

extern void descriptor_event_subscription_process_open(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_node_t * node) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
    snorlaxdbg(subscription->descriptor == nil, false, "critical", "");
    snorlaxdbg(subscription->generator == nil, false, "critical", "");

    snorlaxdbg(descriptor_exception_get(subscription->descriptor), false, "critical", "");
    snorlaxdbg(subscription->descriptor->status & descriptor_state_close, false, "critical", "");
#endif // RELEASE

    descriptor_t * descriptor = subscription->descriptor;
    descriptor_event_generator_epoll_t * generator = (descriptor_event_generator_epoll_t *) subscription->generator;
    event_generator_set_t * set = generator->set;
    event_engine_t * engine = set->engine;

    if(descriptor->value <= invalid) {
        if(descriptor_open(descriptor) == success) {
            if(descriptor->status & descriptor_state_open) {
                descriptor_event_subscription_notify(subscription, descriptor_event_type_open, node);
            }

            if(descriptor_exception_get(descriptor) == nil && (descriptor->status & descriptor_state_close) == 0) {
                descriptor_event_generator_epoll_control(generator, subscription, descriptor_event_generator_epoll_control_type_add);
            }
        }
    } else if((descriptor->status & descriptor_state_open) == 0) {
        if(descriptor_check(descriptor, descriptor_state_open)) {
            descriptor_event_subscription_notify(subscription, descriptor_event_type_open, node);
            if(descriptor_exception_get(descriptor) == nil && (descriptor->status & descriptor_state_close) == 0) {
                descriptor_event_subscription_process_read(subscription, descriptor_event_type_read, node);
            }
        } else {
            event_queue_push(engine->queue, event_gen((event_subscription_t *) subscription,
                                                      (event_subscription_process_t) descriptor_event_subscription_process_open,
                                                      descriptor_event_type_open,
                                                      event_subscription_event_gen((event_subscription_t *) subscription)));
        }
    } else {
#ifndef   RELEASE
        snorlaxdbg(false, true, "caution", "");
#endif // RELEASE
    }

    if(descriptor_exception_get(descriptor)) {
        descriptor_event_subscription_process_exception(subscription, type, node);
    } else if(descriptor->status & descriptor_state_close) {
        descriptor_event_subscription_process_close(subscription, type, node);
    }
}

extern void descriptor_event_subscription_process_read(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_node_t * node) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
    snorlaxdbg(subscription->descriptor == nil, false, "critical", "");
#endif // RELEASE

    descriptor_t * descriptor = subscription->descriptor;
    descriptor_event_generator_epoll_t * generator = (descriptor_event_generator_epoll_t *) subscription->generator;
    event_generator_set_t * set = generator->set;
    event_engine_t * engine = set->engine;

    int32_t retry = 4;

    while(retry > 0 && descriptor_exception_get(descriptor) == nil && (descriptor->status & descriptor_state_close) == 0) {
        int64_t n = descriptor_read(descriptor);

        if(n <= 0) break;

        descriptor_event_subscription_notify(subscription, descriptor_event_type_read, event_subscription_event_parameter_set(node, n));

        while(buffer_length(descriptor->buffer.out) > 0 && descriptor_exception_get(descriptor) == nil && (descriptor->status & descriptor_state_close) == 0) {
            int64_t n = descriptor_write(descriptor);

            if(n <= 0) break;

            descriptor_event_subscription_notify(subscription, descriptor_event_type_write, event_subscription_event_parameter_set(node, n));
        }

        retry = retry - 1;
    }

    if(descriptor_exception_get(descriptor)) {
        descriptor_event_subscription_process_exception(subscription, type, node);
    } else if(descriptor->status & descriptor_state_close) {
        descriptor_event_subscription_process_close(subscription, type, node);
    } else {
        buffer_adjust(descriptor->buffer.in, 0);
        buffer_adjust(descriptor->buffer.out, 0);

        if(descriptor->status & descriptor_state_read) {
            event_queue_push(engine->queue, event_gen((event_subscription_t *) subscription,
                                                      (event_subscription_process_t) descriptor_event_subscription_process_read,
                                                      descriptor_event_type_read,
                                                      event_subscription_event_gen((event_subscription_t *) subscription)));
        } else {
            descriptor_event_generator_epoll_control(generator, subscription, descriptor_event_generator_epoll_control_type_mod);
        }
    }
}

extern void descriptor_event_subscription_process_write(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_node_t * node) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
    snorlaxdbg(subscription->descriptor == nil, false, "critical", "");
#endif // RELEASE

    descriptor_t * descriptor = subscription->descriptor;
    descriptor_event_generator_epoll_t * generator = (descriptor_event_generator_epoll_t *) subscription->generator;
    event_generator_set_t * set = generator->set;
    event_engine_t * engine = set->engine;
    buffer_t * buffer = descriptor->buffer.out;

    while(buffer_length(buffer) > 0 && descriptor_exception_get(descriptor) == nil && (descriptor->status & descriptor_state_close) == 0) {
        int64_t n = descriptor_write(descriptor);

        if(n <= 0) break;

        descriptor_event_subscription_notify(subscription, descriptor_event_type_write, event_subscription_event_parameter_set(node, n));
    }

    if(descriptor_exception_get(descriptor)) {
        descriptor_event_subscription_process_exception(subscription, type, node);
    } else if(descriptor->status & descriptor_state_close) {
        descriptor_event_subscription_process_close(subscription, type, node);
    } else {
        buffer_adjust(descriptor->buffer.in, 0);
        buffer_adjust(descriptor->buffer.out, 0);

        if(descriptor->status & descriptor_state_write) {
            if(buffer_length(descriptor->buffer.out) > 0) {
                event_queue_push(engine->queue, event_gen((event_subscription_t *) subscription,
                                                          (event_subscription_process_t) descriptor_event_subscription_process_write,
                                                          descriptor_event_type_write,
                                                          event_subscription_event_gen((event_subscription_t *) subscription)));
            }
        } else {
            descriptor_event_generator_epoll_control(generator, subscription, descriptor_event_generator_epoll_control_type_mod);
        }
    }
}

extern void descriptor_event_subscription_process_close(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_node_t * node) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
    snorlaxdbg(subscription->descriptor == nil, false, "critical", "");
#endif // RELEASE

    int value = subscription->descriptor->value;

    descriptor_t * descriptor = subscription->descriptor;
    descriptor_event_generator_epoll_t * generator = (descriptor_event_generator_epoll_t *) subscription->generator;

    if(value > invalid) {
        descriptor_event_generator_epoll_control(generator, subscription, descriptor_event_generator_epoll_control_type_del);
    }

    descriptor_close(subscription->descriptor);

    descriptor_event_subscription_notify(subscription, descriptor_event_type_close, event_subscription_event_parameter_set(node, value));

    buffer_reset(descriptor->buffer.in, 0);
    buffer_reset(descriptor->buffer.out, 0);
}

extern void descriptor_event_subscription_process_exception(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_node_t * node) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
    snorlaxdbg(subscription->descriptor == nil, false, "critical", "");
    snorlaxdbg(descriptor_exception_get(subscription->descriptor) == nil, false, "critical", "");
#endif // RELEASE

    descriptor_event_subscription_notify(subscription, descriptor_event_type_exception, event_subscription_event_parameter_set(node, descriptor_exception_get(subscription->descriptor)));

    int value = subscription->descriptor->value;

    descriptor_t * descriptor = subscription->descriptor;
    descriptor_event_generator_epoll_t * generator = (descriptor_event_generator_epoll_t *) subscription->generator;

    if(value > invalid) {
        descriptor_event_generator_epoll_control(generator, subscription, descriptor_event_generator_epoll_control_type_del);
    }

    descriptor_close(subscription->descriptor);

    descriptor_event_subscription_notify(subscription, descriptor_event_type_close, event_subscription_event_parameter_set(node, value));

    buffer_reset(descriptor->buffer.in, 0);
    buffer_reset(descriptor->buffer.out, 0);
}