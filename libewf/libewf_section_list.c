/*
 * Section list functions
 *
 * Copyright (c) 2006-2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations.
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
#include <memory.h>

#include <liberror.h>
#include <libnotify.h>

#include "libewf_list_type.h"
#include "libewf_section_list.h"

/* Frees the section list values
 * Returns 1 if successful or -1 on error
 */
int libewf_section_list_values_free(
     intptr_t *value,
     liberror_error_t **error )
{
	if( value != NULL )
	{
		memory_free(
		 value );
	}
	return( 1 );
}

/* Append an entry to the section list
 * Returns 1 if successful or -1 on error
 */
int libewf_section_list_append(
     libewf_list_t *section_list,
     uint8_t *type,
     size_t type_size,
     off64_t start_offset,
     off64_t end_offset,
     liberror_error_t **error )
{
	libewf_section_list_values_t *section_list_values = NULL;
	static char *function                             = "libewf_section_list_append";

	if( section_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section list.",
		 function );

		return( -1 );
	}
	if( type == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid type.",
		 function );

		return( -1 );
	}
	if( ( type_size == 0 )
	 || ( type_size >= 16 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid type size value out of range.",
		 function );

		return( -1 );
	}
	section_list_values = (libewf_section_list_values_t *) memory_allocate(
	                                                        sizeof( libewf_section_list_values_t ) );

	if( section_list_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create section list values.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     section_list_values,
	     0,
	     sizeof( libewf_section_list_values_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear section list values.",
		 function );

		memory_free(
		 section_list_values );

		return( -1 );
	}
	if( memory_copy(
	     section_list_values->type,
	     type,
	     type_size ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set section list values type.",
		 function );

		memory_free(
		 section_list_values );

		return( -1 );
	}
	section_list_values->type_size    = type_size;
	section_list_values->start_offset = start_offset;
	section_list_values->end_offset   = end_offset;

	if( libewf_list_append_value(
	     section_list,
	     (intptr_t *) section_list_values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append section list values.",
		 function );

		memory_free(
		 section_list_values );

		return( -1 );
	}
	return( 1 );
}

