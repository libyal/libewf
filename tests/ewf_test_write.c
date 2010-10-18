/*
 * Expert Witness Compression Format (EWF) library write testing program
 *
 * Copyright (c) 2006-2010, Joachim Metz <jbmetz@users.sourceforge.net>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <memory.h>

#include <libcstring.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include <stdio.h>

#include <libewf.h>

#include "digest_context.h"
#include "digest_hash.h"
#include "md5.h"

int ewf_test_write(
     const char *filename,
     size_t media_size,
     size_t maximum_segment_size )
{
	uint8_t buffer[ 4096 ];

	digest_hash_t md5_hash[ DIGEST_HASH_SIZE_MD5 ];

	md5_context_t md5_context;

	libewf_error_t *error   = NULL;
	libewf_handle_t *handle = NULL;
	size_t md5_hash_size    = DIGEST_HASH_SIZE_MD5;
	size_t write_size       = 0;
	ssize_t write_count     = 0;
	int result              = 1;
	int sector_iterator     = 0;

	fprintf(
	 stdout,
	 "Testing writing media size: %" PRIzd ", with maximum segment size: %" PRIzd "\t",
	 media_size,
	 maximum_segment_size );

	if( md5_initialize(
	     &md5_context,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize MD5 context.\n" );

		libewf_error_backtrace_fprint(
		 error,
		 stderr );

		libewf_error_free(
		 &error );

		return( -1 );
	}
	if( libewf_handle_initialize(
	     &handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create handle.\n" );

		libewf_error_backtrace_fprint(
		 error,
		 stderr );

		libewf_error_free(
		 &error );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libewf_handle_open_wide(
	     handle,
	     (wchar_t * const *) &filename,
	     1,
	     LIBEWF_OPEN_WRITE,
	     &error ) != 1 )
#else
	if( libewf_handle_open(
	     handle,
	     (char * const *) &filename,
	     1,
	     LIBEWF_OPEN_WRITE,
	     &error ) != 1 )
#endif
	{
		fprintf(
		 stderr,
		 "Unable to open file(s).\n" );

		libewf_error_backtrace_fprint(
		 error,
		 stderr );

		libewf_error_free(
		 &error );
		libewf_handle_free(
		 &handle,
		 NULL );

		return( -1 );
	}
	if( media_size > 0 )
	{
		if( libewf_handle_set_media_size(
		     handle,
		     media_size,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable set media size.\n" );

			libewf_error_backtrace_fprint(
			 error,
			 stderr );

			libewf_error_free(
			 &error );
			libewf_handle_close(
			 handle,
			 NULL );
			libewf_handle_free(
			 &handle,
			 NULL );

			return( -1 );
		}
	}
	if( maximum_segment_size > 0 )
	{
		if( libewf_handle_set_maximum_segment_size(
		     handle,
		     maximum_segment_size,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable set maximum segment size.\n" );

			libewf_error_backtrace_fprint(
			 error,
			 stderr );

			libewf_error_free(
			 &error );
			libewf_handle_close(
			 handle,
			 NULL );
			libewf_handle_free(
			 &handle,
			 NULL );

			return( -1 );
		}
	}
	write_size = 512;

	for( sector_iterator = 0;
	     sector_iterator < 26;
	     sector_iterator++ )
	{
		if( memory_set(
		     buffer,
		     (int) 'A' + sector_iterator,
		     write_size ) == NULL )
		{
			fprintf(
			 stderr,
			 "Unable to initialize buffer with: %d.\n",
			 sector_iterator );

			libewf_handle_close(
			 handle,
			 NULL );
			libewf_handle_free(
			 &handle,
			 NULL );

			return( -1 );
		}
		md5_update(
		 &md5_context,
		 buffer,
		 write_size,
		 &error );

		write_count = libewf_handle_write_buffer(
		               handle,
		               buffer,
		               write_size,
		               &error );

		if( write_count != (ssize_t) write_size )
		{
			if( write_count != (ssize_t) media_size )
			{
				fprintf(
				 stderr,
				 "Unable write block of %" PRIzd " bytes to file(s).\n",
				 write_size );

				libewf_error_backtrace_fprint(
				 error,
				 stderr );

				libewf_error_free(
				 &error );
				libewf_handle_close(
				 handle,
				 NULL );
				libewf_handle_free(
				 &handle,
				 NULL );

				return( -1 );
			}
		}
		if( media_size > 0 )
		{
			media_size -= write_count;
		}
	}
	write_size = 3751;

	for( sector_iterator = 0;
	     sector_iterator < 26;
	     sector_iterator++ )
	{
		if( memory_set(
		     buffer,
		     (int) 'a' + sector_iterator,
		     write_size ) == NULL )
		{
			fprintf(
			 stderr,
			 "Unable to initialize buffer with: %d.\n",
			 sector_iterator );

			libewf_handle_close(
			 handle,
			 NULL );
			libewf_handle_free(
			 &handle,
			 NULL );

			return( -1 );
		}
		md5_update(
		 &md5_context,
		 buffer,
		 write_size,
		 &error );

		write_count = libewf_handle_write_buffer(
		               handle,
		               buffer,
		               write_size,
		               &error );

		if( write_count != (ssize_t) write_size )
		{
			if( write_count != (ssize_t) media_size )
			{
				fprintf(
				 stderr,
				 "Unable write block of %" PRIzd " bytes to file(s).\n",
				 write_size );

				libewf_error_backtrace_fprint(
				 error,
				 stderr );

				libewf_error_free(
				 &error );

				libewf_handle_close(
				 handle,
				 NULL );
				libewf_handle_free(
				 &handle,
				 NULL );

				return( -1 );
			}
		}
		if( media_size > 0 )
		{
			media_size -= write_count;
		}
	}
	if( md5_finalize(
	     &md5_context,
	     md5_hash,
	     &md5_hash_size,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to finalize MD5 context.\n" );

		libewf_error_backtrace_fprint(
		 error,
		 stderr );

		libewf_error_free(
		 &error );

		libewf_handle_close(
		 handle,
		 NULL );
		libewf_handle_free(
		 &handle,
		 NULL );

		return( -1 );
	}
	/* Clean up
	 */
	if( libewf_handle_close(
	     handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close file(s).\n" );

		libewf_error_backtrace_fprint(
		 error,
		 stderr );

		libewf_error_free(
		 &error );

		libewf_handle_free(
		 &handle,
		 NULL );

		return( -1 );
	}
	if( libewf_handle_free(
	     &handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free handle.\n" );

		libewf_error_backtrace_fprint(
		 error,
		 stderr );

		libewf_error_free(
		 &error );

		libewf_handle_free(
		 &handle,
		 NULL );

		return( -1 );
	}
	if( result != 0 )
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	return( result );
}

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	if( ewf_test_write(
	     _LIBCSTRING_SYSTEM_STRING( "test1" ),
	     0,
	     0 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test write.\n" );

		return( EXIT_FAILURE );
	}
	if( ewf_test_write(
	     _LIBCSTRING_SYSTEM_STRING( "test2" ),
	     0,
	     10000 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test write.\n" );

		return( EXIT_FAILURE );
	}
	if( ewf_test_write(
	     _LIBCSTRING_SYSTEM_STRING( "test3" ),
	     100000,
	     0 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test write.\n" );

		return( EXIT_FAILURE );
	}
	if( ewf_test_write(
	     _LIBCSTRING_SYSTEM_STRING( "test4" ),
	     100000,
	     10000 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test write.\n" );

		return( EXIT_FAILURE );
	}
	return( EXIT_SUCCESS );
}

