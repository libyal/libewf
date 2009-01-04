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
#include "libewf_libuna.h"
#include "libewf_string.h"

#include "ewf_crc.h"

/* Prints a dump of data
 */
void libewf_debug_dump_data(
      uint8_t *data,
      size_t size )
{
	static char *function    = "libewf_debug_dump_data";
	ewf_crc_t stored_crc     = 0;
	ewf_crc_t calculated_crc = 0;

	if( size > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid size value exceeds maximum.\n",
		 function );

		return;
	}
	calculated_crc = ewf_crc_calculate(
	                  data,
	                  ( size - sizeof( ewf_crc_t ) ),
	                  1 );

	notify_dump_data(
	 data,
	 size );

	if( memory_copy(
	     &stored_crc,
	     &data[ size - sizeof( ewf_crc_t ) ],
	     sizeof( ewf_crc_t ) ) == NULL )
	{
		notify_warning_printf( "%s: unable to set CRC.\n",
		 function );
	}
	else
	{
		notify_printf( "%s: possible CRC (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n",
		 function, stored_crc, calculated_crc );
	}
}

/* Prints the section data to notify stream
 */
void libewf_debug_section_print(
      ewf_section_t *section )
{
	static char *function    = "libewf_debug_section_print";
	ewf_crc_t calculated_crc = 0;
	ewf_crc_t stored_crc     = 0;
	uint64_t next            = 0;
	uint64_t size            = 0;

	if( section == NULL )
	{
		notify_warning_printf( "%s: invalid section.\n",
		 function );

		return;
	}
	calculated_crc = ewf_crc_calculate(
	                  section,
	                  ( sizeof( ewf_section_t ) - sizeof( ewf_crc_t ) ),
	                  1 );

	endian_little_convert_32bit(
	 stored_crc,
	 section->crc );

	endian_little_convert_64bit(
	 next,
	 section->next );

	endian_little_convert_64bit(
	 size,
	 section->size );

	notify_printf( "Section:\n" );
	notify_printf( "type: %s\n",
	 (char *) section->type );
	notify_printf( "next: %" PRIu64 "\n",
	 next );
	notify_printf( "size: %" PRIu64 "\n",
	 size );
	notify_printf( "crc: %" PRIu32 " ( %" PRIu32 " )\n",
	 stored_crc, calculated_crc );
	notify_printf( "\n" );
}

/* Prints the header data to the notify stream
 */
void libewf_debug_header_print(
      uint8_t *header,
      size_t header_size )
{
	libewf_character_t *header_string = NULL;
	libuna_error_t *error             = NULL;
	static char *function             = "libewf_debug_header_print";
	size_t header_string_size         = 0;

	if( header == NULL )
	{
		notify_warning_printf( "%s: invalid header.\n",
		 function );

		return;
	}
	if( libewf_string_size_from_byte_stream(
	     header,
	     header_size,
	     LIBUNA_CODEPAGE_ASCII,
	     &header_string_size,
	     &error ) != 1 )
	{
		notify_warning_printf( "%s: unable to determine header string size.\n",
		 function );

		libuna_error_free(
		 &error );

		return;
	}
	header_string = (libewf_character_t *) memory_allocate(
	                                        sizeof( libewf_character_t ) * header_string_size );

	if( header_string == NULL )
	{
		notify_warning_printf( "%s: unable to create header string.\n",
		 function );

		return;
	}
	if( libewf_string_copy_from_byte_stream(
	     header_string,
	     header_string_size,
	     header,
	     header_size,
	     LIBUNA_CODEPAGE_ASCII,
	     &error ) != 1 )
	{
		notify_warning_printf( "%s: unable to set header string.\n",
		 function );

		libuna_error_free(
		 &error );
		memory_free(
		 header_string );

		return;
	}
	notify_printf( "%" PRIs_LIBEWF "",
	 header_string );

	memory_free(
	 header_string );
}

/* Prints the header2 data to the notify stream
 */
void libewf_debug_header2_print(
      uint8_t *header2,
      size_t header2_size )
{
	libewf_character_t *header_string = NULL;
	libuna_error_t *error             = NULL;
	static char *function             = "libewf_debug_header2_print";
	size_t header_string_size         = 0;

	if( header2 == NULL )
	{
		notify_warning_printf( "%s: invalid header2.\n",
		 function );

		return;
	}
	if( libewf_string_size_from_utf16_stream(
	     header2,
	     header2_size,
	     LIBUNA_ENDIAN_LITTLE,
	     &header_string_size,
	     &error ) != 1 )
	{
		notify_warning_printf( "%s: unable to determine header string size.\n",
		 function );

		libuna_error_free(
		 &error );

		return;
	}
	header_string = (libewf_character_t *) memory_allocate(
	                                        sizeof( libewf_character_t ) * header_string_size );

	if( header_string == NULL )
	{
		notify_warning_printf( "%s: unable to create header string.\n",
		 function );

		return;
	}
	if( libewf_string_copy_from_utf16_stream(
	     header_string,
	     header_string_size,
	     header2,
	     header2_size,
	     LIBUNA_ENDIAN_LITTLE,
	     &error ) != 1 )
	{
		notify_warning_printf( "%s: unable to set header string.\n",
		 function );

		libuna_error_free(
		 &error );
		memory_free(
		 header_string );

		return;
	}
	notify_printf( "%" PRIs_LIBEWF "",
	 header_string );

	memory_free(
	 header_string );
}

/* Prints the xheader data to the notify stream
 */
void libewf_debug_xheader_print(
      uint8_t *xheader,
      size_t xheader_size )
{
	libewf_character_t *header_string = NULL;
	libuna_error_t *error             = NULL;
	static char *function             = "libewf_debug_xheader_print";
	size_t header_string_size         = 0;

	if( xheader == NULL )
	{
		notify_warning_printf( "%s: invalid xheader.\n",
		 function );

		return;
	}
	if( libewf_string_size_from_utf8_stream(
	     xheader,
	     xheader_size,
	     &header_string_size,
	     &error ) != 1 )
	{
		notify_warning_printf( "%s: unable to determine header string size.\n",
		 function );

		libuna_error_free(
		 &error );

		return;
	}
	header_string = (libewf_character_t *) memory_allocate(
	                                        sizeof( libewf_character_t ) * header_string_size );

	if( header_string == NULL )
	{
		notify_warning_printf( "%s: unable to create header string.\n",
		 function );

		return;
	}
	if( libewf_string_copy_from_utf8_stream(
	     header_string,
	     header_string_size,
	     xheader,
	     xheader_size,
	     &error ) != 1 )
	{
		notify_warning_printf( "%s: unable to set header string.\n",
		 function );

		libuna_error_free(
		 &error );
		memory_free(
		 header_string );

		return;
	}
	notify_printf( "%" PRIs_LIBEWF "",
	 header_string );

	memory_free(
	 header_string );
}

