/**
 * @file        snorlax/eva.h
 * @brief
 * @details
 * 
 * @author      snorlax <opuntia@snorlax.bio>
 * @since       June 21, 2024
 */

#ifndef   __SNORLAX__EVA__H__
#define   __SNORLAX__EVA__H__

#include <snorlax.h>
#include <snorlax/command.h>
#include <snorlax/descriptor.h>
#include <snorlax/socket.h>
#include <snorlax/socket/client.h>
#include <snorlax/socket/server.h>
#include <snorlax/socket/session.h>

struct event_subscription;
struct descriptor_event_subscription;
struct event_engine;
struct event_subscription_event;
struct event_subscription_meta;

struct socket_server_event_subscription;
struct socket_client_event_subscription;
struct socket_client_pool_event_subscription;


typedef struct descriptor_event_subscription descriptor_event_subscription_t;
typedef struct event_subscription event_subscription_t;
typedef struct event_engine event_engine_t;
typedef struct event_subscription_event event_subscription_event_t;
typedef struct socket_server_event_subscription socket_server_event_subscription_t;
typedef struct socket_client_event_subscription socket_client_event_subscription_t;
typedef struct socket_client_pool_event_subscription socket_client_pool_event_subscription_t;
typedef struct event_subscription_meta event_subscription_meta_t;
typedef void (*event_engine_cancel_t)(const event_engine_t *);

extern int32_t snorlax_eva_on(void);
extern int32_t snorlax_eva_off(event_engine_cancel_t cancel);
extern int32_t snorlax_eva_run(void);

extern event_subscription_t * snorlax_eva_command_sub(___notnull command_t * command, int32_t retry, command_event_subscription_handler_t * handler);
extern descriptor_event_subscription_t * snorlax_eva_descriptor_sub(___notnull descriptor_t * descriptor, descriptor_event_subscription_handler_t * handler);
extern socket_event_subscription_t * snorlax_eva_socket_sub(___notnull socket_t * descriptor, socket_event_subscription_handler_t * handler);
extern socket_client_event_subscription_t * snorlax_eva_socket_client_sub(___notnull socket_client_t * descriptor, socket_client_event_subscription_handler_t * handler);
extern socket_server_event_subscription_t * snorlax_eva_socket_server_sub(___notnull socket_server_t * descriptor, socket_session_event_subscription_handler_t * sessionOn, socket_server_event_subscription_handler_t * serverOn);

/**
 * @deprecated
 */
extern buffer_t * snorlax_eva_descriptor_buffer_in_get(descriptor_event_subscription_t * subscription);
/**
 * @deprecated
 */
extern buffer_t * snorlax_eva_descriptor_buffer_out_get(descriptor_event_subscription_t * subscription);

extern buffer_t * snorlax_eva_descriptor_event_subscription_buffer_in_get(descriptor_event_subscription_t * subscription);
extern buffer_t * snorlax_eva_descriptor_event_subscription_buffer_out_get(descriptor_event_subscription_t * subscription);

extern void snorlax_eva_descriptor_write(descriptor_event_subscription_t * subscription, const char * data, uint64_t len);
extern void snorlax_eva_descriptor_close(descriptor_event_subscription_t * subscription);
#define snorlax_eva_subscription_rem(subscription)      (object_rem((object_t *) subscription))

#endif // __SNORLAX__EVA__H__