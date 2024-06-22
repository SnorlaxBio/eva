/**
 * @file        snorlax/descriptor/event/generator/epoll.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 22, 2024
 */

#include <stdlib.h>
#include <errno.h>
#include <sys/epoll.h>

#include "epoll.h"

#include "state.h"
#include "../type.h"

#include "../subscription.h"

#include "../../event/subscription/event.h"

static descriptor_event_generator_epoll_t * descriptor_event_generator_epoll_func_rem(___notnull descriptor_event_generator_epoll_t * generator);
static ___sync int32_t descriptor_event_generator_epoll_func_on(___notnull descriptor_event_generator_epoll_t * generator);
static ___sync int32_t descriptor_event_generator_epoll_func_off(___notnull descriptor_event_generator_epoll_t * generator);
static ___sync int32_t descriptor_event_generator_epoll_func_pub(___notnull descriptor_event_generator_epoll_t * generator, event_queue_t * queue);
    // ___sync int32_t (*add)(___notnull descriptor_event_generator_epoll_t *, ___notnull descriptor_event_subscription_t *);
    // ___sync int32_t (*del)(___notnull descriptor_event_generator_epoll_t *, ___notnull descriptor_event_subscription_t *);
    // ___sync void (*clear)(___notnull descriptor_event_generator_epoll_t *);
    // ___notsync int32_t (*reg)(___notnull descriptor_event_generator_epoll_t *, ___notnull descriptor_event_subscription_t *);
    // ___notsync int32_t (*unreg)(___notnull descriptor_event_generator_epoll_t *, ___notnull descriptor_event_subscription_t *);


static descriptor_event_generator_epoll_func_t func = {
    descriptor_event_generator_epoll_func_rem,
    descriptor_event_generator_epoll_func_on,
    descriptor_event_generator_epoll_func_off,
    descriptor_event_generator_epoll_func_pub
};

extern descriptor_event_generator_epoll_t * descriptor_event_generator_epoll_gen(___notnull event_generator_set_t * set) {
    descriptor_event_generator_epoll_t * generator = (descriptor_event_generator_epoll_t *) calloc(1, sizeof(descriptor_event_generator_epoll_t));

    generator->func = address_of(func);
    generator->value = invalid;
    generator->set = set;

    return generator;
}

static descriptor_event_generator_epoll_t * descriptor_event_generator_epoll_func_rem(___notnull descriptor_event_generator_epoll_t * generator) {
#ifndef   RELEASE
    snorlaxdbg(generator == nil, false, "critical", "");
#endif // RELEASE
    descriptor_event_generator_epoll_off(generator);
    descriptor_event_generator_epoll_clear(generator);

    object_lock(generator);

    if(generator->value > invalid) {
        if(close(generator->value) == fail) {
#ifndef   RELEASE
            snorlaxdbg(false, true, "warning", "fail to close => %d", errno);
#endif // RELEASE
        }

        generator->value = invalid;
    }

    generator->events = memory_rem(generator->events);

    object_unlock(generator);

    generator->sync = sync_rem(generator->sync);

    free(generator);
    return nil;
}

static ___sync int32_t descriptor_event_generator_epoll_func_on(___notnull descriptor_event_generator_epoll_t * generator) {
#ifndef   RELEASE
    snorlaxdbg(generator == nil, false, "critical", "");
#endif // RELEASE

    object_lock(generator);

    if((generator->status & descriptor_event_generator_state_on) == 0) {
        generator->status = generator->status | descriptor_event_generator_state_on;
        generator->timeout = 1;
        generator->max = 1024;

        generator->events = memory_gen(generator->events, sizeof(struct epoll_event) * generator->max);
        if(generator->value <= invalid) {
            generator->value = epoll_create(generator->max);
        }

        for(descriptor_event_subscription_t * subscription = generator->head; subscription != nil; subscription = subscription->next) {
            object_unlock(generator);

            object_lock(subscription);
            descriptor_event_generator_epoll_reg(generator, subscription);
            object_unlock(subscription);

            object_lock(generator);
        }
    } else {
#ifndef   RELEASE
        snorlaxdbg(false, true, "notice", "generator is already on");
#endif // RELEASE
    }



    object_unlock(generator);

    return success;
}

static ___sync int32_t descriptor_event_generator_epoll_func_off(___notnull descriptor_event_generator_epoll_t * generator) {
#ifndef   RELEASE
    snorlaxdbg(generator == nil, false, "critical", "");
#endif // RELEASE

    object_lock(generator);

    if(generator->status & descriptor_event_generator_state_on) {
        generator->status = generator->status & (~descriptor_event_generator_state_on);
        if(generator->value > invalid) {
            if(close(generator->value) == fail) {
#ifndef   RELEASE
                snorlaxdbg(false, true, "warning", "fail to close => %d", errno);
#endif // RELEASE
            }

            generator->value = invalid;
        }
        generator->events = memory_rem(generator->events);

        for(descriptor_event_subscription_t * subscription = generator->head; subscription != nil; subscription = subscription->next) {
            object_unlock(generator);

            object_lock(subscription);
            descriptor_event_generator_epoll_unreg(generator, subscription);
            object_unlock(subscription);

            object_lock(generator);
        }
    }

    object_unlock(generator);

    return success;
}

static ___notsync int32_t descriptor_event_generator_epoll_func_pub(___notnull descriptor_event_generator_epoll_t * generator, event_queue_t * queue) {
#ifndef   RELEASE
    snorlaxdbg(generator == nil, false, "critical", "");
#endif // RELEASE

    struct epoll_event * events = (struct epoll_event *) generator->events;
    int32_t nfds = epoll_wait(generator->value, events, generator->max, generator->timeout);

    int32_t n = 0;

    if(nfds >= 0) {
        descriptor_event_subscription_t * subscription = nil;
        for(int32_t i = 0; i < nfds; i++) {
            subscription = (descriptor_event_subscription_t *) events[i].data.ptr;
            object_lock(subscription);
            // 큐에 삽입을 하는 로직을 만든다.... 
            if(events[i].events & (EPOLLERR | EPOLLPRI | EPOLLHUP | EPOLLRDHUP)) {
                descriptor_exception_t * exception = descriptor_exception_set(subscription->descriptor, descriptor_exception_type_system, events[i].events & (EPOLLERR | EPOLLPRI | EPOLLHUP | EPOLLRDHUP), epoll_wait);
                if(queue) {
                    event_subscription_event_t * node = event_subscription_event_gen((event_subscription_t *) subscription, (uint64_t) exception);
                    event_t * event = event_gen((event_subscription_t *) subscription, descriptor_event_type_exception, node);
                    event_queue_push(queue, event);
                } else {
                    descriptor_event_subscription_on(subscription, descriptor_event_type_exception, (uint64_t) exception);
                }
                object_unlock(subscription);
                continue;
            }
            if(events[i].events & EPOLLOUT) {
                subscription->descriptor->status = subscription->descriptor->status | descriptor_state_write;
                if((events[i].events & EPOLLIN) == 0) {
                    if(queue) {
                        event_subscription_event_t * node = event_subscription_event_gen((event_subscription_t *) subscription, 0);
                        event_t * event = event_gen((event_subscription_t *) subscription, descriptor_event_type_out, node);
                        event_queue_push(queue, event);
                    } else {
                        descriptor_event_subscription_on(subscription, descriptor_event_type_out, 0);
                    }
                }
            }
            if(events[i].events & EPOLLIN) {
                subscription->descriptor->status = subscription->descriptor->status | descriptor_state_read;
                if(queue) {
                    event_subscription_event_t * node = event_subscription_event_gen((event_subscription_t *) subscription, 0);
                    event_t * event = event_gen((event_subscription_t *) subscription, descriptor_event_type_in, node);
                    event_queue_push(queue, event);
                } else {
                    descriptor_event_subscription_on(subscription, descriptor_event_type_in, 0);
                }
                object_unlock(subscription);
                continue;
            }
            object_unlock(subscription);
        }
    } else {
#ifndef   RELEASE
        snorlaxdbg(false, true, "warning", "fail to epoll_wait(...) caused by %d", errno);
#endif // RELEASE
    }

    return n;
}