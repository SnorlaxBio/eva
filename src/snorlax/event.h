/**
 * @file        snorlax/event.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 17, 2024
 */

#ifndef   __SNORLAX__EVENT__H__
#define   __SNORLAX__EVENT__H__

#include <snorlax/eva.h>

struct event {
    event_func_t * func;
    sync_t * sync;

    event_queue_t * queue;
    event_t * prev;
    event_t * next;

    event_subscription_event_t * node;

    event_subscription_t * subscription;
    uint32_t type;
};

struct event_func {
    event_t * (*rem)(event_t *);
    int32_t (*on)(event_t *);
};

// TODO: UPGRADE PARAM event: event_t * 를 없애야 한다.
//       공통적으로 표준적이지 않다. 지금 바꾸자.
extern event_t * event_gen(event_subscription_t * subscription, uint32_t type);

#define event_rem(event)        ((event)->func->rem(event))
#define event_on(event)         ((event)->func->on(event))

#endif // __SNORLAX__EVENT__H__
