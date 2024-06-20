/**
 * @file        snorlax/descriptor/event/generator/epoll.c
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 21, 2024
 */

#include "epoll.h"

static descriptor_event_generator_epoll_func_t func = {

};

extern descriptor_event_generator_epoll_t * descriptor_event_generator_epoll_gen(void) {
    descriptor_event_generator_epoll_t * generator = (descriptor_event_generator_epoll_t *) calloc(1, sizeof(descriptor_event_generator_epoll_t));

    generator->func = &func;

    return generator;
}