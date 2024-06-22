/**
 * @file        snorlax/descriptor.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 22, 2024
 */

#ifndef   __SNORLAX__DESCRIPTOR__H__
#define   __SNORLAX__DESCRIPTOR__H__

#include <snorlax.h>
#include <snorlax/buffer.h>

#define descriptor_state_open           (0x00000001U <<  0U)
#define descriptor_state_read           (0x00000001U <<  1U)
#define descriptor_state_write          (0x00000001U <<  2U)
#define descriptor_state_close          (0x00000001U <<  3U)
#define descriptor_state_exception      (0x00000001U <<  4U)

#define descriptor_exception_type_system    1
#define descriptor_exception_type_lib       2

#define descriptor_exception_no_eof         1


struct descriptor;
struct descriptor_func;

struct descriptor_buffer;
struct descriptor_exception;

typedef struct descriptor descriptor_t;
typedef struct descriptor_func descriptor_func_t;

typedef struct descriptor_buffer descriptor_buffer_t;
typedef struct descriptor_exception descriptor_exception_t;

struct descriptor_buffer {
    buffer_t * in;
    buffer_t * out;
};

struct descriptor_exception {
    int32_t type;
    int32_t no;
    void * func;
};

extern descriptor_exception_t * descriptor_exception_func_set(___notnull descriptor_t * descriptor, int32_t type, int32_t no, void * f);
extern descriptor_exception_t * descriptor_exception_func_get(___notnull descriptor_t * descriptor);

#define descriptor_exception_set(descriptor, type, no, f)    descriptor_exception_func_set(descriptor, type, no, f)
#define descriptor_exception_get(descriptor)                 descriptor_exception_func_get(descriptor)

struct descriptor {
    descriptor_func_t * func;
    sync_t * sync;
    descriptor_buffer_t buffer;
    descriptor_exception_t exception;
    int32_t value;
    uint32_t status;
};

struct descriptor_func {
    descriptor_t * (*rem)(___notnull descriptor_t *);

    int32_t * (*open)(___notnull descriptor_t *);
    int64_t * (*read)(___notnull descriptor_t *);
    int64_t * (*write)(___notnull descriptor_t *);
    int32_t * (*close)(___notnull descriptor_t *);
};

extern descriptor_t * descriptor_gen(int32_t value);

extern int32_t descriptor_func_open(___notnull descriptor_t * descriptor);
extern int32_t descriptor_func_read(___notnull descriptor_t * descriptor);
extern int32_t descriptor_func_write(___notnull descriptor_t * descriptor);
extern int32_t descriptor_func_close(___notnull descriptor_t * descriptor);

#define descriptor_rem(descriptor)      ((descriptor)->func->rem(descriptor))
#define descriptor_open(descriptor)     ((descriptor)->func->open(descriptor))
#define descriptor_read(descriptor)     ((descriptor)->func->read(descriptor))
#define descriptor_write(descriptor)    ((descriptor)->func->write(descriptor))
#define descriptor_close(descriptor)    ((descriptor)->func->close(descriptor))

#endif // __SNORLAX__DESCRIPTOR__H__
