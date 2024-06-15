/**
 * @file        snorlax/eva.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 14, 2024
 */

#ifndef   __SNORLAX__EVA__H__
#define   __SNORLAX__EVA__H__

#include <snorlax.h>

struct event;

struct event_object;
struct event_engine;
struct event_param;

struct event_handler_array;

struct event_processor;
struct event_generator;
struct event_processor_pool;
struct event_generator_set;
struct event_queue;
struct event_subscription;

struct descriptor_event_subscription;

typedef struct event event_t;
typedef struct event_object event_object_t;
typedef struct event_engine event_engine_t;
typedef struct event_param event_param_t;
typedef struct event_queue event_queue_t;
typedef struct event_handler_array event_handler_array_t;

typedef struct event_processor event_processor_t;
typedef struct event_generator event_generator_t;

typedef struct event_subscription event_subscription_t;

typedef struct descriptor_event_subscription descriptor_event_subscription_t;

typedef struct event_processor_pool event_processor_pool_t;
typedef struct event_generator_set event_generator_set_t;

typedef void (*event_engine_cancel_t)(event_engine_t *);

typedef int (*event_handler_t)(event_object_t *, event_param_t *);

extern int event_engine_run(void);
extern void event_engine_cancel(event_engine_cancel_t cancel);

// extern event_subscription_t * 

extern descriptor_event_subscription_t * descriptor_event_sub(descriptor_event_subscription_t * subscription, int descriptor, int types, ...);

// vs

extern void event_engine_descriptor_sub(...);

extern event_subscription_t * event_engine_descriptor_sub(int descriptor, int type, ...);
extern event_subscription_t * event_engine_signal_sub(event_object_t * o);
extern event_subscription_t * event_engine_time_sub(event_object_t * o);
extern event_subscription_t * event_engine_command_sub(event_object_t * o);

#endif // __SNORLAX__EVA__H__
