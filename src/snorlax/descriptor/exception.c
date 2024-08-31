/**
 * @file        snorlax/descriptor/exception.c
 * @brief
 * @details
 * 
 * @author      snorlax <opuntia@snorlax.bio>
 * @since       June 27, 2024
 */

#include "../descriptor.h"

extern descriptor_exception_t * descriptor_exception_func_set(___notnull descriptor_t * descriptor, int32_t type, int32_t no, void * f) {
#ifndef   RELEASE
    snorlaxdbg(descriptor == nil, false, "critical", "");
#endif // RELEASE

    descriptor->exception.type = type;
    descriptor->exception.no = no;
    descriptor->exception.func = f;

    if(descriptor->exception.type) {
        return address_of(descriptor->exception);
    }

    return nil;
}

extern descriptor_exception_t * descriptor_exception_func_get(___notnull descriptor_t * descriptor) {
#ifndef   RELEASE
    snorlaxdbg(descriptor == nil, false, "critical", "");
#endif // RELEASE

    if(descriptor->exception.type) {
        return address_of(descriptor->exception);
    }

    return nil;
}