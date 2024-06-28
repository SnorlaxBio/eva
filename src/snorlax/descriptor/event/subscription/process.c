/**
 * @file        snorlax/descriptor/event/subscription/process.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 29, 2024
 */

#include "process.h"

#include "../generator/epoll/subscription/process.h"

extern event_subscription_process_t descriptor_event_subscription_process_get(uint32_t type) {
    return descriptor_event_generator_epoll_subscription_process_get(type);
}
