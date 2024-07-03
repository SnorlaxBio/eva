#ifndef   __SNORLAX__EVENT__H__
#define   __SNORLAX__EVENT__H__

#include <snorlax.h>

#define event_subscription_type_command         1
#define event_subscription_type_descriptor      2

struct event;
struct event_func;
struct event_queue;
struct event_queue_func;
struct event_subscription;
struct event_subscription_func;
struct event_subscription_event;
struct event_subscription_event_func;
struct event_subscription_event_queue;
struct event_subscription_event_queue_func;
struct event_engine;
struct event_engine_func;
struct event_generator;
struct event_generator_func;
struct event_generator_set;
struct event_generator_set_func;
struct event_processor;
struct event_processor_func;
struct event_processor_pool;
struct event_processor_pool_func;

typedef struct event event_t;
typedef struct event_func event_func_t;
typedef struct event_queue event_queue_t;
typedef struct event_queue_func event_queue_func_t;
typedef struct event_subscription event_subscription_t;
typedef struct event_subscription_func event_subscription_func_t;
typedef struct event_subscription_event event_subscription_event_t;
typedef struct event_subscription_event_func event_subscription_event_func_t;
typedef struct event_subscription_event_queue event_subscription_event_queue_t;
typedef struct event_subscription_event_queue_func event_subscription_event_queue_func_t;
typedef struct event_engine event_engine_t;
typedef struct event_engine_func event_engine_func_t;
typedef struct event_generator event_generator_t;
typedef struct event_generator_func event_generator_func_t;
typedef struct event_generator_set event_generator_set_t;
typedef struct event_generator_set_func event_generator_set_func_t;
typedef struct event_processor event_processor_t;
typedef struct event_processor_func event_processor_func_t;
typedef struct event_processor_pool event_processor_pool_t;
typedef struct event_processor_pool_func event_processor_pool_func_t;

typedef event_subscription_event_t event_subscription_node_t;

typedef void (*event_subscription_process_t)(___notnull event_subscription_t *, uint32_t, event_subscription_node_t *);
typedef void (*event_processor_cancel_t)(___notnull event_processor_t *);
typedef void (*event_engine_cancel_t)(___notnull const event_engine_t *);
typedef void (*event_subscription_handler_t)(___notnull event_subscription_t *, uint32_t, event_subscription_event_t *);

/**
 * @struct      struct event
 * @brief       event object
 * @details     이벤트 라이브러리에서 이벤트가 발생했을 때, 이벤트와 관련한 내용과 상태를 관리하는 이벤트 객체입니다.
 *              O(1) 의 삽입과 삭제가 일어나야 하기 때문에, EVENT 객체에 QUEUE, PREV, NEXT 등과 같은 리스트 노드와
 *              유사한 멤버 변수를 정의하였습니다. 이벤트 서브스크립션이란 객체에서는 발생한 이벤트를 관리합니다.
 *              처리 중일 수도 있고, 그렇지 않을 수도 있습니다. 처리 중이라면 큰 상관이 없지만, 처리를 기다리고 있는
 *              이벤트의 경우 서브스크립션 객체가 메모리 상에서 해제가 되면, 프로그램 상에서 예외를 발생시킬 수 있습니다.
 *              그래서 미처리 이벤트가 있는 서브스크립션의 경우 삭제 시에 모든 이벤트들을 정상적으로 메모리 상에서
 *              해제하거나 처리해주어야 합니다. 즉, 이벤트 큐에 존재하는 이벤트 들을 삭제하는 매커니즘이 필요하고
 *              그런 삭제 로직이 O(1) 로 이루어져야 하기 때문에, 리스트 노드처럼 참조를 통하여 큐 상에서의 이전 이벤트
 *              그리고 다음 이벤트를 멤버로 가지고 있는 것 입니다.
 * 
 *              그 외에 이벤트가 발생한 서브스크립션 객체와 서브스크립션 큐에서 관리하는 노드 그리고 발생한 이벤트 타입을
 *              멤버 변수로 가지고 있어서, 이벤트 큐에서 추출하면 서브스크립션의 이벤트 타입과 관련한 액션을 수행할
 *              것입니다. 이벤트의 파라미터가 필요한 경우 node 의 생성 시에 파라미터 정보를 가지고 있도록 할 수 있습니다.
 * 
 *              c로 이벤트 라이브러리를 만들고 있는 과정에서 이벤트 객체에 대한 설계 컨셉을 포스트합니다. 보통의 객체보다 크기가 큰데,
 *              O(1) 으로 삭제될 수 있도록 컨셉을 잡았기 때문입니다. 디스크립터 객체가 중간에 삭제되었을 경우 이벤트 큐에 있는 관련
 *              이벤트가 삭제되어야 하는데, 이 때 역시 리스트의 노드를 이벤트 객체가 변수로 가지고 있어서, O(1) 의 삭제가 가능합니다.
 * 
 *              In the process of creating an event library in C, I am posting a design concept for an event object.
 *              It is larger than a normal object, and this is because the concept was designed so that it can be deleted in O(1).
 *              If the descriptor object is deleted in the middle, the related event in the event queue must be deleted.
 *              In this case, the event object also has the node in the list as a variable, so deletion is possible in O(1).
 * 
 * @version     0.0.1
 */
struct event {
    event_func_t * func;
    sync_t * sync;
    event_queue_t * queue;
    event_t * prev;
    event_t * next;
    event_subscription_event_t * node;
    event_subscription_t * subscription;
    event_subscription_process_t process;
    uint32_t type;
};

struct event_func {
    event_t * (*rem)(___notnull event_t *);
    void (*on)(___notnull event_t *);
};

extern event_t * event_gen(___notnull event_subscription_t * subscription, event_subscription_process_t process, uint32_t type, ___notnull event_subscription_event_t * node);

extern void event_func_on(___notnull event_t * event);

#define event_rem(event)        ((event)->func->rem(event))
#define event_on(event)         ((event)->func->on(event))

#endif // __SNORLAX__EVENT__H__
