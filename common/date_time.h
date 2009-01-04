/*
 * Date and time functions
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

#if !defined( _DATE_TIME_H )
#define _DATE_TIME_H

#include "common.h"

#include <time.h>

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )

#if defined( HAVE_WINDOWS_API )
#define date_time_wctime_r( timestamp, string, size ) \
	_wctime_s( string, size, timestamp )

#define date_time_wctime( timestamp, string, size ) \
	_wctime_s( string, size, timestamp )

#else
#error Missing wide character equivalent of function ctime()
#endif

#endif

#if defined( HAVE_WINDOWS_API )
#define date_time_localtime_r( timestamp, time_elements ) \
	localtime_s( time_elements, timestamp )

#elif defined( HAVE_LOCALTIME_R )
#define date_time_localtime_r( timestamp, time_elements ) \
	localtime_r( timestamp, time_elements )

#elif !defined( HAVE_LOCALTIME )
#error Missing localtime function
#endif

#if defined( HAVE_WINDOWS_API )
#define date_time_gmtime_r( timestamp, time_elements ) \
	gmtime_s( time_elements, timestamp )

#elif defined( HAVE_GMTIME_R )
#define date_time_gmtime_r( timestamp, time_elements ) \
	gmtime_r( timestamp, time_elements )

#elif !defined( HAVE_GMTIME )
#error Missing gmtime function
#endif

#if defined( HAVE_WINDOWS_API )
#define date_time_ctime( timestamp, string, size ) \
	( ctime_s( string, size, timestamp ) != 0 ? NULL : string )

#elif defined( HAVE_CTIME_R )
#if defined( HAVE_CTIME_R_SIZE )
#define date_time_ctime( timestamp, string, size ) \
	ctime_r( timestamp, string, size )

#else
#define date_time_ctime( timestamp, string, size ) \
	ctime_r( timestamp, string )

#endif

#elif defined( HAVE_CTIME )
char *libewf_date_time_ctime(
       const time_t *timestamp,
       char *string,
       size_t length );

#define date_time_ctime( timestamp, string, size ) \
	libewf_date_time_ctime( timestamp, string, size )

#else
#error Missing ctime function
#endif

#if defined( HAVE_WINDOWS_API )
#define date_time_mktime( time_elements ) \
	mktime( time_elements )

#elif defined( HAVE_MKTIME )
#define date_time_mktime( time_elements ) \
	mktime( time_elements )

#else
#error Missing mktime function
#endif

struct tm *libewf_date_time_localtime(
            const time_t *timestamp );

#define date_time_localtime( timestamp ) \
	libewf_date_time_localtime( timestamp )

struct tm *libewf_date_time_gmtime(
            const time_t *timestamp );

#define date_time_gmtime( timestamp ) \
	libewf_date_time_gmtime( timestamp )

#if defined( __cplusplus )
}
#endif

#endif

