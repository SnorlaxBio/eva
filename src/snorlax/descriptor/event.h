/**
 * @file        snorlax/descriptor/event.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 22, 2024
 */

#ifndef   __SNORLAX__DESCRIPTOR_EVENT__H__
#define   __SNORLAX__DESCRIPTOR_EVENT__H__

#include <snorlax/eva.h>
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

typedef void (*descriptor_event_subscription_handler_t)(___notnull descriptor_event_subscription_t *, uint32_t, uint64_t);
typedef uint64_t (*descriptor_event_subscription_process_t)(___notnull descriptor_event_subscription_t *, uint32_t, uint64_t);

#endif // __SNORLAX__DESCRIPTOR_EVENT__H__
