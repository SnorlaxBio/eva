/**
 * @file        snorlax/socket/client/event/subscription.h
 * @brief
 * @details
 * 
 * @author      snorlax <opuntia@snorlax.bio>
 * @since       June 27, 2024
 */

#ifndef   __SNORLAX__SOCKET_CLIENT_EVENT_SUBSCRIPTION__H__
#define   __SNORLAX__SOCKET_CLIENT_EVENT_SUBSCRIPTION__H__

#include <snorlax/socket/client.h>
#include <snorlax/socket/client/event/subscription/pool.h>

struct socket_client_event_subscription;
struct socket_client_event_subscription_func;

typedef struct socket_client_event_subscription socket_client_event_subscription_t;
typedef struct socket_client_event_subscription_func socket_client_event_subscription_func_t;

struct socket_client_event_subscription {
    socket_client_event_subscription_func_t * func;
    sync_t * sync;
    descriptor_event_generator_t * generator;
    descriptor_event_subscription_t * prev;
    descriptor_event_subscription_t * next;
    event_subscription_event_queue_t * queue;
    socket_client_event_subscription_handler_t * handler;
    uint32_t type;
    uint32_t status;
    socket_client_t * descriptor;

    socket_client_event_subscription_pool_node_t * node;
};

struct socket_client_event_subscription_func {
    socket_client_event_subscription_t * (*rem)(___notnull socket_client_event_subscription_t *);
    void (*on)(___notnull socket_client_event_subscription_t *, socket_client_event_subscription_process_t, uint32_t, event_subscription_event_t *);
    void (*notify)(___notnull socket_client_event_subscription_t *, uint32_t, event_subscription_event_t *);
};

extern socket_client_event_subscription_t * socket_client_event_subscription_gen(socket_client_t * client, socket_client_event_subscription_handler_t * handler, socket_client_event_subscription_pool_t * pool);

extern socket_client_event_subscription_t * socket_client_event_subscription_func_rem(___notnull socket_client_event_subscription_t * subscription);
extern void socket_client_event_subscription_func_on(___notnull socket_client_event_subscription_t * subscription, socket_client_event_subscription_process_t process, uint32_t type, event_subscription_event_t * node);
extern void socket_client_event_subscription_func_notify(___notnull socket_client_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node);

extern descriptor_buffer_t * socket_client_event_subscription_buffer_get(___notnull socket_client_event_subscription_t * subscription);

extern int64_t socket_client_event_subscription_write(___notnull socket_client_event_subscription_t * subscription, const uint8_t * data, uint64_t datalen);

#define socket_client_event_subscription_rem(subscription)                         ((subscription)->func->rem(subscription))
#define socket_client_event_subscription_on(subscription, process, type, node)     ((subscription)->func->on(subscription, process, type, node))
#define socket_client_event_subscription_notify(subscription, type, node)          ((subscription)->func->notify(subscription, type, node))

#endif // __SNORLAX__SOCKET_CLIENT_EVENT_SUBSCRIPTION__H__
