/*
 * Common code for libewf - wraps external function calls
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

#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "libewf_common.h"
#include "libewf_notify.h"

/* Function to wrap malloc
 */
void *libewf_alloc( uint32_t size )
{
	if( size > 0x80000000 )
	{
		LIBEWF_WARNING_PRINT( "libewf_malloc: invalid size only values below 2^32 are supported.\n" );

		return( NULL );
	}
	return( malloc( (size_t) size ) );
}

/* Function to wrap realloc
 */
void *libewf_realloc( void *buffer, uint32_t size )
{
	if( buffer == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_realloc: invalid buffer.\n" );

		return( NULL );
	}
	if( size > 0x80000000 )
	{
		LIBEWF_WARNING_PRINT( "libewf_realloc: invalid size only values below 2^32 are supported.\n" );

		return( NULL );
	}
	return( realloc( buffer, (size_t) size ) );
}

/* Function to wrap free
 */
void libewf_free( void *buffer )
{
	if( buffer == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_free: invalid buffer.\n" );
	}
	free( buffer );
}

/* Function to wrap memset
 */
void *libewf_memset( void *buffer, uint8_t value, uint32_t count )
{
	if( buffer == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_memset: invalid buffer.\n" );

		return( NULL );
	}
	if( count > 0x80000000 )
	{
		LIBEWF_WARNING_PRINT( "libewf_memset: invalid count only values below 2^32 are supported.\n" );

		return( NULL );
	}
	return( memset( buffer, (int) value, (size_t) count ) );
}

/* Function to wrap memcpy
 */
void *libewf_memcpy( void *destination, const void *source, uint32_t size )
{
	if( source == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_memcpy: invalid source.\n" );

		return( NULL );
	}
	if( destination == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_memcpy: invalid destination.\n" );

		return( NULL );
	}
	return( memcpy( destination, source, (size_t) size ) );
}

/* Function to wrap memcmp
 */
int32_t libewf_memcmp( const void *buffer1, const void *buffer2, uint32_t size )
{
	if( ( buffer1 == NULL ) || ( buffer2 == NULL ) )
	{
		LIBEWF_WARNING_PRINT( "libewf_memcmp: invalid buffer.\n" );

		return( -1 );
	}
	return( (int32_t) memcmp( buffer1, buffer2, (size_t) size ) );
}

/* Function to wrap strdup
 */
char *libewf_strdup( const char *string )
{
	if( string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_strdup: invalid string.\n" );

		return( NULL );
	}
	return( strdup( string ) );
}

/* Function to wrap strlen
 */
uint32_t libewf_strlen( const char *string )
{
	if( string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_strlen: invalid string.\n" );

		return( 0 );
	}
	return( (int32_t) strlen( string ) );
}

/* Function to wrap strchr
 */
char *libewf_strchr( const char *string, uint8_t character )
{
	if( string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_strchr: invalid string.\n" );

		return( NULL );
	}
	return( strchr( string, (int) character ) );
}

/* Function to wrap strncmp
 */
uint8_t libewf_strncmp( const char *string1, const char *string2, uint32_t length )
{
	if( string1 == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_strncmp: invalid string1.\n" );

		return( -1 );
	}
	if( string2 == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_strncmp: invalid string2.\n" );

		return( -1 );
	}
	return( (uint8_t) strncmp( string1, string2, (size_t) length ) );
}

/* Function to wrap read
 */
int32_t libewf_read( int file_descriptor, void *buffer, uint32_t count )
{
	if( file_descriptor <= -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_read: invalid file descriptor.\n" );

		return( -1 );
	}
	if( buffer == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_read: invalid buffer.\n" );

		return( -1 );
	}
	if( count > 0x80000000 )
	{
		LIBEWF_WARNING_PRINT( "libewf_read: invalid count only values below 2^32 are supported.\n" );

		return( -1 );
	}
	return( (int32_t) read( file_descriptor, buffer, (size_t) count ) );
}

/* Function to wrap write
 */
int32_t libewf_write( int file_descriptor, const void *buffer, uint32_t count )
{
	if( file_descriptor <= -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write: invalid file descriptor.\n" );

		return( -1 );
	}
	if( buffer == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write: invalid buffer.\n" );

		return( -1 );
	}
	if( count > 0x80000000 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write: invalid count only values below 2^32 are supported.\n" );

		return( -1 );
	}
	return( (int32_t) write( file_descriptor, buffer, (size_t) count ) );
}

/* Function to allocated wiped memory
 */
void *libewf_alloc_cleared( uint32_t size, int8_t clear_value )
{
	void *allocated_buffer = NULL;
	void *cleared_buffer   = NULL;

	allocated_buffer = libewf_alloc( size );

	if( allocated_buffer == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_alloc_cleared: unable to allocate buffer.\n" );

		return( NULL );
	}
	cleared_buffer = libewf_memset( allocated_buffer, clear_value, size );

	if( cleared_buffer == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_alloc_cleared: unable to clear buffer.\n" );

		libewf_free( allocated_buffer );

		return( NULL );
	}
	return( cleared_buffer );
}

/* Function to reallocated fully wiped memory
 */
void *libewf_realloc_full_cleared( void *buffer, uint32_t previous_size, uint32_t new_size, int8_t clear_value )
{
	void *reallocated_buffer = NULL;
	void *cleared_buffer     = NULL;

	if( previous_size > 0x80000000 )
	{
		LIBEWF_WARNING_PRINT( "libewf_realloc_full_cleared: invalid size only values below 2^32 are supported.\n" );

		return( NULL );
	}
	if( new_size <= previous_size )
	{
		LIBEWF_WARNING_PRINT( "libewf_realloc_full_cleared: new size must be greater than previous size.\n" );

		return( NULL );
	}
	reallocated_buffer = libewf_realloc( buffer, (size_t) new_size );

	if( reallocated_buffer == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_realloc_full_cleared: unable to reallocate buffer.\n" );

		return( NULL );
	}
	cleared_buffer = libewf_memset( reallocated_buffer, clear_value, new_size );

	if( cleared_buffer == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_realloc_full_cleared: unable to clear buffer.\n" );

		libewf_free( reallocated_buffer );

		return( NULL );
	}
	return( cleared_buffer );
}

/* Function to reallocated wiped newly allocated memory
 */
void *libewf_realloc_new_cleared( void *buffer, uint32_t previous_size, uint32_t new_size, int8_t clear_value )
{
	void *reallocated_buffer = NULL;
	void *cleared_buffer     = NULL;

	if( previous_size > 0x80000000 )
	{
		LIBEWF_WARNING_PRINT( "libewf_realloc_new_cleared: invalid size only values below 2^32 are supported.\n" );

		return( NULL );
	}
	if( new_size <= previous_size )
	{
		LIBEWF_WARNING_PRINT( "libewf_realloc_new_cleared: new size must be greater than previous size.\n" );

		return( NULL );
	}
	reallocated_buffer = libewf_realloc( buffer, (size_t) new_size );

	if( reallocated_buffer == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_realloc_new_cleared: unable to reallocate buffer.\n" );

		return( NULL );
	}
	cleared_buffer = libewf_memset( ( reallocated_buffer + previous_size ), clear_value, ( new_size - previous_size ) );

	if( cleared_buffer == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_realloc_new_cleared: unable to clear buffer.\n" );

		libewf_free( reallocated_buffer );

		return( NULL );
	}
	return( cleared_buffer );
}

