/**
 * @file        snorlax/descriptor/event/generator/epoll.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 22, 2024
 */

#ifndef   __SNORLAX__DESCRIPTOR_EVENT_GENERATOR_EPOLL__H__
#define   __SNORLAX__DESCRIPTOR_EVENT_GENERATOR_EPOLL__H__

#include <snorlax/descriptor/event.h>

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
    event_generator_set_t * set;
    uint32_t status;

    int value;
    int timeout;
    int max;
    void * events;
};

struct descriptor_event_generator_epoll_func {
    descriptor_event_generator_epoll_t * (*rem)(___notnull descriptor_event_generator_epoll_t *);
    ___sync int32_t (*on)(___notnull descriptor_event_generator_epoll_t *);
    ___sync int32_t (*off)(___notnull descriptor_event_generator_epoll_t *);
    ___notsync int32_t (*pub)(___notnull descriptor_event_generator_epoll_t *, event_queue_t *);
    ___sync int32_t (*add)(___notnull descriptor_event_generator_epoll_t *, ___notnull descriptor_event_subscription_t *);
    ___sync int32_t (*del)(___notnull descriptor_event_generator_epoll_t *, ___notnull descriptor_event_subscription_t *);
    ___sync void (*clear)(___notnull descriptor_event_generator_epoll_t *);
    ___notsync int32_t (*reg)(___notnull descriptor_event_generator_epoll_t *, ___notnull descriptor_event_subscription_t *);
    ___notsync int32_t (*unreg)(___notnull descriptor_event_generator_epoll_t *, ___notnull descriptor_event_subscription_t *);
};

extern descriptor_event_generator_epoll_t * descriptor_event_generator_epoll_gen(___notnull event_generator_set_t * set);

#define descriptor_event_generator_epoll_rem(generator)                     ((generator)->func->rem(generator))
#define descriptor_event_generator_epoll_on(generator)                      ((generator)->func->on(generator))
#define descriptor_event_generator_epoll_off(generator)                     ((generator)->func->off(generator))
#define descriptor_event_generator_epoll_pub(generator, queue)              ((generator)->func->pub(generator, queue))
#define descriptor_event_generator_epoll_add(generator, subscription)       ((generator)->func->add(generator, subscription))
#define descriptor_event_generator_epoll_del(generator, subscription)       ((generator)->func->del(generator, subscription))
#define descriptor_event_generator_epoll_clear(generator)                   ((generator)->func->clear(generator))
#define descriptor_event_generator_epoll_reg(generator, subscription)       ((generator)->func->reg(generator, subscription))
#define descriptor_event_generator_epoll_unreg(generator, subscription)     ((generator)->func->unreg(generator, subscription))

#endif // __SNORLAX__DESCRIPTOR_EVENT_GENERATOR_EPOLL__H__
