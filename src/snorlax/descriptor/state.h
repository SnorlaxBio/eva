/**
 * @file        snorlax/descriptor/state.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 20, 2024
 */

#ifndef   __SNORLAX__DESCRIPTOR_STATE__H__
#define   __SNORLAX__DESCRIPTOR_STATE__H__

#include <snorlax/descriptor.h>

#define descriptor_state_gen                        (0x00000001U <<  0U)
#define descriptor_state_open                       (0x00000001U <<  1U)
#define descriptor_state_in                         (0x00000001U <<  2U)
#define descriptor_state_out                        (0x00000001U <<  3U)
#define descriptor_state_close                      (0x00000001U <<  4U)
#define descriptor_state_exception                  (0x00000001U <<  5U)

#define descriptor_state_del(descriptor, v)         ((descriptor)->status & (~(v)))
#define descriptor_state_add(descriptor, v)         ((descriptor)->status | (v))
#define descriptor_state_set(descriptor, v)         ((descriptor)->status = (v))
#define descriptor_state_has(descriptor, v)         ((descriptor)->status & v)
#define descriptor_state_is(descriptor, v)          ((descriptor)->status == v)

#endif // __SNORLAX__DESCRIPTOR_STATE__H__
