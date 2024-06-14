/**
 * @file        snorlax/event/engine/epoll.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 13, 2024
 */

#include <sys/epoll.h>

#include "epoll.h"

#include "../../descriptor.h"
#include "../../descriptor/state.h"
#include "../../descriptor/event/type.h"

#include "../queue.h"
#include "../engine.h"
#include "../processor/pool.h"

typedef event_object_t * (*event_engine_epoll_add_t)(event_engine_epoll_t *, event_object_t *);
typedef event_object_t * (*event_engine_epoll_del_t)(event_engine_epoll_t *, event_object_t *);
typedef void (*event_engine_epoll_clear_t)(event_engine_t *, event_object_get_t);

static event_engine_epoll_t * event_engine_epoll_func_rem(event_engine_epoll_t * engine);

static int event_engine_epoll_func_run(event_engine_epoll_t * engine);

static int event_engine_epoll_descriptor_func_dispatch(event_engine_epoll_t * engine, descriptor_t * descriptor, int type, event_param_t * param);

static event_engine_epoll_t func = {
    event_engine_epoll_func_rem,

    (event_engine_epoll_add_t) event_engine_func_add,
    (event_engine_epoll_del_t) event_engine_func_del,
    (event_engine_epoll_clear_t) event_engine_func_clear,

    event_engine_epoll_func_run,
    {
        event_engine_epoll_descriptor_func_dispatch
    }
};

extern event_engine_epoll_t * event_engine_epoll_gen(void) {
    event_engine_epoll_t * epoll = (event_engine_epoll_t *) calloc(1, sizeof(event_engine_epoll_t));

    epoll->func = &func;
    epoll->queue = event_queue_gen();
    epoll->descriptor = invalid;
    epoll->events = malloc(sizeof(struct epoll_event) * 1024);
    epoll->maxevents = 1024;
    epoll->timeout = 1;

    return epoll;
}

static event_engine_epoll_t * event_engine_epoll_func_rem(event_engine_epoll_t * engine) {
    engine->func->clear(engine, nil);
    engine->queue = event_queue_rem(engine->queue, nil);

    close(engine->descriptor);
    free(engine->events);

    free(engine);

    return nil;
}

static int event_engine_epoll_func_run(event_engine_epoll_t * engine) {
    while(engine->cancel == nil) {
        struct epoll_event * events = (struct epoll_event *) engine->events;
        int nfds = epoll_wait(engine->descriptor, events, engine->maxevents, engine->timeout);
        if(nfds >= 0) {
            for(int i = 0; i < nfds; i++) {
                descriptor_t * descriptor = (descriptor_t *) events[i].data.ptr;
                if(events[i].events & (EPOLLPRI | EPOLLERR | EPOLLRDHUP | EPOLLHUP)) {

                    event_engine_descriptor_dispatch(engine, descriptor, descriptor_event_type_exception, (event_param_t *) events[i].events);
                    continue;
                }
                if(events[i].events & EPOLLOUT) {
                    descriptor->status = descriptor->status | descriptor_state_writable;
                    if(event_engine_descriptor_dispatch(engine, descriptor, descriptor_event_type_out, nil) == fail) {
                        event_engine_descriptor_dispatch(engine, descriptor, descriptor_event_type_exception, nil);
                        continue;
                    }
                }
                if(events[i].events & EPOLLIN) {
                    descriptor->status = descriptor->status | descriptor_state_readable;
                    if(event_engine_descriptor_dispatch(engine, descriptor, descriptor_event_type_in, nil) == fail) {
                        event_engine_descriptor_dispatch(engine, descriptor, descriptor_event_type_exception, nil);
                        continue;
                    }
                }
            }
        } else {
            // ERROR HANDLING
        }
    }
    if(engine->cancel) {
        engine->cancel(engine);
        engine->cancel = nil;
    }
}

static int event_engine_epoll_descriptor_func_dispatch(event_engine_epoll_t * engine, descriptor_t * descriptor, int type, event_param_t * param) {
    // type 에 따라서, 미리 해야할 일을 하자.
    // EVENT PROCESSOR 의 상태에 따라 달리 I/O 처리를 해야 한다.
    if(engine->pool && engine->pool->size > 0) {

    } else {
        if(type == descriptor_event_type_in) {
            
        } else if(type == descriptor_event_type_out) {

        } else if(type == descriptor_event_type_exception) {

        } else if(type == descriptor_event_type_close) {

        } else {

        }
    }

    return fail;
}