/*
 * Single file entries functions
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
#include <narrow_string.h>
#include <types.h>

#include "libewf_definitions.h"
#include "libewf_lef_file_entry.h"
#include "libewf_lef_permission.h"
#include "libewf_lef_source.h"
#include "libewf_lef_subject.h"
#include "libewf_libcdata.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libfvalue.h"
#include "libewf_libuna.h"
#include "libewf_permission_group.h"
#include "libewf_single_files.h"

/* Creates single files
 * Make sure the value single_files is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_single_files_initialize(
     libewf_single_files_t **single_files,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_files_initialize";

	if( single_files == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single files.",
		 function );

		return( -1 );
	}
	if( *single_files != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid single files value already set.",
		 function );

		return( -1 );
	}
	*single_files = memory_allocate_structure(
	                 libewf_single_files_t );

	if( *single_files == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create single files.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *single_files,
	     0,
	     sizeof( libewf_single_files_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear single files.",
		 function );

		memory_free(
		 *single_files );

		*single_files = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *single_files )->permission_groups ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create permission groups array.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( ( *single_files )->sources ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sources array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *single_files != NULL )
	{
		if( ( *single_files )->permission_groups != NULL )
		{
			libcdata_array_free(
			 &( ( *single_files )->permission_groups ),
			 (int (*)(intptr_t **, libcerror_error_t **)) &libewf_permission_group_free,
			 NULL );
		}
		memory_free(
		 *single_files );

		*single_files = NULL;
	}
	return( -1 );
}

/* Frees single files
 * Returns 1 if successful or -1 on error
 */
int libewf_single_files_free(
     libewf_single_files_t **single_files,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_files_free";
	int result            = 1;

	if( single_files == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single files.",
		 function );

		return( -1 );
	}
	if( *single_files != NULL )
	{
		if( ( *single_files )->permission_groups != NULL )
		{
			if( libcdata_array_free(
			     &( ( *single_files )->permission_groups ),
			     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_permission_group_free,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free permission groups array.",
				 function );

				result = -1;
			}
		}
		if( ( *single_files )->sources != NULL )
		{
			if( libcdata_array_free(
			     &( ( *single_files )->sources ),
			     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_lef_source_free,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free sources array.",
				 function );

				result = -1;
			}
		}
		if( ( *single_files )->file_entry_tree_root_node != NULL )
		{
			if( libcdata_tree_node_free(
			     &( ( *single_files )->file_entry_tree_root_node ),
			     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_lef_file_entry_free,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free file entry tree root node.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *single_files );

		*single_files = NULL;
	}
	return( result );
}

/* Clones the single files
 * Returns 1 if successful or -1 on error
 */
int libewf_single_files_clone(
     libewf_single_files_t **destination_single_files,
     libewf_single_files_t *source_single_files,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_files_clone";

	if( destination_single_files == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination single files.",
		 function );

		return( -1 );
	}
	if( *destination_single_files != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination single files value already set.",
		 function );

		return( -1 );
	}
	if( source_single_files == NULL )
	{
		*destination_single_files = NULL;

		return( 1 );
	}
	*destination_single_files = memory_allocate_structure(
	                             libewf_single_files_t );

	if( *destination_single_files == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination single files.",
		 function );

		goto on_error;
	}
	( *destination_single_files )->permission_groups         = NULL;
	( *destination_single_files )->sources                   = NULL;
	( *destination_single_files )->file_entry_tree_root_node = NULL;

	if( libcdata_array_clone(
	     &( ( *destination_single_files )->permission_groups ),
	     source_single_files->permission_groups,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_permission_group_free,
	     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libewf_permission_group_clone,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination permission groups array.",
		 function );

		goto on_error;
	}
	if( libcdata_array_clone(
	     &( ( *destination_single_files )->sources ),
	     source_single_files->sources,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_lef_source_free,
	     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libewf_lef_source_clone,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination sources array.",
		 function );

		goto on_error;
	}
	if( libcdata_tree_node_clone(
	     &( ( *destination_single_files )->file_entry_tree_root_node ),
	     source_single_files->file_entry_tree_root_node,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_lef_file_entry_free,
	     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libewf_lef_file_entry_clone,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination file entry tree root node.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_single_files != NULL )
	{
		libewf_single_files_free(
		 destination_single_files,
		 NULL );
	}
	return( -1 );
}

/* Parses a line
 * Returns 1 if successful or -1 on error
 */
int libewf_single_files_parse_line(
     libfvalue_split_utf8_string_t *lines,
     int line_index,
     uint8_t **line_string,
     size_t *line_string_size,
     libcerror_error_t **error )
{
	uint8_t *safe_line_string    = NULL;
	static char *function        = "libewf_single_files_parse_line";
	size_t safe_line_string_size = 0;

	if( line_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid line string.",
		 function );

		return( -1 );
	}
	if( line_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid line string size.",
		 function );

		return( -1 );
	}
	if( libfvalue_split_utf8_string_get_segment_by_index(
	     lines,
	     line_index,
	     &safe_line_string,
	     &safe_line_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve line string: %d.",
		 function,
		 line_index );

		return( -1 );
	}
	if( safe_line_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing line string: %d.",
		 function,
		 line_index );

		return( -1 );
	}
	/* Remove trailing carriage return
	 */
	if( safe_line_string_size >= 2 )
	{
		if( safe_line_string[ safe_line_string_size - 2 ] == (uint8_t) '\r' )
		{
			safe_line_string[ safe_line_string_size - 2 ] = 0;

			safe_line_string_size -= 1;
		}
	}
	*line_string      = safe_line_string;
	*line_string_size = safe_line_string_size;

	return( 1 );
}

/* Parses the number of entries in a category
 * Returns 1 if successful or -1 on error
 */
int libewf_single_files_parse_category_number_of_entries(
     libfvalue_split_utf8_string_t *lines,
     int *line_index,
     int *number_of_entries,
     libcerror_error_t **error )
{
	libfvalue_split_utf8_string_t *values = NULL;
	uint8_t *line_string                  = NULL;
	uint8_t *value_string                 = NULL;
	static char *function                 = "libewf_single_files_parse_category_number_of_entries";
	size_t line_string_size               = 0;
	size_t value_string_size              = 0;
	uint64_t value_64bit                  = 0;
	int number_of_values                  = 0;
	int safe_line_index                   = 0;

	if( line_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid line index.",
		 function );

		return( -1 );
	}
	if( number_of_entries == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of entries.",
		 function );

		return( -1 );
	}
	safe_line_index = *line_index;

	if( libewf_single_files_parse_line(
	     lines,
	     safe_line_index,
	     &line_string,
	     &line_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve line string: %d.",
		 function,
		 safe_line_index );

		goto on_error;
	}
	if( libfvalue_utf8_string_split(
	     line_string,
	     line_string_size,
	     (uint8_t) '\t',
	     &values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split line: %d string into values.",
		 function,
		 safe_line_index );

		goto on_error;
	}
	safe_line_index += 1;

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
	if( number_of_values != 2 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported number of values.",
		 function );

		goto on_error;
	}
	if( libfvalue_split_utf8_string_get_segment_by_index(
	     values,
	     1,
	     &value_string,
	     &value_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value string: 1.",
		 function );

		goto on_error;
	}
	if( ( value_string_size != 2 )
	 || ( value_string[ 0 ] != (uint8_t) '1' ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported second value: %s.",
		 function,
		 value_string );

		goto on_error;
	}
	if( libfvalue_split_utf8_string_get_segment_by_index(
	     values,
	     0,
	     &value_string,
	     &value_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value string: 0.",
		 function );

		goto on_error;
	}
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
		 "%s: unable to set number of entries.",
		 function );

		goto on_error;
	}
	if( value_64bit > (uint64_t) INT_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of entries value out of bounds.",
		 function );

		goto on_error;
	}
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
	*line_index        = safe_line_index;
	*number_of_entries = (int) value_64bit;

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

/* Parses the types of a category
 * Returns 1 if successful or -1 on error
 */
int libewf_single_files_parse_category_types(
     libfvalue_split_utf8_string_t *lines,
     int *line_index,
     libfvalue_split_utf8_string_t **types,
     libcerror_error_t **error )
{
	uint8_t *line_string    = NULL;
	static char *function   = "libewf_single_files_parse_category_types";
	size_t line_string_size = 0;
	int safe_line_index     = 0;

	if( line_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid line index.",
		 function );

		return( -1 );
	}
	safe_line_index = *line_index;

	if( libewf_single_files_parse_line(
	     lines,
	     safe_line_index,
	     &line_string,
	     &line_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve line string: %d.",
		 function,
		 safe_line_index );

		return( -1 );
	}
	if( libfvalue_utf8_string_split(
	     line_string,
	     line_string_size,
	     (uint8_t) '\t',
	     types,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split line: %d into types.",
		 function );

		return( -1 );
	}
	safe_line_index += 1;

	*line_index = safe_line_index;

	return( 1 );
}

/* Parses the number of entries
 * Returns 1 if successful or -1 on error
 */
int libewf_single_files_parse_number_of_entries(
     libfvalue_split_utf8_string_t *lines,
     int *line_index,
     int *number_of_entries,
     libcerror_error_t **error )
{
	libfvalue_split_utf8_string_t *values = NULL;
	uint8_t *line_string                  = NULL;
	uint8_t *value_string                 = NULL;
	static char *function                 = "libewf_single_files_parse_number_of_entries";
	size_t line_string_size               = 0;
	size_t value_string_size              = 0;
	uint64_t value_64bit                  = 0;
	int number_of_values                  = 0;
	int safe_line_index                   = 0;

	if( line_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid line index.",
		 function );

		return( -1 );
	}
	if( number_of_entries == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of entries.",
		 function );

		return( -1 );
	}
	safe_line_index = *line_index;

	if( libewf_single_files_parse_line(
	     lines,
	     safe_line_index,
	     &line_string,
	     &line_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve line string: %d.",
		 function,
		 safe_line_index );

		goto on_error;
	}
	if( libfvalue_utf8_string_split(
	     line_string,
	     line_string_size,
	     (uint8_t) '\t',
	     &values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split line: %d string into values.",
		 function,
		 safe_line_index );

		goto on_error;
	}
	safe_line_index += 1;

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
	if( number_of_values != 2 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported number of values.",
		 function );

		goto on_error;
	}
	if( libfvalue_split_utf8_string_get_segment_by_index(
	     values,
	     0,
	     &value_string,
	     &value_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value string: 0.",
		 function );

		goto on_error;
	}
	if( ( value_string_size != 2 )
	 || ( value_string[ 0 ] != (uint8_t) '0' ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported first value: %s.",
		 function,
		 value_string );

		goto on_error;
	}
	if( libfvalue_split_utf8_string_get_segment_by_index(
	     values,
	     1,
	     &value_string,
	     &value_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value string: 1.",
		 function );

		goto on_error;
	}
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
		 "%s: unable to set number of entries.",
		 function );

		goto on_error;
	}
	if( value_64bit > (uint64_t) INT_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of entries value out of bounds.",
		 function );

		goto on_error;
	}
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
	*line_index        = safe_line_index;
	*number_of_entries = (int) value_64bit;

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

/* Determines the EWF format based on the entry types
 * Returns 1 if successful or -1 on error
 */
int libewf_single_files_parse_format(
     libfvalue_split_utf8_string_t *types,
     uint8_t *format,
     libcerror_error_t **error )
{
	uint8_t *type_string    = NULL;
	static char *function   = "libewf_single_files_parse_format";
	size_t type_string_size = 0;
	int number_of_types     = 0;
	int value_index         = 0;

	if( format == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid format.",
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

		return( -1 );
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

			return( -1 );
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

			return( -1 );
		}
		if( type_string_size == 3 )
		{
			/* Data offset
			 * consist of: unknown, offset and size
			 */
			if( ( type_string[ 0 ] == (uint8_t) 'b' )
			 && ( type_string[ 1 ] == (uint8_t) 'e' ) )
			{
				if( value_index == 19 )
				{
					*format = LIBEWF_FORMAT_LOGICAL_ENCASE5;
				}
				else if( ( value_index == 20 )
				      || ( value_index == 21 ) )
				{
					*format = LIBEWF_FORMAT_LOGICAL_ENCASE6;
				}
				else if( value_index == 2 )
				{
					*format = LIBEWF_FORMAT_LOGICAL_ENCASE7;
				}
			}
		}
	}
	return( 1 );
}

/* Parses the "rec" category
 * Returns 1 if successful or -1 on error
 */
int libewf_single_files_parse_rec_category(
     libewf_single_files_t *single_files,
     libfvalue_split_utf8_string_t *lines,
     int *line_index,
     size64_t *media_size,
     libcerror_error_t **error )
{
	uint8_t *line_string    = NULL;
	static char *function   = "libewf_single_files_parse_rec_category";
	size_t line_string_size = 0;
	int safe_line_index     = 0;

	if( single_files == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single files.",
		 function );

		return( -1 );
	}
	if( line_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid line index.",
		 function );

		return( -1 );
	}
	safe_line_index = *line_index;

	if( libewf_single_files_parse_line(
	     lines,
	     safe_line_index,
	     &line_string,
	     &line_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve line string: %d.",
		 function,
		 safe_line_index );

		return( -1 );
	}
	safe_line_index += 1;

	if( ( line_string_size != 4 )
	 || ( line_string[ 0 ] != (uint8_t) 'r' )
	 || ( line_string[ 1 ] != (uint8_t) 'e' )
	 || ( line_string[ 2 ] != (uint8_t) 'c' ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported category header.",
		 function );

		return( -1 );
	}
	if( libewf_single_files_parse_record_values(
	     lines,
	     &safe_line_index,
	     media_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse record values.",
		 function );

		return( -1 );
	}
	/* The category should be followed by an empty line
	 */
	if( libewf_single_files_parse_line(
	     lines,
	     safe_line_index,
	     &line_string,
	     &line_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve line string: %d.",
		 function,
		 safe_line_index );

		return( -1 );
	}
	if( ( line_string_size != 1 )
	 || ( line_string[ 0 ] != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported empty line string: %d - not empty.",
		 function,
		 safe_line_index );

		return( -1 );
	}
	safe_line_index += 1;

	*line_index = safe_line_index;

	return( 1 );
}

/* Parses a record string for the values
 * Returns 1 if successful or -1 on error
 */
int libewf_single_files_parse_record_values(
     libfvalue_split_utf8_string_t *lines,
     int *line_index,
     size64_t *media_size,
     libcerror_error_t **error )
{
	libfvalue_split_utf8_string_t *types  = NULL;
	libfvalue_split_utf8_string_t *values = NULL;
	uint8_t *line_string                  = NULL;
	uint8_t *type_string                  = NULL;
	uint8_t *value_string                 = NULL;
	static char *function                 = "libewf_single_files_parse_record_values";
	size_t line_string_size               = 0;
	size_t type_string_size               = 0;
	size_t value_string_size              = 0;
	uint64_t value_64bit                  = 0;
	int number_of_types                   = 0;
	int number_of_values                  = 0;
	int safe_line_index                   = 0;
	int value_index                       = 0;

	if( line_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid line index.",
		 function );

		return( -1 );
	}
	if( media_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media size.",
		 function );

		return( -1 );
	}
	safe_line_index = *line_index;

	if( libewf_single_files_parse_line(
	     lines,
	     safe_line_index,
	     &line_string,
	     &line_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve line string: %d.",
		 function,
		 safe_line_index );

		goto on_error;
	}
	safe_line_index += 1;

	if( libfvalue_utf8_string_split(
	     line_string,
	     line_string_size,
	     (uint8_t) '\t',
	     &types,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split line string into types.",
		 function );

		goto on_error;
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
	if( libewf_single_files_parse_line(
	     lines,
	     safe_line_index,
	     &line_string,
	     &line_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve line string: %d.",
		 function,
		 safe_line_index );

		goto on_error;
	}
	safe_line_index += 1;

	if( libfvalue_utf8_string_split(
	     line_string,
	     line_string_size,
	     (uint8_t) '\t',
	     &values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split line string into values.",
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
#if defined( HAVE_VERBOSE_OUTPUT )
	if( number_of_types != number_of_values )
	{
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			"%s: mismatch in number of types and values.\n",
			 function );
		}
	}
#endif
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
			if( value_string == NULL )
			{
				libcnotify_printf(
				 "%s: type: %s with value: (empty)\n",
				 function,
				 (char *) type_string );
			}
			else
			{
				libcnotify_printf(
				 "%s: type: %s with value: %s\n",
				 function,
				 (char *) type_string,
				 (char *) value_string );
			}
		}
#endif
		/* Ignore empty values
		 */
		if( value_string == NULL )
		{
			continue;
		}
		if( type_string_size == 3 )
		{
			if( ( type_string[ 0 ] == (uint8_t) 'c' )
			 && ( type_string[ 1 ] == (uint8_t) 'l' ) )
			{
			}
			else if( ( type_string[ 0 ] == (uint8_t) 't' )
			      && ( type_string[ 1 ] == (uint8_t) 'b' ) )
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
					 "%s: unable to set media size.",
					 function );

					goto on_error;
				}
				*media_size = (size64_t) value_64bit;
			}
		}
	}
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
	if( libfvalue_split_utf8_string_free(
	     &types,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free split types.",
		 function );

		goto on_error;
	}
	*line_index = safe_line_index;

	return( 1 );

on_error:
	if( values != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &values,
		 NULL );
	}
	if( types != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &types,
		 NULL );
	}
	return( -1 );
}

/* Parses the "perm" category
 * Returns 1 if successful or -1 on error
 */
int libewf_single_files_parse_perm_category(
     libewf_single_files_t *single_files,
     libfvalue_split_utf8_string_t *lines,
     int *line_index,
     libcerror_error_t **error )
{
	libewf_lef_permission_t *lef_permission     = NULL;
	libewf_permission_group_t *permission_group = NULL;
	libfvalue_split_utf8_string_t *types        = NULL;
	uint8_t *line_string                        = NULL;
	static char *function                       = "libewf_single_files_parse_perm_category";
	size_t line_string_size                     = 0;
	int copy_of_number_of_permission_groups     = 0;
	int entry_index                             = 0;
	int number_of_permission_groups             = 0;
	int permission_group_index                  = 0;
	int safe_line_index                         = 0;

	if( single_files == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single files.",
		 function );

		return( -1 );
	}
	if( line_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid line index.",
		 function );

		return( -1 );
	}
	safe_line_index = *line_index;

	if( libewf_single_files_parse_line(
	     lines,
	     safe_line_index,
	     &line_string,
	     &line_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve line string: %d.",
		 function,
		 safe_line_index );

		goto on_error;
	}
	safe_line_index += 1;

	if( ( line_string_size != 5 )
	 || ( line_string[ 0 ] != (uint8_t) 'p' )
	 || ( line_string[ 1 ] != (uint8_t) 'e' )
	 || ( line_string[ 2 ] != (uint8_t) 'r' )
	 || ( line_string[ 3 ] != (uint8_t) 'm' ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported category header.",
		 function );

		goto on_error;
	}
	if( libewf_single_files_parse_category_number_of_entries(
	     lines,
	     &safe_line_index,
	     &number_of_permission_groups,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse number of permission groups in line: %d.",
		 function,
		 safe_line_index );

		goto on_error;
	}
	if( libewf_single_files_parse_category_types(
	     lines,
	     &safe_line_index,
	     &types,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse types in line: %d.",
		 function,
		 safe_line_index );

		goto on_error;
	}
	if( libewf_single_files_parse_number_of_entries(
	     lines,
	     &safe_line_index,
	     &copy_of_number_of_permission_groups,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse number of permission groups in line: %d.",
		 function,
		 safe_line_index );

		goto on_error;
	}
	if( number_of_permission_groups != copy_of_number_of_permission_groups )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: mismatch between number of permission groups and copy.",
		 function );

		goto on_error;
	}
	/* Parse the category root entry
	 */
	if( libewf_single_files_parse_line(
	     lines,
	     safe_line_index,
	     &line_string,
	     &line_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve line string: %d.",
		 function,
		 safe_line_index );

		goto on_error;
	}
	if( libewf_lef_permission_initialize(
	     &lef_permission,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create permission.",
		 function );

		goto on_error;
	}
	if( libewf_lef_permission_read_data(
	     lef_permission,
	     types,
	     line_string,
	     line_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read permission",
		 function );

		goto on_error;
	}
	safe_line_index += 1;

	if( lef_permission->property_type != 10 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported permission group type.",
		 function );

		goto on_error;
	}
	if( libewf_lef_permission_free(
	     &lef_permission,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to free permission.",
		 function );

		goto on_error;
	}
	for( permission_group_index = 0;
	     permission_group_index < number_of_permission_groups;
	     permission_group_index++ )
	{
		if( libewf_permission_group_initialize(
		     &permission_group,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create permission group.",
			 function );

			goto on_error;
		}
		if( libewf_single_files_parse_permission_group(
		     single_files,
		     types,
		     lines,
		     &safe_line_index,
		     permission_group,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to parse permission group: %d.",
			 function,
			 permission_group_index );

			goto on_error;
		}
		if( libcdata_array_append_entry(
		     single_files->permission_groups,
		     &entry_index,
		     (intptr_t *) permission_group,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append permission group: %d to array.",
			 function,
			 permission_group_index );

			goto on_error;
		}
		permission_group = NULL;
	}
	/* The category should be followed by an empty line
	 */
	if( libewf_single_files_parse_line(
	     lines,
	     safe_line_index,
	     &line_string,
	     &line_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve line string: %d.",
		 function,
		 safe_line_index );

		goto on_error;
	}
	if( ( line_string_size != 1 )
	 || ( line_string[ 0 ] != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported empty line string: %d - not empty.",
		 function,
		 safe_line_index );

		goto on_error;
	}
	safe_line_index += 1;

	if( libfvalue_split_utf8_string_free(
	     &types,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free split types.",
		 function );

		goto on_error;
	}
	*line_index = safe_line_index;

	return( 1 );

on_error:
	if( permission_group != NULL )
	{
		libewf_permission_group_free(
		 &permission_group,
		 NULL );
	}
	if( lef_permission != NULL )
	{
		libewf_lef_permission_free(
		 &lef_permission,
		 NULL );
	}
	if( types != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &types,
		 NULL );
	}
	return( -1 );
}

/* Parses a permission group
 * Returns 1 if successful or -1 on error
 */
int libewf_single_files_parse_permission_group(
     libewf_single_files_t *single_files,
     libfvalue_split_utf8_string_t *types,
     libfvalue_split_utf8_string_t *lines,
     int *line_index,
     libewf_permission_group_t *permission_group,
     libcerror_error_t **error )
{
	libewf_lef_permission_t *lef_permission = NULL;
	uint8_t *line_string                    = NULL;
	static char *function                   = "libewf_single_files_parse_permission_group";
	size_t line_string_size                 = 0;
	int number_of_entries                   = 0;
	int number_of_permissions               = 0;
	int permission_index                    = 0;
	int safe_line_index                     = 0;

	if( single_files == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single files.",
		 function );

		return( -1 );
	}
	if( line_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid line index.",
		 function );

		return( -1 );
	}
	safe_line_index = *line_index;

	if( libewf_single_files_parse_number_of_entries(
	     lines,
	     &safe_line_index,
	     &number_of_permissions,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse number of permissions in line: %d.",
		 function,
		 safe_line_index );

		goto on_error;
	}
	/* Parse the permission group entry
	 */
	if( libewf_single_files_parse_line(
	     lines,
	     safe_line_index,
	     &line_string,
	     &line_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve line string: %d.",
		 function,
		 safe_line_index );

		goto on_error;
	}
	if( libewf_lef_permission_initialize(
	     &lef_permission,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create permission.",
		 function );

		goto on_error;
	}
	if( libewf_lef_permission_read_data(
	     lef_permission,
	     types,
	     line_string,
	     line_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read permission",
		 function );

		goto on_error;
	}
	safe_line_index += 1;

	if( lef_permission->property_type != 10 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported permission group type.",
		 function );

		goto on_error;
	}
	if( libewf_lef_permission_free(
	     &lef_permission,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to free permission.",
		 function );

		goto on_error;
	}
	for( permission_index = 0;
	     permission_index < number_of_permissions;
	     permission_index++ )
	{
		if( libewf_single_files_parse_number_of_entries(
		     lines,
		     &safe_line_index,
		     &number_of_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to parse number of entries in line: %d.",
			 function,
			 safe_line_index );

			goto on_error;
		}
		if( number_of_entries != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid number of entries value out of bounds.",
			 function );

			goto on_error;
		}
		/* Parse the permission entry
		 */
		if( libewf_single_files_parse_line(
		     lines,
		     safe_line_index,
		     &line_string,
		     &line_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve line string: %d.",
			 function,
			 safe_line_index );

			goto on_error;
		}
		if( libewf_lef_permission_initialize(
		     &lef_permission,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create permission: %d.",
			 function,
			 permission_index );

			goto on_error;
		}
		if( libewf_lef_permission_read_data(
		     lef_permission,
		     types,
		     line_string,
		     line_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read permission: %d",
			 function,
			 permission_index );

			goto on_error;
		}
		safe_line_index += 1;

		if( libewf_permission_group_append_permission(
		     permission_group,
		     lef_permission,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append permission: %d to group.",
			 function,
			 permission_index );

			goto on_error;
		}
		lef_permission = NULL;
	}
	*line_index = safe_line_index;

	return( 1 );

on_error:
	if( lef_permission != NULL )
	{
		libewf_lef_permission_free(
		 &lef_permission,
		 NULL );
	}
	return( -1 );
}

/* Parses the "srce" category
 * Returns 1 if successful or -1 on error
 */
int libewf_single_files_parse_srce_category(
     libewf_single_files_t *single_files,
     libfvalue_split_utf8_string_t *lines,
     int *line_index,
     libcerror_error_t **error )
{
	libewf_lef_source_t *lef_source      = NULL;
	libfvalue_split_utf8_string_t *types = NULL;
	uint8_t *line_string                 = NULL;
	static char *function                = "libewf_single_files_parse_srce_category";
	size_t line_string_size              = 0;
	int copy_of_number_of_sources        = 0;
	int entry_index                      = 0;
	int number_of_entries                = 0;
	int number_of_sources                = 0;
	int safe_line_index                  = 0;
	int source_identifier                = 0;
	int source_index                     = 0;

	if( single_files == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single files.",
		 function );

		return( -1 );
	}
	if( line_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid line index.",
		 function );

		return( -1 );
	}
	safe_line_index = *line_index;

	if( libewf_single_files_parse_line(
	     lines,
	     safe_line_index,
	     &line_string,
	     &line_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve line string: %d.",
		 function,
		 safe_line_index );

		goto on_error;
	}
	safe_line_index += 1;

	if( ( line_string_size != 5 )
	 || ( line_string[ 0 ] != (uint8_t) 's' )
	 || ( line_string[ 1 ] != (uint8_t) 'r' )
	 || ( line_string[ 2 ] != (uint8_t) 'c' )
	 || ( line_string[ 3 ] != (uint8_t) 'e' ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported category header.",
		 function );

		goto on_error;
	}
	if( libewf_single_files_parse_category_number_of_entries(
	     lines,
	     &safe_line_index,
	     &number_of_sources,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse number of sources in line: %d.",
		 function,
		 safe_line_index );

		goto on_error;
	}
	if( libewf_single_files_parse_category_types(
	     lines,
	     &safe_line_index,
	     &types,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse types in line: %d.",
		 function,
		 safe_line_index );

		goto on_error;
	}
	if( libewf_single_files_parse_number_of_entries(
	     lines,
	     &safe_line_index,
	     &copy_of_number_of_sources,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse number of sources in line: %d.",
		 function,
		 safe_line_index );

		goto on_error;
	}
	if( number_of_sources != copy_of_number_of_sources )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: mismatch between number of sources and copy.",
		 function );

		goto on_error;
	}
	/* Parse the category root entry
	 */
	if( libewf_single_files_parse_line(
	     lines,
	     safe_line_index,
	     &line_string,
	     &line_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve line string: %d.",
		 function,
		 safe_line_index );

		goto on_error;
	}
	if( libewf_lef_source_initialize(
	     &lef_source,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create source.",
		 function );

		goto on_error;
	}
	if( libewf_lef_source_read_data(
	     lef_source,
	     types,
	     line_string,
	     line_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read source",
		 function );

		goto on_error;
	}
	safe_line_index += 1;

	/* Append the category root so that the source identifiers and array entries align
	 */
	if( libcdata_array_append_entry(
	     single_files->sources,
	     &entry_index,
	     (intptr_t *) lef_source,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append category root source to array.",
		 function );

		goto on_error;
	}
	lef_source = NULL;

	for( source_index = 0;
	     source_index < number_of_sources;
	     source_index++ )
	{
		if( libewf_single_files_parse_number_of_entries(
		     lines,
		     &safe_line_index,
		     &number_of_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to parse number of entries in line: %d.",
			 function,
			 safe_line_index );

			goto on_error;
		}
		if( number_of_entries != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid number of entries value out of bounds.",
			 function );

			goto on_error;
		}
		/* Parse the source entry
		 */
		if( libewf_single_files_parse_line(
		     lines,
		     safe_line_index,
		     &line_string,
		     &line_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve line string: %d.",
			 function,
			 safe_line_index );

			goto on_error;
		}
		if( libewf_lef_source_initialize(
		     &lef_source,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create source: %d.",
			 function,
			 source_index );

			goto on_error;
		}
		if( libewf_lef_source_read_data(
		     lef_source,
		     types,
		     line_string,
		     line_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read source: %d",
			 function,
			 source_index );

			goto on_error;
		}
		safe_line_index += 1;

		if( libewf_lef_source_get_identifier(
		     lef_source,
		     &source_identifier,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve source: %d identifier.",
			 function,
			 safe_line_index );

			goto on_error;
		}
		if( source_identifier != ( source_index + 1 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: mismatch between source identifier and index.",
			 function );

			goto on_error;
		}
		if( libcdata_array_append_entry(
		     single_files->sources,
		     &entry_index,
		     (intptr_t *) lef_source,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append source: %d to array.",
			 function,
			 source_index );

			goto on_error;
		}
		lef_source = NULL;
	}
	/* The category should be followed by an empty line
	 */
	if( libewf_single_files_parse_line(
	     lines,
	     safe_line_index,
	     &line_string,
	     &line_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve line string: %d.",
		 function,
		 safe_line_index );

		goto on_error;
	}
	if( ( line_string_size != 1 )
	 || ( line_string[ 0 ] != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported empty line string: %d - not empty.",
		 function,
		 safe_line_index );

		goto on_error;
	}
	safe_line_index += 1;

	if( libfvalue_split_utf8_string_free(
	     &types,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free split types.",
		 function );

		goto on_error;
	}
	*line_index = safe_line_index;

	return( 1 );

on_error:
	if( lef_source != NULL )
	{
		libewf_lef_source_free(
		 &lef_source,
		 NULL );
	}
	if( types != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &types,
		 NULL );
	}
	return( -1 );
}

/* Parses the "sub" category
 * Returns 1 if successful or -1 on error
 */
int libewf_single_files_parse_sub_category(
     libewf_single_files_t *single_files,
     libfvalue_split_utf8_string_t *lines,
     int *line_index,
     libcerror_error_t **error )
{
	libewf_lef_subject_t *lef_subject    = NULL;
	libfvalue_split_utf8_string_t *types = NULL;
	uint8_t *line_string                 = NULL;
	static char *function                = "libewf_single_files_parse_sub_category";
	size_t line_string_size              = 0;
	int copy_of_number_of_subjects       = 0;
	int number_of_entries                = 0;
	int number_of_subjects               = 0;
	int safe_line_index                  = 0;
	int subject_index                    = 0;

	if( single_files == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single files.",
		 function );

		return( -1 );
	}
	if( line_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid line index.",
		 function );

		return( -1 );
	}
	safe_line_index = *line_index;

	if( libewf_single_files_parse_line(
	     lines,
	     safe_line_index,
	     &line_string,
	     &line_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve line string: %d.",
		 function,
		 safe_line_index );

		goto on_error;
	}
	safe_line_index += 1;

	if( ( line_string_size != 4 )
	 || ( line_string[ 0 ] != (uint8_t) 's' )
	 || ( line_string[ 1 ] != (uint8_t) 'u' )
	 || ( line_string[ 2 ] != (uint8_t) 'b' ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported category header.",
		 function );

		goto on_error;
	}
	if( libewf_single_files_parse_category_number_of_entries(
	     lines,
	     &safe_line_index,
	     &number_of_subjects,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse number of subjects in line: %d.",
		 function,
		 safe_line_index );

		goto on_error;
	}
	if( libewf_single_files_parse_category_types(
	     lines,
	     &safe_line_index,
	     &types,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse types in line: %d.",
		 function,
		 safe_line_index );

		goto on_error;
	}
	if( libewf_single_files_parse_number_of_entries(
	     lines,
	     &safe_line_index,
	     &copy_of_number_of_subjects,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse number of subjects in line: %d.",
		 function,
		 safe_line_index );

		goto on_error;
	}
	if( number_of_subjects != copy_of_number_of_subjects )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: mismatch between number of subjects and copy.",
		 function );

		goto on_error;
	}
	/* Parse the category root entry
	 */
	if( libewf_single_files_parse_line(
	     lines,
	     safe_line_index,
	     &line_string,
	     &line_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve line string: %d.",
		 function,
		 safe_line_index );

		goto on_error;
	}
	if( libewf_lef_subject_initialize(
	     &lef_subject,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create subject.",
		 function );

		goto on_error;
	}
	if( libewf_lef_subject_read_data(
	     lef_subject,
	     types,
	     line_string,
	     line_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read subject",
		 function );

		goto on_error;
	}
	safe_line_index += 1;

/* TODO */

	if( libewf_lef_subject_free(
	     &lef_subject,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to free subject.",
		 function );

		goto on_error;
	}
	for( subject_index = 0;
	     subject_index < number_of_subjects;
	     subject_index++ )
	{
		if( libewf_single_files_parse_number_of_entries(
		     lines,
		     &safe_line_index,
		     &number_of_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to parse number of entries in line: %d.",
			 function,
			 safe_line_index );

			goto on_error;
		}
		if( number_of_entries != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid number of entries value out of bounds.",
			 function );

			goto on_error;
		}
		/* Parse the subject entry
		 */
		if( libewf_single_files_parse_line(
		     lines,
		     safe_line_index,
		     &line_string,
		     &line_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve line string: %d.",
			 function,
			 safe_line_index );

			goto on_error;
		}
		if( libewf_lef_subject_initialize(
		     &lef_subject,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create subject: %d.",
			 function,
			 subject_index );

			goto on_error;
		}
		if( libewf_lef_subject_read_data(
		     lef_subject,
		     types,
		     line_string,
		     line_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read subject: %d",
			 function,
			 subject_index );

			goto on_error;
		}
		safe_line_index += 1;

/* TODO implement append subject to array */

		if( libewf_lef_subject_free(
		     &lef_subject,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to free subject.",
			 function );

			goto on_error;
		}
	}
	/* The category should be followed by an empty line
	 */
	if( libewf_single_files_parse_line(
	     lines,
	     safe_line_index,
	     &line_string,
	     &line_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve line string: %d.",
		 function,
		 safe_line_index );

		goto on_error;
	}
	if( ( line_string_size != 1 )
	 || ( line_string[ 0 ] != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported empty line string: %d - not empty.",
		 function,
		 safe_line_index );

		goto on_error;
	}
	safe_line_index += 1;

	if( libfvalue_split_utf8_string_free(
	     &types,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free split types.",
		 function );

		goto on_error;
	}
	*line_index = safe_line_index;

	return( 1 );

on_error:
	if( lef_subject != NULL )
	{
		libewf_lef_subject_free(
		 &lef_subject,
		 NULL );
	}
	if( types != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &types,
		 NULL );
	}
	return( -1 );
}

/* Parses the "entry" category
 * Returns 1 if successful or -1 on error
 */
int libewf_single_files_parse_entry_category(
     libewf_single_files_t *single_files,
     libfvalue_split_utf8_string_t *lines,
     int *line_index,
     uint8_t *format,
     libcerror_error_t **error )
{
	libfvalue_split_utf8_string_t *types = NULL;
	uint8_t *line_string                 = NULL;
	static char *function                = "libewf_single_files_parse_entry_category";
	size_t line_string_size              = 0;
	int number_of_sub_entries            = 0;
	int safe_line_index                  = 0;

	if( single_files == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single files.",
		 function );

		return( -1 );
	}
	if( single_files->file_entry_tree_root_node != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid single files - file entry tree root node value already set.",
		 function );

		return( -1 );
	}
	if( line_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid line index.",
		 function );

		return( -1 );
	}
	safe_line_index = *line_index;

	if( libewf_single_files_parse_line(
	     lines,
	     safe_line_index,
	     &line_string,
	     &line_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve line string: %d.",
		 function,
		 safe_line_index );

		goto on_error;
	}
	safe_line_index += 1;

	if( ( line_string_size != 6 )
	 || ( line_string[ 0 ] != (uint8_t) 'e' )
	 || ( line_string[ 1 ] != (uint8_t) 'n' )
	 || ( line_string[ 2 ] != (uint8_t) 't' )
	 || ( line_string[ 3 ] != (uint8_t) 'r' )
	 || ( line_string[ 4 ] != (uint8_t) 'y' ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported category header.",
		 function );

		goto on_error;
	}
	if( libewf_single_files_parse_category_number_of_entries(
	     lines,
	     &safe_line_index,
	     &number_of_sub_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse number of entries in line: %d.",
		 function,
		 safe_line_index );

		goto on_error;
	}
	if( libewf_single_files_parse_category_types(
	     lines,
	     &safe_line_index,
	     &types,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse types in line: %d.",
		 function,
		 safe_line_index );

		goto on_error;
	}
	if( libewf_single_files_parse_format(
	     types,
	     format,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse format.",
		 function );

		goto on_error;
	}
	if( libcdata_tree_node_initialize(
	     &( single_files->file_entry_tree_root_node ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file entry tree root node.",
		 function );

		goto on_error;
	}
	if( libewf_single_files_parse_file_entry(
	     single_files->file_entry_tree_root_node,
	     types,
	     lines,
	     &safe_line_index,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse file entry.",
		 function );

		goto on_error;
	}
	/* The category should be followed by an empty line
	 */
	if( libewf_single_files_parse_line(
	     lines,
	     safe_line_index,
	     &line_string,
	     &line_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve line string: %d.",
		 function,
		 safe_line_index );

		goto on_error;
	}
	if( ( line_string_size != 1 )
	 || ( line_string[ 0 ] != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported empty line string: %d - not empty.",
		 function,
		 safe_line_index );

		goto on_error;
	}
	safe_line_index += 1;

	if( libfvalue_split_utf8_string_free(
	     &types,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free split types.",
		 function );

		goto on_error;
	}
	*line_index = safe_line_index;

	return( 1 );

on_error:
	if( single_files->file_entry_tree_root_node != NULL )
	{
		libcdata_tree_node_free(
		 &( single_files->file_entry_tree_root_node ),
		 (int (*)(intptr_t **, libcerror_error_t **)) &libewf_lef_file_entry_free,
		 NULL );
	}
	if( types != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &types,
		 NULL );
	}
	return( -1 );
}

/* Parses a file entry string for the values
 * Returns 1 if successful or -1 on error
 */
int libewf_single_files_parse_file_entry(
     libcdata_tree_node_t *parent_file_entry_node,
     libfvalue_split_utf8_string_t *types,
     libfvalue_split_utf8_string_t *lines,
     int *line_index,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *file_entry_node   = NULL;
	libewf_lef_file_entry_t *lef_file_entry = NULL;
	libfvalue_split_utf8_string_t *values   = NULL;
	uint8_t *line_string                    = NULL;
	static char *function                   = "libewf_single_files_parse_file_entry";
	size_t line_string_size                 = 0;
	int number_of_lines                     = 0;
	int number_of_sub_entries               = 0;
	int safe_line_index                     = 0;
	int sub_entry_index                     = 0;

	if( parent_file_entry_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid parent file entry node.",
		 function );

		return( -1 );
	}
	if( line_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid line index.",
		 function );

		return( -1 );
	}
	safe_line_index = *line_index;

	if( libewf_single_files_parse_file_entry_number_of_sub_entries(
	     lines,
	     &safe_line_index,
	     &number_of_sub_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse file entry number of sub entries.",
		 function );

		goto on_error;
	}
	if( libewf_single_files_parse_line(
	     lines,
	     safe_line_index,
	     &line_string,
	     &line_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve line string: %d.",
		 function,
		 safe_line_index );

		goto on_error;
	}
	safe_line_index += 1;

	if( libewf_lef_file_entry_initialize(
	     &lef_file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file entry.",
		 function );

		goto on_error;
	}
	if( libewf_lef_file_entry_read_data(
	     lef_file_entry,
	     types,
	     line_string,
	     line_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file entry",
		 function );

		goto on_error;
	}
	if( libcdata_tree_node_set_value(
	     parent_file_entry_node,
	     (intptr_t *) lef_file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set file entry in node.",
		 function );

		goto on_error;
	}
	lef_file_entry = NULL;

	if( libfvalue_split_utf8_string_get_number_of_segments(
	     lines,
	     &number_of_lines,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of lines",
		 function );

		goto on_error;
	}
	if( ( safe_line_index > number_of_lines )
	 || ( number_of_sub_entries > ( number_of_lines - safe_line_index ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: number of sub entries exceed the number of available lines.",
		 function );

		goto on_error;
	}
	for( sub_entry_index = 0;
	     sub_entry_index < number_of_sub_entries;
	     sub_entry_index++ )
	{
		if( libcdata_tree_node_initialize(
		     &file_entry_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create sub file entry: %d node.",
			 function,
			 sub_entry_index );

			goto on_error;
		}
		if( libewf_single_files_parse_file_entry(
		     file_entry_node,
		     types,
		     lines,
		     &safe_line_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to parse sub file entry: %d.",
			 function,
			 sub_entry_index );

			goto on_error;
		}
		if( libcdata_tree_node_append_node(
		     parent_file_entry_node,
		     file_entry_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append sub file entry: %d node to parent.",
			 function,
			 sub_entry_index );

			goto on_error;
		}
		file_entry_node = NULL;
	}
	*line_index = safe_line_index;

	return( 1 );

on_error:
	if( file_entry_node != NULL )
	{
		libcdata_tree_node_free(
		 &file_entry_node,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libewf_lef_file_entry_free,
		 NULL );
	}
	if( lef_file_entry != NULL )
	{
		libewf_lef_file_entry_free(
		 &lef_file_entry,
		 NULL );
	}
	if( values != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &values,
		 NULL );
	}
	return( -1 );
}

/* Parses a file entry string for the number of sub entries
 * Returns 1 if successful or -1 on error
 */
int libewf_single_files_parse_file_entry_number_of_sub_entries(
     libfvalue_split_utf8_string_t *lines,
     int *line_index,
     int *number_of_sub_entries,
     libcerror_error_t **error )
{
	libfvalue_split_utf8_string_t *values = NULL;
	uint8_t *line_string                  = NULL;
	uint8_t *value_string                 = NULL;
	static char *function                 = "libewf_single_files_parse_file_entry_number_of_sub_entries";
	size_t line_string_size               = 0;
	size_t value_string_size              = 0;
	uint64_t value_64bit                  = 0;
	int number_of_values                  = 0;
	int safe_line_index                   = 0;

	if( line_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid line index.",
		 function );

		return( -1 );
	}
	if( number_of_sub_entries == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of sub entries.",
		 function );

		return( -1 );
	}
	safe_line_index = *line_index;

	if( libewf_single_files_parse_line(
	     lines,
	     safe_line_index,
	     &line_string,
	     &line_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve line string: %d.",
		 function,
		 safe_line_index );

		goto on_error;
	}
	safe_line_index += 1;

	if( libfvalue_utf8_string_split(
	     line_string,
	     line_string_size,
	     (uint8_t) '\t',
	     &values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split line string into values.",
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
	if( number_of_values != 2 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported number of values.",
		 function );

		goto on_error;
	}
	if( libfvalue_split_utf8_string_get_segment_by_index(
	     values,
	     0,
	     &value_string,
	     &value_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value string: 0.",
		 function );

		goto on_error;
	}
	if( libfvalue_split_utf8_string_get_segment_by_index(
	     values,
	     0,
	     &value_string,
	     &value_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value string: 0.",
		 function );

		goto on_error;
	}
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
		 "%s: unable to set number of entries in parent value.",
		 function );

		goto on_error;
	}
	if( value_64bit > (uint64_t) INT_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of entries in parent value out of bounds.",
		 function );

		goto on_error;
	}
	if( ( value_64bit != 0 )
	 && ( value_64bit != 26 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported number of entries in parent value.",
		 function );

		goto on_error;
	}
	if( libfvalue_split_utf8_string_get_segment_by_index(
	     values,
	     1,
	     &value_string,
	     &value_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value string: 1.",
		 function );

		goto on_error;
	}
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
		 "%s: unable to set number of sub entries.",
		 function );

		goto on_error;
	}
	if( value_64bit > (uint64_t) INT_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of sub entries value out of bounds.",
		 function );

		goto on_error;
	}
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
	*line_index            = safe_line_index;
	*number_of_sub_entries = (int) value_64bit;

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

/* Parses an UTF-8 encoded single files string
 * Returns 1 if successful or -1 on error
 */
int libewf_single_files_parse_utf8_string(
     libewf_single_files_t *single_files,
     const uint8_t *utf8_string,
     size_t utf8_string_size,
     size64_t *media_size,
     uint8_t *format,
     libcerror_error_t **error )
{
	libfvalue_split_utf8_string_t *lines = NULL;
	uint8_t *line_string                 = NULL;
	static char *function                = "libewf_single_files_parse_utf8_string";
	size_t line_string_size              = 0;
	int line_index                       = 0;

	if( single_files == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single files.",
		 function );

		return( -1 );
	}
	if( libfvalue_utf8_string_split(
	     utf8_string,
	     utf8_string_size,
	     (uint8_t) '\n',
	     &lines,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split string into lines.",
		 function );

		goto on_error;
	}
	if( libewf_single_files_parse_line(
	     lines,
	     line_index,
	     &line_string,
	     &line_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve line string: %d.",
		 function,
		 line_index );

		goto on_error;
	}
	if( ( line_string_size != 2 )
	 || ( line_string[ 0 ] != (uint8_t) '5' ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported number of categories string.",
		 function );

		goto on_error;
	}
	line_index += 1;

	if( libewf_single_files_parse_rec_category(
	     single_files,
	     lines,
	     &line_index,
	     media_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse rec category.",
		 function );

		goto on_error;
	}
	if( libewf_single_files_parse_perm_category(
	     single_files,
	     lines,
	     &line_index,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse perm category.",
		 function );

		goto on_error;
	}
	if( libewf_single_files_parse_srce_category(
	     single_files,
	     lines,
	     &line_index,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse srce category.",
		 function );

		goto on_error;
	}
	if( libewf_single_files_parse_sub_category(
	     single_files,
	     lines,
	     &line_index,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse sub category.",
		 function );

		goto on_error;
	}
	if( libewf_single_files_parse_entry_category(
	     single_files,
	     lines,
	     &line_index,
	     format,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse entry category.",
		 function );

		goto on_error;
	}
	if( libfvalue_split_utf8_string_free(
	     &lines,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free split lines.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( lines != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &lines,
		 NULL );
	}
	return( -1 );
}

/* Reads the single files
 * Returns 1 if successful or -1 on error
 */
int libewf_single_files_read_data(
     libewf_single_files_t *single_files,
     const uint8_t *data,
     size_t data_size,
     size64_t *media_size,
     uint8_t *format,
     libcerror_error_t **error )
{
	uint8_t *utf8_string    = NULL;
	static char *function   = "libewf_single_files_read_data";
	size_t utf8_string_size = 0;

	if( single_files == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single files.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_size_from_utf16_stream(
	     data,
	     data_size,
	     LIBUNA_ENDIAN_LITTLE,
	     &utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine UTF-8 string size.",
		 function );

		goto on_error;
	}
	if( ( utf8_string_size == 0 )
	 || ( utf8_string_size > MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid UTF-8 string size value out of bounds.",
		 function );

		goto on_error;
	}
	utf8_string = (uint8_t *) memory_allocate(
	                           sizeof( uint8_t ) * (size_t) utf8_string_size );

	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create UTF-8 string.",
		 function );

		goto on_error;
	}
	if( libuna_utf8_string_copy_from_utf16_stream(
	     utf8_string,
	     utf8_string_size,
	     data,
	     data_size,
	     LIBUNA_ENDIAN_LITTLE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set UTF-8 string.",
		 function );

		goto on_error;
	}
	if( libewf_single_files_parse_utf8_string(
	     single_files,
	     utf8_string,
	     utf8_string_size,
	     media_size,
	     format,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse UTF-8 string.",
		 function );

		goto on_error;
	}
	memory_free(
	 utf8_string );

	return( 1 );

on_error:
	if( utf8_string != NULL )
	{
		memory_free(
		 utf8_string );
	}
	return( -1 );
}

/* Retrieves the file entry tree root node
 * Returns 1 if successful or -1 on error
 */
int libewf_single_files_get_file_entry_tree_root_node(
     libewf_single_files_t *single_files,
     libcdata_tree_node_t **root_node,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_files_get_file_entry_tree_root_node";

	if( single_files == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single files.",
		 function );

		return( -1 );
	}
	if( root_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid root node.",
		 function );

		return( -1 );
	}
	*root_node = single_files->file_entry_tree_root_node;

	return( 1 );
}

/* Retrieves a specific permission group
 * Returns 1 if successful or -1 on error
 */
int libewf_single_files_get_permission_group_by_index(
     libewf_single_files_t *single_files,
     int permission_group_index,
     libewf_permission_group_t **permission_group,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_files_get_permission_group_by_index";

	if( single_files == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single files.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     single_files->permission_groups,
	     permission_group_index,
	     (intptr_t **) permission_group,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry: %d from permission groups array.",
		 function,
		 permission_group_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific source
 * Returns 1 if successful or -1 on error
 */
int libewf_single_files_get_source_by_index(
     libewf_single_files_t *single_files,
     int source_index,
     libewf_lef_source_t **lef_source,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_files_get_source_by_index";

	if( single_files == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single files.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     single_files->sources,
	     source_index,
	     (intptr_t **) lef_source,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry: %d from sources array.",
		 function,
		 source_index );

		return( -1 );
	}
	return( 1 );
}

