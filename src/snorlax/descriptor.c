/**
 * @file        snorlax/descriptor.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 19, 2024
 */

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "descriptor.h"

static descriptor_func_t func = {
    descriptor_func_rem,
    descriptor_func_open,
    descriptor_func_read,
    descriptor_func_write,
    descriptor_func_close
};

extern descriptor_t * descriptor_gen(int32_t value) {
    descriptor_t * descriptor = (descriptor_t *) calloc(1, sizeof(descriptor_t));

    descriptor->func = &func;

    descriptor->buffer.in = buffer_gen(0);
    descriptor->buffer.out = buffer_gen(0);
    descriptor->value = value;

    return descriptor;
}

extern descriptor_t * descriptor_func_rem(___notnull descriptor_t * descriptor) {
    // TODO: DESCRIPTOR VALUE > INVALID
    descriptor->buffer.in = buffer_rem(descriptor->buffer.in);
    descriptor->buffer.out = buffer_rem(descriptor->buffer.out);
    descriptor->sync = sync_rem(descriptor->sync);

    free(descriptor);

    return nil;
}

extern int32_t descriptor_func_open(___notnull descriptor_t * descriptor) {
    return descriptor->value > invalid ? success : fail;
}

/**
 * 
 * @todo        read 를 수행한 후에 버퍼의 사이즈를 줄일 수 있을까?
 *              차후를 위해서 지금 줄이면 안되고 이것을 호출하는 곳에서
 *              줄여야 한다.
 */
extern int64_t descriptor_func_read(___notnull descriptor_t * descriptor) {
    int32_t fd = descriptor->value;
    if(fd > invalid) {
        buffer_t * buffer = descriptor->buffer.in;

        buffer_capacity_set(buffer, buffer_capacity_get(buffer) + 8192);

        int64_t n = read(fd, buffer_back(buffer), buffer_remain(buffer));

        if(n > 0) {
            buffer_size_set(buffer, buffer_size_get(buffer) + n);
        } else if(n == 0) {
            descriptor_exception_set(address_of(descriptor->exception), descriptor_action_read, descriptor_exception_type_eof);

            n = fail;
        } else {
            if(errno == EAGAIN) {
                descriptor->status = (descriptor->status | (~descriptor_state_in));
                n = 0;
            } else {
                descriptor_exception_set(address_of(descriptor->exception), descriptor_action_system_read, errno);
            }
        }

        return n;
    }
    descriptor_exception_set(address_of(descriptor->exception), descriptor_action_read, descriptor_exception_type_closed);
    return fail;
}

extern int64_t descriptor_func_write(___notnull descriptor_t * descriptor) {
    int32_t value = descriptor->value;

    if(value > invalid) {
        buffer_t * buffer = descriptor->buffer.out;
        if(buffer_length(buffer) > 0) {
            int64_t n = write(value, buffer_front(buffer), buffer_length(buffer));

            if(n > 0) {
                buffer_position_set(buffer, buffer_position_get(buffer) + n);
                if(buffer_length(buffer) == 0) buffer_reset(buffer, 0);
            } else if(n == 0) {
                // TODO: CHECK THIS
            } else {
                if(errno == EAGAIN) {
                    n == 0;
                } else {
                    descriptor_exception_set(address_of(descriptor->exception), descriptor_action_system_write, errno);
                }
            }
        }
        return success;
    }
    descriptor_exception_set(address_of(descriptor->exception), descriptor_action_write, descriptor_exception_type_closed);
    return fail;
}

/**
 * @fn          int32_t descriptor_func_close(descriptor_t * descriptor)
 * @brief       디스크립터를 닫는다.
 * @details     
 */
extern int32_t descriptor_func_close(___notnull descriptor_t * descriptor) {
    if(descriptor->value > invalid) {
        int ret = close(descriptor->value);

        if(ret < 0) descriptor_exception_set(address_of(descriptor->exception), descriptor_action_system_close, errno);

        descriptor->value = invalid;
    }
    return success;
}
