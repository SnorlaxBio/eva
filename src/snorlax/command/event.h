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

#include <snorlax/event.h>

struct command_event_generator;
struct command_event_generator_func;
struct command_event_subscription;
struct command_event_subscription_func;

typedef struct command_event_generator command_event_generator_t;
typedef struct command_event_generator_func command_event_generator_func_t;
typedef struct command_event_subscription command_event_subscription_t;
typedef struct command_event_subscription_func command_event_subscription_func_t;

typedef void (*command_event_subscription_handler_t)(___notnull command_event_subscription_t *, int32_t, uint64_t);
typedef uint64_t (*command_event_subscription_process_t)(___notnull command_event_subscription_t *, int32_t, uint64_t);

#endif // __SNORLAX__COMMAND_EVENT__H__
