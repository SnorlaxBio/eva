/**
 * @file        snorlax/event/generator/set.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 18, 2024
 */

#include <stdlib.h>

#include "set.h"

#include "../generator.h"

static event_generator_set_t * event_generator_set_func_rem(event_generator_set_t * set);

static event_generator_set_func_t func = {
    event_generator_set_func_rem
};

extern event_generator_set_t * event_generator_set_gen(void) {
    event_generator_set_t * set = (event_generator_set_t *) calloc(1, sizeof(event_generator_set_t));

    set->func = &func;

    return set;
}

extern int32_t event_generator_set_func_pub(event_generator_set_t * set, event_queue_t * queue) {
    int32_t n = 0;
    n += event_generator_pub(set->command, queue);
    n += event_generator_pub(set->descriptor, queue);
    n += event_generator_pub(set->time, queue);
    n += event_generator_pub(set->signal, queue);
    n += event_generator_pub(set->user, queue);

    return n;
}

static event_generator_set_t * event_generator_set_func_rem(event_generator_set_t * set) {
    object_lock(set);

    set->command = event_generator_rem(set->command);
    set->descriptor = event_generator_rem(set->descriptor);
    set->time = event_generator_rem(set->time);
    set->signal = event_generator_rem(set->signal);
    set->user = event_generator_rem(set->user);

    object_unlock(set);

    set->sync = sync_rem(set->sync);

    return nil;
}