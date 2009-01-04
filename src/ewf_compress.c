/*
 * EWF compression handling
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
#include <zlib.h>

#include "libewf_notify.h"

#include "ewf_compress.h"

/* Compresses data, wraps zlib uncompress function
 * Returns a 1 if compression is successful, -1 on error
 */
int8_t ewf_compress( uint8_t *compressed_data, uint32_t *compressed_size, uint8_t *uncompressed_data, uint32_t uncompressed_size, int8_t compression_level )
{
	uLongf safe_compressed_size = 0;
	int zlib_compression_level  = 0;
	int result                  = 0;

	if( compressed_data == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_compress: invalid compressed data buffer.\n" );

		return( -1 );
	}
	if( uncompressed_data == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_compress: invalid uncompressed data buffer.\n" );

		return( -1 );
	}
	if( compressed_size == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_compress: invalid compressed size.\n" );

		return( -1 );
	}
	if( compression_level == EWF_COMPRESSION_DEFAULT )
	{
		zlib_compression_level = Z_BEST_SPEED;
	}
	else if( compression_level == EWF_COMPRESSION_FAST )
	{
		zlib_compression_level = Z_BEST_SPEED;
	}
	else if( compression_level == EWF_COMPRESSION_BEST )
	{
		zlib_compression_level = Z_BEST_COMPRESSION;
	}
	else if( compression_level == EWF_COMPRESSION_NONE )
	{
		zlib_compression_level = Z_NO_COMPRESSION;
	}
	else
	{
		LIBEWF_WARNING_PRINT( "ewf_compress: unsupported compression level.\n" );

		return( -1 );
	}
	safe_compressed_size = *compressed_size;

	result = compress2( (Bytef *) compressed_data, &safe_compressed_size, (Bytef *) uncompressed_data, (uLong) uncompressed_size, zlib_compression_level );

	if( result == Z_OK )
	{
		*compressed_size = safe_compressed_size;

		return( 1 );
	}
	else if( result == Z_BUF_ERROR )
	{
		LIBEWF_WARNING_PRINT( "ewf_compress: unable to write compressed data: target buffer too small.\n" );
	}
	else if( result == Z_MEM_ERROR )
	{
		LIBEWF_WARNING_PRINT( "ewf_compress: unable to write compressed data: insufficient memory.\n" );
	}
	else
	{
		LIBEWF_WARNING_PRINT( "ewf_compress: zlib returned undefined error: %d.\n", result );
	}
	return( -1 );
}

/* Uncompresses data, wraps zlib uncompress function
 * Returns a 1 if compression is successful, a 0 if unsuccessful, -1 on error
 */
int8_t ewf_uncompress( uint8_t *uncompressed_data, uint32_t *uncompressed_size, uint8_t *compressed_data, uint32_t compressed_size )
{
	uLongf safe_uncompressed_size = 0;
	int result                    = 0;

	if( uncompressed_data == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_uncompress: invalid uncompressed data buffer.\n" );

		return( -1 );
	}
	if( compressed_data == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_uncompress: invalid compressed data buffer.\n" );

		return( -1 );
	}
	if( uncompressed_size == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_uncompress: invalid uncompressed size.\n" );

		return( -1 );
	}
	safe_uncompressed_size = *uncompressed_size;

	result = uncompress( (Bytef *) uncompressed_data, &safe_uncompressed_size, (Bytef *) compressed_data, (uLong) compressed_size );

	if( result == Z_OK )
	{
		*uncompressed_size = safe_uncompressed_size;

		return( 1 );
	}
	else if( result == Z_DATA_ERROR )
	{
		return( 0 );
	}
	else if( result == Z_BUF_ERROR )
	{
		LIBEWF_WARNING_PRINT( "ewf_uncompress: unable to read compressed data: target buffer too small.\n" );
	}
	else if( result == Z_MEM_ERROR )
	{
		LIBEWF_WARNING_PRINT( "ewf_uncompress: unable to read compressed data: insufficient memory.\n" );
	}
	else
	{
		LIBEWF_WARNING_PRINT( "ewf_uncompress: zlib returned undefined error: %d.\n", result );
	}
	return( -1 );
}

