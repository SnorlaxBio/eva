/**
 * @file            snorlax/descriptor/event/subscription/action.c
 * @brief
 * @details
 * 
 * @author          snorlax <ceo@snorlax.bio>
 * @since           June 20, 2024
 */

#include "action.h"

#include "../subscription.h"
#include "../../action.h"
#include "../../state.h"
#include "../../exception.h"

static void descriptor_event_subscription_action_func_open(___notnull descriptor_event_subscription_t * subscription, uint64_t param);
static void descriptor_event_subscription_action_func_in(___notnull descriptor_event_subscription_t * subscription, uint64_t param);
static void descriptor_event_subscription_action_func_out(___notnull descriptor_event_subscription_t * subscription, uint64_t param);
static void descriptor_event_subscription_action_func_close(___notnull descriptor_event_subscription_t * subscription, uint64_t param);
static void descriptor_event_subscription_action_func_exception(___notnull descriptor_event_subscription_t * subscription, uint64_t param);

static descriptor_event_subscription_action_t action[descriptor_action_max] = {
    descriptor_event_subscription_action_func_open,
    descriptor_event_subscription_action_func_in,
    descriptor_event_subscription_action_func_out,
    descriptor_event_subscription_action_func_close,
    descriptor_event_subscription_action_func_exception
};

extern descriptor_event_subscription_action_t descriptor_event_subscription_action_get(uint32_t type) {
#ifndef   RELEASE
    snorlaxdbg(descriptor_action_max <= type, "critical", "");
#endif // RELEASE

    return action[type];
}

static void descriptor_event_subscription_action_func_open(___notnull descriptor_event_subscription_t * subscription, uint64_t param) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, "critical", "");
#endif // RELEASE
    object_lock(subscription);

    descriptor_t * descriptor = subscription->descriptor;

    descriptor_exception_t * exception = descriptor_exception_check(address_of(descriptor->exception));

    if(exception == nil && !descriptor_state_has(descriptor, descriptor_state_close)) {
        if(descriptor->value <= invalid) {
            if(descriptor_open(descriptor) == success) {
                if(descriptor_state_has(descriptor, descriptor_state_open)) {
                    descriptor_event_subscription_on(subscription, descriptor_action_open, 0);
                } else {
                    descriptor_event_subscription_reg(subscription);
                }
            } else {
                descriptor_event_subscription_on(subscription, descriptor_action_exception, (uint64_t) descriptor_exception_check(address_of(descriptor->exception)));
            }
        } else {
            if(descriptor_state_has(descriptor, descriptor_state_open)) {
#ifndef   RELEASE
                snorlaxdbg(false, "caution", "descriptor already open");
#endif // RELEASE
            } else {
                snorlaxdbg(true, "implement", "open check");
            }
        }
    } else {
#ifndef   RELEASE
        snorlaxdbg(false, "caution", "descriptor not avail to open");
#endif // RELEASE
    }

    object_unlock(subscription);
}

static void descriptor_event_subscription_action_func_in(___notnull descriptor_event_subscription_t * subscription, uint64_t param);
static void descriptor_event_subscription_action_func_out(___notnull descriptor_event_subscription_t * subscription, uint64_t param);
static void descriptor_event_subscription_action_func_close(___notnull descriptor_event_subscription_t * subscription, uint64_t param);
static void descriptor_event_subscription_action_func_exception(___notnull descriptor_event_subscription_t * subscription, uint64_t param);
