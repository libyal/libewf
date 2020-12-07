/*
 * Tools imaging_handle type test program
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
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "ewf_test_libcerror.h"
#include "ewf_test_libewf.h"
#include "ewf_test_macros.h"
#include "ewf_test_memory.h"
#include "ewf_test_unused.h"

#include "../ewftools/imaging_handle.h"

/* Tests the imaging_handle_initialize function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_imaging_handle_initialize(
     void )
{
	imaging_handle_t *imaging_handle = NULL;
	libcerror_error_t *error         = NULL;
	int result                       = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests  = 3;
	int number_of_memset_fail_tests  = 2;
	int test_number                  = 0;
#endif

	/* Test regular cases
	 */
	result = imaging_handle_initialize(
	          &imaging_handle,
	          1,
	          0,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "imaging_handle",
	 imaging_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = imaging_handle_free(
	          &imaging_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "imaging_handle",
	 imaging_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = imaging_handle_initialize(
	          NULL,
	          1,
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

	imaging_handle = (imaging_handle_t *) 0x12345678UL;

	result = imaging_handle_initialize(
	          &imaging_handle,
	          1,
	          0,
	          &error );

	imaging_handle = NULL;

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
		/* Test imaging_handle_initialize with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = imaging_handle_initialize(
		          &imaging_handle,
		          1,
		          0,
		          &error );

		if( ewf_test_malloc_attempts_before_fail != -1 )
		{
			ewf_test_malloc_attempts_before_fail = -1;

			if( imaging_handle != NULL )
			{
				imaging_handle_free(
				 &imaging_handle,
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
			 "imaging_handle",
			 imaging_handle );

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
		/* Test imaging_handle_initialize with memset failing
		 */
		ewf_test_memset_attempts_before_fail = test_number;

		result = imaging_handle_initialize(
		          &imaging_handle,
		          1,
		          0,
		          &error );

		if( ewf_test_memset_attempts_before_fail != -1 )
		{
			ewf_test_memset_attempts_before_fail = -1;

			if( imaging_handle != NULL )
			{
				imaging_handle_free(
				 &imaging_handle,
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
			 "imaging_handle",
			 imaging_handle );

			EWF_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_EWF_TEST_MEMORY ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( imaging_handle != NULL )
	{
		imaging_handle_free(
		 &imaging_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the imaging_handle_free function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_imaging_handle_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = imaging_handle_free(
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

/* Tests the imaging_handle_signal_abort function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_imaging_handle_signal_abort(
     imaging_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = imaging_handle_signal_abort(
	          handle,
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
	result = imaging_handle_signal_abort(
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
	libcerror_error_t *error                   = NULL;
	imaging_handle_t *imaging_handle = NULL;
	int result                                 = 0;

	EWF_TEST_UNREFERENCED_PARAMETER( argc )
	EWF_TEST_UNREFERENCED_PARAMETER( argv )

	EWF_TEST_RUN(
	 "imaging_handle_initialize",
	 ewf_test_tools_imaging_handle_initialize );

	EWF_TEST_RUN(
	 "imaging_handle_free",
	 ewf_test_tools_imaging_handle_free );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )
	/* Initialize info handle for tests
	 */
	result = imaging_handle_initialize(
	          &imaging_handle,
	          1,
	          0,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "imaging_handle",
	 imaging_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	EWF_TEST_RUN_WITH_ARGS(
	 "imaging_handle_signal_abort",
	 ewf_test_tools_imaging_handle_signal_abort,
	 imaging_handle );

	/* TODO add tests for imaging_handle_check_write_access */

	/* TODO add tests for imaging_handle_open_output */

	/* TODO add tests for imaging_handle_open_secondary_output */

	/* TODO add tests for imaging_handle_open_output_resume */

	/* TODO add tests for imaging_handle_close */

	/* TODO add tests for imaging_handle_write_storage_media_buffer */

	/* TODO add tests for imaging_handle_seek_offset */

	/* TODO add tests for imaging_handle_get_offset */

	/* TODO add tests for imaging_handle_swap_byte_pairs */

	/* TODO add tests for imaging_handle_initialize_integrity_hash */

	/* TODO add tests for imaging_handle_update_integrity_hash */

	/* TODO add tests for imaging_handle_finalize_integrity_hash */

#if defined( HAVE_MULTI_THREAD_SUPPORT )

	/* TODO add tests for imaging_handle_threads_start */

	/* TODO add tests for imaging_handle_threads_stop */

	/* TODO add tests for imaging_handle_process_storage_media_buffer_callback */

	/* TODO add tests for imaging_handle_output_storage_media_buffer_callback */

	/* TODO add tests for imaging_handle_empty_output_list */

#endif /* defined( HAVE_MULTI_THREAD_SUPPORT ) */

	/* TODO add tests for imaging_handle_get_process_buffer_size */

	/* TODO add tests for imaging_handle_prompt_for_string */

	/* TODO add tests for imaging_handle_prompt_for_compression_method */

	/* TODO add tests for imaging_handle_prompt_for_compression_level */

	/* TODO add tests for imaging_handle_prompt_for_format */

	/* TODO add tests for imaging_handle_prompt_for_media_type */

	/* TODO add tests for imaging_handle_prompt_for_media_flags */

	/* TODO add tests for imaging_handle_prompt_for_bytes_per_sector */

	/* TODO add tests for imaging_handle_prompt_for_sectors_per_chunk */

	/* TODO add tests for imaging_handle_prompt_for_sector_error_granularity */

	/* TODO add tests for imaging_handle_prompt_for_maximum_segment_size */

	/* TODO add tests for imaging_handle_prompt_for_acquiry_offset */

	/* TODO add tests for imaging_handle_prompt_for_acquiry_size */

	/* TODO add tests for imaging_handle_get_output_values */

	/* TODO add tests for imaging_handle_set_string */

	/* TODO add tests for imaging_handle_set_compression_values */

	/* TODO add tests for imaging_handle_set_format */

	/* TODO add tests for imaging_handle_set_media_type */

	/* TODO add tests for imaging_handle_set_media_flags */

	/* TODO add tests for imaging_handle_set_bytes_per_sector */

	/* TODO add tests for imaging_handle_set_sectors_per_chunk */

	/* TODO add tests for imaging_handle_set_sector_error_granularity */

	/* TODO add tests for imaging_handle_set_maximum_segment_size */

	/* TODO add tests for imaging_handle_set_acquiry_offset */

	/* TODO add tests for imaging_handle_set_acquiry_size */

	/* TODO add tests for imaging_handle_set_header_codepage */

	/* TODO add tests for imaging_handle_set_process_buffer_size */

	/* TODO add tests for imaging_handle_set_number_of_threads */

	/* TODO add tests for imaging_handle_set_additional_digest_types */

	/* TODO add tests for imaging_handle_set_output_values */

	/* TODO add tests for imaging_handle_get_header_value */

	/* TODO add tests for imaging_handle_set_header_value */

	/* TODO add tests for imaging_handle_set_hash_value */

	/* TODO add tests for imaging_handle_append_read_error */

	/* TODO add tests for imaging_handle_append_session */

	/* TODO add tests for imaging_handle_append_track */

	/* TODO add tests for imaging_handle_start */

	/* TODO add tests for imaging_handle_update */

	/* TODO add tests for imaging_handle_stop */

	/* TODO add tests for imaging_handle_print_parameters */

	/* TODO add tests for imaging_handle_print_hashes */

	/* Clean up
	 */
	result = imaging_handle_free(
	          &imaging_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "imaging_handle",
	 imaging_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( imaging_handle != NULL )
	{
		imaging_handle_free(
		 &imaging_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

