/**
 * @file        snorlax/descriptor.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 22, 2024
 */

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include "descriptor.h"

static descriptor_t * descriptor_func_rem(___notnull descriptor_t * descriptor);

static descriptor_func_t func = {
    descriptor_func_rem,
    descriptor_func_open,
    descriptor_func_read,
    descriptor_func_write,
    descriptor_func_close,
    descriptor_func_check
};

extern descriptor_func_t * descriptor_func_get(void) {
    return address_of(func);
}

extern descriptor_t * descriptor_gen(int32_t value) {
    descriptor_t * descriptor = (descriptor_t *) calloc(1, sizeof(descriptor_t));

    descriptor->func = address_of(func);

    descriptor->value = value;

    descriptor->buffer.in = buffer_gen(0);
    descriptor->buffer.out = buffer_gen(0);

    if(value > invalid) {
        descriptor->status = descriptor_state_open;
        descriptor_nonblock_on(descriptor);
    } else {
        descriptor->status = descriptor_state_close;
    }
    
    return descriptor;
}

extern int32_t descriptor_func_open(___notnull descriptor_t * descriptor){
#ifndef   RELEASE
    snorlaxdbg(descriptor == nil, false, "critical", "");
#endif // RELEASE

    snorlaxdbg(true, false, "critical", "");

    return fail;
}

extern int64_t descriptor_func_read(___notnull descriptor_t * descriptor) {
#ifndef   RELEASE
    snorlaxdbg(descriptor == nil, false, "critical", "");
#endif // RELEASE
    if(descriptor->value > invalid) {
        if(descriptor->status & descriptor_state_open_in) {
            buffer_t * buffer = descriptor->buffer.in;
            buffer_capacity_set(buffer, buffer_size_get(buffer) + 8192);
            int64_t n = read(descriptor->value, buffer_back(buffer), buffer_remain(buffer));

            if(n > 0) {
                buffer_size_set(buffer, buffer_size_get(buffer) + n);

                descriptor->status = descriptor->status | descriptor_state_read;
            } else if(n == 0) {
                descriptor->status = descriptor->status & (~descriptor_state_read);
#ifndef   RELEASE
                snorlaxdbg(false, true, "descriptor exception", "%d %d %p", descriptor_exception_type_lib, descriptor_exception_no_eof, read);
#endif // RELEASE

                descriptor_exception_set(descriptor, descriptor_exception_type_lib, descriptor_exception_no_eof, read);

                n = fail;
            } else {
                descriptor->status = descriptor->status & (~descriptor_state_read);
                if(errno == EAGAIN) {
                    n = 0;
                } else {
#ifndef   RELEASE
                    snorlaxdbg(false, true, "descriptor exception", "%d %d %p", descriptor_exception_type_system, errno, read);
#endif // RELEASE
                    descriptor_exception_set(descriptor, descriptor_exception_type_system, errno, read);
                }
            }

            return n;
        }
#ifndef   RELEASE
        snorlaxdbg(false, true, "caution", "(descriptor->status & descriptor_state_open_in) == 0");
#endif // RELEASE

        return success;
    } else {
#ifndef   RELEASE
        snorlaxdbg(false, true, "warning", "descriptor is not open");
#endif // RELEASE
    }
    return fail;
}

extern int64_t descriptor_func_write(___notnull descriptor_t * descriptor) {
#ifndef   RELEASE
    snorlaxdbg(descriptor == nil, false, "critical", "");
#endif // RELEASE
    if(descriptor->value > invalid) {
        if(descriptor->status & descriptor_state_open_out) {
            buffer_t * buffer = descriptor->buffer.out;

            if(buffer_length(buffer) > 0) {
                int64_t n = write(descriptor->value, buffer_front(buffer), buffer_length(buffer));

                if(n > 0) {
                    buffer_position_set(buffer, buffer_position_get(buffer) + n);

                    descriptor->status = descriptor->status | descriptor_state_write;
                } else if(n == 0) {
#ifndef   RELEASE
                    snorlaxdbg(false, true, "notice", "check");
#endif // RELEASE
                } else {
                    descriptor->status = descriptor->status & (~descriptor_state_write);

                    if(errno == EAGAIN) {
                        n = 0;
                    } else {
#ifndef   RELEASE
                        snorlaxdbg(false, true, "descriptor exception", "%d %d %p", descriptor_exception_type_system, errno, write);
#endif // RELEASE
                        descriptor_exception_set(descriptor, descriptor_exception_type_system, errno, write);
                    }
                }

                return n;
            }
#ifndef   RELEASE
            snorlaxdbg(false, true, "warning", "(descriptor->status & descriptor_state_open_out) == 0");
#endif // RELEASE
        }
        

        return success;
    } else {
#ifndef   RELEASE
        snorlaxdbg(false, true, "warning", "descriptor is not open");
#endif // RELEASE
    }
    return fail;
}

extern int32_t descriptor_func_close(___notnull descriptor_t * descriptor) {
#ifndef   RELEASE
    snorlaxdbg(descriptor == nil, false, "critical", "");
#endif // RELEASE

    if(descriptor->value > invalid) {
        if(close(descriptor->value) == fail) {
#ifndef   RELEASE
            snorlaxdbg(false, true, "notice", "fail to close => %d", errno);
#endif // RELEASE
        }
        descriptor->value = invalid;
    }

    descriptor->status = descriptor->status & (~(descriptor_state_open | descriptor_state_read | descriptor_state_write));
    descriptor->status = descriptor->status | descriptor_state_close;

#ifndef   RELEASE
    snorlaxdbg(false, true, "descriptor exception", "%d %d %p", descriptor_exception_type_none, descriptor_exception_no_none, nil);
#endif // RELEASE

    descriptor_exception_set(descriptor, descriptor_exception_type_none, descriptor_exception_no_none, nil);

    return success;
}

static descriptor_t * descriptor_func_rem(___notnull descriptor_t * descriptor) {
#ifndef   RELEASE
    snorlaxdbg(descriptor == nil, false, "critical", "");
#endif // RELEASE

    descriptor->buffer.in = buffer_rem(descriptor->buffer.in);
    descriptor->buffer.out = buffer_rem(descriptor->buffer.out);

    descriptor->sync = sync_rem(descriptor->sync);

    free(descriptor);

    return nil;
}

extern int32_t descriptor_func_check(___notnull descriptor_t * descriptor, uint32_t state) {
#ifndef   RELEASE
    snorlaxdbg(descriptor == nil, false, "critical", "");
#endif // RELEASE
    // TODO: UPGRADE THIS
    // 1. NONBLOCK OPEN
    return (descriptor->status & state);
}

extern void descriptor_nonblock_on(___notnull descriptor_t * descriptor) {
#ifndef   RELEASE
    snorlaxdbg(descriptor == nil, false, "critical", "");
#endif // RELEASE

    fcntl(descriptor->value, F_SETFL, fcntl(descriptor->value, F_GETFL, 0) | O_NONBLOCK);
}