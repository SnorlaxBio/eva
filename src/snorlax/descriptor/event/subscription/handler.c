/**
 * @file        snorlax/descriptor/event/handler.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 20, 2024
 */

#include "handler.h"

#include "../subscription.h"

static descriptor_event_subscription_handler_t handler[descriptor_action_max] = {
    descriptor_event_subscription_handler_func_open,
    descriptor_event_subscription_handler_func_in,
    descriptor_event_subscription_handler_func_out,
    descriptor_event_subscription_handler_func_close,
    descriptor_event_subscription_handler_func_exception
};

extern descriptor_event_subscription_handler_t descriptor_event_subscription_handler_get(uint32_t type) {
#ifndef   RELEASE
    snorlaxdbg(descriptor_action_max <= type, "critical", "");
#endif // RELEASE

    return handler[type];
}

extern void descriptor_event_subscription_handler_func_on(___notnull descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, "critical", "");
    snorlaxdbg(subscription->handler == nil, "critical", "");
    snorlaxdbg(descriptor_action_max <= type, "critical", "");
#endif // RELEASE

    descriptor_event_subscription_handler_t on = subscription->handler[type];

    if(on == nil) on = descriptor_event_subscription_handler_get(type);

#ifndef   RELEASE
    on(subscription, type, param);
#endif // RELEASE
}

extern void descriptor_event_subscription_handler_func_open(___notnull descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param) {

}

extern void descriptor_event_subscription_handler_func_in(___notnull descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param) {

}

extern void descriptor_event_subscription_handler_func_out(___notnull descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param) {

}

extern void descriptor_event_subscription_handler_func_close(___notnull descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param) {

}

extern void descriptor_event_subscription_handler_func_exception(___notnull descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param) {

}