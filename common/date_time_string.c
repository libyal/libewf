/*
 * Date and time string functions
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

#include "common.h"
#include "character_string.h"
#include "date_time.h"
#include "date_time_string.h"
#include "memory.h"
#include "notify.h"

/* Generate ctime string
 * Sets ctime string and ctime string length
 * Returns 1 if successful or -1 on error
 */
int libewf_date_time_string_ctime(
     const time_t *timestamp,
     character_t **ctime_string,
     size_t *ctime_string_length )
{
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && !defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
        char *narrow_ctime_string = NULL;
#endif
        static char *function     = "libewf_date_time_string_ctime";

	if( timestamp == NULL )
	{
		notify_warning_printf( "%s: invalid timestamp.\n",
		 function );

		return( -1 );
	}
	if( ctime_string == NULL )
	{
		notify_warning_printf( "%s: invalid ctime string.\n",
		 function );

		return( -1 );
	}
	if( *ctime_string != NULL )
	{
		notify_warning_printf( "%s: ctime string already created.\n",
		 function );

		return( -1 );
	}
	if( ctime_string_length == NULL )
	{
		notify_warning_printf( "%s: invalid ctime string length.\n",
		 function );

		return( -1 );
	}
        /* The date_time_ctime and date_time_wctime functions return a string of length 32
	 */
	*ctime_string_length = 32;

#if !defined( HAVE_WIDE_CHARACTER_TYPE )
        *ctime_string = date_time_ctime(
	                 timestamp );

#elif defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
        *ctime_string = date_time_wctime(
	                 timestamp );

#else
        narrow_ctime_string = date_time_ctime(
	                       timestamp );

        if( narrow_ctime_string == NULL )
        {
                notify_warning_printf( "%s: unable to create narrow ctime string.\n",
                 function );

                return( -1 );
        }
        *ctime_string = (character_t *) memory_allocate(
                                         sizeof( character_t ) * *ctime_string_length );

        if( *ctime_string == NULL )
        {
                notify_warning_printf( "%s: unable to create ctime string.\n",
                 function );

		memory_free(
		 narrow_ctime_string );

		*ctime_string_length = 0;

                return( -1 );
        }
        if( string_copy_char_to_wchar(
	     *ctime_string,
	     narrow_ctime_string,
	     *ctime_string_length ) != 1 )
        {
                notify_warning_printf( "%s: unable to copy narrow ctime string to ctime string.\n",
                 function );

		memory_free(
		 narrow_ctime_string );
		memory_free(
		 *ctime_string );

		*ctime_string        = NULL;
		*ctime_string_length = 0;

                return( -1 );
        }
	memory_free(
	 narrow_ctime_string );
#endif
	return( 1 );
}

