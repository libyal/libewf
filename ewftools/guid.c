/*
 * GUID functions
 *
 * Copyright (c) 2006-2013, Joachim Metz <joachim.metz@gmail.com>
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
#include <byte_stream.h>
#include <types.h>

#if defined( WINAPI )
#include <rpcdce.h>

#elif defined( HAVE_UUID_UUID_H )
#include <uuid/uuid.h>
#endif

#include "ewftools_libcerror.h"
#include "ewftools_libcstring.h"
#include "guid.h"

#if defined( HAVE_GUID_SUPPORT ) || defined( WINAPI )

/* Determines the GUID
 * Returns 1 if successful or -1 on error
 */
int guid_generate(
     uint8_t *guid,
     size_t guid_size,
     uint8_t guid_type,
     libcerror_error_t **error )
{
#if defined( WINAPI )
	UUID uuid             = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };
#endif

	static char *function = "guid_generate";

	if( guid == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid GUID.",
		 function );

		return( -1 );
	}
	if( guid_size < GUID_SIZE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: GUID too small.",
		 function );

		return( -1 );
	}
	if( guid_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid GUID size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( guid_type != GUID_TYPE_RANDOM )
	 && ( guid_type != GUID_TYPE_TIME ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported GUID type.",
		 function );

		return( -1 );
	}
	if( guid_type == GUID_TYPE_RANDOM )
	{
#if defined( WINAPI )
		UuidCreate(
		 &uuid );

#elif defined( HAVE_UUID_GENERATE_RANDOM )
		uuid_generate_random(
		 guid );
#endif
	}
	if( guid_type == GUID_TYPE_TIME )
	{
#if defined( __BORLANDC__ ) && __BORLANDC__ <= 0x0520
		/* No support for the time type GUID */

#elif defined( WINAPI ) && _WIN32_WINNT >= 0x0500
		UuidCreateSequential(
		 &uuid );

#elif defined( HAVE_UUID_GENERATE_TIME )
		uuid_generate_time(
		 guid );
#endif
	}
#if defined( WINAPI )
	byte_stream_copy_from_uint32_little_endian(
	 guid,
	 uuid.Data1 );

	guid += 4;

	byte_stream_copy_from_uint16_little_endian(
	 guid,
	 uuid.Data2 );

	guid += 2;

	byte_stream_copy_from_uint16_little_endian(
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

#endif

/* Converts the GUID into a string
 * Returns 1 if successful or -1 on error
 */
int guid_to_string(
     uint8_t *guid,
     size_t guid_size,
     int byte_order,
     libcstring_system_character_t *string,
     size_t string_size,
     libcerror_error_t **error )
{
	static char *function = "guid_to_string";
	int print_count       = 0;

	if( guid == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid guid.",
		 function );

		return( -1 );
	}
	if( guid_size < GUID_SIZE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: GUID too small.",
		 function );

		return( -1 );
	}
	if( guid_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid GUID size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( byte_order != _BYTE_STREAM_ENDIAN_BIG )
	 && ( byte_order != _BYTE_STREAM_ENDIAN_LITTLE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported byte order.",
		 function );

		return( -1 );
	}
	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( string_size < GUID_STRING_SIZE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: string too small.",
		 function );

		return( -1 );
	}
	if( string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	/* Create the GUID string
	 * It is stored as uint32 - uint16 - uint16 - 8 byte array
	 */
	if( byte_order == _BYTE_STREAM_ENDIAN_BIG )
	{
		print_count = libcstring_system_string_sprintf(
			       string,
			       string_size,
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "-%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "-%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "-%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "-%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 ),
			       guid[ 0 ], guid[ 1 ], guid[ 2 ], guid[ 3 ],
			       guid[ 4 ], guid[ 5 ],
			       guid[ 6 ], guid[ 7 ],
			       guid[ 8 ], guid[ 9 ],
			       guid[ 10 ], guid[ 11 ], guid[ 12 ], guid[ 13 ], guid[ 14 ], guid[ 15 ] );
	}
	else if( byte_order == _BYTE_STREAM_ENDIAN_LITTLE )
	{
		print_count = libcstring_system_string_sprintf(
			       string,
			       string_size,
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "-%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "-%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "-%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "-%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 ),
			       guid[ 3 ], guid[ 2 ], guid[ 1 ], guid[ 0 ],
			       guid[ 5 ], guid[ 4 ],
			       guid[ 7 ], guid[ 6 ],
			       guid[ 8 ], guid[ 9 ],
			       guid[ 10 ], guid[ 11 ], guid[ 12 ], guid[ 13 ], guid[ 14 ], guid[ 15 ] );
	}

	if( ( print_count < 0 )
	 || ( (size_t) print_count > string_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

