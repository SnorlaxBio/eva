/**
 * @file        snorlax/command.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 18, 2024
 */

#ifndef   __SNORLAX__COMMAND__H__
#define   __SNORLAX__COMMAND__H__

#include <snorlax.h>

#define command_event_type_exec                     0

#define command_event_subscription_state_once       (0x00000001U << 31U)

struct command;
struct command_func;

struct command_event;
struct command_event_func;

struct command_event_subscription;
struct command_event_subscription_func;

struct command_event_generator;
struct command_event_generator_func;

typedef struct command command_t;
typedef struct command_func command_func_t;

typedef struct command_event command_event_t;
typedef struct command_event_func command_event_func_t;

typedef struct command_event_subscription command_event_subscription_t;
typedef struct command_event_subscription_func command_event_subscription_func_t;

typedef struct command_event_generator command_event_generator_t;
typedef struct command_event_generator_func command_event_generator_func_t;

typedef int32_t (*command_executor_t)(command_t *);

struct command {
    command_func_t * func;
    sync_t * sync;

    command_executor_t exec;
};

struct command_func {
    command_t * (*rem)(command_t *);
};

#define command_rem(command)        (command ? command->func->rem(command) : nil)
#define command_exec(command)       (command ? command->exec(command) : fail)

extern command_t * command_func_rem(command_t * command);

#endif // __SNORLAX__COMMAND__H__
