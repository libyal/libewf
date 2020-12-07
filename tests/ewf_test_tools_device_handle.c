/*
 * Tools device_handle type test program
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

#include "../ewftools/device_handle.h"
#include "../ewftools/ewftools_libodraw.h"
#include "../ewftools/ewftools_libsmdev.h"
#include "../ewftools/ewftools_libsmraw.h"

/* Tests the device_handle_get_track_type function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_device_handle_get_track_type(
     void )
{
	const char *description = NULL;
	int result              = 0;

	/* Test regular cases
	 */
	description = device_handle_get_track_type(
	               DEVICE_HANDLE_TRACK_TYPE_AUDIO );

	result = memory_compare(
	          description,
	          "audio",
	          6 );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	description = device_handle_get_track_type(
	               DEVICE_HANDLE_TRACK_TYPE_CDG );

	result = memory_compare(
	          description,
	          "CD+G",
	          5 );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	description = device_handle_get_track_type(
	               DEVICE_HANDLE_TRACK_TYPE_MODE1_2048 );

	result = memory_compare(
	          description,
	          "mode1/2048",
	          11 );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	description = device_handle_get_track_type(
	               DEVICE_HANDLE_TRACK_TYPE_MODE1_2352 );

	result = memory_compare(
	          description,
	          "mode1/2352",
	          11 );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	description = device_handle_get_track_type(
	               DEVICE_HANDLE_TRACK_TYPE_MODE2_2048 );

	result = memory_compare(
	          description,
	          "mode2/2048",
	          11 );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	description = device_handle_get_track_type(
	               DEVICE_HANDLE_TRACK_TYPE_MODE2_2324 );

	result = memory_compare(
	          description,
	          "mode2/2324",
	          11 );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	description = device_handle_get_track_type(
	               DEVICE_HANDLE_TRACK_TYPE_MODE2_2336 );

	result = memory_compare(
	          description,
	          "mode2/2336",
	          11 );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	description = device_handle_get_track_type(
	               DEVICE_HANDLE_TRACK_TYPE_MODE2_2352 );

	result = memory_compare(
	          description,
	          "mode2/2352",
	          11 );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	description = device_handle_get_track_type(
	               DEVICE_HANDLE_TRACK_TYPE_CDI_2336 );

	result = memory_compare(
	          description,
	          "CDI/2336",
	          9 );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	description = device_handle_get_track_type(
	               DEVICE_HANDLE_TRACK_TYPE_CDI_2352 );

	result = memory_compare(
	          description,
	          "CDI/2352",
	          9 );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	description = device_handle_get_track_type(
	               0xff );

	result = memory_compare(
	          description,
	          "UNKNOWN",
	          8 );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	return( 1 );

on_error:
	return( 0 );
}

/* Tests the device_handle_initialize function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_device_handle_initialize(
     void )
{
	device_handle_t *device_handle  = NULL;
	libcerror_error_t *error        = NULL;
	int result                      = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests = 2;
	int number_of_memset_fail_tests = 2;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = device_handle_initialize(
	          &device_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "device_handle",
	 device_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = device_handle_free(
	          &device_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "device_handle",
	 device_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = device_handle_initialize(
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

	device_handle = (device_handle_t *) 0x12345678UL;

	result = device_handle_initialize(
	          &device_handle,
	          &error );

	device_handle = NULL;

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
		/* Test device_handle_initialize with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = device_handle_initialize(
		          &device_handle,
		          &error );

		if( ewf_test_malloc_attempts_before_fail != -1 )
		{
			ewf_test_malloc_attempts_before_fail = -1;

			if( device_handle != NULL )
			{
				device_handle_free(
				 &device_handle,
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
			 "device_handle",
			 device_handle );

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
		/* Test device_handle_initialize with memset failing
		 */
		ewf_test_memset_attempts_before_fail = test_number;

		result = device_handle_initialize(
		          &device_handle,
		          &error );

		if( ewf_test_memset_attempts_before_fail != -1 )
		{
			ewf_test_memset_attempts_before_fail = -1;

			if( device_handle != NULL )
			{
				device_handle_free(
				 &device_handle,
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
			 "device_handle",
			 device_handle );

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
	if( device_handle != NULL )
	{
		device_handle_free(
		 &device_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the device_handle_free function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_device_handle_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = device_handle_free(
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

/* Tests the device_handle_signal_abort function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_device_handle_signal_abort(
     device_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = device_handle_signal_abort(
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
	result = device_handle_signal_abort(
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

/* Tests the device_handle_open_input function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_device_handle_open_input(
     void )
{
	device_handle_t *device_handle = NULL;
	libcerror_error_t *error       = NULL;
	char **filenames               = NULL;
	int result                     = 0;

	/* Initialize test
	 */
	result = device_handle_initialize(
	          &device_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "device_handle",
	 device_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = device_handle_open_input(
	          NULL,
	          filenames,
	          1,
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

	result = device_handle_open_input(
	          device_handle,
	          NULL,
	          1,
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
	result = device_handle_free(
	          &device_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "device_handle",
	 device_handle );

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
	if( device_handle != NULL )
	{
		device_handle_free(
		 &device_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the device_handle_open_smdev_input function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_device_handle_open_smdev_input(
     void )
{
	device_handle_t *device_handle = NULL;
	libcerror_error_t *error       = NULL;
	char **filenames               = NULL;
	int result                     = 0;

	/* Initialize test
	 */
	result = device_handle_initialize(
	          &device_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "device_handle",
	 device_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = device_handle_open_smdev_input(
	          NULL,
	          filenames,
	          1,
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

	device_handle->smdev_input_handle = (libsmdev_handle_t *) 0x12345678UL;

	result = device_handle_open_smdev_input(
	          device_handle,
	          filenames,
	          1,
	          &error );

	device_handle->smdev_input_handle = NULL;

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = device_handle_open_smdev_input(
	          device_handle,
	          NULL,
	          1,
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

	result = device_handle_open_smdev_input(
	          device_handle,
	          filenames,
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
	result = device_handle_free(
	          &device_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "device_handle",
	 device_handle );

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
	if( device_handle != NULL )
	{
		device_handle_free(
		 &device_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the device_handle_open_odraw_input function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_device_handle_open_odraw_input(
     void )
{
	device_handle_t *device_handle = NULL;
	libcerror_error_t *error       = NULL;
	char **filenames               = NULL;
	int result                     = 0;

	/* Initialize test
	 */
	result = device_handle_initialize(
	          &device_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "device_handle",
	 device_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = device_handle_open_odraw_input(
	          NULL,
	          filenames,
	          1,
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

	device_handle->odraw_input_handle = (libodraw_handle_t *) 0x12345678UL;

	result = device_handle_open_odraw_input(
	          device_handle,
	          filenames,
	          1,
	          &error );

	device_handle->odraw_input_handle = NULL;

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = device_handle_open_odraw_input(
	          device_handle,
	          NULL,
	          1,
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

	result = device_handle_open_odraw_input(
	          device_handle,
	          filenames,
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
	result = device_handle_free(
	          &device_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "device_handle",
	 device_handle );

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
	if( device_handle != NULL )
	{
		device_handle_free(
		 &device_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the device_handle_open_smraw_input function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_device_handle_open_smraw_input(
     void )
{
	device_handle_t *device_handle = NULL;
	libcerror_error_t *error       = NULL;
	char **filenames               = NULL;
	int result                     = 0;

	/* Initialize test
	 */
	result = device_handle_initialize(
	          &device_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "device_handle",
	 device_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = device_handle_open_smraw_input(
	          NULL,
	          filenames,
	          1,
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

	device_handle->smraw_input_handle = (libsmraw_handle_t *) 0x12345678UL;

	result = device_handle_open_smraw_input(
	          device_handle,
	          filenames,
	          1,
	          &error );

	device_handle->smraw_input_handle = NULL;

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = device_handle_open_smraw_input(
	          device_handle,
	          NULL,
	          1,
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

	result = device_handle_open_smraw_input(
	          device_handle,
	          filenames,
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
	result = device_handle_free(
	          &device_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "device_handle",
	 device_handle );

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
	if( device_handle != NULL )
	{
		device_handle_free(
		 &device_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the device_handle_close function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_device_handle_close(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = device_handle_close(
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

/* Tests the device_handle_get_type function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_device_handle_get_type(
     device_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	uint8_t type             = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = device_handle_get_type(
	          handle,
	          &type,
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
	result = device_handle_get_type(
	          NULL,
	          &type,
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

	result = device_handle_get_type(
	          handle,
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

/* Tests the device_handle_get_media_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_device_handle_get_media_size(
     device_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	size64_t media_size      = 0;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO fix test
	result = device_handle_get_media_size(
	          handle,
	          &media_size,
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
	result = device_handle_get_media_size(
	          NULL,
	          &media_size,
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

	result = device_handle_get_media_size(
	          handle,
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

/* Tests the device_handle_get_media_type function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_device_handle_get_media_type(
     device_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	uint8_t media_type       = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = device_handle_get_media_type(
	          handle,
	          &media_type,
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
	result = device_handle_get_media_type(
	          NULL,
	          &media_type,
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

	result = device_handle_get_media_type(
	          handle,
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

/* Tests the device_handle_get_bytes_per_sector function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_device_handle_get_bytes_per_sector(
     device_handle_t *handle )
{
	libcerror_error_t *error  = NULL;
	uint32_t bytes_per_sector = 0;
	int result                = 0;

	/* Test regular cases
	 */
/* TODO fix test
	result = device_handle_get_bytes_per_sector(
	          handle,
	          &bytes_per_sector,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );
*/

	/* Test error cases
	 */
	result = device_handle_get_bytes_per_sector(
	          NULL,
	          &bytes_per_sector,
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

	result = device_handle_get_bytes_per_sector(
	          handle,
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

/* Tests the device_handle_get_number_of_sessions function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_device_handle_get_number_of_sessions(
     device_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	int number_of_sessions   = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = device_handle_get_number_of_sessions(
	          handle,
	          &number_of_sessions,
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
	result = device_handle_get_number_of_sessions(
	          NULL,
	          &number_of_sessions,
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

	result = device_handle_get_number_of_sessions(
	          handle,
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

/* Tests the device_handle_get_number_of_tracks function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_device_handle_get_number_of_tracks(
     device_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	int number_of_tracks     = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = device_handle_get_number_of_tracks(
	          handle,
	          &number_of_tracks,
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
	result = device_handle_get_number_of_tracks(
	          NULL,
	          &number_of_tracks,
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

	result = device_handle_get_number_of_tracks(
	          handle,
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

/* Tests the device_handle_get_number_of_read_errors function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_device_handle_get_number_of_read_errors(
     device_handle_t *handle )
{
	libcerror_error_t *error  = NULL;
	int number_of_read_errors = 0;
	int result                = 0;

	/* Test regular cases
	 */
	result = device_handle_get_number_of_read_errors(
	          handle,
	          &number_of_read_errors,
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
	result = device_handle_get_number_of_read_errors(
	          NULL,
	          &number_of_read_errors,
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

	result = device_handle_get_number_of_read_errors(
	          handle,
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
	device_handle_t *device_handle = NULL;
	libcerror_error_t *error       = NULL;
	int result                     = 0;

	EWF_TEST_UNREFERENCED_PARAMETER( argc )
	EWF_TEST_UNREFERENCED_PARAMETER( argv )

	EWF_TEST_RUN(
	 "device_handle_get_track_type",
	 ewf_test_tools_device_handle_get_track_type );

	EWF_TEST_RUN(
	 "device_handle_initialize",
	 ewf_test_tools_device_handle_initialize );

	EWF_TEST_RUN(
	 "device_handle_free",
	 ewf_test_tools_device_handle_free );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )
	/* Initialize device handle for tests
	 */
	result = device_handle_initialize(
	          &device_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "device_handle",
	 device_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	device_handle->type = DEVICE_HANDLE_TYPE_FILE;

	EWF_TEST_RUN_WITH_ARGS(
	 "device_handle_signal_abort",
	 ewf_test_tools_device_handle_signal_abort,
	 device_handle );

	EWF_TEST_RUN(
	 "device_handle_open_input",
	 ewf_test_tools_device_handle_open_input );

	EWF_TEST_RUN(
	 "device_handle_open_smdev_input",
	 ewf_test_tools_device_handle_open_smdev_input );

	EWF_TEST_RUN(
	 "device_handle_open_odraw_input",
	 ewf_test_tools_device_handle_open_odraw_input );

	EWF_TEST_RUN(
	 "device_handle_open_smraw_input",
	 ewf_test_tools_device_handle_open_smraw_input );

	EWF_TEST_RUN(
	 "device_handle_close",
	 ewf_test_tools_device_handle_close );

	/* TODO add tests for device_handle_read_storage_media_buffer */

	/* TODO add tests for device_handle_seek_offset */

	/* TODO add tests for device_handle_prompt_for_string */

	/* TODO add tests for device_handle_prompt_for_number_of_error_retries */

	/* TODO add tests for device_handle_prompt_for_zero_buffer_on_error */

	EWF_TEST_RUN_WITH_ARGS(
	 "device_handle_get_type",
	 ewf_test_tools_device_handle_get_type,
	 device_handle );

	EWF_TEST_RUN_WITH_ARGS(
	 "device_handle_get_media_size",
	 ewf_test_tools_device_handle_get_media_size,
	 device_handle );

	EWF_TEST_RUN_WITH_ARGS(
	 "device_handle_get_media_type",
	 ewf_test_tools_device_handle_get_media_type,
	 device_handle );

	EWF_TEST_RUN_WITH_ARGS(
	 "device_handle_get_bytes_per_sector",
	 ewf_test_tools_device_handle_get_bytes_per_sector,
	 device_handle );

	/* TODO add tests for device_handle_get_information_value */

	EWF_TEST_RUN_WITH_ARGS(
	 "device_handle_get_number_of_sessions",
	 ewf_test_tools_device_handle_get_number_of_sessions,
	 device_handle );

	/* TODO add tests for device_handle_get_session */

	EWF_TEST_RUN_WITH_ARGS(
	 "device_handle_get_number_of_tracks",
	 ewf_test_tools_device_handle_get_number_of_tracks,
	 device_handle );

	/* TODO add tests for device_handle_get_track */

	/* TODO add tests for device_handle_set_string */

	/* TODO add tests for device_handle_set_number_of_error_retries */

	/* TODO add tests for device_handle_set_error_values */

	EWF_TEST_RUN_WITH_ARGS(
	 "device_handle_get_number_of_read_errors",
	 ewf_test_tools_device_handle_get_number_of_read_errors,
	 device_handle );

	/* TODO add tests for device_handle_get_read_error */

	/* TODO add tests for device_handle_media_information_fprint */

	/* TODO add tests for device_handle_read_errors_fprint */

	/* TODO add tests for device_handle_sessions_fprint */

	/* TODO add tests for device_handle_tracks_fprint */

	/* Clean up
	 */
	result = device_handle_free(
	          &device_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "device_handle",
	 device_handle );

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
	if( device_handle != NULL )
	{
		device_handle_free(
		 &device_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

