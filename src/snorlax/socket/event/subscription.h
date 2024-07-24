/**
 * @file        snorlax/socket/event/subscription.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 27, 2024
 */

#ifndef   __SNORLAX__SOCKET_EVENT_SUBSCRIPTION__H__
#define   __SNORLAX__SOCKET_EVENT_SUBSCRIPTION__H__

#include <snorlax/socket.h>

struct socket_event_subscription_func;

typedef struct socket_event_subscription_func socket_event_subscription_func_t;

struct socket_event_subscription {
    socket_event_subscription_func_t * func;
    sync_t * sync;
    descriptor_event_generator_t * generator;
    descriptor_event_subscription_t * prev;
    descriptor_event_subscription_t * next;
    event_subscription_event_queue_t * queue;
    socket_event_subscription_handler_t * handler;
    uint32_t type;
    uint32_t status;
    socket_t * descriptor;
};

struct socket_event_subscription_func {
    socket_event_subscription_t * (*rem)(___notnull socket_event_subscription_t *);
    void (*on)(___notnull socket_event_subscription_t *, socket_event_subscription_process_t, uint32_t, event_subscription_event_t *);
    void (*notify)(___notnull socket_event_subscription_t *, uint32_t, event_subscription_event_t *);
};

#define socket_event_subscription_buffer_get(subscription)                  (&((socket_event_subscription_t *)(subscription))->descriptor->buffer)

#define socket_event_subscription_gen(descriptor, handler)                  ((socket_event_subscription_t *) descriptor_event_subscription_gen((descriptor_t *) descriptor, (descriptor_event_subscription_handler_t *) handler))

#define socket_event_subscription_node_gen(subscription)                    (event_subscription_event_gen((event_subscription_t *) subscription))

#define socket_event_subscription_rem(subscription)                         ((subscription)->func->rem(subscription))
#define socket_event_subscription_on(subscription, process, type, node)     ((subscription)->func->on(subscription, process, type, node))
#define socket_event_subscription_notify(subscription, type, node)          ((subscription)->func->notify(subscription, type, node))

#endif // __SNORLAX__SOCKET_EVENT_SUBSCRIPTION__H__
