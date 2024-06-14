/**
 * @file        snorlax/event/processor.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 13, 2024
 */

#ifndef   __SNORLAX__EVENT_PROCESSOR__H__
#define   __SNORLAX__EVENT_PROCESSOR__H__

struct event_processor;
struct event_processor_func;
struct event_processor_pool;

typedef struct event_processor event_processor_t;
typedef struct event_processor_func event_processor_func_t;
typedef struct event_processor_pool event_processor_pool_t;

struct event_processor {
    event_processor_func_t * func;
    event_processor_pool_t * pool;
    event_processor_t * prev;
    event_processor_t * next;
};

#endif // __SNORLAX__EVENT_PROCESSOR__H__
