#include <string.h>
#include <stdlib.h>

#include "pool.h"
#include "../subscription.h"

static socket_client_event_subscription_pool_func_t func = {
    socket_client_event_subscription_pool_func_rem,
    socket_client_event_subscription_pool_func_push,
    socket_client_event_subscription_pool_func_pop,
    socket_client_event_subscription_pool_func_del
};

extern socket_client_event_subscription_pool_t * socket_client_event_subscription_pool_gen(socket_client_event_subscription_handler_t * handler) {
#ifndef   RELEASE
    snorlaxdbg(handler == nil, false, "critical", "");
#endif // RELEASE

    socket_client_event_subscription_pool_t * pool = (socket_client_event_subscription_pool_t *) calloc(1, sizeof(socket_client_event_subscription_pool_t));

    pool->func = address_of(func);

    pool->handler = (socket_client_event_subscription_handler_t *) calloc(descriptor_event_type_max, sizeof(socket_client_event_subscription_handler_t));

    memcpy(pool->handler, handler, descriptor_event_type_max * sizeof(socket_client_event_subscription_handler_t));

    return pool;
}

extern socket_client_event_subscription_pool_t * socket_client_event_subscription_pool_func_rem(socket_client_event_subscription_pool_t * pool) {
#ifndef   RELEASE
    snorlaxdbg(pool == nil, false, "critical", "");
#endif // RELEASE
    while(pool->head) socket_client_event_subscription_rem(pool->head->client);

    pool->handler = memory_rem(pool->handler);
    pool->sync = sync_rem(pool->sync);

    free(pool);

    return nil;
}

extern void socket_client_event_subscription_pool_func_push(socket_client_event_subscription_pool_t * pool, socket_client_event_subscription_pool_node_t * node) {
#ifndef   RELEASE
    snorlaxdbg(pool == nil, false, "critical", "");
    snorlaxdbg(node == nil, false, "critical", "");
    snorlaxdbg(node->collection != nil, false, "critical", "");
#endif // RELEASE

    if(pool->tail) {
        pool->tail->next = node;
        node->prev = pool->tail;
    } else {
        pool->head = node;
    }

    pool->tail = node;
    pool->size = pool->size + 1;
    node->collection = pool;
}

extern socket_client_event_subscription_pool_node_t * socket_client_event_subscription_pool_func_pop(socket_client_event_subscription_pool_t * pool) {
#ifndef   RELEASE
    snorlaxdbg(pool == nil, false, "critical", "");
#endif // RELEASE

    socket_client_event_subscription_pool_node_t * node = pool->head;

    if(node) {
        if(node->next) {
            node->next->prev = nil;
        } else {
            pool->tail = nil;
        }
        pool->head = node->next;
        pool->size = pool->size - 1;
        node->next = nil;
        node->collection = nil;
    }

    return node;
}

extern void socket_client_event_subscription_pool_func_del(socket_client_event_subscription_pool_t * pool, socket_client_event_subscription_pool_node_t * node) {
#ifndef   RELEASE
    snorlaxdbg(pool == nil, false, "critical", "");
    snorlaxdbg(node == nil, false, "critical", "");
    snorlaxdbg(pool != node->collection, false, "critical", "");
#endif // RELEASE

    socket_client_event_subscription_pool_node_t * prev = node->prev;
    socket_client_event_subscription_pool_node_t * next = node->next;

    if(prev) {
        prev->next = next;
        node->prev = nil;
    } else {
        pool->head = next;
    }

    if(next) {
        next->prev = prev;
        node->next = nil;
    } else {
        pool->tail = prev;
    }

    pool->size = pool->size - 1;
    node->collection = nil;
}