/**
 * @file        snorlax/descriptor.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 13, 2024
 */

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "descriptor.h"

static descriptor_func_t func = {
    descriptor_func_rem,
    descriptor_func_open,
    descriptor_func_close,
    descriptor_func_read,
    descriptor_func_write
};

extern descriptor_t * descriptor_gen(int value) {
    descriptor_t * o = (descriptor_t *) calloc(1, sizeof(descriptor_t));

    o->func = &func;
    o->handler = event_handler_array_gen();
    o->value = value;
    o->buffer.in = buffer_gen();
    o->buffer.out = buffer_gen();

    return o;
}

extern descriptor_t * descriptor_func_rem(descriptor_t * o) {
    event_engine_unreg(o->engine, o);

    descriptor_func_close(o);

    o->buffer.in = buffer_rem(o->buffer.in);
    o->buffer.out = buffer_rem(o->buffer.out);

    free(o);

    return nil;
}
extern int descriptor_func_open(descriptor_t * o) {
    return o->value > invalid ? success : fail;
}

extern int descriptor_func_close(descriptor_t * o) {
    if(o->value > invalid) {
        close(o->value);
        o->value = invalid;
    }
    return success;
}

extern int64_t descriptor_func_read(descriptor_t * o) {
    if(o->value > invalid) {
        buffer_reserve(o->buffer.in, 8192);
        int64_t n = read(o->value, buffer_back(o->buffer.in), buffer_remain(o->buffer.in));
        if(n > 0) {
            o->buffer.in->size = o->buffer.in->size + n;
        } else if(n == 0) {
            // TODO: CHECK THIS
        } else {
            if(errno == EAGAIN) {
                n = 0;
            } else {
                // TODO: CHECK THIS
            }
        }
        return n;
    }
    return fail;
}

extern int64_t descriptor_func_write(descriptor_t * o) {
    if(o->value > invalid) {
        if(buffer_len(o->buffer.out)) {
            int64_t n = write(o->value, buffer_front(o->buffer.out), buffer_len(o->buffer.out));
            if(n > 0) {
                o->buffer.out->position = o->buffer.out->position + n;
                if(buffer_len(o->buffer.out) == 0) {
                    buffer_adjust(o->buffer.out, 0);
                }
            } else if(n == 0) {
                // TODO: CHECK THIS
            } else {
                if(errno == EAGAIN) {
                    n = 0;
                } else {
                    // TODO: CHECK THIS
                }
            }
        }
        return 0lu;
    }
    return fail;
}