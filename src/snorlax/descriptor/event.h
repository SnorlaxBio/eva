/**
 * @file        snorlax/descriptor/event.h
 * @brief
 * @details
 * 
 * @author      snorlax <opuntia@snorlax.bio>
 * @since       June 25, 2024
 */

#ifndef   __SNORLAX__DESCRIPTOR_EVENT__H__
#define   __SNORLAX__DESCRIPTOR_EVENT__H__

#include <snorlax/event.h>
#include <snorlax/descriptor.h>

struct descriptor_event_generator;
struct descriptor_event_generator_func;
struct descriptor_event_subscription;
struct descriptor_event_subscription_func;

typedef struct descriptor_event_generator descriptor_event_generator_t;
typedef struct descriptor_event_generator_func descriptor_event_generator_func_t;
typedef struct descriptor_event_subscription descriptor_event_subscription_t;
typedef struct descriptor_event_subscription_func descriptor_event_subscription_func_t;

typedef void (*descriptor_event_subscription_handler_t)(___notnull descriptor_event_subscription_t *, uint32_t, event_subscription_event_t *);
typedef void (*descriptor_event_subscription_process_t)(___notnull descriptor_event_subscription_t *, uint32_t, event_subscription_event_t *);

#endif // __SNORLAX__DESCRIPTOR_EVENT__H__
