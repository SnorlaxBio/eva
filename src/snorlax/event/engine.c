/**
 * @file        snorlax/event/engine.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 18, 2024
 */

#include <stdlib.h>

#include "engine.h"

#include "engine/conf.h"

#include "processor/pool.h"
#include "generator.h"
#include "generator/set.h"
#include "queue.h"

#include "../event.h"

#include "../command/event/generator.h"
#include "../command/event/subscription.h"

#include "../descriptor/event/generator.h"
#include "../descriptor/event/subscription.h"

static event_engine_t * engine = nil;

static void event_engine_func_cancel(event_engine_t * engine);

static event_engine_t * event_engine_func_rem(event_engine_t * engine);

static event_engine_func_t func = {
    event_engine_func_rem
};

extern uint32_t event_engine_pool_size(void) {
    if(engine) {
        object_lock(engine->pool);

        uint32_t n = engine->pool->size;

        object_unlock(engine->pool);

        return n;
    }

    return 0;
}

extern void event_engine_push(event_t * event) {
    if(engine && event) {
        object_lock(engine->queue);
        event_queue_push(engine->queue, event);
        object_unlock(engine->queue);

        object_wakeup(engine->queue, false);
    }
}

extern int32_t event_engine_on(event_engine_conf_t * conf) {
    if(engine == nil) {
        engine = (event_engine_t *) calloc(1, sizeof(event_engine_t));

        engine->func = &func;

        engine->queue = event_queue_gen();
        engine->pool = event_processor_pool_gen(engine, 0);
        engine->set = event_generator_set_gen();

        conf = conf ? conf : event_engine_conf_default_get();

        if(conf->command.on) engine->set->command = (event_generator_t *) command_event_generator_gen();
        if(conf->descriptor.on) engine->set->descriptor = (event_generator_t *) descriptor_event_generator_gen();


        // TODO: IMPLEMENT THIS
    }

    return success;
}

extern int32_t event_engine_off(event_engine_cancel_t cancel) {
    if(engine){
        object_lock(engine);
        engine->cancel = (cancel ? cancel : event_engine_func_cancel);
        object_unlock(engine);
    }
}

extern void event_engine_cancel_set(event_engine_cancel_t cancel) {
    if(engine) engine->cancel = (cancel ? cancel : event_engine_func_cancel);
}

extern int32_t event_engine_run(void) {
    if(engine == nil) event_engine_on(nil);

    object_lock(engine);

    while(engine->cancel == nil) {
        object_unlock(engine);

        if(event_engine_pool_size() == 0) {
            event_generator_set_pub(engine->set, nil);

            object_lock(engine->queue);
            uint64_t limit = engine->queue->size;
            for(uint64_t i = 0; i < limit; i++) {
                event_t * event = event_queue_pop(engine->queue);
                if(event) {
                    object_unlock(engine->queue);

                    event_on(event);
                    // TODO: FREE EVENT

                    object_lock(engine->queue);
                    continue;
                }
                break;
            }
            object_unlock(engine->queue);
        }

        object_lock(engine);
    }

    engine->cancel(engine);
    engine->cancel = nil;

    object_unlock(engine);

    engine = event_engine_rem(engine);

    return success;
}


extern descriptor_event_subscription_t * event_engine_descriptor_add(descriptor_t * descriptor, uint32_t interest) {
    descriptor_event_subscription_t * subscription = descriptor_event_subscription_gen(descriptor, interest);

    if(event_engine_pool_size() > 0) subscription->sync = sync_gen();

    object_lock(engine->set->descriptor);
    descriptor_event_generator_add(((descriptor_event_generator_t *) engine->set->descriptor), subscription);
    object_unlock(engine->set->descriptor);

    return subscription;
}

extern command_event_subscription_t * event_engine_command_add(command_t * command, uint32_t status) {
    command_event_subscription_t * subscription = command_event_subscription_gen(command, status);

    if(event_engine_pool_size() > 0) subscription->sync = sync_gen();

    object_lock(engine->set->command);
    command_event_generator_add(((command_event_generator_t *) engine->set->command), subscription);
    object_unlock(engine->set->command);

    return subscription;
}

static event_engine_t * event_engine_func_rem(event_engine_t * engine) {
    object_lock(engine);
    engine->pool = event_processor_pool_rem(engine->pool);
    engine->set = event_generator_set_rem(engine->set);
    engine->queue = event_queue_rem(engine->queue);
    object_unlock(engine);

    engine->sync = sync_rem(engine->sync);
    free(engine);
    return nil;
}

static void event_engine_func_cancel(event_engine_t * engine) {

}
