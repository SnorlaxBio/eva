/**
 * @file        snorlax/socket/session.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 27, 2024
 */

#include "session.h"

#include "../socket.h"

// REMOVE 시에 세션을 지우자... 

extern socket_session_t * socket_session_gen(int32_t value, int32_t domain, int32_t type, int32_t protocol, void * addr, uint64_t addrlen) {
#ifndef   RELEASE
    snorlaxdbg(value <= invalid, false, "critical", "");
#endif // RELEASE

    socket_session_t * descriptor = (socket_session_t *) socket_gen(domain, type, protocol, addr, addrlen);

    descriptor->value = value;

    return descriptor;
}