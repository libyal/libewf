/*
 * Signal handling functions
 *
 * Copyright (c) 2006-2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _EWFSIGNAL_H )
#define _EWFSIGNAL_H

#include <common.h>
#include <types.h>

#if defined( WINAPI )
#include <winnt.h>
#include <crtdbg.h>
#elif defined( HAVE_SIGNAL_H )
#include <signal.h>
#endif

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( WINAPI )
typedef unsigned long ewfsignal_t;

BOOL WINAPI ewfsignal_handler(
             ewfsignal_t signal );

void ewfsignal_initialize_memory_debug(
      void );

int ewfsignal_attach(
     void (*signal_handler)( ewfsignal_t ) );

int ewfsignal_detach(
     void );

#elif defined( HAVE_SIGNAL_H )
typedef int ewfsignal_t;

int ewfsignal_attach(
     void (*signal_handler)( ewfsignal_t ) );

int ewfsignal_detach(
     void );

#else
#error missing signal functions
#endif

#if defined( __cplusplus )
}
#endif

#endif

