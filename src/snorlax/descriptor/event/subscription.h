/**
 * @file        snorlax/descriptor/event/subscription.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 20, 2024
 */

#ifndef   __SNORLAX__DESCRIPTOR_EVENT_SUBSCRIPTION__H__
#define   __SNORLAX__DESCRIPTOR_EVENT_SUBSCRIPTION__H__

#include <snorlax/descriptor.h>

#include <snorlax/descriptor/action.h>

struct descriptor_event_subscription {
    descriptor_event_subscription_func_t * func;
    sync_t * sync;

    descriptor_event_generator_t * generator;
    descriptor_event_subscription_t * prev;
    descriptor_event_subscription_t * next;

    event_subscription_event_queue_t * queue;

    uint32_t status;
    descriptor_t * descriptor;

    descriptor_event_subscription_handler_t handler[descriptor_action_max];
};

struct descriptor_event_subscription_func {
    descriptor_event_subscription_t * (*rem)(___notnull descriptor_event_subscription_t *);
    void (*on)(___notnull descriptor_event_subscription_t *, uint32_t, uint64_t);

    int32_t (*reg)(___notnull descriptor_event_subscription_t *);
    int32_t (*unreg)(___notnull descriptor_event_subscription_t *);
};

#define descriptor_event_subscription_rem(subscription)                 ((subscription)->func->rem(subscription))
#define descriptor_event_subscription_on(subscription, type, param)     ((subscription)->func->on(subscription, type, param))
#define descriptor_event_subscription_reg(subscription)                 ((subscription)->func->reg(subscription))
#define descriptor_event_subscription_unreg(subscription)               ((subscription)->func->unreg(subscription))

#endif // __SNORLAX__DESCRIPTOR_EVENT_SUBSCRIPTION__H__
