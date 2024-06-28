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
        socket_session_event_subscription_func_rem(subscription);
    }
}