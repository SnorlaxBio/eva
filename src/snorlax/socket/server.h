/**
 * @file        snorlax/socket/tcp/server.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 27, 2024
 */

#ifndef   __SNORLAX__SOCKET_SERVER__H__
#define   __SNORLAX__SOCKET_SERVER__H__

#include <snorlax/socket.h>

extern int32_t socket_server_func_open(___notnull socket_t * s);
extern int64_t socket_server_func_read(___notnull socket_t * s);
extern int64_t socket_server_func_write(___notnull socket_t * s);

extern socket_func_t * socket_server_func_get(void);

#endif // __SNORLAX__SOCKET_SERVER__H__
