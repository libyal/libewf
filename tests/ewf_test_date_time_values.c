/*
 * Library date_time_values functions test program
 *
 * Copyright (C) 2006-2019, Joachim Metz <joachim.metz@gmail.com>
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
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( TIME_WITH_SYS_TIME )
#include <sys/time.h>
#include <time.h>
#elif defined( HAVE_SYS_TIME_H )
#include <sys/time.h>
#else
#include <time.h>
#endif

#include "ewf_test_libcerror.h"
#include "ewf_test_libewf.h"
#include "ewf_test_macros.h"
#include "ewf_test_unused.h"

#include "../libewf/libewf_date_time_values.h"

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

/* Tests the libewf_date_time_values_copy_from_timestamp function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_date_time_values_copy_from_timestamp(
     void )
{
	uint8_t string[ 32 ];

	libcerror_error_t *error = NULL;
	time_t timestamp         = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_date_time_values_copy_from_timestamp(
	          string,
	          32,
	          timestamp,
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

	/* Test error cases
	 */
	result = libewf_date_time_values_copy_from_timestamp(
	          NULL,
	          32,
	          timestamp,
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

	result = libewf_date_time_values_copy_from_timestamp(
	          string,
	          (size_t) SSIZE_MAX + 1,
	          timestamp,
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

	result = libewf_date_time_values_copy_from_timestamp(
	          string,
	          10,
	          timestamp,
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

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

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

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

	EWF_TEST_RUN(
	 "libewf_date_time_values_copy_from_timestamp",
	 ewf_test_date_time_values_copy_from_timestamp );

	/* TODO: add tests for libewf_date_time_values_copy_to_timestamp */

	/* TODO: add tests for libewf_utf8_string_day_of_week_copy_from_time_elements */

	/* TODO: add tests for libewf_utf8_string_day_of_week_copy_to_time_elements */

	/* TODO: add tests for libewf_utf8_string_day_of_month_copy_to_time_elements */

	/* TODO: add tests for libewf_utf8_string_month_copy_from_time_elements */

	/* TODO: add tests for libewf_utf8_string_month_copy_to_time_elements */

	/* TODO: add tests for libewf_utf8_string_year_copy_to_time_elements */

	/* TODO: add tests for libewf_utf8_string_time_copy_from_time_elements */

	/* TODO: add tests for libewf_utf8_string_hours_copy_to_time_elements */

	/* TODO: add tests for libewf_utf8_string_minutes_copy_to_time_elements */

	/* TODO: add tests for libewf_utf8_string_seconds_copy_to_time_elements */

	/* TODO: add tests for libewf_utf16_string_day_of_week_copy_from_time_elements */

	/* TODO: add tests for libewf_utf16_string_day_of_week_copy_to_time_elements */

	/* TODO: add tests for libewf_utf16_string_day_of_month_copy_to_time_elements */

	/* TODO: add tests for libewf_utf16_string_month_copy_from_time_elements */

	/* TODO: add tests for libewf_utf16_string_month_copy_to_time_elements */

	/* TODO: add tests for libewf_utf16_string_year_copy_to_time_elements */

	/* TODO: add tests for libewf_utf16_string_time_copy_from_time_elements */

	/* TODO: add tests for libewf_utf16_string_hours_copy_to_time_elements */

	/* TODO: add tests for libewf_utf16_string_minutes_copy_to_time_elements */

	/* TODO: add tests for libewf_utf16_string_seconds_copy_to_time_elements */

	/* TODO: add tests for libewf_date_time_values_copy_to_utf8_string */

	/* TODO: add tests for libewf_date_time_values_copy_to_utf16_string */

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

