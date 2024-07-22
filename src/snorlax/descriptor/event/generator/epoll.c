/**
 * @file        snorlax/descriptor/event/generator/epoll.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 25, 2024
 */

#include <sys/epoll.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>

#include "epoll.h"

#include "epoll/subscription/state.h"
#include "epoll/subscription/process.h"
#include "epoll/control/type.h"

#include "../subscription.h"
#include "../subscription/type.h"
#include "../type.h"

#include "../../../event/queue.h"
#include "../../../event/engine.h"
#include "../../../event/generator.h"
#include "../../../event/generator/state.h"
#include "../../../event/generator/set.h"
#include "../../../event/subscription/event/queue.h"
#include "../../../event/subscription/event.h"

#include "../../../descriptor.h"

static void descriptor_event_generator_epoll_func_dispatch(descriptor_event_subscription_t * subscription, uint32_t type, event_queue_t * queue, event_engine_t * engine);

static ___notsync int32_t descriptor_event_generator_epoll_func_control_add(___notnull descriptor_event_generator_epoll_t * generator, ___notnull descriptor_event_subscription_t * subscription);
static ___notsync int32_t descriptor_event_generator_epoll_func_control_mod(___notnull descriptor_event_generator_epoll_t * generator, ___notnull descriptor_event_subscription_t * subscription);
static ___notsync int32_t descriptor_event_generator_epoll_func_control_del(___notnull descriptor_event_generator_epoll_t * generator, ___notnull descriptor_event_subscription_t * subscription);

static descriptor_event_generator_epoll_t * descriptor_event_generator_epoll_func_rem(___notnull descriptor_event_generator_epoll_t * generator);
static ___sync int32_t descriptor_event_generator_epoll_func_on(___notnull descriptor_event_generator_epoll_t * generator);
static ___sync int32_t descriptor_event_generator_epoll_func_off(___notnull descriptor_event_generator_epoll_t * generator);
static ___sync int32_t descriptor_event_generator_epoll_func_pub(___notnull descriptor_event_generator_epoll_t * generator, event_queue_t * queue);
static ___sync int32_t descriptor_event_generator_epoll_func_add(___notnull descriptor_event_generator_epoll_t * generator, ___notnull descriptor_event_subscription_t * subscription);
static ___sync int32_t descriptor_event_generator_epoll_func_del(___notnull descriptor_event_generator_epoll_t * generator, ___notnull descriptor_event_subscription_t * subscription);
static ___sync void descriptor_event_generator_epoll_func_clear(___notnull descriptor_event_generator_epoll_t * generator);
static ___notsync int32_t descriptor_event_generator_epoll_func_control(___notnull descriptor_event_generator_epoll_t * generator, ___notnull descriptor_event_subscription_t * subscription, uint32_t type);

static descriptor_event_generator_epoll_func_t func = {
    descriptor_event_generator_epoll_func_rem,
    descriptor_event_generator_epoll_func_on,
    descriptor_event_generator_epoll_func_off,
    descriptor_event_generator_epoll_func_pub,
    descriptor_event_generator_epoll_func_add,
    descriptor_event_generator_epoll_func_del,
    descriptor_event_generator_epoll_func_clear,
    descriptor_event_generator_epoll_func_control
};

extern descriptor_event_generator_epoll_t * descriptor_event_generator_epoll_gen(___notnull event_generator_set_t * set) {
    descriptor_event_generator_epoll_t * generator = (descriptor_event_generator_epoll_t *) calloc(1, sizeof(descriptor_event_generator_epoll_t));

    generator->func = address_of(func);

    generator->set = set;

    generator->value = invalid;
    generator->events = malloc(sizeof(struct epoll_event) * 1024);
    generator->max = 1024;
    generator->timeout = 1;

    return generator;
}

static descriptor_event_generator_epoll_t * descriptor_event_generator_epoll_func_rem(___notnull descriptor_event_generator_epoll_t * generator) {
#ifndef   RELEASE
    snorlaxdbg(generator == nil, false, "critical", "");
#endif // RELEASE

    descriptor_event_generator_epoll_off(generator);
    descriptor_event_generator_epoll_clear(generator);

    generator->sync = sync_rem(generator->sync);

    free(generator);

    return nil;
}

static ___sync int32_t descriptor_event_generator_epoll_func_on(___notnull descriptor_event_generator_epoll_t * generator) {
#ifndef   RELEASE
    snorlaxdbg(generator == nil, false, "critical", "");
#endif // RELEASE

    if(generator->max <= 0) generator->max = 1024;

    if(generator->value <= invalid) {
        generator->value = epoll_create(generator->max);
    }

    if(generator->timeout < 0) generator->timeout = 1;

    generator->events = memory_gen(generator->events, generator->max * sizeof(struct epoll_event));

    generator->status = generator->status | event_generator_state_on;

    return success;
}

static ___sync int32_t descriptor_event_generator_epoll_func_off(___notnull descriptor_event_generator_epoll_t * generator) {
#ifndef   RELEASE
    snorlaxdbg(generator == nil, false, "critical", "");
#endif // RELEASE

    if(generator->value > invalid) {
        if(close(generator->value) == fail) {
#ifndef   RELEASE
            snorlaxdbg(false, true, "warning", "fail to close(...) caused by %d", errno);
#endif // RELEASE
        }
    }

    descriptor_event_subscription_t * subscription = generator->head;
    descriptor_event_subscription_t * next = nil;
    descriptor_t * descriptor = nil;
    while(subscription) {
        object_lock(subscription);
        descriptor = subscription->descriptor;

        event_subscription_event_queue_clear(subscription->queue);

        if(subscription->status) subscription->status = descriptor_event_generator_epoll_subscription_state_none;

#ifndef   RELEASE
        snorlaxdbg(descriptor->status & descriptor_state_exception, false, "critical", "");
#endif // RELEASE

        if(descriptor->status & descriptor_state_open) {
            descriptor->status = descriptor->status | descriptor_state_write;
            descriptor->status = descriptor->status & (~descriptor_state_read);
        } else if(descriptor->status & descriptor_state_close) {
            descriptor->status = descriptor_state_close;
#ifndef   RELEASE
            snorlaxdbg(descriptor->value > invalid, false, "critical", "");
#endif // RELEASE
        } else {
            if(descriptor->value <= invalid) {
#ifndef   RELEASE
                snorlaxdbg(false, true, "check", "");
#endif // RELEASE

                descriptor->status = descriptor_state_close;
            }
        }

        next = subscription->next;
        object_unlock(subscription);
        subscription = next;
    }

    generator->events = memory_rem(generator->events);

    generator->status = generator->status & (~event_generator_state_on);

    return success;
}

static ___sync int32_t descriptor_event_generator_epoll_func_pub(___notnull descriptor_event_generator_epoll_t * generator, event_queue_t * queue) {
#ifndef   RELEASE
    snorlaxdbg(generator == nil, false, "critical", "");
#endif // RELEASE

    struct epoll_event * events = (struct epoll_event *) generator->events;

    event_generator_set_t * set = generator->set;
    event_engine_t * engine = set->engine;

    int32_t nfds = epoll_wait(generator->value, events, generator->max, generator->timeout);
    if(nfds >= 0) {
        descriptor_t * descriptor = nil;
        descriptor_event_subscription_t * subscription = nil;
        uint32_t flags = 0;
        for(int32_t i = 0; i < nfds; i++) {
            subscription = events[i].data.ptr;
            descriptor  = subscription->descriptor;
            flags = events[i].events;

            if(flags & (EPOLLERR | EPOLLPRI | EPOLLHUP | EPOLLRDHUP)) {
#ifndef   RELEASE
                snorlaxdbg(false, true, "descriptor exception", "%d %d %p", descriptor_exception_type_system, flags & (EPOLLERR | EPOLLPRI | EPOLLHUP | EPOLLRDHUP), epoll_wait);
#endif // RELEASE

                descriptor_exception_set(descriptor, descriptor_exception_type_system, flags & (EPOLLERR | EPOLLPRI | EPOLLHUP | EPOLLRDHUP), epoll_wait);

                descriptor_event_generator_epoll_func_dispatch(subscription, descriptor_event_type_exception, queue, engine);
                continue;
            }
            if(flags & EPOLLOUT) {
                descriptor->status = descriptor->status | descriptor_state_write;
                if(flags & EPOLLIN) {
                    descriptor->status = descriptor->status | descriptor_state_read;

                    descriptor_event_generator_epoll_func_dispatch(subscription, descriptor_event_type_read, queue, engine);

                    continue;
                }

                descriptor_event_generator_epoll_func_dispatch(subscription, descriptor_event_type_write, queue, engine);
            }
            if(flags & EPOLLIN) {
                descriptor->status = descriptor->status | descriptor_state_read;

                descriptor_event_generator_epoll_func_dispatch(subscription, descriptor_event_type_read, queue, engine);
            }
        }
    } else {
#ifndef   RELEASE
        snorlaxdbg(false, true, "error", "fail to epoll_wait(...) => %d", errno);
#endif // RELEASE
    }

    return success;
}

/**
 * @todo        디스크립터가 오픈 상태가 아니라면 오픈을 하고 epoll control add 를 수행해야 한다.
 *              그 외에 다양한 에러처리가 필요하다.
 */
static ___sync int32_t descriptor_event_generator_epoll_func_add(___notnull descriptor_event_generator_epoll_t * generator, ___notnull descriptor_event_subscription_t * subscription) {
#ifndef   RELEASE
    snorlaxdbg(generator == nil, false, "critical", "");
    snorlaxdbg(subscription == nil, false, "critical", "");
    snorlaxdbg(subscription->generator, false, "critical", "");
#endif // RELEASE

    if(event_generator_func_add((event_generator_t *) generator, (event_subscription_t *) subscription) == success) {
        // TODO: DESCRIPTOR OPEN => 
        if(descriptor_event_generator_epoll_control(generator, subscription, descriptor_event_generator_epoll_control_type_add) == success) {
            subscription->status = subscription->status | (descriptor_event_generator_epoll_subscription_state_in | descriptor_event_generator_epoll_subscription_state_out);
            descriptor_event_subscription_notify(subscription, descriptor_event_type_subscription, (event_subscription_event_t *) descriptor_event_subscription_type_add);
            return success;
        } else {
            event_generator_func_del((event_generator_t *) generator, (event_subscription_t *) subscription);
        }
    }

    return fail;
}

static ___sync int32_t descriptor_event_generator_epoll_func_del(___notnull descriptor_event_generator_epoll_t * generator, ___notnull descriptor_event_subscription_t * subscription) {
#ifndef   RELEASE
    snorlaxdbg(generator == nil, false, "critical", "");
    snorlaxdbg(subscription == nil, false, "critical", "");
    snorlaxdbg(subscription->generator != (descriptor_event_generator_t *) generator, false, "critical", "");
#endif // RELEASE

    if(subscription->descriptor->value > invalid) {
        descriptor_event_generator_epoll_control(generator, subscription, descriptor_event_generator_epoll_control_type_del);
    }    

    event_generator_func_del((event_generator_t *) generator, (event_subscription_t *) subscription);

    subscription->status = descriptor_event_generator_epoll_subscription_state_none;

    descriptor_event_subscription_notify(subscription, descriptor_event_type_subscription, (event_subscription_event_t *) descriptor_event_subscription_type_del);

    return fail;
}

static ___sync void descriptor_event_generator_epoll_func_clear(___notnull descriptor_event_generator_epoll_t * generator) {
#ifndef   RELEASE
    snorlaxdbg(generator == nil, false, "critical", "");
#endif // RELEASE

    while(generator->head) {
        descriptor_event_generator_epoll_func_del(generator, generator->head);
    }
}

static ___notsync int32_t descriptor_event_generator_epoll_func_control(___notnull descriptor_event_generator_epoll_t * generator, ___notnull descriptor_event_subscription_t * subscription, uint32_t type) {
    if(type == descriptor_event_generator_epoll_control_type_add) {
        return descriptor_event_generator_epoll_func_control_add(generator, subscription);
    } else if(type == descriptor_event_generator_epoll_control_type_mod) {
        return descriptor_event_generator_epoll_func_control_mod(generator, subscription);
    } else if(type == descriptor_event_generator_epoll_control_type_del) {
        return descriptor_event_generator_epoll_func_control_del(generator, subscription);
    } else {
#ifndef   RELEASE
        snorlaxdbg(true, false, "critical", "");
#endif // RELEASE
    }

    return fail;
}

static void descriptor_event_generator_epoll_func_dispatch(descriptor_event_subscription_t * subscription, uint32_t type, event_queue_t * queue, event_engine_t * engine) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
#endif // RELEASE

    if(queue) {
        event_queue_push(queue, event_gen((event_subscription_t *) subscription,
                                            descriptor_event_generator_epoll_subscription_process_get(type),
                                            type,
                                            descriptor_event_subscription_node_gen((event_subscription_t *) subscription)));
    } else {
        if((subscription->descriptor->status & (descriptor_state_open | descriptor_state_close)) == 0 && subscription->descriptor->value > invalid) {
            type = descriptor_event_type_open;
        }

        event_subscription_process_t process = descriptor_event_generator_epoll_subscription_process_get(type);
        
        process((event_subscription_t *) subscription, type, nil);
    }
}

static ___notsync int32_t descriptor_event_generator_epoll_func_control_add(___notnull descriptor_event_generator_epoll_t * generator, ___notnull descriptor_event_subscription_t * subscription) {
#ifndef   RELEASE
    snorlaxdbg(generator == nil, false, "critical", "");
    snorlaxdbg(subscription == nil, false, "critical", "");
#endif // RELEASE

    descriptor_t * descriptor = subscription->descriptor;
    buffer_node_t * out = descriptor->buffer.out ? buffer_front(descriptor->buffer.out) : nil;

    struct epoll_event e;
    e.data.ptr = subscription;

    e.events = 0;

    if((descriptor->status & descriptor_state_read) == 0) {
        if(descriptor->status & descriptor_state_open_in) {
            e.events = e.events | EPOLLIN;
        }
    }

    if(out && buffer_node_length(out) > 0 && (descriptor->status & descriptor_state_write) == 0) {
        if(descriptor->status & descriptor_state_open_out) {
            e.events = e.events | EPOLLOUT;
        }
    }

    if(e.events == 0) {
        if((descriptor->status == descriptor_state_close) == 0) {
            e.events = e.events | (EPOLLIN | EPOLLOUT);
        } else {
#ifndef   RELEASE
            snorlaxdbg(false, true, "warning", "descriptor->value => %d", subscription->descriptor->value);
#endif // RELEASE
            return success;
        }
    }

    e.events = e.events | (EPOLLERR | EPOLLHUP | EPOLLPRI | EPOLLRDHUP | EPOLLONESHOT | EPOLLET);

    if(epoll_ctl(generator->value, EPOLL_CTL_ADD, subscription->descriptor->value, &e) == fail) {
        if(errno == EEXIST) {
            if(epoll_ctl(generator->value, EPOLL_CTL_MOD, subscription->descriptor->value, &e) == fail) {
                if(e.events & EPOLLIN) {
                    subscription->status = subscription->status | descriptor_event_generator_epoll_subscription_state_in;
                } else {
                    subscription->status = subscription->status & (~descriptor_event_generator_epoll_subscription_state_in);
                }

                if(e.events & EPOLLOUT) {
                    subscription->status = subscription->status | descriptor_event_generator_epoll_subscription_state_out;
                } else {
                    subscription->status = subscription->status & (~descriptor_event_generator_epoll_subscription_state_out);
                }
                return success;
            } else {
#ifndef   RELEASE
                    snorlaxdbg(false, true, "warning", "fail to epoll_ctl(...) => %d", errno);
#endif // RELEASE
            }
        } else {
#ifndef   RELEASE
            snorlaxdbg(false, true, "warning", "fail to epoll_ctl(...) => %d", errno);
#endif // RELEASE
        }
        
        subscription->status = descriptor_event_generator_epoll_subscription_state_none;

        return fail;
    }

    if(e.events & EPOLLIN) {
        subscription->status = subscription->status | descriptor_event_generator_epoll_subscription_state_in;
    } else {
        subscription->status = subscription->status & (~descriptor_event_generator_epoll_subscription_state_in);
    }

    if(e.events & EPOLLOUT) {
        subscription->status = subscription->status | descriptor_event_generator_epoll_subscription_state_out;
    } else {
        subscription->status = subscription->status & (~descriptor_event_generator_epoll_subscription_state_out);
    }

    return success;
}

static ___notsync int32_t descriptor_event_generator_epoll_func_control_mod(___notnull descriptor_event_generator_epoll_t * generator, ___notnull descriptor_event_subscription_t * subscription) {
#ifndef   RELEASE
    snorlaxdbg(generator == nil, false, "critical", "");
    snorlaxdbg(subscription == nil, false, "critical", "");
#endif // RELEASE

    descriptor_t * descriptor = subscription->descriptor;
    buffer_node_t * out = descriptor->buffer.out ? buffer_front(descriptor->buffer.out) : nil;

    struct epoll_event e;
    e.data.ptr = subscription;

    e.events = 0;

    if((descriptor->status & descriptor_state_read) == 0) {
        if(descriptor->status & descriptor_state_open_in) {
            e.events = e.events | EPOLLIN;
        }
    }

    if(out && buffer_node_length(out) > 0 && (descriptor->status & descriptor_state_write) == 0) {
        if(descriptor->status & descriptor_state_open_out) {
            e.events = e.events | EPOLLOUT;
        }
    }

    if(e.events == 0) {
        if((descriptor->status == descriptor_state_close) == 0) {
            e.events = e.events | (EPOLLIN | EPOLLOUT);
        } else {
#ifndef   RELEASE
            snorlaxdbg(false, true, "warning", "");
#endif // RELEASE
            return success;
        }
    }

    e.events = e.events | (EPOLLERR | EPOLLHUP | EPOLLPRI | EPOLLRDHUP | EPOLLONESHOT | EPOLLET);

    if(epoll_ctl(generator->value, EPOLL_CTL_MOD, subscription->descriptor->value, &e) == fail) {
        if(errno == ENOENT) {
            if(epoll_ctl(generator->value, EPOLL_CTL_ADD, subscription->descriptor->value, &e) == fail) {
                return success;
            } else {
#ifndef   RELEASE
                snorlaxdbg(false, true, "warning", "fail to epoll_ctl(...) => %d", errno);
#endif // RELEASE
            }
        } else {
#ifndef   RELEASE
            snorlaxdbg(false, true, "warning", "fail to epoll_ctl(...) => %d", errno);
#endif // RELEASE
        }

        subscription->status = descriptor_event_generator_epoll_subscription_state_none;

        return fail;
    }

    return success;
}

static ___notsync int32_t descriptor_event_generator_epoll_func_control_del(___notnull descriptor_event_generator_epoll_t * generator, ___notnull descriptor_event_subscription_t * subscription) {
#ifndef   RELEASE
    snorlaxdbg(generator == nil, false, "critical", "");
    snorlaxdbg(generator->value <= invalid, false, "critical", "");
    snorlaxdbg(subscription == nil, false, "critical", "");
    snorlaxdbg(subscription->descriptor == nil, false, "critical", "");
//    snorlaxdbg(subscription->descriptor->value <= invalid, false, "critical", "");
#endif // RELEASE

    if(subscription->descriptor->value > invalid) {
        struct epoll_event e;
        e.events = (EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP | EPOLLPRI | EPOLLRDHUP);
        if(epoll_ctl(generator->value, EPOLL_CTL_DEL, subscription->descriptor->value, &e) == fail) {
#ifndef   RELEASE
            snorlaxdbg(false, true, "warning", "fail to epoll_ctl(...) => %d", errno);
#endif // RELEASE
        }
    }


    subscription->status = descriptor_event_generator_epoll_subscription_state_none;

    return success;
}
