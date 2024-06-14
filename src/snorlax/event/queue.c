/**
 * @file        snorlax/event/queue.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 12, 2024
 */

#include "queue.h"

#include "../event.h"

// static event_queue_t * event_queue_func_rem(event_queue_t * collection, event_get_t get);
// static uint64_t event_queue_func_size(event_queue_t * collection);
// static void event_queue_func_clear(event_queue_t * collection, event_get_t get);
// static event_t * event_queue_func_push(event_queue_t * collection, event_object_t * object, event_param_t * param);
// static event_t * event_queue_func_pop(event_queue_t * collection);

static event_queue_t * event_queue_func_rem(event_queue_t * queue, event_get_t get);
static void event_queue_func_clear(event_queue_t * queue, event_get_t get);
static uint64_t event_queue_func_size(event_queue_t * queue);
static event_t * event_queue_func_push(event_queue_t * queue, event_t * e);
static event_t * event_queue_func_pop(event_queue_t * e);

static event_queue_func_t func = {
    event_queue_func_rem,
    event_queue_func_clear,
    event_queue_func_size,
    event_queue_func_push,
    event_queue_func_pop
};

extern event_queue_t * event_queue_gen(void) {
    event_queue_t * collection = (event_queue_t *) calloc(1, sizeof(event_queue_t));

    collection->func = &func;

    return collection;
}

static event_queue_t * event_queue_func_rem(event_queue_t * collection , event_get_t get) {
    event_t * e = collection->head;
    while(e) {
        collection->head = collection->head->next;
        e = event_rem(e, get);
        e = collection->head;
    }
    free(collection);
    return nil;
}

static void event_queue_func_clear(event_queue_t * collection, event_get_t get) {
    event_t * e = collection->head;
    while(e) {
        collection->head = collection->head->next;
        e = event_rem(e, get);
        e = collection->head;
    }
    collection->head = nil;
    collection->tail = nil;
    collection->size = 0lu;
}

static uint64_t event_queue_func_size(event_queue_t * collection) {
    return collection->size;
}

static event_t * event_queue_func_push(event_queue_t * collection, event_t * e) {
    e->queue = collection;

    if(collection->tail) {
        collection->tail->next = e;
        e->prev = collection->tail;
    } else {
        collection->head = e;
    }
    
    collection->tail = e;
    collection->size = collection->size + 1;

    return e;
}

static event_t * event_queue_func_pop(event_queue_t * collection) {
    event_t * e = collection->head;

    if(e) {
        collection->head = e->next;
        if(!collection->head) {
            collection->tail = nil;
        }
        collection->size = collection->size - 1;

        e->next = nil;
        e->queue = nil;
    }

    return e;
}