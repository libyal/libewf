/*
 * Tools output functions test program
 *
 * Copyright (C) 2006-2020, Joachim Metz <joachim.metz@gmail.com>
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
#include <file_stream.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include <stdio.h>

#if defined( HAVE_IO_H ) || defined( WINAPI )
#include <io.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "ewf_test_libcerror.h"
#include "ewf_test_macros.h"
#include "ewf_test_unused.h"

#include "../ewftools/ewftools_output.h"

/* Tests the ewftools_output_initialize function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_output_initialize(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = ewftools_output_initialize(
	          _IONBF,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = ewftools_output_initialize(
	          -1,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the ewftools_output_copyright_fprint function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_output_copyright_fprint(
     void )
{
#if defined( HAVE_FMEMOPEN ) && !defined( WINAPI )
	char string[ 1024 ];

	FILE *stream = NULL;
#endif

	/* Test invocation of function only
	 */
#if defined( HAVE_FMEMOPEN ) && !defined( WINAPI )

	stream = fmemopen(
	          string,
	          1024,
	          "w+");

	ewftools_output_copyright_fprint(
	 stream );

	fclose(
	 stream );

#endif /* defined( HAVE_FMEMOPEN ) && !defined( WINAPI ) */

	ewftools_output_copyright_fprint(
	 NULL );

	return( 1 );
}

/* Tests the ewftools_output_version_fprint function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_output_version_fprint(
     void )
{
#if defined( HAVE_FMEMOPEN ) && !defined( WINAPI )
	char string[ 1024 ];

	FILE *stream = NULL;
#endif

	/* Test invocation of function only
	 */
#if defined( HAVE_FMEMOPEN ) && !defined( WINAPI )

	stream = fmemopen(
	          string,
	          1024,
	          "w+");

	ewftools_output_version_fprint(
	 stream,
	 _SYSTEM_STRING( "test" ) );

	fclose(
	 stream );

#endif /* defined( HAVE_FMEMOPEN ) && !defined( WINAPI ) */

	/* Test error cases
	 */
	ewftools_output_version_fprint(
	 NULL,
	 _SYSTEM_STRING( "test" ) );

	ewftools_output_version_fprint(
	 (FILE *) 0x12345678UL,
	 NULL );

	return( 1 );
}

/* Tests the ewftools_output_version_detailed_fprint function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_output_version_detailed_fprint(
     void )
{
#if defined( HAVE_FMEMOPEN ) && !defined( WINAPI )
	char string[ 1024 ];

	FILE *stream = NULL;
#endif

	/* Test invocation of function only
	 */
#if defined( HAVE_FMEMOPEN ) && !defined( WINAPI )

	stream = fmemopen(
	          string,
	          1024,
	          "w+");

	ewftools_output_version_detailed_fprint(
	 stream,
	 _SYSTEM_STRING( "test" ) );

	fclose(
	 stream );

#endif /* defined( HAVE_FMEMOPEN ) && !defined( WINAPI ) */

	/* Test error cases
	 */
	ewftools_output_version_detailed_fprint(
	 NULL,
	 _SYSTEM_STRING( "test" ) );

	ewftools_output_version_detailed_fprint(
	 (FILE *) 0x12345678UL,
	 NULL );

	return( 1 );
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc EWF_TEST_ATTRIBUTE_UNUSED,
     wchar_t * const argv[] EWF_TEST_ATTRIBUTE_UNUSED )
#else
int main(
     int argc EWF_TEST_ATTRIBUTE_UNUSED,
     char * const argv[] EWF_TEST_ATTRIBUTE_UNUSED )
#endif
{
	EWF_TEST_UNREFERENCED_PARAMETER( argc )
	EWF_TEST_UNREFERENCED_PARAMETER( argv )

	EWF_TEST_RUN(
	 "ewftools_output_initialize",
	 ewf_test_tools_output_initialize )

	EWF_TEST_RUN(
	 "ewftools_output_copyright_fprint",
	 ewf_test_tools_output_copyright_fprint )

	EWF_TEST_RUN(
	 "ewftools_output_version_fprint",
	 ewf_test_tools_output_version_fprint )

	EWF_TEST_RUN(
	 "ewftools_output_version_detailed_fprint",
	 ewf_test_tools_output_version_detailed_fprint )

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

