/*
 * Platform functions
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
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#include <liberror.h>

#if defined( HAVE_SYS_UTSNAME_H )
#include <sys/utsname.h>
#endif

#include "system_string.h"

#if !defined( LIBEWF_OPERATING_SYSTEM )
#define LIBEWF_OPERATING_SYSTEM "Unknown"
#endif

/* Determines the operating system string
 * Return 1 if successful or -1 on error
 */
int platform_get_operating_system(
     system_character_t *operating_system_string,
     size_t operating_system_string_size,
     liberror_error_t **error )
{
#if defined( HAVE_UNAME ) && !defined( WINAPI )
	struct utsname utsname_buffer;
#endif

	char *operating_system         = NULL;
	static char *function          = "platform_get_operating_system";
	size_t operating_system_length = 0;

	if( operating_system_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid operating system string.",
		 function );

		return( -1 );
	}
#if defined( WINAPI )
	operating_system = "Windows";

#elif defined( HAVE_UNAME )
	/* Determine the operating system
	 */
	if( uname(
	     &utsname_buffer ) == 0 )
	{
		operating_system = utsname_buffer.sysname;
	}
	else
	{
		operating_system = "Undetermined";
	}
#else
	/* Have configure determine the operating system
	 */
	operating_system = LIBEWF_OPERATING_SYSTEM;
#endif

	operating_system_length = narrow_string_length(
	                           operating_system );

	if( operating_system_string_size < ( operating_system_length + 1 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: operating system string too small.",
		 function );

		return( -1 );
	}	
	if( system_string_copy_from_utf8_string(
	     operating_system_string,
	     operating_system_string_size,
	     (uint8_t *) operating_system,
	     operating_system_length + 1,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set operating system string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

