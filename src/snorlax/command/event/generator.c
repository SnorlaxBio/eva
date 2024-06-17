/**
 * @file        snorlax/command/event/generator.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 18, 2024
 */

#include <stdlib.h>

#include "generator.h"
#include "subscription.h"

#include "../event.h"

#include "../../event/engine.h"
#include "../../event/generator.h"

static command_event_generator_t * command_event_generator_func_rem(command_event_generator_t * generator);
static int32_t command_event_generator_func_on(command_event_generator_t * generator);
static int32_t command_event_generator_func_off(command_event_generator_t * generator);
static int32_t command_event_generator_func_pub(command_event_generator_t * generator, event_queue_t * queue);
static int32_t command_event_generator_func_add(command_event_generator_t * generator, command_event_subscription_t * subscription);
static int32_t command_event_generator_func_del(command_event_generator_t * generator, command_event_subscription_t * subscription);

static command_event_generator_func_t func = {
    command_event_generator_func_rem,
    command_event_generator_func_on,
    command_event_generator_func_off,
    command_event_generator_func_pub,
    command_event_generator_func_add,
    command_event_generator_func_del
};

extern command_event_generator_t * command_event_generator_gen(void) {
    command_event_generator_t * generator = (command_event_generator_t *) calloc(1, sizeof(command_event_generator_t));

    generator->func = &func;

    return generator;
}

static command_event_generator_t * command_event_generator_func_rem(command_event_generator_t * generator) {
    // TODO: ASSERTION 등록된 서브스크립션이 존재하면 안된다.

    generator->sync = sync_rem(generator->sync);

    free(generator);

    return nil;
}

static int32_t command_event_generator_func_on(command_event_generator_t * generator) {
    return success;
}

static int32_t command_event_generator_func_off(command_event_generator_t * generator) {
    return success;
}

static int32_t command_event_generator_func_pub(command_event_generator_t * generator, event_queue_t * queue) {
    int32_t n = 0;
    command_event_subscription_t * subscription = nil;

    object_lock(generator);
    uint64_t limit = generator->size;
    subscription = generator->head;
    object_unlock(generator);
    
    object_lock(subscription);
    for(uint64_t i = 0; i < limit && subscription; i++) {
        if(event_engine_pool_size() == 0) {
            command_exec(subscription->command);

            if(subscription->status & command_event_subscription_state_once) {
                object_lock(generator);
                command_event_generator_func_del(generator, subscription);
                object_unlock(generator);
            }
        } else {
            event_engine_push((event_t *) command_event_gen(subscription, command_event_type_exec));
        }
        object_unlock(subscription);

        subscription = subscription->next;

        object_lock(subscription);
    }
    object_unlock(subscription);
    return n;
}

static int32_t command_event_generator_func_add(command_event_generator_t * generator, command_event_subscription_t * subscription) {
    int32_t ret = event_generator_func_add((event_generator_t *) generator, (event_subscription_t *) subscription);
    // TODO: IF NEED, IMPLEMENT THIS
    return ret;
}

static int32_t command_event_generator_func_del(command_event_generator_t * generator, command_event_subscription_t * subscription) {
    int32_t ret = event_generator_func_del((event_generator_t *) generator, (event_subscription_t *) subscription);
    // TODO: IF NEED, IMPLEMENT THIS
    return ret;
}
