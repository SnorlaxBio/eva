/**
 * @file        snorlax/event/subscription.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 21, 2024
 */

#ifndef   __SNORLAX__EVENT_SUBSCRIPTION__H__
#define   __SNORLAX__EVENT_SUBSCRIPTION__H__

#include <snorlax/event.h>

/**
 * @brief           struct event_subscription
 * @brief           | interface | event subscription object.
 * @details         이벤트 서브스크립션 객체는 일반적인 명령을 수행하는 커맨드, 디스크림터 I/O를 처리하는 디스크립터,
 *                  타임 이벤트를 처리하는 타임, 시그널을 처리하는 시그널 이벤트를 등록하면 그 등록을 관리할 수 있는
 *                  이 서브스크립션 객체를 반환해 준다. 사용자 프로그램 상에서 사용자는 이 객체를 통해서 서브스크립션과
 *                  관련한 정보를 갱신하거나 해제할 수 있다. 서브스크립션의 등록과 해제 정보 업데이트는 이벤트 엔진을
 *                  통해서 이루어진다. 서브스크립션의 해제를 위해서 리스트 노드와 유사하게 GENERATOR, PREV, NEXT 멤버
 *                  변수를 가진다. 리스트의 삭제는 O(1) 로 이루어지기 때문에, 사용자가 등록을 해지할 때, 엔진에 무리를
 *                  주지 않고 삭제할 수 있게 된다. 사용자가 흥미를 가진 이벤트를 handler 배열에 등록되게 된다. 이를
 *                  통해서 엔진은 흥미로운 이벤트를 내부적으로 등록하고 처리하며 처리 후의 결과를 사용자가 등록한
 *                  핸들러를 통해서 통지하게 된다.
 * 
 * @version         0.0.1
 */
struct event_subscription {
    event_subscription_func_t * func;
    sync_t * sync;
    event_generator_t * generator;
    event_subscription_t * prev;
    event_subscription_t * next;
    event_subscription_event_queue_t * queue;
    event_subscription_handler_t * handler;
    uint32_t type;
    event_subscription_meta_t * meta;
};

struct event_subscription_func {
    event_subscription_t * (*rem)(___notnull event_subscription_t *);
    void (*on)(___notnull event_subscription_t *, event_subscription_process_t, uint32_t, event_subscription_event_t *);
    void (*notify)(___notnull event_subscription_t *, uint32_t, event_subscription_event_t *);
};

#define event_subscription_rem(subscription)                        ((subscription)->func->rem(subscription))
#define event_subscription_on(subscription, process, type, node)    ((subscription)->func->on(subscription, process, type, node))
#define event_subscription_notify(subscription, type, node)         ((subscription)->func->notify(subscription, type, node))

#endif // __SNORLAX__EVENT_SUBSCRIPTION__H__
