/**
 * @file        snorlax/socket/main/server.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 27, 2024
 */

#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <snorlax/eva.h>
#include <snorlax/socket.h>
#include <snorlax/socket/server.h>

event_subscription_t * subscription = nil;
socket_t * server = nil;

static void cancel(const event_engine_t * engine);
static void on(___notnull socket_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node);

int main(int argc, char ** argv) {
    snorlax_eva_on();

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = 0;
    addr.sin_port = htons(3371);
    server = socket_gen(socket_server_func_get(), AF_INET, SOCK_STREAM, IPPROTO_TCP, address_of(addr), sizeof(struct sockaddr_in));

    socket_event_subscription_handler_t handler[descriptor_event_type_max] = {
        on,
        on,
        on,
        on,
        on,
        on
    };

    subscription = snorlax_eva_socket_sub(server, handler);

    return snorlax_eva_run();
}

static void on(___notnull socket_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node) {
    printf("type => %u, ret => %p\n", type, node);
}

static void cancel(const event_engine_t * engine) {
}