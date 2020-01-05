/*
 * Library date_time_values functions test program
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
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

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

/* Tests the libewf_date_time_values_copy_to_timestamp function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_date_time_values_copy_to_timestamp(
     void )
{
	libcerror_error_t *error = NULL;
	uint8_t *string          = (uint8_t *) "2006 03 12 12 17 48";
	time_t timestamp         = 0;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO fix test
	result = libewf_date_time_values_copy_to_timestamp(
	          string,
	          19,
	          &timestamp,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );
*/

	/* Test error cases
	 */
	result = libewf_date_time_values_copy_to_timestamp(
	          NULL,
	          19,
	          &timestamp,
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

	result = libewf_date_time_values_copy_to_timestamp(
	          string,
	          (size_t) SSIZE_MAX + 1,
	          &timestamp,
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

	result = libewf_date_time_values_copy_to_timestamp(
	          string,
	          19,
	          NULL,
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

/* Tests the libewf_utf8_string_day_of_week_copy_from_time_elements function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_utf8_string_day_of_week_copy_from_time_elements(
     void )
{
	uint8_t utf8_string[ 8 ];

	struct tm time_elements;

	libcerror_error_t *error = NULL;
	size_t utf8_string_index = 0;
	int result               = 0;

	/* Test regular cases
	 */
	time_elements.tm_wday = 0;
	utf8_string_index     = 0;

	result = libewf_utf8_string_day_of_week_copy_from_time_elements(
	          utf8_string,
	          8,
	          &utf8_string_index,
	          &time_elements,
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
	utf8_string_index = 0;

	result = libewf_utf8_string_day_of_week_copy_from_time_elements(
	          NULL,
	          8,
	          &utf8_string_index,
	          &time_elements,
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

	result = libewf_utf8_string_day_of_week_copy_from_time_elements(
	          utf8_string,
	          (size_t) SSIZE_MAX + 1,
	          &utf8_string_index,
	          &time_elements,
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

	result = libewf_utf8_string_day_of_week_copy_from_time_elements(
	          utf8_string,
	          8,
	          NULL,
	          &time_elements,
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

	result = libewf_utf8_string_day_of_week_copy_from_time_elements(
	          utf8_string,
	          8,
	          &utf8_string_index,
	          NULL,
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

/* Tests the libewf_utf8_string_day_of_week_copy_to_time_elements function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_utf8_string_day_of_week_copy_to_time_elements(
     void )
{
	struct tm time_elements;

	uint8_t day_of_week_strings[ 8 ][ 4 ] = {
		{ 'S', 'u', 'n', 0 },
		{ 'M', 'o', 'n', 0 },
		{ 'T', 'u', 'e', 0 },
		{ 'W', 'e', 'd', 0 },
		{ 'T', 'h', 'u', 0 },
		{ 'F', 'r', 'i', 0 },
		{ 'S', 'a', 't', 0 },
		{ 'X', 'X', 'X', 0 } };

	libcerror_error_t *error = NULL;
	int result               = 0;
	int string_index         = 0;

	/* Test regular cases
	 */
	for( string_index = 0;
	     string_index < 7;
	     string_index++ )
	{
		result = libewf_utf8_string_day_of_week_copy_to_time_elements(
		          day_of_week_strings[ string_index ],
		          4,
		          &time_elements,
		          &error );

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		EWF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test error cases
	 */
	result = libewf_utf8_string_day_of_week_copy_to_time_elements(
	          NULL,
	          4,
	          &time_elements,
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

	result = libewf_utf8_string_day_of_week_copy_to_time_elements(
	          day_of_week_strings[ 0 ],
	          (size_t) SSIZE_MAX + 1,
	          &time_elements,
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

	result = libewf_utf8_string_day_of_week_copy_to_time_elements(
	          day_of_week_strings[ 0 ],
	          4,
	          NULL,
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

	result = libewf_utf8_string_day_of_week_copy_to_time_elements(
	          day_of_week_strings[ 7 ],
	          4,
	          &time_elements,
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

/* Tests the libewf_utf8_string_day_of_month_copy_to_time_elements function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_utf8_string_day_of_month_copy_to_time_elements(
     void )
{
	struct tm time_elements;

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_utf8_string_day_of_month_copy_to_time_elements(
	          (uint8_t *) "01",
	          3,
	          &time_elements,
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
	result = libewf_utf8_string_day_of_month_copy_to_time_elements(
	          NULL,
	          3,
	          &time_elements,
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

	result = libewf_utf8_string_day_of_month_copy_to_time_elements(
	          (uint8_t *) "01",
	          (size_t) SSIZE_MAX + 1,
	          &time_elements,
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

	result = libewf_utf8_string_day_of_month_copy_to_time_elements(
	          (uint8_t *) "01",
	          3,
	          NULL,
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

	result = libewf_utf8_string_day_of_month_copy_to_time_elements(
	          (uint8_t *) "XX",
	          3,
	          &time_elements,
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

	result = libewf_utf8_string_day_of_month_copy_to_time_elements(
	          (uint8_t *) "99",
	          3,
	          &time_elements,
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

/* Tests the libewf_utf8_string_month_copy_from_time_elements function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_utf8_string_month_copy_from_time_elements(
     void )
{
	uint8_t utf8_string[ 8 ];

	struct tm time_elements;

	libcerror_error_t *error = NULL;
	size_t utf8_string_index = 0;
	int result               = 0;

	/* Test regular cases
	 */
	time_elements.tm_mon = 0;
	utf8_string_index    = 0;

	result = libewf_utf8_string_month_copy_from_time_elements(
	          utf8_string,
	          8,
	          &utf8_string_index,
	          &time_elements,
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
	utf8_string_index = 0;

	result = libewf_utf8_string_month_copy_from_time_elements(
	          NULL,
	          8,
	          &utf8_string_index,
	          &time_elements,
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

	result = libewf_utf8_string_month_copy_from_time_elements(
	          utf8_string,
	          (size_t) SSIZE_MAX + 1,
	          &utf8_string_index,
	          &time_elements,
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

	result = libewf_utf8_string_month_copy_from_time_elements(
	          utf8_string,
	          8,
	          NULL,
	          &time_elements,
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

	result = libewf_utf8_string_month_copy_from_time_elements(
	          utf8_string,
	          8,
	          &utf8_string_index,
	          NULL,
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

/* Tests the libewf_utf8_string_month_copy_to_time_elements function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_utf8_string_month_copy_to_time_elements(
     void )
{
	struct tm time_elements;

	uint8_t month_strings[ 13 ][ 4 ] = {
		{ 'J', 'a', 'n', 0 },
		{ 'F', 'e', 'b', 0 },
		{ 'M', 'a', 'r', 0 },
		{ 'A', 'p', 'r', 0 },
		{ 'M', 'a', 'y', 0 },
		{ 'J', 'u', 'n', 0 },
		{ 'J', 'u', 'l', 0 },
		{ 'A', 'u', 'g', 0 },
		{ 'S', 'e', 'p', 0 },
		{ 'O', 'c', 't', 0 },
		{ 'N', 'o', 'v', 0 },
		{ 'D', 'e', 'c', 0 },
		{ 'X', 'X', 'X', 0 } };

	libcerror_error_t *error = NULL;
	int result               = 0;
	int string_index         = 0;

	/* Test regular cases
	 */
	for( string_index = 0;
	     string_index < 12;
	     string_index++ )
	{
		result = libewf_utf8_string_month_copy_to_time_elements(
		          month_strings[ string_index ],
		          4,
		          &time_elements,
		          &error );

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		EWF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test error cases
	 */
	result = libewf_utf8_string_month_copy_to_time_elements(
	          NULL,
	          3,
	          &time_elements,
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

	result = libewf_utf8_string_month_copy_to_time_elements(
	          month_strings[ 0 ],
	          (size_t) SSIZE_MAX + 1,
	          &time_elements,
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

	result = libewf_utf8_string_month_copy_to_time_elements(
	          month_strings[ 0 ],
	          3,
	          NULL,
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

	result = libewf_utf8_string_month_copy_to_time_elements(
	          month_strings[ 12 ],
	          3,
	          &time_elements,
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

/* Tests the libewf_utf8_string_year_copy_to_time_elements function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_utf8_string_year_copy_to_time_elements(
     void )
{
	struct tm time_elements;

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_utf8_string_year_copy_to_time_elements(
	          (uint8_t *) "2000",
	          5,
	          &time_elements,
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
	result = libewf_utf8_string_year_copy_to_time_elements(
	          NULL,
	          5,
	          &time_elements,
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

	result = libewf_utf8_string_year_copy_to_time_elements(
	          (uint8_t *) "2000",
	          (size_t) SSIZE_MAX + 1,
	          &time_elements,
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

	result = libewf_utf8_string_year_copy_to_time_elements(
	          (uint8_t *) "2000",
	          5,
	          NULL,
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

	result = libewf_utf8_string_year_copy_to_time_elements(
	          (uint8_t *) "XXXX",
	          5,
	          &time_elements,
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

	result = libewf_utf8_string_year_copy_to_time_elements(
	          (uint8_t *) "12000",
	          6,
	          &time_elements,
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

/* Tests the libewf_utf8_string_time_copy_from_time_elements function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_utf8_string_time_copy_from_time_elements(
     void )
{
	uint8_t utf8_string[ 10 ];

	struct tm time_elements;

	libcerror_error_t *error = NULL;
	size_t utf8_string_index = 0;
	int result               = 0;

	/* Test regular cases
	 */
	time_elements.tm_hour = 1;
	time_elements.tm_min  = 2;
	time_elements.tm_sec  = 3;
	utf8_string_index     = 0;

	result = libewf_utf8_string_time_copy_from_time_elements(
	          utf8_string,
	          10,
	          &utf8_string_index,
	          &time_elements,
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
	utf8_string_index = 0;

	result = libewf_utf8_string_time_copy_from_time_elements(
	          NULL,
	          10,
	          &utf8_string_index,
	          &time_elements,
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

	result = libewf_utf8_string_time_copy_from_time_elements(
	          utf8_string,
	          (size_t) SSIZE_MAX + 1,
	          &utf8_string_index,
	          &time_elements,
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

	result = libewf_utf8_string_time_copy_from_time_elements(
	          utf8_string,
	          10,
	          &utf8_string_index,
	          NULL,
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

	result = libewf_utf8_string_time_copy_from_time_elements(
	          utf8_string,
	          10,
	          NULL,
	          &time_elements,
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

/* Tests the libewf_utf8_string_hours_copy_to_time_elements function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_utf8_string_hours_copy_to_time_elements(
     void )
{
	struct tm time_elements;

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_utf8_string_hours_copy_to_time_elements(
	          (uint8_t *) "01",
	          3,
	          &time_elements,
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
	result = libewf_utf8_string_hours_copy_to_time_elements(
	          NULL,
	          3,
	          &time_elements,
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

	result = libewf_utf8_string_hours_copy_to_time_elements(
	          (uint8_t *) "01",
	          (size_t) SSIZE_MAX + 1,
	          &time_elements,
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

	result = libewf_utf8_string_hours_copy_to_time_elements(
	          (uint8_t *) "01",
	          3,
	          NULL,
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

	result = libewf_utf8_string_hours_copy_to_time_elements(
	          (uint8_t *) "XX",
	          3,
	          &time_elements,
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

	result = libewf_utf8_string_hours_copy_to_time_elements(
	          (uint8_t *) "99",
	          3,
	          &time_elements,
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

/* Tests the libewf_utf8_string_minutes_copy_to_time_elements function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_utf8_string_minutes_copy_to_time_elements(
     void )
{
	struct tm time_elements;

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_utf8_string_minutes_copy_to_time_elements(
	          (uint8_t *) "01",
	          3,
	          &time_elements,
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
	result = libewf_utf8_string_minutes_copy_to_time_elements(
	          NULL,
	          3,
	          &time_elements,
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

	result = libewf_utf8_string_minutes_copy_to_time_elements(
	          (uint8_t *) "01",
	          (size_t) SSIZE_MAX + 1,
	          &time_elements,
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

	result = libewf_utf8_string_minutes_copy_to_time_elements(
	          (uint8_t *) "01",
	          3,
	          NULL,
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

	result = libewf_utf8_string_minutes_copy_to_time_elements(
	          (uint8_t *) "XX",
	          3,
	          &time_elements,
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

	result = libewf_utf8_string_minutes_copy_to_time_elements(
	          (uint8_t *) "99",
	          3,
	          &time_elements,
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

/* Tests the libewf_utf8_string_seconds_copy_to_time_elements function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_utf8_string_seconds_copy_to_time_elements(
     void )
{
	struct tm time_elements;

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_utf8_string_seconds_copy_to_time_elements(
	          (uint8_t *) "01",
	          3,
	          &time_elements,
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
	result = libewf_utf8_string_seconds_copy_to_time_elements(
	          NULL,
	          3,
	          &time_elements,
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

	result = libewf_utf8_string_seconds_copy_to_time_elements(
	          (uint8_t *) "01",
	          (size_t) SSIZE_MAX + 1,
	          &time_elements,
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

	result = libewf_utf8_string_seconds_copy_to_time_elements(
	          (uint8_t *) "01",
	          3,
	          NULL,
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

	result = libewf_utf8_string_seconds_copy_to_time_elements(
	          (uint8_t *) "XX",
	          3,
	          &time_elements,
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

	result = libewf_utf8_string_seconds_copy_to_time_elements(
	          (uint8_t *) "99",
	          3,
	          &time_elements,
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

/* Tests the libewf_utf16_string_day_of_week_copy_from_time_elements function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_utf16_string_day_of_week_copy_from_time_elements(
     void )
{
	uint16_t utf16_string[ 8 ];

	struct tm time_elements;

	libcerror_error_t *error  = NULL;
	size_t utf16_string_index = 0;
	int result                = 0;

	/* Test regular cases
	 */
	time_elements.tm_wday = 0;
	utf16_string_index    = 0;

	result = libewf_utf16_string_day_of_week_copy_from_time_elements(
	          utf16_string,
	          8,
	          &utf16_string_index,
	          &time_elements,
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
	utf16_string_index = 0;

	result = libewf_utf16_string_day_of_week_copy_from_time_elements(
	          NULL,
	          8,
	          &utf16_string_index,
	          &time_elements,
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

	result = libewf_utf16_string_day_of_week_copy_from_time_elements(
	          utf16_string,
	          (size_t) SSIZE_MAX + 1,
	          &utf16_string_index,
	          &time_elements,
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

	result = libewf_utf16_string_day_of_week_copy_from_time_elements(
	          utf16_string,
	          8,
	          NULL,
	          &time_elements,
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

	result = libewf_utf16_string_day_of_week_copy_from_time_elements(
	          utf16_string,
	          8,
	          &utf16_string_index,
	          NULL,
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

/* Tests the libewf_utf16_string_month_copy_from_time_elements function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_utf16_string_month_copy_from_time_elements(
     void )
{
	uint16_t utf16_string[ 8 ];

	struct tm time_elements;

	libcerror_error_t *error  = NULL;
	size_t utf16_string_index = 0;
	int result                = 0;

	/* Test regular cases
	 */
	time_elements.tm_mon = 0;
	utf16_string_index   = 0;

	result = libewf_utf16_string_month_copy_from_time_elements(
	          utf16_string,
	          8,
	          &utf16_string_index,
	          &time_elements,
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
	utf16_string_index = 0;

	result = libewf_utf16_string_month_copy_from_time_elements(
	          NULL,
	          8,
	          &utf16_string_index,
	          &time_elements,
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

	result = libewf_utf16_string_month_copy_from_time_elements(
	          utf16_string,
	          (size_t) SSIZE_MAX + 1,
	          &utf16_string_index,
	          &time_elements,
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

	result = libewf_utf16_string_month_copy_from_time_elements(
	          utf16_string,
	          8,
	          NULL,
	          &time_elements,
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

	result = libewf_utf16_string_month_copy_from_time_elements(
	          utf16_string,
	          8,
	          &utf16_string_index,
	          NULL,
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

/* Tests the libewf_utf16_string_time_copy_from_time_elements function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_utf16_string_time_copy_from_time_elements(
     void )
{
	uint16_t utf16_string[ 10 ];

	struct tm time_elements;

	libcerror_error_t *error  = NULL;
	size_t utf16_string_index = 0;
	int result                = 0;

	/* Test regular cases
	 */
	time_elements.tm_hour = 1;
	time_elements.tm_min  = 2;
	time_elements.tm_sec  = 3;
	utf16_string_index    = 0;

	result = libewf_utf16_string_time_copy_from_time_elements(
	          utf16_string,
	          10,
	          &utf16_string_index,
	          &time_elements,
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
	utf16_string_index = 0;

	result = libewf_utf16_string_time_copy_from_time_elements(
	          NULL,
	          10,
	          &utf16_string_index,
	          &time_elements,
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

	result = libewf_utf16_string_time_copy_from_time_elements(
	          utf16_string,
	          (size_t) SSIZE_MAX + 1,
	          &utf16_string_index,
	          &time_elements,
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

	result = libewf_utf16_string_time_copy_from_time_elements(
	          utf16_string,
	          10,
	          &utf16_string_index,
	          NULL,
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

	result = libewf_utf16_string_time_copy_from_time_elements(
	          utf16_string,
	          10,
	          NULL,
	          &time_elements,
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

	EWF_TEST_RUN(
	 "libewf_date_time_values_copy_to_timestamp",
	 ewf_test_date_time_values_copy_to_timestamp );

	EWF_TEST_RUN(
	 "libewf_utf8_string_day_of_week_copy_from_time_elements",
	 ewf_test_utf8_string_day_of_week_copy_from_time_elements );

	EWF_TEST_RUN(
	 "libewf_utf8_string_day_of_week_copy_to_time_elements",
	 ewf_test_utf8_string_day_of_week_copy_to_time_elements );

	EWF_TEST_RUN(
	 "libewf_utf8_string_day_of_month_copy_to_time_elements",
	 ewf_test_utf8_string_day_of_month_copy_to_time_elements );

	EWF_TEST_RUN(
	 "libewf_utf8_string_month_copy_from_time_elements",
	 ewf_test_utf8_string_month_copy_from_time_elements );

	EWF_TEST_RUN(
	 "libewf_utf8_string_month_copy_to_time_elements",
	 ewf_test_utf8_string_month_copy_to_time_elements );

	EWF_TEST_RUN(
	 "libewf_utf8_string_year_copy_to_time_elements",
	 ewf_test_utf8_string_year_copy_to_time_elements );

	EWF_TEST_RUN(
	 "libewf_utf8_string_time_copy_from_time_elements",
	 ewf_test_utf8_string_time_copy_from_time_elements );

	EWF_TEST_RUN(
	 "libewf_utf8_string_hours_copy_to_time_elements",
	 ewf_test_utf8_string_hours_copy_to_time_elements );

	EWF_TEST_RUN(
	 "libewf_utf8_string_minutes_copy_to_time_elements",
	 ewf_test_utf8_string_minutes_copy_to_time_elements );

	EWF_TEST_RUN(
	 "libewf_utf8_string_seconds_copy_to_time_elements",
	 ewf_test_utf8_string_seconds_copy_to_time_elements );

	EWF_TEST_RUN(
	 "libewf_utf16_string_day_of_week_copy_from_time_elements",
	 ewf_test_utf16_string_day_of_week_copy_from_time_elements );

	EWF_TEST_RUN(
	 "libewf_utf16_string_month_copy_from_time_elements",
	 ewf_test_utf16_string_month_copy_from_time_elements );

	EWF_TEST_RUN(
	 "libewf_utf16_string_time_copy_from_time_elements",
	 ewf_test_utf16_string_time_copy_from_time_elements );

	/* TODO: add tests for libewf_date_time_values_copy_to_utf8_string */

	/* TODO: add tests for libewf_date_time_values_copy_to_utf16_string */

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

