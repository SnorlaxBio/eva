#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#include <snorlax/event.h>
#include <snorlax/event/engine.h>
#include <snorlax/socket/client.h>
#include <snorlax/socket/client/event/subscription.h>

static void clientEventOn(___notnull socket_client_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node);

static socket_client_event_subscription_handler_t clientOn[descriptor_event_type_max] = {
    clientEventOn,
    clientEventOn,
    clientEventOn,
    clientEventOn,
    clientEventOn,
    clientEventOn
};

static event_engine_t * engine = nil;

int main(int argc, char ** argv) {
    engine = event_engine_gen();

    if(event_engine_on(engine) == fail) {
        engine = event_engine_rem(engine);
        return fail;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = ntohs(2078);

    socket_client_t * client = socket_client_gen(AF_INET, SOCK_STREAM, IPPROTO_TCP, address_of(addr), sizeof(struct sockaddr_in));
    socket_client_event_subscription_t * subscription = event_engine_socket_client_sub(engine, client, clientOn, nil, nil);

    return event_engine_run(engine);
}

static void clientEventOn(___notnull socket_client_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node) {
    if(type == descriptor_event_type_read) {
        descriptor_buffer_t * buffer = socket_client_event_subscription_buffer_get(subscription);
        /**
         * TODO: SIMPLE HUMAN READABLE PROTOCOL
         * 
         * [TYPE:4 BYTES]\r\n[LENGTH:N BYTES]\r\n[DATA: M BYTES]\r\n
         */
    }
}