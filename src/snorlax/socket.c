/**
 * @file        snorlax/socket.c
 * @brief
 * @details
 * @author      snorlax <opuntia@snorlax.bio>
 * @since       June 27, 2024
 */

#include <sys/socket.h>
#include <errno.h>
#include <string.h>

#include <snorlax/buffer/mem.h>

#include "socket.h"

typedef int32_t (*socket_open_t)(___notnull socket_t *);
typedef int64_t (*socket_read_t)(___notnull socket_t *);
typedef int64_t (*socket_write_t)(___notnull socket_t *);
typedef int32_t (*socket_close_t)(___notnull socket_t *);
typedef int32_t (*socket_check_t)(___notnull socket_t *, uint32_t);

static socket_func_t func = {
    socket_func_rem,
    socket_func_open,
    (socket_read_t) descriptor_func_read,
    (socket_write_t) descriptor_func_write,
    (socket_close_t) descriptor_func_close,
    socket_func_check,
    socket_func_shutdown
};

extern socket_t * socket_gen(int32_t domain, int32_t type, int32_t protocol, void * addr, uint64_t addrlen) {
    socket_t * descriptor = (socket_t *) calloc(1, sizeof(socket_t));

    descriptor->func = address_of(func);

    descriptor->value = invalid;
    descriptor->buffer.in = (buffer_t *) buffer_mem_gen(0);
    descriptor->buffer.out = (buffer_t *) buffer_mem_gen(0);
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

extern int32_t socket_func_open(___notnull socket_t * descriptor) {
#ifndef   RELEASE
    snorlaxdbg(descriptor == nil, false, "critical", "");
#endif // RELEASE

    if(descriptor->value <= invalid) {
        descriptor->value = socket(descriptor->domain, descriptor->type, descriptor->protocol);

        if(descriptor->value <= invalid){
            descriptor_exception_set(descriptor, descriptor_exception_type_system, errno, socket);
            return fail;
        }

        descriptor->status = descriptor->status & (~descriptor_state_close);
        descriptor->status = descriptor->status | (descriptor_state_open | descriptor_state_write);
    }

    return success;
}

extern socket_t * socket_func_rem(___notnull socket_t * descriptor) {
#ifndef   RELEASE
    snorlaxdbg(descriptor == nil, false, "critical", "");
    snorlaxdbg(descriptor->value > invalid, false, "critical", "");
#endif // RELEASE

    if(descriptor->buffer.in) descriptor->buffer.in = buffer_rem(descriptor->buffer.in);
    if(descriptor->buffer.out) descriptor->buffer.out = buffer_rem(descriptor->buffer.out);

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

extern int32_t socket_func_check(___notnull socket_t * descriptor, uint32_t state) {
#ifndef   RELEASE
    snorlaxdbg(descriptor == nil, false, "critical", "");
#endif // RELEASE
    int32_t err = 0;
    socklen_t n = sizeof(int32_t);
    if(getsockopt(descriptor->value, SOL_SOCKET, SO_ERROR, &err, &n) == 0) {
        if(err == EAGAIN || err == EINPROGRESS) {
#ifndef   RELEASE
            snorlaxdbg(false, true, "check", "again || progress");
#endif // RELEASE
        }

        if(err == 0) {
            descriptor->status = descriptor->status | (descriptor_state_open | descriptor_state_write);
            return true;
        }
    } else {
#ifndef   RELEASE
            snorlaxdbg(false, true, "check", "fail to getsockopt => %d", errno);
#endif // RELEASE
    }

    return false;
}

extern int32_t socket_func_error_retrieve(socket_t * descriptor) {
#ifndef   RELEASE
    snorlaxdbg(descriptor == nil, false, "critical", "");
#endif // RELEASE

    int32_t value = 0;
    socklen_t valuelen = sizeof(int32_t);

    if(getsockopt(descriptor->value, SOL_SOCKET, SO_ERROR, &value, &valuelen) == success) {
        return value;
    }

    snorlaxdbg(false, true, "caution", "fail to getsockopt(...) => %d", errno);

    return 0;
}