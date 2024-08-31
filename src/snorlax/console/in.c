/**
 * @file        snorlax/console/in.c
 * @brief
 * @details
 * 
 * @author      snorlax <opuntia@snorlax.bio>
 * @since       June 27, 2024
 */

#include <stdio.h>
#include <unistd.h>

#include <snorlax/buffer/mem.h>

#include "in.h"

static console_in_t * consolein = nil;

static console_in_t * console_in_func_rem(___notnull console_in_t * o);
static int32_t console_in_func_open(___notnull console_in_t * o);
static int32_t console_in_func_close(___notnull console_in_t * o);

typedef int64_t (*console_in_read_t)(console_in_t *);
typedef int64_t (*console_in_write_t)(console_in_t *);
typedef int32_t (*console_in_check_t)(___notnull console_in_t *, uint32_t);

static console_in_func_t func = {
    console_in_func_rem,
    console_in_func_open,
    (console_in_read_t) descriptor_func_read,
    (console_in_write_t) descriptor_func_write,
    console_in_func_close,
    (console_in_check_t) descriptor_func_check
};

extern console_in_t * console_in_gen(void) {
    if(consolein == nil) {
        consolein = (console_in_t *) calloc(1, sizeof(console_in_t));

        consolein->func = address_of(func);
        
        consolein->buffer.in = (buffer_t *) buffer_mem_gen(0);
        consolein->buffer.out = (buffer_t *) buffer_mem_gen(0);

        consolein->value = STDIN_FILENO;
        descriptor_nonblock_on((descriptor_t *) consolein);
        consolein->status = descriptor_state_open_in;
    }
    return consolein;
}

static int32_t console_in_func_open(___notnull console_in_t * o) {
#ifndef   RELEASE
    snorlaxdbg(o == nil, false, "critical", "");
#endif // RELEASE

    if(o->status & (descriptor_state_exception & descriptor_state_close)) {
#ifndef   RELEASE
        snorlaxdbg(o->status & (descriptor_state_exception & descriptor_state_close), false, "critical", "");
#endif // RELEASE
    } else {
        o->status = descriptor_state_open;
    }

    return success;
}

static int32_t console_in_func_close(___notnull console_in_t * o) {
#ifndef   RELEASE
    snorlaxdbg(o == nil, false, "critical", "");
#endif // RELEASE

    if(o->status & descriptor_state_close) {
#ifndef   RELEASE
        snorlaxdbg(o->status & (descriptor_state_exception & descriptor_state_close), false, "critical", "");
#endif // RELEASE
    }

    o->status = descriptor_state_close;

    return success;
}

static console_in_t * console_in_func_rem(___notnull console_in_t * o) {
#ifndef   RELEASE
    snorlaxdbg(o == nil, false, "critical", "");
#endif // RELEASE

    o->buffer.in = buffer_rem(o->buffer.in);
    o->buffer.out = buffer_rem(o->buffer.out);

    o->sync = sync_rem(o->sync);

    free(o);

    consolein = nil;

    return nil;
}