/**
 * @file        snorlax/socket/server/event/subscription/list.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 27, 2024
 */

#ifndef   __SNORLAX__SOCKET_SERVER_EVENT_SUBSCRIPTION_LIST__H__
#define   __SNORLAX__SOCKET_SERVER_EVENT_SUBSCRIPTION_LIST__H__

#include <snorlax.h>
#include <snorlax/socket/session.h>
#include <snorlax/socket/server.h>

struct socket_server_event_subscription_list;
struct socket_server_event_subscription_list_func;
struct socket_server_event_subscription_list_node;
struct socket_server_event_subscription_list_node_func;

typedef struct socket_server_event_subscription_list socket_server_event_subscription_list_t;
typedef struct socket_server_event_subscription_list_func socket_server_event_subscription_list_func_t;
typedef struct socket_server_event_subscription_list_node socket_server_event_subscription_list_node_t;
typedef struct socket_server_event_subscription_list_node_func socket_server_event_subscription_list_node_func_t;

struct socket_server_event_subscription_list {
    socket_server_event_subscription_list_func_t * func;
    sync_t * sync;
    uint64_t size;
    socket_server_event_subscription_list_node_t * head;
    socket_server_event_subscription_list_node_t * tail;

    socket_server_event_subscription_t * server;
};

typedef void (*socket_session_event_subscription_callback_t)(socket_session_event_subscription_t *);

struct socket_server_event_subscription_list_func {
    socket_server_event_subscription_list_t * (*rem)(socket_server_event_subscription_list_t *);
    socket_server_event_subscription_list_node_t * (*add)(socket_server_event_subscription_list_t *, socket_session_event_subscription_t *);
    socket_server_event_subscription_list_node_t * (*del)(socket_server_event_subscription_list_t *, socket_server_event_subscription_list_node_t *);

    void (*clear)(socket_server_event_subscription_list_t *, socket_session_event_subscription_callback_t);
};

extern socket_server_event_subscription_list_t * socket_server_event_subscription_list_gen(___notnull socket_server_event_subscription_t * server);

#define socket_server_event_subscription_list_rem(collection)                   ((collection)->func->rem(collection))
#define socket_server_event_subscription_list_add(collection, subscription)     ((collection)->func->add(collection, subscription))
#define socket_server_event_subscription_list_del(collection, node)             ((collection)->func->del(collection, node))
#define socket_server_event_subscription_list_clear(collection, callback)       ((collection)->func->clear(collection, callback))

struct socket_server_event_subscription_list_node {
    socket_server_event_subscription_list_node_func_t * func;
    socket_server_event_subscription_list_t * collection;
    socket_server_event_subscription_list_node_t * prev;
    socket_server_event_subscription_list_node_t * next;

    socket_session_event_subscription_t * session;
};

struct socket_server_event_subscription_list_node_func {
    socket_server_event_subscription_list_node_t * (*rem)(socket_server_event_subscription_list_node_t *);
};

extern socket_server_event_subscription_list_node_t * socket_server_event_subscription_list_node_gen(socket_server_event_subscription_list_t * list);

#define socket_server_event_subscription_list_node_rem(node)        ((node)->func->rem(node))

#endif // __SNORLAX__SOCKET_SERVER_EVENT_SUBSCRIPTION_LIST__H__
