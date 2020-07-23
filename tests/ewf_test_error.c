/*
 * Library error functions test program
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
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "ewf_test_libewf.h"
#include "ewf_test_macros.h"
#include "ewf_test_unused.h"

/* Tests the libewf_error_free function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_error_free(
     void )
{
	/* Test invocation of function only
	 */
	libewf_error_free(
	 NULL );

	return( 1 );
}

/* Tests the libewf_error_fprint function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_error_fprint(
     void )
{
	/* Test invocation of function only
	 */
	libewf_error_fprint(
	 NULL,
	 NULL );

	return( 1 );
}

/* Tests the libewf_error_sprint function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_error_sprint(
     void )
{
	/* Test invocation of function only
	 */
	libewf_error_sprint(
	 NULL,
	 NULL,
	 0 );

	return( 1 );
}

/* Tests the libewf_error_backtrace_fprint function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_error_backtrace_fprint(
     void )
{
	/* Test invocation of function only
	 */
	libewf_error_backtrace_fprint(
	 NULL,
	 NULL );

	return( 1 );
}

/* Tests the libewf_error_backtrace_sprint function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_error_backtrace_sprint(
     void )
{
	/* Test invocation of function only
	 */
	libewf_error_backtrace_sprint(
	 NULL,
	 NULL,
	 0 );

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
	 "libewf_error_free",
	 ewf_test_error_free );

	EWF_TEST_RUN(
	 "libewf_error_fprint",
	 ewf_test_error_fprint );

	EWF_TEST_RUN(
	 "libewf_error_sprint",
	 ewf_test_error_sprint );

	EWF_TEST_RUN(
	 "libewf_error_backtrace_fprint",
	 ewf_test_error_backtrace_fprint );

	EWF_TEST_RUN(
	 "libewf_error_backtrace_sprint",
	 ewf_test_error_backtrace_sprint );

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

