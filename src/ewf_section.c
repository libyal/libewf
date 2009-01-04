/*
 * EWF section start
 *
 * Copyright (c) 2006, Joachim Metz <forensics@hoffmannbv.nl>,
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
 * - All advertising materials mentioning features or use of this software
 *   must acknowledge the contribution by people stated in the acknowledgements.
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

#include <stdlib.h>

#include "definitions.h"
#include "libewf_common.h"
#include "libewf_endian.h"
#include "libewf_notify.h"

#include "ewf_section.h"
#include "ewf_crc.h"

/* Allocates memory for a new ewf section struct
 * Return a pointer to the new instance, NULL on error
 */
EWF_SECTION *ewf_section_alloc( void )
{
	EWF_SECTION *section = NULL;

	section = (EWF_SECTION *) libewf_alloc_cleared( EWF_SECTION_SIZE );

	if( section == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_section_alloc: unable to allocate ewf_section.\n" );

		return( NULL );
	}
	return( section );
}

/* Frees memory of a section
 */
void ewf_section_free( EWF_SECTION *section )
{
	if( section == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_section_free: invalid section.\n" );

		return;
	}
	libewf_free( section );
}

/* Reads the section from a file descriptor
 * Return a pointer to the new instance, NULL on error
 */
EWF_SECTION *ewf_section_read( int file_descriptor )
{
	EWF_SECTION *section = NULL;
	uint32_t size        = EWF_SECTION_SIZE;
	int32_t count        = 0;

	section = ewf_section_alloc();

	if( section == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_section_read: unable to create section.\n" );

		return( NULL );
	}
	count = read( file_descriptor, section, size );

	if( count < size )
	{
		ewf_section_free( section );

		LIBEWF_WARNING_PRINT( "ewf_section_read: unable to read section.\n" );

		return( NULL );
	}
	return( section );
}

/* Writes a section to a file descriptor
 * Returns a -1 on error, the amount of bytes written on success
 */
int32_t ewf_section_write( EWF_SECTION *section, int file_descriptor )
{
	EWF_CRC *crc  = NULL;
	uint32_t size = EWF_SECTION_SIZE;
	int32_t count = 0;

	if( section == NULL )
	{
		LIBEWF_VERBOSE_PRINT( "ewf_section_write: invalid section.\n" );

		return( -1 );
	}
	crc = ewf_crc_calculate( (void *) section, ( size - EWF_CRC_SIZE ), 1 );

	if( crc == NULL )
	{
		LIBEWF_VERBOSE_PRINT( "ewf_section_write: unable to calculate CRC.\n" );

		return( -1 );
	}
	revert_32bit( *crc, section->crc );

	ewf_crc_free( crc );

	count = write( file_descriptor, section, size );

	if( count < size )
	{
		return( -1 );
	}
	return( count );
}

/* Test if section is of a certain type
 */
uint8_t ewf_section_is_type( EWF_SECTION *section, const char *type )
{
	return( libewf_memcmp( section->type, type, libewf_strlen( type ) ) == 0 );
}

/* Test if section is of type header
 */
uint8_t ewf_section_is_type_header( EWF_SECTION *section )
{
	return( ewf_section_is_type( section, "header" ) );
}

/* Test if section is of type header2
 */
uint8_t ewf_section_is_type_header2( EWF_SECTION *section )
{
	return( ewf_section_is_type( section, "header2" ) );
}

/* Test if section is of type volume
 */
uint8_t ewf_section_is_type_volume( EWF_SECTION *section )
{
	return( ewf_section_is_type( section, "volume" ) );
}

/* Test if section is of type disk
 */
uint8_t ewf_section_is_type_disk( EWF_SECTION *section )
{
	return( ewf_section_is_type( section, "disk" ) );
}

/* Test if section is of type table
 */
uint8_t ewf_section_is_type_table( EWF_SECTION *section )
{
	return( ewf_section_is_type( section, "table" ) );
}

/* Test if section is of type table2
 */
uint8_t ewf_section_is_type_table2( EWF_SECTION *section )
{
	return( ewf_section_is_type( section, "table2" ) );
}

/* Test if section is of type sectors
 */
uint8_t ewf_section_is_type_sectors( EWF_SECTION *section )
{
	return( ewf_section_is_type( section, "sectors" ) );
}

/* Test if section is of type hash
 */
uint8_t ewf_section_is_type_hash( EWF_SECTION *section )
{
	return( ewf_section_is_type( section, "hash" ) );
}

/* Test if section is of type done
 */
uint8_t ewf_section_is_type_done( EWF_SECTION *section )
{
	return( ewf_section_is_type( section, "done" ) );
}

/* Test if section is of type next
 */
uint8_t ewf_section_is_type_next( EWF_SECTION *section )
{
	return( ewf_section_is_type( section, "next" ) );
}

/* Test if section is of type data
 */
uint8_t ewf_section_is_type_data( EWF_SECTION *section )
{
	return( ewf_section_is_type( section, "data" ) );
}

/* Test if section is of type error2
 */
uint8_t ewf_section_is_type_error2( EWF_SECTION *section )
{
	return( ewf_section_is_type( section, "error2" ) );
}

/* Test if section is of type ltree
 */
uint8_t ewf_section_is_type_ltree( EWF_SECTION *section )
{
	return( ewf_section_is_type( section, "ltree" ) );
}

/* Print the section data to a stream
 */
void ewf_section_fprint( FILE *stream, EWF_SECTION *section )
{
	EWF_CRC *calculated_crc = NULL;
	EWF_CRC stored_crc      = 0;
	uint64_t next           = 0;
	uint64_t size           = 0;

	if( stream == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_section_fprint: invalid stream.\n" );

		return;
	}
	if( section == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_section_fprint: invalid section.\n" );

		return;
	}
	calculated_crc = ewf_crc_calculate( (void *) section, ( EWF_SECTION_SIZE - EWF_CRC_SIZE ), 1 );

	if( calculated_crc == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_section_fprint: unable to calculate CRC.\n" );

		return;
	}
	stored_crc = convert_32bit( section->crc );
	next       = convert_64bit( section->next );
	size       = convert_64bit( section->size );

	fprintf( stream, "Section:\n" );
	fprintf( stream, "type: %s\n", section->type );
	fprintf( stream, "next: %" PRIu64 "\n", next );
	fprintf( stream, "size: %" PRIu64 "\n", size );
	fprintf( stream, "crc: %" PRIu32 " ( %" PRIu32 " )\n", stored_crc, *calculated_crc );
	fprintf( stream, "\n" );

	ewf_crc_free( calculated_crc );
}

