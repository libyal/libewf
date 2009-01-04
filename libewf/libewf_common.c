/*
 * Common code for libewf - wraps external function calls
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
#include <memory.h>
#include <notify.h>
#include <types.h>

#if defined( HAVE_STRING_H )
#include <string.h>
#endif

#include <libewf/definitions.h>

#include "libewf_common.h"

/* Function to wrap open()
 */
int libewf_common_open(
     const char *filename,
     uint8_t flags )
{
	static char *function = "libewf_common_open";
	int file_descriptor   = 0;
	int open_flags        = 0;

	if( filename == NULL )
	{
		notify_warning_printf( "%s: invalid filename.\n",
		 function );

		return( -1 );
	}
	/* Ignore flags other than the read and write flags
	 */
	flags &= ( LIBEWF_FLAG_READ | LIBEWF_FLAG_WRITE );

	if( flags == LIBEWF_OPEN_READ )
	{
#if defined( HAVE_WINDOWS_API )
		open_flags = _O_RDONLY | _O_BINARY;
#else
		open_flags = O_RDONLY;
#endif
	}
	else if( flags == LIBEWF_OPEN_WRITE )
	{
#if defined( HAVE_WINDOWS_API )
		open_flags = _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY;
#else
		open_flags = O_WRONLY | O_CREAT | O_TRUNC;
#endif
	}
	else if( flags == LIBEWF_OPEN_READ_WRITE )
	{
#if defined( HAVE_WINDOWS_API )
		open_flags = _O_RDWR | _O_BINARY;
#else
		open_flags = O_RDWR ;
#endif
	}
	else
	{
		notify_warning_printf( "%s: flags not supported.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_WINDOWS_API )
	if( _sopen_s(
	     &file_descriptor,
	     filename,
	     open_flags,
	     _SH_DENYRW,
	     ( _S_IREAD | _S_IWRITE ) ) != 0 )
#else
	file_descriptor = open(
	                   filename,
	                   open_flags,
	                   0644 );

	if( file_descriptor == -1 )
#endif
	{
		notify_warning_printf( "%s: error opening file.\n",
		 function );

		return( -1 );
	}
	return( file_descriptor );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )

/* Function to wrap wide character equivalent of open()
 */
int libewf_common_wide_open(
     const wchar_t *filename,
     uint8_t flags )
{
	static char *function = "libewf_common_wide_open";
#if defined( HAVE_WINDOWS_API )
	int file_descriptor   = 0;
#endif
	int open_flags        = 0;

	if( filename == NULL )
	{
		notify_warning_printf( "%s: invalid filename.\n",
		 function );

		return( -1 );
	}
	/* Ignore flags other than the read and write flags
	 */
	flags &= ( LIBEWF_FLAG_READ | LIBEWF_FLAG_WRITE );

	if( flags == LIBEWF_OPEN_READ )
	{
#if defined( HAVE_WINDOWS_API )
		open_flags = _O_RDONLY | _O_BINARY;
#else
		open_flags = O_RDONLY;
#endif
	}
	else if( flags == LIBEWF_OPEN_WRITE )
	{
#if defined( HAVE_WINDOWS_API )
		open_flags = _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY;
#else
		open_flags = O_WRONLY | O_CREAT | O_TRUNC;
#endif
	}
	else if( flags == LIBEWF_OPEN_READ_WRITE )
	{
#if defined( HAVE_WINDOWS_API )
		open_flags = _O_RDWR | _O_BINARY;
#else
		open_flags = O_RDWR ;
#endif
	}
	else
	{
		notify_warning_printf( "%s: flags not supported.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_WINDOWS_API )
	if( _wsopen_s(
	     &file_descriptor,
	     filename,
	     open_flags,
	     _SH_DENYRW,
	     ( _S_IREAD | _S_IWRITE ) ) != 0 )
	{
		notify_warning_printf( "%s: error opening file.\n",
		 function );

		return( -1 );
	}
	return( file_descriptor );
#else
#error libewf_common_wide_open: missing wide character equivalent of open()
#endif
}
#endif

/* Check for empty block, a block that contains the same value for every byte
 * Returns 1 if block is empty, or 0 otherwise
 */
int libewf_common_test_empty_block(
     uint8_t *block_buffer,
     size_t size )
{
	static char *function = "libewf_common_test_empty_block";
	size_t iterator       = 0;

	if( block_buffer == NULL )
	{
		notify_warning_printf( "%s: invalid block buffer.\n",
		 function );

		return( 0 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( 0 );
	}
	for( iterator = 1; iterator < size; iterator++ )
	{
		if( block_buffer[ 0 ] != block_buffer[ iterator ] )
		{
			return( 0 );
		}
	}
	return( 1 );
}

#if defined( HAVE_WINDOWS_API )
#define libewf_common_localtime_r( timestamp, time_elements ) \
	localtime_s( time_elements, timestamp )

#elif defined( HAVE_LOCALTIME_R )
#define libewf_common_localtime_r( timestamp, time_elements ) \
	localtime_r( timestamp, time_elements )

#endif

/* Returns a structured representation of a time using the local time zone, or NULL on error
 */
struct tm *libewf_common_localtime(
            const time_t *timestamp )
{
#if !defined( libewf_common_localtime_r ) && defined( HAVE_LOCALTIME )
	struct tm *static_time_elements = NULL;
#endif
	struct tm *time_elements        = NULL;
	static char *function           = "libewf_common_localtime";

	if( timestamp == NULL )
	{
		notify_warning_printf( "%s: invalid time stamp.\n",
		 function );

		return( NULL );
	}
	time_elements = (struct tm *) memory_allocate(
	                               sizeof( struct tm ) );

	if( time_elements == NULL )
	{
		notify_warning_printf( "%s: unable to create time elements.\n",
		 function );

		return( NULL );
	}
#if defined( libewf_common_localtime_r )
#if defined( HAVE_WINDOWS_API )
	if( libewf_common_localtime_r(
	     timestamp,
	     time_elements ) != 0 )
#else
	if( libewf_common_localtime_r(
	     timestamp,
	     time_elements ) == NULL )
#endif
	{
		notify_warning_printf( "%s: unable to set time elements.\n",
		 function );

		memory_free(
		 time_elements );

		return( NULL );
	}
	return( time_elements );
#elif defined( HAVE_LOCALTIME )
	static_time_elements = localtime(
	                        timestamp );

	if( static_time_elements == NULL )
	{
		notify_warning_printf( "%s: unable to create static time elements.\n",
		 function );

		memory_free(
		 time_elements );

		return( NULL );
	}
	if( memory_copy(
	     time_elements,
	     static_time_elements,
	     sizeof( struct tm ) ) == NULL )
	{
		notify_warning_printf( "%s: unable to set time elements.\n",
		 function );

		memory_free(
		 time_elements );

		return( NULL );
	}
#else
#error Missing equivalent of function localtime
#endif
}

#if defined( HAVE_WINDOWS_API )
#define libewf_common_ctime_r( timestamp, string, size ) \
	ctime_s( string, size, timestamp )

#elif defined( HAVE_CTIME_R )
#if defined( HAVE_CTIME_R_SIZE )
#define libewf_common_ctime_r( timestamp, string, size ) \
	ctime_r( timestamp, string, size )

#else
#define libewf_common_ctime_r( timestamp, string, size ) \
	ctime_r( timestamp, string )

#endif
#endif

/* Returns a structured representation of a time using the local time zone, or NULL on error
 */
char *libewf_common_ctime(
       const time_t *timestamp )
{
	static char *function    = "libewf_common_ctime";
#if !defined( libewf_common_ctime_r ) && defined( HAVE_CTIME )
	char *static_time_string = NULL;
#endif
	char *time_string        = NULL;
	size_t time_string_size  = 32;

	if( timestamp == NULL )
	{
		notify_warning_printf( "%s: invalid time stamp.\n",
		 function );

		return( NULL );
	}
	time_string = (char *) memory_allocate(
	                        sizeof( char ) * time_string_size );

	if( time_string == NULL )
	{
		notify_warning_printf( "%s: unable to create time string.\n",
		 function );

		return( NULL );
	}
#if defined( libewf_common_ctime_r )
#if defined( HAVE_WINDOWS_API )
	if( libewf_common_ctime_r(
             timestamp,
             time_string,
             time_string_size ) != 0 )
#else
	if( libewf_common_ctime_r(
	     timestamp,
	     time_string,
	     time_string_size ) == NULL )
#endif
	{
		notify_warning_printf( "%s: unable to set time string.\n",
		 function );

		memory_free(
		 time_string );

		return( NULL );
	}
	return( time_string );
#elif defined( HAVE_CTIME )
	static_time_string = ctime(
	                      timestamp );

	if( static_time_string == NULL )
	{
		notify_warning_printf( "%s: unable to create static time string.\n",
		 function );

		memory_free(
		 time_string );

		return( NULL );
	}
	if( libewf_common_string_copy(
             time_string,
             static_time_string,
             time_string_size ) == NULL )
	{
		notify_warning_printf( "%s: unable to set time string.\n",
		 function );

		memory_free(
		 time_string );

		return( NULL );
	}
	return( time_string );
#else
#error Missing equivalent of function ctime
#endif
}

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )

#if defined( HAVE_WINDOWS_API )
#define libewf_common_wide_ctime_r( timestamp, string, size ) \
	_wctime_s( string, size, timestamp )

#else
#error Missing wide character equivalent of function ctime()
#endif

/* Returns a structured representation of a time using the local time zone, or NULL on error
 */
wchar_t *libewf_common_wide_ctime(
          const time_t *timestamp )
{
	static char *function   = "libewf_common_wide_ctime";
	wchar_t *time_string    = NULL;
	size_t time_string_size = 32;

	if( timestamp == NULL )
	{
		notify_warning_printf( "%s: invalid time stamp.\n",
		 function );

		return( NULL );
	}
	time_string = (wchar_t *) memory_allocate(
	                           sizeof( wchar_t ) * time_string_size );

	if( time_string == NULL )
	{
		notify_warning_printf( "%s: unable to create time string.\n",
		 function );

		return( NULL );
	}
#if defined( libewf_common_wide_ctime_r )
	if( libewf_common_wide_ctime_r(
	     timestamp,
	     time_string,
	     time_string_size ) != 0 )
	{
		notify_warning_printf( "%s: unable to set time string.\n",
		 function );

		memory_free(
		 time_string );

		return( NULL );
	}
	return( time_string );
#endif
}
#endif

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Copies the source string (of wchar_t) into the destination string (of char) for a certain size
 * Terminates the destination string with \0 at ( size - 1 )
 * Returns 1 if successful, -1 on error
 */
int libewf_common_copy_wchar_to_char(
     char *destination,
     const wchar_t *source,
     size_t size )
{
	static char *function = "libewf_common_copy_wchar_to_char";
	size_t iterator       = 0;

	if( source == NULL )
	{
		notify_warning_printf( "%s: invalid source.\n",
		 function );

		return( -1 );
	}
	if( destination == NULL )
	{
		notify_warning_printf( "%s: invalid destination.\n",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	for( iterator = 0; iterator < size; iterator++ )
	{
		destination[ iterator ] = (char) wctob(
		                                  (wint_t) source[ iterator ] );

		/* If character is out of the basic ASCII range use '_' as a place holder
		 */
		if( destination[ iterator ] == EOF )
		{
			destination[ iterator ] = '_';
		}
	}
	destination[ size - 1 ] = (char) '\0';

	return( 1 );
}

/* Copies the source string (of char) into the destination string (of wchar_t) for a certain size
 * Terminates the destination string with \0 at ( size - 1 )
 * Returns 1 if successful, -1 on error
 */
int libewf_common_copy_char_to_wchar(
     wchar_t *destination,
     const char *source,
     size_t size )
{
	static char *function = "libewf_common_copy_char_to_wchar";
	size_t iterator       = 0;

	if( source == NULL )
	{
		notify_warning_printf( "%s: invalid source.\n",
		 function );

		return( -1 );
	}
	if( destination == NULL )
	{
		notify_warning_printf( "%s: invalid destination.\n",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	for( iterator = 0; iterator < size; iterator++ )
	{
		destination[ iterator ] = (wchar_t) btowc(
		                                     (int) source[ iterator ] );
	}
	destination[ size - 1 ] = (wchar_t) '\0';

	return( 1 );
}

#endif

