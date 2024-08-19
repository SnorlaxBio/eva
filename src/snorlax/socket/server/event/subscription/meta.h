/**
 * @file        snorlax/socket/server/event/subscription/meta.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       Aug 19, 2024
 */

#ifndef   __SOCKET__SERVER_EVENT_SUBSCRIPTION_META__H__
#define   __SOCKET__SERVER_EVENT_SUBSCRIPTION_META__H__

#include <snorlax/event/subscription/meta.h>

struct socket_server_event_subscription_meta;
struct socket_server_event_subscription_meta_func;

typedef struct socket_server_event_subscription_meta socket_server_event_subscription_meta_t;
typedef struct socket_server_event_subscription_meta_func socket_server_event_subscription_meta_func_t;

typedef event_subscription_meta_t * (*event_subscription_meta_gen_t)(void);

struct socket_server_event_subscription_meta {
    socket_server_event_subscription_meta_func_t * func;
    sync_t * sync;
    event_subscription_meta_gen_t * metagen;
};

struct socket_server_event_subscription_meta_func {
    socket_server_event_subscription_meta_t * (*rem)(socket_server_event_subscription_meta_t *);
};

#define socket_server_event_subscription_meta_rem(meta)         ((meta)->func->rem(meta))

#endif // __SOCKET__SERVER_EVENT_SUBSCRIPTION_META__H__
