/*
 * Tools byte_size_string functions test program
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

#include "../ewftools/byte_size_string.h"

/* Tests the byte_size_string_create function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_byte_size_string_create(
     void )
{
	system_character_t byte_size_string[ 32 ];

	system_character_t expected_byte_size_string[ 8 ] = {
		'8', '.', '0', ' ', 'G', 'i', 'B', 0 };

	libcerror_error_t *error                          = NULL;
	int result                                        = 0;

	/* Test regular cases
	 */
	result = byte_size_string_create(
	          byte_size_string,
	          32,
	          (size64_t) 8192 * 1024 * 1024,
	          BYTE_SIZE_STRING_UNIT_MEBIBYTE,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          byte_size_string,
	          expected_byte_size_string,
	          sizeof( system_character_t ) * 8 );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	result = byte_size_string_create(
	          NULL,
	          32,
	          (size64_t) 8192 * 1024 * 1024,
	          BYTE_SIZE_STRING_UNIT_MEBIBYTE,
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

/* Tests the byte_size_string_create_with_decimal_point function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_byte_size_string_create_with_decimal_point(
     void )
{
	system_character_t byte_size_string[ 32 ];

	system_character_t expected_byte_size_string1[ 8 ] = {
		'8', '.', '0', ' ', 'G', 'i', 'B', 0 };

	system_character_t expected_byte_size_string2[ 6 ] = {
		'9', '9', '9', ' ', 'B', 0 };

	libcerror_error_t *error                           = NULL;
	size64_t size                                      = 0;
	int factor                                         = 0;
	int result                                         = 0;

	/* Test regular cases
	 */
	result = byte_size_string_create_with_decimal_point(
	          byte_size_string,
	          32,
	          (size64_t) 8192 * 1024 * 1024,
	          BYTE_SIZE_STRING_UNIT_MEBIBYTE,
	          (int) '.',
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          byte_size_string,
	          expected_byte_size_string1,
	          sizeof( system_character_t ) * 8 );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	size = 999;

	result = byte_size_string_create_with_decimal_point(
	          byte_size_string,
	          32,
	          size,
	          BYTE_SIZE_STRING_UNIT_MEBIBYTE,
	          (int) '.',
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          byte_size_string,
	          expected_byte_size_string2,
	          sizeof( system_character_t ) * 6 );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	for( factor = 1;
	     factor <= 8;
	     factor++ )
	{
		size *= 1000;

		result = byte_size_string_create_with_decimal_point(
		          byte_size_string,
		          32,
		          size,
		          BYTE_SIZE_STRING_UNIT_MEGABYTE,
		          (int) '.',
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
	result = byte_size_string_create_with_decimal_point(
	          NULL,
	          32,
	          (size64_t) 8192 * 1024 * 1024,
	          BYTE_SIZE_STRING_UNIT_MEBIBYTE,
	          (int) '.',
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

	result = byte_size_string_create_with_decimal_point(
	          byte_size_string,
	          0,
	          (size64_t) 8192 * 1024 * 1024,
	          BYTE_SIZE_STRING_UNIT_MEBIBYTE,
	          (int) '.',
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

/* Tests the byte_size_string_convert function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_byte_size_string_convert(
     void )
{
	system_character_t byte_size_string[ 6 ] = {
		'8', ' ', 'G', 'i', 'B', 0 };

	libcerror_error_t *error                 = NULL;
	size64_t size                            = 0;
	int result                               = 0;

	/* Test regular cases
	 */
	result = byte_size_string_convert(
	          byte_size_string,
	          5,
	          &size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_UINT64(
	 "size",
	 size,
	 (uint64_t) 8192 * 1024 * 1024 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = byte_size_string_convert(
	          NULL,
	          5,
	          &size,
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

/* Tests the byte_size_string_convert_with_decimal_point function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_byte_size_string_convert_with_decimal_point(
     void )
{
	system_character_t byte_size_string1[ 6 ]  = {
		'8', ' ', 'G', 'i', 'B', 0 };

	system_character_t byte_size_string2[ 6 ]  = {
		'9', '9', '9', ' ', 'B', 0 };

	system_character_t byte_size_string3[ 5 ]  = {
		'8', ' ', 'k', 'B', 0 };

	system_character_t byte_size_string4[ 8 ] = {
		'8', '.', '9', ' ', 'K', 'i', 'B', 0 };

	system_character_t byte_size_string5[ 5 ]  = {
		'8', ' ', 'M', 'B', 0 };

	system_character_t byte_size_string6[ 5 ]  = {
		'8', ' ', 'G', 'B', 0 };

	system_character_t byte_size_string7[ 5 ]  = {
		'8', ' ', 'T', 'B', 0 };

	system_character_t byte_size_string8[ 5 ]  = {
		'8', ' ', 'P', 'B', 0 };

	system_character_t byte_size_string9[ 5 ]  = {
		'8', ' ', 'E', 'B', 0 };

	system_character_t byte_size_string10[ 5 ]  = {
		'8', ' ', 'Z', 'B', 0 };

	system_character_t byte_size_string11[ 5 ] = {
		'8', ' ', 'Y', 'B', 0 };

	system_character_t byte_size_string12[ 5 ] = {
		'8', ' ', 'k', 'g', 0 };

	libcerror_error_t *error                   = NULL;
	size64_t size                              = 0;
	int result                                 = 0;

	/* Test regular cases
	 */
	result = byte_size_string_convert_with_decimal_point(
	          byte_size_string1,
	          5,
	          (int) '.',
	          &size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_UINT64(
	 "size",
	 size,
	 (uint64_t) 8192 * 1024 * 1024 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = byte_size_string_convert_with_decimal_point(
	          byte_size_string2,
	          5,
	          (int) '.',
	          &size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_UINT64(
	 "size",
	 size,
	 (uint64_t) 999 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = byte_size_string_convert_with_decimal_point(
	          byte_size_string3,
	          4,
	          (int) '.',
	          &size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_UINT64(
	 "size",
	 size,
	 (uint64_t) 8000 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = byte_size_string_convert_with_decimal_point(
	          byte_size_string4,
	          7,
	          (int) '.',
	          &size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_UINT64(
	 "size",
	 size,
	 (uint64_t) 9092 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = byte_size_string_convert_with_decimal_point(
	          byte_size_string5,
	          4,
	          (int) '.',
	          &size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_UINT64(
	 "size",
	 size,
	 (uint64_t) 8000 * 1000 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = byte_size_string_convert_with_decimal_point(
	          byte_size_string6,
	          4,
	          (int) '.',
	          &size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_UINT64(
	 "size",
	 size,
	 (uint64_t) 8000 * 1000 * 1000 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = byte_size_string_convert_with_decimal_point(
	          byte_size_string7,
	          4,
	          (int) '.',
	          &size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_UINT64(
	 "size",
	 size,
	 (uint64_t) 8000 * 1000 * 1000 * 1000 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = byte_size_string_convert_with_decimal_point(
	          byte_size_string8,
	          4,
	          (int) '.',
	          &size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_UINT64(
	 "size",
	 size,
	 (uint64_t) 8000 * 1000 * 1000 * 1000 * 1000 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = byte_size_string_convert_with_decimal_point(
	          byte_size_string9,
	          4,
	          (int) '.',
	          &size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_UINT64(
	 "size",
	 size,
	 (uint64_t) 8000 * 1000 * 1000 * 1000 * 1000 * 1000 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = byte_size_string_convert_with_decimal_point(
	          byte_size_string10,
	          4,
	          (int) '.',
	          &size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_UINT64(
	 "size",
	 size,
	 (uint64_t) 8000 * 1000 * 1000 * 1000 * 1000 * 1000 * 1000 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = byte_size_string_convert_with_decimal_point(
	          byte_size_string11,
	          4,
	          (int) '.',
	          &size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_UINT64(
	 "size",
	 size,
	 (uint64_t) 8000 * 1000 * 1000 * 1000 * 1000 * 1000 * 1000 * 1000 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = byte_size_string_convert_with_decimal_point(
	          NULL,
	          5,
	          (int) '.',
	          &size,
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

	result = byte_size_string_convert_with_decimal_point(
	          byte_size_string1,
	          5,
	          (int) '.',
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

	result = byte_size_string_convert_with_decimal_point(
	          byte_size_string12,
	          4,
	          (int) '.',
	          &size,
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
	 "byte_size_string_create",
	 ewf_test_tools_byte_size_string_create );

	EWF_TEST_RUN(
	 "byte_size_string_create_with_decimal_point",
	 ewf_test_tools_byte_size_string_create_with_decimal_point );

	EWF_TEST_RUN(
	 "byte_size_string_convert",
	 ewf_test_tools_byte_size_string_convert );

	EWF_TEST_RUN(
	 "byte_size_string_convert_with_decimal_point",
	 ewf_test_tools_byte_size_string_convert_with_decimal_point );

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

