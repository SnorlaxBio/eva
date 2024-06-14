/**
 * @file        snorlax/event/handler/array.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 13, 2024
 */

#ifndef   __SNORLAX__EVENT_HANDLER_ARRAY__H__
#define   __SNORLAX__EVENT_HANDLER_ARRAY__H__

#include <snorlax/event/handler.h>

struct event_handler_array;

typedef struct event_handler_array event_handler_array_t;

struct event_handler_array {
    uint32_t size;
    event_handler_t * func;
};

extern event_handler_array_t * event_handler_array_gen(void);
extern event_handler_array_t * event_handler_array_rem(event_handler_array_t * collection);

extern void event_handler_array_set(event_handler_array_t * collection, uint64_t index, event_handler_t handler);

#define event_handler_array_get(collection, index)      (index < collection->size ? collection->func[index] : nil)


#endif // __SNORLAX__EVENT_HANDLER_ARRAY__H__