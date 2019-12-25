/*
 * Logical Evidence File (LEF) source functions
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

#include "libewf_lef_source.h"
#include "libewf_libcerror.h"

/* Creates a source
 * Make sure the value lef_source is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_initialize(
     libewf_lef_source_t **source,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_initialize";

	if( source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	if( *source != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid source value already set.",
		 function );

		return( -1 );
	}
	*source = memory_allocate_structure(
	           libewf_lef_source_t );

	if( *source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create source.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *source,
	     0,
	     sizeof( libewf_lef_source_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear source.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *source != NULL )
	{
		memory_free(
		 *source );

		*source = NULL;
	}
	return( -1 );
}

/* Frees a source
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_free(
     libewf_lef_source_t **source,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_free";

	if( source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	if( *source != NULL )
	{
		memory_free(
		 *source );

		*source = NULL;
	}
	return( 1 );
}

/* Clones the source
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_clone(
     libewf_lef_source_t **destination_source,
     libewf_lef_source_t *source_source,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_clone";

	if( destination_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination source.",
		 function );

		return( -1 );
	}
	if( *destination_source != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination source value already set.",
		 function );

		return( -1 );
	}
	if( source_source == NULL )
	{
		*destination_source = NULL;

		return( 1 );
	}
	*destination_source = memory_allocate_structure(
	                       libewf_lef_source_t );

	if( *destination_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination source.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_source,
	     source_source,
	     sizeof( libewf_lef_source_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination source.",
		 function );

		memory_free(
		 *destination_source );

		*destination_source = NULL;

		return( -1 );
	}
/* TODO implement */

	return( 1 );

on_error:
	if( *destination_source != NULL )
	{
		memory_free(
		 *destination_source );

		*destination_source = NULL;
	}
	return( -1 );
}

/* Reads a source
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_read_data(
     libewf_lef_source_t *source,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_read_data";

	if( source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing data.",
		 function );

		return( -1 );
	}
	if( ( data_size == 0 )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
/* TODO implement */

	return( 1 );
}

