/**
 * @file        snorlax/event/generator/set.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 18, 2024
 */

#include "set.h"

#include "../generator.h"

extern int32_t event_generator_set_func_pub(event_generator_set_t * set, event_queue_t * queue) {
    int32_t n = 0;
    n += event_generator_pub(set->command, queue);
    n += event_generator_pub(set->descriptor, queue);
    n += event_generator_pub(set->time, queue);
    n += event_generator_pub(set->signal, queue);
    n += event_generator_pub(set->user, queue);

    return n;
}