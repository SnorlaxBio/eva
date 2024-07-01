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

const int total = 100;

socket_client_event_subscription_t * subscriptions[100];
socket_client_t * clients[100];


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

    for(int i = 0; i < total; i++) {
        clients[i] = socket_client_gen(AF_INET, SOCK_STREAM, IPPROTO_TCP, (struct sockaddr *) &addr, addrlen);
        subscriptions[i] = snorlax_eva_socket_client_sub(clients[i], handler);
    }


    return snorlax_eva_run();
}

static int count = 0;

static void on(___notnull socket_client_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node) {
    printf("%p %d %p\n", subscription, type, node);
    if(type == descriptor_event_type_open) {
        printf("open\n");
        count = count + 1;
        printf("count = %d\n", count);
        snorlax_eva_descriptor_write((descriptor_event_subscription_t *) subscription, "PING\r\n", 6);

    } else if(type == descriptor_event_type_read) {
        printf("read\n");
        buffer_t * buffer = snorlax_eva_descriptor_buffer_in_get((descriptor_event_subscription_t *) subscription);
        if(buffer_length(buffer) < 16) {
            char buf[16];
            memcpy(buf, buffer_front(buffer), buffer_length(buffer));
            buf[buffer_length(buffer)] = 0;
            printf("%s", buf);
        }
        buffer_position_set(buffer, buffer_size_get(buffer));
        snorlax_eva_descriptor_close((descriptor_event_subscription_t *) subscription);

    } else if(type == descriptor_event_type_close) {
        // NEED TO RECONNECT
        count = count - 1;
        printf("count = %d\n", count);
        if(count >= total) {
            snorlax_eva_off(cancel);
        }
    }
}

static void cancel(const event_engine_t * engine) {
    for(int i = 0; i < total; i++) {
        subscriptions[i] = (socket_client_event_subscription_t * ) object_rem((object_t *) subscriptions[i]);
        clients[i] = (socket_client_t *) object_rem((object_t *) clients[i]);
    }
}