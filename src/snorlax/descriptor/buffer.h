/**
 * @file        snorlax/descriptor/buffer.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 13, 2024
 */

#ifndef   __SNORLAX__DESCRIPTOR_BUFFER__H__
#define   __SNORLAX__DESCRIPTOR_BUFFER__H__

#include <snorlax/buffer.h>

struct descriptor_buffer;

typedef struct descriptor_buffer descriptor_buffer_t;

struct descriptor_buffer {
    buffer_t * in;
    buffer_t * out;
};

#endif // __SNORLAX__DESCRIPTOR_BUFFER__H__
