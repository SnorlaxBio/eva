/**
 * @file        snorlax/command/event/generator.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 22, 2024
 */

#include "generator.h"

#include "../../event/generator/set.h"
#include "../../event/processor/pool.h"
#include "../../event/engine.h"

static command_event_generator_t * command_event_generator_func_rem(___notnull command_event_generator_t * generator);
    // int32_t (*on)(___notnull command_event_generator_t *);
    // int32_t (*off)(___notnull command_event_generator_t *);
    // int32_t (*pub)(___notnull command_event_generator_t *, event_queue_t *);
    // int32_t (*add)(___notnull command_event_generator_t *, ___notnull command_event_subscription_t *);
    // int32_t (*del)(___notnull command_event_generator_t *, ___notnull command_event_subscription_t *);

static command_event_generator_func_t func = {
    command_event_generator_func_rem
};

extern command_event_generator_t * command_event_generator_gen(___notnull event_generator_set_t * set) {
#ifndef   RELEASE
    snorlaxdbg(set == nil, "critical", "");
    snorlaxdbg(set->engine == nil, "critical", "");
    snorlaxdbg(set->engine->pool == nil, "critical", "");
#endif // RELEASE

    command_event_generator_t * generator = (command_event_generator_t *) calloc(1, sizeof(command_event_generator_t));

    generator->func = address_of(func);
    generator->set = set;

    if(set->engine->pool->size > 0) generator->sync = sync_gen();

    return generator;
}

static command_event_generator_t * command_event_generator_func_rem(___notnull command_event_generator_t * generator) {
    
}



// struct command_event_generator {
//     command_event_generator_func_t * func;
//     sync_t * sync;
//     uint64_t size;
//     command_event_subscription_t * head;
//     command_event_subscription_t * tail;

//     event_generator_set_t * set;
// };

// struct command_event_generator_func {
//     command_event_generator_t * (*rem)(___notnull command_event_generator_t *);
//     int32_t (*on)(___notnull command_event_generator_t *);
//     int32_t (*off)(___notnull command_event_generator_t *);
//     int32_t (*pub)(___notnull command_event_generator_t *, event_queue_t *);
//     int32_t (*add)(___notnull command_event_generator_t *, ___notnull command_event_subscription_t *);
//     int32_t (*del)(___notnull command_event_generator_t *, ___notnull command_event_subscription_t *);
// };

// extern command_event_generator_t * command_event_generator_gen(___notnull event_generator_set_t * set);

// #define command_event_generator_rem(generator)                  ((generator)->func->rem(generator))
// #define command_event_generator_on(generator)                   ((generator)->func->on(generator))
// #define command_event_generator_off(generator)                  ((generator)->func->off(generator))
// #define command_event_generator_pub(generator, queue)           ((generator)->func->pub(generator, queue))
// #define command_event_generator_add(generator, subscription)    ((generator)->func->add(generator, subscription))
// #define command_event_generator_del(generator, subscription)    ((generator)->func->del(generator, subscription))