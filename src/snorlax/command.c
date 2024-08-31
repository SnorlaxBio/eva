/**
 * @file        snorlax/command.c
 * @brief
 * @details
 * 
 * @author      snorlax <opuntia@snorlax.bio>
 * @since       June 22, 2024
 */

#include <stdlib.h>

#include "command.h"

static command_t * command_func_rem(___notnull command_t * command);

static command_func_t func = {
    command_func_rem
};

extern command_t * command_gen(command_execute_t execute) {
#ifndef   RELEASE
    snorlaxdbg(execute == nil, false, "critical", "");
#endif // RELEASE

    command_t * command = (command_t *) calloc(1, sizeof(command_t));

    command->func = address_of(func);

    command->execute = execute;

    return command;
}

static command_t * command_func_rem(___notnull command_t * command) {
#ifndef   RELEASE
    snorlaxdbg(command == nil, false, "critical", "");
#endif // RELEASE

    command->sync = sync_rem(command->sync);

    free(command);

    return nil;
}
