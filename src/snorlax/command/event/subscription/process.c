/**
 * @file        snorlax/command/event/subscription/process.c
 * @brief
 * @details
 * 
 * @author      snorlax <opuntia@snorlax.bio>
 * @since       June 25, 2024
 */

#include "process.h"

#include "../subscription.h"
#include "../subscription/type.h"

#include "../generator.h"

#include "../../../command.h"
#include "../../../event/subscription/event.h"

extern void command_event_subscription_process_execute(command_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
    snorlaxdbg(subscription->command == nil, false, "critical", "");
#endif // RELEASE

    command_t * command = subscription->command;

    int32_t ret = command->execute(command);

    command_event_subscription_handler_t on = subscription->handler[type];

    if(on) {
        on(subscription, type, event_subscription_event_parameter_set(node, ret));
    }

    if(subscription->retry > 0) {
        subscription->retry = subscription->retry - 1;
        if(subscription->retry == 0) {
            command_event_generator_del(subscription->generator, subscription);
        }
    }
}