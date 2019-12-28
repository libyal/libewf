/*
 * Attribute functions
 *
 * Copyright (C) 2006-2019, Joachim Metz <joachim.metz@gmail.com>
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
#include <memory.h>
#include <types.h>

#include "libewf_attribute.h"
#include "libewf_lef_extended_attribute.h"
#include "libewf_libcerror.h"
#include "libewf_libcthreads.h"
#include "libewf_types.h"

/* Creates a attribute
 * Make sure the value attribute is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_attribute_initialize(
     libewf_attribute_t **attribute,
     libewf_lef_extended_attribute_t *lef_extended_attribute,
     libcerror_error_t **error )
{
	libewf_internal_attribute_t *internal_attribute = NULL;
	static char *function                           = "libewf_attribute_initialize";

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	if( *attribute != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid attribute value already set.",
		 function );

		return( -1 );
	}
	if( lef_extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extended attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = memory_allocate_structure(
	                      libewf_internal_attribute_t );

	if( internal_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create attribute.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_attribute,
	     0,
	     sizeof( libewf_internal_attribute_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear attribute.",
		 function );

		memory_free(
		 internal_attribute );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_attribute->read_write_lock ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize read/write lock.",
		 function );

		goto on_error;
	}
#endif
	internal_attribute->lef_extended_attribute = lef_extended_attribute;

	*attribute = (libewf_attribute_t *) internal_attribute;

	return( 1 );

on_error:
	if( internal_attribute != NULL )
	{
		memory_free(
		 internal_attribute );
	}
	return( -1 );
}

/* Frees a attribute
 * Returns 1 if successful or -1 on error
 */
int libewf_attribute_free(
     libewf_attribute_t **attribute,
     libcerror_error_t **error )
{
	libewf_internal_attribute_t *internal_attribute = NULL;
	static char *function                                             = "libewf_attribute_free";
	int result                                                        = 1;

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	if( *attribute != NULL )
	{
		internal_attribute = (libewf_internal_attribute_t *) *attribute;
		*attribute         = NULL;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( internal_attribute->read_write_lock ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free read/write lock.",
			 function );

			result = -1;
		}
#endif
		memory_free(
		 internal_attribute );
	}
	return( result );
}

