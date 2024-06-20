/**
 * @file            snorlax/descriptor.c
 * @brief
 * @details
 * 
 * @author          snorlax <ceo@snorlax.bio>
 * @since           June 20, 2024
 */

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "descriptor.h"

#include "descriptor/state.h"
#include "descriptor/exception/no.h"
#include "descriptor/exception/type.h"

static descriptor_func_t func = {
    descriptor_func_rem,
    descriptor_func_open,
    descriptor_func_read,
    descriptor_func_write,
    descriptor_func_close,
    descriptor_func_exception
};

___notsynchronized extern descriptor_t * descriptor_gen(int32_t value) {
    descriptor_t * descriptor = (descriptor_t *) calloc(1, sizeof(descriptor_t));

    descriptor->func = &func;

    descriptor->value = value;
    descriptor->status = descriptor_state_gen;

    if(descriptor->value > invalid) {
        descriptor_state_add(descriptor, descriptor_state_open);
    } else {
        descriptor_state_add(descriptor, descriptor_state_close);
    }

    descriptor->buffer.in = buffer_gen(0);
    descriptor->buffer.out = buffer_gen(0);

    return descriptor;
}

___notsynchronized extern descriptor_t * descriptor_func_rem(___notnull descriptor_t * descriptor) {
#ifndef   RELEASE
    snorlaxdbg(descriptor == nil, "critical", "");
    snorlaxdbg(descriptor->value > invalid, "warning", "");
#endif // RELEASE

    descriptor->buffer.in = buffer_rem(descriptor->buffer.in);
    descriptor->buffer.out = buffer_rem(descriptor->buffer.out);
    descriptor->sync = sync_rem(descriptor->sync);

    free(descriptor);

    return nil;
}

___notsynchronized extern int32_t descriptor_func_open(___notnull descriptor_t * descriptor) {
#ifndef   RELEASE
    snorlaxdbg(descriptor == nil, "critical", "");
    snorlaxdbg(!descriptor_state_is(descriptor, descriptor_state_gen), "critical", "");
    snorlaxdbg(descriptor_state_is(descriptor, descriptor_state_exception), "critical", "");
#endif // RELEASE

    if(descriptor->value > invalid) {
        if(!descriptor_state_is(descriptor, descriptor_state_open)){
            if(descriptor_state_is(descriptor, descriptor_state_close)) {
                descriptor_state_del(descriptor, descriptor_state_close);
            }

            descriptor_state_add(descriptor, descriptor_state_open);

            buffer_reset(descriptor->buffer.in, 0);
            buffer_reset(descriptor->buffer.out, 0);
        } else {
#ifndef   RELEASE
            snorlaxdbg(false, "warning", "already open");

            if(descriptor_state_is(descriptor, descriptor_state_close)){
                snorlaxdbg(false, "warning", "descriptor will be closed");
            }
#endif // RELEASE
        }
    }

    return (descriptor->value <= invalid || descriptor_state_is(descriptor, descriptor_state_close)) ? fail : success;
}

___notsynchronized extern int64_t descriptor_func_read(___notnull descriptor_t * descriptor) {
#ifndef   RELEASE
    snorlaxdbg(descriptor == nil, "critical", "");
    snorlaxdbg(descriptor->value <= invalid, "critical", "");
    snorlaxdbg(descriptor->buffer.in == nil, "critical", "");
    snorlaxdbg(!descriptor_state_is(descriptor, descriptor_state_gen), "critical", "");
    snorlaxdbg(!descriptor_state_is(descriptor, descriptor_state_open), "critical", "");
    snorlaxdbg(descriptor_state_is(descriptor, descriptor_state_exception), "critical", "");
    snorlaxdbg(descriptor_state_is(descriptor, descriptor_state_close), "critical", "");
#endif // RELEASE
    buffer_t * buffer = descriptor->buffer.in;
    buffer_capacity_set(buffer, buffer_capacity_get(buffer) + 8192);
    int64_t n = read(descriptor->value, buffer_back(buffer), buffer_remain(buffer));
    if(n > 0) {
        buffer_size_set(buffer, buffer_size_get(buffer) + n);
    } else if(n == 0) {
        descriptor_exception(descriptor, descriptor_exception_type_lib, descriptor_func_read, descriptor_exception_no_eof);

        n = fail;
    } else {
        if(errno == EAGAIN) {
            descriptor_state_del(descriptor, descriptor_state_in);

            n = 0;
        } else {
            descriptor_exception(descriptor, descriptor_exception_type_system, read, errno);
        }
    }
    return n;
}

___notsynchronized extern int64_t descriptor_func_write(___notnull descriptor_t * descriptor) {
#ifndef   RELEASE
    snorlaxdbg(descriptor == nil, "critical", "");
    snorlaxdbg(descriptor->value <= invalid, "critical", "");
    snorlaxdbg(descriptor->buffer.out == nil, "critical", "");
    snorlaxdbg(!descriptor_state_is(descriptor, descriptor_state_gen), "critical", "");
    snorlaxdbg(!descriptor_state_is(descriptor, descriptor_state_open), "critical", "");
    snorlaxdbg(descriptor_state_is(descriptor, descriptor_state_exception), "critical", "");
    snorlaxdbg(descriptor_state_is(descriptor, descriptor_state_close), "critical", "");
#endif // RELEASE
    int64_t n = success;
    buffer_t * buffer = descriptor->buffer.out;
    if(buffer_length(buffer) > 0) {
        n = write(descriptor->value, buffer_front(buffer), buffer_length(buffer));
        if(n > 0) {
            buffer_position_set(buffer, buffer_position_get(buffer) + n);
        } else if(n == 0) {
            snorlaxdbg(false, "debug", "check");
        } else {
            if(errno == EAGAIN) {
                descriptor_state_del(descriptor, descriptor_state_out);

                n = 0;
            } else {
                descriptor_exception(descriptor, descriptor_exception_type_system, write, errno);
            }
        }
    }

    return n;
}

___notsynchronized extern int32_t descriptor_func_close(___notnull descriptor_t * descriptor, descriptor_close_callback_t callback) {
#ifndef   RELEASE
    snorlaxdbg(descriptor == nil, "critical", "");
    snorlaxdbg(!descriptor_state_is(descriptor, descriptor_state_gen), "critical", "");
#endif // RELEASE
    if(descriptor->value > invalid) {
        int32_t value = descriptor->value;
        if(close(descriptor->value) == fail) {
            snorlaxdbg(false, "error", "fail to close => %d", errno);
        }

        descriptor->value = invalid;

        descriptor_state_set(descriptor, descriptor_state_close | descriptor_state_gen);

        if(callback) callback(descriptor, value);

        buffer_reset(descriptor->buffer.in, 0);
        buffer_reset(descriptor->buffer.out, 0);
    } else {
#ifndef   RELEASE
        snorlaxdbg(descriptor_state_is(descriptor, descriptor_state_open), "critical", "");
#endif // RELEASE
    }

    return success;
}

___notsynchronized extern int32_t descriptor_func_exception(___notnull descriptor_t * descriptor, uint32_t type, address_t func, uint32_t no) {
#ifndef   RELEASE
    snorlaxdbg(descriptor == nil, "critical", "");
    snorlaxdbg(!descriptor_state_is(descriptor, descriptor_state_gen), "critical", "");
#endif // RELEASE
    descriptor->exception.type = type;

    if(type){
        descriptor_state_add(descriptor, descriptor_state_exception);

        descriptor->exception.func = func;
        descriptor->exception.no = no;
    } else {
        descriptor_state_del(descriptor, descriptor_state_exception);

        descriptor->exception.func = nil;
        descriptor->exception.no = descriptor_exception_no_none;

#ifndef   RELEASE
        snorlaxdbg(func || no, "warning", "");
#endif // RELEASE
    }

    return success;
}