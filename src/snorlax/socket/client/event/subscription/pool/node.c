#include <stdlib.h>

#include "../pool.h"
#include "../../subscription.h"

static socket_client_event_subscription_pool_node_func_t func = {
    socket_client_event_subscription_pool_node_func_rem
};

extern socket_client_event_subscription_pool_node_t * socket_client_event_subscription_pool_node_gen(socket_client_event_subscription_t * subscription) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
#endif // RELEASE

    socket_client_event_subscription_pool_node_t * node = (socket_client_event_subscription_pool_node_t *) calloc(1, sizeof(socket_client_event_subscription_pool_node_t));

    node->func = address_of(func);
    node->client = subscription;

    return nil;
}

extern socket_client_event_subscription_pool_node_t * socket_client_event_subscription_pool_node_func_rem(socket_client_event_subscription_pool_node_t * node) {
#ifndef   RELEASE
    snorlaxdbg(node == nil, false, "critical", "");
#endif // RELEASE

    if(node->collection) socket_client_event_subscription_pool_del(node->collection, node);
    if(node->client) socket_client_event_subscription_rem(node->client);

    free(node);

    return nil;
}