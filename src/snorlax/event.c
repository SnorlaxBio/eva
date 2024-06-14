/**
 * @file        snorlax/event.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 12, 2024
 */

#include "event.h"

#include "event/object/queue.h"

static event_func_t func = {

};

extern event_t * event_gen(event_object_t * o, event_param_t * param, event_handler_t on) {
    event_t * e = (event_t *) calloc(1, sizeof(event_t));

    e->func = &func;
    e->param = param;
    e->on = on;

    event_object_queue_add(o, e);

    return e;
}