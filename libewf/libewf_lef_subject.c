/*
 * Logical Evidence File (LEF) subject functions
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

#include "libewf_lef_subject.h"
#include "libewf_libcerror.h"

/* Creates a subject
 * Make sure the value lef_subject is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_subject_initialize(
     libewf_lef_subject_t **lef_subject,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_subject_initialize";

	if( lef_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid subject.",
		 function );

		return( -1 );
	}
	if( *lef_subject != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid subject value already set.",
		 function );

		return( -1 );
	}
	*lef_subject = memory_allocate_structure(
	                libewf_lef_subject_t );

	if( *lef_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create subject.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *lef_subject,
	     0,
	     sizeof( libewf_lef_subject_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear subject.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *lef_subject != NULL )
	{
		memory_free(
		 *lef_subject );

		*lef_subject = NULL;
	}
	return( -1 );
}

/* Frees a subject
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_subject_free(
     libewf_lef_subject_t **lef_subject,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_subject_free";

	if( lef_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid subject.",
		 function );

		return( -1 );
	}
	if( *lef_subject != NULL )
	{
		memory_free(
		 *lef_subject );

		*lef_subject = NULL;
	}
	return( 1 );
}

/* Clones the subject
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_subject_clone(
     libewf_lef_subject_t **destination_lef_subject,
     libewf_lef_subject_t *source_lef_subject,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_subject_clone";

	if( destination_lef_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination subject.",
		 function );

		return( -1 );
	}
	if( *destination_lef_subject != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination subject value already set.",
		 function );

		return( -1 );
	}
	if( source_lef_subject == NULL )
	{
		*destination_lef_subject = NULL;

		return( 1 );
	}
	*destination_lef_subject = memory_allocate_structure(
	                            libewf_lef_subject_t );

	if( *destination_lef_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination subject.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_lef_subject,
	     source_lef_subject,
	     sizeof( libewf_lef_subject_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination subject.",
		 function );

		memory_free(
		 *destination_lef_subject );

		*destination_lef_subject = NULL;

		return( -1 );
	}
/* TODO implement */

	return( 1 );

on_error:
	if( *destination_lef_subject != NULL )
	{
		memory_free(
		 *destination_lef_subject );

		*destination_lef_subject = NULL;
	}
	return( -1 );
}

/* Reads a subject
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_subject_read_data(
     libewf_lef_subject_t *lef_subject,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_subject_read_data";

	if( lef_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid subject.",
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

