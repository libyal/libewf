/*
 * Libewf function wrapper for the ewftools
 *
 * Copyright (c) 2008-2009, Joachim Metz <forensics@hoffmannbv.nl>,
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
#include <memory.h>
#include <narrow_string.h>
#include <types.h>

#include <liberror.h>

/* If libtool DLL support is enabled set LIBEWF_DLL_IMPORT
 * before including libewf.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBEWF_DLL_IMPORT
#endif

#include <libewf.h>

#include "character_string.h"
#include "ewflibewf.h"

/* Retrieves the header value from the libewf handle
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int ewflibewf_get_header_value(
     libewf_handle_t *handle,
     const char *utf8_header_value_identifier,
     size_t utf8_header_value_identifier_length,
     character_t *header_value,
     size_t header_value_size,
     liberror_error_t **error )
{
	char utf8_header_value_buffer[ 128 ];

	char *utf8_header_value       = NULL;
	static char *function         = "ewflibewf_get_header_value";
	size_t utf8_header_value_size = 0;
	int result                    = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( utf8_header_value_identifier == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header value identifier.",
		 function );

		return( -1 );
	}
	if( header_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header value.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE )
	utf8_header_value      = utf8_header_value_buffer;
	utf8_header_value_size = 128;
#else
	utf8_header_value      = header_value;
	utf8_header_value_size = header_value_size;
#endif

	result = libewf_get_header_value(
	          handle,
	          utf8_header_value_identifier,
	          utf8_header_value,
	          utf8_header_value_size );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		return( 0 );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE )
	utf8_header_value_size = 1 + narrow_string_length(
	                              utf8_header_value );

	if( string_size_from_utf8_string(
	     utf8_header_value,
	     utf8_header_value_size,
	     &header_value_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine header value size.",
		 function );

		return( -1 );
	}
	if( string_copy_from_utf8_string(
	     header_value,
	     header_value_size,
	     utf8_header_value,
	     utf8_header_value_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set header value.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Sets a header value in the libewf handle
 * Returns 1 if successful or -1 on error
 */
int ewflibewf_set_header_value(
     libewf_handle_t *handle,
     const char *utf8_header_value_identifier,
     size_t utf8_header_value_identifier_length,
     character_t *header_value, 
     liberror_error_t **error )
{
	char *utf8_header_value         = NULL;
	static char *function           = "ewflibewf_set_header_value";
	size_t header_value_length      = 0;
	size_t utf8_header_value_length = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( utf8_header_value_identifier == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header value identifier.",
		 function );

		return( -1 );
	}
	if( header_value == NULL )
	{
		utf8_header_value        = NULL;
		utf8_header_value_length = 0;
	}
	else
	{
		header_value_length = string_length(
		                       header_value );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
		if( utf8_string_size_from_string(
	             header_value,
		     header_value_length + 1,
		     &utf8_header_value_length,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_CONVERSION,
			 LIBERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to determine UTF-8 header value size.",
			 function );

			return( -1 );
		}
		utf8_header_value = (char *) memory_allocate(
		                              sizeof( char ) * utf8_header_value_length );

		if( utf8_header_value == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create UTF-8 header value.",
			 function );

			return( -1 );
		}
		if( utf8_string_copy_from_string(
		     utf8_header_value,
		     utf8_header_value_length,
	             header_value,
		     header_value_length + 1,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_CONVERSION,
			 LIBERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set UTF-8 header value.",
			 function );

			memory_free(
			 utf8_header_value );

			return( -1 );
		}
		utf8_header_value_length -= 1;
#else
		utf8_header_value        = header_value;
		utf8_header_value_length = header_value_length;
#endif
	}
	if( libewf_set_header_value(
	     handle,
	     utf8_header_value_identifier,
	     utf8_header_value,
	     utf8_header_value_length ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: %s in handle.",
		 function,
		 utf8_header_value );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
		memory_free(
		 utf8_header_value );
#endif

		return( -1 );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE )
	memory_free(
	 utf8_header_value );
#endif
	return( 1 );
}

/* Retrieves the hash value from the libewf handle
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int ewflibewf_get_hash_value(
     libewf_handle_t *handle,
     const char *utf8_hash_value_identifier,
     size_t utf8_hash_value_identifier_length,
     character_t *hash_value,
     size_t hash_value_size,
     liberror_error_t **error )
{
	char utf8_hash_value_buffer[ 128 ];

	char *utf8_hash_value       = NULL;
	static char *function       = "ewflibewf_get_hash_value";
	size_t utf8_hash_value_size = 0;
	int result                  = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( utf8_hash_value_identifier == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash value identifier.",
		 function );

		return( -1 );
	}
	if( hash_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash value.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE )
	utf8_hash_value      = utf8_hash_value_buffer;
	utf8_hash_value_size = 128;
#else
	utf8_hash_value      = hash_value;
	utf8_hash_value_size = hash_value_size;
#endif

	result = libewf_get_hash_value(
	          handle,
	          utf8_hash_value_identifier,
	          utf8_hash_value,
	          utf8_hash_value_size );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve hash value.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		return( 0 );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE )
	utf8_hash_value_size = 1 + narrow_string_length(
	                            utf8_hash_value );

	if( string_size_from_utf8_string(
	     utf8_hash_value,
	     utf8_hash_value_size,
	     &hash_value_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine hash value size.",
		 function );

		return( -1 );
	}
	if( string_copy_from_utf8_string(
	     hash_value,
	     hash_value_size,
	     utf8_hash_value,
	     utf8_hash_value_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set hash value.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Sets a hash value in the libewf handle
 * Returns 1 if successful or -1 on error
 */
int ewflibewf_set_hash_value(
     libewf_handle_t *handle,
     const char *utf8_hash_value_identifier,
     size_t utf8_hash_value_identifier_length,
     character_t *hash_value, 
     liberror_error_t **error )
{
	char *utf8_hash_value         = NULL;
	static char *function         = "ewflibewf_set_hash_value";
	size_t hash_value_length      = 0;
	size_t utf8_hash_value_length = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( utf8_hash_value_identifier == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash value identifier.",
		 function );

		return( -1 );
	}
	if( hash_value == NULL )
	{
		utf8_hash_value        = NULL;
		utf8_hash_value_length = 0;
	}
	else
	{
		hash_value_length = string_length(
		                     hash_value );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
		if( utf8_string_size_from_string(
	             hash_value,
		     hash_value_length + 1,
		     &utf8_hash_value_length,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_CONVERSION,
			 LIBERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to determine UTF-8 hash value size.",
			 function );

			return( -1 );
		}
		utf8_hash_value = (char *) memory_allocate(
		                            sizeof( char ) * utf8_hash_value_length );

		if( utf8_hash_value == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create UTF-8 hash value.",
			 function );

			return( -1 );
		}
		if( utf8_string_copy_from_string(
		     utf8_hash_value,
		     utf8_hash_value_length,
	             hash_value,
		     hash_value_length + 1,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_CONVERSION,
			 LIBERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set UTF-8 hash value.",
			 function );

			memory_free(
			 utf8_hash_value );

			return( -1 );
		}
		utf8_hash_value_length -= 1;
#else
		utf8_hash_value        = hash_value;
		utf8_hash_value_length = hash_value_length;
#endif
	}
	if( libewf_set_hash_value(
	     handle,
	     utf8_hash_value_identifier,
	     utf8_hash_value,
	     utf8_hash_value_length ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set hash value: %s in handle.",
		 function,
		 utf8_hash_value );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
		memory_free(
		 utf8_hash_value );
#endif

		return( -1 );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE )
	memory_free(
	 utf8_hash_value );
#endif
	return( 1 );
}

