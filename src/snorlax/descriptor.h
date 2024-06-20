/**
 * @file        snorlax/descriptor.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 20, 2024
 */

#ifndef   __SNORLAX__DESCRIPTOR__H__
#define   __SNORLAX__DESCRIPTOR__H__

#include <snorlax.h>
#include <snorlax/buffer.h>
#include <snorlax/eva.h>

struct descriptor_event;
struct descriptor_event_func;
struct descriptor_event_subscription;
struct descriptor_event_subscription_func;
struct descriptor_event_generator;
struct descriptor_event_generator_func;

typedef struct descriptor_event descriptor_event_t;
typedef struct descriptor_event_func descriptor_event_func_t;
typedef struct descriptor_event_subscription descriptor_event_subscription_t;
typedef struct descriptor_event_subscription_func descriptor_event_subscription_func_t;
typedef struct descriptor_event_generator descriptor_event_generator_t;
typedef struct descriptor_event_generator_func descriptor_event_generator_func_t;

typedef void (*descriptor_event_subscription_action_t)(___notnull descriptor_event_subscription_t *, uint64_t);
typedef void (*descriptor_event_subscription_handler_t)(___notnull descriptor_event_subscription_t *, uint32_t, uint64_t);

typedef void (*descriptor_callback_t)(___notnull descriptor_t *);
typedef void (*descriptor_close_callback_t)(___notnull descriptor_t *, int32_t);

struct descriptor_buffer {
    buffer_t * in;
    buffer_t * out;
};

struct descriptor_exception {
    uint32_t type;
    address_t func;
    uint32_t no;
};

struct descriptor {
    descriptor_func_t * func;
    sync_t * sync;

    int32_t value;
    uint32_t status;
    descriptor_buffer_t buffer;
    descriptor_exception_t exception;
};

struct descriptor_func {
    ___notsynchronized descriptor_t * (*rem)(___notnull descriptor_t *);

    ___notsynchronized int32_t (*open)(___notnull descriptor_t *);
    ___notsynchronized int64_t (*read)(___notnull descriptor_t *);
    ___notsynchronized int64_t (*write)(___notnull descriptor_t *);
    ___notsynchronized int32_t (*close)(___notnull descriptor_t *, descriptor_close_callback_t);
    ___notsynchronized int32_t (*exception)(___notnull descriptor_t *, uint32_t, address_t, uint32_t);
};

#define descriptor_rem(descriptor)                              ((descriptor)->func->rem(descriptor))
#define descriptor_open(descriptor)                             ((descriptor)->func->open(descriptor))
#define descriptor_read(descriptor)                             ((descriptor)->func->read(descriptor))
#define descriprot_write(descriptor)                            ((descriptor)->func->write(descriptor))
#define descriptor_close(descriptor, callback)                  ((descriptor)->func->close(descriptor, callback))
#define descriptor_exception(descriptor, type, f, no)           ((descriptor)->func->exception(descriptor, type, f, no))

___notsynchronized extern descriptor_t * descriptor_func_rem(___notnull descriptor_t * descriptor);

___notsynchronized extern int32_t descriptor_func_open(___notnull descriptor_t * descriptor);
___notsynchronized extern int64_t descriptor_func_read(___notnull descriptor_t * descriptor);
___notsynchronized extern int64_t descriptor_func_write(___notnull descriptor_t * descriptor);
___notsynchronized extern int32_t descriptor_func_close(___notnull descriptor_t * descriptor, descriptor_close_callback_t callback);
___notsynchronized extern int32_t descriptor_func_exception(___notnull descriptor_t * descriptor, uint32_t type, address_t func, uint32_t no);

___notsynchronized extern descriptor_t * descriptor_gen(int32_t value);

#endif // __SNORLAX__DESCRIPTOR__H__
