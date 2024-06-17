/**
 * @file        snorlax/command.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 18, 2024
 */

#include <stdlib.h>

#include "command.h"

extern command_t * command_gen(command_executor_t exec) {
    command_t * command = (command_t *) calloc(1, sizeof(command_t));

    command->exec = exec;

    return command;
}

extern command_t * command_func_rem(command_t * command) {
    if(command) free(command);
    return nil;
}
