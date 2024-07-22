/**
 * @file        snorlax/socket/server/event/subscription.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 27, 2024
 */

#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>

#include "subscription.h"

#include "../../server.h"
#include "../../session.h"
#include "../../session/event/subscription.h"

#include "../../../descriptor/event/generator.h"
#include "../../../descriptor/event/subscription.h"
#include "../../../descriptor/event/subscription/type.h"
#include "../../../descriptor/event/subscription/process.h"

#include "../../../event/subscription/event/queue.h"

static socket_server_event_subscription_func_t func = {
    socket_server_event_subscription_func_rem,
    (socket_server_event_subscription_on_t) nil,
    socket_server_event_subscription_func_notify
};

extern socket_server_event_subscription_t * socket_server_event_subscription_gen(socket_server_t * descriptor, socket_session_event_subscription_handler_t * sessionOn,socket_server_event_subscription_handler_t * serverOn) {
#ifndef   RELEASE
    snorlaxdbg(descriptor == nil, false, "critical", "");
#endif // RELEASE
    socket_server_event_subscription_t * subscription = (socket_server_event_subscription_t *) calloc(1, sizeof(socket_server_event_subscription_t));

    subscription->func = address_of(func);
    subscription->queue = event_subscription_event_queue_gen();
    subscription->handler = (socket_server_event_subscription_handler_t *) calloc(descriptor_event_type_max, sizeof(socket_server_event_subscription_handler_t));
    subscription->descriptor = descriptor;
    subscription->type = event_subscription_type_descriptor;
    subscription->session.list = socket_server_event_subscription_list_gen(subscription);

    subscription->session.handler = (socket_session_event_subscription_handler_t *) calloc(descriptor_event_type_max, sizeof(socket_session_event_subscription_handler_t));

    if(sessionOn) {
        for(uint32_t i = 0; i < descriptor_event_type_max; i++) {
            subscription->session.handler[i] = sessionOn[i];
        }
    }

    if(serverOn) {
        for(uint32_t i = 0; i < descriptor_event_type_max; i++) {
            subscription->handler[i] = serverOn[i];
        }
    }

    return subscription;
}

extern socket_server_event_subscription_t * socket_server_event_subscription_func_rem(socket_server_event_subscription_t * subscription) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
#endif // RELEASE

    descriptor_event_generator_t * generator = subscription->generator;

    if(generator) descriptor_event_generator_del(generator, (descriptor_event_subscription_t *) subscription);

    event_subscription_event_queue_clear(subscription->queue);

    descriptor_event_subscription_notify(subscription, descriptor_event_type_subscription, (event_subscription_event_t *) descriptor_event_subscription_type_rem);

    subscription->sync = sync_rem(subscription->sync);

    subscription->session.list = socket_server_event_subscription_list_rem(subscription->session.list);
    subscription->session.handler = memory_rem(subscription->session.handler);

    subscription->queue = event_subscription_event_queue_rem(subscription->queue);
    subscription->handler = memory_rem(subscription->handler);

    free(subscription);

    return nil;
}

extern void socket_server_event_subscription_func_notify(___notnull socket_server_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
#endif // RELEASE

    if(type == descriptor_event_type_read) {
        socket_server_t * server = subscription->descriptor;
        buffer_node_t * buffer = buffer_front(server->buffer.in);

        socklen_t addrlen = *((socklen_t *) buffer_node_front(buffer));
        struct sockaddr * addr = (struct sockaddr *) (buffer_node_front(buffer) + sizeof(socklen_t));
        int32_t value = *((int32_t *)((buffer_node_front(buffer) + sizeof(socklen_t) + addrlen)));

        socket_session_t * descriptor = socket_session_gen(value, server->domain, server->type, server->protocol, addr, addrlen);
        socket_session_event_subscription_t * session = socket_session_event_subscription_gen(descriptor, subscription->session.handler, socket_server_event_subscription_list_node_gen(subscription->session.list));

        socket_session_event_subscription_notify(session, descriptor_event_type_open, nil);

        if(descriptor_event_generator_add(subscription->generator, (descriptor_event_subscription_t *) session) == success) {
            if(descriptor->status & descriptor_state_write) {
                event_subscription_process_t process = descriptor_event_subscription_process_get(descriptor_event_type_write);

                process((event_subscription_t *) session, descriptor_event_type_write, nil);
            }
        } else {
            socket_session_event_subscription_rem(session);
        }

        socket_server_event_subscription_handler_t on = subscription->handler[type];

        if(on) {
            on(subscription, type, node);
        }

        buffer_node_position_set(buffer, buffer_node_position_get(buffer) + sizeof(socklen_t) + addrlen + sizeof(int));

        // buffer_position_set(buffer, buffer_position_get(buffer) + sizeof(socklen_t) + addrlen + sizeof(int));
        // buffer_adjust(buffer, 64);      // TODO: ADJUST VALUE

    } else {
        socket_server_event_subscription_handler_t on = subscription->handler[type];

        if(on) {
            on(subscription, type, node);
        }
    }


}
