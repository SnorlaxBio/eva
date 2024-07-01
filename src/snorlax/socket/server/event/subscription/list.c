/**
 * @file        snorlax/socket/server/subscription/list.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 30, 2024
 */

#include "list.h"

#include "../../../session/event/subscription.h"

static socket_server_event_subscription_list_t * socket_server_event_subscription_list_func_rem(socket_server_event_subscription_list_t * list);
static socket_server_event_subscription_list_node_t * socket_server_event_subscription_list_func_add(socket_server_event_subscription_list_t * list, socket_session_event_subscription_t * subscription);
static socket_server_event_subscription_list_node_t * socket_server_event_subscription_list_func_del(socket_server_event_subscription_list_t * list, socket_server_event_subscription_list_node_t * node);
static void socket_server_event_subscription_list_func_clear(socket_server_event_subscription_list_t * list, socket_session_event_subscription_callback_t callback);

static socket_server_event_subscription_list_func_t func = {
    socket_server_event_subscription_list_func_rem,
    socket_server_event_subscription_list_func_add,
    socket_server_event_subscription_list_func_del,
    socket_server_event_subscription_list_func_clear
};

extern socket_server_event_subscription_list_t * socket_server_event_subscription_list_gen(___notnull socket_server_event_subscription_t * server) {
#ifndef   RELEASE
    snorlaxdbg(server == nil, false, "critical", "");
#endif // RELEASE

    socket_server_event_subscription_list_t * list = (socket_server_event_subscription_list_t *) calloc(1, sizeof(socket_server_event_subscription_list_t));

    list->func = address_of(func);
    list->server = server;

    return list;
}


static socket_server_event_subscription_list_t * socket_server_event_subscription_list_func_rem(socket_server_event_subscription_list_t * list) {
#ifndef   RELEASE
    snorlaxdbg(list == nil, false, "critical", "");
#endif // RELEASE

    while(list->head) {
        socket_session_event_subscription_rem(list->head->session);
    }

    free(list);

    return nil;
}
static socket_server_event_subscription_list_node_t * socket_server_event_subscription_list_func_add(socket_server_event_subscription_list_t * list, socket_session_event_subscription_t * subscription) {
#ifndef   RELEASE
    snorlaxdbg(list == nil, false, "critical", "");
    snorlaxdbg(subscription == nil, false, "critical", "");
#endif // RELEASE

    socket_server_event_subscription_list_node_t * node = socket_server_event_subscription_list_node_gen(list);

    subscription->node = node;

    return node;
}

static socket_server_event_subscription_list_node_t * socket_server_event_subscription_list_func_del(socket_server_event_subscription_list_t * list, socket_server_event_subscription_list_node_t * node) {
#ifndef   RELEASE
    snorlaxdbg(list == nil, false, "critical", "");
    snorlaxdbg(node == nil, false, "critical", "");
    snorlaxdbg(node->collection != list, false, "critical", "");
#endif // RELEASE

    socket_server_event_subscription_list_node_t * prev = node->prev;
    socket_server_event_subscription_list_node_t * next = node->next;

    if(prev) {
        prev->next = next;
        node->prev = nil;
    } else {
        list->head = next;
    }

    if(next) {
        next->prev = prev;
        node->next = nil;
    } else {
        list->tail = prev;
    }

    list->size = list->size - 1;
    node->collection = nil;

    return node;
}

static void socket_server_event_subscription_list_func_clear(socket_server_event_subscription_list_t * list, socket_session_event_subscription_callback_t callback) {
#ifndef   RELEASE
    snorlaxdbg(list == nil, false, "critical", "");
#endif // RELEASE

    while(list->head) {
        socket_session_event_subscription_rem(list->head->session);
    }
}
