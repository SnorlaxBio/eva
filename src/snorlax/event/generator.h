/**
 * @file        snorlax/event/generator.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 21, 2024
 */

#ifndef   __SNORLAX__EVENT_GENERATOR__H__
#define   __SNORLAX__EVENT_GENERATOR__H__

#include <snorlax/event.h>

/**
 * @struct          struct event_generator
 * @details         | abstract | event generator object.
 * @details         이벤트를 생성하는 제네레이터 객체다. 직접적으로 이벤트 엔진과 커뮤니케이션 하지 않고 제네레이터들을
 *                  관리하는 제네레이터 셋을 통해서 관리되어 진다. 이벤트를 생성하고 서브스크립션을 관리하는 객체로
 *                  서브스크립션을 리스트의 아이템 형태로 관리하기 때문에, O(1) 로 삽입과 삭제가 가능하다. 
 * 
 * @version         0.0.1
 */
struct event_generator {
    event_generator_func_t * func;
    sync_t * sync;
    uint64_t size;
    event_subscription_t * head;
    event_subscription_t * tail;
    event_generator_set_t * set;
    uint32_t status;
};

struct event_generator_func {
    event_generator_t * (*rem)(___notnull event_generator_t *);
    int32_t (*on)(___notnull event_generator_t *);
    int32_t (*off)(___notnull event_generator_t *);
    int32_t (*pub)(___notnull event_generator_t *, event_queue_t *);
    int32_t (*control)(___notnull event_generator_t *, ___notnull event_subscription_t *, uint32_t, event_subscription_event_t *);
    int32_t (*add)(___notnull event_generator_t *, ___notnull event_subscription_t *);
    int32_t (*del)(___notnull event_generator_t *, ___notnull event_subscription_t *);
    ___sync void (*clear)(___notnull event_generator_t *);
};

extern int32_t event_generator_func_add(___notnull event_generator_t * generator, ___notnull event_subscription_t * subscription);
extern int32_t event_generator_func_del(___notnull event_generator_t * generator, ___notnull event_subscription_t * subscription);
extern ___sync void event_generator_func_clear(___notnull event_generator_t * generator);

#define event_generator_rem(generator)                                  ((generator)->func->rem(generator))
#define event_generator_on(generator)                                   ((generator)->func->on(generator))
#define event_generator_off(generator)                                  ((generator)->func->off(generator))
#define event_generator_pub(generator, queue)                           ((generator)->func->pub(generator, queue))
#define event_generator_control(generator, subscription, type, node)    ((generator)->func->control(generator, subscription, type, node))
#define event_generator_add(generator, subscription)                    ((generator)->func->add(generator, subscription))
#define event_generator_del(generator, subscription)                    ((generator)->func->del(generator, subscription))
#define event_generator_clear(generator)                                ((generator)->func->clear(generator))

#endif // __SNORLAX__EVENT_GENERATOR__H__
