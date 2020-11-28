/*
 * Library write_io_handle type test program
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

#include "ewf_test_functions.h"
#include "ewf_test_libcerror.h"
#include "ewf_test_libewf.h"
#include "ewf_test_macros.h"
#include "ewf_test_memory.h"
#include "ewf_test_unused.h"

#include "../libewf/libewf_io_handle.h"
#include "../libewf/libewf_media_values.h"
#include "../libewf/libewf_segment_file.h"
#include "../libewf/libewf_segment_table.h"
#include "../libewf/libewf_write_io_handle.h"

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

/* Tests the libewf_write_io_handle_initialize function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_write_io_handle_initialize(
     void )
{
	libcerror_error_t *error                  = NULL;
	libewf_io_handle_t *io_handle             = NULL;
	libewf_write_io_handle_t *write_io_handle = NULL;
	int result                                = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests           = 2;
	int number_of_memset_fail_tests           = 1;
	int test_number                           = 0;
#endif

	/* Initialize test
	 */
	result = libewf_io_handle_initialize(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_write_io_handle_initialize(
	          &write_io_handle,
	          io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "write_io_handle",
	 write_io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_write_io_handle_free(
	          &write_io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "write_io_handle",
	 write_io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_write_io_handle_initialize(
	          NULL,
	          io_handle,
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

	write_io_handle = (libewf_write_io_handle_t *) 0x12345678UL;

	result = libewf_write_io_handle_initialize(
	          &write_io_handle,
	          io_handle,
	          &error );

	write_io_handle = NULL;

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_write_io_handle_initialize(
	          &write_io_handle,
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

#if defined( HAVE_EWF_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libewf_write_io_handle_initialize with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = libewf_write_io_handle_initialize(
		          &write_io_handle,
		          io_handle,
		          &error );

		if( ewf_test_malloc_attempts_before_fail != -1 )
		{
			ewf_test_malloc_attempts_before_fail = -1;

			if( write_io_handle != NULL )
			{
				libewf_write_io_handle_free(
				 &write_io_handle,
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
			 "write_io_handle",
			 write_io_handle );

			EWF_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
	for( test_number = 0;
	     test_number < number_of_memset_fail_tests;
	     test_number++ )
	{
		/* Test libewf_write_io_handle_initialize with memset failing
		 */
		ewf_test_memset_attempts_before_fail = test_number;

		result = libewf_write_io_handle_initialize(
		          &write_io_handle,
		          io_handle,
		          &error );

		if( ewf_test_memset_attempts_before_fail != -1 )
		{
			ewf_test_memset_attempts_before_fail = -1;

			if( write_io_handle != NULL )
			{
				libewf_write_io_handle_free(
				 &write_io_handle,
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
			 "write_io_handle",
			 write_io_handle );

			EWF_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_EWF_TEST_MEMORY ) */

	/* Clean up
	 */
	result = libewf_io_handle_free(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( write_io_handle != NULL )
	{
		libewf_write_io_handle_free(
		 &write_io_handle,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libewf_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_write_io_handle_free function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_write_io_handle_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libewf_write_io_handle_free(
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

/* Tests the libewf_write_io_handle_clone function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_write_io_handle_clone(
     void )
{
	libcerror_error_t *error                              = NULL;
	libewf_io_handle_t *io_handle                         = NULL;
	libewf_write_io_handle_t *destination_write_io_handle = NULL;
	libewf_write_io_handle_t *source_write_io_handle      = NULL;
	int result                                            = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests                       = 1;
	int test_number                                       = 0;

#if defined( OPTIMIZATION_DISABLED )
	int number_of_memcpy_fail_tests                       = 1;
#endif
#endif /* defined( HAVE_EWF_TEST_MEMORY ) */

	/* Initialize test
	 */
	result = libewf_io_handle_initialize(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_write_io_handle_initialize(
	          &source_write_io_handle,
	          io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "source_write_io_handle",
	 source_write_io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_write_io_handle_clone(
	          &destination_write_io_handle,
	          source_write_io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "destination_write_io_handle",
	 destination_write_io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_write_io_handle_free(
	          &destination_write_io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "destination_write_io_handle",
	 destination_write_io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_write_io_handle_clone(
	          &destination_write_io_handle,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "destination_write_io_handle",
	 destination_write_io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_write_io_handle_clone(
	          NULL,
	          source_write_io_handle,
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

	destination_write_io_handle = (libewf_write_io_handle_t *) 0x12345678UL;

	result = libewf_write_io_handle_clone(
	          &destination_write_io_handle,
	          source_write_io_handle,
	          &error );

	destination_write_io_handle = NULL;

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

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libewf_write_io_handle_clone with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = libewf_write_io_handle_clone(
		          &destination_write_io_handle,
		          source_write_io_handle,
		          &error );

		if( ewf_test_malloc_attempts_before_fail != -1 )
		{
			ewf_test_malloc_attempts_before_fail = -1;

			if( destination_write_io_handle != NULL )
			{
				libewf_write_io_handle_free(
				 &destination_write_io_handle,
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
			 "destination_write_io_handle",
			 destination_write_io_handle );

			EWF_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#if defined( OPTIMIZATION_DISABLED )

	for( test_number = 0;
	     test_number < number_of_memcpy_fail_tests;
	     test_number++ )
	{
		/* Test libewf_write_io_handle_clone with memcpy failing
		 */
		ewf_test_memcpy_attempts_before_fail = test_number;

		result = libewf_write_io_handle_clone(
		          &destination_write_io_handle,
		          source_write_io_handle,
		          &error );

		if( ewf_test_memcpy_attempts_before_fail != -1 )
		{
			ewf_test_memcpy_attempts_before_fail = -1;

			if( destination_write_io_handle != NULL )
			{
				libewf_write_io_handle_free(
				 &destination_write_io_handle,
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
			 "destination_write_io_handle",
			 destination_write_io_handle );

			EWF_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( OPTIMIZATION_DISABLED ) */
#endif /* defined( HAVE_EWF_TEST_MEMORY ) */

	/* Clean up
	 */
	result = libewf_write_io_handle_free(
	          &source_write_io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "source_write_io_handle",
	 source_write_io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_io_handle_free(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( destination_write_io_handle != NULL )
	{
		libewf_write_io_handle_free(
		 &destination_write_io_handle,
		 NULL );
	}
	if( source_write_io_handle != NULL )
	{
		libewf_write_io_handle_free(
		 &source_write_io_handle,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libewf_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_write_io_handle_initialize_values function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_write_io_handle_initialize_values(
     void )
{
	libcerror_error_t *error                  = NULL;
	libewf_io_handle_t *io_handle             = NULL;
	libewf_media_values_t *media_values       = NULL;
	libewf_segment_table_t *segment_table     = NULL;
	libewf_write_io_handle_t *write_io_handle = NULL;
	int result                                = 0;

	/* Initialize test
	 */
	result = libewf_io_handle_initialize(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	io_handle->format = LIBEWF_FORMAT_V2_ENCASE7;

	result = libewf_media_values_initialize(
	          &media_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "media_values",
	 media_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_segment_table_initialize(
	          &segment_table,
	          io_handle,
	          LIBEWF_DEFAULT_SEGMENT_FILE_SIZE,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "segment_table",
	 segment_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_write_io_handle_initialize(
	          &write_io_handle,
	          io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "write_io_handle",
	 write_io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_write_io_handle_initialize_values(
	          write_io_handle,
	          io_handle,
	          media_values,
	          segment_table,
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
	result = libewf_write_io_handle_initialize_values(
	          NULL,
	          io_handle,
	          media_values,
	          segment_table,
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

	result = libewf_write_io_handle_initialize_values(
	          write_io_handle,
	          NULL,
	          media_values,
	          segment_table,
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

	result = libewf_write_io_handle_initialize_values(
	          write_io_handle,
	          io_handle,
	          NULL,
	          segment_table,
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

	result = libewf_write_io_handle_initialize_values(
	          write_io_handle,
	          io_handle,
	          media_values,
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
	result = libewf_write_io_handle_free(
	          &write_io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "write_io_handle",
	 write_io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_segment_table_free(
	          &segment_table,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "segment_table",
	 segment_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_media_values_free(
	          &media_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "media_values",
	 media_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_io_handle_free(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( write_io_handle != NULL )
	{
		libewf_write_io_handle_free(
		 &write_io_handle,
		 NULL );
	}
	if( segment_table != NULL )
	{
		libewf_segment_table_free(
		 &segment_table,
		 NULL );
	}
	if( media_values != NULL )
	{
		libewf_media_values_free(
		 &media_values,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libewf_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_write_io_handle_resize_table_entries function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_write_io_handle_resize_table_entries(
     void )
{
	libcerror_error_t *error                  = NULL;
	libewf_io_handle_t *io_handle             = NULL;
	libewf_write_io_handle_t *write_io_handle = NULL;
	int result                                = 0;

	/* Initialize test
	 */
	result = libewf_io_handle_initialize(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	io_handle->format = LIBEWF_FORMAT_V2_ENCASE7;

	result = libewf_write_io_handle_initialize(
	          &write_io_handle,
	          io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "write_io_handle",
	 write_io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	write_io_handle->table_entry_size = 16;

	/* Test regular cases
	 */
	result = libewf_write_io_handle_resize_table_entries(
	          write_io_handle,
	          8,
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
	result = libewf_write_io_handle_resize_table_entries(
	          NULL,
	          8,
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

	result = libewf_write_io_handle_resize_table_entries(
	          write_io_handle,
	          0xffffffffUL,
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
	result = libewf_write_io_handle_free(
	          &write_io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "write_io_handle",
	 write_io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_io_handle_free(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( write_io_handle != NULL )
	{
		libewf_write_io_handle_free(
		 &write_io_handle,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libewf_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_write_io_handle_calculate_chunks_per_segment_file function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_write_io_handle_calculate_chunks_per_segment_file(
     void )
{
	libcerror_error_t *error                  = NULL;
	libewf_io_handle_t *io_handle             = NULL;
	libewf_media_values_t *media_values       = NULL;
	libewf_write_io_handle_t *write_io_handle = NULL;
	int result                                = 0;

	/* Initialize test
	 */
	result = libewf_io_handle_initialize(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	io_handle->format = LIBEWF_FORMAT_V2_ENCASE7;

	result = libewf_media_values_initialize(
	          &media_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "media_values",
	 media_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_write_io_handle_initialize(
	          &write_io_handle,
	          io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "write_io_handle",
	 write_io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	write_io_handle->table_entry_size = 16;

	/* Test regular cases
	 */
	result = libewf_write_io_handle_calculate_chunks_per_segment_file(
	          write_io_handle,
	          media_values,
	          LIBEWF_SEGMENT_FILE_TYPE_EWF1,
	          LIBEWF_FORMAT_ENCASE7,
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
	result = libewf_write_io_handle_calculate_chunks_per_segment_file(
	          NULL,
	          media_values,
	          LIBEWF_SEGMENT_FILE_TYPE_EWF1,
	          LIBEWF_FORMAT_ENCASE7,
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
	result = libewf_write_io_handle_free(
	          &write_io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "write_io_handle",
	 write_io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_media_values_free(
	          &media_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "media_values",
	 media_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_io_handle_free(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( write_io_handle != NULL )
	{
		libewf_write_io_handle_free(
		 &write_io_handle,
		 NULL );
	}
	if( media_values != NULL )
	{
		libewf_media_values_free(
		 &media_values,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libewf_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_write_io_handle_calculate_chunks_per_section function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_write_io_handle_calculate_chunks_per_section(
     void )
{
	libcerror_error_t *error                  = NULL;
	libewf_io_handle_t *io_handle             = NULL;
	libewf_write_io_handle_t *write_io_handle = NULL;
	int result                                = 0;

	/* Initialize test
	 */
	result = libewf_io_handle_initialize(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	io_handle->format = LIBEWF_FORMAT_V2_ENCASE7;

	result = libewf_write_io_handle_initialize(
	          &write_io_handle,
	          io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "write_io_handle",
	 write_io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	write_io_handle->table_entry_size = 16;

	/* Test regular cases
	 */
	result = libewf_write_io_handle_calculate_chunks_per_section(
	          write_io_handle,
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
	result = libewf_write_io_handle_calculate_chunks_per_section(
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
	result = libewf_write_io_handle_free(
	          &write_io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "write_io_handle",
	 write_io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_io_handle_free(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( write_io_handle != NULL )
	{
		libewf_write_io_handle_free(
		 &write_io_handle,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libewf_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_write_io_handle_test_segment_file_full function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_write_io_handle_test_segment_file_full(
     void )
{
	libcerror_error_t *error                  = NULL;
	libewf_io_handle_t *io_handle             = NULL;
	libewf_media_values_t *media_values       = NULL;
	libewf_write_io_handle_t *write_io_handle = NULL;
	int result                                = 0;

	/* Initialize test
	 */
	result = libewf_io_handle_initialize(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	io_handle->format = LIBEWF_FORMAT_V2_ENCASE7;

	result = libewf_media_values_initialize(
	          &media_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "media_values",
	 media_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_write_io_handle_initialize(
	          &write_io_handle,
	          io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "write_io_handle",
	 write_io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	write_io_handle->table_entry_size = 16;

	/* Test regular cases
	 */
	result = libewf_write_io_handle_test_segment_file_full(
	          write_io_handle,
	          media_values,
	          LIBEWF_SEGMENT_FILE_TYPE_EWF1,
	          LIBEWF_FORMAT_ENCASE7,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_write_io_handle_test_segment_file_full(
	          NULL,
	          media_values,
	          LIBEWF_SEGMENT_FILE_TYPE_EWF1,
	          LIBEWF_FORMAT_ENCASE7,
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
	result = libewf_write_io_handle_free(
	          &write_io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "write_io_handle",
	 write_io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_media_values_free(
	          &media_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "media_values",
	 media_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_io_handle_free(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( write_io_handle != NULL )
	{
		libewf_write_io_handle_free(
		 &write_io_handle,
		 NULL );
	}
	if( media_values != NULL )
	{
		libewf_media_values_free(
		 &media_values,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libewf_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_write_io_handle_test_chunks_section_full function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_write_io_handle_test_chunks_section_full(
     void )
{
	libcerror_error_t *error                  = NULL;
	libewf_io_handle_t *io_handle             = NULL;
	libewf_media_values_t *media_values       = NULL;
	libewf_write_io_handle_t *write_io_handle = NULL;
	int result                                = 0;

	/* Initialize test
	 */
	result = libewf_io_handle_initialize(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	io_handle->format = LIBEWF_FORMAT_V2_ENCASE7;

	result = libewf_media_values_initialize(
	          &media_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "media_values",
	 media_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_write_io_handle_initialize(
	          &write_io_handle,
	          io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "write_io_handle",
	 write_io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	write_io_handle->table_entry_size = 16;

	/* Test regular cases
	 */
	result = libewf_write_io_handle_test_chunks_section_full(
	          write_io_handle,
	          media_values,
	          0,
	          LIBEWF_SEGMENT_FILE_TYPE_EWF1,
	          LIBEWF_FORMAT_ENCASE7,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_write_io_handle_test_chunks_section_full(
	          NULL,
	          media_values,
	          0,
	          LIBEWF_SEGMENT_FILE_TYPE_EWF1,
	          LIBEWF_FORMAT_ENCASE7,
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
	result = libewf_write_io_handle_free(
	          &write_io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "write_io_handle",
	 write_io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_media_values_free(
	          &media_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "media_values",
	 media_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_io_handle_free(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( write_io_handle != NULL )
	{
		libewf_write_io_handle_free(
		 &write_io_handle,
		 NULL );
	}
	if( media_values != NULL )
	{
		libewf_media_values_free(
		 &media_values,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libewf_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_write_io_handle_generate_table_entries_data function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_write_io_handle_generate_table_entries_data(
     void )
{
	uint8_t table_entries_data[ 16 ];

	libcerror_error_t *error                  = NULL;
	libewf_io_handle_t *io_handle             = NULL;
	libewf_write_io_handle_t *write_io_handle = NULL;
	int result                                = 0;

	/* Initialize test
	 */
	result = libewf_io_handle_initialize(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_write_io_handle_initialize(
	          &write_io_handle,
	          io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "write_io_handle",
	 write_io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libewf_write_io_handle_generate_table_entries_data(
	          NULL,
	          1,
	          0,
	          table_entries_data,
	          16,
	          0,
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

	result = libewf_write_io_handle_generate_table_entries_data(
	          write_io_handle,
	          0,
	          0,
	          table_entries_data,
	          16,
	          0,
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

	result = libewf_write_io_handle_generate_table_entries_data(
	          write_io_handle,
	          1,
	          0,
	          NULL,
	          16,
	          0,
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

	result = libewf_write_io_handle_generate_table_entries_data(
	          write_io_handle,
	          1,
	          0,
	          table_entries_data,
	          (size_t) SSIZE_MAX + 1,
	          0,
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

	result = libewf_write_io_handle_generate_table_entries_data(
	          write_io_handle,
	          1,
	          0,
	          table_entries_data,
	          0,
	          0,
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

	result = libewf_write_io_handle_generate_table_entries_data(
	          write_io_handle,
	          1,
	          0,
	          table_entries_data,
	          16,
	          0,
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

	/* Clean up
	 */
	result = libewf_write_io_handle_free(
	          &write_io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "write_io_handle",
	 write_io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_io_handle_free(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( write_io_handle != NULL )
	{
		libewf_write_io_handle_free(
		 &write_io_handle,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libewf_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_write_io_handle_write_chunks_section_start function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_write_io_handle_write_chunks_section_start(
     void )
{
	uint8_t section_data[ 256 ];

	libbfio_pool_t *file_io_pool              = NULL;
	libcerror_error_t *error                  = NULL;
	libewf_io_handle_t *io_handle             = NULL;
	libewf_segment_file_t *segment_file       = NULL;
	libewf_write_io_handle_t *write_io_handle = NULL;
	ssize_t write_count                       = 0;
	int result                                = 0;

	/* Initialize test
	 */
	result = libewf_io_handle_initialize(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	io_handle->format = LIBEWF_FORMAT_V2_ENCASE7;

	result = libewf_segment_file_initialize(
	          &segment_file,
	          io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "segment_file",
	 segment_file );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_write_io_handle_initialize(
	          &write_io_handle,
	          io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "write_io_handle",
	 write_io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize file IO pool
	 */
	result = ewf_test_open_file_io_pool(
	          &file_io_pool,
	          section_data,
	          256,
	          LIBBFIO_OPEN_WRITE,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "file_io_pool",
	 file_io_pool );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
/* TODO implement tests
	write_count = libewf_write_io_handle_write_chunks_section_start(
	               section_descriptor,
	               io_handle,
	               file_io_pool,
	               0,
	               segment_file,
	               &error );

	EWF_TEST_ASSERT_EQUAL_SSIZE(
	 "write_count",
	 write_count,
	 (ssize_t) 156 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );
*/

	/* Test error cases
	 */
	write_count = libewf_write_io_handle_write_chunks_section_start(
	               NULL,
	               io_handle,
	               file_io_pool,
	               0,
	               segment_file,
	               &error );

	EWF_TEST_ASSERT_EQUAL_SSIZE(
	 "write_count",
	 write_count,
	 (ssize_t) -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	write_count = libewf_write_io_handle_write_chunks_section_start(
	               write_io_handle,
	               NULL,
	               file_io_pool,
	               0,
	               segment_file,
	               &error );

	EWF_TEST_ASSERT_EQUAL_SSIZE(
	 "write_count",
	 write_count,
	 (ssize_t) -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	write_count = libewf_write_io_handle_write_chunks_section_start(
	               write_io_handle,
	               io_handle,
	               file_io_pool,
	               0,
	               NULL,
	               &error );

	EWF_TEST_ASSERT_EQUAL_SSIZE(
	 "write_count",
	 write_count,
	 (ssize_t) -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up file IO pool
	 */
	result = ewf_test_close_file_io_pool(
	          &file_io_pool,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libewf_write_io_handle_free(
	          &write_io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "write_io_handle",
	 write_io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_segment_file_free(
	          &segment_file,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "segment_file",
	 segment_file );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_io_handle_free(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( file_io_pool != NULL )
	{
		libbfio_pool_free(
		 &file_io_pool,
		 NULL );
	}
	if( write_io_handle != NULL )
	{
		libewf_write_io_handle_free(
		 &write_io_handle,
		 NULL );
	}
	if( segment_file != NULL )
	{
		libewf_segment_file_free(
		 &segment_file,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libewf_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_write_io_handle_write_chunks_section_end function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_write_io_handle_write_chunks_section_end(
     void )
{
	uint8_t section_data[ 256 ];

	libbfio_pool_t *file_io_pool              = NULL;
	libcerror_error_t *error                  = NULL;
	libewf_io_handle_t *io_handle             = NULL;
	libewf_segment_file_t *segment_file       = NULL;
	libewf_write_io_handle_t *write_io_handle = NULL;
	ssize_t write_count                       = 0;
	int result                                = 0;

	/* Initialize test
	 */
	result = libewf_io_handle_initialize(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	io_handle->format = LIBEWF_FORMAT_V2_ENCASE7;

	result = libewf_segment_file_initialize(
	          &segment_file,
	          io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "segment_file",
	 segment_file );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_write_io_handle_initialize(
	          &write_io_handle,
	          io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "write_io_handle",
	 write_io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize file IO pool
	 */
	result = ewf_test_open_file_io_pool(
	          &file_io_pool,
	          section_data,
	          256,
	          LIBBFIO_OPEN_WRITE,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "file_io_pool",
	 file_io_pool );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
/* TODO implement tests
	write_count = libewf_write_io_handle_write_chunks_section_end(
	               section_descriptor,
	               io_handle,
	               file_io_pool,
	               0,
	               segment_file,
	               &error );

	EWF_TEST_ASSERT_EQUAL_SSIZE(
	 "write_count",
	 write_count,
	 (ssize_t) 156 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );
*/

	/* Test error cases
	 */
	write_count = libewf_write_io_handle_write_chunks_section_end(
	               NULL,
	               io_handle,
	               file_io_pool,
	               0,
	               segment_file,
	               &error );

	EWF_TEST_ASSERT_EQUAL_SSIZE(
	 "write_count",
	 write_count,
	 (ssize_t) -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	write_count = libewf_write_io_handle_write_chunks_section_end(
	               write_io_handle,
	               NULL,
	               file_io_pool,
	               0,
	               segment_file,
	               &error );

	EWF_TEST_ASSERT_EQUAL_SSIZE(
	 "write_count",
	 write_count,
	 (ssize_t) -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	write_count = libewf_write_io_handle_write_chunks_section_end(
	               write_io_handle,
	               io_handle,
	               file_io_pool,
	               0,
	               NULL,
	               &error );

	EWF_TEST_ASSERT_EQUAL_SSIZE(
	 "write_count",
	 write_count,
	 (ssize_t) -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up file IO pool
	 */
	result = ewf_test_close_file_io_pool(
	          &file_io_pool,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libewf_write_io_handle_free(
	          &write_io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "write_io_handle",
	 write_io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_segment_file_free(
	          &segment_file,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "segment_file",
	 segment_file );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_io_handle_free(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( file_io_pool != NULL )
	{
		libbfio_pool_free(
		 &file_io_pool,
		 NULL );
	}
	if( write_io_handle != NULL )
	{
		libewf_write_io_handle_free(
		 &write_io_handle,
		 NULL );
	}
	if( segment_file != NULL )
	{
		libewf_segment_file_free(
		 &segment_file,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libewf_io_handle_free(
		 &io_handle,
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

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

	EWF_TEST_RUN(
	 "libewf_write_io_handle_initialize",
	 ewf_test_write_io_handle_initialize );

	EWF_TEST_RUN(
	 "libewf_write_io_handle_free",
	 ewf_test_write_io_handle_free );

	EWF_TEST_RUN(
	 "libewf_write_io_handle_clone",
	 ewf_test_write_io_handle_clone );

	EWF_TEST_RUN(
	 "libewf_write_io_handle_initialize_values",
	 ewf_test_write_io_handle_initialize_values );

	/* TODO: add tests for libewf_write_io_handle_initialize_resume */

	EWF_TEST_RUN(
	 "libewf_write_io_handle_resize_table_entries",
	 ewf_test_write_io_handle_resize_table_entries );

	EWF_TEST_RUN(
	 "libewf_write_io_handle_calculate_chunks_per_segment_file",
	 ewf_test_write_io_handle_calculate_chunks_per_segment_file );

	EWF_TEST_RUN(
	 "libewf_write_io_handle_calculate_chunks_per_section",
	 ewf_test_write_io_handle_calculate_chunks_per_section );

	EWF_TEST_RUN(
	 "libewf_write_io_handle_test_segment_file_full",
	 ewf_test_write_io_handle_test_segment_file_full );

	EWF_TEST_RUN(
	 "libewf_write_io_handle_test_chunks_section_full",
	 ewf_test_write_io_handle_test_chunks_section_full );

	/* TODO: add tests for libewf_write_io_handle_create_segment_file */

	EWF_TEST_RUN(
	 "libewf_write_io_handle_generate_table_entries_data",
	 ewf_test_write_io_handle_generate_table_entries_data );

	EWF_TEST_RUN(
	 "libewf_write_io_handle_write_chunks_section_start",
	 ewf_test_write_io_handle_write_chunks_section_start );

	EWF_TEST_RUN(
	 "libewf_write_io_handle_write_chunks_section_end",
	 ewf_test_write_io_handle_write_chunks_section_end );

	/* TODO: add tests for libewf_write_io_handle_write_new_chunk */

	/* TODO: add tests for libewf_write_io_handle_finalize_write_sections_corrections */

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */
}

