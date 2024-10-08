#include "subscription.h"

#include "../../../descriptor/event/subscription/type.h"
#include "../../../descriptor/event/subscription/process.h"
#include "../../../descriptor/event/subscription.h"
#include "../../../descriptor/event/generator.h"
#include "../../../event/subscription/meta.h"
#include "../../../event/subscription/event/queue.h"
#include "../../../event/subscription/event.h"

static socket_client_event_subscription_func_t func = {
    socket_client_event_subscription_func_rem,
    socket_client_event_subscription_func_on,
    socket_client_event_subscription_func_notify
};

extern socket_client_event_subscription_t * socket_client_event_subscription_gen(socket_client_t * client, socket_client_event_subscription_handler_t * handler, socket_client_event_subscription_pool_t * pool) {
#ifndef   RELEASE
    snorlaxdbg(client == nil, false, "critical", "");
#endif // RELEASE

    socket_client_event_subscription_t * subscription = (socket_client_event_subscription_t *) calloc(1, sizeof(socket_client_event_subscription_t));

    subscription->func = address_of(func);
    subscription->descriptor = client;
    subscription->type = event_subscription_type_descriptor;

    if(handler) {
        subscription->handler = (socket_client_event_subscription_handler_t *) calloc(descriptor_event_type_max, sizeof(socket_client_event_subscription_handler_t));

        // MEMCPY ?????
        for(uint32_t i = 0; i < descriptor_event_type_max; i++) {
            subscription->handler[i] = handler[i];
        }
    }

    if(pool) {
        subscription->node = socket_client_event_subscription_pool_node_gen(subscription);
        socket_client_event_subscription_pool_push(pool, subscription->node);
    }

    return subscription;
}

extern socket_client_event_subscription_t * socket_client_event_subscription_func_rem(___notnull socket_client_event_subscription_t * subscription) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
#endif // RELEASE

    subscription->queue = event_subscription_event_queue_rem(subscription->queue);

    socket_client_t * descriptor = subscription->descriptor;
    if(descriptor) {
        if(descriptor->value > invalid) {
            if(subscription->generator) {
                descriptor_event_generator_del(subscription->generator, (descriptor_event_subscription_t *) subscription);
            }

            int32_t value = descriptor->value;
            descriptor_close(descriptor);

            descriptor_event_subscription_notify(subscription, descriptor_event_type_close, event_subscription_event_parameter_set(nil, value));

            if(descriptor->buffer.in) buffer_clear(descriptor->buffer.in);
            if(descriptor->buffer.out) buffer_clear(descriptor->buffer.out);
        }

        subscription->descriptor = descriptor_rem(descriptor);
    }

    if(subscription->node) {
        if(subscription->node->collection) {
            socket_client_event_subscription_pool_del(subscription->node->collection, subscription->node);
        }
        socket_client_event_subscription_pool_node_t * node = subscription->node;
        subscription->node = nil;
        node->client = nil;

        subscription->node = socket_client_event_subscription_pool_node_rem(node);
    }

    subscription->sync = sync_rem(subscription->sync);

    free(subscription);

    return nil;
}

extern void socket_client_event_subscription_func_on(___notnull socket_client_event_subscription_t * subscription, socket_client_event_subscription_process_t process, uint32_t type, event_subscription_event_t * node) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
    snorlaxdbg(process == nil, false, "critical", "");
#endif // RELEASE

    process(subscription, type, node);
}

extern void socket_client_event_subscription_func_notify(___notnull socket_client_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
#endif // RELEASE

    // TODO: ERROR HANDLING: NO EXCEPTION
    socket_client_event_subscription_handler_t on = subscription->handler ? subscription->handler[type] : (subscription->node ? subscription->node->collection->handler[type] : nil);

    if(on) {
        on(subscription, type, node);
    }

    if(type == descriptor_event_type_close) {
        if(subscription->descriptor->value > invalid) {
            // TODO: UPGRADE RECONNECT
        }
    }
}

extern descriptor_buffer_t * socket_client_event_subscription_buffer_get(___notnull socket_client_event_subscription_t * subscription) {
    return address_of(subscription->descriptor->buffer);
}

/**
 * 디스크립터 WRITE 에 동일하다. 하나로 합쳐서 공통적으로 호출하도록 사용하면 좋을 것 같다.
 */
extern int64_t socket_client_event_subscription_write(___notnull socket_client_event_subscription_t * subscription, const uint8_t * data, uint64_t datalen) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
#endif // RELEASE

    snorlaxdbg(false, true, "implement", "descriptor is not open or alive");

    if(subscription->descriptor->value == invalid) {
        return fail;
    }

    buffer_t * out = subscription->descriptor->buffer.out;

    buffer_push(subscription->descriptor->buffer.out, data, datalen);

    if(subscription->descriptor->status & descriptor_state_open_out) {
        // 아래 루틴을 가볍게 바꿀 수 있다. 매크로로 바꾸던, 함수를 만들던, 함수 호출 비용을 생각해보자.
        event_subscription_process_t process = descriptor_event_subscription_process_get(descriptor_event_type_write);
        process((event_subscription_t *) subscription, descriptor_event_type_write, nil);
    }

    return datalen;
}
