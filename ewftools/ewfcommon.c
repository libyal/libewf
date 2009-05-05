/*
 * Common functions for the ewftools
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
#include <endian.h>
#include <memory.h>
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#include <liberror.h>

#include <errno.h>

#if defined( HAVE_STDLIB_H )
#include <stdlib.h>
#endif

#if defined( HAVE_STRING_H )
#include <string.h>
#endif

#if defined( HAVE_SYS_UTSNAME_H )
#include <sys/utsname.h>
#endif

#if defined( WINAPI )
#include <rpcdce.h>

#elif defined( HAVE_UUID_UUID_H )
#include <uuid/uuid.h>
#endif

/* If libtool DLL support is enabled set LIBEWF_DLL_IMPORT
 * before including libewf.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBEWF_DLL_IMPORT
#endif

#include <libewf.h>

#include "ewfcommon.h"
#include "file_io.h"
#include "notify.h"
#include "system_string.h"

#if !defined( LIBEWF_OPERATING_SYSTEM )
#define LIBEWF_OPERATING_SYSTEM "Unknown"
#endif

/* Swaps the byte order of byte pairs within a buffer of a certain size
 * Returns 1 if successful, -1 on error
 */
int ewfcommon_swap_byte_pairs(
     uint8_t *buffer,
     size_t size )
{
	static char *function = "ewfcommon_swap_byte_pairs";
	uint8_t byte          = 0;
	size_t iterator       = 0;

	if( buffer == NULL )
	{
		notify_warning_printf( "%s: invalid buffer.\n",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	for( iterator = 0; iterator < size; iterator += 2 )
	{
		byte                   = buffer[ iterator ];
		buffer[ iterator ]     = buffer[ iterator + 1 ];
		buffer[ iterator + 1 ] = byte;
	}
	return( 1 );
}

/* Determines the operating system string
 * Return 1 if successful or -1 on error
 */
int ewfcommon_determine_operating_system_string(
     system_character_t *operating_system_string,
     size_t operating_system_string_size,
     liberror_error_t **error )
{
#if defined( HAVE_SYS_UTSNAME_H )
	struct utsname utsname_buffer;
#endif

	char *operating_system         = NULL;
	static char *function          = "ewfcommon_determine_operating_system_string";
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
#if defined( HAVE_SYS_UTSNAME_H )
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

/* Determines the GUID
 * Returns 1 if successful or -1 on error
 */
int ewfcommon_determine_guid(
     uint8_t *guid,
     uint8_t libewf_format,
     liberror_error_t **error )
{
#if defined( WINAPI )
	UUID uuid             = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };
#endif

	static char *function = "ewfcommon_determine_guid";

	if( guid == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid GUID.",
		 function );

		return( -1 );
	}
	if( ( libewf_format == LIBEWF_FORMAT_ENCASE5 )
	 || ( libewf_format == LIBEWF_FORMAT_ENCASE6 )
	 || ( libewf_format == LIBEWF_FORMAT_EWFX ) )
	{
#if defined( WINAPI )
		UuidCreate(
		 &uuid );

#elif defined( HAVE_UUID_GENERATE_RANDOM )
		uuid_generate_random(
		 guid );
#endif
	}
	if( ( libewf_format == LIBEWF_FORMAT_LINEN5 )
	 || ( libewf_format == LIBEWF_FORMAT_LINEN6 ) )
	{
#if defined( WINAPI )
		UuidCreateSequential(
		 &uuid );

#elif defined( HAVE_UUID_GENERATE_TIME )
		uuid_generate_time(
		 guid );
#endif
	}
#if defined( WINAPI )
	endian_little_revert_32bit(
	 guid,
	 uuid.Data1 );

	guid += 4;

	endian_little_revert_16bit(
	 guid,
	 uuid.Data2 );

	guid += 2;

	endian_little_revert_16bit(
	 guid,
	 uuid.Data3 );

	guid += 2;

	guid[ 0 ] = uuid.Data4[ 0 ];
	guid[ 1 ] = uuid.Data4[ 1 ];
	guid[ 2 ] = uuid.Data4[ 2 ];
	guid[ 3 ] = uuid.Data4[ 3 ];
	guid[ 4 ] = uuid.Data4[ 4 ];
	guid[ 5 ] = uuid.Data4[ 5 ];
	guid[ 6 ] = uuid.Data4[ 6 ];
	guid[ 7 ] = uuid.Data4[ 7 ];
#endif
	return( 1 );
}

