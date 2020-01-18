/*
 * Library huffman_tree type test program
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

#include "ewf_test_libcerror.h"
#include "ewf_test_libcnotify.h"
#include "ewf_test_libewf.h"
#include "ewf_test_macros.h"
#include "ewf_test_memory.h"
#include "ewf_test_unused.h"

#include "../libewf/libewf_bit_stream.h"
#include "../libewf/libewf_huffman_tree.h"

/* Define to make ewf_test_huffman_tree generate verbose output
#define EWF_TEST_HUFFMAN_TREE_VERBOSE
 */

/* The sequence: abcdefghijklmnopqrstuvwxyz compressed in LZXpress-Huffman
 */
uint8_t ewf_test_huffman_tree_data1[ 276 ] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x50, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x45, 0x44, 0x04, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xd8, 0x52, 0x3e, 0xd7, 0x94, 0x11, 0x5b, 0xe9, 0x19, 0x5f, 0xf9, 0xd6, 0x7c, 0xdf, 0x8d, 0x04,
	0x00, 0x00, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

/* Tests the libewf_huffman_tree_initialize function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_huffman_tree_initialize(
     void )
{
	libcerror_error_t *error             = NULL;
	libewf_huffman_tree_t *huffman_tree = NULL;
	int result                           = 0;

	/* Test libewf_huffman_tree_initialize
	 */
	result = libewf_huffman_tree_initialize(
	          &huffman_tree,
	          512,
	          15,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "huffman_tree",
	 huffman_tree );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_huffman_tree_free(
	          &huffman_tree,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "huffman_tree",
	 huffman_tree );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_huffman_tree_initialize(
	          NULL,
	          512,
	          15,
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

	huffman_tree = (libewf_huffman_tree_t *) 0x12345678UL;

	result = libewf_huffman_tree_initialize(
	          &huffman_tree,
	          512,
	          15,
	          &error );

	huffman_tree = NULL;

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_huffman_tree_initialize(
	          &huffman_tree,
	          9999,
	          15,
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

	result = libewf_huffman_tree_initialize(
	          &huffman_tree,
	          512,
	          99,
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

#if defined( HAVE_EWF_TEST_MEMORY )

	/* Test libewf_huffman_tree_initialize with malloc failing
	 */
	ewf_test_malloc_attempts_before_fail = 0;

	result = libewf_huffman_tree_initialize(
	          &huffman_tree,
	          512,
	          15,
	          &error );

	if( ewf_test_malloc_attempts_before_fail != -1 )
	{
		ewf_test_malloc_attempts_before_fail = -1;

		if( huffman_tree != NULL )
		{
			libewf_huffman_tree_free(
			 &huffman_tree,
			 NULL );
		}
	}
	else
	{
		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		EWF_TEST_ASSERT_IS_NULL(
		 "huffman_tree",
		 huffman_tree );

		EWF_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	/* Test libewf_huffman_tree_initialize with memset failing
	 */
	ewf_test_memset_attempts_before_fail = 0;

	result = libewf_huffman_tree_initialize(
	          &huffman_tree,
	          512,
	          15,
	          &error );

	if( ewf_test_memset_attempts_before_fail != -1 )
	{
		ewf_test_memset_attempts_before_fail = -1;

		if( huffman_tree != NULL )
		{
			libewf_huffman_tree_free(
			 &huffman_tree,
			 NULL );
		}
	}
	else
	{
		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		EWF_TEST_ASSERT_IS_NULL(
		 "huffman_tree",
		 huffman_tree );

		EWF_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_EWF_TEST_MEMORY ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( huffman_tree != NULL )
	{
		libewf_huffman_tree_free(
		 &huffman_tree,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_huffman_tree_free function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_huffman_tree_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libewf_huffman_tree_free(
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

/* Tests the libewf_huffman_tree_build function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_huffman_tree_build(
     void )
{
	uint8_t code_size_array[ 512 ];

	libcerror_error_t *error             = NULL;
	libewf_huffman_tree_t *huffman_tree = NULL;
	size_t byte_offset                   = 0;
	uint32_t symbol                      = 0;
	uint8_t byte_value                   = 0;
	int result                           = 0;

	/* Initialize test
	 */
	for( byte_offset = 0;
	     byte_offset < 256;
	     byte_offset++ )
	{
		byte_value = ewf_test_huffman_tree_data1[ byte_offset ];

		code_size_array[ symbol++ ] = byte_value & 0x0f;

		byte_value >>= 4;

		code_size_array[ symbol++ ] = byte_value & 0x0f;
	}
	result = libewf_huffman_tree_initialize(
	          &huffman_tree,
	          512,
	          15,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "huffman_tree",
	 huffman_tree );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_huffman_tree_build(
	          huffman_tree,
	          code_size_array,
	          512,
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
	result = libewf_huffman_tree_build(
	          NULL,
	          code_size_array,
	          512,
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

	result = libewf_huffman_tree_build(
	          huffman_tree,
	          NULL,
	          512,
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

	result = libewf_huffman_tree_build(
	          huffman_tree,
	          code_size_array,
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

#if defined( HAVE_EWF_TEST_MEMORY )

	/* Test libewf_huffman_tree_build with malloc failing
	 */
	ewf_test_malloc_attempts_before_fail = 0;

	result = libewf_huffman_tree_build(
	          huffman_tree,
	          code_size_array,
	          512,
	          &error );

	if( ewf_test_malloc_attempts_before_fail != -1 )
	{
		ewf_test_malloc_attempts_before_fail = -1;
	}
	else
	{
		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		EWF_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#if defined( OPTIMIZATION_DISABLED )

	/* Test libewf_huffman_tree_build with memset failing
	 */
	ewf_test_memset_attempts_before_fail = 0;

	result = libewf_huffman_tree_build(
	          huffman_tree,
	          code_size_array,
	          512,
	          &error );

	if( ewf_test_memset_attempts_before_fail != -1 )
	{
		ewf_test_memset_attempts_before_fail = -1;
	}
	else
	{
		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		EWF_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( OPTIMIZATION_DISABLED ) */
#endif /* defined( HAVE_EWF_TEST_MEMORY ) */

	/* TODO test errornous data */

	/* Clean up
	 */
	result = libewf_huffman_tree_free(
	          &huffman_tree,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "huffman_tree",
	 huffman_tree );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( huffman_tree != NULL )
	{
		libewf_huffman_tree_free(
		 &huffman_tree,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_huffman_tree_get_symbol_from_bit_stream function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_huffman_tree_get_symbol_from_bit_stream(
     void )
{
	uint8_t code_size_array[ 512 ];

	libcerror_error_t *error             = NULL;
	libewf_bit_stream_t *bit_stream     = NULL;
	libewf_huffman_tree_t *huffman_tree = NULL;
	size_t byte_offset                   = 0;
	uint32_t symbol                      = 0;
	uint8_t byte_value                   = 0;
	int result                           = 0;

	/* Initialize test
	 */
	for( byte_offset = 0;
	     byte_offset < 256;
	     byte_offset++ )
	{
		byte_value = ewf_test_huffman_tree_data1[ byte_offset ];

		code_size_array[ symbol++ ] = byte_value & 0x0f;

		byte_value >>= 4;

		code_size_array[ symbol++ ] = byte_value & 0x0f;
	}
	result = libewf_huffman_tree_initialize(
	          &huffman_tree,
	          512,
	          15,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "huffman_tree",
	 huffman_tree );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_huffman_tree_build(
	          huffman_tree,
	          code_size_array,
	          512,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_bit_stream_initialize(
	          &bit_stream,
	          &( ewf_test_huffman_tree_data1[ 256 ] ),
	          277 - 256,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "bit_stream",
	 bit_stream );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	symbol = 0;

	result = libewf_huffman_tree_get_symbol_from_bit_stream(
	          huffman_tree,
	          bit_stream,
	          &symbol,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_UINT32(
	 "symbol",
	 symbol,
	 (uint32_t) 120 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	symbol = 0;

	result = libewf_huffman_tree_get_symbol_from_bit_stream(
	          NULL,
	          bit_stream,
	          &symbol,
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

	result = libewf_huffman_tree_get_symbol_from_bit_stream(
	          huffman_tree,
	          NULL,
	          &symbol,
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

	result = libewf_huffman_tree_get_symbol_from_bit_stream(
	          huffman_tree,
	          bit_stream,
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

	/* Clean up
	 */
	result = libewf_huffman_tree_free(
	          &huffman_tree,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "huffman_tree",
	 huffman_tree );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_bit_stream_free(
	          &bit_stream,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "bit_stream",
	 bit_stream );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( huffman_tree != NULL )
	{
		libewf_huffman_tree_free(
		 &huffman_tree,
		 NULL );
	}
	if( bit_stream != NULL )
	{
		libewf_bit_stream_free(
		 &bit_stream,
		 NULL );
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

#if defined( HAVE_DEBUG_OUTPUT ) && defined( EWF_TEST_HUFFMAN_TREE_VERBOSE )
	libcnotify_verbose_set(
	 1 );
	libcnotify_stream_set(
	 stderr,
	 NULL );
#endif

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

	EWF_TEST_RUN(
	 "libewf_huffman_tree_initialize",
	 ewf_test_huffman_tree_initialize );

	EWF_TEST_RUN(
	 "libewf_huffman_tree_free",
	 ewf_test_huffman_tree_free );

	EWF_TEST_RUN(
	 "libewf_huffman_tree_build",
	 ewf_test_huffman_tree_build );

	EWF_TEST_RUN(
	 "libewf_huffman_tree_get_symbol_from_bit_stream",
	 ewf_test_huffman_tree_get_symbol_from_bit_stream );

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )
on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */
}

