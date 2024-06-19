/**
 * @file        snorlax/descriptor/event/generator/epoll.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 19, 2024
 */

#include <sys/epoll.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include "epoll.h"

#include "../../event.h"
#include "../subscription.h"

#include "../../../event/generator.h"

static descriptor_event_generator_epoll_t * descriptor_event_generator_epoll_func_rem(descriptor_event_generator_epoll_t * generator);
static int32_t descriptor_event_generator_epoll_func_on(descriptor_event_generator_epoll_t * generator);
static int32_t descriptor_event_generator_epoll_func_off(descriptor_event_generator_epoll_t * generator);
static int32_t descriptor_event_generator_epoll_func_pub(descriptor_event_generator_epoll_t * generator, event_queue_t * queue);
static int32_t descriptor_event_generator_epoll_func_add(descriptor_event_generator_epoll_t * generator, descriptor_event_subscription_t * subscription);
static int32_t descriptor_event_generator_epoll_func_del(descriptor_event_generator_epoll_t * generator, descriptor_event_subscription_t * subscription);

static int32_t descriptor_event_generator_epoll_func_system_add(descriptor_event_generator_epoll_t * generator, descriptor_event_subscription_t * subscription);
static int32_t descriptor_event_generator_epoll_func_system_mod(descriptor_event_generator_epoll_t * generator, descriptor_event_subscription_t * subscription);
static int32_t descriptor_event_generator_epoll_func_system_del(descriptor_event_generator_epoll_t * generator, descriptor_event_subscription_t * subscription);

static descriptor_event_generator_epoll_func_t func = {
    descriptor_event_generator_epoll_func_rem,
    descriptor_event_generator_epoll_func_on,
    descriptor_event_generator_epoll_func_off,
    descriptor_event_generator_epoll_func_pub,
    descriptor_event_generator_epoll_func_add,
    descriptor_event_generator_epoll_func_del
};

extern descriptor_event_generator_epoll_t * descriptor_event_generator_epoll_gen(void) {
    descriptor_event_generator_epoll_t * generator = (descriptor_event_generator_epoll_t *) calloc(1, sizeof(descriptor_event_generator_epoll_t));

    generator->func = &func;

    generator->max = 1024;
    generator->events = (struct epoll_event *) malloc(sizeof(struct epoll_event) * generator->max);
    generator->timeout = 1;

    generator->fd = epoll_create(generator->max);

    return generator;
}

static int32_t descriptor_event_generator_epoll_func_on(descriptor_event_generator_epoll_t * generator) {
    object_lock(generator);
    if(generator->fd <= invalid) {
        generator->fd = epoll_create(generator->max);
        // TODO: CHECK fd is invalid

        descriptor_event_subscription_t * subscription = generator->head;
        while(subscription) {
            object_lock(subscription);

            if(subscription->descriptor->value > invalid) {
                descriptor_event_generator_epoll_func_system_add(generator, subscription);
            } else {
                // TODO: OPEN DESCRIPTOR LOGIC
            }

            descriptor_event_subscription_t * next = subscription->next;
            object_unlock(subscription);

            subscription = next;
        }
    }
    object_unlock(generator);

    return success;
}

static int32_t descriptor_event_generator_epoll_func_off(descriptor_event_generator_epoll_t * generator) {
    object_lock(generator);
    if(generator->fd > invalid) {
        close(generator->fd);
        generator->fd = invalid;
    }

    descriptor_event_subscription_t * subscription = generator->head;
    while(subscription) {
        object_lock(subscription);
        subscription->status = subscription->status & (~descriptor_event_subscription_state_wait);
        descriptor_event_subscription_t * next = subscription->next;
        object_unlock(subscription);

        subscription = next;
    }
    object_unlock(generator);

    return success;
}

/**
 * 
 */
static int32_t descriptor_event_generator_epoll_func_pub(descriptor_event_generator_epoll_t * generator, event_queue_t * queue) {
#ifndef   RELEASE
    snorlaxdbg(generator == nil, "critical", "");
    snorlaxdbg(generator->fd <= invalid, "critical", "");

    snorlaxdbg(queue == nil, "critical", "");
#endif // RELEASE
    struct epoll_event * events = (struct epoll_event *) generator->events;

    int32_t n = epoll_wait(generator->fd, events, generator->max, generator->timeout);

    if(n >= 0) {
        descriptor_event_subscription_t * subscription = nil;
        for(int32_t i = 0; i < n; i++) {
            subscription = events[i].data.ptr;
            object_lock(subscription);
            int32_t flags = events[i].events;
            if(flags & (EPOLLERR | EPOLLPRI | EPOLLRDHUP | EPOLLHUP)) {
                // TODO: SUBSCRIPTION STATUS & DESCRIPTOR STATUS
                uint64_t param = uint64_from_uint32x2(descriptor_action_system_engine ,(flags & (EPOLLERR | EPOLLPRI | EPOLLRDHUP | EPOLLHUP)));
                event_engine_subscription_dispatch(subscription, descriptor_event_type_exception, param, generator);
                object_unlock(subscription);
                continue;
            }
            if(flags & EPOLLOUT) {
                subscription->status = subscription->status & (~descriptor_event_subscription_state_wait_out);
                subscription->descriptor->status = subscription->descriptor->status | descriptor_state_out;
                if(buffer_length(subscription->descriptor->buffer.out) > 0) {
                    if(event_engine_subscription_dispatch(subscription, descriptor_event_type_out, 0, generator) == fail) {
                        object_unlock(subscription);
                        continue;
                    }
                }
            }
            if(flags & EPOLLIN) {
                subscription->status = subscription->status & (~descriptor_event_subscription_state_wait_in);
                subscription->descriptor->status = subscription->descriptor->status | descriptor_state_in;
                if(event_engine_subscription_dispatch(subscription, descriptor_event_type_in, 0, generator) == fail) {
                    object_unlock(subscription);
                    continue;
                }
            }
            object_unlock(subscription);
        }
    } else {
        int err = errno;

        if(err == EINTR) {
            n = success;
        } else {
            snorlaxdbg(false, "error", "epoll_wait(...) => %d", err);
        }
    }

    return n;
}

/**
 * @fn          static int32_t descriptor_event_generator_epoll_func_add(descriptor_event_generator_epoll_t * generator, descriptor_event_subscription_t * subscription)
 * @brief
 * @details
 */
static int32_t descriptor_event_generator_epoll_func_add(descriptor_event_generator_epoll_t * generator, descriptor_event_subscription_t * subscription) {
#ifndef   REELASE
    snorlaxdbg(generator == nil, "critical", "");

    snorlaxdbg(subscription == nil, "critical", "");
    snorlaxdbg(subscription->descriptor == nil, "critical", "");
    snorlaxdbg(subscription->generator != nil, "critical", "");
#endif // RELEASE
    int32_t ret = event_generator_func_add((event_generator_t *) generator, (event_subscription_t *) subscription);

    if(ret == success) {
        if(generator->fd > invalid) {
            if(subscription->descriptor->value <= invalid) {
                ret = descriptor_open(subscription->descriptor);
                if(ret == success && (subscription->descriptor->status & descriptor_state_open) == 0) {
                    descriptor_event_subscription_on(subscription, descriptor_event_type_open, success);
                }
            }

            if(ret == success) {
                ret = descriptor_event_generator_epoll_func_system_add(generator, subscription);
            }
            
            if(ret == fail) {
                event_generator_func_del((event_generator_t *) generator, (event_subscription_t *) subscription);
            }
        } else {
            event_engine_push((event_t *) descriptor_event_gen(subscription, descriptor_event_type_open, 0));
        }
    }

    return ret;
}

/**
 * @fn          static int32_t descriptor_event_generator_epoll_func_del(descriptor_event_generator_epoll_t * generator, descriptor_event_subscription_t * subscription)
 * @brief
 * @details
 */
static int32_t descriptor_event_generator_epoll_func_del(descriptor_event_generator_epoll_t * generator, descriptor_event_subscription_t * subscription) {
#ifndef   REELASE
    snorlaxdbg(generator == nil, "critical", "");

    snorlaxdbg(subscription == nil, "critical", "");
    snorlaxdbg(subscription->descriptor == nil, "critical", "");
#endif // RELEASE
    if(generator->fd > invalid && subscription->descriptor->value > invalid) {
        descriptor_event_generator_epoll_func_system_del(generator, subscription);
    }
    
    event_generator_func_del((event_generator_t *) generator, (event_subscription_t *) subscription);

    return success;
}

/**
 * @fn          static int32_t descriptor_event_generator_epoll_func_system_add(descriptor_event_generator_epoll_t * generator, descriptor_event_subscription_t * subscription)
 * @brief
 * @details
 */
static int32_t descriptor_event_generator_epoll_func_system_add(descriptor_event_generator_epoll_t * generator, descriptor_event_subscription_t * subscription) {
#ifndef   REELASE
    snorlaxdbg(generator == nil, "critical", "");
    snorlaxdbg(generator->fd <= invalid, "critical", "");

    snorlaxdbg(subscription == nil, "critical", "");
    snorlaxdbg(subscription->descriptor == nil, "critical", "");
    snorlaxdbg(subscription->descriptor->value <= invalid, "critical", "");
#endif // RELEASE

    int32_t fd = subscription->descriptor->value;
    int32_t epoll = generator->fd;

    struct epoll_event event;
    event.data.ptr = subscription;
    event.events = (EPOLLET | EPOLLONESHOT);

    event.events = event.events | (EPOLLHUP | EPOLLPRI | EPOLLERR | EPOLLRDHUP);

    if(subscription->interest & descriptor_event_subscription_interest_in) event.events = event.events | EPOLLIN;
    if(subscription->interest & descriptor_event_subscription_interest_out) event.events = event.events | EPOLLOUT;
    
    int32_t ret = epoll_ctl(epoll, EPOLL_CTL_ADD, fd, &event);

    if(ret != success) {
        int err = errno;

        if(err == EEXIST) {
            ret = epoll_ctl(epoll, EPOLL_CTL_MOD, fd, &event);
            if(ret != success) {
                descriptor_event_subscription_descriptor_exception_set(subscription, descriptor_exception_type_system, epoll_ctl, err);
#ifndef   RELEASE
                snorlaxdbg(epoll_ctl, "critical", "errno => %d", err);
#endif // RELEASE
            }
        } else {
            descriptor_event_subscription_descriptor_exception_set(subscription, descriptor_exception_type_system, epoll_ctl, err);
#ifndef   RELEASE
            snorlaxdbg(epoll_ctl, "critical", "errno => %d", err);
#endif // RELEASE
        }
    }

    if(ret == success) {
        if(subscription->interest & descriptor_event_subscription_interest_in) {
            subscription->status = subscription->status | descriptor_event_subscription_state_wait_in;
        }
        if(subscription->interest & descriptor_event_subscription_interest_out) {
            subscription->status = subscription->status | descriptor_event_subscription_state_wait_out;
        }
    }

    return ret;
}

/**
 * @fn          static int32_t descriptor_event_generator_epoll_func_system_mod(descriptor_event_generator_epoll_t * generator, descriptor_event_subscription_t * subscription)
 * @brief
 * @details
 */
static int32_t descriptor_event_generator_epoll_func_system_mod(descriptor_event_generator_epoll_t * generator, descriptor_event_subscription_t * subscription) {
#ifndef   REELASE
    snorlaxdbg(generator == nil, "critical", "");
    snorlaxdbg(generator->fd <= invalid, "critical", "");

    snorlaxdbg(subscription == nil, "critical", "");
    snorlaxdbg(subscription->descriptor == nil, "critical", "");
    snorlaxdbg(subscription->descriptor->value <= invalid, "critical", "");
#endif // RELEASE

    int32_t epoll = generator->fd;
    int32_t fd = subscription->descriptor->value;

    struct epoll_event event;
    event.data.ptr = subscription;
    event.events = (EPOLLET | EPOLLONESHOT);

    event.events = event.events | (EPOLLHUP | EPOLLPRI | EPOLLERR | EPOLLRDHUP);

    if(subscription->interest & descriptor_event_subscription_interest_in) event.events = event.events | EPOLLIN;
    if(subscription->interest & descriptor_event_subscription_interest_out) event.events = event.events | EPOLLOUT;
    
    int32_t ret = epoll_ctl(epoll, EPOLL_CTL_MOD, fd, &event);

    if(ret != success) {
        int err = errno;

        if(err == ENOENT) {
            ret = epoll_ctl(epoll, EPOLL_CTL_ADD, fd, &event);
            if(ret != success) {
                descriptor_event_subscription_descriptor_exception_set(subscription, descriptor_exception_type_system, epoll_ctl, err);
#ifndef   RELEASE
                snorlaxdbg(epoll_ctl, "critical", "errno => %d", err);
#endif // RELEASE
            }
        } else {
            descriptor_event_subscription_descriptor_exception_set(subscription, descriptor_exception_type_system, epoll_ctl, err);
#ifndef   RELEASE
            snorlaxdbg(epoll_ctl, "critical", "errno => %d", err);
#endif // RELEASE
        }
    }

    if(ret == success) {
        if(subscription->interest & descriptor_event_subscription_interest_in) {
            subscription->status = subscription->status | descriptor_event_subscription_state_wait_in;
        }
        if(subscription->interest & descriptor_event_subscription_interest_out) {
            subscription->status = subscription->status | descriptor_event_subscription_state_wait_out;
        }
    }

    return ret;
}

/**
 * @fn          static int32_t descriptor_event_generator_epoll_func_system_del(___notnull descriptor_event_generator_epoll_t * generator, ___notnull descriptor_event_subscription_t * subscription)
 * @brief
 * @details
 * 
 * 
 * @param[in]   generator
 * @param[in]   subscription
 * 
 * @return      | int32_t |
 */
static int32_t descriptor_event_generator_epoll_func_system_del(___notnull descriptor_event_generator_epoll_t * generator, ___notnull descriptor_event_subscription_t * subscription) {
#ifndef   RELEASE
    snorlaxdbg(generator == nil, "critical", "");
    snorlaxdbg(generator->fd <= invalid, "critical", "");

    snorlaxdbg(subscription == nil, "critical", "");
    snorlaxdbg(subscription->descriptor == nil, "critical", "");
    snorlaxdbg(subscription->descriptor->value <= invalid, "critical", "");
#endif // RELEASE

    int32_t fd = subscription->descriptor->value;
    int32_t epoll = generator->fd;
    struct epoll_event event;

    event.data.ptr = subscription;
    event.events = (EPOLLET | EPOLLONESHOT | EPOLLHUP | EPOLLPRI | EPOLLERR | EPOLLRDHUP | EPOLLIN | EPOLLOUT);
    
    int32_t ret = epoll_ctl(epoll, EPOLL_CTL_DEL, fd, &event);

    if(ret == fail) {
        int err = errno;

        if(err == ENOENT) {
            ret == success;
        } else {
            descriptor_event_subscription_descriptor_exception_set(subscription, descriptor_exception_type_system, epoll_ctl, err);
#ifndef   RELEASE
            snorlaxdbg(epoll_ctl, "critical", "errno => %d", err);
#endif // RELEASE
        }
    }

    if(ret == success) {
        subscription->status = subscription->status & (~descriptor_event_subscription_state_wait);
    }

    return ret;
}