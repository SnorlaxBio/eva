/**
 * @file        snorlax/command.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 22, 2024
 */

#ifndef   __SNORLAX__COMMAND__H__
#define   __SNORLAX__COMMAND__H__

#include <snorlax.h>

struct command;
struct command_func;
struct command_event_subscription;

typedef struct command command_t;
typedef struct command_func command_func_t;
typedef struct command_event_subscription command_event_subscription_t;

typedef int32_t (*command_execute_t)(___notnull command_t *);

typedef void (*command_event_subscription_handler_t)(___notnull command_event_subscription_t *, uint32_t, uint64_t);

struct command {
    command_func_t * func;
    sync_t * sync;

    command_execute_t execute;
};

struct command_func {
    command_t * (*rem)(___notnull command_t *);
};

extern command_t * command_gen(___notnull command_execute_t execute);

#define command_rem(command)        ((command)->func->rem(command))
#define command_execute(command)    ((command)->execute(command))

#endif // __SNORLAX__COMMAND__H__
