/**
 * @file        snorlax/socket/client/event/subscription/pool.h
 * @brief       
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       July 18, 2024
 */

#ifndef   __SNORLAX__SOCKET_CLIENT_EVENT_SUBSCRIPTION_POOL__H__
#define   __SNORLAX__SOCKET_CLIENT_EVENT_SUBSCRIPTION_POOL__H__

#include <snorlax.h>
#include <snorlax/socket/client.h>

struct socket_client_event_subscription;
struct socket_client_event_subscription_pool;
struct socket_client_event_subscription_pool_func;

struct socket_client_event_subscription_pool_node;
struct socket_client_event_subscription_pool_node_func;

typedef struct socket_client_event_subscription socket_client_event_subscription_t;

typedef struct socket_client_event_subscription_pool socket_client_event_subscription_pool_t;
typedef struct socket_client_event_subscription_pool_func socket_client_event_subscription_pool_func_t;

typedef struct socket_client_event_subscription_pool_node socket_client_event_subscription_pool_node_t;
typedef struct socket_client_event_subscription_pool_node_func socket_client_event_subscription_pool_node_func_t;

// REGISTER CLIENT SUBSCRIPTION POOL ... NOT MANAGED ...

struct socket_client_event_subscription_pool {
    socket_client_event_subscription_pool_func_t * func;
    sync_t * sync;
    uint64_t size;
    socket_client_event_subscription_pool_node_t * head;
    socket_client_event_subscription_pool_node_t * tail;
    socket_client_event_subscription_handler_t * handler;
};

struct socket_client_event_subscription_pool_func {
    socket_client_event_subscription_pool_t * (*rem)(socket_client_event_subscription_pool_t *);
    void (*push)(socket_client_event_subscription_pool_t *, socket_client_event_subscription_pool_node_t *);
    socket_client_event_subscription_pool_node_t * (*pop)(socket_client_event_subscription_pool_t *);
    void (*del)(socket_client_event_subscription_pool_t *, socket_client_event_subscription_pool_node_t *);
};

extern socket_client_event_subscription_pool_t * socket_client_event_subscription_pool_gen(socket_client_event_subscription_handler_t * handler);

extern socket_client_event_subscription_pool_t * socket_client_event_subscription_pool_func_rem(socket_client_event_subscription_pool_t * pool);
extern void socket_client_event_subscription_pool_func_push(socket_client_event_subscription_pool_t * pool, socket_client_event_subscription_pool_node_t * node);
extern socket_client_event_subscription_pool_node_t * socket_client_event_subscription_pool_func_pop(socket_client_event_subscription_pool_t * pool);
extern void socket_client_event_subscription_pool_func_del(socket_client_event_subscription_pool_t * pool, socket_client_event_subscription_pool_node_t * node);

#define socket_client_event_subscription_pool_rem(pool)             ((pool)->func->rem(pool))
#define socket_client_event_subscription_pool_push(pool, node)      ((pool)->func->push(pool, node))
#define socket_client_event_subscription_pool_pop(pool)             ((pool)->func->pop(pool))
#define socket_client_event_subscription_pool_del(pool, node)       ((pool)->func->del(pool, node))

struct socket_client_event_subscription_pool_node {
    socket_client_event_subscription_pool_node_func_t * func;
    sync_t * sync;
    socket_client_event_subscription_pool_t * collection;
    socket_client_event_subscription_pool_node_t * prev;
    socket_client_event_subscription_pool_node_t * next;

    socket_client_event_subscription_t * client;
};

struct socket_client_event_subscription_pool_node_func {
    socket_client_event_subscription_pool_node_t * (*rem)(socket_client_event_subscription_pool_node_t *);
};

extern socket_client_event_subscription_pool_node_t * socket_client_event_subscription_pool_node_gen(socket_client_event_subscription_t * subscription);
extern socket_client_event_subscription_pool_node_t * socket_client_event_subscription_pool_node_func_rem(socket_client_event_subscription_pool_node_t * node);

#define socket_client_event_subscription_pool_node_rem(node)        ((node)->func->rem(node))

#endif // __SNORLAX__SOCKET_CLIENT_EVENT_SUBSCRIPTION_POOL__H__
