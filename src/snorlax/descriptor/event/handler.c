/**
 * @file        snorlax/descriptor/event/handler.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 19, 2024
 */

#include "handler.h"

static void descriptor_event_handler_func_all(descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param);
static void descriptor_event_handler_func_open(descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param);
static void descriptor_event_handler_func_in(descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param);
static void descriptor_event_handler_func_out(descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param);
static void descriptor_event_handler_func_close(descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param);
static void descriptor_event_handler_func_exception(descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param);

static descriptor_event_handler_t descriptor_event_handler[descriptor_event_type_max] = {
    descriptor_event_handler_func_all,
    descriptor_event_handler_func_open,
    descriptor_event_handler_func_in,
    descriptor_event_handler_func_out,
    descriptor_event_handler_func_close,
    descriptor_event_handler_func_exception
};

extern descriptor_event_handler_t descriptor_event_handler_get(uint32_t type) {
    return type < descriptor_event_type_max ? descriptor_event_handler[type] : nil;
}

static void descriptor_event_handler_func_all(descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param) {
    // TODO: IMPLEMENT THIS
    return fail;
}

static void descriptor_event_handler_func_open(descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param) {
    // TODO: IMPLEMENT THIS
    return fail;
}

static void descriptor_event_handler_func_in(descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param) {
    // TODO: IMPLEMENT THIS
    return fail;
}

static void descriptor_event_handler_func_out(descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param) {
    // TODO: IMPLEMENT THIS
    return fail;
}

static void descriptor_event_handler_func_close(descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param) {
    // TODO: IMPLEMENT THIS
    return fail;
}

static void descriptor_event_handler_func_exception(descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param) {
    // TODO: IMPLEMENT THIS
    return fail;
}
