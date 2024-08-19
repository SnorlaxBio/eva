/**
 * @file        snorlax/event/engine.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 21, 2024
 */

#ifndef   __SNORLAX__EVENT_ENGINE__H__
#define   __SNORLAX__EVENT_ENGINE__H__

#include <snorlax/event.h>

#include <snorlax/event/engine/state.h>

/**
 * @struct          struct event_engine
 * @brief           event engine object
 * @details         이벤트 엔진 객체는 이벤트 처리 및 관리를 위한 멤버 변수들을 가진다. 이벤트 생성기들을 관리하는
 *                  이벤트 제네레이터 셋과 멀티 스레드 환경에서 여러 스레드에서 등록된 이벤트를 처리할 수 있는 프로세서들을
 *                  관리하는 프로세스 풀, 그리고 이벤트 큐, 기타 상태를 관리하는 상태 변수와 이벤트 엔진 수행 상태를 관리하며
 *                  종료 후에 콜백으로 사용되어지는 CANCEL 등이 정의되어 있다.
 * @version         0.0.1
 * 
 * @todo            refactor function.
 */
struct event_engine {
    event_engine_func_t * func;
    sync_t * sync;
    uint32_t status;
    event_queue_t * queue;
    event_processor_pool_t * pool;
    event_generator_set_t * set;
    event_engine_cancel_t cancel;
};

struct event_engine_func {
    event_engine_t * (*rem)(___notnull event_engine_t *);
    int32_t (*on)(___notnull event_engine_t *);
    int32_t (*off)(___notnull event_engine_t *, event_engine_cancel_t);
    int32_t (*run)(___notnull event_engine_t *);
};

extern event_engine_t * event_engine_gen(void);

#define event_engine_rem(engine)            ((engine)->func->rem(engine))
#define event_engine_on(engine)             ((engine)->func->on(engine))
#define event_engine_off(engine, cancel)    ((engine)->func->off(engine, cancel))
#define event_engine_run(engine)            ((engine)->func->run(engine))

#include <snorlax/descriptor.h>
#include <snorlax/socket.h>
#include <snorlax/socket/server.h>
#include <snorlax/socket/session.h>
#include <snorlax/socket/client.h>

extern descriptor_event_subscription_t * event_engine_descriptor_sub(event_engine_t * engine, descriptor_t * descriptor, descriptor_event_subscription_handler_t * handler, event_subscription_meta_t * meta);
extern socket_event_subscription_t * event_engine_socket_sub(event_engine_t * engine, socket_t * descriptor, socket_event_subscription_handler_t * handler, event_subscription_meta_t * meta);
extern socket_server_event_subscription_t * event_engine_socket_server_sub(event_engine_t * engine, socket_server_t * descriptor, socket_session_event_subscription_handler_t * sessionOn, socket_server_event_subscription_handler_t * serverOn, event_subscription_meta_t * meta);
extern socket_client_event_subscription_t * event_engine_socket_client_sub(event_engine_t * engine, socket_client_t * descriptor, socket_client_event_subscription_handler_t * handler, socket_client_event_subscription_pool_t * pool, event_subscription_meta_t * meta);

#endif // __SNORLAX__EVENT_ENGINE__H__
