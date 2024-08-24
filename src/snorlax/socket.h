/**
 * @file        snorlax/socket.h
 * @brief       
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 27, 2024
 */

#ifndef   __SNORLAX__SOCKET__H__
#define   __SNORLAX__SOCKET__H__

#include <snorlax/descriptor.h>

#define socket_shutdown_type_in         1
#define socket_shutdown_type_out        2
#define socket_shutdown_type_all        3

struct socket;
struct socket_func;

typedef struct socket socket_t;
typedef struct socket_func socket_func_t;

typedef descriptor_buffer_t socket_buffer_t;
typedef descriptor_exception_t socket_exception_t;

struct socket_addr;

typedef struct socket_addr socket_addr_t;

struct socket_event_subscription;

typedef struct socket_event_subscription socket_event_subscription_t;

typedef void (*socket_event_subscription_handler_t)(___notnull socket_event_subscription_t *, uint32_t, event_subscription_event_t *);
typedef void (*socket_event_subscription_process_t)(___notnull socket_event_subscription_t *, uint32_t, event_subscription_event_t *);
typedef descriptor_buffer_t socket_buffer_t;

struct socket_addr {
    uint8_t * value;
    uint64_t len;
};

struct socket {
    socket_func_t * func;
    sync_t * sync;
    event_object_meta_t * meta;
    socket_buffer_t buffer;
    descriptor_exception_t exception;
    int32_t value;
    uint32_t status;
    int32_t domain;
    int32_t type;
    int32_t protocol;
    socket_addr_t addr;
};

struct socket_func {
    socket_t * (*rem)(___notnull socket_t *);

    int32_t (*open)(___notnull socket_t *);
    int64_t (*read)(___notnull socket_t *);
    int64_t (*write)(___notnull socket_t *);
    int32_t (*close)(___notnull socket_t *);
    int32_t (*check)(___notnull socket_t *, uint32_t);

    int32_t (*shutdown)(___notnull socket_t *, uint32_t);
};

typedef int32_t (*socket_open_t)(___notnull socket_t *);
typedef int64_t (*socket_read_t)(___notnull socket_t *);
typedef int64_t (*socket_write_t)(___notnull socket_t *);
typedef int32_t (*socket_close_t)(___notnull socket_t *);
typedef int32_t (*socket_check_t)(___notnull socket_t *, uint32_t);

extern socket_t * socket_func_rem(___notnull socket_t * descriptor);
extern int32_t socket_func_open(___notnull socket_t * descriptor);
extern int32_t socket_func_shutdown(___notnull socket_t * descriptor, uint32_t how);
extern int32_t socket_func_check(___notnull socket_t * descriptir, uint32_t state);
extern socket_t * socket_gen(int32_t domain, int32_t type, int32_t protocol, void * addr, uint64_t addrlen);

#define socket_rem(descriptor)                  ((descriptor)->func->rem(descriptor))
#define socket_open(descriptor)                 ((descriptor)->func->open(descriptor))
#define socket_read(descriptor)                 ((descriptor)->func->read(descriptor))
#define socket_write(descriptor)                ((descriptor)->func->write(descriptor))
#define socket_close(descriptor)                ((descriptor)->func->close(descriptor))
#define socket_check(descriptor, state)         ((descriptor)->func->check(descriptor, state))
#define socket_shutdown(descriptor, how)        ((descriptor)->func->shutdown(descriptor, how))

#endif // __SNORLAX__SOCKET__H__
