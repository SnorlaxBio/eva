/**
 * @file        snorlax/socket/client/main.c
 * @brief
 * @details
 * 
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 29, 2024
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

#include <snorlax/eva.h>
#include <snorlax/socket/client.h>

socket_client_event_subscription_t * subscription = nil;
socket_client_t * client = nil;


static void cancel(const event_engine_t * engine);
static void on(___notnull socket_client_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node);

int main(int argc, char ** argv) {
    snorlax_eva_on();

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(6379);
    socklen_t addrlen = sizeof(struct sockaddr_in);

    socket_client_event_subscription_handler_t handler[descriptor_event_type_max] = {
        on,
        on,
        on,
        on,
        on,
        on
    };

    client = socket_client_gen(AF_INET, SOCK_STREAM, IPPROTO_TCP, (struct sockaddr *) &addr, addrlen);
    subscription = snorlax_eva_socket_client_sub(client, handler);
    return snorlax_eva_run();
}

static void on(___notnull socket_client_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node) {
    if(type == descriptor_event_type_open) {
        snorlax_eva_descriptor_write((descriptor_event_subscription_t *) subscription, "PING\r\n", 6);
    } else if(type == descriptor_event_type_read) {
        buffer_t * buffer = snorlax_eva_descriptor_buffer_in_get((descriptor_event_subscription_t *) subscription);
        if(buffer_length(buffer) < 16) {
            char buf[16];
            memcpy(buf, buffer_front(buffer), buffer_length(buffer));
            buf[buffer_length(buffer)] = 0;
        }
        buffer_position_set(buffer, buffer_size_get(buffer));
        snorlax_eva_descriptor_close((descriptor_event_subscription_t *) subscription);

    } else if(type == descriptor_event_type_close) {
        snorlax_eva_off(cancel);
    }
}

static void cancel(const event_engine_t * engine) {
    subscription = (socket_client_event_subscription_t * ) object_rem((object_t *) subscription);
    client = (socket_client_t *) object_rem((object_t *) client);
}