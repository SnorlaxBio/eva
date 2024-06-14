/**
 * @file        snorlax/event/handler/array.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 13, 2024
 */

#include <stdlib.h>

#include "array.h"

extern event_handler_array_t * event_handler_array_gen(void) {
    event_handler_array_t * collection = (event_handler_array_t *) calloc(1, sizeof(event_handler_array_t));

    return collection;
}

extern event_handler_array_t * event_handler_array_rem(event_handler_array_t * collection) {
    free(collection);
    if(collection->func) free(collection->func);
    return nil;
}

extern void event_handler_array_set(event_handler_array_t * collection, uint64_t index, event_handler_t handler) {
    if(collection->size <= index) {
        collection->func = (event_handler_t *) (collection->func ? realloc(collection->func, sizeof(event_handler_t) * (index + 1)) : malloc(sizeof(event_handler_t) * (index + 1)));
    }
    collection->func[index] = handler;
}
