/*
 * Debugging functions
 *
 * Copyright (C) 2006-2020, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "libewf_debug.h"
#include "libewf_definitions.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libfguid.h"
#include "libewf_libuna.h"

#if defined( HAVE_DEBUG_OUTPUT )

/* Prints the compression method
 */
void libewf_debug_print_compression_method(
      uint16_t compression_method )
{
	switch( compression_method )
	{
		case LIBEWF_COMPRESSION_METHOD_NONE:
			libcnotify_printf(
			 "none" );
			break;

		case LIBEWF_COMPRESSION_METHOD_DEFLATE:
			libcnotify_printf(
			 "deflate" );
			break;

		case LIBEWF_COMPRESSION_METHOD_BZIP2:
			libcnotify_printf(
			 "bzip2" );
			break;

		default:
			libcnotify_printf(
			 "UNKNOWN" );
	}
}

/* Prints the section type
 */
void libewf_debug_print_section_type(
      uint32_t section_type )
{
	switch( section_type )
	{
		case LIBEWF_SECTION_TYPE_DEVICE_INFORMATION:
			libcnotify_printf(
			 "Device information" );
			break;

		case LIBEWF_SECTION_TYPE_CASE_DATA:
			libcnotify_printf(
			 "Case data" );
			break;

		case LIBEWF_SECTION_TYPE_SECTOR_DATA:
			libcnotify_printf(
			 "Sector data" );
			break;

		case LIBEWF_SECTION_TYPE_SECTOR_TABLE:
			libcnotify_printf(
			 "Sector table" );
			break;

		case LIBEWF_SECTION_TYPE_ERROR_TABLE:
			libcnotify_printf(
			 "Error table" );
			break;

		case LIBEWF_SECTION_TYPE_SESSION_TABLE:
			libcnotify_printf(
			 "Session table" );
			break;

		case LIBEWF_SECTION_TYPE_INCREMENT_DATA:
			libcnotify_printf(
			 "Increment data" );
			break;

		case LIBEWF_SECTION_TYPE_MD5_HASH:
			libcnotify_printf(
			 "MD5 hash" );
			break;

		case LIBEWF_SECTION_TYPE_SHA1_HASH:
			libcnotify_printf(
			 "SHA1 hash" );
			break;

		case LIBEWF_SECTION_TYPE_RESTART_DATA:
			libcnotify_printf(
			 "Restart data" );
			break;

		case LIBEWF_SECTION_TYPE_ENCRYPTION_KEYS:
			libcnotify_printf(
			 "Encryption keys" );
			break;

		case LIBEWF_SECTION_TYPE_MEMORY_EXTENTS_TABLE:
			libcnotify_printf(
			 "Memory extents" );
			break;

		case LIBEWF_SECTION_TYPE_NEXT:
			libcnotify_printf(
			 "Next" );
			break;

		case LIBEWF_SECTION_TYPE_FINAL_INFORMATION:
			libcnotify_printf(
			 "Final information" );
			break;

		case LIBEWF_SECTION_TYPE_DONE:
			libcnotify_printf(
			 "Done" );
			break;

		case LIBEWF_SECTION_TYPE_ANALYTICAL_DATA:
			libcnotify_printf(
			 "Analytical data" );
			break;

		default:
			libcnotify_printf(
			 "UNKNOWN" );
	}
}

/* Prints the section data flags
 */
void libewf_debug_print_section_data_flags(
      uint32_t section_data_flags )
{
	if( ( section_data_flags & LIBEWF_SECTION_DATA_FLAGS_HAS_INTEGRITY_HASH ) != 0 )
	{
		libcnotify_printf(
		 "\tHas integrity hash (MD5HASHED)\n" );
	}
	if( ( section_data_flags & LIBEWF_SECTION_DATA_FLAGS_IS_ENCRYPTED ) != 0 )
	{
		libcnotify_printf(
		 "\tIs encrypted (ENCRYPTED)\n" );
	}
}

/* Prints a GUID/UUID value
 * Returns 1 if successful or -1 on error
 */
int libewf_debug_print_guid_value(
     const char *function_name,
     const char *value_name,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int byte_order,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
        system_character_t guid_string[ 48 ];

        libfguid_identifier_t *guid = NULL;
	static char *function       = "libewf_debug_print_guid_value";

	if( libfguid_identifier_initialize(
	     &guid,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create GUID.",
		 function );

		goto on_error;
	}
	if( libfguid_identifier_copy_from_byte_stream(
	     guid,
	     byte_stream,
	     byte_stream_size,
	     byte_order,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy byte stream to GUID.",
		 function );

		goto on_error;
	}
	if( libfguid_identifier_copy_to_utf8_string(
	     guid,
	     (uint8_t *) guid_string,
	     48,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy GUID to string.",
		 function );

		goto on_error;
	}
	libcnotify_printf(
	 "%s: %s: %s\n",
	 function_name,
	 value_name,
	 guid_string );

	if( libfguid_identifier_free(
	     &guid,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free GUID.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( guid != NULL )
	{
		libfguid_identifier_free(
		 &guid,
		 NULL );
	}
	return( -1 );
}

/* Prints an UTF-16 string value
 * Returns 1 if successful or -1 on error
 */
int libewf_debug_print_utf16_string_value(
     const char *function_name,
     const char *value_name,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int byte_order,
     libcerror_error_t **error )
{
	system_character_t *string = NULL;
	static char *function      = "libewf_debug_print_utf16_string_value";
	size_t string_size         = 0;
	int result                 = 0;

	if( ( byte_stream == NULL )
	 || ( byte_stream_size == 0 ) )
	{
		libcnotify_printf(
		 "%s: %s: \n",
		 function_name,
		 value_name );

		return( 1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libuna_utf16_string_size_from_utf16_stream(
		  byte_stream,
		  byte_stream_size,
		  byte_order,
		  &string_size,
		  error );
#else
	result = libuna_utf8_string_size_from_utf16_stream(
		  byte_stream,
		  byte_stream_size,
		  byte_order,
		  &string_size,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine size of string.",
		 function );

		goto on_error;
	}
	if( ( string_size > (size_t) SSIZE_MAX )
	 || ( ( sizeof( system_character_t ) * string_size ) > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid string size value exceeds maximum.",
		 function );

		goto on_error;
	}
	string = system_string_allocate(
	          string_size );

	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create string.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libuna_utf16_string_copy_from_utf16_stream(
		  (libuna_utf16_character_t *) string,
		  string_size,
		  byte_stream,
		  byte_stream_size,
		  byte_order,
		  error );
#else
	result = libuna_utf8_string_copy_from_utf16_stream(
		  (libuna_utf8_character_t *) string,
		  string_size,
		  byte_stream,
		  byte_stream_size,
		  byte_order,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set string.",
		 function );

		goto on_error;
	}
	libcnotify_printf(
	 "%s: %s: %s\n",
	 function_name,
	 value_name,
	 string );

	memory_free(
	 string );

	return( 1 );

on_error:
	if( string != NULL )
	{
		memory_free(
		 string );
	}
	return( -1 );
}

/* Prints the byte stream data to the notify stream
 * Returns 1 if successful or -1 on error
 */
int libewf_debug_byte_stream_print(
     const char *header_string,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     libcerror_error_t **error )
{
	system_character_t *string = NULL;
	static char *function      = "libewf_debug_byte_stream_print";
	size_t string_size         = 0;
	int result                 = 0;

	if( header_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header string.",
		 function );

		return( -1 );
	}
	if( byte_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid byte stream.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libuna_utf16_string_size_from_byte_stream(
	          byte_stream,
	          byte_stream_size,
	          LIBUNA_CODEPAGE_ASCII,
	          &string_size,
	          error );
#else
	result = libuna_utf8_string_size_from_byte_stream(
	          byte_stream,
	          byte_stream_size,
	          LIBUNA_CODEPAGE_ASCII,
	          &string_size,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine string size.",
		 function );

		return( -1 );
	}
	string = system_string_allocate(
	          string_size );

	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create string.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libuna_utf16_string_copy_from_byte_stream(
	          (uint16_t *) string,
	          string_size,
	          byte_stream,
	          byte_stream_size,
	          LIBUNA_CODEPAGE_ASCII,
	          error );
#else
	result = libuna_utf8_string_copy_from_byte_stream(
	          (uint8_t *) string,
	          string_size,
	          byte_stream,
	          byte_stream_size,
	          LIBUNA_CODEPAGE_ASCII,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy string from byte stream.",
		 function );

		memory_free(
		 string );

		return( -1 );
	}
	libcnotify_printf(
	 "%s:\n"
	 "%" PRIs_SYSTEM "",
	 header_string,
	 string );

	memory_free(
	 string );

	return( 1 );
}

/* Prints the UTF-8 stream data to the notify stream
 * Returns 1 if successful or -1 on error
 */
int libewf_debug_utf8_stream_print(
     const char *header_string,
     const uint8_t *utf8_stream,
     size_t utf8_stream_size,
     libcerror_error_t **error )
{
	system_character_t *string = NULL;
	static char *function      = "libewf_debug_utf8_stream_print";
	size_t string_size         = 0;
	int result                 = 0;

	if( header_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header string.",
		 function );

		return( -1 );
	}
	if( utf8_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 stream.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libuna_utf16_string_size_from_utf8_stream(
	          utf8_stream,
	          utf8_stream_size,
	          &string_size,
	          error );
#else
	result = libuna_utf8_string_size_from_utf8_stream(
	          utf8_stream,
	          utf8_stream_size,
	          &string_size,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine string size.",
		 function );

		return( -1 );
	}
	string = system_string_allocate(
	          string_size );

	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create string.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libuna_utf16_string_copy_from_utf8_stream(
	          (uint16_t *) string,
	          string_size,
	          utf8_stream,
	          utf8_stream_size,
	          error );
#else
	result = libuna_utf8_string_copy_from_utf8_stream(
	          (uint8_t *) string,
	          string_size,
	          utf8_stream,
	          utf8_stream_size,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy string from UTF-8 stream.",
		 function );

		memory_free(
		 string );

		return( -1 );
	}
	libcnotify_printf(
	 "%s:\n"
	 "%" PRIs_SYSTEM "",
	 header_string,
	 string );

	memory_free(
	 string );

	return( 1 );
}

/* Prints the UTF-16 data to the notify stream
 * Returns 1 if successful or -1 on error
 */
int libewf_debug_utf16_stream_print(
     const char *header_string,
     const uint8_t *utf16_stream,
     size_t utf16_stream_size,
     libcerror_error_t **error )
{
	system_character_t *string = NULL;
	static char *function      = "libewf_debug_utf16_stream_print";
	size_t string_size         = 0;
	int result                 = 0;

	if( header_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header string.",
		 function );

		return( -1 );
	}
	if( utf16_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 stream.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libuna_utf16_string_size_from_utf16_stream(
	          utf16_stream,
	          utf16_stream_size,
	          LIBUNA_ENDIAN_LITTLE,
	          &string_size,
	          error );
#else
	result = libuna_utf8_string_size_from_utf16_stream(
	          utf16_stream,
	          utf16_stream_size,
	          LIBUNA_ENDIAN_LITTLE,
	          &string_size,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine string size.",
		 function );

		return( -1 );
	}
	string = system_string_allocate(
	          string_size );

	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create string.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libuna_utf16_string_copy_from_utf16_stream(
	          (uint16_t *) string,
	          string_size,
	          utf16_stream,
	          utf16_stream_size,
	          LIBUNA_ENDIAN_LITTLE,
	          error );
#else
	result = libuna_utf8_string_copy_from_utf16_stream(
	          (uint8_t *) string,
	          string_size,
	          utf16_stream,
	          utf16_stream_size,
	          LIBUNA_ENDIAN_LITTLE,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy string from UTF-16 stream.",
		 function );

		memory_free(
		 string );

		return( -1 );
	}
	libcnotify_printf(
	 "%s:\n"
	 "%" PRIs_SYSTEM "",
	 header_string,
	 string );

	memory_free(
	 string );

	return( 1 );
}

#endif /* defined( HAVE_DEBUG_OUTPUT ) */

