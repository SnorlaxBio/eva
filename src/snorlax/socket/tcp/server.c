/**
 * @file        snorlax/socket/tcp/server.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 27, 2024
 */

#include "server.h"

    // socket_t * (*rem)(___notnull socket_t *);

    // int32_t (*open)(___notnull socket_t *);
    // int64_t (*read)(___notnull socket_t *);
    // int64_t (*write)(___notnull socket_t *);
    // int32_t (*close)(___notnull socket_t *);
    // int32_t (*check)(___notnull socket_t *, uint32_t);

static socket_func_t func = {

};

extern socket_func_t * socket_tcp_server_func_get(void) {
    return address_of(func);
}