/**
 * @file        snorlax/console/main.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 27, 2024
 */

#include <string.h>

#include <snorlax/eva.h>
#include <snorlax/console/in.h>
#include <snorlax/console/out.h>

static void cancel(const event_engine_t * engine);

console_in_t * consolein = nil;
console_out_t * consoleout = nil;

event_subscription_t * subscription[2];

static void on(___notnull descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node);


int main(int argc, char ** argv) {
    snorlax_eva_on();

    consolein = console_in_gen();
    consoleout = console_out_gen();

    console_in_event_subscription_handler_t handler[descriptor_event_type_max] = {
        on,
        on,
        on,
        on,
        on
    };

    subscription[0] = (event_subscription_t *) snorlax_eva_descriptor_sub((descriptor_t *) consolein, handler, nil);
    subscription[1] = (event_subscription_t *) snorlax_eva_descriptor_sub((descriptor_t *) consoleout, handler, nil);

    return snorlax_eva_run();
}

static void on(descriptor_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * ret) {
    if(type == descriptor_event_type_read) {
        buffer_t * in = snorlax_eva_descriptor_buffer_in_get(subscription);
        if(strncmp(buffer_node_front(buffer_front(in)), "cancel\n", 7) == 0) {
            snorlax_eva_off(cancel);
        }
    }
}

static void cancel(const event_engine_t * engine) {
    subscription[0] = (event_subscription_t *) snorlax_eva_subscription_rem(subscription[0]);
    subscription[1] = (event_subscription_t *) snorlax_eva_subscription_rem(subscription[1]);

    console_in_rem(consolein);
    console_out_rem(consoleout);
}