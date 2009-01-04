/*
 * EWF ltree section
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
#include "libewf_endian.h"
#include "libewf_notify.h"

#include "ewf_crc.h"
#include "ewf_header2.h"
#include "ewf_ltree.h"

/* Allocates memory for a new ewf ltree struct
 * Returns a pointer to the new instance, NULL on error
 */
EWF_LTREE *ewf_ltree_alloc( void )
{
	EWF_LTREE *ltree = NULL;

	ltree = (EWF_LTREE *) libewf_alloc_cleared( EWF_LTREE_SIZE, 0 );

	if( ltree == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_ltree_alloc: unable to allocate ewf_ltree.\n" );

		return( NULL );
	}
	return( ltree );
}

/* Frees memory of a ltree
 */
void ewf_ltree_free( EWF_LTREE *ltree )
{
	if( ltree == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_ltree_free: invalid ltree.\n" );

		return;
	}
	libewf_free( ltree );
}

/* Reads the ltree from a file descriptor
 * Returns a pointer to the new instance, NULL on error
 */
EWF_LTREE *ewf_ltree_read( int file_descriptor )
{
	EWF_LTREE *ltree = NULL;
	uint32_t size    = EWF_LTREE_SIZE;
	int32_t count    = 0;

	ltree = ewf_ltree_alloc();

	if( ltree == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_ltree_read: unable to read ewf_ltree.\n" );

		return( NULL );
	}
	count = libewf_read( file_descriptor, ltree, size );

	if( count < size )
	{
		ewf_ltree_free( ltree );

		LIBEWF_WARNING_PRINT( "ewf_ltree_read: unable to read ewf_ltree.\n" );

		return( NULL );
	}
	return( ltree );
}

/* Writes the ltree to a file descriptor
 * Returns a -1 on error, the amount of bytes written on success
 */
int32_t ewf_ltree_write( EWF_LTREE *ltree, int file_descriptor )
{
/*
	EWF_CRC *crc  = NULL;
*/
	uint32_t size = EWF_LTREE_SIZE;
	int32_t count = 0;

	if( ltree == NULL )
	{
		LIBEWF_VERBOSE_PRINT( "ewf_ltree_write: invalid ltree.\n" );

		return( -1 );
	}
/*
	crc = ewf_crc_calculate( (void *) ltree, ( size - EWF_CRC_SIZE ), 1 );

	if( ltree == NULL )
	{
		LIBEWF_VERBOSE_PRINT( "ewf_ltree_write: unable to calculate CRC.\n" );

		return( -1 );
	}
	revert_32bit( *crc, ltree->crc );

	ewf_crc_free( crc );
*/

	count = libewf_write( file_descriptor, ltree, size );

	if( count < size )
	{
		return( -1 );
	}
	return( count );
}

/* Reads the tree data from a file descriptor
 * Test function
 * Returns a pointer to the new instance, NULL on error
 */
EWF_HEADER *ewf_tree_data_read( int file_descriptor, uint32_t size )
{
	EWF_HEADER *ascii_header        = NULL;
	EWF_HEADER *uncompressed_header = NULL;
	int32_t count                   = 0;

	uncompressed_header = ewf_header_alloc( size );

	if( uncompressed_header == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_ltree_data_read: invalid uncompressed header.\n" );

		return( NULL );
	}
	count = libewf_read( file_descriptor, uncompressed_header, size );

	if( count < size )
	{
		ewf_header_free( uncompressed_header );

		LIBEWF_WARNING_PRINT( "ewf_tree_data_read: unable to read tree_data.\n" );

		return( NULL );
	}
	ascii_header = ewf_header2_convert_utf16_to_ascii( uncompressed_header, size );

	ewf_header_free( uncompressed_header );

	return( ascii_header );
}

