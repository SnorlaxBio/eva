/**
 * @file        snorlax/event/engine.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 22, 2024
 */

#include "engine.h"

#include "queue.h"
#include "processor/pool.h"
#include "generator/set.h"
#include "generator.h"
#include "../descriptor/event/subscription.h"
#include "../socket/event/subscription.h"
#include "../socket/server/event/subscription.h"
#include "../socket/client/event/subscription.h"

static void event_engine_func_default_cancel(const event_engine_t * engine);

static event_engine_t * event_engine_func_rem(___notnull event_engine_t * engine);
static int32_t event_engine_func_on(___notnull event_engine_t * engine);
static int32_t event_engine_func_off(___notnull event_engine_t * engine, event_engine_cancel_t cancel);
static int32_t event_engine_func_run(___notnull event_engine_t * engine);

static event_engine_func_t func = {
    event_engine_func_rem,
    event_engine_func_on,
    event_engine_func_off,
    event_engine_func_run
};

extern event_engine_t * event_engine_gen(void) {
    event_engine_t * engine = (event_engine_t *) calloc(1, sizeof(event_engine_t));

    engine->func = address_of(func);

    engine->pool = event_processor_pool_gen(0, engine);
    engine->set = event_generator_set_gen(engine);
    engine->queue = event_queue_gen();

    if(engine->pool->size > 0) sync_on(engine->queue);

    return engine;
}

static event_engine_t * event_engine_func_rem(___notnull event_engine_t * engine) {
#ifndef   RELEASE
    snorlaxdbg(engine == nil, false, "critical", "");
#endif // RELEASE

    engine->set = event_generator_set_rem(engine->set);
    engine->pool = event_processor_pool_rem(engine->pool);
    engine->queue = event_queue_rem(engine->queue);

    engine->sync = sync_rem(engine->sync);

    free(engine);

    return nil;
}

static int32_t event_engine_func_on(___notnull event_engine_t * engine) {
#ifndef   RELEASE
    snorlaxdbg(engine == nil, false, "critical", "");
#endif // RELEASE

    if((engine->status & event_engine_state_on) == 0) {
        engine->status = engine->status | event_engine_state_on;

        event_processor_pool_on(engine->pool);
        event_generator_set_on(engine->set);
    } else {
#ifndef   RELEASE
        snorlaxdbg(false, true, "notice", "event engine already on");
#endif // RELEASE
    }

    return success;
}

static int32_t event_engine_func_off(___notnull event_engine_t * engine, event_engine_cancel_t cancel) {
#ifndef   RELEASE
    snorlaxdbg(engine == nil, false, "critical", "");
#endif // RELEASE

    if(engine->status & event_engine_state_on) {
        engine->status = engine->status & (~event_engine_state_on);
        if(engine->cancel == nil) {
            engine->cancel = cancel ? cancel : event_engine_func_default_cancel;
        } else {
#ifndef   RELEASE
            snorlaxdbg(false, true, "notice", "event engine cancel already set");
#endif // RELEASE
        }
    } else {
#ifndef   RELEASE
        snorlaxdbg(false, true, "notice", "event engine already off");
#endif // RELEASE
    }

    return success;
}

static int32_t event_engine_func_run(___notnull event_engine_t * engine) {
#ifndef   RELEASE
    snorlaxdbg(engine == nil, false, "critical", "");
#endif // RELEASE
    event_processor_pool_t * pool = engine->pool;

    while(engine->cancel == nil) {
        event_queue_t * queue = pool->size > 0 ? engine->queue : nil;

        event_generator_set_pub(engine->set, queue);

        queue = engine->queue;

        if(queue != nil) {
            queue = engine->queue;
            object_lock(queue);
            uint64_t limit = queue->size;
            for(uint64_t i = 0; i < limit; i++) {
                event_t * event = event_queue_pop(queue);
                if(event) {
                    object_unlock(queue);
                    event_on(event);
                    object_lock(queue);
                    continue;;
                }
                break;
            }
            object_unlock(queue);
        }
    }
#ifndef   RELEASE
    snorlaxdbg(engine->status & event_engine_state_on, false, "critical", "");
#endif // RELEASE
    engine->cancel(engine);
    engine->cancel = nil;

    engine = event_engine_rem(engine);

    return success;
}

static void event_engine_func_default_cancel(const event_engine_t * engine) {
    
}

extern descriptor_event_subscription_t * event_engine_descriptor_sub(event_engine_t * engine, descriptor_t * descriptor, descriptor_event_subscription_handler_t * handler) {
#ifndef   RELEASE
    snorlaxdbg(engine == nil, false, "critical", "");
    snorlaxdbg(engine->set == nil, false, "critical", "");
    snorlaxdbg(engine->set->descriptor == nil, false, "critical", "");
    snorlaxdbg(descriptor == nil, false, "critical", "");
#endif // RELEASE

    descriptor_event_subscription_t * subscription = descriptor_event_subscription_gen(descriptor, handler);

    if(descriptor->value <= invalid) {
        if(descriptor_open(descriptor) == fail) {
            return descriptor_event_subscription_rem(subscription);
        }
    }

    if(subscription->descriptor->status & (descriptor_state_open)) {
        descriptor_event_subscription_notify(subscription, descriptor_event_type_open, nil);
    }

    // TODO: CHECK ERROR HANDLING
    event_generator_add(engine->set->descriptor, (event_subscription_t *) subscription);

    return subscription;
}

extern socket_event_subscription_t * event_engine_socket_sub(event_engine_t * engine, socket_t * descriptor, socket_event_subscription_handler_t * handler) {
#ifndef   RELEASE
    snorlaxdbg(engine == nil, false, "critical", "");
    snorlaxdbg(engine->set == nil, false, "critical", "");
    snorlaxdbg(engine->set->descriptor == nil, false, "critical", "");
    snorlaxdbg(descriptor == nil, false, "critical", "");
#endif // RELEASE

    socket_event_subscription_t * subscription = socket_event_subscription_gen(descriptor, handler);

    if(descriptor->value <= invalid) {
        if(socket_open(descriptor) == fail) {
            return socket_event_subscription_rem(subscription);
        }
    }

    if(subscription->descriptor->status & (descriptor_state_open)) {
        socket_event_subscription_notify(subscription, descriptor_event_type_open, nil);
    }

    // TODO: CHECK ERROR HANDLING
    event_generator_add(engine->set->descriptor, (event_subscription_t *) subscription);

    return subscription;
}

extern socket_server_event_subscription_t * event_engine_socket_server_sub(event_engine_t * engine, socket_server_t * descriptor, socket_session_event_subscription_handler_t * sessionOn, socket_server_event_subscription_handler_t * serverOn) {
#ifndef   RELEASE
    snorlaxdbg(engine == nil, false, "critical", "");
    snorlaxdbg(engine->set == nil, false, "critical", "");
    snorlaxdbg(engine->set->descriptor == nil, false, "critical", "");
    snorlaxdbg(descriptor == nil, false, "critical", "");
#endif // RELEASE

    socket_server_event_subscription_t * subscription = socket_server_event_subscription_gen(descriptor, sessionOn, serverOn);

    if(descriptor->value <= invalid) {
        if(socket_server_open(descriptor) == fail) {
            return socket_server_event_subscription_rem(subscription);
        }
    }

    if(subscription->descriptor->status & (descriptor_state_open)) {
        socket_server_event_subscription_notify(subscription, descriptor_event_type_open, nil);
    }

    // TODO: CHECK ERROR HANDLING
    event_generator_add(engine->set->descriptor, (event_subscription_t *) subscription);

    return subscription;
}

extern socket_client_event_subscription_t * event_engine_socket_client_sub(event_engine_t * engine, socket_client_t * descriptor, socket_client_event_subscription_handler_t * handler, socket_client_event_subscription_pool_t * pool) {
#ifndef   RELEASE
    snorlaxdbg(engine == nil, false, "critical", "");
    snorlaxdbg(engine->set == nil, false, "critical", "");
    snorlaxdbg(engine->set->descriptor == nil, false, "critical", "");
    snorlaxdbg(descriptor == nil, false, "critical", "");
#endif // RELEASE

    socket_client_event_subscription_t * subscription = socket_client_event_subscription_gen(descriptor, handler, pool);

    if(descriptor->value <= invalid) {
        if(socket_client_open(descriptor) == fail) {
            return socket_client_event_subscription_rem(subscription);
        }
    }
    
    if(subscription->descriptor->status & (descriptor_state_open)) {
        socket_client_event_subscription_notify(subscription, descriptor_event_type_open, nil);
    }

    // TODO: CHECK ERROR HANDLING
    event_generator_add(engine->set->descriptor, (event_subscription_t *) subscription);

    return subscription;
}
