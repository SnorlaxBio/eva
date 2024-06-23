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

/**
 * @struct          struct event_processor_pool
 * @brief           event processor pool object
 * @details         이벤트 프로세서 풀은 같은 이벤트 큐를 바라보면서 큐에서 아이템을 가지고 와서 이벤트를
 *                  처리하는 간단한 프로세스들을 관리하는 객체다. 이것 역시 더블 링크드 리스트로 풀의
 *                  삭제와 삽입이 O(1) 이 되어지도록 구현하였다.
 * 
 * @version         0.0.1
 */
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
