/*
 * Library segment_file type test program
 *
 * Copyright (C) 2006-2017, Joachim Metz <joachim.metz@gmail.com>
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

#include "ewf_test_libcerror.h"
#include "ewf_test_libewf.h"
#include "ewf_test_macros.h"
#include "ewf_test_memory.h"
#include "ewf_test_unused.h"

#include "../libewf/libewf_segment_file.h"

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

/* Tests the libewf_segment_file_initialize function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_segment_file_initialize(
     void )
{
	libcerror_error_t *error            = NULL;
	libewf_io_handle_t *io_handle       = NULL;
	libewf_segment_file_t *segment_file = NULL;
	int result                          = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests     = 1;
	int number_of_memset_fail_tests     = 1;
	int test_number                     = 0;
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

	/* Test error cases
	 */
	result = libewf_segment_file_initialize(
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

	segment_file = (libewf_segment_file_t *) 0x12345678UL;

	result = libewf_segment_file_initialize(
	          &segment_file,
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

	segment_file = NULL;

	result = libewf_segment_file_initialize(
	          &segment_file,
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
		/* Test libewf_segment_file_initialize with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = libewf_segment_file_initialize(
		          &segment_file,
		          io_handle,
		          &error );

		if( ewf_test_malloc_attempts_before_fail != -1 )
		{
			ewf_test_malloc_attempts_before_fail = -1;

			if( segment_file != NULL )
			{
				libewf_segment_file_free(
				 &segment_file,
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
			 "segment_file",
			 segment_file );

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
		/* Test libewf_segment_file_initialize with memset failing
		 */
		ewf_test_memset_attempts_before_fail = test_number;

		result = libewf_segment_file_initialize(
		          &segment_file,
		          io_handle,
		          &error );

		if( ewf_test_memset_attempts_before_fail != -1 )
		{
			ewf_test_memset_attempts_before_fail = -1;

			if( segment_file != NULL )
			{
				libewf_segment_file_free(
				 &segment_file,
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
			 "segment_file",
			 segment_file );

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

/* Tests the libewf_segment_file_free function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_segment_file_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libewf_segment_file_free(
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

/* Tests the libewf_segment_file_clone function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_segment_file_clone(
     void )
{
	libcerror_error_t *error                        = NULL;
	libewf_io_handle_t *io_handle                   = NULL;
	libewf_segment_file_t *destination_segment_file = NULL;
	libewf_segment_file_t *source_segment_file      = NULL;
	int result                                      = 0;

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

	result = libewf_segment_file_initialize(
	          &source_segment_file,
	          io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "source_segment_file",
	 source_segment_file );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_segment_file_clone(
	          &destination_segment_file,
	          source_segment_file,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "destination_segment_file",
	 destination_segment_file );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_segment_file_free(
	          &destination_segment_file,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "destination_segment_file",
	 destination_segment_file );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_segment_file_clone(
	          &destination_segment_file,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "destination_segment_file",
	 destination_segment_file );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_segment_file_clone(
	          NULL,
	          source_segment_file,
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
	result = libewf_segment_file_free(
	          &source_segment_file,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "source_segment_file",
	 source_segment_file );

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
	if( destination_segment_file != NULL )
	{
		libewf_segment_file_free(
		 &destination_segment_file,
		 NULL );
	}
	if( source_segment_file != NULL )
	{
		libewf_segment_file_free(
		 &source_segment_file,
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

/* Tests the libewf_segment_file_get_number_of_sections function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_segment_file_get_number_of_sections(
     void )
{
	libcerror_error_t *error            = NULL;
	libewf_io_handle_t *io_handle       = NULL;
	libewf_segment_file_t *segment_file = NULL;
	int number_of_sections              = 0;
	int number_of_sections_is_set       = 0;
	int result                          = 0;

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

	/* Test regular cases
	 */
	result = libewf_segment_file_get_number_of_sections(
	          segment_file,
	          &number_of_sections,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	number_of_sections_is_set = result;

	/* Test error cases
	 */
	result = libewf_segment_file_get_number_of_sections(
	          NULL,
	          &number_of_sections,
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

	if( number_of_sections_is_set != 0 )
	{
		result = libewf_segment_file_get_number_of_sections(
		          segment_file,
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
	}
	/* Clean up
	 */
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
	 "libewf_segment_file_initialize",
	 ewf_test_segment_file_initialize );

	EWF_TEST_RUN(
	 "libewf_segment_file_free",
	 ewf_test_segment_file_free );

#if defined( TODO )

/* TODO: fix libewf_segment_file_clone */
	EWF_TEST_RUN(
	 "libewf_segment_file_clone",
	 ewf_test_segment_file_clone );

#endif

	EWF_TEST_RUN(
	 "libewf_segment_file_get_number_of_sections",
	 ewf_test_segment_file_get_number_of_sections );

	/* TODO: add tests for libewf_segment_file_get_section_by_index */

	/* TODO: add tests for libewf_segment_file_read_file_header */

	/* TODO: add tests for libewf_segment_file_write_file_header */

	/* TODO: add tests for libewf_segment_file_seek_offset */

	/* TODO: add tests for libewf_segment_file_read_table_section */

	/* TODO: add tests for libewf_segment_file_read_table2_section */

	/* TODO: add tests for libewf_segment_file_read_volume_section */

	/* TODO: add tests for libewf_segment_file_write_device_information_section */

	/* TODO: add tests for libewf_segment_file_write_case_data_section */

	/* TODO: add tests for libewf_segment_file_write_header_section */

	/* TODO: add tests for libewf_segment_file_write_header2_section */

	/* TODO: add tests for libewf_segment_file_write_xheader_section */

	/* TODO: add tests for libewf_segment_file_write_header_sections */

	/* TODO: add tests for libewf_segment_file_write_last_section */

	/* TODO: add tests for libewf_segment_file_write_start */

	/* TODO: add tests for libewf_segment_file_write_chunks_section_start */

	/* TODO: add tests for libewf_segment_file_write_chunks_section_end */

	/* TODO: add tests for libewf_segment_file_write_chunk_data */

	/* TODO: add tests for libewf_segment_file_write_hash_sections */

	/* TODO: add tests for libewf_segment_file_write_close */

	/* TODO: add tests for libewf_segment_file_reopen */

	/* TODO: add tests for libewf_segment_file_write_sections_correction */

	/* TODO: add tests for libewf_segment_file_read_element_data */

	/* TODO: add tests for libewf_segment_file_read_section_element_data */

	/* TODO: add tests for libewf_segment_file_read_chunk_group_element_data */

	/* TODO: add tests for libewf_segment_file_get_chunk_group_by_offset */

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

