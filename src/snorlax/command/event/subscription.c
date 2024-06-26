/**
 * @file        snorlax/command/event/subscription.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 22, 2024
 */

#include "subscription.h"

#include "subscription/type.h"

#include "type.h"
#include "generator.h"

#include "../../event/subscription/event/queue.h"

static command_event_subscription_t * command_event_subscription_func_rem(___notnull command_event_subscription_t * subscription);
static void command_event_subscription_func_on(___notnull command_event_subscription_t * subscription, command_event_subscription_process_t process, uint32_t type, event_subscription_event_t * node);
static void command_event_subscription_func_notify(___notnull command_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node);

static command_event_subscription_func_t func = {
    command_event_subscription_func_rem,
    command_event_subscription_func_on,
    command_event_subscription_func_notify
};

extern command_event_subscription_t * command_event_subscription_gen(___notnull command_t * command, int32_t retry, command_event_subscription_handler_t * handler) {
#ifndef   RELEASE
    snorlaxdbg(command == nil, false, "critical", "");
#endif // RELEASE

    command_event_subscription_t * subscription = (command_event_subscription_t *) calloc(1, sizeof(command_event_subscription_t));

    subscription->func = address_of(func);
    subscription->command = command;
    subscription->retry = retry;
    subscription->queue = event_subscription_event_queue_gen();
    subscription->type = event_subscription_type_command;

    subscription->handler = (command_event_subscription_handler_t *) calloc(command_event_type_max, sizeof(command_event_subscription_handler_t));

    if(handler) {
        for(uint32_t i = 0; i < command_event_type_max; i++) {
            subscription->handler[i] = handler[i];
        }
    }

    return subscription;
}

/**
 * 
 * 동기화 관련하여 고민을 해야 한다.
 */
static command_event_subscription_t * command_event_subscription_func_rem(___notnull command_event_subscription_t * subscription) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
#endif // RELEASE

    command_event_generator_t * generator = subscription->generator;

    if(generator) command_event_generator_del(generator, subscription);

    event_subscription_event_queue_clear(subscription->queue);

    command_event_subscription_notify(subscription, command_event_type_subscription, (event_subscription_event_t *) command_event_subscription_type_rem);

    subscription->sync = sync_rem(subscription->sync);

    subscription->handler = memory_rem(subscription->handler);
    subscription->queue = event_subscription_event_queue_rem(subscription->queue);

    free(subscription);

    return nil;
}

static void command_event_subscription_func_on(___notnull command_event_subscription_t * subscription, command_event_subscription_process_t process, uint32_t type, event_subscription_event_t * node) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
    snorlaxdbg(process == nil, false, "critical", "");
#endif // RELEASE

    process(subscription, type, node);
}

static void command_event_subscription_func_notify(___notnull command_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, false, "critical", "");
#endif // RELEASE

    command_event_subscription_handler_t on = subscription->handler[type];

    if(on) {
        on(subscription, type, node);
    }
}































// #include "subscription/type.h"

// #include "generator.h"
// #include "type.h"

// #include "../../event/generator.h"
// #include "../../event/subscription/event/queue.h"

// static command_event_subscription_t * command_event_subscription_func_rem(___notnull command_event_subscription_t * subscription);
// static void command_event_subscription_func_on(___notnull command_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * event);

// static event_subscription_event_t * command_event_subscription_processor_func_subscription(___notnull command_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * event);
// static event_subscription_event_t * command_event_subscription_processor_func_execute(___notnull command_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * event);

// static void command_event_subscription_subscriber_func_subscription(___notnull command_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * event);
// static void command_event_subscription_subscriber_func_execute(___notnull command_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * event);

// static command_event_subscription_process_t processor[command_event_type_max] = {
//     command_event_subscription_processor_func_subscription,
//     command_event_subscription_processor_func_execute
// };

// static command_event_subscription_handler_t subscriber[command_event_type_max] = {
//     command_event_subscription_subscriber_func_subscription,
//     command_event_subscription_subscriber_func_execute
// };

// static command_event_subscription_func_t func = {
//     command_event_subscription_func_rem,
//     command_event_subscription_func_on
// };

// extern command_event_subscription_t * command_event_subscription_gen(___notnull command_t * command, int32_t retry, command_event_subscription_handler_t * handler) {
// #ifndef   RELEASE
//     snorlaxdbg(command == nil, false, "critical", "");
// #endif // RELEASE

//     command_event_subscription_t * subscription = (command_event_subscription_t *) calloc(1, sizeof(command_event_subscription_t));

//     subscription->func = address_of(func);
//     subscription->command = command;
//     subscription->retry = retry;
//     subscription->queue = event_subscription_event_queue_gen();
//     subscription->handler = (command_event_subscription_handler_t *) calloc(command_event_type_max, sizeof(command_event_subscription_handler_t));

//     if(handler) {
//         for(int32_t i = 0; i < command_event_type_max; i++) {
//             subscription->handler[i] = handler[i];
//         }
//     }

//     return subscription;
// }

// static command_event_subscription_t * command_event_subscription_func_rem(___notnull command_event_subscription_t * subscription) {
// #ifndef   RELEASE
//     snorlaxdbg(subscription == nil, false, "critical", "");
// #endif // RELEASE
//     command_event_generator_t * generator = subscription->generator;
//     if(generator) command_event_generator_del(generator, subscription);

//     object_lock(subscription);

//     // TODO: CHECK THIS LOGIC
//     event_subscription_event_queue_clear(subscription->queue);

//     command_event_subscription_on(subscription, command_event_type_subscription, (event_subscription_event_t *) command_event_subscription_type_rem);

//     subscription->queue = event_subscription_event_queue_rem(subscription->queue);

//     object_unlock(subscription);

//     subscription->sync = sync_rem(subscription->sync);
//     free(subscription->handler);
//     free(subscription);

//     return nil;
// }

// static void command_event_subscription_func_on(___notnull command_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node) {
// #ifndef   RELEASE
//     snorlaxdbg(subscription == nil, false, "critical", "");
// #endif // RELEASE



// //     if(type == command_event_type_subscription && node == command_event_subscription_type_rem) {

// //     } else {
// // #ifndef   RELEASE
// //         snorlaxdbg(subscription->generator == nil, false, "critical", "");
// // #endif // RELEASE
// //     }
    
//     // command_event_generator_t * generator = subscription->generator;

//     // command_event_generator_process_t process = command_event_generator_processor(generator, type);

//     // process(subscription, type, event);
// }

// static event_subscription_event_t * command_event_subscription_processor_func_subscription(___notnull command_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * event) {
// #ifndef   RELEASE
//     snorlaxdbg(subscription == nil, false, "critical", "");
// #endif // RELEASE

//     command_event_subscription_handler_t on = subscription->handler[type];
//     on = on ? on : subscriber[type];

//     on(subscription, type, event);

//     return event;
// }

// static event_subscription_event_t * command_event_subscription_processor_func_execute(___notnull command_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * event) {
// #ifndef   RELEASE
//     snorlaxdbg(subscription == nil, false, "critical", "");
//     snorlaxdbg(subscription->command == nil, false, "critical", "");
// #endif // RELEASE

//     int32_t ret = command_execute(subscription->command);

//     command_event_subscription_handler_t on = subscription->handler[type];
//     on = on ? on : subscriber[type];

//     on(subscription, type, event);

//     if(type == command_event_type_execute) {
//         if(subscription->retry > 0) {
//             subscription->retry = subscription->retry - 1;
//         }
        
//         if(subscription->retry == 0) {
//             command_event_generator_t * generator = subscription->generator;

//             object_lock(generator);
//             event_generator_func_del((event_generator_t *) generator, (event_subscription_t *) subscription);
//             object_unlock(generator);

//             command_event_subscription_on(subscription, command_event_type_subscription, (event_subscription_event_t *) command_event_subscription_type_del);
//         }
//     }

//     return event;
// }

// static void command_event_subscription_subscriber_func_subscription(___notnull command_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * event) {

// }

// static void command_event_subscription_subscriber_func_execute(___notnull command_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * event) {

// }