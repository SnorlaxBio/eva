/**
 * @file        snorlax/descriptor/exception.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 19, 2024
 */

#include "../descriptor.h"

extern descriptor_exception_t * descriptor_exception_func_set(___notnull descriptor_exception_t * exception, uint32_t type, void * func, uint32_t no) {
#ifndef   RELEASE
    snorlaxdbg(exception == nil, "critical", "");
#endif // RELEASE
    exception->type = type;
    exception->func = func;
    exception->no = no;

    return exception;
}
