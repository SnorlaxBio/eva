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
#include "../descriptor.h"

static socket_func_t func = {
    socket_func_rem,
    socket_func_open,
    (socket_func_read_t) descriptor_func_read,
    (socket_func_write_t) descriptor_func_write,
    (socket_func_close_t) descriptor_func_close,
    (socket_func_check_t) descriptor_func_check,
    socket_func_shutdown
};

extern socket_func_t * socket_session_func_get(void) {
    return address_of(func);
}