/**
 * @file            snorlax/descriptor/event/subscription/handler.h
 * @brief
 * @details
 * 
 * @author          snorlax <ceo@snorlax.bio>
 * @since           June 20, 2024
 */

#ifndef   __SNORLAX__DESCRIPTOR_EVENT_SUBSCRITON_HANDLER__H__
#define   __SNORLAX__DESCRIPTOR_EVENT_SUBSCRITON_HANDLER__H__

#include <snorlax/descriptor.h>

extern descriptor_event_subscription_handler_t descriptor_event_subscription_handler_get(uint32_t type);

extern void descriptor_event_subscription_handler_func_on(___notnull descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param);

extern void descriptor_event_subscription_handler_func_open(___notnull descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param);
extern void descriptor_event_subscription_handler_func_in(___notnull descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param);
extern void descriptor_event_subscription_handler_func_out(___notnull descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param);
extern void descriptor_event_subscription_handler_func_close(___notnull descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param);
extern void descriptor_event_subscription_handler_func_exception(___notnull descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param);

#endif // __SNORLAX__DESCRIPTOR_EVENT_SUBSCRITON_HANDLER__H__
