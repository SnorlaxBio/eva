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
#include "event/generator.h"
#include "event/generator/set.h"

#include "command/event/subscription.h"

static event_engine_t * engine = nil;

extern int32_t snorlax_eva_on(void) {
    if(engine == nil) {
        engine = event_engine_gen();
    }

    event_engine_on(engine);

    return success;
}

extern int32_t snorlax_eva_run(void) {
#ifndef   RELEASE
    snorlaxdbg(engine == nil, false, "critical", "");
#endif // RELEASE

    return event_engine_run(engine);
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