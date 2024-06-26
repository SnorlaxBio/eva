/**
 * @file        snorlax/socket/tcp/client.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 27, 2024
 */

#ifndef   __SNORLAX__SOCKET_TCP_CLIENT__H__
#define   __SNORLAX__SOCKET_TCP_CLIENT__H__

#include <snorlax/socket.h>

extern int32_t socket_client_func_open(___notnull socket_t * s);

extern socket_func_t * socket_client_func_get(void);

#endif // __SNORLAX__SOCKET_TCP_CLIENT__H__
