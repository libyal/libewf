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

#if !defined( _LIBEWF_NOTIFY_H )
#define _LIBEWF_NOTIFY_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include <stdio.h>

#include "libewf_extern.h"

#if defined( __cplusplus )
extern "C" {
#endif

extern int libewf_notify_verbose;

LIBEWF_EXTERN void libewf_set_notify_values(
                    FILE *stream,
                    int verbose );

void libewf_notify_printf(
      char *format,
      ... );

#define libewf_notify_verbose_printf \
	if( libewf_notify_verbose != 0 ) libewf_notify_printf

void libewf_notify_error_backtrace(
      liberror_error_t *error );

void libewf_notify_dump_data(
      void *data,
      size_t size );

#define libewf_notify_verbose_dump_data \
	if( libewf_notify_verbose != 0 ) libewf_notify_dump_data

#if defined( __cplusplus )
}
#endif

#endif

