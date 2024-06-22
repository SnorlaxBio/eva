/**
 * @file        snorlax/descriptor/exception.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 22, 2024
 */

#include "../descriptor.h"

extern descriptor_exception_t * descriptor_exception_func_set(___notnull descriptor_t * descriptor, int32_t type, int32_t no, void * f) {
#ifndef   RELEASE
    snorlaxdbg(descriptor == nil, false, "critical", "");
#endif // RELEASE

    descriptor_exception_t * exception = address_of(descriptor->exception);

    if(type) {
        exception->type = type;
        exception->no = no;
        exception->func = f;

        descriptor->status = descriptor->status | descriptor_state_exception;

        return exception;
    } else {
        exception->type = type;
        exception->no = 0;
        exception->func = nil;

        descriptor->status = descriptor->status & (~descriptor_state_exception);

        return nil;
    }
}

extern descriptor_exception_t * descriptor_exception_func_get(___notnull descriptor_t * descriptor) {
#ifndef   RELEASE
    snorlaxdbg(descriptor == nil, false, "critical", "");
#endif // RELEASE

    return descriptor->exception.type == 0 ? nil : address_of(descriptor->exception);
}
