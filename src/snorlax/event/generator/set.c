/**
 * @file        snorlax/event/generator/set.c
 * @brief
 * @details
 * 
 * @author      snorlax <opuntia@snorlax.bio>
 * @since       June 21, 2024
 */

#include <stdlib.h>

#include "set.h"

#include "../engine.h"
#include "../generator.h"
#include "../processor/pool.h"

#include "../../command/event/generator.h"
#include "../../descriptor/event/generator.h"

static ___sync event_generator_set_t * event_generator_set_func_rem(___notnull event_generator_set_t * set);
static ___sync int32_t event_generator_set_func_on(___notnull event_generator_set_t * set);
static ___sync int32_t event_generator_set_func_off(___notnull event_generator_set_t * set);
static ___sync int32_t event_generator_set_func_pub(___notnull event_generator_set_t * set, event_queue_t * queue);

static event_generator_set_func_t func = {
    event_generator_set_func_rem,
    event_generator_set_func_on,
    event_generator_set_func_off,
    event_generator_set_func_pub
};

extern event_generator_set_t * event_generator_set_gen(___notnull event_engine_t * engine) {
#ifndef   RELEASE
    snorlaxdbg(engine == nil, false, "critical", "");
#endif // RELEASE

    event_generator_set_t * set = (event_generator_set_t *) calloc(1, sizeof(event_generator_set_t));

    set->func = address_of(func);
    set->engine = engine;

    if(engine->pool->size > 0) set->sync = sync_gen();

    set->command = (event_generator_t *) command_event_generator_gen(set);
    set->descriptor = (event_generator_t *) descriptor_event_generator_gen(set);

    return set;
}

static ___sync event_generator_set_t * event_generator_set_func_rem(___notnull event_generator_set_t * set) {
#ifndef   RELEASE
    snorlaxdbg(set == nil, false, "critical", "");
#endif // RELEASE

    object_lock(set);
    if(set->command) set->command = event_generator_rem(set->command);
    if(set->descriptor) set->descriptor = event_generator_rem(set->descriptor);
    if(set->time) set->time = event_generator_rem(set->time);
    if(set->signal) set->signal = event_generator_rem(set->signal);
    object_unlock(set);

    set->sync = sync_rem(set->sync);

    free(set);

    return nil;
}

static ___sync int32_t event_generator_set_func_on(___notnull event_generator_set_t * set) {
#ifndef   RELEASE
    snorlaxdbg(set == nil, false, "critical", "");
#endif // RELEASE

    object_lock(set);
    if(set->command) event_generator_on(set->command);
    if(set->descriptor) event_generator_on(set->descriptor);
    if(set->time) event_generator_on(set->time);
    if(set->signal) event_generator_on(set->signal);
    object_unlock(set);

    return success;
}

static ___sync int32_t event_generator_set_func_off(___notnull event_generator_set_t * set) {
#ifndef   RELEASE
    snorlaxdbg(set == nil, false, "critical", "");
#endif // RELEASE

    object_lock(set);
    if(set->command) event_generator_off(set->command);
    if(set->descriptor) event_generator_off(set->descriptor);
    if(set->time) event_generator_off(set->time);
    if(set->signal) event_generator_off(set->signal);
    object_unlock(set);

    return success;
}

static ___sync int32_t event_generator_set_func_pub(___notnull event_generator_set_t * set, event_queue_t * queue) {
#ifndef   RELEASE
    snorlaxdbg(set == nil, false, "critical", "");
#endif // RELEASE

    int32_t n = 0;

    object_lock(set);
    if(set->command) n += event_generator_pub(set->command, queue);
    object_unlock(set);

    object_lock(set);
    if(set->descriptor) n += event_generator_pub(set->descriptor, queue);
    object_unlock(set);

    object_lock(set);
    if(set->time) n += event_generator_pub(set->time, queue);
    object_unlock(set);

    object_lock(set);
    if(set->signal) n += event_generator_pub(set->signal, queue);
    object_unlock(set);

    return n;
}
