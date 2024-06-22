/**
 * @file        snorlax/command/event/subscription.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 22, 2024
 */

#include "subscription.h"

#include "generator.h"
#include "type.h"

#include "../../event/generator.h"
#include "../../event/subscription/event/queue.h"

static command_event_subscription_t * command_event_subscription_func_rem(___notnull command_event_subscription_t * subscription);
static void command_event_subscription_func_on(___notnull command_event_subscription_t * subscription, uint32_t type, uint64_t param);

static uint64_t command_event_subscription_processor_func_subscription(___notnull command_event_subscription_t * subscription, uint32_t type, uint64_t param);
static uint64_t command_event_subscription_processor_func_execute(___notnull command_event_subscription_t * subscription, uint32_t type, uint64_t param);

static void command_event_subscription_subscriber_func_subscription(___notnull command_event_subscription_t * subscription, uint32_t type, uint64_t param);
static void command_event_subscription_subscriber_func_execute(___notnull command_event_subscription_t * subscription, uint32_t type, uint64_t param);

static command_event_subscription_process_t processor[command_event_type_max] = {
    command_event_subscription_processor_func_subscription,
    command_event_subscription_processor_func_execute
};

static command_event_subscription_handler_t subscriber[command_event_type_max] = {
    command_event_subscription_subscriber_func_subscription,
    command_event_subscription_subscriber_func_execute
};

static command_event_subscription_func_t func = {
    command_event_subscription_func_rem,
    command_event_subscription_func_on
};

extern command_event_subscription_t * command_event_subscription_gen(___notnull command_t * command, int32_t retry, command_event_subscription_handler_t * handler) {
#ifndef   RELEASE
    snorlaxdbg(command == nil, "critical", "");
#endif // RELEASE

    command_event_subscription_t * subscription = (command_event_subscription_t *) calloc(1, sizeof(command_event_subscription_t));

    subscription->func = address_of(func);
    subscription->command = command;
    subscription->retry = retry;
    subscription->handler = (command_event_subscription_handler_t *) calloc(command_event_type_max, sizeof(command_event_subscription_handler_t));

    if(handler) {
        for(int32_t i = 0; i < command_event_type_max; i++) {
            subscription->handler[i] = handler[i];
        }
    }

    return subscription;
}

static command_event_subscription_t * command_event_subscription_func_rem(___notnull command_event_subscription_t * subscription) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, "critical", "");
#endif // RELEASE
    command_event_generator_t * generator = subscription->generator;
    if(generator) command_event_generator_func_del(generator, subscription);

    object_lock(subscription);

    event_subscription_event_queue_clear(subscription->queue);

    command_event_subscription_on(subscription, command_event_type_subscription, (uint64_t) command_event_type_subscription_rem);
    object_unlock(subscription);

    subscription->sync = sync_rem(subscription->sync);
    free(subscription->handler);
    free(subscription);

    return nil;
}

static void command_event_subscription_func_on(___notnull command_event_subscription_t * subscription, uint32_t type, uint64_t param) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, "critical", "");
    snorlaxdbg(command_event_type_max <= type, "critical", "");
#endif // RELEASE

    command_event_subscription_process_t process = processor[type];
    uint64_t ret = process(subscription, type, param);

    command_event_subscription_handler_t on = subscription->handler[type];
    on = on ? on : subscriber[type];

    on(subscription, type, ret);

    if(subscription->retry > 0) {
        subscription->retry = subscription->retry - 1;
        if(subscription->retry == 0) {
            command_event_generator_t * generator = subscription->generator;

            object_lock(generator);
            event_generator_func_del((event_generator_t *) generator, (event_subscription_t *) subscription);
            object_unlock(generator);

            command_event_subscription_on(subscription, command_event_type_subscription, (uint64_t) command_event_type_subscription_del);
        }
    }
}

static uint64_t command_event_subscription_processor_func_subscription(___notnull command_event_subscription_t * subscription, uint32_t type, uint64_t param) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, "critical", "");
#endif // RELEASE

    return param;
}

static uint64_t command_event_subscription_processor_func_execute(___notnull command_event_subscription_t * subscription, uint32_t type, uint64_t param) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, "critical", "");
    snorlaxdbg(subscription->command == nil, "critical", "");
#endif // RELEASE

    return command_execute(subscription->command);
}

static void command_event_subscription_subscriber_func_subscription(___notnull command_event_subscription_t * subscription, uint32_t type, uint64_t param) {

}

static void command_event_subscription_subscriber_func_execute(___notnull command_event_subscription_t * subscription, uint32_t type, uint64_t param) {

}