/**
 * @file        snorlax/command/event/subscription/action.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 22, 2024
 */

#include "action.h"

#include "../type.h"
#include "../subscription.h"
#include "../generator.h"

#include "../../../event/generator/set.h"
#include "../../../event/engine.h"
#include "../../../event/processor/pool.h"

extern void command_event_action_execute(___notnull command_event_subscription_t * subscription, event_queue_t * queue) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, "critical", "");
    snorlaxdbg(subscription->generator == nil, "critical", "");
    snorlaxdbg(subscription->generator->set == nil, "critical", "");
    snorlaxdbg(subscription->generator->set->engine == nil, "critical", "");
    snorlaxdbg(subscription->generator->set->engine->pool == nil, "critical", "");
    snorlaxdbg(subscription->generator->set->engine->pool->size == nil, "critical", "");
#endif // RELEASE

    object_lock(subscription);

    if(subscription->generator->set->engine->pool->size > 0) {
        command_event_subscription_event_t * node = command_event_subscription_event_gen(subscription);

        command_event_t * event = command_event_gen(subscription, command_event_type_execute, node);
        // CREATE
    } else {

    }

    object_lock(subscription);
}
