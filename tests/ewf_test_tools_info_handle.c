/*
 * Tools info_handle type test program
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

#include "../ewftools/info_handle.h"

/* Tests the info_handle_initialize function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_info_handle_initialize(
     void )
{
	info_handle_t *info_handle      = NULL;
	libcerror_error_t *error        = NULL;
	int result                      = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests = 2;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = info_handle_initialize(
	          &info_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "info_handle",
	 info_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = info_handle_free(
	          &info_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "info_handle",
	 info_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = info_handle_initialize(
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

	info_handle = (info_handle_t *) 0x12345678UL;

	result = info_handle_initialize(
	          &info_handle,
	          &error );

	info_handle = NULL;

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
		/* Test info_handle_initialize with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = info_handle_initialize(
		          &info_handle,
		          &error );

		if( ewf_test_malloc_attempts_before_fail != -1 )
		{
			ewf_test_malloc_attempts_before_fail = -1;

			if( info_handle != NULL )
			{
				info_handle_free(
				 &info_handle,
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
			 "info_handle",
			 info_handle );

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
		/* Test info_handle_initialize with memset failing
		 */
		ewf_test_memset_attempts_before_fail = test_number;

		result = info_handle_initialize(
		          &info_handle,
		          &error );

		if( ewf_test_memset_attempts_before_fail != -1 )
		{
			ewf_test_memset_attempts_before_fail = -1;

			if( info_handle != NULL )
			{
				info_handle_free(
				 &info_handle,
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
			 "info_handle",
			 info_handle );

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
	if( info_handle != NULL )
	{
		info_handle_free(
		 &info_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the info_handle_free function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_info_handle_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = info_handle_free(
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

/* Tests the info_handle_signal_abort function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_info_handle_signal_abort(
     info_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = info_handle_signal_abort(
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
	result = info_handle_signal_abort(
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

/* Tests the info_handle_dfxml_header_fprint function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_info_handle_dfxml_header_fprint(
     void )
{
#if defined( HAVE_FMEMOPEN ) && !defined( WINAPI )
	char string[ 1024 ];

	FILE *stream             = NULL;
#endif

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
#if defined( HAVE_FMEMOPEN ) && !defined( WINAPI )

	stream = fmemopen(
	          string,
	          1024,
	          "w+");

	result = info_handle_dfxml_header_fprint(
	          stream,
	          &error );

	fclose(
	 stream );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#endif /* defined( HAVE_FMEMOPEN ) && !defined( WINAPI ) */

	/* Test error cases
	 */
	result = info_handle_dfxml_header_fprint(
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

/* Tests the info_handle_dfxml_footer_fprint function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_info_handle_dfxml_footer_fprint(
     void )
{
#if defined( HAVE_FMEMOPEN ) && !defined( WINAPI )
	char string[ 1024 ];

	FILE *stream             = NULL;
#endif

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
#if defined( HAVE_FMEMOPEN ) && !defined( WINAPI )

	stream = fmemopen(
	          string,
	          1024,
	          "w+");

	result = info_handle_dfxml_footer_fprint(
	          stream,
	          &error );

	fclose(
	 stream );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#endif /* defined( HAVE_FMEMOPEN ) && !defined( WINAPI ) */

	/* Test error cases
	 */
	result = info_handle_dfxml_footer_fprint(
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

/* Tests the info_handle_dfxml_build_environment_fprint function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_info_handle_dfxml_build_environment_fprint(
     void )
{
#if defined( HAVE_FMEMOPEN ) && !defined( WINAPI )
	char string[ 1024 ];

	FILE *stream             = NULL;
#endif

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
#if defined( HAVE_FMEMOPEN ) && !defined( WINAPI )

	stream = fmemopen(
	          string,
	          1024,
	          "w+");

	result = info_handle_dfxml_build_environment_fprint(
	          stream,
	          &error );

	fclose(
	 stream );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#endif /* defined( HAVE_FMEMOPEN ) && !defined( WINAPI ) */

	/* Test error cases
	 */
	result = info_handle_dfxml_build_environment_fprint(
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

/* Tests the info_handle_dfxml_execution_environment_fprint function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_info_handle_dfxml_execution_environment_fprint(
     void )
{
#if defined( HAVE_FMEMOPEN ) && !defined( WINAPI )
	char string[ 1024 ];

	FILE *stream             = NULL;
#endif

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
#if defined( HAVE_FMEMOPEN ) && !defined( WINAPI )

	stream = fmemopen(
	          string,
	          1024,
	          "w+");

	result = info_handle_dfxml_execution_environment_fprint(
	          stream,
	          &error );

	fclose(
	 stream );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#endif /* defined( HAVE_FMEMOPEN ) && !defined( WINAPI ) */

	/* Test error cases
	 */
	result = info_handle_dfxml_execution_environment_fprint(
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
	info_handle_t *info_handle = NULL;
	libcerror_error_t *error   = NULL;
	int result                 = 0;

	EWF_TEST_UNREFERENCED_PARAMETER( argc )
	EWF_TEST_UNREFERENCED_PARAMETER( argv )

	EWF_TEST_RUN(
	 "info_handle_initialize",
	 ewf_test_tools_info_handle_initialize );

	EWF_TEST_RUN(
	 "info_handle_free",
	 ewf_test_tools_info_handle_free );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )
	/* Initialize info handle for tests
	 */
	result = info_handle_initialize(
	          &info_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "info_handle",
	 info_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	EWF_TEST_RUN_WITH_ARGS(
	 "info_handle_signal_abort",
	 ewf_test_tools_info_handle_signal_abort,
	 info_handle );

	/* TODO add tests for info_handle_set_bodyfile */

	/* TODO add tests for info_handle_set_maximum_number_of_open_handles */

	/* TODO add tests for info_handle_open_input */

	/* TODO add tests for info_handle_close */

	/* TODO add tests for info_handle_set_output_format */

	/* TODO add tests for info_handle_set_date_format */

	/* TODO add tests for info_handle_set_header_codepage */

	/* TODO add tests for info_handle_posix_time_value_fprint */

	/* TODO add tests for info_handle_section_header_fprint */

	/* TODO add tests for info_handle_section_footer_fprint */

	/* TODO add tests for info_handle_section_value_string_fprint */

	/* TODO add tests for info_handle_section_value_32bit_fprint */

	/* TODO add tests for info_handle_section_value_64bit_fprint */

	/* TODO add tests for info_handle_section_value_size_fprint */

	/* TODO add tests for info_handle_section_value_boolean_fprint */

	/* TODO add tests for info_handle_sub_section_header_fprint */

	/* TODO add tests for info_handle_sub_section_footer_fprint */

	/* TODO add tests for info_handle_header_value_fprint */

	/* TODO add tests for info_handle_header_values_fprint */

	/* TODO add tests for info_handle_header_value_password_fprint */

	/* TODO add tests for info_handle_header_value_compression_level_fprint */

	/* TODO add tests for info_handle_header_value_extents_fprint */

	/* TODO add tests for info_handle_media_information_fprint */

	/* TODO add tests for info_handle_hash_value_fprint */

	/* TODO add tests for info_handle_hash_values_fprint */

	/* TODO add tests for info_handle_acquiry_errors_fprint */

	/* TODO add tests for info_handle_sessions_fprint */

	/* TODO add tests for info_handle_tracks_fprint */

	/* TODO add tests for info_handle_access_control_entry_value_fprint */

	/* TODO add tests for info_handle_attribute_value_fprint */

	/* TODO add tests for info_handle_source_value_fprint */

	/* TODO add tests for info_handle_file_entry_value_fprint */

	/* TODO add tests for info_handle_logical_files_hierarchy_fprint_file_entry */

	/* TODO add tests for info_handle_file_entry_fprint_by_path */

	/* TODO add tests for info_handle_logical_files_hierarchy_fprint */

	/* TODO add tests for info_handle_file_entry_fprint */

	/* TODO add tests for info_handle_image_fprint */

	EWF_TEST_RUN(
	 "info_handle_dfxml_header_fprint",
	 ewf_test_tools_info_handle_dfxml_header_fprint );

	EWF_TEST_RUN(
	 "info_handle_dfxml_footer_fprint",
	 ewf_test_tools_info_handle_dfxml_footer_fprint );

	EWF_TEST_RUN(
	 "info_handle_dfxml_build_environment_fprint",
	 ewf_test_tools_info_handle_dfxml_build_environment_fprint );

	EWF_TEST_RUN(
	 "info_handle_dfxml_execution_environment_fprint",
	 ewf_test_tools_info_handle_dfxml_execution_environment_fprint );

	/* Clean up
	 */
	result = info_handle_free(
	          &info_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "info_handle",
	 info_handle );

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
	if( info_handle != NULL )
	{
		info_handle_free(
		 &info_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

