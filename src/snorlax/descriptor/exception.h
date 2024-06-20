/**
 * @file        snorlax/descriptor/exception.h
 * @brief
 * @details
 * 
 * @author      snorlax <ceo@snorlax.bio>
 * @since       June 20, 2024
 */

#ifndef   __SNORLAX__DESCRIPTOR_EXCEPTION__H__
#define   __SNORLAX__DESCRIPTOR_EXCEPTION__H__

#include <snorlax/descriptor.h>

#define descriptor_exception_check(exception)       ((exception)->type ? exception : nil)

#endif // __SNORLAX__DESCRIPTOR_EXCEPTION__H__
