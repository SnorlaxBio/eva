/**
 * @file        snorlax/event/processor.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 18, 2024
 */

#ifndef   __SNORLAX__EVENT_PROCESSOR__H__
#define   __SNORLAX__EVENT_PROCESSOR__H__

#include <snorlax/eva.h>

struct event_processor {
    event_processor_func_t * func;
    sync_t * sync;

    event_processor_pool_t * pool;
    event_processor_t * prev;
    event_processor_t * next;
};

struct event_processor_func {
    event_processor_t * (*rem)(event_processor_t *);
};

#define event_processor_rem(processor)      (processor->func->rem(processor))

#endif // __SNORLAX__EVENT_PROCESSOR__H__
