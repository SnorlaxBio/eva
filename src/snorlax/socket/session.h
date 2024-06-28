/**
 * @file        snorlax/socket/session.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 27, 2024
 */

#ifndef   __SNORLAX__SOCKET_SESSION__H__
#define   __SNORLAX__SOCKET_SESSION__H__

#include <snorlax/socket.h>

struct socket_session;
struct socket_session_func;

typedef struct socket_session socket_session_t;
typedef struct socket_session_func socket_session_func_t;

struct socket_session {
    socket_session_func_t * func;
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

struct socket_session_func {
    socket_session_t * (*rem)(___notnull socket_session_t *);

    int32_t (*open)(___notnull socket_session_t *);
    int64_t (*read)(___notnull socket_session_t *);
    int64_t (*write)(___notnull socket_session_t *);
    int32_t (*close)(___notnull socket_session_t *);
    int32_t (*check)(___notnull socket_session_t *, uint32_t);

    int32_t (*shutdown)(___notnull socket_session_t *, uint32_t);
};

struct socket_session_event_subscription;

typedef struct socket_session_event_subscription socket_session_event_subscription_t;

typedef void (*socket_session_event_subscription_handler_t)(___notnull socket_session_event_subscription_t *, uint32_t, event_subscription_event_t *);
typedef void (*socket_session_event_subscription_process_t)(___notnull socket_session_event_subscription_t *, uint32_t, event_subscription_event_t *);

typedef int32_t (*socket_session_open_t)(___notnull socket_session_t *);
typedef int64_t (*socket_session_read_t)(___notnull socket_session_t *);
typedef int64_t (*socket_session_write_t)(___notnull socket_session_t *);
typedef int32_t (*socket_session_close_t)(___notnull socket_session_t *);
typedef int32_t (*socket_session_check_t)(___notnull socket_session_t *, uint32_t);
typedef int32_t (*socket_session_shutdown_t)(___notnull socket_session_t *, uint32_t);

extern socket_session_t * socket_session_gen(int32_t value, int32_t domain, int32_t type, int32_t protocol, void * addr, uint64_t addrlen);

#endif // __SNORLAX__SOCKET_SESSION__H__
