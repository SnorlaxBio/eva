/**
 * @file        snorlax/event/subscription/event.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 21, 2024
 * @version     0.0.1
 */

#ifndef   __SNORLAX__EVENT_SUBSCRIPTION_EVENT__H__
#define   __SNORLAX__EVENT_SUBSCRIPTION_EVENT__H__

#include <snorlax/event.h>

/**
 * @struct          struct event_subscription_event
 * @brief           event subscription event object
 * @details         이벤트 서브스크립션 이벤트(이벤트 서브스크립션과 연관된 이벤트 큐의 아이템)는 이벤트와 관련한 파라미터를
 *                  저장할 수도 있다. 아래 도식화된 것에는 이벤트와 관련한 파라미터를 저장하는 멤버 변수는 없다. 필요에
 *                  따라서 상속하여 파라미터를 관리할 수 있도록 한다. 즉, 각 이벤트 서브스크립션에서 관리하는 것이다.
 *                  오리지널 이벤트를 멤버 변수로 참조하는데, 이 이벤트는 엔진의 종료와 기타 사용자에 의한 서브스크립션 취소가
 *                  일어날 경우 서브스크립션에 등록된 이벤트를 비워야 하는 경우에 처리되지 않은 이벤트의 O(1) 삭제를 위해서
 *                  상호 참조하도록 되어 있다. 삭제 시에 유의하여 제거하도록 구현되어져 있다.
 */
struct event_subscription_event {
    event_subscription_event_func_t * func;
    sync_t * sync;
    event_subscription_event_queue_t * queue;
    event_subscription_event_t * prev;
    event_subscription_event_t * next;

    event_t * origin;

    int64_t parameter;
};

struct event_subscription_event_func {
    event_subscription_event_t * (*rem)(___notnull event_subscription_event_t *);
    event_subscription_event_t * (*parameter_set)(___notnull event_subscription_event_t *, int64_t);
    int64_t (*parameter_get)(___notnull event_subscription_event_t *);
};

extern event_subscription_event_t * event_subscription_event_gen(event_subscription_t * subscription);
extern event_subscription_event_t * event_subscription_event_func_parameter_set(___notnull event_subscription_event_t * event, int64_t parameter);
extern int64_t event_subscription_event_func_parameter_get(___notnull event_subscription_event_t * event);
extern event_subscription_event_t * event_subscription_event_func_rem(___notnull event_subscription_event_t * event);

#define event_subscription_event_rem(event)                             ((event)->func->rem(event))
#define event_subscription_event_parameter_set(event, parameter)        ((event) ? (event)->func->parameter_set(event, ((int64_t)(parameter))) : (event_subscription_event_t *)((uint64_t)(parameter)))
#define event_subscription_event_parameter_get(event)                   ((event)->func->parameter_get(event))

#endif // __SNORLAX__EVENT_SUBSCRIPTION_EVENT__H__
