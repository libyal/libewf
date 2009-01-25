/*
 * Support functions
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

#include <common.h>
#include <types.h>

#include <liberror.h>

#include <stdio.h>

#include "libewf_definitions.h"
#include "libewf_error.h"
#include "libewf_handle.h"
#include "libewf_notify.h"
#include "libewf_support.h"

/* Returns the library version as a string
 */
const char *libewf_get_version(
             void )
{
	return( (const char *) LIBEWF_VERSION_STRING );
}

/* Signals the libewf handle to abort its current activity
 * Returns 1 if successful or -1 on error
 */
int libewf_signal_abort(
     libewf_handle_t *handle )
{
	liberror_error_t *error = NULL;
	static char *function   = "libewf_signal_abort";

	if( handle == NULL )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		libewf_notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( -1 );
	}
	( (libewf_internal_handle_t *) handle )->abort = 1;

	return( 1 );
}

/* Free an error and its elements
 */
void libewf_error_free(
      libewf_error_t **error )
{
	liberror_error_free(
	 (liberror_error_t **) error );
}

/* Prints a descriptive string of the error to the stream
 */
void libewf_error_fprint(
      libewf_error_t *error,
      FILE *stream )
{
	liberror_error_fprint(
	 (liberror_error_t *) error,
	 stream );
}

/* Prints a backtrace of the error to the stream
 */
void libewf_error_backtrace_fprint(
      libewf_error_t *error,
      FILE *stream )
{
	liberror_error_backtrace_fprint(
	 (liberror_error_t *) error,
	 stream );
}

