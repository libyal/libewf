/*
 * Notification function
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

#if !defined( _NOTIFY_H )
#define _NOTIFY_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include <stdio.h>

#if defined( __cplusplus )
extern "C" {
#endif

extern int notify_verbose;

void notify_set_values(
      FILE *stream,
      int verbose );

void notify_printf(
      char *format,
      ... );

#define notify_verbose_printf \
	if( notify_verbose != 0 ) notify_printf

#define notify_warning_printf \
	if( notify_verbose != 0 ) notify_printf

void notify_error_backtrace(
      liberror_error_t *error );

#if defined( __cplusplus )
}
#endif

#endif

