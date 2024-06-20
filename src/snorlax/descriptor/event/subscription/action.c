/**
 * @file        snorlax/descriptor/event/subscription/action.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 20, 2024
 */

#include <unistd.h>
#include <errno.h>

#include "action.h"

#include "../subscription.h"
#include "../generator.h"

/**
 * @var         handler[descriptor_action_max]
 */
static descriptor_event_action_t handler[descriptor_action_max] = {
    descriptor_event_subscription_action_func_open,
    descriptor_event_subscription_action_func_in,
    descriptor_event_subscription_action_func_out,
    descriptor_event_subscription_action_func_close,
    descriptor_event_subscription_action_func_exception
};

extern descriptor_event_action_t descriptor_event_subscription_action_get(uint32_t type) {
#ifndef   RELEASE
    snorlaxdbg(descriptor_action_max <= type, "critical", "");
#endif // RELEASE

    return handler[type];
}

extern int32_t descriptor_event_subscription_action_func_open(___notnull descriptor_event_subscription_t * subscription, uint64_t param) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, "critical", "");
    snorlaxdbg(subscription->descriptor == nil, "critical", "");
#endif // RELEASE
    descriptor_t * descriptor = subscription->descriptor;
    if(descriptor->value <= invalid) {
        descriptor->value = descriptor_open(descriptor);    // 서브스크립션의 예외 핸들링을 하지 않는다.
        if(descriptor->value > invalid) {
            if((descriptor->status & descriptor_state_open) == 0) descriptor_event_subscription_handler_func_on(subscription, descriptor_action_open, 0);

            if(descriptor_event_generator_func_reg(subscription->generator, subscription) == fail) {
                snorlaxdbg(true, "unknown", "check this logic");
            }

            return success;
        } else {
            descriptor_event_subscription_handler_func_on(subscription, descriptor_action_exception, (uint64_t) descriptor_exception_get(descriptor));
        }
        return fail;
    } else {
        if(descriptor->status & descriptor_state_open) {
            snorlaxdbg(true, "implement", "check open status and implement");
        } else {
            snorlaxdbg(false, "notice", "already open");
        }
        return success;
    }
}

extern int32_t descriptor_event_subscription_action_func_in(___notnull descriptor_event_subscription_t * subscription, uint64_t param) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, "critical", "");
    snorlaxdbg(subscription->descriptor == nil, "critical", "");
    snorlaxdbg(subscription->descriptor->buffer.in == nil, "critical", "");
    snorlaxdbg(subscription->descriptor->value <= invalid, "critical", "");
#endif // RELEASE
    int32_t ret = success;

    descriptor_t * descriptor = subscription->descriptor;
    descriptor_event_generator_t * generator = subscription->generator;

    int32_t retry = generator->retry;

    do {
        int64_t n = descriptor_read(descriptor);

        if(n > 0) {
            descriptor_event_subscription_handler_func_on(subscription, descriptor_action_in, n);

            if(descriptor_exception_get(descriptor)){
                ret = fail;
                break;
            }

            retry = retry - 1;
        } else if(n == 0) {
            if(descriptor_event_generator_func_reg(generator, subscription) == fail) {
                ret = fail;
                snorlaxdbg(true, "unknown", "check this logic");
            }
            break;
        } else {
            descriptor_event_subscription_handler_func_on(subscription, descriptor_action_exception, (uint64_t) descriptor_exception_get(descriptor));
            ret = fail;
            break;
        }
    } while(retry > 0);

    return ret;
}

/**
 * @fn          extern int32_t descriptor_event_subscription_action_func_out(___notnull descriptor_event_subscription_t * subscription, uint64_t param)
 */
extern int32_t descriptor_event_subscription_action_func_out(___notnull descriptor_event_subscription_t * subscription, uint64_t param) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, "critical", "");
    snorlaxdbg(subscription->descriptor == nil, "critical", "");
    snorlaxdbg(subscription->descriptor->buffer.out == nil, "critical", "");
    snorlaxdbg(subscription->descriptor->value <= invalid, "critical", "");
#endif // RELEASE
    int32_t ret = success;

    descriptor_t * descriptor = subscription->descriptor;
    descriptor_event_generator_t * generator = subscription->generator;
    buffer_t * buffer = descriptor->buffer.out;

    while(buffer_length(buffer) > 0) {
        int64_t n = descriptor_write(descriptor);
        if(n > 0) {
            descriptor_event_subscription_handler_func_on(subscription, descriptor_action_out, n);

            if(descriptor_exception_get(descriptor)){
                ret = fail;
                break;
            }

            if(buffer_position_get(buffer) == buffer_size_get(buffer)) buffer_reset(buffer, 0);
        } else if(n == 0) {
            if(descriptor_event_generator_func_reg(generator, subscription) == fail) {
                ret = fail;
                snorlaxdbg(true, "unknown", "check this logic");
            }
            break;
        } else {
            ret = fail;
        }
    }

    return ret;
}

/**
 * @fn          extern int32_t descriptor_event_subscription_action_func_close(___notnull descriptor_event_subscription_t * subscription, uint64_t param)
 * @brief       디스크립터 이벤트 서브스크립션 객체의 디스크립터 닫기를 수행한다.
 * @details     디스크립터가 닫히면 디스크립터와 서브스크립션 객체에 등록된 상태 변수들을 초기화한다.
 */
extern int32_t descriptor_event_subscription_action_func_close(___notnull descriptor_event_subscription_t * subscription, uint64_t param) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, "critical", "");
    snorlaxdbg(subscription->descriptor == nil, "critical", "");
#endif // RELEASE
    descriptor_t * descriptor = subscription->descriptor;
    descriptor_event_generator_t * generator = subscription->generator;

    if(descriptor->value > invalid) {
        descriptor_event_generator_func_unreg(generator, subscription);

        int32_t value = descriptor->value;

        descriptor_close(descriptor);

        descriptor_event_subscription_handler_func_on(subscription, descriptor_action_close, value);

        snorlaxdbg(false, "check", "reopen");

        buffer_reset(descriptor->buffer.in, 0);
        buffer_reset(descriptor->buffer.out, 0);

        event_subscription_event_queue_clear(subscription->queue, nil);

        descriptor_exception_clear(address_of(descriptor->exception));
    } else {
        snorlaxdbg(false, "notice", "already close");
    }

    return success;
}

/**
 * @fn          extern int32_t descriptor_event_subscription_action_func_exception(___notnull descriptor_event_subscription_t * subscription, uint64_t param)
 * @brief       예외가 발생한 디스크립터 이벤트 서브스크립션 객체의 예외를 출력하고 디스크립터 닫기를 수행한다.
 * @details     
 */
extern int32_t descriptor_event_subscription_action_func_exception(___notnull descriptor_event_subscription_t * subscription, uint64_t param) {
#ifndef   RELEASE
    snorlaxdbg(subscription == nil, "critical", "");
    snorlaxdbg(subscription->descriptor == nil, "critical", "");
#endif // RELEASE
    descriptor_t * descriptor = subscription->descriptor;

    descriptor_exception_t * exception = descriptor_exception_get(descriptor);

#ifndef   RELEASE
    snorlaxdbg(exception == nil, "critical", "");
#endif // RELEASE
    snorlaxdbg(false, "exception", "descriptor exception => %d:%p:%d", exception->type, exception->func, exception->no);

    descriptor_event_subscription_handler_func_on(subscription, descriptor_action_exception, (uint64_t) exception);

    descriptor_event_subscription_action_func_close(subscription, 0);

    return success;
}