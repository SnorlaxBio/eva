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

const int size = 1024;
const int repeat = 16;
const int request = 1000000;
int response = 0;


socket_client_event_subscription_t * subscriptions[100] = { 0, };
socket_client_t * clients[100] = { 0, };

char * data = nil;

static void init(void);
static void cancel(const event_engine_t * engine);
static void on(___notnull socket_client_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node);

int main(int argc, char ** argv) {
    init();
    snorlax_eva_on();

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(3371);
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
    if(type == descriptor_event_type_open) {
        count = count + 1;

        snorlax_eva_descriptor_write((descriptor_event_subscription_t *) subscription, data, size * repeat);

    } else if(type == descriptor_event_type_read) {
        buffer_t * buffer = snorlax_eva_descriptor_buffer_in_get((descriptor_event_subscription_t *) subscription);
        uint64_t n = buffer_length(buffer);
        uint64_t res = n / size;
        buffer_position_set(buffer, buffer_position_get(buffer) + res * size);
        for(int i = 0; i < res && response < request; i++) {
            snorlax_eva_descriptor_write((descriptor_event_subscription_t *) subscription, data, size);
            response = response + 1;
            if(request <= response) {
                break;
            }
        }
    } else if(type == descriptor_event_type_close) {
        count = count - 1;
    }
}

static void cancel(const event_engine_t * engine) {
    for(int i = 0; i < total; i++) {
        subscriptions[i] = (socket_client_event_subscription_t * ) object_rem((object_t *) subscriptions[i]);
        clients[i] = (socket_client_t *) object_rem((object_t *) clients[i]);
    }
    free(data);
}

static void init(void) {
    data = malloc(size * repeat);
    char * origin = malloc(size);
    for(int i = 0; i < size; i++) {
        origin[i] = (char) i;
    }
    origin[1023] = 0;
    for(int i = 0; i < repeat; i++) {
        memcpy(&data[i], origin, 1024);
    }
    free(origin);
}