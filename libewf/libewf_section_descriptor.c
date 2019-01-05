/*
 * Section descriptor functions
 *
 * Copyright (C) 2006-2019, Joachim Metz <joachim.metz@gmail.com>
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libewf_libbfio.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_section_descriptor.h"

/* Creates a section descriptor
 * Make sure the value section_descriptor is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_section_descriptor_initialize(
     libewf_section_descriptor_t **section_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libewf_section_descriptor_initialize";

	if( section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section descriptor.",
		 function );

		return( -1 );
	}
	if( *section_descriptor != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid section descriptor value already set.",
		 function );

		return( -1 );
	}
	*section_descriptor = memory_allocate_structure(
	                       libewf_section_descriptor_t );

	if( *section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create section descriptor.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *section_descriptor,
	     0,
	     sizeof( libewf_section_descriptor_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear section descriptor.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *section_descriptor != NULL )
	{
		memory_free(
		 *section_descriptor );

		*section_descriptor = NULL;
	}
	return( -1 );
}

/* Frees a section descriptor
 * Returns 1 if successful or -1 on error
 */
int libewf_section_descriptor_free(
     libewf_section_descriptor_t **section_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libewf_section_descriptor_free";

	if( section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section descriptor.",
		 function );

		return( -1 );
	}
	if( *section_descriptor != NULL )
	{
		memory_free(
		 *section_descriptor );

		*section_descriptor = NULL;
	}
	return( 1 );
}

/* Clones the section descriptor
 * Returns 1 if successful or -1 on error
 */
int libewf_section_descriptor_clone(
     libewf_section_descriptor_t **destination_section_descriptor,
     libewf_section_descriptor_t *source_section_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libewf_section_descriptor_clone";

	if( destination_section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination section descriptor.",
		 function );

		return( -1 );
	}
	if( *destination_section_descriptor != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination section descriptor value already set.",
		 function );

		return( -1 );
	}
	if( source_section_descriptor == NULL )
	{
		*destination_section_descriptor = NULL;

		return( 1 );
	}
	*destination_section_descriptor = memory_allocate_structure(
	                                   libewf_section_descriptor_t );

	if( *destination_section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination section descriptor.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_section_descriptor,
	     source_section_descriptor,
	     sizeof( libewf_section_descriptor_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination section descriptor.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_section_descriptor != NULL )
	{
		memory_free(
		 *destination_section_descriptor );

		*destination_section_descriptor = NULL;
	}
	return( -1 );
}

