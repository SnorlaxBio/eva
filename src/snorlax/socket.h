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

struct socket;
struct socket_addr;
struct socket_func;

typedef struct socket socket_t;
typedef struct socket_addr socket_addr_t;
typedef struct socket_func socket_func_t;

typedef struct socket_event_subscription socket_event_subscription_t;

struct socket_addr {
    uint8_t * value;
    uint64_t len;
};

typedef int32_t (*socket_open_t)(___notnull socket_t *);
typedef int64_t (*sockt_read_t)(___notnull socket_t *);

struct socket {
    socket_func_t * func;
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

struct socket_func {
    socket_t * (*rem)(___notnull socket_t *);

    int32_t (*open)(___notnull socket_t *);
    int64_t (*read)(___notnull socket_t *);
    int64_t (*write)(___notnull socket_t *);
    int32_t (*close)(___notnull socket_t *);
    int32_t (*check)(___notnull socket_t *, uint32_t);

    int32_t (*shutdown)(___notnull socket_t *, uint32_t);
};

typedef void (*socket_event_subscription_handler_t)(___notnull socket_event_subscription_t *, uint32_t, event_subscription_event_t *);

extern socket_t * socket_gen(socket_func_t * func, int32_t domain, int32_t type, int32_t protocol, void * addr, uint64_t addrlen);

extern socket_t * socket_func_rem(___notnull socket_t * s);
extern int64_t socket_func_shutdown(___notnull socket_t * s, uint32_t how);
    // int64_t (*write)(___notnull socket_t *);
    // int32_t (*close)(___notnull socket_t *);
    // int32_t (*check)(___notnull socket_t *, uint32_t);

    // int32_t (*shutdown)(___notnull socket_t *, uint32_t);

#define socket_rem(s)               ((s)->func->rem(s))
#define socket_open(s)              ((s)->func->open(s))
#define socket_read(s)              ((s)->func->read(s))
#define socket_write(s)             ((s)->func->write(s))
#define socket_close(s)             ((s)->func->close(s))
#define socket_check(s, state)      ((s)->func->check(s, state))
#define socket_shutdown(s, how)     ((s)->func->shutdown(s, how))

#endif // __SNORLAX__SOCKET__H__
