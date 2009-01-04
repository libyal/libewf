/*
 * libewf debug
 *
 * Copyright (c) 2006-2008, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of the creator, related organisations, nor the names of
 *   its contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER, COMPANY AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <common.h>
#include <character_string.h>
#include <memory.h>
#include <notify.h>
#include <types.h>

#include <libewf/definitions.h>

#include "libewf_compression.h"
#include "libewf_debug.h"
#include "libewf_endian.h"
#include "libewf_segment_file.h"
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

	if( libewf_endian_convert_32bit(
	     &stored_crc,
	     section->crc ) != 1 )
	{
		notify_warning_printf( "%s: unable to convert stored CRC value.\n",
		 function );

		return;
	}
	if( libewf_endian_convert_64bit(
	     &next,
	     section->next ) != 1 )
	{
		notify_warning_printf( "%s: unable to convert next offset value.\n",
		 function );

		return;
	}
	if( libewf_endian_convert_64bit(
	     &size,
	     section->size ) != 1 )
	{
		notify_warning_printf( "%s: unable to convert size value.\n",
		 function );

		return;
	}
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
      ewf_char_t *header,
      size_t size )
{
	character_t *header_string = NULL;
	static char *function      = "libewf_debug_header_print";

	if( header == NULL )
	{
		notify_warning_printf( "%s: invalid header.\n",
		 function );

		return;
	}
	header_string = (character_t *) memory_allocate(
	                                 sizeof( character_t ) * size );

	if( header_string == NULL )
	{
		notify_warning_printf( "%s: unable to create header string.\n",
		 function );

		return;
	}
	if( string_copy_from_char(
	     header_string,
	     (char *) header,
	     size ) != 1 )
	{
		notify_warning_printf( "%s: unable to copy header to header string.\n",
		 function );

		memory_free(
		 header_string );

		return;
	}
	notify_printf( "%" PRIs "",
	 header_string );

	memory_free(
	 header_string );
}

/* Prints the header2 data to the notify stream
 */
void libewf_debug_header2_print(
      ewf_char_t *header2,
      size_t size )
{
	character_t *header_string = NULL;
	static char *function      = "libewf_debug_header2_print";
	size_t header_size         = 0;

	if( header2 == NULL )
	{
		notify_warning_printf( "%s: invalid header2.\n",
		 function );

		return;
	}
	header_size   = ( size - 1 ) / 2;
	header_string = (character_t *) memory_allocate(
	                                 sizeof( character_t ) * header_size );

	if( header_string == NULL )
	{
		notify_warning_printf( "%s: unable to create header string.\n",
		 function );

		return;
	}
	if( libewf_string_copy_from_header2(
	     header_string,
	     header_size,
	     header2,
	     size ) != 1 )
	{
		notify_warning_printf( "%s: unable to copy header2 to header string.\n",
		 function );

		memory_free(
		 header_string );

		return;
	}
	notify_printf( "%" PRIs "",
	 header_string );

	memory_free(
	 header_string );
}

