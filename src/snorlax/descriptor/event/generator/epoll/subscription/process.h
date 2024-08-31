/**
 * @file        snorlax/descriptor/event/generator/epoll/process.h
 * @brief
 * @details
 * 
 * @author      snorlax <opuntia@snorlax.bio>
 * @since       June 26, 2024
 */

#ifndef   __SNORLAX__DESCRIPTOR_EVENT_GENERATOR_EPOLL_PROCESS__H__
#define   __SNORLAX__DESCRIPTOR_EVENT_GENERATOR_EPOLL_PROCESS__H__

#include <snorlax/descriptor/event.h>

extern event_subscription_process_t descriptor_event_generator_epoll_subscription_process_get(uint32_t type);

extern void descriptor_event_subscription_process_subscription(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_node_t * node);
extern void descriptor_event_subscription_process_open(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_node_t * node);
extern void descriptor_event_subscription_process_read(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_node_t * node);
extern void descriptor_event_subscription_process_write(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_node_t * node);
extern void descriptor_event_subscription_process_close(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_node_t * node);
extern void descriptor_event_subscription_process_exception(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_node_t * node);

#endif // __SNORLAX__DESCRIPTOR_EVENT_GENERATOR_EPOLL_PROCESS__H__
