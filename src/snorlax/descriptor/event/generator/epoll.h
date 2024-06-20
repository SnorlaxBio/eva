/**
 * @file        snorlax/descriptor/event/generator/epoll.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 21, 2024
 */

#ifndef   __SNORLAX__DESCRIPTOR_EVENT_GENERATOR_EPOLL__H__
#define   __SNORLAX__DESCRIPTOR_EVENT_GENERATOR_EPOLL__H__

#include <snorlax/descriptor.h>

struct descriptor_event_generator_epoll;
struct descriptor_event_generator_epoll_func;
typedef struct descriptor_event_generator_epoll descriptor_event_generator_epoll_t;
typedef struct descriptor_event_generator_epoll_func descriptor_event_generator_epoll_func_t;

struct descriptor_event_generator_epoll {
    descriptor_event_generator_epoll_func_t * func;
    sync_t * sync;

    uint64_t size;
    descriptor_event_subscription_t * head;
    descriptor_event_subscription_t * tail;

    int32_t epoll;
    int32_t timeout;
    int32_t max;
    address_t events;
};

struct descriptor_event_generator_epoll_func {
    descriptor_event_generator_epoll_t * (*rem)(___notnull descriptor_event_generator_epoll_t *);
    int32_t (*on)(___notnull descriptor_event_generator_epoll_t *);
    int32_t (*off)(___notnull descriptor_event_generator_epoll_t *);
    int32_t (*pub)(___notnull descriptor_event_generator_epoll_t *, event_queue_t *);
    int32_t (*add)(___notnull descriptor_event_generator_epoll_t *, ___notnull descriptor_event_subscription_t *);
    int32_t (*del)(___notnull descriptor_event_generator_epoll_t *, ___notnull descriptor_event_subscription_t *);
};

extern descriptor_event_generator_epoll_t * descriptor_event_generator_epoll_gen(void);

#define descriptor_event_generator_epoll_rem(generator)                 (generator ? generator->func->rem(generator) : nil)
#define descriptor_event_generator_epoll_on(generator)                  (generator ? generator->func->on(generator) : fail)
#define descriptor_event_generator_epoll_off(generator)                 (generator ? generator->func->off(generator) : fail)
#define descriptor_event_generator_epoll_pub(generator, queue)          (generator ? generator->func->pub(generator, queue) : fail)
#define descriptor_event_generator_epoll_add(generator, subscription)   (generator ? generator->func->add(generator, subscription) : fail)
#define descriptor_event_generator_epoll_del(generator, subscription)   (generator ? generator->func->del(generator, subscription) : fail)

#endif // __SNORLAX__DESCRIPTOR_EVENT_GENERATOR_EPOLL__H__
