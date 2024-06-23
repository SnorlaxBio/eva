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

/**
 * @struct          struct event_processor
 * @brief           event processor object
 * @details         이벤트 프로세서는 이벤트 큐를 처리하는 스레드이다. 풀을 통해서 생성, 삭제, 시작과 종료를 수행한다.
 *                  즉, 엔진은 직접 프로세스와 관련한 작업을 할 수 없다. 스레드를 HAS-A 관계로 가지고 있으며,
 *                  프로세스 CANCEL 과 관계된 변수는 THREAD 에서 가지고 있다. PROCESS CANCEL 관련한 멤버에 접근하려면
 *                  형변환을 통해서 이루어진다.
 * 
 * @version         0.0.1
 */
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
