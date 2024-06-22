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

int main(int argc, char ** argv) {
    snorlax_eva_on();

    command_t * command = command_gen(execute);

    event_subscription_t * subscription = snorlax_eva_command_sub(command, 0, nil);

    return snorlax_eva_run();
}

static int32_t execute(___notnull command_t * command) {
    printf("hello world\n");
}