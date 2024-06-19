/**
 * @file        snorlax/descriptor/exception.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 19, 2024
 */

#include "../descriptor.h"

extern void descriptor_exception_func_set(___notnull descriptor_exception_t * exception, uint32_t action, uint32_t type) {
    exception->action = action;
    exception->type = type;
}