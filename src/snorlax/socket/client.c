/**
 * @file        snorlax/socket/tcp/client.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 27, 2024
 */

#include <sys/socket.h>
#include <errno.h>

#include "client.h"

static socket_func_t func = {
    socket_func_rem,
    socket_client_func_open,
    (socket_func_read_t) descriptor_func_read,
    (socket_func_write_t) descriptor_func_write,
    (socket_func_close_t) descriptor_func_close,
    (socket_func_check_t) descriptor_func_check,
    socket_func_shutdown
};

extern int32_t socket_client_func_open(___notnull socket_t * s) {
#ifndef   RELEASE
    snorlaxdbg(s == nil, false, "critical", "");
#endif // RELEASE

    if(s->value <= invalid) {
        s->value = socket(s->domain, s->type, s->protocol);
        if(s->value > invalid) {
            descriptor_nonblock_on((descriptor_t *) s);
            if(connect(s->value, (struct sockaddr *) s->addr.value, (socklen_t) s->addr.len) == fail) {
#ifndef   RELEASE
                snorlaxdbg(false, true, "implement", "nonblock connect");
                snorlaxdbg(false, true, "descriptor exception", "%d %d %p", descriptor_exception_type_system, errno, connect);
#endif // RELEASE
                descriptor_exception_set((descriptor_t *) s, descriptor_exception_type_system, errno, connect);
                return fail;
            } else {
                s->status = s->status | (descriptor_state_open | descriptor_state_write);
            }
        } else {
#ifndef   RELEASE
            snorlaxdbg(false, true, "descriptor exception", "%d %d %p", descriptor_exception_type_system, errno, socket);
#endif // RELEASE
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

extern socket_func_t * socket_client_func_get(void) {
    return address_of(func);
}
