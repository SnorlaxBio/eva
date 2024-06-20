/**
 * @file            snorlax/descriptor/event.h
 * @brief
 * @details
 * 
 * @author          snorlax <ceo@snorlax.bio>
 * @since           June 20, 2024
 */

#ifndef   __SNORLAX__DESCRIPTOR_EVENT__H__
#define   __SNORLAX__DESCRIPTOR_EVENT__H__

#include <snorlax/descriptor.h>

struct descriptor_event {
    descriptor_event_func_t * func;
    sync_t * sync;

    event_queue_t * queue;
    event_t * prev;
    event_t * next;

    event_subscription_event_t * node;

    descriptor_event_subscription_t * subscription;
    uint32_t type;
    uint64_t param;
};

struct descriptor_event_func {
    descriptor_event_t * (*rem)(___notnull descriptor_event_t *);
    void (*on)(___notnull descriptor_event_t *);
};

extern descriptor_event_t * descriptor_event_gen(___notnull descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param);

extern descriptor_event_t * descriptor_event_func_rem(___notnull descriptor_event_t * event);
extern void descriptor_event_func_on(___notnull descriptor_event_t * event);

#define descriptor_event_rem(event)     ((event)->func->rem(event))
#define descriptor_event_on(event)      ((event)->func->on(event))

#endif // __SNORLAX__DESCRIPTOR_EVENT__H__
