/**
 * @file        snorlax/console/in.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 26, 2024
 */

#ifndef   __SNORLAX__CONSOLE_IN__H__
#define   __SNORLAX__CONSOLE_IN__H__

#include <snorlax/descriptor.h>
#include <snorlax/descriptor/event.h>
#include <snorlax/descriptor/event/type.h>

struct console_in;
struct console_in_func;

typedef struct console_in console_in_t;
typedef struct console_in_func console_in_func_t;

struct console_in {
    console_in_func_t * func;
    sync_t * sync;
    descriptor_buffer_t buffer;
    descriptor_exception_t exception;
    int32_t value;
    uint32_t status;
};

struct console_in_func {
    console_in_t * (*rem)(___notnull console_in_t *);

    int32_t (*open)(___notnull console_in_t *);
    int64_t (*read)(___notnull console_in_t *);
    int64_t (*write)(___notnull console_in_t *);
    int32_t (*close)(___notnull console_in_t *);
    int32_t (*check)(___notnull console_in_t *, uint32_t);
};

typedef void (*console_in_event_subscription_handler_t)(___notnull descriptor_event_subscription_t *, uint32_t, event_subscription_event_t *);

extern console_in_t * console_in_gen(void);

#define console_in_rem(descriptor)              ((descriptor)->func->rem(descriptor))
#define console_in_open(descriptor)             ((descriptor)->func->open(descriptor))
#define console_in_read(descriptor)             ((descriptor)->func->read(descriptor))
#define console_in_write(descriptor)            ((descriptor)->func->write(descriptor))
#define console_in_close(descriptor)            ((descriptor)->func->close(descriptor))
#define console_in_check(descriptor, state)     ((descriptor)->func->check(descriptor, state))

#endif // __SNORLAX__CONSOLE_IN__H__


