#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#include <snorlax/event.h>
#include <snorlax/event/engine.h>
#include <snorlax/socket/server.h>
#include <snorlax/socket/session/event/subscription.h>

event_engine_t * engine = nil;

static void sessionEventOn(___notnull socket_session_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node);
static void serverEventOn(___notnull socket_server_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node);

static socket_session_event_subscription_handler_t sessionOn[descriptor_event_type_max] = {
    sessionEventOn,
    sessionEventOn,
    sessionEventOn,
    sessionEventOn,
    sessionEventOn,
    sessionEventOn,
};

static socket_server_event_subscription_handler_t serverOn[descriptor_event_type_max] = {
    serverEventOn,
    serverEventOn,
    serverEventOn,
    serverEventOn,
    serverEventOn,
    serverEventOn
};

int main(int argc, char ** argv) {
    engine = event_engine_gen();

    if(event_engine_on(engine) == fail) {
        engine = event_engine_rem(engine);
        return fail;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = 0;
    addr.sin_port = ntohs(2078);

    socket_server_t * server = socket_server_gen(AF_INET, SOCK_STREAM, IPPROTO_TCP, address_of(addr), sizeof(struct sockaddr_in));

    socket_server_event_subscription_t * subscription = event_engine_socket_server_sub(engine, server, sessionOn, serverOn);
    
    return event_engine_run(engine);
}

static void sessionEventOn(___notnull socket_session_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node) {
    if(type == descriptor_event_type_read) {
        descriptor_buffer_t * buffer = socket_session_event_subscription_buffer_get(subscription);

        buffer_node_t * in = buffer_front(buffer->in);
        buffer_node_t * out = buffer_back(buffer->out, buffer_node_length(in));

        memcpy(buffer_node_back(out), buffer_node_front(in), buffer_node_length(in));

        buffer_node_size_set(out, buffer_node_size_get(out) + buffer_node_length(in));

        buffer_node_position_set(in, buffer_node_position_get(in) + buffer_node_length(in));
        buffer_shrink(buffer->in);
    }
}

static void serverEventOn(___notnull socket_server_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node) {

}
