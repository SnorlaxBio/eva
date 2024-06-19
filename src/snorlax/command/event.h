/**
 * @file        snorlax/command/event.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 18, 2024
 */

#ifndef   __SNORLAX__COMMAND_EVENT__H__
#define   __SNORLAX__COMMAND_EVENT__H__

#include <snorlax/eva.h>
#include <snorlax/command.h>

struct command_event {
    command_event_func_t * func;
    sync_t * sync;

    event_queue_t * queue;
    event_t * prev;
    event_t * next;

    event_subscription_event_t * node;

    command_event_subscription_t * subscription;
    uint32_t type;
};

struct command_event_func {
    command_event_t * (*rem)(command_event_t *);
    int32_t (*on)(command_event_t *);
};

// TODO: EVENT 객체와 동일하기에 매크로로 변경할 수 있다. 필요시에 재정의하면
//       라이브러리의 크기가 작아진다.
extern command_event_t * command_event_gen(command_event_subscription_t * subscription, uint32_t type);

#define command_event_rem(event)        ((event)->func->rem(event))

#endif // __SNORLAX__COMMAND_EVENT__H__
