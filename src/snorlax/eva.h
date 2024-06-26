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
#include <snorlax/descriptor.h>
#include <snorlax/socket.h>

struct event_subscription;
struct descriptor_event_subscription;
struct event_engine;
struct event_subscription_event;

typedef struct descriptor_event_subscription descriptor_event_subscription_t;
typedef struct event_subscription event_subscription_t;
typedef struct event_engine event_engine_t;
typedef struct event_subscription_event event_subscription_event_t;
typedef void (*event_engine_cancel_t)(const event_engine_t *);

extern int32_t snorlax_eva_on(void);
extern int32_t snorlax_eva_off(event_engine_cancel_t cancel);
extern int32_t snorlax_eva_run(void);

extern event_subscription_t * snorlax_eva_command_sub(___notnull command_t * command, int32_t retry, command_event_subscription_handler_t * handler);
extern event_subscription_t * snorlax_eva_descriptor_sub(___notnull descriptor_t * descriptor, descriptor_event_subscription_handler_t * handler);
extern event_subscription_t * snorlax_eva_socket_sub(___notnull socket_t * s, socket_event_subscription_handler_t * handler);

extern buffer_t * snorlax_eva_descriptor_buffer_in_get(descriptor_event_subscription_t * subscription);

#define snorlax_eva_subscription_rem(subscription)      (object_rem((object_t *) subscription))

#endif // __SNORLAX__EVA__H__