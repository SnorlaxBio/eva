/**
 * @file        snorlax/event/engine/epoll.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 14, 2024
 */

#ifndef   __SNORLAX__EVENT_ENGINE_EPOLL__H__
#define   __SNORLAX__EVENT_ENGINE_EPOLL__H__

#include <snorlax/eva.h>

struct event_engine_epoll;
struct event_engine_epoll_func;

typedef struct event_engine_epoll event_engine_epoll_t;
typedef struct event_engine_epoll_func event_engine_epoll_func_t;

typedef void (*event_engine_epoll_cancel_t)(event_engine_epoll_t *);

struct event_engine_epoll {
    event_engine_epoll_func_t * func;
    uint64_t size;
    event_object_t * head;
    event_object_t * tail;

    event_engine_epoll_cancel_t cancel;

    int descriptor;
    void * events;
    int max;
    int timeout;
};

struct event_engine_epoll_func {
    event_engine_epoll_t * (*rem)(event_engine_epoll_t *);

    event_object_t * (*add)(event_engine_epoll_t *, event_object_t *);
    event_object_t * (*del)(event_engine_epoll_t *, event_object_t *);

    int (*run)(event_engine_epoll_t *);
};

extern event_engine_epoll_t * event_engine_epoll_gen(void);

extern event_engine_epoll_t * event_engine_epoll_func_rem(event_engine_epoll_t * engine);

extern event_object_t * event_engine_epoll_func_add(event_engine_epoll_t * engine, event_object_t * o);
extern event_object_t * event_engine_epoll_func_del(event_engine_epoll_t * engine, event_object_t * o);

#endif // __SNORLAX__EVENT_ENGINE_EPOLL__H__
