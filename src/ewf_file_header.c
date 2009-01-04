/*
 * EWF file header
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

#include "libewf_common.h"
#include "libewf_notify.h"

#include "ewf_file_header.h"

uint8_t evf_file_signature[] = { 0x45, 0x56, 0x46, 0x09, 0x0D, 0x0A, 0xFF, 0x00 };
uint8_t lvf_file_signature[] = { 0x4c, 0x56, 0x46, 0x09, 0x0D, 0x0A, 0xFF, 0x00 };

/* Allocates memory for a new ewf file header struct
 * Returns a pointer to the new instance, NULL on error
 */
EWF_FILE_HEADER *ewf_file_header_alloc( void )
{
	EWF_FILE_HEADER *file_header = NULL;
	EWF_FILE_HEADER *data_set    = NULL;

	file_header = (EWF_FILE_HEADER *) libewf_alloc_cleared( EWF_FILE_HEADER_SIZE, 0 );

	if( file_header == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_file_header_alloc: unable to allocate ewf_file_header.\n" );

		return( NULL );
	}
	data_set = (void *) libewf_memcpy( (uint8_t *) file_header, (uint8_t *) evf_file_signature, 8 );

	if( data_set == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_file_header_alloc: unable to set data.\n" );

		libewf_free( file_header );

		return( NULL );
	}
	file_header->fields_start = 1;

	return( file_header );
}

/* Frees memory of a file header
 */
void ewf_file_header_free( EWF_FILE_HEADER *file_header )
{
	if( file_header == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_file_header_free: invalid file_header.\n" );

		return;
	}
	libewf_free( file_header );
}

/* Validates the EWF file signature (EWF-E01, EWF-S01, EWF-L01)
 * Returns 1 if the signature matches, 0 otherwise
 */
uint8_t ewf_file_header_check_signature( uint8_t *signature )
{
	if( signature == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_file_header_check_signature: invalid signature.\n" );

		return( 0 );
	}
	return( ( libewf_memcmp( evf_file_signature, signature, sizeof( evf_file_signature ) ) == 0 )
	|| ( libewf_memcmp( lvf_file_signature, signature, sizeof( lvf_file_signature ) ) == 0 ) );
}

/* Reads the file header from a file descriptor
 * Returns a pointer to the new instance, NULL on error
 */
EWF_FILE_HEADER *ewf_file_header_read( int file_descriptor )
{
	EWF_FILE_HEADER *file_header = NULL;
	uint32_t size                = EWF_FILE_HEADER_SIZE;
	int32_t count                = 0;

	file_header = ewf_file_header_alloc();

	if( file_header == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_file_header_read: unable create file header.\n" );

		return( NULL );
	}
	count = libewf_read( file_descriptor, file_header, size );

	if( count < size )
	{
		ewf_file_header_free( file_header );

		LIBEWF_WARNING_PRINT( "ewf_file_header_read: unable to read file header.\n" );

		return( NULL );
	}
	if( ewf_file_header_check_signature( file_header->signature ) == 0 )
	{
		ewf_file_header_free( file_header );

		LIBEWF_WARNING_PRINT( "ewf_file_header_read: file signature does not match.\n" );

		return( NULL );
	}
	return( file_header );
}

/* Writes the file header to a file descriptor
 * Returns a -1 on error, the amount of bytes written on success
 */
int32_t ewf_file_header_write( EWF_FILE_HEADER *file_header, int file_descriptor )
{
	uint32_t size = 0;
	int32_t count = 0;

	if( file_header == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_file_header_write: invalid file header.\n" );

		return( -1 );
	}
	count = libewf_write( file_descriptor, file_header, size );

	if( count < size )
	{
		LIBEWF_WARNING_PRINT( "ewf_file_header_write: error writing file header.\n" );

		return( -1 );
	}
	return( count );
}

