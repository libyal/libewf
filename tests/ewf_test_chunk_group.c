/*
 * Library chunk_group type test program
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
#include "ewf_test_libewf.h"
#include "ewf_test_macros.h"
#include "ewf_test_memory.h"
#include "ewf_test_rwlock.h"
#include "ewf_test_unused.h"

#include "../libewf/libewf_chunk_group.h"
#include "../libewf/libewf_section_descriptor.h"

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

/* Tests the libewf_chunk_group_initialize function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_chunk_group_initialize(
     void )
{
	libcerror_error_t *error          = NULL;
	libewf_chunk_group_t *chunk_group = NULL;
	libewf_io_handle_t *io_handle     = NULL;
	int result                        = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests   = 2;
	int number_of_memset_fail_tests   = 1;
	int test_number                   = 0;
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
	result = libewf_chunk_group_initialize(
	          &chunk_group,
	          io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "chunk_group",
	 chunk_group );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_chunk_group_free(
	          &chunk_group,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "chunk_group",
	 chunk_group );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_chunk_group_initialize(
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

	chunk_group = (libewf_chunk_group_t *) 0x12345678UL;

	result = libewf_chunk_group_initialize(
	          &chunk_group,
	          io_handle,
	          &error );

	chunk_group = NULL;

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_chunk_group_initialize(
	          &chunk_group,
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
		/* Test libewf_chunk_group_initialize with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = libewf_chunk_group_initialize(
		          &chunk_group,
		          io_handle,
		          &error );

		if( ewf_test_malloc_attempts_before_fail != -1 )
		{
			ewf_test_malloc_attempts_before_fail = -1;

			if( chunk_group != NULL )
			{
				libewf_chunk_group_free(
				 &chunk_group,
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
			 "chunk_group",
			 chunk_group );

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
		/* Test libewf_chunk_group_initialize with memset failing
		 */
		ewf_test_memset_attempts_before_fail = test_number;

		result = libewf_chunk_group_initialize(
		          &chunk_group,
		          io_handle,
		          &error );

		if( ewf_test_memset_attempts_before_fail != -1 )
		{
			ewf_test_memset_attempts_before_fail = -1;

			if( chunk_group != NULL )
			{
				libewf_chunk_group_free(
				 &chunk_group,
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
			 "chunk_group",
			 chunk_group );

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
	if( chunk_group != NULL )
	{
		libewf_chunk_group_free(
		 &chunk_group,
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

/* Tests the libewf_chunk_group_free function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_chunk_group_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libewf_chunk_group_free(
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

/* Tests the libewf_chunk_group_empty function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_chunk_group_empty(
     void )
{
	libcerror_error_t *error          = NULL;
	libewf_chunk_group_t *chunk_group = NULL;
	libewf_io_handle_t *io_handle     = NULL;
	int result                        = 0;

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

	result = libewf_chunk_group_initialize(
	          &chunk_group,
	          io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "chunk_group",
	 chunk_group );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_chunk_group_empty(
	          chunk_group,
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
	result = libewf_chunk_group_empty(
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
	result = libewf_chunk_group_free(
	          &chunk_group,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "chunk_group",
	 chunk_group );

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
	if( chunk_group != NULL )
	{
		libewf_chunk_group_free(
		 &chunk_group,
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

/* Tests the libewf_chunk_group_clone function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_chunk_group_clone(
     void )
{
	libcerror_error_t *error                      = NULL;
	libewf_chunk_group_t *destination_chunk_group = NULL;
	libewf_chunk_group_t *source_chunk_group      = NULL;
	libewf_io_handle_t *io_handle                 = NULL;
	int result                                    = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests               = 1;
	int test_number                               = 0;

#if defined( OPTIMIZATION_DISABLED )
	int number_of_memcpy_fail_tests               = 1;
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

	result = libewf_chunk_group_initialize(
	          &source_chunk_group,
	          io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "source_chunk_group",
	 source_chunk_group );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_chunk_group_clone(
	          &destination_chunk_group,
	          source_chunk_group,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "destination_chunk_group",
	 destination_chunk_group );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_chunk_group_free(
	          &destination_chunk_group,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "destination_chunk_group",
	 destination_chunk_group );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_chunk_group_clone(
	          &destination_chunk_group,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "destination_chunk_group",
	 destination_chunk_group );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_chunk_group_clone(
	          NULL,
	          source_chunk_group,
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

	destination_chunk_group = (libewf_chunk_group_t *) 0x12345678UL;

	result = libewf_chunk_group_clone(
	          &destination_chunk_group,
	          source_chunk_group,
	          &error );

	destination_chunk_group = NULL;

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
		/* Test libewf_chunk_group_clone with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = libewf_chunk_group_clone(
		          &destination_chunk_group,
		          source_chunk_group,
		          &error );

		if( ewf_test_malloc_attempts_before_fail != -1 )
		{
			ewf_test_malloc_attempts_before_fail = -1;

			if( destination_chunk_group != NULL )
			{
				libewf_chunk_group_free(
				 &destination_chunk_group,
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
			 "destination_chunk_group",
			 destination_chunk_group );

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
		/* Test libewf_chunk_group_clone with memcpy failing
		 */
		ewf_test_memcpy_attempts_before_fail = test_number;

		result = libewf_chunk_group_clone(
		          &destination_chunk_group,
		          source_chunk_group,
		          &error );

		if( ewf_test_memcpy_attempts_before_fail != -1 )
		{
			ewf_test_memcpy_attempts_before_fail = -1;

			if( destination_chunk_group != NULL )
			{
				libewf_chunk_group_free(
				 &destination_chunk_group,
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
			 "destination_chunk_group",
			 destination_chunk_group );

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
	result = libewf_chunk_group_free(
	          &source_chunk_group,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "source_chunk_group",
	 source_chunk_group );

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
	if( destination_chunk_group != NULL )
	{
		libewf_chunk_group_free(
		 &destination_chunk_group,
		 NULL );
	}
	if( source_chunk_group != NULL )
	{
		libewf_chunk_group_free(
		 &source_chunk_group,
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

/* Tests the libewf_chunk_group_fill_v1 function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_chunk_group_fill_v1(
     void )
{
	uint8_t table_entries_data[ 16 ];

	libcerror_error_t *error                   = NULL;
	libewf_chunk_group_t *chunk_group          = NULL;
	libewf_io_handle_t *io_handle              = NULL;
	libewf_section_descriptor_t *table_section = NULL;
	int result                                 = 0;

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

	result = libewf_chunk_group_initialize(
	          &chunk_group,
	          io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "chunk_group",
	 chunk_group );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_section_descriptor_initialize(
	          &table_section,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "table_section",
	 table_section );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libewf_chunk_group_fill_v1(
	          NULL,
	          0,
	          0,
	          0,
	          table_section,
	          0,
	          0,
	          table_entries_data,
	          16,
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

	result = libewf_chunk_group_fill_v1(
	          chunk_group,
	          0,
	          0,
	          0,
	          NULL,
	          0,
	          0,
	          table_entries_data,
	          16,
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

	result = libewf_chunk_group_fill_v1(
	          chunk_group,
	          0,
	          0,
	          0,
	          table_section,
	          -1,
	          0,
	          table_entries_data,
	          16,
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

	result = libewf_chunk_group_fill_v1(
	          chunk_group,
	          0,
	          0,
	          0,
	          table_section,
	          0,
	          0,
	          NULL,
	          16,
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

	result = libewf_chunk_group_fill_v1(
	          chunk_group,
	          0,
	          0,
	          0,
	          table_section,
	          0,
	          0,
	          table_entries_data,
	          (size_t) SSIZE_MAX + 1,
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

	/* Clean up
	 */
	result = libewf_section_descriptor_free(
	          &table_section,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "table_section",
	 table_section );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_chunk_group_free(
	          &chunk_group,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "chunk_group",
	 chunk_group );

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
	if( table_section != NULL )
	{
		libewf_section_descriptor_free(
		 &table_section,
		 NULL );
	}
	if( chunk_group != NULL )
	{
		libewf_chunk_group_free(
		 &chunk_group,
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

/* Tests the libewf_chunk_group_fill_v2 function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_chunk_group_fill_v2(
     void )
{
	uint8_t table_entries_data[ 16 ];

	libcerror_error_t *error                   = NULL;
	libewf_chunk_group_t *chunk_group          = NULL;
	libewf_io_handle_t *io_handle              = NULL;
	libewf_section_descriptor_t *table_section = NULL;
	int result                                 = 0;

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

	result = libewf_chunk_group_initialize(
	          &chunk_group,
	          io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "chunk_group",
	 chunk_group );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_section_descriptor_initialize(
	          &table_section,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "table_section",
	 table_section );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libewf_chunk_group_fill_v2(
	          NULL,
	          0,
	          0,
	          0,
	          table_section,
	          0,
	          table_entries_data,
	          16,
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

	result = libewf_chunk_group_fill_v2(
	          chunk_group,
	          0,
	          0,
	          0,
	          NULL,
	          0,
	          table_entries_data,
	          16,
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

	result = libewf_chunk_group_fill_v2(
	          chunk_group,
	          0,
	          0,
	          0,
	          table_section,
	          0,
	          NULL,
	          16,
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

	result = libewf_chunk_group_fill_v2(
	          chunk_group,
	          0,
	          0,
	          0,
	          table_section,
	          0,
	          table_entries_data,
	          (size_t) SSIZE_MAX + 1,
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

	/* Clean up
	 */
	result = libewf_section_descriptor_free(
	          &table_section,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "table_section",
	 table_section );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_chunk_group_free(
	          &chunk_group,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "chunk_group",
	 chunk_group );

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
	if( table_section != NULL )
	{
		libewf_section_descriptor_free(
		 &table_section,
		 NULL );
	}
	if( chunk_group != NULL )
	{
		libewf_chunk_group_free(
		 &chunk_group,
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

/* Tests the libewf_chunk_group_correct_v1 function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_chunk_group_correct_v1(
     void )
{
	uint8_t table_entries_data[ 16 ];

	libcerror_error_t *error                   = NULL;
	libewf_chunk_group_t *chunk_group          = NULL;
	libewf_io_handle_t *io_handle              = NULL;
	libewf_section_descriptor_t *table_section = NULL;
	int result                                 = 0;

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

	result = libewf_chunk_group_initialize(
	          &chunk_group,
	          io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "chunk_group",
	 chunk_group );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_section_descriptor_initialize(
	          &table_section,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "table_section",
	 table_section );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libewf_chunk_group_correct_v1(
	          NULL,
	          0,
	          0,
	          0,
	          table_section,
	          0,
	          0,
	          table_entries_data,
	          16,
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

	result = libewf_chunk_group_correct_v1(
	          chunk_group,
	          0,
	          0,
	          0,
	          NULL,
	          0,
	          0,
	          table_entries_data,
	          16,
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

	result = libewf_chunk_group_correct_v1(
	          chunk_group,
	          0,
	          0,
	          0,
	          table_section,
	          -1,
	          0,
	          table_entries_data,
	          16,
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

	result = libewf_chunk_group_correct_v1(
	          chunk_group,
	          0,
	          0,
	          0,
	          table_section,
	          0,
	          0,
	          NULL,
	          16,
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

	result = libewf_chunk_group_correct_v1(
	          chunk_group,
	          0,
	          0,
	          0,
	          table_section,
	          0,
	          0,
	          table_entries_data,
	          (size_t) SSIZE_MAX + 1,
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

	/* Clean up
	 */
	result = libewf_section_descriptor_free(
	          &table_section,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "table_section",
	 table_section );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_chunk_group_free(
	          &chunk_group,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "chunk_group",
	 chunk_group );

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
	if( table_section != NULL )
	{
		libewf_section_descriptor_free(
		 &table_section,
		 NULL );
	}
	if( chunk_group != NULL )
	{
		libewf_chunk_group_free(
		 &chunk_group,
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
	 "libewf_chunk_group_initialize",
	 ewf_test_chunk_group_initialize );

	EWF_TEST_RUN(
	 "libewf_chunk_group_free",
	 ewf_test_chunk_group_free );

	EWF_TEST_RUN(
	 "libewf_chunk_group_empty",
	 ewf_test_chunk_group_empty );

	EWF_TEST_RUN(
	 "libewf_chunk_group_clone",
	 ewf_test_chunk_group_clone );

	EWF_TEST_RUN(
	 "libewf_chunk_group_fill_v1",
	 ewf_test_chunk_group_fill_v1 );

	EWF_TEST_RUN(
	 "libewf_chunk_group_fill_v2",
	 ewf_test_chunk_group_fill_v2 );

	EWF_TEST_RUN(
	 "libewf_chunk_group_correct_v1",
	 ewf_test_chunk_group_correct_v1 );

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */
}

