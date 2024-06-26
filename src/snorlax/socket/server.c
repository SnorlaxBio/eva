/**
 * @file        snorlax/socket/tcp/server.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 27, 2024
 */

#include <sys/socket.h>
#include <errno.h>

#include "server.h"

static socket_func_t func = {
    socket_func_rem,
    socket_server_func_open,
    socket_server_func_read,
    socket_server_func_write,
    (socket_func_close_t) descriptor_func_close,
    (socket_func_check_t) descriptor_func_check,
    socket_func_shutdown
};

extern int32_t socket_server_func_open(___notnull socket_t * s) {
#ifndef   RELEASE
    snorlaxdbg(s == nil, false, "critical", "");
#endif // RELEASE

    if(s->value <= invalid) {
        s->value = socket(s->domain, s->type, s->protocol);
        if(s->value > invalid) {
            descriptor_nonblock_on((descriptor_t *) s);
            if(bind(s->value, (struct sockaddr *) s->addr.value, (socklen_t) s->addr.len) == fail) {
                descriptor_exception_set((descriptor_t *) s, descriptor_exception_type_system, errno, bind);
                return fail;
            }
            if(listen(s->value, SOMAXCONN) == fail) {
                descriptor_exception_set((descriptor_t *) s, descriptor_exception_type_system, errno, listen);
                return fail;
            }
            s->status = s->status | descriptor_state_open_in;
        } else {
            descriptor_exception_set((descriptor_t *) s, descriptor_exception_type_system, errno, socket);
            return fail;
        }
    } else {
#ifndef   RELEASE
        snorlaxdbg(false, true, "warning", "");
#endif // RELEASE
    }

    return success;
}

extern int64_t socket_server_func_read(___notnull socket_t * s) {
#ifndef   RELEASE
    snorlaxdbg(s == nil, false, "critical", "");
#endif // RELEASE
    if(s->value > invalid) {
        if(s->status & descriptor_state_open_in) {
            buffer_t * buffer = s->buffer.in;
            uint64_t size = sizeof(int32_t) + s->addr.len;
            if(buffer_remain(buffer) < size) {
                buffer_capacity_set(buffer, buffer_capacity_get(buffer) + (size * 16));
            }
            socklen_t len = s->addr.len;
            int32_t value = accept(s->value, (struct sockaddr *) buffer_back(buffer) + sizeof(int32_t), &len);
            if(value > invalid) {
                s->status = s->status | descriptor_state_read;

                *((int32_t *) buffer_back(buffer)) = value;
                buffer_size_set(buffer, buffer_size_get(buffer) + sizeof(int32_t) + s->addr.len);

                return sizeof(int32_t) + s->addr.len;
            } else {
                descriptor_exception_set((descriptor_t *) s, descriptor_exception_type_system, errno, accept);
#ifndef   RELEASE
                snorlaxdbg(true, false, "critical", "");
#endif // RELEASE
            }

            return fail;
        }
#ifndef   RELEASE
        snorlaxdbg(false, true, "caution", "(descriptor->status & descriptor_state_open_in) == 0");
#endif // RELEASE

        return success;
    } else {
#ifndef   RELEASE
        snorlaxdbg(false, true, "warning", "descriptor is not open");
#endif // RELEASE
    }

    return fail;
}

extern int64_t socket_server_func_write(___notnull socket_t * s) {
#ifndef   RELEASE
    snorlaxdbg(s == nil, false, "critical", "");
#endif // RELEASE

    snorlaxdbg(true, false, "critical", "");

    return success;
}

extern socket_func_t * socket_server_func_get(void) {
    return address_of(func);
}