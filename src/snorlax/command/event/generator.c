/**
 * @file        snorlax/command/event/generator.c
 * @brief
 * @details
 * 
 * @author      snorlax <opuntia@snorlax.bio>
 * @since       June 22, 2024
 */

#include "generator.h"

#include "subscription.h"
#include "subscription/type.h"
#include "subscription/process.h"
#include "type.h"

#include "../../event/queue.h"
#include "../../event/engine.h"
#include "../../event/generator.h"
#include "../../event/generator/set.h"
#include "../../event/generator/state.h"
#include "../../event/processor/pool.h"
#include "../../event/subscription/event.h"
#include "../../event/subscription.h"

static command_event_generator_t * command_event_generator_func_rem(___notnull command_event_generator_t * generator);
static ___sync int32_t command_event_generator_func_on(___notnull command_event_generator_t * generator);
static ___sync int32_t command_event_generator_func_off(___notnull command_event_generator_t * generator);
static ___sync int32_t command_event_generator_func_pub(___notnull command_event_generator_t * generator, event_queue_t * queue);
static ___sync int32_t command_event_generator_func_add(___notnull command_event_generator_t * generator, ___notnull command_event_subscription_t * subscription);
static ___sync int32_t command_event_generator_func_del(___notnull command_event_generator_t * generator, ___notnull command_event_subscription_t * subscription);
static ___sync void command_event_generator_func_clear(___notnull command_event_generator_t * generator);

static command_event_generator_func_t func = {
    command_event_generator_func_rem,
    command_event_generator_func_on,
    command_event_generator_func_off,
    command_event_generator_func_pub,
    command_event_generator_func_add,
    command_event_generator_func_del,
    command_event_generator_func_clear
};

extern command_event_generator_t * command_event_generator_gen(___notnull event_generator_set_t * set) {
#ifndef   RELEASE
    snorlaxdbg(set == nil, false, "critical", "");
    snorlaxdbg(set->engine == nil, false, "critical", "");
    snorlaxdbg(set->engine->pool == nil, false, "critical", "");
#endif // RELEASE

    command_event_generator_t * generator = (command_event_generator_t *) calloc(1, sizeof(command_event_generator_t));

    generator->func = address_of(func);
    generator->set = set;

    if(set->engine->pool->size > 0) generator->sync = sync_gen();

    return generator;
}

static command_event_generator_t * command_event_generator_func_rem(___notnull command_event_generator_t * generator) {
#ifndef   RELEASE
    snorlaxdbg(generator == nil, false, "critical", "");
#endif // RELEASE

    command_event_generator_off(generator);
    command_event_generator_clear(generator);

    generator->sync = sync_rem(generator->sync);

    free(generator);

    return nil;
}

static ___sync int32_t command_event_generator_func_on(___notnull command_event_generator_t * generator) {
#ifndef   RELEASE
    snorlaxdbg(generator == nil, false, "critical", "");
#endif // RELEASE

    object_lock(generator);

    if((generator->status & event_generator_state_on) == 0) {
        generator->status = generator->status | event_generator_state_on;
    } else {
#ifndef   RELEASE
        snorlaxdbg(false, true, "notice", "command event generator already on");
#endif // RELEASE
    }

    object_unlock(generator);

    return success;
}

static ___sync int32_t command_event_generator_func_off(___notnull command_event_generator_t * generator) {
#ifndef   RELEASE
    snorlaxdbg(generator == nil, false, "critical", "");
#endif // REELASE

    object_lock(generator);
    if(generator->status & event_generator_state_on) {
        generator->status = generator->status & (~event_generator_state_on);
    } else {
#ifndef   RELEASE
        snorlaxdbg(false, true, "notice", "command event generator already off");
#endif // RELEASE
    }
    object_unlock(generator);

    return success;
}

static ___sync int32_t command_event_generator_func_pub(___notnull command_event_generator_t * generator, event_queue_t * queue) {
#ifndef   RELEASE
    snorlaxdbg(generator == nil, false, "critical", "");
#endif // RELEASE

    int32_t n = 0;

    object_lock(generator);
    command_event_subscription_t * subscription = generator->head;

    while(subscription) {
        object_unlock(generator);

        object_lock(subscription);
        command_event_subscription_t * next = subscription->next;

        // TODO: UPGRADE

        // 이벤트 프로세스 함수는 GENERATOR 를 통해서 접근해야 한다.

        if(queue) {
            event_queue_push(queue, event_gen((event_subscription_t *) subscription,
                                              (event_subscription_process_t) command_event_subscription_process_execute,
                                              command_event_type_execute,
                                              command_event_subscription_node_gen((event_subscription_t *) subscription)));
        } else {
            command_event_subscription_process_execute(subscription, command_event_type_execute, nil);
        }
        // TODO: UPGRADE

        object_unlock(subscription);

        object_lock(generator);
        subscription = next;
        n = n + 1;
    }

    object_unlock(generator);

    return n;
}

static int32_t command_event_generator_func_control(___notnull command_event_generator_t * generator, ___notnull command_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * event) {
    return success;
}

static ___sync int32_t command_event_generator_func_add(___notnull command_event_generator_t * generator, ___notnull command_event_subscription_t * subscription) {
#ifndef   RELEASE
    snorlaxdbg(generator == nil, false, "critical", "");
    snorlaxdbg(subscription == nil, false, "critical", "");
#endif // RELEASE

    object_lock(generator);
    event_generator_func_add((event_generator_t *) generator, (event_subscription_t *) subscription);
    object_unlock(generator);

    object_lock(subscription);
    command_event_subscription_notify(subscription, command_event_type_subscription, (event_subscription_event_t *) command_event_subscription_type_add);
    object_unlock(subscription);
}

static ___sync int32_t command_event_generator_func_del(___notnull command_event_generator_t * generator, ___notnull command_event_subscription_t * subscription) {
#ifndef   RELEASE
    snorlaxdbg(generator == nil, false, "critical", "");
    snorlaxdbg(subscription == nil, false, "critical", "");
#endif // RELEASE

    object_lock(generator);
    event_generator_func_del((event_generator_t *) generator, (event_subscription_t *) subscription);
    object_unlock(generator);

    object_lock(subscription);
    command_event_subscription_notify(subscription, command_event_type_subscription, (event_subscription_event_t *) command_event_subscription_type_del);
    object_unlock(subscription);
}

static ___sync void command_event_generator_func_clear(___notnull command_event_generator_t * generator) {
#ifndef   RELEASE
    snorlaxdbg(generator == nil, false, "critical", "");
#endif // RELEASE

    object_lock(generator);
    while(generator->head) {
        command_event_subscription_t * subscription = generator->head;
        event_generator_func_del((event_generator_t *) generator, (event_subscription_t *) subscription);
        object_unlock(generator);

        object_lock(subscription);
        command_event_subscription_notify(subscription, command_event_type_subscription, (event_subscription_event_t *) command_event_subscription_type_del);
        object_unlock(subscription);

        object_lock(generator);
    }
    object_unlock(generator);
}
