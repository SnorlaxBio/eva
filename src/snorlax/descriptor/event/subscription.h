/**
 * @file        snorlax/descriptor/event/subscription.h
 * @brief
 * @details
 * 
 * @author      snorlax <opuntia@snorlax.bio>
 * @since       June 25, 2024
 */

#ifndef   __SNORLAX__DESCRIPTOR_EVENT_SUBSCRIPTION__H__
#define   __SNORLAX__DESCRIPTOR_EVENT_SUBSCRIPTION__H__

#include <snorlax/descriptor.h>
#include <snorlax/descriptor/event.h>

struct descriptor_event_subscription {
    descriptor_event_subscription_func_t * func;
    sync_t * sync;
    descriptor_event_generator_t * generator;
    descriptor_event_subscription_t * prev;
    descriptor_event_subscription_t * next;
    event_subscription_event_queue_t * queue;
    descriptor_event_subscription_handler_t * handler;
    uint32_t type;
    uint32_t status;
    descriptor_t * descriptor;
};

struct descriptor_event_subscription_func {
    descriptor_event_subscription_t * (*rem)(___notnull descriptor_event_subscription_t *);
    void (*on)(___notnull descriptor_event_subscription_t *, descriptor_event_subscription_process_t, uint32_t, event_subscription_event_t *);
    void (*notify)(___notnull descriptor_event_subscription_t *, uint32_t, event_subscription_event_t *);
};

extern descriptor_event_subscription_t * descriptor_event_subscription_gen(___notnull descriptor_t * descriptor, descriptor_event_subscription_handler_t * handler);

extern void descriptor_event_subscription_func_on(___notnull descriptor_event_subscription_t * subscription, descriptor_event_subscription_process_t process, uint32_t type, event_subscription_event_t * node);
extern void descriptor_event_subscription_func_notify(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node);

extern int64_t descriptor_event_subscription_write(___notnull descriptor_event_subscription_t * subscription, const uint8_t * data, uint64_t datalen);

#define descriptor_event_subscription_buffer_get(subscription)                  (&((descriptor_event_subscription_t *)(subscription))->descriptor->buffer)

#define descriptor_event_subscription_node_gen(subscription)                    (event_subscription_event_gen((event_subscription_t *) subscription))

#define descriptor_event_subscription_rem(subscription)                         ((subscription)->func->rem(subscription))
#define descriptor_event_subscription_on(subscription, process, type, node)     ((subscription)->func->on(subscription, process, type, node))
#define descriptor_event_subscription_notify(subscription, type, node)          ((subscription)->func->notify(subscription, type, node))

#endif // __SNORLAX__DESCRIPTOR_EVENT_SUBSCRIPTION__H__
