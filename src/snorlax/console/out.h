/**
 * @file        snorlax/console/out.h
 * @brief
 * @details
 * 
 * @author      snorlax <opuntia@snorlax.bio>
 * @since       June 26, 2024
 */

#ifndef   __SNORLAX__CONSOLE_OUT__H__
#define   __SNORLAX__CONSOLE_OUT__H__

#include <snorlax/descriptor.h>

struct console_out;
struct console_out_func;

typedef struct console_out console_out_t;
typedef struct console_out_func console_out_func_t;

struct console_out {
    console_out_func_t * func;
    sync_t * sync;
    ___reference event_object_meta_t * meta;
    descriptor_buffer_t buffer;
    descriptor_exception_t exception;
    int32_t value;
    uint32_t status;
};

struct console_out_func {
    console_out_t * (*rem)(___notnull console_out_t *);

    int32_t (*open)(___notnull console_out_t *);
    int64_t (*read)(___notnull console_out_t *);
    int64_t (*write)(___notnull console_out_t *);
    int32_t (*close)(___notnull console_out_t *);
    int32_t (*check)(___notnull console_out_t *, uint32_t);
};

extern console_out_t * console_out_gen(void);

#define console_out_rem(descriptor)             ((descriptor)->func->rem(descriptor))
#define console_out_open(descriptor)            ((descriptor)->func->open(descriptor))
#define console_out_read(descriptor)            ((descriptor)->func->read(descriptor))
#define console_out_write(descriptor)           ((descriptor)->func->write(descriptor))
#define console_out_close(descriptor)           ((descriptor)->func->close(descriptor))
#define console_out_check(descriptor, state)    ((descriptor)->func->check(descriptor, state))

#endif // __SNORLAX__CONSOLE_OUT__H__


