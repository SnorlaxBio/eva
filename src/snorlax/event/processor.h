/**
 * @file        snorlax/event/processor.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 17, 2024
 */

#ifndef   __SNORLAX__EVENT_PROCESSOR__H__
#define   __SNORLAX__EVENT_PROCESSOR__H__

#include <snorlax/eva.h>

#include <snorlax/thread.h>

struct event_processor {
    event_processor_func_t * func;
    sync_t * sync;

    event_processor_pool_t * pool;
    event_processor_t * prev;
    event_processor_t * next;

    thread_t * thread;
};

struct event_processor_func {
    event_processor_t * (*rem)(event_processor_t *, event_processor_cancel_t);
};

extern event_processor_t * event_processor_gen(event_processor_pool_t * pool);

extern void event_processor_func_cancel(event_processor_t * processor);

#define event_processor_rem(processor, cancel)      (processor ? processor->func->rem(processor, cancel) : nil)
#define event_processor_on(processor)               (processor ? thread_on(processor->thread) : fail)
#define event_processor_off(processor, cancel)      (processor ? thread_off(processor->thread, (thread_cancel_t) cancel) : success)

#endif // __SNORLAX__EVENT_PROCESSOR__H__
