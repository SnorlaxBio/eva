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

#include <snorlax/eva.h>
#include <snorlax/socket/client.h>

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

    socket_client_t * client = socket_client_gen(AF_INET, SOCK_STREAM, IPPROTO_TCP, (struct sockaddr *) &addr, addrlen);
    socket_client_event_subscription_t * subscription = snorlax_eva_socket_client_sub(client, handler);
    return snorlax_eva_run();
}

static void on(___notnull socket_client_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node) {
    printf("%p %d %p\n", subscription, type, node);
}