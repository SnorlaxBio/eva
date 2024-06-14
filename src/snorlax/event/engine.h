/**
 * @file        snorlax/event/engine.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 13, 2024
 */

#ifndef   __SNORLAX__EVENT_ENGINE__H__
#define   __SNORLAX__EVENT_ENGINE__H__

#include <snorlax.h>

struct descriptor;
struct event_object;
struct event_queue;
struct event_engine;
struct event_param;
struct event_engine_func;
struct event_processor_pool;
struct event_engine_descriptor_func;

typedef struct event_param event_param_t;
typedef struct descriptor descriptor_t;
typedef struct event_queue event_queue_t;
typedef struct event_object event_object_t;
typedef struct event_engine event_engine_t;
typedef struct event_engine_func event_engine_func_t;
typedef struct event_engine_descriptor_func event_engine_descriptor_func_t;

typedef struct event_processor_pool event_processor_pool_t;

typedef void (*event_object_get_t)(event_object_t *);

struct event_engine {
    event_engine_func_t * func;
    uint64_t size;
    event_object_t * head;
    event_object_t * tail;
    event_queue_t * queue;
    event_processor_pool_t * pool;
    void (*cancel)(event_engine_t *);
};

struct event_engine_descriptor_func {
    int (*dispatch)(event_engine_t *, descriptor_t *, int, event_param_t *);
};

struct event_engine_func {
    event_engine_t * (*rem)(event_engine_t *);

    event_object_t * (*add)(event_engine_t *, event_object_t *);
    event_object_t * (*del)(event_engine_t *, event_object_t *);
    void (*clear)(event_engine_t *, event_object_get_t);

    int (*run)(event_engine_t *);

    event_engine_descriptor_func_t descriptor;


};

#define event_engine_rem(engine)        (engine->func->rem(engine))

#define event_engine_add(engine, o)     (engine->func->add(engine, o))

#define event_engine_run(engine)        (engine->func->run(engine))

extern event_object_t * event_engine_func_add(event_engine_t * engine, event_object_t * o);
extern event_object_t * event_engine_func_del(event_engine_t * engine, event_object_t * o);
extern void event_engine_func_clear(event_engine_t * engine, event_object_get_t get);

#define event_engine_descriptor_dispatch(engine, descriptor, type, param)       (engine->func->descriptor.dispatch(engine, descriptor, type, param))

#endif // __SNORLAX__EVENT_ENGINE__H__
