/*
 * EWF ltree section specification
 *
 * Copyright (c) 2006, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
 *
 * This code is derrived from information and software contributed by
 * - Expert Witness Compression Format specification by Andrew Rosen
 *   (http://www.arsdata.com/SMART/whitepaper.html)
 * - libevf from PyFlag by Michael Cohen
 *   (http://pyflag.sourceforge.net/)
 * - Open SSL for the implementation of the MD5 hash algorithm
 * - Wietse Venema for error handling code
 *
 * Additional credits go to
 * - Robert Jan Mora for testing and other contribution
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
 *   must acknowledge the contribution by people stated above.
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

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libewf_endian.h"
#include "libewf_notify.h"

#include "ewf_crc.h"
#include "ewf_header2.h"
#include "ewf_ltree.h"

/* Allocates memory for a new ewf ltree struct
 */
EWF_LTREE *ewf_ltree_alloc( void )
{
	EWF_LTREE *ltree = (EWF_LTREE *) malloc( EWF_LTREE_SIZE );

	if( ltree == NULL )
	{
		LIBEWF_FATAL_PRINT( "ewf_ltree_alloc: unable to allocate ewf_ltree.\n" );
	}
	memset( ltree, 0, EWF_LTREE_SIZE );

	return( ltree );
}

/* Frees memory of a ltree
 */
void ewf_ltree_free( EWF_LTREE *ltree )
{
	if( ltree == NULL )
	{
		LIBEWF_FATAL_PRINT( "ewf_ltree_free: invalid ltree.\n" );
	}
	free( ltree );
}

/* Reads the ltree from a file descriptor
 */
EWF_LTREE *ewf_ltree_read( int file_descriptor )
{
	EWF_LTREE *ltree = ewf_ltree_alloc();
	ssize_t count    = read( file_descriptor, ltree, EWF_LTREE_SIZE );

	if( count < EWF_LTREE_SIZE )
	{
		ewf_ltree_free( ltree );

		LIBEWF_FATAL_PRINT( "ewf_ltree_read: unable to read ewf_ltree.\n" );
	}
	return( ltree );
}

/* Writes the ltree to a file descriptor
 * Returns a -1 on error, the amount of bytes written on success
 */
ssize_t ewf_ltree_write( EWF_LTREE *ltree, int file_descriptor )
{
	EWF_CRC *crc  = NULL;
	ssize_t count = 0;
	size_t size   = EWF_LTREE_SIZE;

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

	count = write( file_descriptor, ltree, size );

	if( count < size )
	{
		return( -1 );
	}
	return( count );
}

/* Reads the tree data from a file descriptor
 * Test function
 */
EWF_HEADER *ewf_tree_data_read( int file_descriptor, size_t size )
{
	EWF_HEADER *ascii_header        = NULL;
	EWF_HEADER *uncompressed_header = NULL;
	ssize_t count                   = 0;

	uncompressed_header = ewf_header_alloc( size );

	if( uncompressed_header == NULL )
	{
		LIBEWF_FATAL_PRINT( "ewf_ltree_data_read: invalid uncompressed header.\n" );
	}
	count = read( file_descriptor, uncompressed_header, size );

	if( count < size )
	{
		free( uncompressed_header );

		LIBEWF_FATAL_PRINT( "ewf_tree_data_read: unable to read tree_data.\n" );
	}
	ascii_header = ewf_header2_convert_utf16_to_ascii( uncompressed_header, size );

	ewf_header_free( uncompressed_header );

	return( ascii_header );
}

