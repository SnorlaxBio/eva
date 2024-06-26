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
#include "descriptor.h"

extern socket_t * socket_gen(socket_func_t * func, int32_t domain, int32_t type, int32_t protocol, void * addr, uint64_t addrlen) {
    socket_t * s = (socket_t *) calloc(1, sizeof(socket_t));

    s->func = func;
    s->domain = domain;
    s->type = type;
    s->protocol = protocol;
    if(addr && addrlen) {
        s->addr.value = malloc(s->addr.len = addrlen);
        memcpy(s->addr.value, addr, addrlen);
    }
    s->value = invalid;

    s->buffer.in = buffer_gen(0);
    s->buffer.out = buffer_gen(0);

    return s;
}

extern int32_t socket_func_shutdown(___notnull socket_t * s, uint32_t how) {
#ifndef   RELEASE
    snorlaxdbg(s == nil, false, "critical", "");
#endif // RELEASE
    if(s->value > invalid) {
        int32_t v = invalid;
        if(how == socket_shutdown_type_in) {
            v = SHUT_RD;
        } else if(how == socket_shutdown_type_out) {
            v = SHUT_WR;
        } else if(how == socket_shutdown_type_all) {
            v = SHUT_RDWR;
        } else {
#ifndef   RELEASE
            snorlaxdbg(true, false, "critical", "");
#endif // RELEASE
        }

        if(shutdown(s->value, v) == fail) {
#ifndef   RELEASE
            snorlaxdbg(false, true, "warning", "fail to shutdown => %d", errno);
#endif // RELEASE
        } else {
            if(how == socket_shutdown_type_in) {
                s->status = s->status & (~descriptor_state_open_in);
            } else if(how == socket_shutdown_type_out) {
                s->status = s->status & (~descriptor_state_open_out);
            } else if(how == socket_shutdown_type_all) {
                s->status = s->status & (~descriptor_state_open);
            } else {
#ifndef   RELEASE
                snorlaxdbg(true, false, "critical", "");
#endif // RELEASE
            }

            if((s->status & descriptor_state_open) == 0) {
                s->value = invalid;
                s->status = s->status | descriptor_state_close;
            }
        }
    }

    return success;
}

extern int32_t socket_func_open(___notnull socket_t * s) {
#ifndef   RELEASE
    snorlaxdbg(s == nil, false, "critical", "");
#endif // RELEASE

    snorlaxdbg(true, false, "critical", "");

    return success;
}

extern socket_t * socket_func_rem(___notnull socket_t * s) {
#ifndef   RELEASE
    snorlaxdbg(s == nil, false, "critical", "");
    snorlaxdbg(s->value >= invalid, false, "critical", "");
#endif // RELEASE

    s->addr.value = memory_rem(s->addr.value);

    s->buffer.in = buffer_rem(s->buffer.in);
    s->buffer.out = buffer_rem(s->buffer.out);

    s->sync = sync_rem(s->sync);

    free(s);

    return nil;
}