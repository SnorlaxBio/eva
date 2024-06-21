/**
 * @file        snorlax/command/event.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 22, 2024
 */

#ifndef   __SNORLAX__COMMAND_EVENT__H__
#define   __SNORLAX__COMMAND_EVENT__H__

#include <snorlax/eva.h>
#include <snorlax/command.h>
#include <snorlax/event.h>

struct command_event;
struct command_event_func;

struct command_event_subscription;
struct command_event_subscription_func;
struct command_event_generator;
struct command_event_generator_func;

typedef struct command_event command_event_t;
typedef struct command_event_func command_event_func_t;

typedef struct command_event_subscription command_event_subscription_t;
typedef struct command_event_subscription_func command_event_subscription_func_t;

typedef struct command_event_generator command_event_generator_t;
typedef struct command_event_generator_func command_event_generator_func_t;

typedef event_subscription_event_t command_event_subscription_event_t;
typedef event_subscription_event_queue_t command_event_subscription_event_queue_t;

struct command_event {
    command_event_func_t * func;
    sync_t * sync;

    event_queue_t * queue;
    event_t * prev;
    event_t * next;
    command_event_subscription_event_t * node;
    command_event_subscription_t * subscription;
    uint32_t type;
};

struct command_event_func {
    command_event_t * (*rem)(___notnull command_event_t *);
};

extern command_event_t * command_event_gen(___notnull command_event_subscription_t * subscription, uint32_t type, ___notnull command_event_subscription_event_t * node);

#define command_event_rem(event)        ((event)->func->rem(event))
#define command_event_on(event)         (event_func_on((event_t *) event))

#endif // __SNORLAX__COMMAND_EVENT__H__
