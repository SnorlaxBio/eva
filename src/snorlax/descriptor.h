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

#define descriptor_event_subscription_interest_type_none            0
#define descriptor_event_subscription_interest_type_open            1
#define descriptor_event_subscription_interest_type_in              2
#define descriptor_event_subscription_interest_type_out             3
#define descriptor_event_subscription_interest_type_close           4
#define descriptor_event_subscription_interest_type_exception       5
#define descriptor_event_subscription_interest_type_max             6


#define descriptor_event_subscription_interest_in       (0x00000001U <<  0U)
#define descriptor_event_subscription_interest_out      (0x00000001U <<  1U)

#define descriptor_event_subscription_state_generated   (0x00000001U <<  0U)
#define descriptor_event_subscription_state_subscribed  (0x00000001U <<  1U)
#define descriptor_event_subscription_state_registered  (0x00000001U <<  2U)
#define descriptor_event_subscription_state_wait_in     (0x00000001U <<  3U)
#define descriptor_event_subscription_state_wait_out    (0x00000001U <<  4U)

#define descriptor_event_subscription_state_wait        (descriptor_event_subscription_state_wait_in | descriptor_event_subscription_state_wait_out)

#define descriptor_event_subscription_interest_type_none            0
#define descriptor_event_subscription_interest_type_open            1
#define descriptor_event_subscription_interest_type_in              2
#define descriptor_event_subscription_interest_type_out             3
#define descriptor_event_subscription_interest_type_close           4
#define descriptor_event_subscription_interest_type_exception       5
#define descriptor_event_subscription_interest_type_max             6


#define descriptor_action_open                  0
#define descriptor_action_in                    1
#define descriptor_action_out                   2
#define descriptor_action_close                 3
#define descriptor_action_exception             4
#define descriptor_action_max                   5

#define descriptor_action_type_system                5
#define descriptor_action_type_lib                   6
#define descriptor_action_type_user                  7

#define descriptor_action_system_open           5
#define descriptor_action_system_read           6
#define descriptor_action_system_write          7
#define descriptor_action_system_close          8

#define descriptor_action_system_engine         9

#define descriptor_action_event_on              9

#define descriptor_state_gen                    (0x00000001U <<  0U)
#define descriptor_state_open                   (0x00000001U <<  1U)
#define descriptor_state_in                     (0x00000001U <<  2U)
#define descriptor_state_out                    (0x00000001U <<  3U)
#define descriptor_state_close                  (0x00000001U <<  4U)
#define descriptor_state_exception              (0x00000001U <<  5U)

#define descriptor_state_subscription_gen       (0x00000001U <<  6U)
#define descriptor_state_subscription_sub       (0x00000001U <<  7U)
#define descriptor_state_subscription_reg       (0x00000001U <<  8U)
#define descriptor_state_subscription_wait_in   (0x00000001U <<  9U)
#define descriptor_state_subscription_wait_out  (0x00000001U << 10U)

#define descriptor_state_subscription_wait      (descriptor_state_subscription_wait_in | descriptor_state_subscription_wait_out)

#define descriptor_state_subscription_all       (descriptor_state_subscription_gen | descriptor_state_subscription_sub | descriptor_state_subscription_reg | descriptor_state_subscription_wait)




#define descriptor_event_type_open              1
#define descriptor_event_type_in                2
#define descriptor_event_type_out               3
#define descriptor_event_type_close             4
#define descriptor_event_type_exception         5
#define descriptor_event_type_max               6

#define descriptor_exception_type_system                (0x01000000U)
#define descriptor_exception_type_lib                   (0x02000000U)
// LIB & SYSTEM 은 은닉시킨다.
#define descriptor_exception_type_user                  (0x03000000U)

#define descriptor_exception_no_dead                   1
#define descriptor_exception_no_eof                    2
#define descriptor_exception_no_invalid_parameter      3

struct descriptor;
struct descriptor_func;
struct descriptor_buffer;
struct descriptor_exception;

struct descriptor_event;
struct descriptor_event_func;
struct descriptor_event_subscription;
struct descriptor_event_subscription_func;
struct descriptor_event_subscription_event;
struct descriptor_event_subscription_event_func;
struct descriptor_event_generator;
struct descriptor_event_generator_func;

typedef struct descriptor descriptor_t;
typedef struct descriptor_func descriptor_func_t;
typedef struct descriptor_buffer descriptor_buffer_t;
typedef struct descriptor_exception descriptor_exception_t;
typedef struct descriptor_event descriptor_event_t;
typedef struct descriptor_event_func descriptor_event_func_t;
typedef struct descriptor_event_subscription descriptor_event_subscription_t;
typedef struct descriptor_event_subscription_func descriptor_event_subscription_func_t;
typedef struct descriptor_event_subscription_event descriptor_event_subscription_event_t;
typedef struct descriptor_event_subscription_event_func descriptor_event_subscription_event_func_t;
typedef struct descriptor_event_generator descriptor_event_generator_t;
typedef struct descriptor_event_generator_func descriptor_event_generator_func_t;

typedef void (*descriptor_event_handler_t)(descriptor_event_subscription_t *, uint32_t, uint64_t);
typedef int32_t (*descriptor_event_action_t)(descriptor_event_subscription_t *, uint64_t);

typedef void (*descriptor_event_subscription_handler_t)(descriptor_event_subscription_t *, uint32_t, uint64_t);
typedef int32_t (*descriptor_event_subscription_action_t)(descriptor_event_subscription_t *, uint64_t);

struct descriptor_buffer {
    buffer_t * in;
    buffer_t * out;
};

struct descriptor_exception {
    uint32_t type;
    void * func;
    uint32_t no;
};

extern descriptor_exception_t * descriptor_exception_func_set(___notnull descriptor_exception_t * exception, uint32_t type, void * func, uint32_t no);

#define descriptor_exception_set(exception, type, func, no)     descriptor_exception_func_set(exception, type, func, no)


//  do {   \
//     (exception)->type = t;                                  \
//     (exception)->func = f;                                  \
//     (exception)->no = n;                                    \
// } while(0)

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
