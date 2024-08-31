/**
 * @file        snorlax/console/out.c
 * @brief
 * @details
 * 
 * @author      snorlax <opuntia@snorlax.bio>
 * @since       June 27, 2024
 */

#include <stdio.h>
#include <unistd.h>

#include <snorlax/buffer/mem.h>

#include "out.h"

static console_out_t * consoleout = nil;

static console_out_t * console_out_func_rem(___notnull console_out_t * o);
static int32_t console_out_func_open(___notnull console_out_t * o);
static int32_t console_out_func_close(___notnull console_out_t * o);

typedef int64_t (*console_out_read_t)(console_out_t *);
typedef int64_t (*console_out_write_t)(console_out_t *);
typedef int32_t (*console_out_check_t)(___notnull console_out_t *, uint32_t);

static console_out_func_t func = {
    console_out_func_rem,
    console_out_func_open,
    (console_out_read_t) descriptor_func_read,
    (console_out_write_t) descriptor_func_write,
    console_out_func_close,
    (console_out_check_t) descriptor_func_check
};

extern console_out_t * console_out_gen(void) {
    if(consoleout == nil) {
        consoleout = (console_out_t *) calloc(1, sizeof(console_out_t));

        consoleout->func = address_of(func);
        
        consoleout->buffer.in = (buffer_t *) buffer_mem_gen(0);
        consoleout->buffer.out = (buffer_t *) buffer_mem_gen(0);

        consoleout->value = STDOUT_FILENO;
        descriptor_nonblock_on((descriptor_t *)consoleout);
        consoleout->status = descriptor_state_open_out | descriptor_state_write;
    }
    return consoleout;
}

static int32_t console_out_func_open(___notnull console_out_t * o) {
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

static int32_t console_out_func_close(___notnull console_out_t * o) {
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

static console_out_t * console_out_func_rem(___notnull console_out_t * o) {
#ifndef   RELEASE
    snorlaxdbg(o == nil, false, "critical", "");
#endif // RELEASE
    o->buffer.in = buffer_rem(o->buffer.in);
    o->buffer.out = buffer_rem(o->buffer.out);

    o->sync = sync_rem(o->sync);

    free(o);

    consoleout = nil;

    return nil;
}
