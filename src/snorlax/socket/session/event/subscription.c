/**
 * @file        snorlax/socket/session/event/subscription.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 28, 2024
 */

#include "subscription.h"
#include "../../../descriptor/event/subscription.h"
#include "../../../descriptor/event/subscription/process.h"
#include "../../../descriptor/event/generator.h"
#include "../../server/event/subscription/list.h"
#include "../../../event/subscription/event/queue.h"
#include "../../../event/subscription/event.h"

typedef void (*socket_session_event_subscription_on_t)(___notnull socket_session_event_subscription_t *, socket_session_event_subscription_process_t, uint32_t, event_subscription_event_t *);
typedef void (*socket_session_event_subscription_notify_t)(___notnull socket_session_event_subscription_t *, uint32_t, event_subscription_event_t *);

static socket_session_event_subscription_t * socket_session_event_subscription_func_rem(___notnull socket_session_event_subscription_t * subscription);
static void socket_session_event_subscription_func_notify(___notnull socket_session_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node);

static socket_session_event_subscription_func_t func = {
    socket_session_event_subscription_func_rem,
    (socket_session_event_subscription_on_t) descriptor_event_subscription_func_on,
    socket_session_event_subscription_func_notify
};

extern socket_session_event_subscription_t * socket_session_event_subscription_gen(socket_session_t * descriptor, socket_session_event_subscription_handler_t * handler, socket_server_event_subscription_list_node_t * node) {
#ifndef   RELEASE
    snorlaxdbg(descriptor == nil, false, "critical", "");
#endif // RELEASE

    socket_session_event_subscription_t * subscription = (socket_session_event_subscription_t *) calloc(1, sizeof(socket_session_event_subscription_t));

    subscription->func = address_of(func);
    subscription->queue = event_subscription_event_queue_gen();
    subscription->handler = handler;
    subscription->descriptor = descriptor;
    subscription->type = event_subscription_type_descriptor;
    subscription->node = node;
    subscription->node->session = subscription;

    return subscription;
}

static void socket_session_event_subscription_func_notify(___notnull socket_session_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
#endif // RELEASE

    socket_session_event_subscription_handler_t on = subscription->handler[type];

    if(on) {
        on(subscription, type, node);
    }

    if(type == descriptor_event_type_close) {
        if(subscription->descriptor->value > invalid) {
            socket_session_event_subscription_func_rem(subscription);
        }
    }
}

static socket_session_event_subscription_t * socket_session_event_subscription_func_rem(___notnull socket_session_event_subscription_t * subscription) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
#endif // RELEASE

    subscription->queue = event_subscription_event_queue_rem(subscription->queue);

    socket_session_t * descriptor = subscription->descriptor;
    if(descriptor) {
        if(descriptor->value > invalid) {
            if(subscription->generator) {
                descriptor_event_generator_del(subscription->generator, (descriptor_event_subscription_t *) subscription);
            }

            int32_t value = descriptor->value;
            descriptor_close(descriptor);

            descriptor_event_subscription_notify(subscription, descriptor_event_type_close, event_subscription_event_parameter_set(nil, value));

            if(descriptor->buffer.in) buffer_reset(descriptor->buffer.in, 0);
            if(descriptor->buffer.out) buffer_reset(descriptor->buffer.out, 0);
        }

        subscription->descriptor = descriptor_rem(descriptor);
    }

    if(subscription->node) {
        if(subscription->node->collection) {
            socket_server_event_subscription_list_del(subscription->node->collection, subscription->node);
        }
        socket_server_event_subscription_list_node_t * node = subscription->node;
        subscription->node = nil;
        node->session = nil;

        subscription->node = socket_server_event_subscription_list_node_rem(node);
    }

    

    free(subscription);

    return nil;
}
