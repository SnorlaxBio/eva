/**
 * @file        snorlax/command/event.h
 * @brief
 * @details
 * 
 * @author      snorlax <opuntia@snorlax.bio>
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

typedef event_subscription_event_t * (*command_event_subscription_procedure_t)(___notnull command_event_subscription_t *, uint32_t, event_subscription_event_t *);

typedef void (*command_event_subscription_handler_t)(___notnull command_event_subscription_t *, uint32_t, event_subscription_event_t *);
typedef event_subscription_event_t * (*command_event_subscription_process_t)(___notnull command_event_subscription_t *, uint32_t, event_subscription_event_t *);

#endif // __SNORLAX__COMMAND_EVENT__H__
