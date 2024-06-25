/**
 * @file        snorlax/command/event/subscription/process.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 25, 2024
 */

#ifndef   __SNORLAX__COMMAND_EVENT_SUBSCRIPTION_PROCESS__H__
#define   __SNORLAX__COMMAND_EVENT_SUBSCRIPTION_PROCESS__H__

#include <snorlax/command.h>
#include <snorlax/command/event.h>

extern void command_event_subscription_process_execute(command_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node);

#endif // __SNORLAX__COMMAND_EVENT_SUBSCRIPTION_PROCESS__H__
