/**
 * @file        snorlax/socket/server/event/subscription/list/node.c
 * @brief
 * @details
 * 
 * @author      snorlax <opuntia@snorlax.bio>
 * @since       June 30, 2024
 */

#include "../list.h"
#include "../../../../session/event/subscription.h"

static socket_server_event_subscription_list_node_t * socket_server_event_subscription_list_node_func_rem(socket_server_event_subscription_list_node_t * node);

static socket_server_event_subscription_list_node_func_t func = {
    socket_server_event_subscription_list_node_func_rem
};

extern socket_server_event_subscription_list_node_t * socket_server_event_subscription_list_node_gen(socket_server_event_subscription_list_t * list) {
    socket_server_event_subscription_list_node_t * node = (socket_server_event_subscription_list_node_t *) calloc(1, sizeof(socket_server_event_subscription_list_node_t));

    node->func = address_of(func);

    if(list->tail) {
        list->tail->next = node;
        node->prev = list->tail;
    } else {
        list->head = node;
    }

    node->collection = list;
    list->tail = node;
    list->size = list->size + 1;

    return node;
}

static socket_server_event_subscription_list_node_t * socket_server_event_subscription_list_node_func_rem(socket_server_event_subscription_list_node_t * node) {
#ifndef   RELEASE
    snorlaxdbg(node == nil, false, "critical", "");
#endif // RELEASE

    if(node->collection) socket_server_event_subscription_list_del(node->collection, node);

    if(node->session) {
        node->session = socket_session_event_subscription_rem(node->session);
    }

    free(node);

    return nil;
}
