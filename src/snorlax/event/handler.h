/**
 * @file        snorlax/event/handler.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 12, 2024
 */

#ifndef   __SNORLAX__EVENT_HANDLER__H__
#define   __SNORLAX__EVENT_HANDLER__H__

#include <snorlax.h>

struct event_object;
struct event_param;
struct event_engine;

typedef struct event_object event_object_t;
typedef struct event_param event_param_t;
typedef struct event_engine event_engine_t;

typedef int (*event_handler_t)(event_object_t *, event_param_t *, event_engine_t *);

#endif // __SNORLAX__EVENT_HANDLER__H__
