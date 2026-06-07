/*
 * library glob testing program
 *
 * Copyright (C) 2006-2026, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <narrow_string.h>
#include <types.h>
#include <system_string.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include <stdio.h>

#include "ewf_test_macros.h"
#include "ewf_test_libewf.h"

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	wchar_t **filenames       = NULL;
	wchar_t *glob_string      = NULL;
#else
	char **filenames          = NULL;
	char *glob_string         = NULL;
#endif
	libewf_error_t *error      = NULL;
	size_t glob_string_length = 0;
	int filename_index        = 0;
	int number_of_filenames   = 0;
	int result                = 0;

	if( argc < 2 )
	{
		fprintf(
		 stderr,
		 "Missing filename(s).\n" );

		return( EXIT_FAILURE );
	}
	glob_string = argv[ 1 ];

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "glob_string",
	 glob_string );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	glob_string_length = wide_string_length(
	                      glob_string );

	result = libewf_glob_wide(
	          glob_string,
	          glob_string_length,
	          LIBEWF_FORMAT_UNKNOWN,
	          &filenames,
	          &number_of_filenames,
	          &error );
#else
	glob_string_length = narrow_string_length(
	                      argv[ 1 ] );

	result = libewf_glob(
	          glob_string,
	          glob_string_length,
	          LIBEWF_FORMAT_UNKNOWN,
	          &filenames,
	          &number_of_filenames,
	          &error );
#endif
	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "filenames",
	 filenames );

	EWF_TEST_ASSERT_GREATER_THAN_INT(
	 "number_of_filenames",
	 number_of_filenames,
	 0 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	for( filename_index = 0;
	     filename_index < number_of_filenames;
	     filename_index++ )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		fprintf(
		 stdout,
		 "%ls\n",
		 filenames[ filename_index ] );
#else
		fprintf(
		 stdout,
		 "%s\n",
		 filenames[ filename_index ] );
#endif
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_glob_wide_free(
	          filenames,
	          number_of_filenames,
	          &error );
#else
	result = libewf_glob_free(
	          filenames,
	          number_of_filenames,
	          &error );
#endif
	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	filenames = NULL;

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( EXIT_SUCCESS );

on_error:
	if( filenames != NULL )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		libewf_glob_wide_free(
		 filenames,
		 number_of_filenames,
		 NULL );
#else
		libewf_glob_free(
		 filenames,
		 number_of_filenames,
		 NULL );
#endif
	}
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

