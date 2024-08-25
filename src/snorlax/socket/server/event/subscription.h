/**
 * @file        snorlax/socket/server/event/subscription.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 27, 2024
 */

#ifndef   __SNORLAX__SOCKET_SERVER_EVENT_SUBSCRIPTION__H__
#define   __SNORLAX__SOCKET_SERVER_EVENT_SUBSCRIPTION__H__

#include <snorlax/socket/server.h>
#include <snorlax/socket/server/event/subscription/list.h>

struct socket_server_event_subscription_func;

typedef struct socket_server_event_subscription_func socket_server_event_subscription_func_t;

struct socket_server_session {
    socket_session_event_subscription_handler_t * handler;     // TODO: RENAME, CALLBACK, HANDLER
    socket_server_event_subscription_list_t * list;
};

typedef struct socket_server_session socket_server_session_t;

struct socket_server_event_subscription {
    socket_server_event_subscription_func_t * func;
    sync_t * sync;
    descriptor_event_generator_t * generator;
    descriptor_event_subscription_t * prev;
    descriptor_event_subscription_t * next;
    event_subscription_event_queue_t * queue;
    socket_server_event_subscription_handler_t * handler;
    uint32_t type;
    uint32_t status;
    socket_server_t * descriptor;
    socket_server_session_t session;
};

struct socket_server_event_subscription_func {
    socket_server_event_subscription_t * (*rem)(___notnull socket_server_event_subscription_t *);
    void (*on)(___notnull socket_server_event_subscription_t *, socket_server_event_subscription_process_t, uint32_t, event_subscription_event_t *);
    void (*notify)(___notnull socket_server_event_subscription_t *, uint32_t, event_subscription_event_t *);
};

typedef socket_server_event_subscription_t * (*socket_server_event_subscription_rem_t)(___notnull socket_server_event_subscription_t *);
typedef void (*socket_server_event_subscription_on_t)(___notnull socket_server_event_subscription_t *, socket_server_event_subscription_process_t, uint32_t, event_subscription_event_t *);
typedef void (*socket_server_event_subscription_notify_t)(___notnull socket_server_event_subscription_t *, uint32_t, event_subscription_event_t *);

/**
 * 서버의 meta 파라미터는 meta generation 이 되도록 구현할 필요가 있다.
 */
extern socket_server_event_subscription_t * socket_server_event_subscription_gen(socket_server_t * descriptor, socket_session_event_subscription_handler_t * sessionOn,socket_server_event_subscription_handler_t * serverOn);
extern socket_server_event_subscription_t * socket_server_event_subscription_func_rem(socket_server_event_subscription_t * subscription);
extern void socket_server_event_subscription_func_notify(___notnull socket_server_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node);

#define socket_server_event_subscription_node_gen(subscription)                    (event_subscription_event_gen((event_subscription_t *) subscription))

#define socket_server_event_subscription_rem(subscription)                         ((subscription)->func->rem(subscription))
#define socket_server_event_subscription_on(subscription, process, type, node)     ((subscription)->func->on(subscription, process, type, node))
#define socket_server_event_subscription_notify(subscription, type, node)          ((subscription)->func->notify(subscription, type, node))

#endif // __SNORLAX__SOCKET_SERVER_EVENT_SUBSCRIPTION__H__
