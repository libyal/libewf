/*
 * Error functions
 *
 * Copyright (c) 2008, Joachim Metz <forensics@hoffmannbv.nl>,
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

#if !defined( _LIBEWF_INTERNAL_ERROR_H )
#define _LIBEWF_INTERNAL_ERROR_H

#include <common.h>
#include <types.h>

#include <stdio.h>

#if defined( HAVE_STDARG_H )
#include <stdarg.h>
#elif defined( HAVE_VARARGS_H )
#include <varargs.h>
#else
#error Missing headers stdarg.h and varargs.h
#endif

#include <libewf/error.h>

#include "libewf_extern.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_internal_error libewf_internal_error_t;

struct libewf_internal_error
{
	/* The error domain
	 */
	int domain;

	/* The error code
	 */
	int code;

	/* The amount of messages
	 */
	int amount_of_messages;

	/* The error messages
	 */
	char **message;
};

void libewf_error_set(
      libewf_error_t **error,
      int error_domain,
      int error_code,
      const char *format,
      ... );

LIBEWF_EXTERN void libewf_error_free(
                    libewf_error_t **error );

LIBEWF_EXTERN int libewf_error_matches(
                   libewf_error_t *error,
                   int error_domain,
                   int error_code );

LIBEWF_EXTERN void libewf_error_fprint(
                    libewf_error_t *error,
                    FILE *stream );

LIBEWF_EXTERN void libewf_error_backtrace_fprint(
                    libewf_error_t *error,
                    FILE *stream );

void libewf_error_backtrace_notify(
      libewf_error_t *error );

#if defined( __cplusplus )
}
#endif

#endif

