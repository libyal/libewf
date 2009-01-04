/*
 * Support functions
 *
 * Copyright (c) 2006-2008, Joachim Metz <forensics@hoffmannbv.nl>,
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
#include <character_string.h>
#include <notify.h>

#include "libewf_definitions.h"
#include "libewf_handle.h"
#include "libewf_support.h"

/* Returns the library version as a string
 */
const character_t *libewf_get_version(
                    void )
{
	return( (const character_t *) LIBEWF_VERSION_STRING );
}

/* Signals the libewf handle to abort its current activity
 * Returns 1 if successful or -1 on error
 */
int libewf_signal_abort(
     libewf_handle_t *handle )
{
	static char *function = "libewf_signal_abort";

	if( handle == NULL )
	{
		notify_warning_printf( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	( (libewf_internal_handle_t *) handle )->abort = 1;

	return( 1 );
}

/* Set the notify values
 */
void libewf_set_notify_values(
      FILE *stream,
      uint8_t verbose )
{
	notify_set_values(
	 stream,
	 verbose );
}

