/**
 * @file        snorlax/command/event/subscription/action.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 22, 2024
 */

#include "action.h"

#include "event.h"

#include "../type.h"
#include "../subscription.h"

#include "../../event/queue.h"

extern void command_event_action_execute(___notnull command_event_subscription_t * subscription, event_queue_t * queue) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, "critical", "");
#endif // RELEASE

    object_lock(subscription);

    if(queue) {
        command_event_subscription_event_t * node = command_event_subscription_event_gen(subscription);
        command_event_t * event = command_event_gen(subscription, command_event_type_execute, node);
        event_queue_push(queue, event);
    } else {
        if(subscription->count < subscription->retry) {
            command_t * command = subscription->command;
            int32_t ret = command_execute(command);
            subscription->count = subscription->count + 1;

            command_event_subscription_on(subscription, command_event_type_execute, ret);
            // 사용자가 SUBSCRIPTION 객체를 해지할 경우 .. 그리고 지울 경우... 

            if(subscription->count == subscription->retry) {
                command_event_generator_t * generator = subscription->generator;

                object_lock(generator);
                command_event_generator_del(generator, subscription);
                object_unlock(generator);
            }
        } else {
#ifndef   RELEASE
            snorlaxdbg(false, "warning", "subscription->count >= subscription->retry");
#endif // RELEASE
        }
        
        
    }

    object_lock(subscription);
}
