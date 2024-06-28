/**
 * @file        snorlax/socket.c
 * @brief
 * @details
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 27, 2024
 */

#include <sys/socket.h>
#include <errno.h>
#include <string.h>

#include "socket.h"

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

static socket_func_t func = {
    socket_func_rem,
    (socket_open_t) descriptor_func_open,
    (socket_read_t) descriptor_func_read,
    (socket_write_t) descriptor_func_write,
    (socket_close_t) descriptor_func_close,
    (socket_check_t) descriptor_func_check,
    socket_func_shutdown
};

extern socket_t * socket_gen(int32_t domain, int32_t type, int32_t protocol, void * addr, uint64_t addrlen) {
    socket_t * descriptor = (socket_t *) calloc(1, sizeof(socket_t));

    descriptor->func = address_of(func);

    descriptor->value = invalid;
    descriptor->buffer.in = buffer_gen(0);
    descriptor->buffer.out = buffer_gen(0);
    descriptor->status = descriptor_state_close;
    descriptor->domain = domain;
    descriptor->type = type;
    descriptor->protocol = protocol;

    if(addr && addrlen) {
        descriptor->addr.value = malloc(addrlen);
        descriptor->addr.len = addrlen;
    } else {
#ifndef   RELEASE
        snorlaxdbg(addr == nil || addrlen == 0, false, "critical", "");
#endif // RELEASE
    }

    return descriptor;
}

extern socket_t * socket_func_rem(___notnull socket_t * descriptor) {
#ifndef   RELEASE
    snorlaxdbg(descriptor == nil, false, "critical", "");
    snorlaxdbg(descriptor->value > invalid, false, "critical", "");
#endif // RELEASE

    descriptor->buffer.in = buffer_rem(descriptor->buffer.in);
    descriptor->buffer.out = buffer_rem(descriptor->buffer.out);
    descriptor->addr.value = memory_rem(descriptor->addr.value);
    descriptor->sync = sync_rem(descriptor->sync);

    free(descriptor);

    return nil;
}

extern int32_t socket_func_shutdown(___notnull socket_t * descriptor, uint32_t how) {
#ifndef   RELEASE
    snorlaxdbg(descriptor == nil, false, "critical", "");
#endif // RELEASE
    
    if(descriptor->value > invalid) {
        int32_t v = invalid;
        if(how == socket_shutdown_type_in) {
            v = SHUT_RD;
        } else if(how == socket_shutdown_type_out) {
            v = SHUT_WR;
        } else if(how == socket_shutdown_type_all){
            v = SHUT_RDWR;
        } else {
#ifndef   RELEASE
            snorlaxdbg(true, false, "critical", "");
#endif // RELEASE
        }
        if(v != invalid) {
            if(shutdown(descriptor->value, v) == fail) {
#ifndef   RELEASE
                snorlaxdbg(false, true, "warning", "fail to shutdown => %d", errno);
#endif // RELEASE
            }
            if(how == socket_shutdown_type_in) {
                descriptor->status = descriptor->status & (~descriptor_state_open_in);
            } else if(how == socket_shutdown_type_out) {
                descriptor->status = descriptor->status & (~descriptor_state_open_out);
            } else if(how == socket_shutdown_type_all){
                descriptor->status = descriptor->status & (~descriptor_state_open);
            }

            return success;
        }

        return fail;
    }

    return success;
}