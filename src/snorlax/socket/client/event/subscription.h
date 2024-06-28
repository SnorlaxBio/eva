/**
 * @file        snorlax/socket/client/event/subscription.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 27, 2024
 */

#ifndef   __SNORLAX__SOCKET_CLIENT_EVENT_SUBSCRIPTION__H__
#define   __SNORLAX__SOCKET_CLIENT_EVENT_SUBSCRIPTION__H__

#include <snorlax/socket/client.h>

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
};

struct socket_client_event_subscription_func {
    socket_client_event_subscription_t * (*rem)(___notnull socket_client_event_subscription_t *);
    void (*on)(___notnull socket_client_event_subscription_t *, socket_client_event_subscription_process_t, uint32_t, event_subscription_event_t *);
    void (*notify)(___notnull socket_client_event_subscription_t *, uint32_t, event_subscription_event_t *);
};

#define socket_client_event_subscription_gen(descriptor, handler)                  ((socket_client_event_subscription_t *) descriptor_event_subscription_gen((descriptor_t *) descriptor, (descriptor_event_subscription_handler_t *) handler))

#define socket_client_event_subscription_node_gen(subscription)                    (event_subscription_event_gen((event_subscription_t *) subscription))

#define socket_client_event_subscription_rem(subscription)                         ((subscription)->func->rem(subscription))
#define socket_client_event_subscription_on(subscription, process, type, node)     ((subscription)->func->on(subscription, process, type, node))
#define socket_client_event_subscription_notify(subscription, type, node)          ((subscription)->func->notify(subscription, type, node))


#endif // __SNORLAX__SOCKET_CLIENT_EVENT_SUBSCRIPTION__H__
