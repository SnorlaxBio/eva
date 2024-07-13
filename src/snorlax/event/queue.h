/**
 * @file        snorlax/event/queue.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 21, 2024
 * @version     0.0.1
 */

#ifndef   __SNORLAX__EVENT_QUEUE__H__
#define   __SNORLAX__EVENT_QUEUE__H__

#include <snorlax/event.h>

/**
 * @struct          struct event_queue
 * @brief           event queue object.
 * @details         이벤트 큐는 생성된 이벤트를 저장하는 큐이다. 일반적인 큐를 상속하지 않은 것은 불필요한
 *                  메서드들까지 구현하지 않고, 최소한의 필요한 메서드를 구현하여 차후에 적은 라이브리리 사이즈가
 *                  필요할 때, 대응하기 위해서다. 더블 링크드 리스트와 같기 때문에, 특별히 설명할 것은 없다.
 *                  이벤트 엔진에서 사용하는 메서드는 PUSH, POP, DEL, 등과 같은 메서드이며 큐를 생성하는
 *                  `gen(sync: sync): queue` 메서드가 존재한다.
 * 
 * @version         0.0.1
 */
struct event_queue {
    event_queue_func_t * func;
    sync_t * sync;
    uint64_t size;
    event_t * head;
    event_t * tail;
};

struct event_queue_func {
    ___sync event_queue_t * (*rem)(___notnull event_queue_t *);
    ___sync void (*push)(___notnull event_queue_t *, ___notnull event_t *);
    ___notsync event_t * (*pop)(___notnull event_queue_t *);
    ___sync void (*del)(___notnull event_queue_t *, ___notnull event_t *);
};

extern event_queue_t * event_queue_gen(void);

#define event_queue_rem(queue)              ((queue)->func->rem(queue))
#define event_queue_push(queue, event)      ((queue)->func->push(queue, event))
#define event_queue_pop(queue)              ((queue)->func->pop(queue))
#define event_queue_del(queue, event)       ((queue)->func->del(queue, event))
#define event_queue_size(queue)             ((queue)->size)

#endif // __SNORLAX__EVENT_QUEUE__H__
