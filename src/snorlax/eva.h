/**
 * @file        snorlax/eva.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 21, 2024
 */

#ifndef   __SNORLAX__EVA__H__
#define   __SNORLAX__EVA__H__

#include <snorlax.h>
#include <snorlax/command.h>

struct event_subscription;

typedef struct event_subscription event_subscription_t;

extern int32_t snorlax_eva_on(void);
extern int32_t snorlax_eva_run(void);

extern event_subscription_t * snorlax_eva_command_sub(___notnull command_t * command, int32_t retry, command_event_subscription_handler_t * handler);

#endif // __SNORLAX__EVA__H__