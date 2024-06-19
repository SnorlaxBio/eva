/**
 * @file        snorlax/descriptor.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 18, 2024
 */

#ifndef   __SNORLAX__DESCRIPTOR__H__
#define   __SNORLAX__DESCRIPTOR__H__

#include <snorlax.h>
#include <snorlax/buffer.h>

#define descriptor_action_open                  1
#define descriptor_action_read                  2
#define descriptor_action_write                 3
#define descriptor_action_close                 4

#define descriptor_action_system_open           5
#define descriptor_action_system_read           6
#define descriptor_action_system_write          7
#define descriptor_action_system_close          8

#define descriptor_state_in                     (0x00000001U <<  0U)
#define descriptor_state_out                    (0x00000001U <<  1U)


#define descriptor_exception_type_closed        1
#define descriptor_exception_type_eof           2

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
    uint32_t action;
    uint32_t type;
};

extern void descriptor_exception_func_set(___notnull descriptor_exception_t * exception, uint32_t action, uint32_t type);

#define descriptor_exception_set(exception, category, v) do {   \
    (exception)->action = category;                             \
    (exception)->type = v;                                      \
} while(0)

/**
 * 사용자에게 노출시키지 말자... 다만 상속을 가능하도록 만들자...
 */
struct descriptor {
    descriptor_func_t * func;
    sync_t * sync;

    int32_t value;
    uint32_t status;
    descriptor_buffer_t buffer;
    descriptor_exception_t exception;
};

struct descriptor_func {
    descriptor_t * (*rem)(descriptor_t *);
    int32_t (*open)(descriptor_t *);
    int64_t (*read)(descriptor_t *);
    int64_t (*write)(descriptor_t *);
    int32_t (*close)(descriptor_t *);
};

extern descriptor_t * descriptor_gen(int32_t value);

extern descriptor_t * descriptor_func_rem(___notnull descriptor_t * descriptor);
extern int32_t descriptor_func_open(___notnull descriptor_t * descriptor);
extern int64_t descriptor_func_read(___notnull descriptor_t * descriptor);
extern int64_t descriptor_func_write(___notnull descriptor_t * descriptor);
extern int32_t descriptor_func_close(___notnull descriptor_t * descriptor);

#define descriptor_rem(descriptor)                          ((descriptor)->func->rem(descriptor))
#define descriptor_open(descriptor)                         ((descriptor)->func->open(descriptor))
#define descriptor_read(descriptor)                         ((descriptor)->func->read(descriptor))
#define descriptor_write(descriptor)                        ((descriptor)->func->write(descriptor))
#define descriptor_close(descriptor)                        ((descriptor)->func->close(descriptor))

#endif // __SNORLAX__DESCRIPTOR__H__
