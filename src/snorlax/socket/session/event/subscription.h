/**
 * @file        snorlax/socket/session/event/subscription.h
 * @brief
 * @details
 * 
 * @author      snorlax <opuntia@snorlax.bio>
 * @since       June 27, 2024
 */

#ifndef   __SNORLAX__SESSION_EVENT_SUBSCRIPTION__H__
#define   __SNORLAX__SESSION_EVENT_SUBSCRIPTION__H__

#include <snorlax/socket/session.h>
#include <snorlax/socket/server/event/subscription/list.h>

struct socket_session_event_subscription_func;

typedef struct socket_session_event_subscription_func socket_session_event_subscription_func_t;

struct socket_session_event_subscription {
    socket_session_event_subscription_func_t * func;
    sync_t * sync;
    descriptor_event_generator_t * generator;
    descriptor_event_subscription_t * prev;
    descriptor_event_subscription_t * next;
    event_subscription_event_queue_t * queue;
    socket_session_event_subscription_handler_t * handler;
    uint32_t type;
    uint32_t status;
    socket_session_t * descriptor;

    socket_server_event_subscription_list_node_t * node;
};

typedef socket_session_event_subscription_t * (*socket_session_event_subscription_rem_t)(___notnull socket_session_event_subscription_t *);
typedef void (*socket_session_event_subscription_on_t)(___notnull socket_session_event_subscription_t *, socket_session_event_subscription_process_t, uint32_t, event_subscription_event_t *);
typedef void (*socket_session_event_subscription_notify_t)(___notnull socket_session_event_subscription_t *, uint32_t, event_subscription_event_t *);

struct socket_session_event_subscription_func {
    socket_session_event_subscription_t * (*rem)(___notnull socket_session_event_subscription_t *);
    void (*on)(___notnull socket_session_event_subscription_t *, socket_session_event_subscription_process_t, uint32_t, event_subscription_event_t *);
    void (*notify)(___notnull socket_session_event_subscription_t *, uint32_t, event_subscription_event_t *);
};

extern socket_session_event_subscription_t * socket_session_event_subscription_gen(socket_session_t * descriptor, socket_session_event_subscription_handler_t* handler, socket_server_event_subscription_list_node_t * node);

#define socket_session_event_subscription_rem(subscription)                         ((subscription)->func->rem(subscription))
#define socket_session_event_subscription_on(subscription, process, type, node)     ((subscription)->func->on(subscription, process, type, node))
#define socket_session_event_subscription_notify(subscription, type, node)          ((subscription)->func->notify(subscription, type, node))

extern descriptor_buffer_t * socket_session_event_subscription_buffer_get(___notnull socket_session_event_subscription_t * subscription);

#endif // __SNORLAX__SESSION_EVENT_SUBSCRIPTION__H__
