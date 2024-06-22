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

command_t * command = nil;
event_subscription_t * subscription = nil;

int main(int argc, char ** argv) {
    snorlax_eva_on();

    command = command_gen(execute);

    subscription = snorlax_eva_command_sub(command, 0, nil);

    return snorlax_eva_run();
}

static int32_t execute(___notnull command_t * command) {
    printf("hello world\n");
    snorlax_eva_off(cancel);
}

static void cancel(const event_engine_t * engine) {
    printf("cancel\n");

    subscription = (event_subscription_t *) snorlax_eva_subscription_rem(subscription);
    command = command_rem(command);
}