/**
 * @file        snorlax/descriptor.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 13, 2024
 */

#ifndef   __SNORLAX__DESCRIPTOR__H__
#define   __SNORLAX__DESCRIPTOR__H__

#include <snorlax.h>

#include <snorlax/event/object/queue.h>
#include <snorlax/descriptor/buffer.h>
#include <snorlax/event/handler/array.h>

struct descriptor;
struct descriptor_func;

typedef struct descriptor descriptor_t;
typedef struct descriptor_func descriptor_func_t;

struct event_engine;
struct event_object;
struct event_queue;

typedef struct event_engine event_engine_t;
typedef struct event_object event_object_t;
typedef struct event_queue event_queue_t;

struct descriptor {
    descriptor_func_t * func;
    event_engine_t * engine;
    event_object_t * prev;
    event_object_t * next;

    event_object_queue_t queue;

    uint32_t status;
    event_handler_array_t * handler;

    int value;
    descriptor_buffer_t buffer;
};

struct descriptor_func {
    descriptor_t * (*rem)(descriptor_t *);

    int (*open)(descriptor_t *);
    int (*close)(descriptor_t *);
    int64_t (*read)(descriptor_t *);
    int64_t (*write)(descriptor_t *);
};

extern descriptor_t * descriptor_gen(int value);

extern descriptor_t * descriptor_func_rem(descriptor_t * o);
extern int descriptor_func_open(descriptor_t * o);
extern int descriptor_func_close(descriptor_t * o);
extern int64_t descriptor_func_read(descriptor_t * o);
extern int64_t descriptor_func_write(descriptor_t * o);

#endif // __SNORLAX__DESCRIPTOR__H__