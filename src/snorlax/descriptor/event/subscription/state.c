/**
 * @file        snorlax/descriptor/event/subscription/state.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 20, 2024
 */

#include "state.h"

static uint32_t state[descriptor_action_max] = {
    descriptor_state_open,
    descriptor_state_in,
    descriptor_state_out,
    descriptor_state_close,
    descriptor_state_exception,
};

extern uint32_t descriptor_event_subscription_state_get(uint32_t action) {
#ifndef   RELEASE
    snorlaxdbg(descriptor_action_max <= action, "critical", "");
#endif // RELEASE

    return state[action];
}