/**
 * @file        snorlax/command.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 18, 2024
 */

#include "command.h"

extern command_t * command_func_rem(command_t * command) {
    if(command) free(command);
    return nil;
}
