/**
 * @file        snorlax/event/handler/array.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 15, 2024
 */

#include "array.h"

extern event_handler_array_t * event_handler_array_gen(uint32_t max){
    event_handler_array_t * collection = (event_handler_array_t *) calloc(1, sizeof(event_handler_array_t));

    collection->handler = calloc(max, sizeof(event_handler_t));
    collection->max = max;

    return collection;
}

extern event_handler_array_t * event_handler_array_rem(event_handler_array_t * collection) {
    free(collection->handler);
    free(collection);
    
    return nil;
}