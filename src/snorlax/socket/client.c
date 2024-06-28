/**
 * @file        snorlax/socket/client.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 27, 2024
 */

#include <sys/socket.h>
#include <errno.h>

#include "client.h"

static socket_client_func_t func = {
    (socket_client_rem_t) socket_func_rem,
    socket_client_func_open,
    (socket_client_read_t) descriptor_func_read,
    (socket_client_write_t) descriptor_func_write,
    (socket_client_close_t) descriptor_func_close,
    (socket_client_check_t) descriptor_func_check,
    (socket_client_shutdown_t) socket_func_shutdown
};

extern socket_client_t * socket_client_gen(int32_t domain, int32_t type, int32_t protocol, void * addr, uint64_t addrlen) {
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

extern int32_t socket_client_func_open(___notnull socket_client_t * descriptor) {
#ifndef   RELEASE
    snorlaxdbg(descriptor == nil, false, "critical", "");
#endif // RELEASE

    if(descriptor_exception_get(descriptor) || (descriptor->status & (descriptor_state_exception | descriptor_state_close))) {
        return fail;
    }

    if(descriptor->value <= invalid) {
        descriptor->value = socket(descriptor->domain, descriptor->type, descriptor->protocol);
        if(descriptor->value <= invalid) {
            descriptor_exception_set(descriptor, descriptor_exception_type_system, errno, socket);
            return fail;
        }
        descriptor_nonblock_on((descriptor_t *) descriptor);

        if(connect(descriptor->value, (struct sockaddr *) descriptor->addr.value, descriptor->addr.len) == fail) {
            if(errno == EAGAIN || errno == EALREADY) {
                return success;
            }

            if(close(descriptor->value) == fail) {
#ifndef   RELEASE
                snorlaxdbg(false, true, "warning", "fail to close => %d", errno);
#endif // RELEASE
            }

            descriptor->value = invalid;
            descriptor_exception_set(descriptor, descriptor_exception_type_system, errno, socket);
            return fail;
        }

        descriptor->status = descriptor->status | descriptor_state_open;
        descriptor->status = descriptor->status & (~descriptor_state_close);

        return success;
    } else if((descriptor->status & descriptor_state_open) == 0) {
        snorlaxdbg(descriptor->status & descriptor_state_open, false, "implement", "");
    }

    return success;
}
