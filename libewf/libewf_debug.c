/*
 * Debugging functions
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
#include <endian.h>
#include <memory.h>
#include <notify.h>
#include <types.h>

#include "libewf_debug.h"
#include "libewf_error.h"
#include "libewf_string.h"

#include "ewf_crc.h"

/* Prints a dump of data
 * Returns 1 if successful or -1 on error
 */
int libewf_debug_dump_data(
     libewf_character_t *header,
     uint8_t *data,
     size_t data_size,
     libewf_error_t **error )
{
	uint8_t *crc_data        = NULL;
	static char *function    = "libewf_debug_dump_data";
	ewf_crc_t stored_crc     = 0;
	ewf_crc_t calculated_crc = 0;

	if( header == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header.\n",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.\n",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	calculated_crc = ewf_crc_calculate(
	                  data,
	                  ( data_size - sizeof( ewf_crc_t ) ),
	                  1 );

	crc_data = &data[ data_size - sizeof( ewf_crc_t ) ];

	endian_little_convert_32bit(
	 stored_crc,
	 crc_data );

	notify_printf(
	 "%" PRIs_LIBEWF ":\n",
	 header );

	notify_dump_data(
	 data,
	 data_size );

	notify_printf(
	 "%s: possible CRC (in file: %" PRIu32 " calculated: %" PRIu32 ").\n",
	 function,	
	 stored_crc,	
	 calculated_crc );

	return( 1 );
}

/* Prints the section data to notify stream
 * Returns 1 if successful or -1 on error
 */
int libewf_debug_section_print(
     ewf_section_t *section,
     libewf_error_t **error )
{
	static char *function    = "libewf_debug_section_print";
	ewf_crc_t calculated_crc = 0;
	ewf_crc_t stored_crc     = 0;
	uint64_t section_next    = 0;
	uint64_t section_size    = 0;

	if( section == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.\n",
		 function );

		return( -1 );
	}
	calculated_crc = ewf_crc_calculate(
	                  section,
	                  sizeof( ewf_section_t ) - sizeof( ewf_crc_t ),
	                  1 );

	endian_little_convert_32bit(
	 stored_crc,
	 section->crc );

	endian_little_convert_64bit(
	 section_next,
	 section->next );

	endian_little_convert_64bit(
	 section_size,
	 section->size );

	notify_printf(
	 "Section:\n"
	 "type:\t%s\n"
	 "next:\t%" PRIu64 "\n"
	 "size:\t%" PRIu64 "\n"
	 "crc:\t%" PRIu32 " ( %" PRIu32 " )\n"
	 "\n",
	 (char *) section->type,
	 section_next,
	 section_size,
	 stored_crc,
	 calculated_crc );

	return( 1 );
}

/* Prints the byte stream data to the notify stream
 * Returns 1 if successful or -1 on error
 */
int libewf_debug_byte_stream_print(
     libewf_character_t *header,
     uint8_t *byte_stream,
     size_t byte_stream_size,
     libewf_error_t **error )
{
	libewf_character_t *string = NULL;
	static char *function      = "libewf_debug_byte_stream_print";
	size_t string_size         = 0;

	if( header == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header.\n",
		 function );

		return( -1 );
	}
	if( byte_stream == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid byte stream.\n",
		 function );

		return( -1 );
	}
	if( libewf_string_size_from_byte_stream(
	     byte_stream,
	     byte_stream_size,
	     LIBUNA_CODEPAGE_ASCII,
	     &string_size,
	     error ) != 1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_CONVERSION,
		 LIBEWF_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine string size.\n",
		 function );

		return( -1 );
	}
	string = (libewf_character_t *) memory_allocate(
	                                 sizeof( libewf_character_t ) * string_size );

	if( string == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_MEMORY,
		 LIBEWF_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create string.\n",
		 function );

		return( -1 );
	}
	if( libewf_string_copy_from_byte_stream(
	     string,
	     string_size,
	     byte_stream,
	     byte_stream_size,
	     LIBUNA_CODEPAGE_ASCII,
	     error ) != 1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_CONVERSION,
		 LIBEWF_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set string.\n",
		 function );

		memory_free(
		 string );

		return( -1 );
	}
	notify_printf(
	 "%" PRIs_LIBEWF ":\n",
	 "%" PRIs_LIBEWF "",
	 header,
	 string );

	memory_free(
	 string );

	return( 1 );
}

/* Prints the UTF-8 stream data to the notify stream
 * Returns 1 if successful or -1 on error
 */
int libewf_debug_utf8_stream_print(
     libewf_character_t *header,
     uint8_t *utf8_stream,
     size_t utf8_stream_size,
     libewf_error_t **error )
{
	libewf_character_t *string = NULL;
	static char *function      = "libewf_debug_utf8_stream_print";
	size_t string_size         = 0;

	if( header == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header.\n",
		 function );

		return( -1 );
	}
	if( utf8_stream == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 stream.\n",
		 function );

		return( -1 );
	}
	if( libewf_string_size_from_utf8_stream(
	     utf8_stream,
	     utf8_stream_size,
	     &string_size,
	     error ) != 1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_CONVERSION,
		 LIBEWF_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine string size.\n",
		 function );

		return( -1 );
	}
	string = (libewf_character_t *) memory_allocate(
	                                 sizeof( libewf_character_t ) * string_size );

	if( string == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_MEMORY,
		 LIBEWF_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create string.\n",
		 function );

		return( -1 );
	}
	if( libewf_string_copy_from_utf8_stream(
	     string,
	     string_size,
	     utf8_stream,
	     utf8_stream_size,
	     error ) != 1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_CONVERSION,
		 LIBEWF_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set string.\n",
		 function );

		memory_free(
		 string );

		return( -1 );
	}
	notify_printf(
	 "%" PRIs_LIBEWF ":",
	 "%" PRIs_LIBEWF "",
	 header,
	 string );

	memory_free(
	 string );

	return( 1 );
}

/* Prints the UTF-16 data to the notify stream
 * Returns 1 if successful or -1 on error
 */
int libewf_debug_utf16_stream_print(
     libewf_character_t *header,
     uint8_t *utf16_stream,
     size_t utf16_stream_size,
     libewf_error_t **error )
{
	libewf_character_t *string = NULL;
	static char *function      = "libewf_debug_utf16_stream_print";
	size_t string_size         = 0;

	if( header == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header.\n",
		 function );

		return( -1 );
	}
	if( utf16_stream == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 stream.\n",
		 function );

		return( -1 );
	}
	if( libewf_string_size_from_utf16_stream(
	     utf16_stream,
	     utf16_stream_size,
	     LIBUNA_ENDIAN_LITTLE,
	     &string_size,
	     error ) != 1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_CONVERSION,
		 LIBEWF_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine string size.\n",
		 function );

		return( -1 );
	}
	string = (libewf_character_t *) memory_allocate(
	                                 sizeof( libewf_character_t ) * string_size );

	if( string == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_MEMORY,
		 LIBEWF_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create string.\n",
		 function );

		return( -1 );
	}
	if( libewf_string_copy_from_utf16_stream(
	     string,
	     string_size,
	     utf16_stream,
	     utf16_stream_size,
	     LIBUNA_ENDIAN_LITTLE,
	     error ) != 1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_CONVERSION,
		 LIBEWF_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set string.\n",
		 function );

		memory_free(
		 string );

		return( -1 );
	}
	notify_printf(
	 "%" PRIs_LIBEWF ":",
	 "%" PRIs_LIBEWF "",
	 header,
	 string );

	memory_free(
	 string );

	return( 1 );
}

