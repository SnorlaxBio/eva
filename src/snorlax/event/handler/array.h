/**
 * @file        snorlax/event/handler/array.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 15, 2024
 */

#ifndef   __SNORLAX__EVENT_HANDLER_ARRAY__H__
#define   __SNORLAX__EVENT_HANDLER_ARRAY__H__

#include <snorlax/eva.h>

struct event_handler_array {
    uint32_t max;
    event_handler_t * func;
};

extern event_handler_array_t * event_handler_array_gen(uint32_t max);
extern event_handler_array_t * event_handler_array_rem(event_handler_array_t * collection);

#define event_handler_array_get(collection, index)              (index < collection->max ? collection->func[index] : nil)
#define event_handler_array_set(collection, index, handler)     (collection->func[index] = handler)

#endif // __SNORLAX__EVENT_HANDLER_ARRAY__H__
