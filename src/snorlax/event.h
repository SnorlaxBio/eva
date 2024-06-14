/**
 * @file        snorlax/event.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 12, 2024
 */

#ifndef   __SNORLAX__EVENT__H__
#define   __SNORLAX__EVENT__H__

struct event;
struct event_func;

struct event_queue;
struct event_param;
struct event_object;
struct event_engine;

typedef struct event event_t;
typedef struct event_func event_func_t;

typedef struct event_queue event_queue_t;
typedef struct event_param event_param_t;
typedef struct event_object event_object_t;
typedef struct event_engine event_engine_t;

typedef int (*event_handler_t)(event_object_t *, event_param_t *, event_engine_t *);
typedef void (event_get_t)(event_t *);

struct event {
    event_func_t * func;
    event_queue_t * queue;
    event_t * prev;
    event_t * next;

    struct {
        event_object_t * o;
        event_t * prev;
        event_t * next;
    } node;

    event_param_t * param;
    event_handler_t on;
};

struct event_func {
    event_t * (*rem)(event_t *, event_get_t);
};

extern event_t * event_gen(event_object_t * o, event_param_t * param, event_handler_t on);

#define event_rem(e, get)       (e->func->rem(e, get))

#define event_on(e, engine)     (e->on(e->o, e->param, engine))

#endif // __SNORLAX__EVENT__H__
