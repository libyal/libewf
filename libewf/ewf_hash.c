/*
 * EWF hash section
 *
 * Copyright (c) 2006-2007, Joachim Metz <forensics@hoffmannbv.nl>,
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

#include "libewf_includes.h"

#include "libewf_common.h"
#include "libewf_endian.h"
#include "libewf_notify.h"

#include "ewf_crc.h"
#include "ewf_hash.h"

/* Reads the hash from a file descriptor
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t ewf_hash_read( EWF_HASH *hash, int file_descriptor )
{
	ssize_t count = 0;
	size_t size   = EWF_HASH_SIZE;

	if( hash == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_hash_read: unable to create hash.\n" );

		return( -1 );
	}
	count = libewf_common_read( file_descriptor, hash, size );

	if( count < (ssize_t) size )
	{
		LIBEWF_WARNING_PRINT( "ewf_hash_read: unable to read hash.\n" );

		return( -1 );
	}
	return( count );
}

/* Writes the hash to a file descriptor
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t ewf_hash_write( EWF_HASH *hash, int file_descriptor )
{
	EWF_CRC crc   = 0;
	ssize_t count = 0;
	size_t size   = EWF_HASH_SIZE;

	if( hash == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_hash_write: invalid hash.\n" );

		return( -1 );
	}
	if( ewf_crc_calculate( &crc, (uint8_t *) hash, ( size - EWF_CRC_SIZE ), 1 ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "ewf_hash_write: unable to calculate CRC.\n" );

		return( -1 );
	}
	if( libewf_endian_revert_32bit( crc, hash->crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "ewf_hash_write: unable to revert CRC value.\n" );

		return( -1 );
	}
	count = libewf_common_write( file_descriptor, hash, size );

	if( count < (ssize_t) size )
	{
		LIBEWF_WARNING_PRINT( "ewf_hash_write: unable to write hash.\n" );

		return( -1 );
	}
	return( count );
}

