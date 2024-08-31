/**
 * @file        snorlax/event/subscription/meta.h
 * @brief
 * @details
 * 
 * @author      snorlax <opuntia@snorlax.bio>
 * @since       Aug 19, 2024
 */

#ifndef   __SNORLAX__EVENT_SUBSCRIPTION_META__H__
#define   __SNORLAX__EVENT_SUBSCRIPTION_META__H__

#include <snorlax/event.h>

struct event_subscription_meta;
struct event_subscription_meta_func;

typedef struct event_subscription_meta event_subscription_meta_t;
typedef struct event_subscription_meta_func event_subscription_meta_func_t;

struct event_subscription_meta {
    event_subscription_meta_func_t * func;
    sync_t * sync;
};

struct event_subscription_meta_func {
    event_subscription_meta_t * (*rem)(event_subscription_meta_t *);
};

#define event_subscription_meta_rem(meta)           ((meta)->func->rem(meta))

#endif // __SNORLAX__EVENT_SUBSCRIPTION_META__H__
