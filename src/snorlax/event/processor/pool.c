/**
 * @file        snorlax/event/processor/pool.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 21, 2024
 */

#include <stdlib.h>

#include "pool.h"

#include "pool/state.h"

#include "../engine.h"
#include "../processor.h"

static ___sync event_processor_pool_t * event_processor_pool_func_rem(___notnull event_processor_pool_t * pool);
static ___sync int32_t event_processor_pool_func_on(___notnull event_processor_pool_t * pool);
static ___sync int32_t event_processor_pool_func_off(___notnull event_processor_pool_t * pool);
static ___notsync int32_t event_processor_pool_func_add(___notnull event_processor_pool_t * pool, ___notnull event_processor_t * processor);
static ___notsync int32_t event_processor_pool_func_del(___notnull event_processor_pool_t * pool, ___notnull event_processor_t * processor);

static event_processor_pool_func_t func = {
    event_processor_pool_func_rem,
    event_processor_pool_func_on,
    event_processor_pool_func_off,
    event_processor_pool_func_add,
    event_processor_pool_func_del
};

extern ___sync event_processor_pool_t * event_processor_pool_gen(uint64_t n, ___notnull event_engine_t * engine) {
#ifndef   RELEASE
    snorlaxdbg(engine == nil, "critical", "");
#endif // RELEASE

    event_processor_pool_t * pool = (event_processor_pool_t *) calloc(1, sizeof(event_processor_pool_t));

    pool->func = address_of(func);
    pool->engine = engine;

    if(engine->status & event_engine_state_on) pool->status = event_processor_pool_state_on;

    if(n > 0) {
        pool->sync = sync_gen();

        object_lock(pool);
        for(uint64_t i = 0; i < n; i++) {
            event_processor_pool_add(pool, event_processor_gen());
        }
        object_unlock(pool);
    }


    return pool;
}

static ___sync event_processor_pool_t * event_processor_pool_func_rem(___notnull event_processor_pool_t * pool) {
#ifndef   RELEASE
    snorlaxdbg(pool == nil, "critical", "");
#endif // RELEASE
    object_lock(pool);
    while(pool->head) event_processor_rem(pool->head);
    object_unlock(pool);

    pool->sync = sync_rem(pool->sync);

    free(pool);

    return nil;
}

static ___sync int32_t event_processor_pool_func_on(___notnull event_processor_pool_t * pool) {
#ifndef   RELEASE
    snorlaxdbg(pool == nil, "critical", "");
#endif // RELEASE

    object_lock(pool);
    if((pool->status & event_processor_pool_state_on) == 0U) {
        pool->status = pool->status | event_processor_pool_state_on;

        event_processor_t * processor = pool->head;
        while(processor) {
            event_processor_on(processor);
            processor = processor->next;
        }

    } else {
#ifndef   RELEASE
        snorlaxdbg(false, "notice", "event processor pool is already on");
#endif // RELEASE
    }
    object_unlock(pool);

    return success;
}

static ___sync int32_t event_processor_pool_func_off(___notnull event_processor_pool_t * pool) {
#ifndef   RELEASE
    snorlaxdbg(pool == nil, "critical", "");
#endif // RELEASE

    object_lock(pool);
    if(pool->status & event_processor_pool_state_on) {
        event_processor_t * processor = pool->head;
        while(processor) {
            event_processor_off(processor, nil);
            processor = processor->next;
        }
        pool->status = pool->status & (~event_processor_pool_state_on);
    } else {
#ifndef   RELEASE
        snorlaxdbg(false, "notice", "event processor pool is already off");
#endif // RELEASE
    }
    object_unlock(pool);

    return success;
}

static ___notsync int32_t event_processor_pool_func_add(___notnull event_processor_pool_t * pool, ___notnull event_processor_t * processor) {
#ifndef   RELEASE
    snorlaxdbg(pool == nil, "critical", "");
    snorlaxdbg(processor == nil, "critical", "");
#endif // RELEASE

    if(pool->tail) {
        pool->tail->next = processor;
        processor->prev = pool->tail;
    } else {
        pool->head = processor;
    }

    pool->tail = processor;
    pool->size = pool->size;

    processor->pool = pool;

    if(pool->status & event_processor_pool_state_on) {
        event_processor_on(processor);
    }

    return success;
}

static ___notsync int32_t event_processor_pool_func_del(___notnull event_processor_pool_t * pool, ___notnull event_processor_t * processor) {
#ifndef   RELEASE
    snorlaxdbg(pool == nil, "critical", "");
    snorlaxdbg(processor == nil, "critical", "");
#endif // RELEASE

    event_processor_off(processor, nil);

    event_processor_t * prev = processor->prev;
    event_processor_t * next = processor->next;

    if(prev) {
        prev->next = next;
        processor->next = nil;
    } else {
        pool->head = next;
    }

    if(next) {
        next->prev = prev;
        processor->prev = nil;
    } else {
        pool->tail = prev;
    }

    pool->size = pool->size - 1;
    processor->pool = pool;

    return success;
}
