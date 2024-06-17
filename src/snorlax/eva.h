/**
 * @file        snorlax/eva.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 17, 2024
 */

#ifndef   __SNORLAX__EVA__H__
#define   __SNORLAX__EVA__H__

#include <snorlax.h>

struct event;
struct event_func;
struct event_queue;
struct event_queue_func;
struct event_queue_node;
struct event_queue_node_func;
struct event_subscription;
struct event_subscription_func;
struct event_engine;
struct event_engine_func;
struct event_engine_conf;

struct event_subscription_event;
struct event_subscription_event_func;
struct event_subscription_event_queue;
struct event_subscription_event_queue_func;

struct event_processor;
struct event_processor_func;
struct event_processor_pool;
struct event_processor_pool_func;

struct event_generator;
struct event_generator_func;
struct event_generator_set;
struct event_generator_set_func;

struct command;
struct command_event_subscription;

typedef struct event event_t;
typedef struct event_func event_func_t;
typedef struct event_queue event_queue_t;
typedef struct event_queue_func event_queue_func_t;
typedef struct event_queue_node event_queue_node_t;
typedef struct event_queue_node_func event_queue_node_func_t;
typedef struct event_subscription event_subscription_t;
typedef struct event_subscription_func event_subscription_func_t;

typedef struct event_subscription_event event_subscription_event_t;
typedef struct event_subscription_event_func event_subscription_event_func_t;
typedef struct event_subscription_event_queue event_subscription_event_queue_t;
typedef struct event_subscription_event_queue_func event_subscription_event_queue_func_t;

typedef struct event_engine event_engine_t;
typedef struct event_engine_func event_engine_func_t;

typedef struct event_engine_conf event_engine_conf_t;

typedef struct event_processor event_processor_t;
typedef struct event_processor_func event_processor_func_t;
typedef struct event_processor_pool event_processor_pool_t;
typedef struct event_processor_pool_func event_processor_pool_func_t;

typedef struct event_generator event_generator_t;
typedef struct event_generator_func event_generator_func_t;
typedef struct event_generator_set event_generator_set_t;
typedef struct event_generator_set_func event_generator_set_func_t;

typedef struct command command_t;
typedef struct command_event_subscription command_event_subscription_t;

typedef void (*event_engine_cancel_t)(event_engine_t *);

extern int32_t event_engine_on(const event_engine_conf_t * conf);

extern command_event_subscription_t * event_engine_command_add(command_t * command, uint32_t status);

extern int32_t event_engine_run(void);

#endif // __SNORLAX__EVA__H__
