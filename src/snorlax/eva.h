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

// 사용자는 EVENT 를 알 필요가 없다.

struct event;
struct event_func;
struct event_queue;
struct event_queue_func;
struct event_engine;
struct event_engine_func;

struct event_param;

struct event_processor;
struct event_processor_func;
struct event_processor_pool;
struct event_processor_pool_func;

struct event_subscription;

typedef struct event event_t;
typedef struct event_func event_func_t;
typedef struct event_param event_param_t;
typedef struct event_queue event_queue_t;
typedef struct event_queue_func event_queue_func_t;
typedef struct event_subscription event_subscription_t;

typedef struct event_processor event_processor_t;
typedef struct event_processor_func event_processor_func_t;

typedef struct event_processor_pool event_processor_pool_t;
typedef struct event_processor_pool_func event_processor_pool_func_t;

typedef struct event_engine event_engine_t;
typedef struct event_engine_func event_engine_func_t;

typedef int32_t (*event_handler_t)(event_t *);

typedef void (*event_callback_t)(event_t *);

typedef void (*event_engine_cancel_t)(event_engine_t *);

typedef void (*event_processor_cancel_t)(event_processor_t *);
typedef int32_t (*event_processor_routine_t)(event_processor_t *);


extern int32_t event_engine_on(void);
extern int32_t event_engine_off(event_engine_cancel_t cancel);
extern int32_t event_engine_run(void);

#endif // __SNORLAX__EVA__H__
