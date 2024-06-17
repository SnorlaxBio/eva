/**
 * @file        snorlax/command/event/check.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 18, 2024
 */

#include <stdlib.h>
#include <stdio.h>

#include <snorlax/eva.h>
#include <snorlax/command.h>

static int32_t exec(command_t * command);

int main(int argc, char ** argv) {
    command_event_subscription_t * subscription = event_engine_command_add(command_gen(exec), command_event_subscription_state_once);

    int32_t ret = event_engine_run();

    free(command_event_subscription_command_get(subscription));
    subscription = command_event_subscription_rem(subscription);

    return ret;
}

static int32_t exec(command_t * command) {
    printf("hello world\n");

    return success;
}