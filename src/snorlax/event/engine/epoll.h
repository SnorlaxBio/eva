/**
 * @file        snorlax/event/engine/epoll.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 13, 2024
 */

#ifndef   __SNORLAX__EVENT_ENGINE_EPOLL__H__
#define   __SNORLAX__EVENT_ENGINE_EPOLL__H__

#include <snorlax.h>

struct event_engine_epoll;
struct event_engine_epoll_func;

struct event_object;
struct event_queue;

struct event_engine_epoll_descriptor_func;

typedef struct event_engine_epoll event_engine_epoll_t;
typedef struct event_engine_epoll_func event_engine_epoll_func_t;
typedef struct event_object event_object_t;
typedef struct event_queue event_queue_t;

typedef struct event_engine_epoll_descriptor_func event_engine_epoll_descriptor_func_t;

typedef void (*event_object_get_t)(event_object_t *);

struct event_engine_epoll {
    event_engine_epoll_func_t * func;
    uint64_t size;
    event_object_t * head;
    event_object_t * tail;
    event_queue_t * queue;
    event_processor_pool_t * pool;
    void (*cancel)(event_engine_epoll_t *);

    int descriptor;
    void * events;
    int maxevents;
    int timeout;
};

struct event_engine_epoll_descriptor_func {
    int (*dispatch)(event_engine_epoll_t *, descriptor_t *, int, event_param_t *);
};

struct event_engine_epoll_func {
    event_engine_epoll_t * (*rem)(event_engine_epoll_t *);

    event_object_t * (*add)(event_engine_epoll_t *, event_object_t *);
    event_object_t * (*del)(event_engine_epoll_t *, event_object_t *);
    void (*clear)(event_engine_epoll_t *, event_object_get_t);

    int (*run)(event_engine_epoll_t *);

    event_engine_epoll_descriptor_func_t descriptor;
};

extern event_engine_epoll_t * event_engine_epoll_gen(void);

#define event_engine_epoll_rem(engine)      (engine->func->rem(engine))
#define event_engine_epoll_add(engine, o)   (engine->func->add(engine, o))

#endif // __SNORLAX__EVENT_ENGINE_EPOLL__H__
