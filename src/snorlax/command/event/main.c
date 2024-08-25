/**
 * @file        snorlax/command/event/main.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 22, 2024
 */

#include <stdio.h>

#include <snorlax/eva.h>

static int32_t execute(___notnull command_t * command);
static void cancel(const event_engine_t * engine);
static void on(command_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * ret);

command_t * command = nil;
event_subscription_t * subscription = nil;


int main(int argc, char ** argv) {
    snorlax_eva_on();

    command = command_gen(execute);

    command_event_subscription_handler_t handler[2] = { on, on };

    subscription = snorlax_eva_command_sub(command, 5, handler);

    return snorlax_eva_run();
}

static int32_t execute(___notnull command_t * command) {
    printf("hello world\n");

    return success;
}

static void cancel(const event_engine_t * engine) {
    subscription = (event_subscription_t *) snorlax_eva_subscription_rem(subscription);
    command = command_rem(command);
}

static void on(command_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * ret) {
    if(type == 0 && ret == (event_subscription_event_t *) 2) {
        snorlax_eva_off(cancel);
    }
}