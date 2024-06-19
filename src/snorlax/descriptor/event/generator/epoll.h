/**
 * @file        snorlax/descriptor/event/generator/epoll.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 19, 2024
 */

#ifndef   __SNORLAX__DESCRIPTOR_EVENT_GENERATOR_EPOLL__H__
#define   __SNORLAX__DESCRIPTOR_EVENT_GENERATOR_EPOLL__H__

#include <snorlax/descriptor.h>

#include <snorlax/eva.h>

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

    int32_t fd;
    void * events;
    int32_t max;
    int32_t timeout;
};

struct descriptor_event_generator_epoll_func {
    descriptor_event_generator_epoll_t * (*rem)(descriptor_event_generator_epoll_t *);
    int32_t (*on)(descriptor_event_generator_epoll_t *);
    int32_t (*off)(descriptor_event_generator_epoll_t *);
    int32_t (*pub)(descriptor_event_generator_epoll_t *, event_queue_t *);
    int32_t (*add)(descriptor_event_generator_epoll_t *, descriptor_event_subscription_t *);
    int32_t (*del)(descriptor_event_generator_epoll_t *, descriptor_event_subscription_t *);
};

extern descriptor_event_generator_epoll_t * descriptor_event_generator_epoll_gen(void);

#define descriptor_event_generator_epoll_rem(generator)                     ((generator)->func->rem(generator))
#define descriptor_event_generator_epoll_on(generator)                      ((generator)->func->on(generator))
#define descriptor_event_generator_epoll_off(generator)                     ((generator)->func->off(generator))
#define descriptor_event_generator_epoll_pub(generator, queue)              ((generator)->func->pub(generator, queue))
#define descriptor_event_generator_epoll_add(generator, subscription)       ((generator)->func->add(generator, subscription))
#define descriptor_event_generator_epoll_del(generator, subscription)       ((generator)->func->del(generator, subscription))

#endif // __SNORLAX__DESCRIPTOR_EVENT_GENERATOR_EPOLL__H__
