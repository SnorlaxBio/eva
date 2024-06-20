/**
 * @file        snorlax/descriptor/event/subscription/action.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 20, 2024
 */

#ifndef   __SNORLAX__DESCRIPTOR_EVENT_SUBSCRIPTION_ACTION__H__
#define   __SNORLAX__DESCRIPTOR_EVENT_SUBSCRIPTION_ACTION__H__

#include <snorlax/descriptor.h>

extern descriptor_event_action_t descriptor_event_subscription_action_get(uint32_t type);

extern int32_t descriptor_event_subscription_action_func_open(___notnull descriptor_event_subscription_t * subscription, uint64_t param);
extern int32_t descriptor_event_subscription_action_func_in(___notnull descriptor_event_subscription_t * subscription, uint64_t param);
extern int32_t descriptor_event_subscription_action_func_out(___notnull descriptor_event_subscription_t * subscription, uint64_t param);
extern int32_t descriptor_event_subscription_action_func_close(___notnull descriptor_event_subscription_t * subscription, uint64_t param);
extern int32_t descriptor_event_subscription_action_func_exception(___notnull descriptor_event_subscription_t * subscription, uint64_t param);

#endif // __SNORLAX__DESCRIPTOR_EVENT_SUBSCRIPTION_ACTION__H__
