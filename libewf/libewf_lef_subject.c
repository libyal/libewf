/*
 * Logical Evidence File (LEF) subject functions
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
#include <memory.h>
#include <types.h>

#include "libewf_lef_subject.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libfvalue.h"
#include "libewf_serialized_string.h"

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

		memory_free(
		 *lef_subject );

		*lef_subject = NULL;

		return( -1 );
	}
	if( libewf_serialized_string_initialize(
	     &( ( *lef_subject)->name ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create name string.",
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
	int result            = 1;

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
		if( ( *lef_subject )->name != NULL )
		{
			if( libewf_serialized_string_free(
			     &( ( *lef_subject )->name ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free name string.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *lef_subject );

		*lef_subject = NULL;
	}
	return( result );
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
	( *destination_lef_subject )->name = NULL;

	if( libewf_serialized_string_clone(
	     &( ( *destination_lef_subject )->name ),
	     source_lef_subject->name,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to clone destination name string.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_lef_subject != NULL )
	{
		libewf_lef_subject_free(
		 destination_lef_subject,
		 NULL );
	}
	return( -1 );
}

/* Reads a subject
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_subject_read_data(
     libewf_lef_subject_t *lef_subject,
     libfvalue_split_utf8_string_t *types,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfvalue_split_utf8_string_t *values = NULL;
	uint8_t *type_string                  = NULL;
	uint8_t *value_string                 = NULL;
	static char *function                 = "libewf_lef_subject_read_data";
	size_t type_string_size               = 0;
	size_t value_string_size              = 0;
	int number_of_types                   = 0;
	int number_of_values                  = 0;
	int value_index                       = 0;

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
	if( libfvalue_split_utf8_string_get_number_of_segments(
	     types,
	     &number_of_types,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of types",
		 function );

		goto on_error;
	}
	if( libfvalue_utf8_string_split(
	     data,
	     data_size,
	     (uint8_t) '\t',
	     &values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split data into string values.",
		 function );

		goto on_error;
	}
	if( libfvalue_split_utf8_string_get_number_of_segments(
	     values,
	     &number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of values",
		 function );

		goto on_error;
	}
	if( number_of_types != number_of_values )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: mismatch in number of types and values.",
		 function );

		goto on_error;
	}
	for( value_index = 0;
	     value_index < number_of_types;
	     value_index++ )
	{
		if( libfvalue_split_utf8_string_get_segment_by_index(
		     types,
		     value_index,
		     &type_string,
		     &type_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve type string: %d.",
			 function,
			 value_index );

			goto on_error;
		}
		if( ( type_string == NULL )
		 || ( type_string_size < 2 )
		 || ( type_string[ 0 ] == 0 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing type string: %d.",
			 function,
			 value_index );

			goto on_error;
		}
		if( value_index >= number_of_values )
		{
			value_string      = NULL;
			value_string_size = 0;
		}
		else
		{
			if( libfvalue_split_utf8_string_get_segment_by_index(
			     values,
			     value_index,
			     &value_string,
			     &value_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value string: %d.",
				 function,
				 value_index );

				goto on_error;
			}
			if( ( value_string == NULL )
			 || ( value_string_size < 2 )
			 || ( value_string[ 0 ] == 0 ) )
			{
				value_string      = NULL;
				value_string_size = 0;
			}
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: type: %s with value: %s\n",
			 function,
			 (char *) type_string,
			 (char *) value_string );
		}
#endif
		if( ( value_string == NULL )
		 || ( value_string_size == 0 ) )
		{
			/* Ignore empty values
			 */
		}
		else if( type_string_size == 2 )
		{
			if( type_string[ 0 ] == (uint8_t) 'n' )
			{
				if( libewf_serialized_string_read_data(
				     lef_subject->name,
				     value_string,
				     value_string_size - 1,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read name string.",
					 function );

					goto on_error;
				}
			}
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	if( libfvalue_split_utf8_string_free(
	     &values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free split values.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( values != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &values,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the identifier
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_subject_get_identifier(
     libewf_lef_subject_t *lef_subject,
     uint32_t *identifier,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_subject_get_identifier";

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
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	*identifier = lef_subject->identifier;

	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded name value
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_subject_get_utf8_name_size(
     libewf_lef_subject_t *lef_subject,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_subject_get_utf8_name_size";
	int result            = 0;

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
	result = libewf_serialized_string_get_utf8_string_size(
	          lef_subject->name,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve name UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded name value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_subject_get_utf8_name(
     libewf_lef_subject_t *lef_subject,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_subject_get_utf8_name";
	int result            = 0;

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
	result = libewf_serialized_string_get_utf8_string(
	          lef_subject->name,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy name to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded name value
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_subject_get_utf16_name_size(
     libewf_lef_subject_t *lef_subject,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_subject_get_utf16_name_size";
	int result            = 0;

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
	result = libewf_serialized_string_get_utf16_string_size(
	          lef_subject->name,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve name UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded name value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_subject_get_utf16_name(
     libewf_lef_subject_t *lef_subject,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_subject_get_utf16_name";
	int result            = 0;

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
	result = libewf_serialized_string_get_utf16_string(
	          lef_subject->name,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy name to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

