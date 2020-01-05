/*
 * Memory allocation functions for testing
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

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ )
#define __USE_GNU
#include <dlfcn.h>
#undef __USE_GNU
#endif

#include "ewf_test_memory.h"

#if defined( HAVE_EWF_TEST_MEMORY )

static void *(*ewf_test_real_malloc)(size_t)                       = NULL;
static void *(*ewf_test_real_memcpy)(void *, const void *, size_t) = NULL;
static void *(*ewf_test_real_memset)(void *, int, size_t)          = NULL;
static void *(*ewf_test_real_realloc)(void *, size_t)              = NULL;

int ewf_test_malloc_attempts_before_fail                           = -1;
int ewf_test_memcpy_attempts_before_fail                           = -1;
int ewf_test_memset_attempts_before_fail                           = -1;
int ewf_test_realloc_attempts_before_fail                          = -1;

/* Custom malloc for testing memory error cases
 * Note this function might fail if compiled with optimation
 * Returns a pointer to newly allocated data or NULL
 */
void *malloc(
       size_t size )
{
	void *ptr = NULL;

	if( ewf_test_real_malloc == NULL )
	{
		ewf_test_real_malloc = dlsym(
		                        RTLD_NEXT,
		                        "malloc" );
	}
	if( ewf_test_malloc_attempts_before_fail == 0 )
	{
		ewf_test_malloc_attempts_before_fail = -1;

		return( NULL );
	}
	else if( ewf_test_malloc_attempts_before_fail > 0 )
	{
		ewf_test_malloc_attempts_before_fail--;
	}
	ptr = ewf_test_real_malloc(
	       size );

	return( ptr );
}

/* Custom memcpy for testing memory error cases
 * Note this function might fail if compiled with optimation and as a shared libary
 * Returns a pointer to newly allocated data or NULL
 */
void *memcpy(
       void *destination,
       const void *source,
       size_t size )
{
	if( ewf_test_real_memcpy == NULL )
	{
		ewf_test_real_memcpy = dlsym(
		                        RTLD_NEXT,
		                        "memcpy" );
	}
	if( ewf_test_memcpy_attempts_before_fail == 0 )
	{
		ewf_test_memcpy_attempts_before_fail = -1;

		return( NULL );
	}
	else if( ewf_test_memcpy_attempts_before_fail > 0 )
	{
		ewf_test_memcpy_attempts_before_fail--;
	}
	destination = ewf_test_real_memcpy(
	               destination,
	               source,
	               size );

	return( destination );
}

/* Custom memset for testing memory error cases
 * Note this function might fail if compiled with optimation and as a shared libary
 * Returns a pointer to newly allocated data or NULL
 */
void *memset(
       void *ptr,
       int constant,
       size_t size )
{
	if( ewf_test_real_memset == NULL )
	{
		ewf_test_real_memset = dlsym(
		                        RTLD_NEXT,
		                        "memset" );
	}
	if( ewf_test_memset_attempts_before_fail == 0 )
	{
		ewf_test_memset_attempts_before_fail = -1;

		return( NULL );
	}
	else if( ewf_test_memset_attempts_before_fail > 0 )
	{
		ewf_test_memset_attempts_before_fail--;
	}
	ptr = ewf_test_real_memset(
	       ptr,
	       constant,
	       size );

	return( ptr );
}

/* Custom realloc for testing memory error cases
 * Note this function might fail if compiled with optimation
 * Returns a pointer to reallocated data or NULL
 */
void *realloc(
       void *ptr,
       size_t size )
{
	if( ewf_test_real_realloc == NULL )
	{
		ewf_test_real_realloc = dlsym(
		                         RTLD_NEXT,
		                         "realloc" );
	}
	if( ewf_test_realloc_attempts_before_fail == 0 )
	{
		ewf_test_realloc_attempts_before_fail = -1;

		return( NULL );
	}
	else if( ewf_test_realloc_attempts_before_fail > 0 )
	{
		ewf_test_realloc_attempts_before_fail--;
	}
	ptr = ewf_test_real_realloc(
	       ptr,
	       size );

	return( ptr );
}

#endif /* defined( HAVE_EWF_TEST_MEMORY ) */

