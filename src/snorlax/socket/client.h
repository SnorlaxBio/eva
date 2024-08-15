/**
 * @file        snorlax/socket/client.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 27, 2024
 */

#ifndef   __SNORLAX__SOCKET_CLIENT__H__
#define   __SNORLAX__SOCKET_CLIENT__H__

#include <snorlax/socket.h>

struct socket_client;
struct socket_client_func;

typedef struct socket_client socket_client_t;
typedef struct socket_client_func socket_client_func_t;

struct socket_client {
    socket_client_func_t * func;
    sync_t * sync;
    descriptor_buffer_t buffer;
    descriptor_exception_t exception;
    int32_t value;
    uint32_t status;
    int32_t domain;
    int32_t type;
    int32_t protocol;
    socket_addr_t addr;
};

struct socket_client_func {
    socket_client_t * (*rem)(___notnull socket_client_t *);

    int32_t (*open)(___notnull socket_client_t *);
    int64_t (*read)(___notnull socket_client_t *);
    int64_t (*write)(___notnull socket_client_t *);
    int32_t (*close)(___notnull socket_client_t *);
    int32_t (*check)(___notnull socket_client_t *, uint32_t);

    int32_t (*shutdown)(___notnull socket_client_t *, uint32_t);
};

struct socket_client_event_subscription;
struct socket_client_event_subscription_pool;

typedef struct socket_client_event_subscription socket_client_event_subscription_t;
typedef struct socket_client_event_subscription_pool socket_client_event_subscription_pool_t;

typedef void (*socket_client_event_subscription_handler_t)(___notnull socket_client_event_subscription_t *, uint32_t, event_subscription_event_t *);
typedef void (*socket_client_event_subscription_process_t)(___notnull socket_client_event_subscription_t *, uint32_t, event_subscription_event_t *);

typedef socket_client_t * (*socket_client_rem_t)(___notnull socket_client_t *);
typedef int32_t (*socket_client_open_t)(___notnull socket_client_t *);
typedef int64_t (*socket_client_read_t)(___notnull socket_client_t *);
typedef int64_t (*socket_client_write_t)(___notnull socket_client_t *);
typedef int32_t (*socket_client_close_t)(___notnull socket_client_t *);
typedef int32_t (*socket_client_check_t)(___notnull socket_client_t *, uint32_t);
typedef int32_t (*socket_client_shutdown_t)(___notnull socket_client_t *, uint32_t);

extern int32_t socket_client_func_open(___notnull socket_client_t * descriptor);

extern socket_client_t * socket_client_gen(int32_t domain, int32_t type, int32_t protocol, void * addr, uint64_t addrlen);

#define socket_client_rem(descriptor)               ((descriptor)->func->rem(descriptor))
#define socket_client_open(descriptor)              ((descriptor)->func->open(descriptor))
#define socket_client_read(descriptor)              ((descriptor)->func->read(descriptor))
#define socket_client_write(descriptor)             ((descriptor)->func->write(descriptor))
#define socket_client_close(descriptor)             ((descriptor)->func->close(descriptor))
#define socket_client_check(descriptor, state)      ((descriptor)->func->check(descriptor, state))
#define socket_client_shutdown(descriptor, how)     ((descriptor)->func->shutdown(descriptor, how))

#endif // __SNORLAX__SOCKET_CLIENT__H__
