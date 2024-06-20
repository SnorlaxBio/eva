/**
 * @file        snorlax/event/engine/conf.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 18, 2024
 */

#include "conf.h"

static event_engine_conf_t conf = {
    { true },
    { true },
    0,
};

extern event_engine_conf_t * event_engine_conf_default_get(void) {
    return &conf;
}