/*
 * Tools system_string functions test program
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
#include <memory.h>
#include <narrow_string.h>
#include <types.h>
#include <system_string.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "ewf_test_libcerror.h"
#include "ewf_test_macros.h"
#include "ewf_test_unused.h"

#include "../ewftools/ewftools_system_string.h"

/* Tests the ewftools_system_string_decimal_copy_to_64_bit function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_system_string_decimal_copy_to_64_bit(
     void )
{
	system_character_t system_string1[ 2 ]  = { '1', 0 };
	system_character_t system_string2[ 3 ]  = { '-', '1', 0 };
	system_character_t system_string3[ 3 ]  = { '+', '1', 0 };
	system_character_t system_string4[ 25 ] = {
		'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5', '6',
		'7', '8', '9', '0', '1', '2', '3', '4', 0 };
	libcerror_error_t *error                = NULL;
	uint64_t value_64bit                    = 0;
	int result                              = 0;

	/* Test regular cases
	 */
	result = ewftools_system_string_decimal_copy_to_64_bit(
	          system_string1,
	          2,
	          &value_64bit,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_UINT64(
	 "value_64bit",
	 value_64bit,
	 (uint64_t) 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = ewftools_system_string_decimal_copy_to_64_bit(
	          system_string2,
	          3,
	          &value_64bit,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_UINT64(
	 "value_64bit",
	 value_64bit,
	 (uint64_t) 0xffffffffffffffffUL );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = ewftools_system_string_decimal_copy_to_64_bit(
	          system_string3,
	          3,
	          &value_64bit,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_UINT64(
	 "value_64bit",
	 value_64bit,
	 (uint64_t) 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = ewftools_system_string_decimal_copy_to_64_bit(
	          NULL,
	          2,
	          &value_64bit,
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

	result = ewftools_system_string_decimal_copy_to_64_bit(
	          system_string1,
	          (size_t) SSIZE_MAX + 1,
	          &value_64bit,
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

	result = ewftools_system_string_decimal_copy_to_64_bit(
	          system_string1,
	          0,
	          &value_64bit,
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

	result = ewftools_system_string_decimal_copy_to_64_bit(
	          system_string1,
	          2,
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

	result = ewftools_system_string_decimal_copy_to_64_bit(
	          system_string4,
	          24,
	          &value_64bit,
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

	system_string4[ 6 ] = 'Z';

	result = ewftools_system_string_decimal_copy_to_64_bit(
	          system_string4,
	          24,
	          &value_64bit,
	          &error );

	system_string4[ 6 ] = '7';

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

/* Tests the ewftools_string_size_to_utf8_string function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_string_size_to_utf8_string(
     void )
{
	system_character_t system_string1[ 2 ] = { '1', 0 };
	libcerror_error_t *error               = NULL;
	size_t utf8_string_size                = 0;
	int result                             = 0;

	/* Test regular cases
	 */
	result = ewftools_string_size_to_utf8_string(
	          system_string1,
	          2,
	          &utf8_string_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_SIZE(
	 "utf8_string_size",
	 utf8_string_size,
	 (size_t) 2 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = ewftools_string_size_to_utf8_string(
	          NULL,
	          2,
	          &utf8_string_size,
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

	result = ewftools_string_size_to_utf8_string(
	          system_string1,
	          (size_t) SSIZE_MAX + 1,
	          &utf8_string_size,
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

	result = ewftools_string_size_to_utf8_string(
	          system_string1,
	          0,
	          &utf8_string_size,
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

	result = ewftools_string_size_to_utf8_string(
	          system_string1,
	          2,
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

/* Tests the ewftools_string_copy_to_utf8_string function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_string_copy_to_utf8_string(
     void )
{
	uint8_t utf8_string[ 32 ];

	system_character_t system_string1[ 2 ] = { '1', 0 };
	libcerror_error_t *error               = NULL;
	int result                             = 0;

	/* Test regular cases
	 */
	result = ewftools_string_copy_to_utf8_string(
	          system_string1,
	          2,
	          utf8_string,
	          32,
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
	result = ewftools_string_copy_to_utf8_string(
	          NULL,
	          2,
	          utf8_string,
	          32,
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

	result = ewftools_string_copy_to_utf8_string(
	          system_string1,
	          (size_t) SSIZE_MAX + 1,
	          utf8_string,
	          32,
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

	result = ewftools_string_copy_to_utf8_string(
	          system_string1,
	          0,
	          utf8_string,
	          32,
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

	result = ewftools_string_copy_to_utf8_string(
	          system_string1,
	          2,
	          NULL,
	          32,
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

	result = ewftools_string_copy_to_utf8_string(
	          system_string1,
	          2,
	          utf8_string,
	          (size_t) SSIZE_MAX + 1,
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

	result = ewftools_string_copy_to_utf8_string(
	          system_string1,
	          2,
	          utf8_string,
	          0,
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
	 "ewftools_system_string_decimal_copy_to_64_bit",
	 ewf_test_tools_system_string_decimal_copy_to_64_bit );

	EWF_TEST_RUN(
	 "ewftools_string_size_to_utf8_string",
	 ewf_test_tools_string_size_to_utf8_string );

	EWF_TEST_RUN(
	 "ewftools_string_copy_to_utf8_string",
	 ewf_test_tools_string_copy_to_utf8_string );

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

