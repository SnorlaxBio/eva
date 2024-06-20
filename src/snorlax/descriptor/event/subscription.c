/**
 * @file        snorlax/descriptor/event/subscription.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 21, 2024
 */

#include "subscription.h"
#include "subscription/handler.h"

static descriptor_event_subscription_t * descriptor_event_subscription_func_rem(___notnull descriptor_event_subscription_t * subscription);
static void descriptor_event_subscription_func_on(___notnull descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param);
static int32_t descriptor_event_subscription_func_reg(___notnull descriptor_event_subscription_t * subscription);
static int32_t descriptor_event_subscription_func_unreg(___notnull descriptor_event_subscription_t * subscription);

static descriptor_event_subscription_func_t func = {
    descriptor_event_subscription_func_rem,
    descriptor_event_subscription_func_on,
    descriptor_event_subscription_func_reg,
    descriptor_event_subscription_func_unreg
};

static descriptor_event_subscription_t * descriptor_event_subscription_func_rem(___notnull descriptor_event_subscription_t * subscription) {
    
    subscription->sync = sync_rem(subscription->sync);

    free(subscription);

    return nil;
}

static void descriptor_event_subscription_func_on(___notnull descriptor_event_subscription_t * subscription, uint32_t type, uint64_t param) {
    descriptor_event_subscription_handler_t handler = subscription->handler[type];

    if(handler == nil) handler = descriptor_event_subscription_handler_get(type);

    handler(subscription, type, param);
}