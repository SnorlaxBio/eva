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
#include "../subscription/action.h"

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

/**
 * @fn          extern descriptor_event_generator_epoll_t * descriptor_event_generator_epoll_gen(void)
 */
extern descriptor_event_generator_epoll_t * descriptor_event_generator_epoll_gen(void) {
    descriptor_event_generator_epoll_t * generator = (descriptor_event_generator_epoll_t *) calloc(1, sizeof(descriptor_event_generator_epoll_t));

    generator->func = &func;

    generator->max = 1024;
    generator->events = (struct epoll_event *) malloc(sizeof(struct epoll_event) * generator->max);
    generator->timeout = 1;

    generator->fd = epoll_create(generator->max);

    generator->retry = 4;

    return generator;
}

/**
 * @fn          static int32_t descriptor_event_generator_epoll_func_on(descriptor_event_generator_epoll_t * generator)
 */
static int32_t descriptor_event_generator_epoll_func_on(descriptor_event_generator_epoll_t * generator) {
    int32_t ret = success;
    object_lock(generator);
    if(generator->fd <= invalid) {
        generator->fd = epoll_create(generator->max);
        if(generator->fd > invalid) {
            descriptor_event_subscription_t * subscription = generator->head;
            while(subscription) {
                object_lock(subscription);

                descriptor_event_generator_epoll_func_system_add(generator, subscription);

                descriptor_event_subscription_t * next = subscription->next;
                object_unlock(subscription);

                subscription = next;
            }
        } else {
            ret = fail;
        }
    }
    object_unlock(generator);

    return ret;
}

/**
 * @fn          static int32_t descriptor_event_generator_epoll_func_off(descriptor_event_generator_epoll_t * generator)
 */
static int32_t descriptor_event_generator_epoll_func_off(descriptor_event_generator_epoll_t * generator) {
    object_lock(generator);
    if(generator->fd > invalid) {
        close(generator->fd);
        generator->fd = invalid;
    }

    descriptor_event_subscription_t * subscription = generator->head;
    while(subscription) {
        object_lock(subscription);

        subscription->descriptor->status = subscription->descriptor->status & (~(descriptor_state_subscription_reg | descriptor_state_subscription_wait));
        descriptor_event_subscription_t * next = subscription->next;
        object_unlock(subscription);

        subscription = next;
    }
    object_unlock(generator);

    return success;
}

// TODO: QUEUE 를 사용하는 구조로 변경하자...
static int32_t descriptor_event_generator_epoll_func_pub(descriptor_event_generator_epoll_t * generator, event_queue_t * queue) {
#ifndef   RELEASE
    snorlaxdbg(generator == nil, "critical", "");
    snorlaxdbg(generator->fd <= invalid, "critical", "");

    snorlaxdbg(queue == nil, "critical", "");
#endif // RELEASE
    // 큐가 사용되지 않는다....

    struct epoll_event * events = (struct epoll_event *) generator->events;

    int32_t n = epoll_wait(generator->fd, events, generator->max, generator->timeout);

    if(n >= 0) {
        descriptor_event_subscription_t * subscription = nil;
        descriptor_t * descriptor = nil;
        for(int32_t i = 0; i < n; i++) {
            subscription = events[i].data.ptr;
            object_lock(subscription);
            descriptor = subscription->descriptor;
            int32_t flags = events[i].events;
            if(flags & (EPOLLERR | EPOLLPRI | EPOLLRDHUP | EPOLLHUP)) {
                descriptor->status = descriptor->status | descriptor_state_exception;
                descriptor_exception_t * exception = descriptor_exception_set(address_of(subscription->descriptor->exception), descriptor_exception_type_system, epoll_wait, (flags & (EPOLLERR | EPOLLPRI | EPOLLRDHUP | EPOLLHUP)));
                descriptor_event_subscription_dispatch(subscription, descriptor_action_exception, exception, queue);
                object_unlock(subscription);
                continue;
            }
            if(flags & EPOLLOUT) {
                descriptor->status = descriptor->status | descriptor_state_out;
                descriptor->status = descriptor->status & (~descriptor_state_subscription_wait_out);
                if(buffer_length(subscription->descriptor->buffer.out) > 0) {
                    if(descriptor_event_subscription_dispatch(subscription, descriptor_action_out, 0, queue) == fail) {
                        object_unlock(subscription);
                        continue;
                    }
                }
            }
            if(flags & EPOLLIN) {
                descriptor->status = descriptor->status | descriptor_state_in;
                descriptor->status = descriptor->status & (~descriptor_state_subscription_wait_in);
                if(descriptor_event_subscription_dispatch(subscription, descriptor_action_in, 0, queue) == fail) {
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
    descriptor_t * descriptor = subscription->descriptor;
    int32_t ret = event_generator_func_add((event_generator_t *) generator, (event_subscription_t *) subscription);

    if(ret == success) {
        descriptor->status = descriptor->status | descriptor_event_subscription_state_subscribed;

        if(generator->fd > invalid) {
            if(subscription->descriptor->value <= invalid) {
                ret = descriptor_event_subscription_action_func_open(subscription, 0);
            } else {
                ret = descriptor_event_generator_epoll_func_system_add(generator, subscription);
            }

            if(ret == success) {
                ret = descriptor_event_generator_epoll_func_system_add(generator, subscription);
            }
            
            if(ret == fail) {
                event_generator_func_del((event_generator_t *) generator, (event_subscription_t *) subscription);

                subscription->status = subscription->status & (~descriptor_event_subscription_state_subscribed);
            }
        } else {
            event_engine_push((event_t *) descriptor_event_gen(subscription, descriptor_event_type_open, 0));
        }
    } else {
        subscription->status = subscription->status & (~descriptor_event_subscription_state_subscribed);
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

    subscription->status = subscription->status & (~descriptor_event_subscription_state_subscribed);

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
        subscription->status = subscription->status | descriptor_event_subscription_state_registered;

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
        subscription->status = subscription->status | descriptor_event_subscription_state_registered;

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
        subscription->status = subscription->status & (~(descriptor_event_subscription_state_wait | descriptor_event_subscription_state_registered));
    }

    return ret;
}