/**
 * @file        snorlax/eva.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 14, 2024
 */

#ifndef   __SNORLAX__EVA__H__
#define   __SNORLAX__EVA__H__

#include <snorlax.h>

struct event;

struct event_object;
struct event_engine;
struct event_param;

struct event_handler_array;

typedef struct event event_t;
typedef struct event_object event_object_t;
typedef struct event_engine event_engine_t;
typedef struct event_param event_param_t;
typedef struct event_handler_array event_handler_array_t;

typedef void (*event_engine_cancel_t)(event_engine_t *);

typedef int (*event_handler_t)(event_object_t *, event_param_t *);

extern int event_engine_run(void);
extern void event_engine_cancel(event_engine_cancel_t cancel);
extern event_object_t * event_engine_object_add(event_object_t * o);
extern event_object_t * event_engine_object_del(event_object_t * o);

#endif // __SNORLAX__EVA__H__
