/**
 * @file        snorlax/event/processor/pool.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 21, 2024
 */

#ifndef   __SNORLAX__EVENT_PROCESSOR_POOL__H__
#define   __SNORLAX__EVENT_PROCESSOR_POOL__H__

#include <snorlax/event.h>

#include <snorlax/event/processor/pool/state.h>

struct event_processor_pool {
    event_processor_pool_func_t * func;
    sync_t * sync;
    uint64_t size;
    event_processor_t * head;
    event_processor_t * tail;
    uint32_t status;
    event_engine_t * engine;
};

struct event_processor_pool_func {
    ___sync event_processor_pool_t * (*rem)(___notnull event_processor_pool_t *);
    ___sync int32_t (*on)(___notnull event_processor_pool_t *);
    ___sync int32_t (*off)(___notnull event_processor_pool_t *);
    ___notsync int32_t (*add)(___notnull event_processor_pool_t *, ___notnull event_processor_t *);
    ___notsync int32_t (*del)(___notnull event_processor_pool_t *, ___notnull event_processor_t *);
};

___sync extern event_processor_pool_t * event_processor_pool_gen(uint64_t n, ___notnull event_engine_t * engine);

#define event_processor_pool_rem(pool)                  ((pool)->func->rem(pool))
#define event_processor_pool_on(pool)                   ((pool)->func->on(pool))
#define event_processor_pool_off(pool)                  ((pool)->func->off(pool))
#define event_processor_pool_add(pool, processor)       ((pool)->func->add(pool, processor))
#define event_processor_pool_del(pool, processor)       ((pool)->func->del(pool, processor))

#endif // __SNORLAX__EVENT_PROCESSOR_POOL__H__
