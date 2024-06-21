/**
 * @file        snorlax/event/subscription/event.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 21, 2024
 * @version     0.0.1
 */

#include <stdlib.h>

#include "event.h"

extern event_subscription_event_t * event_subscription_event_func_rem(___notnull event_subscription_event_t * event) {
#ifndef   RELEASE
    snorlaxdbg(event == nil, "critical", "");
    snorlaxdbg(event->queue, "critical", "");
#endif // RELEASE

    event->sync = sync_rem(event->sync);

    free(event);
    return nil;
}