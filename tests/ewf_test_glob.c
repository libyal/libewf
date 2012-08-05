/*
 * Expert Witness Compression Format (EWF) library glob testing program
 *
 * Copyright (c) 2006-2012, Joachim Metz <joachim.metz@gmail.com>
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

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include <stdio.h>

#include "ewf_test_libcstring.h"
#include "ewf_test_libewf.h"

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	wchar_t **filenames     = NULL;
#else
	char **filenames        = NULL;
#endif
	libewf_error_t *error   = NULL;
	int number_of_filenames = 0;
	int filename_iterator   = 0;

	if( argc < 2 )
	{
		fprintf(
		 stderr,
		 "Missing filename(s).\n" );

		return( EXIT_FAILURE );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libewf_glob_wide(
	     argv[ 1 ],
	     libcstring_wide_string_length(
	      argv[ 1 ] ),
	     LIBEWF_FORMAT_UNKNOWN,
	     &filenames,
	     &number_of_filenames,
	     &error ) != 1 )
#else
	if( libewf_glob(
	     argv[ 1 ],
	     libcstring_narrow_string_length(
	      argv[ 1 ] ),
	     LIBEWF_FORMAT_UNKNOWN,
	     &filenames,
	     &number_of_filenames,
	     &error ) != 1 )
#endif
	{
		fprintf(
		 stderr,
		 "Unable to glob filenames.\n" );

		goto on_error;
	}
	if( number_of_filenames < 0 )
	{
		fprintf(
		 stderr,
		 "Invalid number of filenames.\n" );

		return( EXIT_FAILURE );
	}
	else if( number_of_filenames == 0 )
	{
		fprintf(
		 stderr,
		 "Glob missing filenames.\n" );

		return( EXIT_FAILURE );
	}
	for( filename_iterator = 0;
	     filename_iterator < number_of_filenames;
	     filename_iterator++ )
	{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		fprintf(
		 stdout,
		 "%ls",
		 filenames[ filename_iterator ] );
#else
		fprintf(
		 stdout,
		 "%s",
		 filenames[ filename_iterator ] );
#endif
		if( filename_iterator == ( number_of_filenames - 1 ) )
		{
			fprintf(
			 stdout,
			 "\n" );
		}
		else
		{
			fprintf(
			 stdout,
			 " " );
		}
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libewf_glob_wide_free(
	     filenames,
	     number_of_filenames,
	     &error ) != 1 )
#else
	if( libewf_glob_free(
	     filenames,
	     number_of_filenames,
	     &error ) != 1 )
#endif
	{
		fprintf(
		 stderr,
		 "Unable to free glob.\n" );

		goto on_error;
	}
	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libewf_error_backtrace_fprint(
		 error,
		 stderr );
		libewf_error_free(
		 &error );
	}
	return( EXIT_FAILURE );
}

