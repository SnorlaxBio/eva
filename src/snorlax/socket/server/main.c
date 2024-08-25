/**
 * @file        snorlax/socket/server/main.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 30, 2024
 */


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

#include <snorlax/eva.h>
#include <snorlax/socket/server.h>
#include <snorlax/socket/session.h>

socket_server_event_subscription_t * subscription = nil;
socket_server_t * server = nil;

static void cancel(const event_engine_t * engine);
static void serverOn(___notnull socket_server_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node);
static void sessionOn(___notnull socket_session_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node);

int main(int argc, char ** argv) {
    snorlax_eva_on();

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(3371);
    socklen_t addrlen = sizeof(struct sockaddr_in);

    socket_server_event_subscription_handler_t callback[descriptor_event_type_max] = {
        serverOn,
        serverOn,
        serverOn,
        serverOn,
        serverOn,
        serverOn
    };

    socket_session_event_subscription_handler_t handler[descriptor_event_type_max] = {
        sessionOn,
        sessionOn,
        sessionOn,
        sessionOn,
        sessionOn,
        sessionOn
    };

    server = socket_server_gen(AF_INET, SOCK_STREAM, IPPROTO_TCP, (struct sockaddr *) &addr, addrlen);
    subscription = snorlax_eva_socket_server_sub(server, handler, callback);
    return snorlax_eva_run();
}

static void serverOn(___notnull socket_server_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node) {
}

static int count = 0;

static void sessionOn(___notnull socket_session_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node) {
    if(type == descriptor_event_type_open) {
        count = count + 1;
    } else if(type == descriptor_event_type_close) {
        count = count - 1;
    } else if(type == descriptor_event_type_read) {
        buffer_t * in = snorlax_eva_descriptor_buffer_in_get((descriptor_event_subscription_t *) subscription);
        buffer_t * out = snorlax_eva_descriptor_buffer_out_get((descriptor_event_subscription_t *) subscription);
        int64_t n = 0;
        // UPGRADE & CONCEPT SERIALIZER & DESERIALIZER

        // while(n = string_simple_deserialize(in, out)) {
        //     char * s = index(buffer_node_front(buffer_front(out)), '\r');
        //     if(s) {
        //         *s = '\n';
        //     }
        //     if(strncmp(buffer_node_front(buffer_front(out)), "quit\n\n", 6) == 0) {
        //         snorlax_eva_off(cancel);
        //         break;
        //     }
        // }
    }
}

static void cancel(const event_engine_t * engine) {
    subscription = (socket_server_event_subscription_t * ) object_rem((object_t *) subscription);
    socket_close(server);
    server = (socket_server_t *) object_rem((object_t *) server);
}