/**
 * @file        snorlax/event/object/queue.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 13, 2024
 */

#include "queue.h"

#include "../event.h"
#include "../event/object.h"

extern event_t * event_object_queue_add(event_object_t * o, event_t * e) {
    // TODO: ASSERTION
    e->object.o = o;

    if(o->queue.tail) {
        o->queue.tail->object.next = e;
        e->object.prev = o->queue.tail;
    } else {
        o->queue.head = e;
    }

    o->queue.tail = e;
    o->queue.size = o->queue.size + 1;

    return nil;
}

extern event_t * event_object_queue_del(event_object_t * o, event_t * e) {

    event_t * prev = e->object.prev;
    event_t * next = e->object.next;

    if(prev) {
        prev->object.next = next;
    } else {
        o->queue.head = next;
    }

    if(next) {
        next->object.prev = prev;
    } else {
        o->queue.tail = prev;
    }

    e->object.prev = nil;
    e->object.next = nil;
    e->object.o = nil;

    return e;
}
