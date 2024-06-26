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

extern event_subscription_t * snorlax_eva_descriptor_sub(___notnull descriptor_t * descriptor, descriptor_event_subscription_handler_t * handler) {
#ifndef   RELEASE
    snorlaxdbg(descriptor == nil, false, "critical", "");
    snorlaxdbg(engine == nil, false, "critical", "");
    snorlaxdbg(engine->set == nil, false, "critical", "");
    snorlaxdbg(engine->set->descriptor == nil, false, "critical", "");
#endif // RELEASE
    descriptor_event_subscription_t * subscription = descriptor_event_subscription_gen(descriptor, handler);

    event_generator_add(engine->set->descriptor, (event_subscription_t *) subscription);

    return (event_subscription_t *) subscription;
}

extern buffer_t * snorlax_eva_descriptor_buffer_in_get(descriptor_event_subscription_t * subscription) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
#endif // RELEASE

    return subscription->descriptor->buffer.in;
}
