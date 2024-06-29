/**
 * @file        snorlax/socket/server.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 27, 2024
 */

#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>

#include "server.h"

static socket_server_func_t func = {
    (socket_server_rem_t) socket_func_rem,
    (socket_server_open_t) socket_server_func_open,
    (socket_server_read_t) socket_server_func_read,
    (socket_server_write_t) socket_server_func_write,
    (socket_server_close_t) descriptor_func_close,
    (socket_server_check_t) socket_func_check,
    (socket_server_shutdown_t) socket_func_shutdown,
};

extern socket_server_t * socket_server_gen(int32_t domain, int32_t type, int32_t protocol, void * addr, uint64_t addrlen) {
    socket_server_t * descriptor = (socket_server_t *) calloc(1, sizeof(socket_server_t));

    descriptor->func = address_of(func);

    descriptor->value = invalid;
    descriptor->buffer.in = buffer_gen(0);
    descriptor->buffer.out = nil;
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

extern int32_t socket_server_func_open(___notnull socket_server_t * descriptor) {
#ifndef   RELEASE
    snorlaxdbg(descriptor == nil, false, "critical", "");
#endif // RELEASE

    if(descriptor_exception_get(descriptor)) {
        return fail;
    }

    if(descriptor->value <= invalid) {
        descriptor->value = socket(descriptor->domain, descriptor->type, descriptor->protocol);
        if(descriptor->value <= invalid) {
            descriptor_exception_set(descriptor, descriptor_exception_type_system, errno, socket);
            return fail;
        }

        descriptor_nonblock_on((descriptor_t *) descriptor);

        if(bind(descriptor->value, (struct sockaddr *) descriptor->addr.value, (socklen_t) descriptor->addr.len) == fail) {
            if(close(descriptor->value) == fail) {
#ifndef   RELEASE
                snorlaxdbg(false, true, "warning", "fail to close => %d", errno);
#endif // RELEASE
            }

            descriptor->value = invalid;
            descriptor_exception_set(descriptor, descriptor_exception_type_system, errno, socket);
            return fail;
        }

        if(listen(descriptor->value, SOMAXCONN) == fail) {
            if(close(descriptor->value) == fail) {
#ifndef   RELEASE
                snorlaxdbg(false, true, "warning", "fail to close => %d", errno);
#endif // RELEASE
            }

            descriptor->value = invalid;

            descriptor_exception_set(descriptor, descriptor_exception_type_system, errno, socket);

            return fail;
        }

        descriptor->status = descriptor->status | descriptor_state_open_in;
        descriptor->status = descriptor->status & (~descriptor_state_close);

        return success;
    }

    return success;
}

extern int64_t socket_server_func_read(___notnull socket_server_t * descriptor) {
#ifndef   RELEASE
    snorlaxdbg(descriptor == nil, false, "critical", "");
#endif // RELEASE

    if(descriptor_exception_get(descriptor) || (descriptor->status & (descriptor_state_exception | descriptor_state_close))) {
        return fail;
    }

    if(descriptor->value > invalid) {
        buffer_t * buffer = descriptor->buffer.in;
        int32_t value = accept(descriptor->value, (struct sockaddr *)(buffer_back(buffer) + sizeof(socklen_t)), (socklen_t *) (buffer_back(buffer)));
        if(value > invalid) {
            uint64_t n = *((socklen_t *) (buffer_back(buffer))) + sizeof(socklen_t);

            *(int32_t *)(buffer_back(buffer) + n) = value;

            n = n + sizeof(int32_t);

            buffer_size_set(buffer, buffer_size_get(buffer) + n);

            return n;
        } else {
            if(errno == EAGAIN || errno == EWOULDBLOCK) {
                descriptor->status = descriptor->status & (~descriptor_state_read);
                return success;
            } else {
                descriptor_exception_set(descriptor, descriptor_exception_type_system, errno, accept);
                return fail;
            }
        }
    }
#ifndef   RELEASE
    snorlaxdbg(false, true, "warning", "");
#endif // RELEASE

    return fail;
}

extern int64_t socket_server_func_write(___notnull socket_server_t * descriptor) {
    return success;
}
