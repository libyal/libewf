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

#include <common.h>
#include <memory.h>
#include <notify.h>
#include <types.h>

#if defined( HAVE_STRING_H )
#include <string.h>
#endif

#include <libewf/definitions.h>

#include "date_time.h"

/* Returns a structured representation of a time using the local time zone, or NULL on error
 */
struct tm *libewf_date_time_localtime(
            const time_t *timestamp )
{
#if !defined( date_time_localtime_r ) && defined( HAVE_LOCALTIME )
	struct tm *static_time_elements = NULL;
#endif
	struct tm *time_elements        = NULL;
	static char *function           = "libewf_date_time_localtime";

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
#if defined( date_time_localtime_r )
#if defined( HAVE_WINDOWS_API )
	if( date_time_localtime_r(
	     timestamp,
	     time_elements ) != 0 )
#else
	if( date_time_localtime_r(
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
#endif
}

/* Returns a structured representation of a time using UTC (GMT), or NULL on error
 */
struct tm *libewf_date_time_gmtime(
            const time_t *timestamp )
{
#if !defined( date_time_gmtime_r ) && defined( HAVE_GMTIME )
	struct tm *static_time_elements = NULL;
#endif
	struct tm *time_elements        = NULL;
	static char *function           = "libewf_date_time_gmtime";

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
#if defined( date_time_gmtime_r )
#if defined( HAVE_WINDOWS_API )
	if( date_time_gmtime_r(
	     timestamp,
	     time_elements ) != 0 )
#else
	if( date_time_gmtime_r(
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
#elif defined( HAVE_GMTIME )
	static_time_elements = gmtime(
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
#endif
}

/* Returns a structured representation of a time using the local time zone, or NULL on error
 */
char *libewf_date_time_ctime(
       const time_t *timestamp )
{
	static char *function    = "libewf_date_time_ctime";
#if !defined( date_time_ctime_r ) && defined( HAVE_CTIME )
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
#if defined( date_time_ctime_r )
#if defined( HAVE_WINDOWS_API )
	if( date_time_ctime_r(
             timestamp,
             time_string,
             time_string_size ) != 0 )
#else
	if( date_time_ctime_r(
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
	if( date_time_string_copy(
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
#endif
}

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )

/* Returns a structured representation of a time using the local time zone, or NULL on error
 */
wchar_t *libewf_date_time_wctime(
          const time_t *timestamp )
{
	static char *function   = "libewf_date_time_wctime";
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
#if defined( date_time_wctime_r )
	if( date_time_wctime_r(
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

