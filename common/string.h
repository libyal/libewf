/*
 * String functions
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

#if !defined( _LIBEWF_COMMON_H )
#define _LIBEWF_COMMON_H

#include "libewf_includes.h"

#if defined( HAVE_STRING_H )
#include <string.h>
#endif

#if defined( HAVE_PRINTF_JD )

#define PRIjd_EWF	"jd"

#elif SIZEOF_OFF_T == 8

#define PRIjd_EWF	PRId64

#else

#define PRIjd_EWF	PRId32

#endif

#if defined( HAVE_PRINTF_ZD )

#define PRIzd_EWF	"zd"
#define PRIzu_EWF	"zu"
#define PRIzx_EWF	"zx"

#elif SIZEOF_SIZE_T == 8

#define PRIzd_EWF	PRId32
#define PRIzu_EWF	PRIu32
#define PRIzx_EWF	PRIx32

#else

#define PRIzd_EWF	PRId32
#define PRIzu_EWF	PRIu32
#define PRIzx_EWF	PRIx32

#endif

#include "libewf_char.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_STRLEN )
#define libewf_common_string_length( string ) \
	strlen( string )
#else
#error Missing string length function (strlen)
#endif

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )

#if defined( HAVE_WCSLEN )
#define libewf_common_wide_string_length( string ) \
	wcslen( string )
#else
#error Missing wide character string length function (wcslen)
#endif

#if defined( HAVE_WMEMCPY )
#define libewf_common_wide_memcpy( destination, source, length ) \
	(wchar_t *) wmemcpy( (void *) destination, (void *) source, length )
#else
#error Missing wide character memory copy function (wmemcpy)
#endif

#endif

int libewf_common_open(
     const char *filename,
     uint8_t flags );

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
int libewf_common_wide_open(
     const wchar_t *filename,
     uint8_t flags );
#endif

#if defined( HAVE_WINDOWS_API )

#define libewf_common_read( file_descriptor, buffer, size ) \
	_read( file_descriptor, (void *) buffer, (unsigned int) size )
#else

#define libewf_common_read( file_descriptor, buffer, size ) \
	read( file_descriptor, (void *) buffer, size )
#endif

#if defined( HAVE_WINDOWS_API )

#define libewf_common_lseek( file_descriptor, offset, whence ) \
	_lseeki64( file_descriptor, offset, whence )
#else

#define libewf_common_lseek( file_descriptor, offset, whence ) \
	lseek( file_descriptor, offset, whence ) 
#endif

#if defined( HAVE_WINDOWS_API )

#define libewf_common_write( file_descriptor, buffer, size ) \
	_write( file_descriptor, (const void *) buffer, (unsigned int) size )
#else

#define libewf_common_write( file_descriptor, buffer, size ) \
	write( file_descriptor, (const void *) buffer, size )
#endif

#if defined( HAVE_WINDOWS_API )

#define libewf_common_close( file_descriptor ) \
	_close( file_descriptor )
#else

#define libewf_common_close( file_descriptor ) \
	close( file_descriptor )
#endif

int libewf_common_test_empty_block(
     uint8_t *block_buffer,
     size_t size );

struct tm *libewf_common_localtime(
            const time_t *timestamp );

char *libewf_common_ctime(
       const time_t *timestamp );

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
wchar_t *libewf_common_wide_ctime(
          const time_t *timestamp );
#endif

#if defined( HAVE_WINDOWS_API )
#define libewf_common_mktime( time_elements ) \
	mktime( time_elements )
#elif defined( HAVE_MKTIME )
#define libewf_common_mktime( time_elements ) \
	mktime( time_elements )
#else
#error Missing function mktime
#endif

#if defined( HAVE_WIDE_CHARACTER_TYPE )
int libewf_common_copy_wchar_to_char(
     char *destination,
     const wchar_t *source,
     size_t length );

int libewf_common_copy_char_to_wchar(
     wchar_t *destination,
     const char *source,
     size_t length );
#endif

#if defined( __cplusplus )
}
#endif

#endif

