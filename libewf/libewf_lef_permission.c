/*
 * Logical Evidence File (LEF) permission functions
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

#include "libewf_lef_permission.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libfvalue.h"
#include "libewf_serialized_string.h"

/* Creates a permission
 * Make sure the value lef_permission is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_permission_initialize(
     libewf_lef_permission_t **lef_permission,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_permission_initialize";

	if( lef_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid permission.",
		 function );

		return( -1 );
	}
	if( *lef_permission != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid permission value already set.",
		 function );

		return( -1 );
	}
	*lef_permission = memory_allocate_structure(
	                   libewf_lef_permission_t );

	if( *lef_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create permission.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *lef_permission,
	     0,
	     sizeof( libewf_lef_permission_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear permission.",
		 function );

		memory_free(
		 *lef_permission );

		*lef_permission = NULL;

		return( -1 );
	}
	if( libewf_serialized_string_initialize(
	     &( ( *lef_permission )->name ),
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
	if( libewf_serialized_string_initialize(
	     &( ( *lef_permission )->identifier ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create identifier string.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *lef_permission != NULL )
	{
		if( ( *lef_permission )->name != NULL )
		{
			libewf_serialized_string_free(
			 &( ( *lef_permission )->name ),
			 NULL );
		}
		memory_free(
		 *lef_permission );

		*lef_permission = NULL;
	}
	return( -1 );
}

/* Frees a permission
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_permission_free(
     libewf_lef_permission_t **lef_permission,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_permission_free";
	int result            = 1;

	if( lef_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid permission.",
		 function );

		return( -1 );
	}
	if( *lef_permission != NULL )
	{
		if( ( *lef_permission )->name != NULL )
		{
			if( libewf_serialized_string_free(
			     &( ( *lef_permission )->name ),
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
		if( ( *lef_permission )->identifier != NULL )
		{
			if( libewf_serialized_string_free(
			     &( ( *lef_permission )->identifier ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free identifier string.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *lef_permission );

		*lef_permission = NULL;
	}
	return( result );
}

/* Clones the permission
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_permission_clone(
     libewf_lef_permission_t **destination_lef_permission,
     libewf_lef_permission_t *source_lef_permission,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_permission_clone";

	if( destination_lef_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination permission.",
		 function );

		return( -1 );
	}
	if( *destination_lef_permission != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination permission value already set.",
		 function );

		return( -1 );
	}
	if( source_lef_permission == NULL )
	{
		*destination_lef_permission = NULL;

		return( 1 );
	}
	*destination_lef_permission = memory_allocate_structure(
	                               libewf_lef_permission_t );

	if( *destination_lef_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination permission.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_lef_permission,
	     source_lef_permission,
	     sizeof( libewf_lef_permission_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination permission.",
		 function );

		memory_free(
		 *destination_lef_permission );

		*destination_lef_permission = NULL;

		return( -1 );
	}
	( *destination_lef_permission )->name       = NULL;
	( *destination_lef_permission )->identifier = NULL;

	if( libewf_serialized_string_clone(
	     &( ( *destination_lef_permission )->name ),
	     source_lef_permission->name,
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
	if( libewf_serialized_string_clone(
	     &( ( *destination_lef_permission )->identifier ),
	     source_lef_permission->identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to clone destination identifier string.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_lef_permission != NULL )
	{
		libewf_lef_permission_free(
		 destination_lef_permission,
		 NULL );
	}
	return( -1 );
}

/* Reads a permission
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_permission_read_data(
     libewf_lef_permission_t *lef_permission,
     libfvalue_split_utf8_string_t *types,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfvalue_split_utf8_string_t *values = NULL;
	uint8_t *type_string                  = NULL;
	uint8_t *value_string                 = NULL;
	static char *function                 = "libewf_lef_permission_read_data";
	size_t type_string_size               = 0;
	size_t value_string_size              = 0;
	uint64_t value_64bit                  = 0;
	int number_of_types                   = 0;
	int number_of_values                  = 0;
	int value_index                       = 0;

	if( lef_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid permission.",
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
		else if( type_string_size == 4 )
		{
			if( ( type_string[ 0 ] == (uint8_t) 'n' )
			 && ( type_string[ 1 ] == (uint8_t) 't' )
			 && ( type_string[ 2 ] == (uint8_t) 'a' ) )
			{
				if( libfvalue_utf8_string_copy_to_integer(
				     value_string,
				     value_string_size,
				     &value_64bit,
				     64,
				     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set access mask.",
					 function );

					goto on_error;
				}
				if( value_64bit > (uint64_t) UINT32_MAX )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid access mask value out of bounds.",
					 function );

					goto on_error;
				}
				lef_permission->access_mask = (uint32_t) value_64bit;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'n' )
			      && ( type_string[ 1 ] == (uint8_t) 't' )
			      && ( type_string[ 2 ] == (uint8_t) 'i' ) )
			{
				if( libfvalue_utf8_string_copy_to_integer(
				     value_string,
				     value_string_size,
				     &value_64bit,
				     64,
				     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set ACE flags.",
					 function );

					goto on_error;
				}
				if( value_64bit > (uint64_t) UINT32_MAX )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid ACE flags value out of bounds.",
					 function );

					goto on_error;
				}
				lef_permission->ace_flags = (uint32_t) value_64bit;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'n' )
			      && ( type_string[ 1 ] == (uint8_t) 't' )
			      && ( type_string[ 2 ] == (uint8_t) 's' ) )
			{
/* TODO implement */
			}
		}
		else if( type_string_size == 3 )
		{
			if( ( type_string[ 0 ] == (uint8_t) 'p' )
			 && ( type_string[ 1 ] == (uint8_t) 'r' ) )
			{
				if( libfvalue_utf8_string_copy_to_integer(
				     value_string,
				     value_string_size,
				     &value_64bit,
				     64,
				     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set property type.",
					 function );

					goto on_error;
				}
				if( value_64bit > (uint64_t) UINT32_MAX )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid property type value out of bounds.",
					 function );

					goto on_error;
				}
				lef_permission->property_type = (uint32_t) value_64bit;
			}
		}
		else if( type_string_size == 2 )
		{
			if( type_string[ 0 ] == (uint8_t) 'n' )
			{
				if( libewf_serialized_string_read_data(
				     lef_permission->name,
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
			else if( type_string[ 0 ] == (uint8_t) 's' )
			{
				if( libewf_serialized_string_read_data(
				     lef_permission->identifier,
				     value_string,
				     value_string_size - 1,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read identifier string.",
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

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_lef_permission_get_utf8_name_size(
     libewf_lef_permission_t *lef_permission,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_permission_get_utf8_name_size";
	int result            = 0;

	if( lef_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid permission.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf8_string_size(
	          lef_permission->name,
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
	return( result );
}

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_lef_permission_get_utf8_name(
     libewf_lef_permission_t *lef_permission,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_permission_get_utf8_name";
	int result            = 0;

	if( lef_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid permission.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf8_string(
	          lef_permission->name,
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
	return( result );
}

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_lef_permission_get_utf16_name_size(
     libewf_lef_permission_t *lef_permission,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_permission_get_utf16_name_size";
	int result            = 0;

	if( lef_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid permission.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf16_string_size(
	          lef_permission->name,
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
	return( result );
}

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_lef_permission_get_utf16_name(
     libewf_lef_permission_t *lef_permission,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_permission_get_utf16_name";
	int result            = 0;

	if( lef_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid permission.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf16_string(
	          lef_permission->name,
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
	return( result );
}

/* Retrieves the size of the UTF-8 encoded identifier
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_lef_permission_get_utf8_identifier_size(
     libewf_lef_permission_t *lef_permission,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_permission_get_utf8_identifier_size";
	int result            = 0;

	if( lef_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid permission.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf8_string_size(
	          lef_permission->identifier,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve identifier UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the UTF-8 encoded identifier
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_lef_permission_get_utf8_identifier(
     libewf_lef_permission_t *lef_permission,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_permission_get_utf8_identifier";
	int result            = 0;

	if( lef_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid permission.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf8_string(
	          lef_permission->identifier,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy identifier to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the size of the UTF-16 encoded identifier
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_lef_permission_get_utf16_identifier_size(
     libewf_lef_permission_t *lef_permission,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_permission_get_utf16_identifier_size";
	int result            = 0;

	if( lef_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid permission.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf16_string_size(
	          lef_permission->identifier,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve identifier UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the UTF-16 encoded identifier
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_lef_permission_get_utf16_identifier(
     libewf_lef_permission_t *lef_permission,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_permission_get_utf16_identifier";
	int result            = 0;

	if( lef_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid permission.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf16_string(
	          lef_permission->identifier,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy identifier to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the property type
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_permission_get_property_type(
     libewf_lef_permission_t *lef_permission,
     uint32_t *property_type,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_permission_get_property_type";

	if( lef_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid permission.",
		 function );

		return( -1 );
	}
	if( property_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid property type.",
		 function );

		return( -1 );
	}
	*property_type = lef_permission->property_type;

	return( 1 );
}

/* Retrieves the access mask
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_permission_get_access_mask(
     libewf_lef_permission_t *lef_permission,
     uint32_t *access_mask,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_permission_get_access_mask";

	if( lef_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid permission.",
		 function );

		return( -1 );
	}
	if( access_mask == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid access mask.",
		 function );

		return( -1 );
	}
	*access_mask = lef_permission->access_mask;

	return( 1 );
}

/* Retrieves the Windows NT access control entry (ACE) flags
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_permission_get_ace_flags(
     libewf_lef_permission_t *lef_permission,
     uint32_t *ace_flags,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_permission_get_ace_flags";

	if( lef_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid permission.",
		 function );

		return( -1 );
	}
	if( ace_flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid ACE flags.",
		 function );

		return( -1 );
	}
	*ace_flags = lef_permission->ace_flags;

	return( 1 );
}

