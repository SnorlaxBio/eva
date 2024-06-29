/**
 * @file        snorlax/eva.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 22, 2024
 */

#include "eva.h"

#include "event/engine.h"
#include "event/subscription.h"
#include "event/generator.h"
#include "event/generator/set.h"

#include "command/event/subscription.h"
#include "descriptor/event/subscription.h"
#include "descriptor/event/subscription/process.h"
#include "socket.h"
#include "socket/event/subscription.h"
#include "socket/client/event/subscription.h"

static event_engine_t * engine = nil;

extern int32_t snorlax_eva_on(void) {
    if(engine == nil) {
        engine = event_engine_gen();
    }

    event_engine_on(engine);

    return success;
}

extern int32_t snorlax_eva_off(event_engine_cancel_t cancel) {
    if(engine) event_engine_off(engine, cancel);
}

extern int32_t snorlax_eva_run(void) {
#ifndef   RELEASE
    snorlaxdbg(engine == nil, false, "critical", "");
#endif // RELEASE

    int32_t ret = event_engine_run(engine);

    engine = nil;

    return ret;
}

extern event_subscription_t * snorlax_eva_command_sub(___notnull command_t * command, int32_t retry, command_event_subscription_handler_t * handler) {
#ifndef   RELEASE
    snorlaxdbg(command == nil, false, "critical", "");
    snorlaxdbg(engine == nil, false, "critical", "");
    snorlaxdbg(engine->set == nil, false, "critical", "");
    snorlaxdbg(engine->set->command == nil, false, "critical", "");
#endif // RELEASE
    command_event_subscription_t * subscription = command_event_subscription_gen(command, retry, handler);

    event_generator_add(engine->set->command, (event_subscription_t *) subscription);

    return (event_subscription_t *) subscription;
}

extern descriptor_event_subscription_t * snorlax_eva_descriptor_sub(___notnull descriptor_t * descriptor, descriptor_event_subscription_handler_t * handler) {
#ifndef   RELEASE
    snorlaxdbg(descriptor == nil, false, "critical", "");
    snorlaxdbg(engine == nil, false, "critical", "");
    snorlaxdbg(engine->set == nil, false, "critical", "");
    snorlaxdbg(engine->set->descriptor == nil, false, "critical", "");
#endif // RELEASE
    descriptor_event_subscription_t * subscription = descriptor_event_subscription_gen(descriptor, handler);

    event_generator_add(engine->set->descriptor, (event_subscription_t *) subscription);

    return subscription;
}

extern socket_event_subscription_t * snorlax_eva_socket_sub(___notnull socket_t * s, socket_event_subscription_handler_t * handler) {
#ifndef   RELEASE
    snorlaxdbg(s == nil, false, "critical", "");
    snorlaxdbg(engine == nil, false, "critical", "");
    snorlaxdbg(engine->set == nil, false, "critical", "");
    snorlaxdbg(engine->set->descriptor == nil, false, "critical", "");
#endif // RELEASE
    socket_event_subscription_t * subscription = socket_event_subscription_gen(s, handler);

    if(s->value <= invalid) {
        if(socket_open(s) == fail) {
            return (socket_event_subscription_t *) socket_event_subscription_rem((socket_event_subscription_t *) subscription);
        }
    }

    event_generator_add(engine->set->descriptor, (event_subscription_t *) subscription);

    return subscription;
}

extern socket_client_event_subscription_t * snorlax_eva_socket_client_sub(___notnull socket_client_t * s, socket_client_event_subscription_handler_t * handler) {
#ifndef   RELEASE
    snorlaxdbg(s == nil, false, "critical", "");
    snorlaxdbg(engine == nil, false, "critical", "");
    snorlaxdbg(engine->set == nil, false, "critical", "");
    snorlaxdbg(engine->set->descriptor == nil, false, "critical", "");
#endif // RELEASE
    socket_client_event_subscription_t * subscription = socket_client_event_subscription_gen(s, handler);

    if(s->value <= invalid) {
        if(socket_client_open(s) == fail) {
            return socket_client_event_subscription_rem((socket_client_event_subscription_t *) subscription);
        }
    }

    if(subscription->descriptor->status & (descriptor_state_open)) {
        socket_client_event_subscription_notify(subscription, descriptor_event_type_open, nil);
    }

    event_generator_add(engine->set->descriptor, (event_subscription_t *) subscription);

    return subscription; 
}

extern buffer_t * snorlax_eva_descriptor_buffer_in_get(descriptor_event_subscription_t * subscription) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
#endif // RELEASE

    return subscription->descriptor->buffer.in;
}

extern void snorlax_eva_descriptor_write(descriptor_event_subscription_t * subscription, const char * data, uint64_t len) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
    snorlaxdbg(subscription->descriptor == nil, false, "critical", "");
#endif // RELEASE
    buffer_write(subscription->descriptor->buffer.out, data, len);

    if(subscription->descriptor->status & descriptor_state_open_out) {
        event_subscription_process_t process = descriptor_event_subscription_process_get(descriptor_event_type_write);
        process((event_subscription_t *) subscription, descriptor_event_type_write, nil);
    }
}