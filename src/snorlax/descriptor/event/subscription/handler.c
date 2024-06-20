/**
 * @file        snorlax/descriptor/event/subscription/handler.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 20, 2024
 */

#include "handler.h"

#include "../subscription.h"
#include "../../action.h"

static void descriptor_event_subscription_handler_func_open(___notnull descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param);
static void descriptor_event_subscription_handler_func_in(___notnull descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param);
static void descriptor_event_subscription_handler_func_out(___notnull descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param);
static void descriptor_event_subscription_handler_func_close(___notnull descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param);
static void descriptor_event_subscription_handler_func_exception(___notnull descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param);

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

static void descriptor_event_subscription_handler_func_open(___notnull descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, "critical", "");
    snorlaxdbg(subscription->descriptor == nil, "critical", "");
#endif // RELEASE
}

static void descriptor_event_subscription_handler_func_in(___notnull descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, "critical", "");
    snorlaxdbg(subscription->descriptor == nil, "critical", "");
    snorlaxdbg(subscription->descriptor->buffer.in == nil, "critical", "");
#endif // RELEASE

    buffer_t * buffer = subscription->descriptor->buffer.in;

    buffer_reset(buffer, 0);
}

static void descriptor_event_subscription_handler_func_out(___notnull descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, "critical", "");
    snorlaxdbg(subscription->descriptor == nil, "critical", "");
    snorlaxdbg(subscription->descriptor->buffer.out == nil, "critical", "");
#endif // RELEASE

    buffer_t * buffer = subscription->descriptor->buffer.out;

    if(buffer_position_get(buffer) == buffer_size_get(buffer)) buffer_reset(buffer, 0);
}

static void descriptor_event_subscription_handler_func_close(___notnull descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, "critical", "");
    snorlaxdbg(subscription->descriptor == nil, "critical", "");
#endif // RELEASE
}

static void descriptor_event_subscription_handler_func_exception(___notnull descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, "critical", "");
    snorlaxdbg(subscription->descriptor == nil, "critical", "");
#endif // RELEASE
    descriptor_exception_t * exception = descriptor_exception_check(subscription->descriptor);
    if(exception) {
        snorlaxdbg(false, "exception", "%d:%p:%d", exception->type, exception->func, exception->no);
    }
}
