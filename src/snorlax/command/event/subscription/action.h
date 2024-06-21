/**
 * @file        snorlax/command/event/action.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 22, 2024
 */

#ifndef   __SNORLAX__COMMAND_EVENT_SUBSCRIPTION_ACTION__H__
#define   __SNORLAX__COMMAND_EVENT_SUBSCRIPTION_ACTION__H__

#include <snorlax/command/event.h>

extern void command_event_action_execute(___notnull command_event_subscription_t * subscription, event_queue_t * queue);


#endif // __SNORLAX__COMMAND_EVENT_SUBSCRIPTION_ACTION__H__
