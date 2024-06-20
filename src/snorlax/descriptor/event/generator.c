/**
 * @file            snorlax/descriptor/event/generator.c
 * @brief
 * @details
 * 
 * @author          snorlax <ceo@snorlax.bio>
 * @since           June 20, 2024
 */

#include "generator.h"

#include "generator/epoll.h"

extern descriptor_event_generator_t * descriptor_event_generator_gen(void) {
    return (descriptor_event_generator_t *) descriptor_event_generator_epoll_gen();
}
