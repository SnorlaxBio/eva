/**
 * @file        snorlax/event/object.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 14, 2024
 */

#ifndef   __SNORLAX__EVENT_OBJECT__H__
#define   __SNORLAX__EVENT_OBJECT__H__

#include <snorlax/eva.h>

struct event_object_func;

typedef struct event_object_func event_object_func_t;

struct event_object {
    event_object_func_t * func;
    event_engine_t * engine;
    event_object_t * prev;
    event_object_t * next;

    uint32_t status;
    event_handler_array_t * handler;        // 이벤트 타입은 얼마나 될까?
};

struct event_object_func {
    event_object_t * (*rem)(event_object_t *);
};

#endif // __SNORLAX__EVENT_OBJECT__H__
