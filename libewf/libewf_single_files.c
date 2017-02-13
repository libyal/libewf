/*
 * Single file entries functions
 *
 * Copyright (C) 2006-2017, Joachim Metz <joachim.metz@gmail.com>
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
#include <narrow_string.h>
#include <types.h>

#include "libewf_definitions.h"
#include "libewf_libcdata.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libfvalue.h"
#include "libewf_libuna.h"
#include "libewf_single_file_entry.h"
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

		goto on_error;
	}
	return( 1 );

on_error:
	if( *single_files != NULL )
	{
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
		if( ( *single_files )->section_data != NULL )
		{
			memory_free(
			 ( *single_files )->section_data );
		}
		if( ( *single_files )->root_file_entry_node != NULL )
		{
			if( libcdata_tree_node_free(
			     &( ( *single_files )->root_file_entry_node ),
			     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_single_file_entry_free,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free single file entry tree.",
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

/* Parse an EWF ltree for the values
 * Returns 1 if successful or -1 on error
 */
int libewf_single_files_parse(
     libewf_single_files_t *single_files,
     size64_t *media_size,
     uint8_t *format,
     libcerror_error_t **error )
{
	uint8_t *file_entries_string    = NULL;
	static char *function           = "libewf_single_files_parse";
	size_t file_entries_string_size = 0;

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
	if( single_files->ltree_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid single files - missing ltree data.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_size_from_utf16_stream(
	     single_files->ltree_data,
	     single_files->ltree_data_size,
	     LIBUNA_ENDIAN_LITTLE,
	     &file_entries_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine file entries string size.",
		 function );

		return( -1 );
	}
	file_entries_string = (uint8_t *) memory_allocate(
	                                   sizeof( uint8_t ) * (size_t) file_entries_string_size );

	if( file_entries_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file entries string.",
		 function );

		goto on_error;
	}
	if( libuna_utf8_string_copy_from_utf16_stream(
	     file_entries_string,
	     file_entries_string_size,
	     single_files->ltree_data,
	     single_files->ltree_data_size,
	     LIBUNA_ENDIAN_LITTLE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set file entries string.",
		 function );

		goto on_error;
	}
	if( libewf_single_files_parse_file_entries(
	     single_files,
	     media_size,
	     file_entries_string,
	     file_entries_string_size,
	     format,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse file entries string.",
		 function );

		goto on_error;
	}
	memory_free(
	 file_entries_string );

	return( 1 );

on_error:
	if( file_entries_string != NULL )
	{
		memory_free(
		 file_entries_string );
	}
	return( -1 );
}

/* Parse a single file entries string for the values
 * Returns 1 if successful or -1 on error
 */
int libewf_single_files_parse_file_entries(
     libewf_single_files_t *single_files,
     size64_t *media_size,
     const uint8_t *entries_string,
     size_t entries_string_size,
     uint8_t *format,
     libcerror_error_t **error )
{
	libfvalue_split_utf8_string_t *lines = NULL;
	libfvalue_split_utf8_string_t *types = NULL;
	uint8_t *line_string                 = NULL;
	static char *function                = "libewf_single_files_parse_file_entries";
	size_t line_string_size              = 0;
	int line_index                       = 0;
	int number_of_lines                  = 0;

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
	if( entries_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entries string.",
		 function );

		return( -1 );
	}
	if( libfvalue_utf8_string_split(
	     entries_string,
	     entries_string_size - 1,
	     (uint8_t) '\n',
	     &lines,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split entries string into lines.",
		 function );

		goto on_error;
	}
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
	if( number_of_lines > 0 )
	{
		if( libfvalue_split_utf8_string_get_segment_by_index(
		     lines,
		     0,
		     &line_string,
		     &line_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve line string: 0.",
			 function );

			goto on_error;
		}
		if( ( line_string == NULL )
		 || ( line_string_size < 2 )
		 || ( line_string[ 0 ] == 0 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing line string: 0.",
			 function );

			goto on_error;
		}
		/* Remove trailing carriage return
		 */
		else if( line_string[ line_string_size - 2 ] == (uint8_t) '\r' )
		{
			line_string[ line_string_size - 2 ] = 0;

			line_string_size -= 1;
		}
		if( line_string_size != 2 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported single file entries string.",
			 function );

			goto on_error;
		}
		if( ( line_string[ 0 ] < (uint8_t) '0' )
		 || ( line_string[ 0 ] > (uint8_t) '9' ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported single file entries string.",
			 function );

			goto on_error;
		}
		/* Find the line containing: "rec"
		 */
		for( line_index = 0;
		     line_index < number_of_lines;
		     line_index++ )
		{
			if( libfvalue_split_utf8_string_get_segment_by_index(
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
			if( line_string_size == 4 )
			{
				if( line_string == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: missing line string: %d.",
					 function,
					 line_index );

					goto on_error;
				}
				if( ( line_string[ 0 ] == (uint8_t) 'r' )
				 && ( line_string[ 1 ] == (uint8_t) 'e' )
				 && ( line_string[ 2 ] == (uint8_t) 'c' ) )
				{
					line_index += 1;

					break;
				}
			}
		}
		if( libewf_single_files_parse_record_values(
		     media_size,
		     lines,
		     &line_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to parse record values.",
			 function );

			goto on_error;
		}
		/* Find the line containing: "entry"
		 */
		for( line_index = 0;
		     line_index < number_of_lines;
		     line_index++ )
		{
			if( libfvalue_split_utf8_string_get_segment_by_index(
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
			if( line_string_size == 6 )
			{
				if( line_string == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: missing line string: %d.",
					 function,
					 line_index );

					goto on_error;
				}
				if( ( line_string[ 0 ] == (uint8_t) 'e' )
				 && ( line_string[ 1 ] == (uint8_t) 'n' )
				 && ( line_string[ 2 ] == (uint8_t) 't' )
				 && ( line_string[ 3 ] == (uint8_t) 'r' )
				 && ( line_string[ 4 ] == (uint8_t) 'y' ) )
				{
					line_index += 2;

					break;
				}
			}
		}
		if( line_index < number_of_lines )
		{
			if( libfvalue_split_utf8_string_get_segment_by_index(
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
			line_index += 1;

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
				 "%s: unable to split entries string into types.",
				 function );

				goto on_error;
			}
			if( libcdata_tree_node_initialize(
			     &( single_files->root_file_entry_node ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create root single file entry node.",
				 function );

				goto on_error;
			}
			if( libewf_single_files_parse_file_entry(
			     single_files->root_file_entry_node,
			     lines,
			     &line_index,
			     types,
			     format,
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
			/* The single files entries should be followed by an empty line
			 */
			if( libfvalue_split_utf8_string_get_segment_by_index(
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
			if( ( line_string_size != 1 )
			 || ( line_string[ 0 ] != 0 ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported empty line string: %d - not empty.",
				 function,
				 line_index );

				goto on_error;
			}
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
	if( types != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &types,
		 NULL );
	}
	if( lines != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &lines,
		 NULL );
	}
	return( -1 );
}

/* Parse a record string for the values
 * Returns 1 if successful or -1 on error
 */
int libewf_single_files_parse_record_values(
     size64_t *media_size,
     libfvalue_split_utf8_string_t *lines,
     int *line_index,
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
	int value_index                       = 0;

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
	if( libfvalue_split_utf8_string_get_segment_by_index(
	     lines,
	     *line_index,
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
		 *line_index );

		goto on_error;
	}
	*line_index += 1;

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
		 "%s: unable to split entries string into types.",
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
	if( libfvalue_split_utf8_string_get_segment_by_index(
	     lines,
	     *line_index,
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
		 *line_index );

		goto on_error;
	}
	*line_index += 1;

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
		 "%s: unable to split entries string into values.",
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
		/* Remove trailing carriage return
		 */
		else if( type_string[ type_string_size - 2 ] == (uint8_t) '\r' )
		{
			type_string[ type_string_size - 2 ] = 0;

			type_string_size -= 1;
		}
		if( value_index < number_of_values )
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
			/* Remove trailing carriage return
			 */
			else if( value_string[ value_string_size - 2 ] == (uint8_t) '\r' )
			{
				value_string[ value_string_size - 2 ] = 0;

				value_string_size -= 1;
			}
		}
		else
		{
			value_string      = NULL;
			value_string_size = 0;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: type: %s with value: %s.\n",
			 function,
			 (char *) type_string,
			 (char *) value_string );
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

/* Parse a single file entry string for the values
 * Returns 1 if successful or -1 on error
 */
int libewf_single_files_parse_file_entry(
     libcdata_tree_node_t *parent_file_entry_node,
     libfvalue_split_utf8_string_t *lines,
     int *line_index,
     libfvalue_split_utf8_string_t *types,
     uint8_t *format,
     libcerror_error_t **error )
{
	libewf_single_file_entry_t *single_file_entry = NULL;
	libfvalue_split_utf8_string_t *values         = NULL;
	libcdata_tree_node_t *file_entry_node         = NULL;
	uint8_t *line_string                          = NULL;
	uint8_t *type_string                          = NULL;
	uint8_t *value_string                         = NULL;
	static char *function                         = "libewf_single_files_parse_file_entry";
	size_t line_string_size                       = 0;
	size_t type_string_size                       = 0;
	size_t value_string_size                      = 0;
	size_t value_string_index                     = 0;
	uint64_t number_of_sub_entries                = 0;
	uint64_t value_64bit                          = 0;
	int number_of_lines                           = 0;
	int number_of_types                           = 0;
	int number_of_values                          = 0;
	int value_index                               = 0;
	int zero_values_only                          = 0;

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

		goto on_error;
	}
	if( libewf_single_files_parse_file_entry_number_of_sub_entries(
	     lines,
	     line_index,
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
	if( libfvalue_split_utf8_string_get_segment_by_index(
	     lines,
	     *line_index,
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
		 *line_index );

		goto on_error;
	}
	*line_index += 1;

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
		 "%s: unable to split entries string into values.",
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
	if( libewf_single_file_entry_initialize(
	     &single_file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create single file entry.",
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
		/* Remove trailing carriage return
		 */
		else if( type_string[ type_string_size - 2 ] == (uint8_t) '\r' )
		{
			type_string[ type_string_size - 2 ] = 0;

			type_string_size -= 1;
		}
		if( value_index < number_of_values )
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
			/* Remove trailing carriage return
			 */
			else if( value_string[ value_string_size - 2 ] == (uint8_t) '\r' )
			{
				value_string[ value_string_size - 2 ] = 0;

				value_string_size -= 1;
			}
		}
		else
		{
			value_string      = NULL;
			value_string_size = 0;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: type: %s with value: %s.\n",
			 function,
			 (char *) type_string,
			 (char *) value_string );
		}
#endif
		if( value_string == NULL )
		{
			/* Ignore empty values
			 */
		}
		else if( type_string_size == 4 )
		{
			if( ( type_string[ 0 ] == (uint8_t) 'c' )
			 && ( type_string[ 1 ] == (uint8_t) 'i' )
			 && ( type_string[ 2 ] == (uint8_t) 'd' ) )
			{
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'o' )
			      && ( type_string[ 1 ] == (uint8_t) 'p' )
			      && ( type_string[ 2 ] == (uint8_t) 'r' ) )
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
					 "%s: unable to set entry flags.",
					 function );

					goto on_error;
				}
				if( value_64bit > (uint64_t) UINT32_MAX )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
					 "%s: invalid entry flags value exceeds maximum.",
					 function );

					goto on_error;
				}
				single_file_entry->flags = (uint32_t) value_64bit;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 's' )
			      && ( type_string[ 1 ] == (uint8_t) 'r' )
			      && ( type_string[ 2 ] == (uint8_t) 'c' ) )
			{
			}
			else if( ( type_string[ 0 ] == (uint8_t) 's' )
			      && ( type_string[ 1 ] == (uint8_t) 'u' )
			      && ( type_string[ 2 ] == (uint8_t) 'b' ) )
			{
			}
			else if( ( type_string[ 0 ] == (uint8_t) 's' )
			      && ( type_string[ 1 ] == (uint8_t) 'h' )
			      && ( type_string[ 2 ] == (uint8_t) 'a' ) )
			{
				single_file_entry->sha1_hash = (uint8_t *) memory_allocate(
				                                            sizeof( uint8_t ) * value_string_size );

				if( single_file_entry->sha1_hash == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
					 "%s: unable to create MD5 hash.",
					 function );

					goto on_error;
				}
				zero_values_only = 1;

				for( value_string_index = 0;
				     value_string_index < value_string_size - 1;
				     value_string_index++ )
				{
					if( ( value_string[ value_string_index ] >= (uint8_t) '0' )
					 && ( value_string[ value_string_index ] <= (uint8_t) '9' ) )
					{
						single_file_entry->sha1_hash[ value_string_index ] = value_string[ value_string_index ];
					}
					else if( ( value_string[ value_string_index ] >= (uint8_t) 'A' )
					      && ( value_string[ value_string_index ] <= (uint8_t) 'F' ) )
					{
						single_file_entry->sha1_hash[ value_string_index ] = (uint8_t) ( 'a' - 'A' ) + value_string[ value_string_index ];
					}
					else if( ( value_string[ value_string_index ] >= (uint8_t) 'a' )
					      && ( value_string[ value_string_index ] <= (uint8_t) 'f' ) )
					{
						single_file_entry->sha1_hash[ value_string_index ] = value_string[ value_string_index ];
					}
					else
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
						 "%s: unsupported character in MD5 hash.",
						 function );

						goto on_error;
					}
					if( value_string[ value_string_index ] != (uint8_t) '0' )
					{
						zero_values_only = 0;
					}
				}
				single_file_entry->sha1_hash[ value_string_size - 1 ] = 0;

				if( zero_values_only == 0 )
				{
					single_file_entry->sha1_hash_size = value_string_size;
				}
			}
		}
		else if( type_string_size == 3 )
		{
			/* Access time
			 */
			if( ( type_string[ 0 ] == (uint8_t) 'a' )
			 && ( type_string[ 1 ] == (uint8_t) 'c' ) )
			{
				if( libfvalue_utf8_string_copy_to_integer(
				     value_string,
				     value_string_size,
				     &value_64bit,
				     32,
				     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set access time.",
					 function );

					goto on_error;
				}
				single_file_entry->access_time = (uint32_t) value_64bit;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'a' )
			      && ( type_string[ 1 ] == (uint8_t) 'q' ) )
			{
			}
			/* Data offset
			 * consist of: unknown, offset and size
			 */
			else if( ( type_string[ 0 ] == (uint8_t) 'b' )
			      && ( type_string[ 1 ] == (uint8_t) 'e' ) )
			{
				if( libewf_single_files_parse_file_entry_offset_values(
				     single_file_entry,
				     value_string,
				     value_string_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_CONVERSION,
					 LIBCERROR_CONVERSION_ERROR_GENERIC,
					 "%s: unable to parse offset values string.",
					 function );

					goto on_error;
				}
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
			/* Creation time
			 */
			else if( ( type_string[ 0 ] == (uint8_t) 'c' )
			      && ( type_string[ 1 ] == (uint8_t) 'r' ) )
			{
				if( libfvalue_utf8_string_copy_to_integer(
				     value_string,
				     value_string_size,
				     &value_64bit,
				     32,
				     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set creation time.",
					 function );

					goto on_error;
				}
				single_file_entry->creation_time = (uint32_t) value_64bit;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'd' )
			      && ( type_string[ 1 ] == (uint8_t) 'l' ) )
			{
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'd' )
			      && ( type_string[ 1 ] == (uint8_t) 'u' ) )
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
					 "%s: unable to set duplicate data offset.",
					 function );

					goto on_error;
				}
				single_file_entry->duplicate_data_offset = (off64_t) value_64bit;
			}
			/* MD5 digest hash
			 */
			else if( ( type_string[ 0 ] == (uint8_t) 'h' )
			      && ( type_string[ 1 ] == (uint8_t) 'a' ) )
			{
				single_file_entry->md5_hash = (uint8_t *) memory_allocate(
				                                           sizeof( uint8_t ) * value_string_size );

				if( single_file_entry->md5_hash == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
					 "%s: unable to create MD5 hash.",
					 function );

					goto on_error;
				}
				zero_values_only = 1;

				for( value_string_index = 0;
				     value_string_index < value_string_size - 1;
				     value_string_index++ )
				{
					if( ( value_string[ value_string_index ] >= (uint8_t) '0' )
					 && ( value_string[ value_string_index ] <= (uint8_t) '9' ) )
					{
						single_file_entry->md5_hash[ value_string_index ] = value_string[ value_string_index ];
					}
					else if( ( value_string[ value_string_index ] >= (uint8_t) 'A' )
					      && ( value_string[ value_string_index ] <= (uint8_t) 'F' ) )
					{
						single_file_entry->md5_hash[ value_string_index ] = (uint8_t) ( 'a' - 'A' ) + value_string[ value_string_index ];
					}
					else if( ( value_string[ value_string_index ] >= (uint8_t) 'a' )
					      && ( value_string[ value_string_index ] <= (uint8_t) 'f' ) )
					{
						single_file_entry->md5_hash[ value_string_index ] = value_string[ value_string_index ];
					}
					else
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
						 "%s: unsupported character in MD5 hash.",
						 function );

						goto on_error;
					}
					if( value_string[ value_string_index ] != (uint8_t) '0' )
					{
						zero_values_only = 0;
					}
				}
				single_file_entry->md5_hash[ value_string_size - 1 ] = 0;

				if( zero_values_only == 0 )
				{
					single_file_entry->md5_hash_size = value_string_size;
				}
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'i' )
			      && ( type_string[ 1 ] == (uint8_t) 'd' ) )
			{
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'j' )
			      && ( type_string[ 1 ] == (uint8_t) 'q' ) )
			{
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'l' )
			      && ( type_string[ 1 ] == (uint8_t) 'o' ) )
			{
			}
			/* Size
			 */
			else if( ( type_string[ 0 ] == (uint8_t) 'l' )
			      && ( type_string[ 1 ] == (uint8_t) 's' ) )
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
					 "%s: unable to set size.",
					 function );

					goto on_error;
				}
				single_file_entry->size = (size64_t) value_64bit;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'm' )
			      && ( type_string[ 1 ] == (uint8_t) 'o' ) )
			{
				if( libfvalue_utf8_string_copy_to_integer(
				     value_string,
				     value_string_size,
				     &value_64bit,
				     32,
				     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set entry modification time.",
					 function );

					goto on_error;
				}
				single_file_entry->entry_modification_time = (uint32_t) value_64bit;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'p' )
			      && ( type_string[ 1 ] == (uint8_t) 'm' ) )
			{
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'p' )
			      && ( type_string[ 1 ] == (uint8_t) 'o' ) )
			{
			}
			/* Modification time
			 */
			else if( ( type_string[ 0 ] == (uint8_t) 'w' )
			      && ( type_string[ 1 ] == (uint8_t) 'r' ) )
			{
				if( libfvalue_utf8_string_copy_to_integer(
				     value_string,
				     value_string_size,
				     &value_64bit,
				     32,
				     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set modification time.",
					 function );

					goto on_error;
				}
				single_file_entry->modification_time = (uint32_t) value_64bit;
			}
		}
		else if( type_string_size == 2 )
		{
			/* Name
			 */
			if( type_string[ 0 ] == (uint8_t) 'n' )
			{
				single_file_entry->name = (uint8_t *) memory_allocate(
								       sizeof( uint8_t ) * value_string_size );

				if( single_file_entry->name == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
					 "%s: unable to create name.",
					 function );

					goto on_error;
				}
				if( narrow_string_copy(
				     single_file_entry->name,
				     value_string,
				     value_string_size - 1 ) == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set name.",
					 function );

					goto on_error;
				}
				single_file_entry->name[ value_string_size - 1 ] = 0;

				single_file_entry->name_size = value_string_size;
			}
		}
		if( type_string_size == 2 )
		{
			if( type_string[ 0 ] == (uint8_t) 'p' )
			{
				/* p = 1 if directory
				 * p = empty if file
				 */
				if( value_string == NULL )
				{
					single_file_entry->type = LIBEWF_FILE_ENTRY_TYPE_FILE;
				}
				else if( ( value_string_size == 2 )
				      && ( value_string[ 0 ] == (uint8_t) '1' ) )
				{
					single_file_entry->type = LIBEWF_FILE_ENTRY_TYPE_DIRECTORY;
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
	if( libcdata_tree_node_set_value(
	     parent_file_entry_node,
	     (intptr_t *) single_file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set single file entry in node.",
		 function );

		goto on_error;
	}
	single_file_entry = NULL;

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
	if( ( *line_index + number_of_sub_entries ) > (uint64_t) number_of_lines )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: number of sub entries exceed the number of available lines.",
		 function );

		goto on_error;
	}
	while( number_of_sub_entries > 0 )
	{
		if( libcdata_tree_node_initialize(
		     &file_entry_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create single file entry node.",
			 function );

			goto on_error;
		}
		if( libewf_single_files_parse_file_entry(
		     file_entry_node,
		     lines,
		     line_index,
		     types,
		     format,
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
		if( libcdata_tree_node_append_node(
		     parent_file_entry_node,
		     file_entry_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append single file entry node to parent.",
			 function );

			goto on_error;
		}
		file_entry_node = NULL;

		number_of_sub_entries--;
	}
	return( 1 );

on_error:
	if( file_entry_node != NULL )
	{
		libcdata_tree_node_free(
		 &file_entry_node,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libewf_single_file_entry_free,
		 NULL );
	}
	if( single_file_entry != NULL )
	{
		libewf_single_file_entry_free(
		 &single_file_entry,
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

/* Parse a single file entry string for the number of sub entries
 * Returns 1 if successful or -1 on error
 */
int libewf_single_files_parse_file_entry_number_of_sub_entries(
     libfvalue_split_utf8_string_t *lines,
     int *line_index,
     uint64_t *number_of_sub_entries,
     libcerror_error_t **error )
{
	libfvalue_split_utf8_string_t *values = NULL;
	uint8_t *line_string                  = NULL;
	uint8_t *value_string                 = NULL;
	static char *function                 = "libewf_single_files_parse_file_entry_number_of_sub_entries";
	size_t line_string_size               = 0;
	size_t value_string_size              = 0;
	int number_of_values                  = 0;

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
	if( libfvalue_split_utf8_string_get_segment_by_index(
	     lines,
	     *line_index,
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
		 *line_index );

		goto on_error;
	}
	*line_index += 1;

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
		 "%s: unable to split entries string into values.",
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
	if( value_string_size == 2 )
	{
		if( value_string[ 0 ] != (uint8_t) '0' )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported first value: %c.",
			 function,
			 value_string[ 0 ] );

			goto on_error;
		}
	}
	else if( value_string_size == 3 )
	{
		if( ( value_string[ 0 ] != (uint8_t) '2' )
		 || ( value_string[ 1 ] != (uint8_t) '6' ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported first value: %c%c.",
			 function,
			 value_string[ 0 ],
			 value_string[ 1 ] );

			goto on_error;
		}
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported first value.",
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
	     number_of_sub_entries,
	     64,
	     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to set number fo sub entries.",
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

/* Parse a single file entry offset values string for the values
 * Returns 1 if successful or -1 on error
 */
int libewf_single_files_parse_file_entry_offset_values(
     libewf_single_file_entry_t *single_file_entry,
     const uint8_t *offset_values_string,
     size_t offset_values_string_size,
     libcerror_error_t **error )
{
	libfvalue_split_utf8_string_t *offset_values  = NULL;
	uint8_t *offset_value_string                  = NULL;
	static char *function                         = "libewf_single_files_parse_file_entry_offset_values";
	size_t offset_value_string_size               = 0;
	uint64_t value_64bit                          = 0;
	int number_of_offset_values                   = 0;

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
		 function );

		return( -1 );
	}
	if( libfvalue_utf8_string_split(
	     offset_values_string,
	     offset_values_string_size,
	     (uint8_t) ' ',
	     &offset_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split string into offset values.",
		 function );

		goto on_error;
	}
	if( libfvalue_split_utf8_string_get_number_of_segments(
	     offset_values,
	     &number_of_offset_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of offset values",
		 function );

		goto on_error;
	}
	if( ( number_of_offset_values != 1 )
	 && ( number_of_offset_values != 3 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported number of offset values.",
		 function );

		goto on_error;
	}
	if( number_of_offset_values == 3 )
	{
		if( libfvalue_split_utf8_string_get_segment_by_index(
		     offset_values,
		     1,
		     &offset_value_string,
		     &offset_value_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offset value string: 1.",
			 function );

			goto on_error;
		}
		if( libfvalue_utf8_string_copy_to_integer(
		     offset_value_string,
		     offset_value_string_size,
		     &value_64bit,
		     64,
		     LIBFVALUE_INTEGER_FORMAT_TYPE_HEXADECIMAL | LIBFVALUE_INTEGER_FORMAT_FLAG_NO_BASE_INDICATOR,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to set data offset.",
			 function );

			goto on_error;
		}
		single_file_entry->data_offset = (off64_t) value_64bit;

		if( libfvalue_split_utf8_string_get_segment_by_index(
		     offset_values,
		     2,
		     &offset_value_string,
		     &offset_value_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offset value string: 2.",
			 function );

			goto on_error;
		}
		if( libfvalue_utf8_string_copy_to_integer(
		     offset_value_string,
		     offset_value_string_size,
		     &value_64bit,
		     64,
		     LIBFVALUE_INTEGER_FORMAT_TYPE_HEXADECIMAL | LIBFVALUE_INTEGER_FORMAT_FLAG_NO_BASE_INDICATOR,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to set data size.",
			 function );

			goto on_error;
		}
		single_file_entry->data_size = (size64_t) value_64bit;
	}
	if( libfvalue_split_utf8_string_free(
	     &offset_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free split offset values.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( offset_values != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &offset_values,
		 NULL );
	}
	return( -1 );
}

