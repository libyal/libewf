/*
 * EWF character type string functions
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
#include "libewf_notify.h"

#include "ewf_compress.h"
#include "ewf_string.h"

/* Uncompresses the string
 * Returns a pointer to the new instance, NULL on error
 */
EWF_CHAR *ewf_string_uncompress( EWF_CHAR *compressed_string, size_t *size )
{
	EWF_CHAR *uncompressed_string = NULL;
	EWF_CHAR *reallocation        = NULL;
	size_t compressed_size        = 0;
	int8_t result                 = 0;

	if( compressed_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_string_uncompress: invalid compressed string.\n" );

		return( NULL );
	}
	if( size == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_string_uncompress: invalid size.\n" );

		return( NULL );
	}
	compressed_size = *size;

	/* On average the uncompressed string will be twice as large
	 */
	*size *= 2;

	/* Add a byte for the end of string
	 */
	uncompressed_string = (EWF_CHAR *) libewf_common_alloc( EWF_CHAR_SIZE * ( *size + 1 ) );

	if( uncompressed_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_string_uncompress: unable to allocate uncompressed string.\n" );

		return( NULL );
	}
	result = ewf_uncompress( (uint8_t *) uncompressed_string, size, (uint8_t *) compressed_string, compressed_size );

	while( ( result == -1 ) && ( *size > 0 ) )
	{
		/* Add a byte for the end of string
		 */
		reallocation = (EWF_CHAR *) libewf_common_realloc( uncompressed_string, ( *size + 1 ) );

		if( reallocation == NULL )
		{
			LIBEWF_WARNING_PRINT( "ewf_string_uncompress: unable to reallocate uncompressed string.\n" );

			libewf_common_free( uncompressed_string );

			return( NULL );
		}
		uncompressed_string = reallocation;

		result = ewf_uncompress( (uint8_t *) uncompressed_string, size, (uint8_t *) compressed_string, compressed_size );
	}
	if( result == -1 )
	{
		LIBEWF_WARNING_PRINT( "ewf_string_uncompress: unable to uncompress string.\n" );

		libewf_common_free( uncompressed_string );

		return( NULL );
	}
	uncompressed_string[ *size ] = (EWF_CHAR) '\0';

	*size += 1;

	return( uncompressed_string );
}

/* Compresses the string
 * Returns a pointer to the new instance, NULL on error
 */
EWF_CHAR *ewf_string_compress( EWF_CHAR *uncompressed_string, size_t *size, int8_t compression_level )
{
	EWF_CHAR *compressed_string  = NULL;
	EWF_CHAR *reallocation       = NULL;
	size_t uncompressed_size     = 0;
	int8_t result                = 0;

	if( uncompressed_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_string_compress: invalid uncompressed string.\n" );

		return( NULL );
	}
	if( size == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_string_compress: invalid size.\n" );

		return( NULL );
	}
	uncompressed_size = *size;
	compressed_string = (EWF_CHAR *) libewf_common_alloc( EWF_CHAR_SIZE * *size );

	if( compressed_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_string_compress: unable to allocate compressed string.\n" );

		return( NULL );
	}
	result = ewf_compress( (uint8_t *) compressed_string, size, (uint8_t *) uncompressed_string, uncompressed_size, compression_level );

	if( ( result == -1 ) && ( *size > 0 ) )
	{
		reallocation = (EWF_CHAR *) libewf_common_realloc( compressed_string, *size );

		if( reallocation == NULL )
		{
			LIBEWF_WARNING_PRINT( "ewf_string_compress: unable to reallocate compressed string.\n" );

			libewf_common_free( compressed_string );

			return( NULL );
		}
		compressed_string = reallocation;

		result = ewf_compress( (uint8_t *) compressed_string, size, (uint8_t *) uncompressed_string, uncompressed_size, compression_level );
	}
	if( result == -1 )
	{
		LIBEWF_WARNING_PRINT( "ewf_string_compress: unable to compress string.\n" );

		libewf_common_free( compressed_string );

		return( NULL );
	}
	return( compressed_string );
}

/* Reads the string from a file descriptor into a buffer
 * Returns the amount of bytes read on success, -1 on error
 */
ssize_t ewf_string_read_to_buffer( EWF_CHAR *string, int file_descriptor, size_t size )
{
	ssize_t count = 0;

	if( string == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_string_read_to_buffer: invalid string.\n" );

		return( -1 );
	}
	size *= EWF_CHAR_SIZE;

	if( size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "ewf_string_read_to_buffer: invalid size value exceeds maximum.\n" );

		return( -1 );
	}
	count = libewf_common_read( file_descriptor, string, size );

	if( count < (ssize_t) size )
	{
		LIBEWF_WARNING_PRINT( "ewf_string_read_to_buffer: unable to read string.\n" );

		return( -1 );
	}
	return( count );
}

/* Writes the string to a file descriptor
 * Returns the amount of bytes written on success, -1 on error
 */
ssize_t ewf_string_write_from_buffer( EWF_CHAR *string, int file_descriptor, size_t size )
{
	ssize_t count = 0;

	if( string == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_string_write_from_buffer: invalid string.\n" );

		return( -1 );
	}
	size *= EWF_CHAR_SIZE;

	if( size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "ewf_string_write_from_buffer: invalid size value exceeds maximum.\n" );

		return( -1 );
	}
	count = libewf_common_write( file_descriptor, string, size );

	if( count < (ssize_t) size )
	{
		LIBEWF_WARNING_PRINT( "ewf_string_write_from_buffer: unable to write string.\n" );

		return( -1 );
	}
	return( count );
}

/* Reads a string from a file descriptor
 * Returns a pointer to the new instance of the string, NULL on error
 */
EWF_CHAR *ewf_string_read( int file_descriptor, size_t size )
{
	EWF_CHAR *string = NULL;

	if( size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "ewf_string_read: invalid size value exceeds maximum.\n" );

		return( NULL );
	}
	string = (EWF_CHAR *) libewf_common_alloc( EWF_CHAR_SIZE * size );

	if( string == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_string_read: unable to allocate string.\n" );

		return( NULL );
	}
	if( ewf_string_read_to_buffer( string, file_descriptor, size ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "ewf_string_read: unable to read string.\n" );

		libewf_common_free( string );

		return( NULL );
	}
	return( string );
}

/* Reads a compressed string from a file descriptor and uncompresses it
 * Returns a pointer to the new instance of the uncompressed string, NULL on error
 */
EWF_CHAR *ewf_string_read_compressed( int file_descriptor, size_t *size )
{
	EWF_CHAR *uncompressed_string = NULL;
	EWF_CHAR *compressed_string   = NULL;

	if( size == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_string_read_compressed: invalid size.\n" );

		return( NULL );
	}
	compressed_string = ewf_string_read( file_descriptor, *size );

	if( compressed_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_string_read_compressed: unable to read compressed string.\n" );

		return( NULL );
	}
	uncompressed_string = ewf_string_uncompress( compressed_string, size );

	libewf_common_free( compressed_string );

	if( uncompressed_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_string_read_compressed: unable to uncompress string.\n" );

		return( NULL );
	}
	return( uncompressed_string );
}

/* Writes an uncompressed string compressed to a file descriptor
 * Returns the amount of bytes written on success, -1 on error
 */
ssize_t ewf_string_write_compressed( EWF_CHAR *uncompressed_string, int file_descriptor, size_t *size, int8_t compression_level )
{
	EWF_CHAR *compressed_string = NULL;
	ssize_t count               = 0;

	if( uncompressed_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_string_write_compressed: invalid uncompressed string.\n" );

		return( -1 );
	}
	if( size == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_string_write_compressed: invalid size.\n" );

		return( -1 );
	}
	compressed_string = ewf_string_compress( uncompressed_string, size, compression_level );

        if( compressed_string == NULL )
        {
		LIBEWF_WARNING_PRINT( "ewf_string_write_compressed: unable to compress string.\n" );

		return( -1 );
	}
	count = ewf_string_write( compressed_string, file_descriptor, *size );

	libewf_common_free( compressed_string );

	return( count );
}

