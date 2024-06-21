/**
 * @file        snorlax/event/processor.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 21, 2024
 */

#ifndef   __SNORLAX__EVENT_PROCESSOR__H__
#define   __SNORLAX__EVENT_PROCESSOR__H__

#include <snorlax/thread.h>
#include <snorlax/event.h>

struct event_processor {
    event_processor_func_t * func;
    sync_t * sync;
    event_processor_pool_t * pool;
    event_processor_t * prev;
    event_processor_t * next;
    thread_t * thread;
};

struct event_processor_func {
    event_processor_t * (*rem)(___notnull event_processor_t *);
    int32_t (*on)(___notnull event_processor_t *);
    int32_t (*off)(___notnull event_processor_t *, event_processor_cancel_t);
};

extern event_processor_t * event_processor_gen(void);

#define event_processor_rem(processor)          ((processor)->func->rem(processor))
#define event_processor_on(processor)           ((processor)->func->on(processor))
#define event_processor_off(processor, cancel)  ((processor)->func->off(processor, cancel))

#endif // __SNORLAX__EVENT_PROCESSOR__H__
