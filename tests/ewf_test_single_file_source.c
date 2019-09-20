/*
 * Library single_file_source type test program
 *
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

#include "../libewf/libewf_single_file_source.h"

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

/* Tests the libewf_single_file_source_initialize function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_source_initialize(
     void )
{
	libcerror_error_t *error                        = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	int result                                      = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests                 = 1;
	int number_of_memset_fail_tests                 = 1;
	int test_number                                 = 0;
#endif

	/* Test regular cases
	 */
	result = libewf_single_file_source_initialize(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_source",
	 single_file_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_single_file_source_free(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_source",
	 single_file_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_single_file_source_initialize(
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

	single_file_source = (libewf_single_file_source_t *) 0x12345678UL;

	result = libewf_single_file_source_initialize(
	          &single_file_source,
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

	single_file_source = NULL;

#if defined( HAVE_EWF_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libewf_single_file_source_initialize with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = libewf_single_file_source_initialize(
		          &single_file_source,
		          &error );

		if( ewf_test_malloc_attempts_before_fail != -1 )
		{
			ewf_test_malloc_attempts_before_fail = -1;

			if( single_file_source != NULL )
			{
				libewf_single_file_source_free(
				 &single_file_source,
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
			 "single_file_source",
			 single_file_source );

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
		/* Test libewf_single_file_source_initialize with memset failing
		 */
		ewf_test_memset_attempts_before_fail = test_number;

		result = libewf_single_file_source_initialize(
		          &single_file_source,
		          &error );

		if( ewf_test_memset_attempts_before_fail != -1 )
		{
			ewf_test_memset_attempts_before_fail = -1;

			if( single_file_source != NULL )
			{
				libewf_single_file_source_free(
				 &single_file_source,
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
			 "single_file_source",
			 single_file_source );

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
	if( single_file_source != NULL )
	{
		libewf_single_file_source_free(
		 &single_file_source,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_source_free function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_source_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libewf_single_file_source_free(
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

/* Tests the libewf_single_file_source_clone function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_source_clone(
     void )
{
	libcerror_error_t *error                                    = NULL;
	libewf_single_file_source_t *destination_single_file_source = NULL;
	libewf_single_file_source_t *source_single_file_source      = NULL;
	int result                                                  = 0;

	/* Initialize test
	 */
	result = libewf_single_file_source_initialize(
	          &source_single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "source_single_file_source",
	 source_single_file_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_source_clone(
	          &destination_single_file_source,
	          source_single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "destination_single_file_source",
	 destination_single_file_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_single_file_source_free(
	          &destination_single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "destination_single_file_source",
	 destination_single_file_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_single_file_source_clone(
	          &destination_single_file_source,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "destination_single_file_source",
	 destination_single_file_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_single_file_source_clone(
	          NULL,
	          source_single_file_source,
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
	result = libewf_single_file_source_free(
	          &source_single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "source_single_file_source",
	 source_single_file_source );

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
	if( destination_single_file_source != NULL )
	{
		libewf_single_file_source_free(
		 &destination_single_file_source,
		 NULL );
	}
	if( source_single_file_source != NULL )
	{
		libewf_single_file_source_free(
		 &source_single_file_source,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_source_get_id function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_source_get_id(
     void )
{
	libcerror_error_t *error                        = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	uint32_t id                                     = 0;
	int id_is_set                                   = 0;
	int result                                      = 0;

	/* Initialize test
	 */
	result = libewf_single_file_source_initialize(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_source",
	 single_file_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_source_get_id(
	          single_file_source,
	          &id,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	id_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_source_get_id(
	          NULL,
	          &id,
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

	if( id_is_set != 0 )
	{
		result = libewf_single_file_source_get_id(
		          single_file_source,
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
	result = libewf_single_file_source_free(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_source",
	 single_file_source );

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
	if( single_file_source != NULL )
	{
		libewf_single_file_source_free(
		 &single_file_source,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_source_get_type function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_source_get_type(
     void )
{
	libcerror_error_t *error                        = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	uint8_t type                                    = 0;
	int result                                      = 0;
	int type_is_set                                 = 0;

	/* Initialize test
	 */
	result = libewf_single_file_source_initialize(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_source",
	 single_file_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_source_get_type(
	          single_file_source,
	          &type,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	type_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_source_get_type(
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

	if( type_is_set != 0 )
	{
		result = libewf_single_file_source_get_type(
		          single_file_source,
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
	result = libewf_single_file_source_free(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_source",
	 single_file_source );

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
	if( single_file_source != NULL )
	{
		libewf_single_file_source_free(
		 &single_file_source,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_source_get_drive_type function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_source_get_drive_type(
     void )
{
	libcerror_error_t *error                        = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	uint8_t drive_type                              = 0;
	int result                                      = 0;
	int drive_type_is_set                           = 0;

	/* Initialize test
	 */
	result = libewf_single_file_source_initialize(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_source",
	 single_file_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_source_get_drive_type(
	          single_file_source,
	          &drive_type,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	drive_type_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_source_get_drive_type(
	          NULL,
	          &drive_type,
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

	if( drive_type_is_set != 0 )
	{
		result = libewf_single_file_source_get_drive_type(
		          single_file_source,
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
	result = libewf_single_file_source_free(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_source",
	 single_file_source );

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
	if( single_file_source != NULL )
	{
		libewf_single_file_source_free(
		 &single_file_source,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_source_get_physical_offset function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_source_get_physical_offset(
     void )
{
	libcerror_error_t *error                        = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	off64_t physical_offset                         = 0;
	int physical_offset_is_set                      = 0;
	int result                                      = 0;

	/* Initialize test
	 */
	result = libewf_single_file_source_initialize(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_source",
	 single_file_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_source_get_physical_offset(
	          single_file_source,
	          &physical_offset,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	physical_offset_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_source_get_physical_offset(
	          NULL,
	          &physical_offset,
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

	if( physical_offset_is_set != 0 )
	{
		result = libewf_single_file_source_get_physical_offset(
		          single_file_source,
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
	result = libewf_single_file_source_free(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_source",
	 single_file_source );

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
	if( single_file_source != NULL )
	{
		libewf_single_file_source_free(
		 &single_file_source,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_source_get_logical_offset function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_source_get_logical_offset(
     void )
{
	libcerror_error_t *error                        = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	off64_t logical_offset                          = 0;
	int logical_offset_is_set                       = 0;
	int result                                      = 0;

	/* Initialize test
	 */
	result = libewf_single_file_source_initialize(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_source",
	 single_file_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_source_get_logical_offset(
	          single_file_source,
	          &logical_offset,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	logical_offset_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_source_get_logical_offset(
	          NULL,
	          &logical_offset,
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

	if( logical_offset_is_set != 0 )
	{
		result = libewf_single_file_source_get_logical_offset(
		          single_file_source,
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
	result = libewf_single_file_source_free(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_source",
	 single_file_source );

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
	if( single_file_source != NULL )
	{
		libewf_single_file_source_free(
		 &single_file_source,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_source_get_total_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_source_get_total_size(
     void )
{
	libcerror_error_t *error                        = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	size64_t total_size                             = 0;
	int total_size_is_set                           = 0;
	int result                                      = 0;

	/* Initialize test
	 */
	result = libewf_single_file_source_initialize(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_source",
	 single_file_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_source_get_total_size(
	          single_file_source,
	          &total_size,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	total_size_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_source_get_total_size(
	          NULL,
	          &total_size,
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

	if( total_size_is_set != 0 )
	{
		result = libewf_single_file_source_get_total_size(
		          single_file_source,
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
	result = libewf_single_file_source_free(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_source",
	 single_file_source );

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
	if( single_file_source != NULL )
	{
		libewf_single_file_source_free(
		 &single_file_source,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_source_get_utf8_name_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_source_get_utf8_name_size(
     void )
{
	libcerror_error_t *error                        = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	size_t utf8_name_size                           = 0;
	int result                                      = 0;
	int utf8_name_size_is_set                       = 0;

	/* Initialize test
	 */
	result = libewf_single_file_source_initialize(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_source",
	 single_file_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_source_get_utf8_name_size(
	          single_file_source,
	          &utf8_name_size,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_name_size_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_source_get_utf8_name_size(
	          NULL,
	          &utf8_name_size,
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

	if( utf8_name_size_is_set != 0 )
	{
		result = libewf_single_file_source_get_utf8_name_size(
		          single_file_source,
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
	result = libewf_single_file_source_free(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_source",
	 single_file_source );

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
	if( single_file_source != NULL )
	{
		libewf_single_file_source_free(
		 &single_file_source,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_source_get_utf8_name function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_source_get_utf8_name(
     void )
{
	uint8_t utf8_name[ 512 ];

	libcerror_error_t *error                        = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	int result                                      = 0;
	int utf8_name_is_set                            = 0;

	/* Initialize test
	 */
	result = libewf_single_file_source_initialize(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_source",
	 single_file_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_source_get_utf8_name(
	          single_file_source,
	          utf8_name,
	          512,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_name_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_source_get_utf8_name(
	          NULL,
	          utf8_name,
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

	if( utf8_name_is_set != 0 )
	{
		result = libewf_single_file_source_get_utf8_name(
		          single_file_source,
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

		result = libewf_single_file_source_get_utf8_name(
		          single_file_source,
		          utf8_name,
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

		result = libewf_single_file_source_get_utf8_name(
		          single_file_source,
		          utf8_name,
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
	}
	/* Clean up
	 */
	result = libewf_single_file_source_free(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_source",
	 single_file_source );

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
	if( single_file_source != NULL )
	{
		libewf_single_file_source_free(
		 &single_file_source,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_source_get_utf16_name_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_source_get_utf16_name_size(
     void )
{
	libcerror_error_t *error                        = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	size_t utf16_name_size                          = 0;
	int result                                      = 0;
	int utf16_name_size_is_set                      = 0;

	/* Initialize test
	 */
	result = libewf_single_file_source_initialize(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_source",
	 single_file_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_source_get_utf16_name_size(
	          single_file_source,
	          &utf16_name_size,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_name_size_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_source_get_utf16_name_size(
	          NULL,
	          &utf16_name_size,
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

	if( utf16_name_size_is_set != 0 )
	{
		result = libewf_single_file_source_get_utf16_name_size(
		          single_file_source,
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
	result = libewf_single_file_source_free(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_source",
	 single_file_source );

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
	if( single_file_source != NULL )
	{
		libewf_single_file_source_free(
		 &single_file_source,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_source_get_utf16_name function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_source_get_utf16_name(
     void )
{
	uint16_t utf16_name[ 512 ];

	libcerror_error_t *error                        = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	int result                                      = 0;
	int utf16_name_is_set                           = 0;

	/* Initialize test
	 */
	result = libewf_single_file_source_initialize(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_source",
	 single_file_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_source_get_utf16_name(
	          single_file_source,
	          utf16_name,
	          512,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_name_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_source_get_utf16_name(
	          NULL,
	          utf16_name,
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

	if( utf16_name_is_set != 0 )
	{
		result = libewf_single_file_source_get_utf16_name(
		          single_file_source,
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

		result = libewf_single_file_source_get_utf16_name(
		          single_file_source,
		          utf16_name,
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

		result = libewf_single_file_source_get_utf16_name(
		          single_file_source,
		          utf16_name,
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
	}
	/* Clean up
	 */
	result = libewf_single_file_source_free(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_source",
	 single_file_source );

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
	if( single_file_source != NULL )
	{
		libewf_single_file_source_free(
		 &single_file_source,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_source_get_utf8_serial_number_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_source_get_utf8_serial_number_size(
     void )
{
	libcerror_error_t *error                        = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	size_t utf8_serial_number_size                           = 0;
	int result                                      = 0;
	int utf8_serial_number_size_is_set                       = 0;

	/* Initialize test
	 */
	result = libewf_single_file_source_initialize(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_source",
	 single_file_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_source_get_utf8_serial_number_size(
	          single_file_source,
	          &utf8_serial_number_size,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_serial_number_size_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_source_get_utf8_serial_number_size(
	          NULL,
	          &utf8_serial_number_size,
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

	if( utf8_serial_number_size_is_set != 0 )
	{
		result = libewf_single_file_source_get_utf8_serial_number_size(
		          single_file_source,
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
	result = libewf_single_file_source_free(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_source",
	 single_file_source );

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
	if( single_file_source != NULL )
	{
		libewf_single_file_source_free(
		 &single_file_source,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_source_get_utf8_serial_number function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_source_get_utf8_serial_number(
     void )
{
	uint8_t utf8_serial_number[ 512 ];

	libcerror_error_t *error                        = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	int result                                      = 0;
	int utf8_serial_number_is_set                            = 0;

	/* Initialize test
	 */
	result = libewf_single_file_source_initialize(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_source",
	 single_file_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_source_get_utf8_serial_number(
	          single_file_source,
	          utf8_serial_number,
	          512,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_serial_number_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_source_get_utf8_serial_number(
	          NULL,
	          utf8_serial_number,
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

	if( utf8_serial_number_is_set != 0 )
	{
		result = libewf_single_file_source_get_utf8_serial_number(
		          single_file_source,
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

		result = libewf_single_file_source_get_utf8_serial_number(
		          single_file_source,
		          utf8_serial_number,
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

		result = libewf_single_file_source_get_utf8_serial_number(
		          single_file_source,
		          utf8_serial_number,
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
	}
	/* Clean up
	 */
	result = libewf_single_file_source_free(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_source",
	 single_file_source );

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
	if( single_file_source != NULL )
	{
		libewf_single_file_source_free(
		 &single_file_source,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_source_get_utf16_serial_number_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_source_get_utf16_serial_number_size(
     void )
{
	libcerror_error_t *error                        = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	size_t utf16_serial_number_size                          = 0;
	int result                                      = 0;
	int utf16_serial_number_size_is_set                      = 0;

	/* Initialize test
	 */
	result = libewf_single_file_source_initialize(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_source",
	 single_file_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_source_get_utf16_serial_number_size(
	          single_file_source,
	          &utf16_serial_number_size,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_serial_number_size_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_source_get_utf16_serial_number_size(
	          NULL,
	          &utf16_serial_number_size,
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

	if( utf16_serial_number_size_is_set != 0 )
	{
		result = libewf_single_file_source_get_utf16_serial_number_size(
		          single_file_source,
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
	result = libewf_single_file_source_free(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_source",
	 single_file_source );

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
	if( single_file_source != NULL )
	{
		libewf_single_file_source_free(
		 &single_file_source,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_source_get_utf16_serial_number function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_source_get_utf16_serial_number(
     void )
{
	uint16_t utf16_serial_number[ 512 ];

	libcerror_error_t *error                        = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	int result                                      = 0;
	int utf16_serial_number_is_set                           = 0;

	/* Initialize test
	 */
	result = libewf_single_file_source_initialize(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_source",
	 single_file_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_source_get_utf16_serial_number(
	          single_file_source,
	          utf16_serial_number,
	          512,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_serial_number_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_source_get_utf16_serial_number(
	          NULL,
	          utf16_serial_number,
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

	if( utf16_serial_number_is_set != 0 )
	{
		result = libewf_single_file_source_get_utf16_serial_number(
		          single_file_source,
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

		result = libewf_single_file_source_get_utf16_serial_number(
		          single_file_source,
		          utf16_serial_number,
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

		result = libewf_single_file_source_get_utf16_serial_number(
		          single_file_source,
		          utf16_serial_number,
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
	}
	/* Clean up
	 */
	result = libewf_single_file_source_free(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_source",
	 single_file_source );

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
	if( single_file_source != NULL )
	{
		libewf_single_file_source_free(
		 &single_file_source,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_source_get_utf8_manufacturer_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_source_get_utf8_manufacturer_size(
     void )
{
	libcerror_error_t *error                        = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	size_t utf8_manufacturer_size                           = 0;
	int result                                      = 0;
	int utf8_manufacturer_size_is_set                       = 0;

	/* Initialize test
	 */
	result = libewf_single_file_source_initialize(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_source",
	 single_file_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_source_get_utf8_manufacturer_size(
	          single_file_source,
	          &utf8_manufacturer_size,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_manufacturer_size_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_source_get_utf8_manufacturer_size(
	          NULL,
	          &utf8_manufacturer_size,
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

	if( utf8_manufacturer_size_is_set != 0 )
	{
		result = libewf_single_file_source_get_utf8_manufacturer_size(
		          single_file_source,
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
	result = libewf_single_file_source_free(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_source",
	 single_file_source );

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
	if( single_file_source != NULL )
	{
		libewf_single_file_source_free(
		 &single_file_source,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_source_get_utf8_manufacturer function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_source_get_utf8_manufacturer(
     void )
{
	uint8_t utf8_manufacturer[ 512 ];

	libcerror_error_t *error                        = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	int result                                      = 0;
	int utf8_manufacturer_is_set                            = 0;

	/* Initialize test
	 */
	result = libewf_single_file_source_initialize(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_source",
	 single_file_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_source_get_utf8_manufacturer(
	          single_file_source,
	          utf8_manufacturer,
	          512,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_manufacturer_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_source_get_utf8_manufacturer(
	          NULL,
	          utf8_manufacturer,
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

	if( utf8_manufacturer_is_set != 0 )
	{
		result = libewf_single_file_source_get_utf8_manufacturer(
		          single_file_source,
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

		result = libewf_single_file_source_get_utf8_manufacturer(
		          single_file_source,
		          utf8_manufacturer,
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

		result = libewf_single_file_source_get_utf8_manufacturer(
		          single_file_source,
		          utf8_manufacturer,
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
	}
	/* Clean up
	 */
	result = libewf_single_file_source_free(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_source",
	 single_file_source );

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
	if( single_file_source != NULL )
	{
		libewf_single_file_source_free(
		 &single_file_source,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_source_get_utf16_manufacturer_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_source_get_utf16_manufacturer_size(
     void )
{
	libcerror_error_t *error                        = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	size_t utf16_manufacturer_size                          = 0;
	int result                                      = 0;
	int utf16_manufacturer_size_is_set                      = 0;

	/* Initialize test
	 */
	result = libewf_single_file_source_initialize(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_source",
	 single_file_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_source_get_utf16_manufacturer_size(
	          single_file_source,
	          &utf16_manufacturer_size,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_manufacturer_size_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_source_get_utf16_manufacturer_size(
	          NULL,
	          &utf16_manufacturer_size,
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

	if( utf16_manufacturer_size_is_set != 0 )
	{
		result = libewf_single_file_source_get_utf16_manufacturer_size(
		          single_file_source,
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
	result = libewf_single_file_source_free(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_source",
	 single_file_source );

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
	if( single_file_source != NULL )
	{
		libewf_single_file_source_free(
		 &single_file_source,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_source_get_utf16_manufacturer function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_source_get_utf16_manufacturer(
     void )
{
	uint16_t utf16_manufacturer[ 512 ];

	libcerror_error_t *error                        = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	int result                                      = 0;
	int utf16_manufacturer_is_set                           = 0;

	/* Initialize test
	 */
	result = libewf_single_file_source_initialize(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_source",
	 single_file_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_source_get_utf16_manufacturer(
	          single_file_source,
	          utf16_manufacturer,
	          512,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_manufacturer_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_source_get_utf16_manufacturer(
	          NULL,
	          utf16_manufacturer,
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

	if( utf16_manufacturer_is_set != 0 )
	{
		result = libewf_single_file_source_get_utf16_manufacturer(
		          single_file_source,
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

		result = libewf_single_file_source_get_utf16_manufacturer(
		          single_file_source,
		          utf16_manufacturer,
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

		result = libewf_single_file_source_get_utf16_manufacturer(
		          single_file_source,
		          utf16_manufacturer,
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
	}
	/* Clean up
	 */
	result = libewf_single_file_source_free(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_source",
	 single_file_source );

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
	if( single_file_source != NULL )
	{
		libewf_single_file_source_free(
		 &single_file_source,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_source_get_utf8_model_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_source_get_utf8_model_size(
     void )
{
	libcerror_error_t *error                        = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	size_t utf8_model_size                          = 0;
	int result                                      = 0;
	int utf8_model_size_is_set                      = 0;

	/* Initialize test
	 */
	result = libewf_single_file_source_initialize(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_source",
	 single_file_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_source_get_utf8_model_size(
	          single_file_source,
	          &utf8_model_size,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_model_size_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_source_get_utf8_model_size(
	          NULL,
	          &utf8_model_size,
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

	if( utf8_model_size_is_set != 0 )
	{
		result = libewf_single_file_source_get_utf8_model_size(
		          single_file_source,
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
	result = libewf_single_file_source_free(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_source",
	 single_file_source );

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
	if( single_file_source != NULL )
	{
		libewf_single_file_source_free(
		 &single_file_source,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_source_get_utf8_model function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_source_get_utf8_model(
     void )
{
	uint8_t utf8_model[ 512 ];

	libcerror_error_t *error                        = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	int result                                      = 0;
	int utf8_model_is_set                           = 0;

	/* Initialize test
	 */
	result = libewf_single_file_source_initialize(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_source",
	 single_file_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_source_get_utf8_model(
	          single_file_source,
	          utf8_model,
	          512,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_model_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_source_get_utf8_model(
	          NULL,
	          utf8_model,
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

	if( utf8_model_is_set != 0 )
	{
		result = libewf_single_file_source_get_utf8_model(
		          single_file_source,
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

		result = libewf_single_file_source_get_utf8_model(
		          single_file_source,
		          utf8_model,
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

		result = libewf_single_file_source_get_utf8_model(
		          single_file_source,
		          utf8_model,
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
	}
	/* Clean up
	 */
	result = libewf_single_file_source_free(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_source",
	 single_file_source );

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
	if( single_file_source != NULL )
	{
		libewf_single_file_source_free(
		 &single_file_source,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_source_get_utf16_model_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_source_get_utf16_model_size(
     void )
{
	libcerror_error_t *error                        = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	size_t utf16_model_size                         = 0;
	int result                                      = 0;
	int utf16_model_size_is_set                     = 0;

	/* Initialize test
	 */
	result = libewf_single_file_source_initialize(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_source",
	 single_file_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_source_get_utf16_model_size(
	          single_file_source,
	          &utf16_model_size,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_model_size_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_source_get_utf16_model_size(
	          NULL,
	          &utf16_model_size,
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

	if( utf16_model_size_is_set != 0 )
	{
		result = libewf_single_file_source_get_utf16_model_size(
		          single_file_source,
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
	result = libewf_single_file_source_free(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_source",
	 single_file_source );

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
	if( single_file_source != NULL )
	{
		libewf_single_file_source_free(
		 &single_file_source,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_source_get_utf16_model function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_source_get_utf16_model(
     void )
{
	uint16_t utf16_model[ 512 ];

	libcerror_error_t *error                        = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	int result                                      = 0;
	int utf16_model_is_set                          = 0;

	/* Initialize test
	 */
	result = libewf_single_file_source_initialize(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_source",
	 single_file_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_source_get_utf16_model(
	          single_file_source,
	          utf16_model,
	          512,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_model_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_source_get_utf16_model(
	          NULL,
	          utf16_model,
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

	if( utf16_model_is_set != 0 )
	{
		result = libewf_single_file_source_get_utf16_model(
		          single_file_source,
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

		result = libewf_single_file_source_get_utf16_model(
		          single_file_source,
		          utf16_model,
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

		result = libewf_single_file_source_get_utf16_model(
		          single_file_source,
		          utf16_model,
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
	}
	/* Clean up
	 */
	result = libewf_single_file_source_free(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_source",
	 single_file_source );

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
	if( single_file_source != NULL )
	{
		libewf_single_file_source_free(
		 &single_file_source,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_source_get_utf8_guid function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_source_get_utf8_guid(
     void )
{
	uint8_t utf8_guid[ 512 ];

	libcerror_error_t *error                        = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	int result                                      = 0;
	int utf8_guid_is_set                            = 0;

	/* Initialize test
	 */
	result = libewf_single_file_source_initialize(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_source",
	 single_file_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_source_get_utf8_guid(
	          single_file_source,
	          utf8_guid,
	          512,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_guid_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_source_get_utf8_guid(
	          NULL,
	          utf8_guid,
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

	if( utf8_guid_is_set != 0 )
	{
		result = libewf_single_file_source_get_utf8_guid(
		          single_file_source,
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

		result = libewf_single_file_source_get_utf8_guid(
		          single_file_source,
		          utf8_guid,
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

		result = libewf_single_file_source_get_utf8_guid(
		          single_file_source,
		          utf8_guid,
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
	}
	/* Clean up
	 */
	result = libewf_single_file_source_free(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_source",
	 single_file_source );

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
	if( single_file_source != NULL )
	{
		libewf_single_file_source_free(
		 &single_file_source,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_source_get_utf16_guid function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_source_get_utf16_guid(
     void )
{
	uint16_t utf16_guid[ 512 ];

	libcerror_error_t *error                        = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	int result                                      = 0;
	int utf16_guid_is_set                           = 0;

	/* Initialize test
	 */
	result = libewf_single_file_source_initialize(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_source",
	 single_file_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_source_get_utf16_guid(
	          single_file_source,
	          utf16_guid,
	          512,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_guid_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_source_get_utf16_guid(
	          NULL,
	          utf16_guid,
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

	if( utf16_guid_is_set != 0 )
	{
		result = libewf_single_file_source_get_utf16_guid(
		          single_file_source,
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

		result = libewf_single_file_source_get_utf16_guid(
		          single_file_source,
		          utf16_guid,
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

		result = libewf_single_file_source_get_utf16_guid(
		          single_file_source,
		          utf16_guid,
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
	}
	/* Clean up
	 */
	result = libewf_single_file_source_free(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_source",
	 single_file_source );

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
	if( single_file_source != NULL )
	{
		libewf_single_file_source_free(
		 &single_file_source,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_source_get_utf8_hash_value_md5 function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_source_get_utf8_hash_value_md5(
     void )
{
	uint8_t utf8_hash_value_md5[ 512 ];

	libcerror_error_t *error                      = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	int result                                    = 0;
	int utf8_hash_value_md5_is_set                = 0;

	/* Initialize test
	 */
	result = libewf_single_file_source_initialize(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_source",
	 single_file_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_source_get_utf8_hash_value_md5(
	          single_file_source,
	          utf8_hash_value_md5,
	          512,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_hash_value_md5_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_source_get_utf8_hash_value_md5(
	          NULL,
	          utf8_hash_value_md5,
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

	if( utf8_hash_value_md5_is_set != 0 )
	{
		result = libewf_single_file_source_get_utf8_hash_value_md5(
		          single_file_source,
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

		result = libewf_single_file_source_get_utf8_hash_value_md5(
		          single_file_source,
		          utf8_hash_value_md5,
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

		result = libewf_single_file_source_get_utf8_hash_value_md5(
		          single_file_source,
		          utf8_hash_value_md5,
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
	}
	/* Clean up
	 */
	result = libewf_single_file_source_free(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_source",
	 single_file_source );

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
	if( single_file_source != NULL )
	{
		libewf_single_file_source_free(
		 &single_file_source,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_source_get_utf16_hash_value_md5 function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_source_get_utf16_hash_value_md5(
     void )
{
	uint16_t utf16_hash_value_md5[ 512 ];

	libcerror_error_t *error                      = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	int result                                    = 0;
	int utf16_hash_value_md5_is_set               = 0;

	/* Initialize test
	 */
	result = libewf_single_file_source_initialize(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_source",
	 single_file_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_source_get_utf16_hash_value_md5(
	          single_file_source,
	          utf16_hash_value_md5,
	          512,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_hash_value_md5_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_source_get_utf16_hash_value_md5(
	          NULL,
	          utf16_hash_value_md5,
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

	if( utf16_hash_value_md5_is_set != 0 )
	{
		result = libewf_single_file_source_get_utf16_hash_value_md5(
		          single_file_source,
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

		result = libewf_single_file_source_get_utf16_hash_value_md5(
		          single_file_source,
		          utf16_hash_value_md5,
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

		result = libewf_single_file_source_get_utf16_hash_value_md5(
		          single_file_source,
		          utf16_hash_value_md5,
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
	}
	/* Clean up
	 */
	result = libewf_single_file_source_free(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_source",
	 single_file_source );

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
	if( single_file_source != NULL )
	{
		libewf_single_file_source_free(
		 &single_file_source,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_source_get_utf8_hash_value_sha1 function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_source_get_utf8_hash_value_sha1(
     void )
{
	uint8_t utf8_hash_value_sha1[ 512 ];

	libcerror_error_t *error                      = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	int result                                    = 0;
	int utf8_hash_value_sha1_is_set               = 0;

	/* Initialize test
	 */
	result = libewf_single_file_source_initialize(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_source",
	 single_file_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_source_get_utf8_hash_value_sha1(
	          single_file_source,
	          utf8_hash_value_sha1,
	          512,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_hash_value_sha1_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_source_get_utf8_hash_value_sha1(
	          NULL,
	          utf8_hash_value_sha1,
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

	if( utf8_hash_value_sha1_is_set != 0 )
	{
		result = libewf_single_file_source_get_utf8_hash_value_sha1(
		          single_file_source,
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

		result = libewf_single_file_source_get_utf8_hash_value_sha1(
		          single_file_source,
		          utf8_hash_value_sha1,
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

		result = libewf_single_file_source_get_utf8_hash_value_sha1(
		          single_file_source,
		          utf8_hash_value_sha1,
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
	}
	/* Clean up
	 */
	result = libewf_single_file_source_free(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_source",
	 single_file_source );

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
	if( single_file_source != NULL )
	{
		libewf_single_file_source_free(
		 &single_file_source,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_source_get_utf16_hash_value_sha1 function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_source_get_utf16_hash_value_sha1(
     void )
{
	uint16_t utf16_hash_value_sha1[ 512 ];

	libcerror_error_t *error                      = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	int result                                    = 0;
	int utf16_hash_value_sha1_is_set              = 0;

	/* Initialize test
	 */
	result = libewf_single_file_source_initialize(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_source",
	 single_file_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_source_get_utf16_hash_value_sha1(
	          single_file_source,
	          utf16_hash_value_sha1,
	          512,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_hash_value_sha1_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_source_get_utf16_hash_value_sha1(
	          NULL,
	          utf16_hash_value_sha1,
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

	if( utf16_hash_value_sha1_is_set != 0 )
	{
		result = libewf_single_file_source_get_utf16_hash_value_sha1(
		          single_file_source,
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

		result = libewf_single_file_source_get_utf16_hash_value_sha1(
		          single_file_source,
		          utf16_hash_value_sha1,
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

		result = libewf_single_file_source_get_utf16_hash_value_sha1(
		          single_file_source,
		          utf16_hash_value_sha1,
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
	}
	/* Clean up
	 */
	result = libewf_single_file_source_free(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_source",
	 single_file_source );

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
	if( single_file_source != NULL )
	{
		libewf_single_file_source_free(
		 &single_file_source,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_source_get_utf8_primary_guid function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_source_get_utf8_primary_guid(
     void )
{
	uint8_t utf8_primary_guid[ 512 ];

	libcerror_error_t *error                        = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	int result                                      = 0;
	int utf8_primary_guid_is_set                    = 0;

	/* Initialize test
	 */
	result = libewf_single_file_source_initialize(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_source",
	 single_file_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_source_get_utf8_primary_guid(
	          single_file_source,
	          utf8_primary_guid,
	          512,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_primary_guid_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_source_get_utf8_primary_guid(
	          NULL,
	          utf8_primary_guid,
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

	if( utf8_primary_guid_is_set != 0 )
	{
		result = libewf_single_file_source_get_utf8_primary_guid(
		          single_file_source,
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

		result = libewf_single_file_source_get_utf8_primary_guid(
		          single_file_source,
		          utf8_primary_guid,
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

		result = libewf_single_file_source_get_utf8_primary_guid(
		          single_file_source,
		          utf8_primary_guid,
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
	}
	/* Clean up
	 */
	result = libewf_single_file_source_free(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_source",
	 single_file_source );

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
	if( single_file_source != NULL )
	{
		libewf_single_file_source_free(
		 &single_file_source,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_source_get_utf16_primary_guid function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_source_get_utf16_primary_guid(
     void )
{
	uint16_t utf16_primary_guid[ 512 ];

	libcerror_error_t *error                        = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	int result                                      = 0;
	int utf16_primary_guid_is_set                   = 0;

	/* Initialize test
	 */
	result = libewf_single_file_source_initialize(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_source",
	 single_file_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_source_get_utf16_primary_guid(
	          single_file_source,
	          utf16_primary_guid,
	          512,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_primary_guid_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_source_get_utf16_primary_guid(
	          NULL,
	          utf16_primary_guid,
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

	if( utf16_primary_guid_is_set != 0 )
	{
		result = libewf_single_file_source_get_utf16_primary_guid(
		          single_file_source,
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

		result = libewf_single_file_source_get_utf16_primary_guid(
		          single_file_source,
		          utf16_primary_guid,
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

		result = libewf_single_file_source_get_utf16_primary_guid(
		          single_file_source,
		          utf16_primary_guid,
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
	}
	/* Clean up
	 */
	result = libewf_single_file_source_free(
	          &single_file_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_source",
	 single_file_source );

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
	if( single_file_source != NULL )
	{
		libewf_single_file_source_free(
		 &single_file_source,
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
	 "libewf_single_file_source_initialize",
	 ewf_test_single_file_source_initialize );

	EWF_TEST_RUN(
	 "libewf_single_file_source_free",
	 ewf_test_single_file_source_free );

	EWF_TEST_RUN(
	 "libewf_single_file_source_clone",
	 ewf_test_single_file_source_clone );	

	EWF_TEST_RUN(
	 "libewf_single_file_source_get_id",
	 ewf_test_single_file_source_get_id );

	EWF_TEST_RUN(
	 "libewf_single_file_source_get_type",
	 ewf_test_single_file_source_get_type );

	EWF_TEST_RUN(
	 "libewf_single_file_source_get_drive_type",
	 ewf_test_single_file_source_get_drive_type );
	
	EWF_TEST_RUN(
	 "libewf_single_file_source_get_physical_offset",
	 ewf_test_single_file_source_get_physical_offset );

	EWF_TEST_RUN(
	 "libewf_single_file_source_get_logical_offset",
	 ewf_test_single_file_source_get_logical_offset );

	EWF_TEST_RUN(
	 "libewf_single_file_source_get_total_size",
	 ewf_test_single_file_source_get_total_size );

	EWF_TEST_RUN(
	 "libewf_single_file_source_get_utf8_name_size",
	 ewf_test_single_file_source_get_utf8_name_size );

	EWF_TEST_RUN(
	 "libewf_single_file_source_get_utf8_name",
	 ewf_test_single_file_source_get_utf8_name );

	EWF_TEST_RUN(
	 "libewf_single_file_source_get_utf16_name_size",
	 ewf_test_single_file_source_get_utf16_name_size );

	EWF_TEST_RUN(
	 "libewf_single_file_source_get_utf16_name",
	 ewf_test_single_file_source_get_utf16_name );
	
	EWF_TEST_RUN(
	 "libewf_single_file_source_get_utf8_model_size",
	 ewf_test_single_file_source_get_utf8_model_size );

	EWF_TEST_RUN(
	 "libewf_single_file_source_get_utf8_model",
	 ewf_test_single_file_source_get_utf8_model );

	EWF_TEST_RUN(
	 "libewf_single_file_source_get_utf16_model_size",
	 ewf_test_single_file_source_get_utf16_model_size );

	EWF_TEST_RUN(
	 "libewf_single_file_source_get_utf16_model",
	 ewf_test_single_file_source_get_utf16_model );

	EWF_TEST_RUN(
	 "libewf_single_file_source_get_utf8_guid",
	 ewf_test_single_file_source_get_utf8_guid );

	EWF_TEST_RUN(
	 "libewf_single_file_source_get_utf16_guid",
	 ewf_test_single_file_source_get_utf16_guid );

	EWF_TEST_RUN(
	 "libewf_single_file_source_get_utf8_guid",
	 ewf_test_single_file_source_get_utf8_guid );

	EWF_TEST_RUN(
	 "libewf_single_file_source_get_utf16_guid",
	 ewf_test_single_file_source_get_utf16_guid );

	EWF_TEST_RUN(
	 "libewf_single_file_source_get_utf8_hash_value_md5",
	 ewf_test_single_file_source_get_utf8_hash_value_md5 );

	EWF_TEST_RUN(
	 "libewf_single_file_source_get_utf16_hash_value_md5",
	 ewf_test_single_file_source_get_utf16_hash_value_md5 );

	EWF_TEST_RUN(
	 "libewf_single_file_source_get_utf8_hash_value_sha1",
	 ewf_test_single_file_source_get_utf8_hash_value_sha1 );

	EWF_TEST_RUN(
	 "libewf_single_file_source_get_utf16_hash_value_sha1",
	 ewf_test_single_file_source_get_utf16_hash_value_sha1 );

	EWF_TEST_RUN(
	 "libewf_single_file_source_get_utf8_primary_guid",
	 ewf_test_single_file_source_get_utf8_primary_guid );

	EWF_TEST_RUN(
	 "libewf_single_file_source_get_utf16_primary_guid",
	 ewf_test_single_file_source_get_utf16_primary_guid );

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

