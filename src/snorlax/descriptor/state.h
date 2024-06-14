/**
 * @file        snorlax/descriptor/state.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 14, 2024
 */

#ifndef   __SNORLAX__DESCRIPTOR_STATE__H__
#define   __SNORLAX__DESCRIPTOR_STATE__H__

#define descriptor_state_writable           (0x00000001u <<  0u)
#define descriptor_state_readable           (0x00000001u <<  1u)
#define descriptor_state_wait_in            (0x00000001u <<  2u)
#define descriptor_state_wait_out           (0x00000001u <<  3u)

#endif // __SNORLAX__DESCRIPTOR_STATE__H__
