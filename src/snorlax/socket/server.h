/**
 * @file        snorlax/socket/server.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 27, 2024
 */

#ifndef   __SNORLAX__SOCKET_SERVER__H__
#define   __SNORLAX__SOCKET_SERVER__H__

#include <snorlax/socket.h>

struct socket_server;
struct socket_server_func;

typedef struct socket_server socket_server_t;
typedef struct socket_server_func socket_server_func_t;

struct socket_server {
    socket_server_func_t * func;
    sync_t * sync;
    ___reference event_object_meta_t * meta;
    descriptor_buffer_t buffer;
    descriptor_exception_t exception;
    int32_t value;
    uint32_t status;
    int32_t domain;
    int32_t type;
    int32_t protocol;
    socket_addr_t addr;
};

struct socket_server_func {
    socket_server_t * (*rem)(___notnull socket_server_t *);

    int32_t (*open)(___notnull socket_server_t *);
    int64_t (*read)(___notnull socket_server_t *);
    int64_t (*write)(___notnull socket_server_t *);
    int32_t (*close)(___notnull socket_server_t *);
    int32_t (*check)(___notnull socket_server_t *, uint32_t);

    int32_t (*shutdown)(___notnull socket_server_t *, uint32_t);
};

struct socket_server_event_subscription;

typedef struct socket_server_event_subscription socket_server_event_subscription_t;

typedef void (*socket_server_event_subscription_handler_t)(___notnull socket_server_event_subscription_t *, uint32_t, event_subscription_event_t *);
typedef void (*socket_server_event_subscription_process_t)(___notnull socket_server_event_subscription_t *, uint32_t, event_subscription_event_t *);

typedef socket_server_t * (*socket_server_rem_t)(___notnull socket_server_t *);
typedef int32_t (*socket_server_open_t)(___notnull socket_server_t *);
typedef int64_t (*socket_server_read_t)(___notnull socket_server_t *);
typedef int64_t (*socket_server_write_t)(___notnull socket_server_t *);
typedef int32_t (*socket_server_close_t)(___notnull socket_server_t *);
typedef int32_t (*socket_server_check_t)(___notnull socket_server_t *, uint32_t);
typedef int32_t (*socket_server_shutdown_t)(___notnull socket_server_t *, uint32_t);

extern int32_t socket_server_func_open(___notnull socket_server_t * descriptor);
extern int64_t socket_server_func_read(___notnull socket_server_t * descriptor);
extern int64_t socket_server_func_write(___notnull socket_server_t * descriptor);

extern socket_server_t * socket_server_gen(int32_t domain, int32_t type, int32_t protocol, void * addr, uint64_t addrlen);

#define socket_server_rem(descriptor)               ((descriptor)->func->rem(descriptor))
#define socket_server_open(descriptor)              ((descriptor)->func->open(descriptor))
#define socket_server_read(descriptor)              ((descriptor)->func->read(descriptor))
#define socket_server_write(descriptor)             ((descriptor)->func->write(descriptor))
#define socket_server_close(descriptor)             ((descriptor)->func->close(descriptor))
#define socket_server_check(descriptor, state)      ((descriptor)->func->check(descriptor, state))
#define socket_server_shutdown(descriptor, how)     ((descriptor)->func->shutdown(descriptor, how))


#endif // __SNORLAX__SOCKET_SERVER__H__
