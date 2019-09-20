/*
 * Single file entries functions
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
#include "libewf_single_file_source.h"
#include "libewf_single_file_subject.h"
#include "libewf_single_file_permission.h"
#include "libewf_single_file_extended_attribute.h"
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
		if ((*single_files)->root_file_entry_node != NULL)
		{
			if (libcdata_tree_node_free(
				&((*single_files)->root_file_entry_node),
				(int(*)(intptr_t **, libcerror_error_t **)) &libewf_single_file_entry_free,
				error) != 1)
			{
				libcerror_error_set(
					error,
					LIBCERROR_ERROR_DOMAIN_RUNTIME,
					LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					"%s: unable to free single file entry tree.",
					function);

				result = -1;
			}
		}
		if ((*single_files)->root_file_source_node != NULL)
		{
			if (libcdata_tree_node_free(
				&((*single_files)->root_file_source_node),
				(int(*)(intptr_t **, libcerror_error_t **)) &libewf_single_file_source_free,
				error) != 1)
			{
				libcerror_error_set(
					error,
					LIBCERROR_ERROR_DOMAIN_RUNTIME,
					LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					"%s: unable to free single file source tree.",
					function);

				result = -1;
			}
		}
		if ((*single_files)->root_file_subject_node != NULL)
		{
			if (libcdata_tree_node_free(
				&((*single_files)->root_file_subject_node),
				(int(*)(intptr_t **, libcerror_error_t **)) &libewf_single_file_subject_free,
				error) != 1)
			{
				libcerror_error_set(
					error,
					LIBCERROR_ERROR_DOMAIN_RUNTIME,
					LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					"%s: unable to free single file subject tree.",
					function);

				result = -1;
			}
		}
		if ((*single_files)->root_file_permission_node != NULL)
		{
			if (libcdata_tree_node_free(
				&((*single_files)->root_file_permission_node),
				(int(*)(intptr_t **, libcerror_error_t **)) &libewf_single_file_permission_free,
				error) != 1)
			{
				libcerror_error_set(
					error,
					LIBCERROR_ERROR_DOMAIN_RUNTIME,
					LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					"%s: unable to free single file permission tree.",
					function);

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
	int id								 = -1;

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
		/* Find the line containing: "perm"
*/
		for (line_index = 0;
			line_index < number_of_lines;
			line_index++)
		{
			if (libfvalue_split_utf8_string_get_segment_by_index(
				lines,
				line_index,
				&line_string,
				&line_string_size,
				error) != 1)
			{
				libcerror_error_set(
					error,
					LIBCERROR_ERROR_DOMAIN_RUNTIME,
					LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					"%s: unable to retrieve line string: %d.",
					function,
					line_index);

				goto on_error;
			}
			if (line_string_size == 5)
			{
				if (line_string == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_RUNTIME,
						LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
						"%s: missing line string: %d.",
						function,
						line_index);

					goto on_error;
				}
				if ((line_string[0] == (uint8_t) 'p')
				 && (line_string[1] == (uint8_t) 'e')
				 && (line_string[2] == (uint8_t) 'r')
				 && (line_string[3] == (uint8_t) 'm'))
				{
					line_index += 2;

					break;
				}
			}
		}
		if (line_index < number_of_lines)
		{
			if (libfvalue_split_utf8_string_get_segment_by_index(
				lines,
				line_index,
				&line_string,
				&line_string_size,
				error) != 1)
			{
				libcerror_error_set(
					error,
					LIBCERROR_ERROR_DOMAIN_RUNTIME,
					LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					"%s: unable to retrieve line string: %d.",
					function,
					line_index);

				goto on_error;
			}
			line_index += 1;

			if (libfvalue_utf8_string_split(
				line_string,
				line_string_size,
				(uint8_t) '\t',
				&types,
				error) != 1)
			{
				libcerror_error_set(
					error,
					LIBCERROR_ERROR_DOMAIN_RUNTIME,
					LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					"%s: unable to split entries string into types.",
					function);

				goto on_error;
			}
			if (libcdata_tree_node_initialize(
				&(single_files->root_file_permission_node),
				error) != 1)
			{
				libcerror_error_set(
					error,
					LIBCERROR_ERROR_DOMAIN_RUNTIME,
					LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					"%s: unable to create root single file permission node.",
					function);

				goto on_error;
			}
			id = -1;
			if (libewf_single_files_parse_file_permission(
				single_files->root_file_permission_node,
				lines,
				&line_index,
				types,
				format,
				0,
				-1,
				error) != 1)
			{
				libcerror_error_set(
					error,
					LIBCERROR_ERROR_DOMAIN_CONVERSION,
					LIBCERROR_CONVERSION_ERROR_GENERIC,
					"%s: unable to parse file permission.",
					function);

				goto on_error;
			}
			/* The single files permissions should be followed by an empty line
			 */
			if (libfvalue_split_utf8_string_get_segment_by_index(
				lines,
				line_index,
				&line_string,
				&line_string_size,
				error) != 1)
			{
				libcerror_error_set(
					error,
					LIBCERROR_ERROR_DOMAIN_RUNTIME,
					LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					"%s: unable to retrieve line string: %d.",
					function,
					line_index);

				goto on_error;
			}
			if ((line_string_size != 1)
				|| (line_string[0] != 0))
			{
				libcerror_error_set(
					error,
					LIBCERROR_ERROR_DOMAIN_RUNTIME,
					LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					"%s: unsupported empty line string: %d - not empty.",
					function,
					line_index);

				goto on_error;
			}
		}
		if (libfvalue_split_utf8_string_free(
			&types,
			error) != 1)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_RUNTIME,
				LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				"%s: unable to free split types.",
				function);

			goto on_error;
		}

		/* Find the line containing: "srce"
		*/
		for (line_index = 0;
			line_index < number_of_lines;
			line_index++)
		{
			if (libfvalue_split_utf8_string_get_segment_by_index(
				lines,
				line_index,
				&line_string,
				&line_string_size,
				error) != 1)
			{
				libcerror_error_set(
					error,
					LIBCERROR_ERROR_DOMAIN_RUNTIME,
					LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					"%s: unable to retrieve line string: %d.",
					function,
					line_index);

				goto on_error;
			}
			if (line_string_size == 5)
			{
				if (line_string == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_RUNTIME,
						LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
						"%s: missing line string: %d.",
						function,
						line_index);

					goto on_error;
				}
				if ((line_string[0] == (uint8_t) 's')
					&& (line_string[1] == (uint8_t) 'r')
					&& (line_string[2] == (uint8_t) 'c')
					&& (line_string[3] == (uint8_t) 'e'))
				{
					line_index += 2;

					break;
				}
			}
		}
		if (line_index < number_of_lines)
		{
			if (libfvalue_split_utf8_string_get_segment_by_index(
				lines,
				line_index,
				&line_string,
				&line_string_size,
				error) != 1)
			{
				libcerror_error_set(
					error,
					LIBCERROR_ERROR_DOMAIN_RUNTIME,
					LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					"%s: unable to retrieve line string: %d.",
					function,
					line_index);

				goto on_error;
			}
			line_index += 1;

			if (libfvalue_utf8_string_split(
				line_string,
				line_string_size,
				(uint8_t) '\t',
				&types,
				error) != 1)
			{
				libcerror_error_set(
					error,
					LIBCERROR_ERROR_DOMAIN_RUNTIME,
					LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					"%s: unable to split entries string into types.",
					function);

				goto on_error;
			}
			if (libcdata_tree_node_initialize(
				&(single_files->root_file_source_node),
				error) != 1)
			{
				libcerror_error_set(
					error,
					LIBCERROR_ERROR_DOMAIN_RUNTIME,
					LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					"%s: unable to create root single file source node.",
					function);

				goto on_error;
			}
			if (libewf_single_files_parse_file_source(
				single_files->root_file_source_node,
				lines,
				&line_index,
				types,
				format,
				error) != 1)
			{
				libcerror_error_set(
					error,
					LIBCERROR_ERROR_DOMAIN_CONVERSION,
					LIBCERROR_CONVERSION_ERROR_GENERIC,
					"%s: unable to parse file source.",
					function);

				goto on_error;
			}
			/* The single files sources should be followed by an empty line
			 */
			if (libfvalue_split_utf8_string_get_segment_by_index(
				lines,
				line_index,
				&line_string,
				&line_string_size,
				error) != 1)
			{
				libcerror_error_set(
					error,
					LIBCERROR_ERROR_DOMAIN_RUNTIME,
					LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					"%s: unable to retrieve line string: %d.",
					function,
					line_index);

				goto on_error;
			}
			if ((line_string_size != 1)
				|| (line_string[0] != 0))
			{
				libcerror_error_set(
					error,
					LIBCERROR_ERROR_DOMAIN_RUNTIME,
					LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					"%s: unsupported empty line string: %d - not empty.",
					function,
					line_index);

				goto on_error;
			}
		}
		if (libfvalue_split_utf8_string_free(
			&types,
			error) != 1)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_RUNTIME,
				LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				"%s: unable to free split types.",
				function);

			goto on_error;
		}

		/* Find the line containing: "sub"
		*/
		for (line_index = 0;
			line_index < number_of_lines;
			line_index++)
		{
			if (libfvalue_split_utf8_string_get_segment_by_index(
				lines,
				line_index,
				&line_string,
				&line_string_size,
				error) != 1)
			{
				libcerror_error_set(
					error,
					LIBCERROR_ERROR_DOMAIN_RUNTIME,
					LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					"%s: unable to retrieve line string: %d.",
					function,
					line_index);

				goto on_error;
			}
			if (line_string_size == 4)
			{
				if (line_string == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_RUNTIME,
						LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
						"%s: missing line string: %d.",
						function,
						line_index);

					goto on_error;
				}
				if ((line_string[0] == (uint8_t) 's')
					&& (line_string[1] == (uint8_t) 'u')
					&& (line_string[2] == (uint8_t) 'b'))
				{
					line_index += 2;

					break;
				}
			}
		}
		if (line_index < number_of_lines)
		{
			if (libfvalue_split_utf8_string_get_segment_by_index(
				lines,
				line_index,
				&line_string,
				&line_string_size,
				error) != 1)
			{
				libcerror_error_set(
					error,
					LIBCERROR_ERROR_DOMAIN_RUNTIME,
					LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					"%s: unable to retrieve line string: %d.",
					function,
					line_index);

				goto on_error;
			}
			line_index += 1;

			if (libfvalue_utf8_string_split(
				line_string,
				line_string_size,
				(uint8_t) '\t',
				&types,
				error) != 1)
			{
				libcerror_error_set(
					error,
					LIBCERROR_ERROR_DOMAIN_RUNTIME,
					LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					"%s: unable to split entries string into types.",
					function);

				goto on_error;
			}
			if (libcdata_tree_node_initialize(
				&(single_files->root_file_subject_node),
				error) != 1)
			{
				libcerror_error_set(
					error,
					LIBCERROR_ERROR_DOMAIN_RUNTIME,
					LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					"%s: unable to create root single file subject node.",
					function);

				goto on_error;
			}
			if (libewf_single_files_parse_file_subject(
				single_files->root_file_subject_node,
				lines,
				&line_index,
				types,
				format,
				error) != 1)
			{
				libcerror_error_set(
					error,
					LIBCERROR_ERROR_DOMAIN_CONVERSION,
					LIBCERROR_CONVERSION_ERROR_GENERIC,
					"%s: unable to parse file subject.",
					function);

				goto on_error;
			}
			/* The single files subjects should be followed by an empty line
			 */
			if (libfvalue_split_utf8_string_get_segment_by_index(
				lines,
				line_index,
				&line_string,
				&line_string_size,
				error) != 1)
			{
				libcerror_error_set(
					error,
					LIBCERROR_ERROR_DOMAIN_RUNTIME,
					LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					"%s: unable to retrieve line string: %d.",
					function,
					line_index);

				goto on_error;
			}
			if ((line_string_size != 1)
				|| (line_string[0] != 0))
			{
				libcerror_error_set(
					error,
					LIBCERROR_ERROR_DOMAIN_RUNTIME,
					LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					"%s: unsupported empty line string: %d - not empty.",
					function,
					line_index);

				goto on_error;
			}
		}
		if (libfvalue_split_utf8_string_free(
			&types,
			error) != 1)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_RUNTIME,
				LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				"%s: unable to free split types.",
				function);

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


int libewf_single_files_read_hex_string(
	const uint8_t *value_string, 
	size_t value_string_size, 
	uint8_t *destination_string, 
	size_t *destination_size,
	libcerror_error_t **error)
{
	int zero_values_only = 1;
	size_t value_string_index;
	static char *function = "libewf_single_files_read_hex_string";

	for (value_string_index = 0;
		value_string_index < value_string_size - 1;
		value_string_index++)
	{
		if ((value_string[value_string_index] >= (uint8_t) '0')
			&& (value_string[value_string_index] <= (uint8_t) '9'))
		{
			destination_string[value_string_index] = value_string[value_string_index];
		}
		else if ((value_string[value_string_index] >= (uint8_t) 'A')
			&& (value_string[value_string_index] <= (uint8_t) 'F'))
		{
			destination_string[value_string_index] = (uint8_t)('a' - 'A') + value_string[value_string_index];
		}
		else if ((value_string[value_string_index] >= (uint8_t) 'a')
			&& (value_string[value_string_index] <= (uint8_t) 'f'))
		{
			destination_string[value_string_index] = value_string[value_string_index];
		}
		else
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_RUNTIME,
				LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				"%s: unsupported character in hex string.",
				function);

			return -1;
		}
		if (value_string[value_string_index] != (uint8_t) '0')
		{
			zero_values_only = 0;
		}
	}
	destination_string[value_string_size - 1] = 0;

	if (zero_values_only == 0)
	{
		*destination_size = value_string_size;
	}
	return 1;
}

/* Parse a single file permission string for the values
 * Returns 1 if successful or -1 on error
 */
int libewf_single_files_parse_file_permission(
	libcdata_tree_node_t *parent_file_permission_node,
	libfvalue_split_utf8_string_t *lines,
	int *line_index,
	libfvalue_split_utf8_string_t *types,
	uint8_t *format,
	int tree_level,
	int id,
	libcerror_error_t **error)
{
	libewf_single_file_permission_t *single_file_permission = NULL;
	libfvalue_split_utf8_string_t *values					= NULL;
	libcdata_tree_node_t *file_permission_node				= NULL;
	uint8_t *line_string									= NULL;
	uint8_t *type_string									= NULL;
	uint8_t *value_string									= NULL;
	static char *function									= "libewf_single_files_parse_file_permission";
	size_t line_string_size									= 0;
	size_t type_string_size									= 0;
	size_t value_string_size								= 0;
	uint64_t number_of_sub_entries							= 0;
	uint64_t value_64bit									= 0;
	int number_of_lines										= 0;
	int number_of_types										= 0;
	int number_of_values									= 0;
	int value_index											= 0;

	if (parent_file_permission_node == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid parent file permission node.",
			function);

		return(-1);
	}
	if (line_index == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid line index.",
			function);

		return(-1);
	}
	if (format == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid format.",
			function);

		return(-1);
	}
	if (libfvalue_split_utf8_string_get_number_of_segments(
		types,
		&number_of_types,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve number of types",
			function);

		goto on_error;
	}

	if (libewf_single_files_parse_file_entry_number_of_sub_entries(
		lines,
		line_index,
		&number_of_sub_entries,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_CONVERSION,
			LIBCERROR_CONVERSION_ERROR_GENERIC,
			"%s: unable to parse file entry number of sub entries.",
			function);

		goto on_error;
	}

	if (libfvalue_split_utf8_string_get_segment_by_index(
		lines,
		*line_index,
		&line_string,
		&line_string_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve line string: %d.",
			function,
			*line_index);

		goto on_error;
	}
	*line_index += 1;

	if (libfvalue_utf8_string_split(
		line_string,
		line_string_size,
		(uint8_t) '\t',
		&values,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			"%s: unable to split entries string into values.",
			function);

		goto on_error;
	}
	if (libfvalue_split_utf8_string_get_number_of_segments(
		values,
		&number_of_values,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve number of values",
			function);

		goto on_error;
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	if (number_of_types != number_of_values)
	{
		if (libcnotify_verbose != 0)
		{
			libcnotify_printf(
				"%s: mismatch in number of types and values.\n",
				function);
		}
	}
#endif
	if (libewf_single_file_permission_initialize(
		&single_file_permission,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			"%s: unable to create single file permission.",
			function);

		goto on_error;
	}
	single_file_permission->id = id;
	for (value_index = 0;
		value_index < number_of_types;
		value_index++)
	{
		if (libfvalue_split_utf8_string_get_segment_by_index(
			types,
			value_index,
			&type_string,
			&type_string_size,
			error) != 1)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_RUNTIME,
				LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				"%s: unable to retrieve type string: %d.",
				function,
				value_index);

			goto on_error;
		}
		if ((type_string == NULL)
			|| (type_string_size < 2)
			|| (type_string[0] == 0))
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_RUNTIME,
				LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				"%s: missing type string: %d.",
				function,
				value_index);

			goto on_error;
		}
		/* Remove trailing carriage return
		 */
		else if (type_string[type_string_size - 2] == (uint8_t) '\r')
		{
			type_string[type_string_size - 2] = 0;

			type_string_size -= 1;
		}
		if (value_index < number_of_values)
		{
			if (libfvalue_split_utf8_string_get_segment_by_index(
				values,
				value_index,
				&value_string,
				&value_string_size,
				error) != 1)
			{
				libcerror_error_set(
					error,
					LIBCERROR_ERROR_DOMAIN_RUNTIME,
					LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					"%s: unable to retrieve value string: %d.",
					function,
					value_index);

				goto on_error;
			}
			if ((value_string == NULL)
				|| (value_string_size < 2)
				|| (value_string[0] == 0))
			{
				value_string = NULL;
				value_string_size = 0;
			}
			/* Remove trailing carriage return
			 */
			else if (value_string[value_string_size - 2] == (uint8_t) '\r')
			{
				value_string[value_string_size - 2] = 0;

				value_string_size -= 1;
			}
		}
		else
		{
			value_string = NULL;
			value_string_size = 0;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if (libcnotify_verbose != 0)
		{
			libcnotify_printf(
				"%s: type: %s with value: %s.\n",
				function,
				(char *)type_string,
				(char *)value_string);
		}
#endif
		if (value_string == NULL)
		{
			/* Ignore empty values
			 */
		}
		else if (type_string_size == 4)
		{
			if ((type_string[0] == (uint8_t) 'n')
			 && (type_string[1] == (uint8_t) 't')
			 && (type_string[2] == (uint8_t) 'a'))
			{
				if (libfvalue_utf8_string_copy_to_integer(
					value_string,
					value_string_size,
					&value_64bit,
					32,
					LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
					error) != 1)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set access_mask.",
						function);

					goto on_error;
				}
				single_file_permission->access_mask = (uint32_t)value_64bit;
			}
			else if ((type_string[0] == (uint8_t) 'n')
				  && (type_string[1] == (uint8_t) 't')
				  && (type_string[2] == (uint8_t) 'i'))
			{
				if (libfvalue_utf8_string_copy_to_integer(
					value_string,
					value_string_size,
					&value_64bit,
					32,
					LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
					error) != 1)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set inheritance_flags.",
						function);

					goto on_error;
				}
				single_file_permission->inheritance_flags = (uint32_t)value_64bit;
			}
		}
		else if (type_string_size == 3)
		{
			if ((type_string[0] == (uint8_t) 'p')
			 && (type_string[1] == (uint8_t) 'r'))
			{
				if (libfvalue_utf8_string_copy_to_integer(
					value_string,
					value_string_size,
					&value_64bit,
					32,
					LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
					error) != 1)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set props.",
						function);

					goto on_error;
				}
				single_file_permission->props = (uint32_t)value_64bit;
			}
		}
		else if (type_string_size == 2)
		{
			if (type_string[0] == (uint8_t) 'n')
			{
				single_file_permission->name = (uint8_t *)memory_allocate(
					sizeof(uint8_t) * value_string_size);

				if (single_file_permission->name == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
						"%s: unable to create name.",
						function);

					goto on_error;
				}
				if (narrow_string_copy(
					single_file_permission->name,
					value_string,
					value_string_size - 1) == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set name.",
						function);

					goto on_error;
				}
				single_file_permission->name[value_string_size - 1] = 0;

				single_file_permission->name_size = value_string_size;
			}
			else if (type_string[0] == (uint8_t) 's')
			{
				single_file_permission->uid = (uint8_t *)memory_allocate(
					sizeof(uint8_t) * value_string_size);

				if (single_file_permission->uid == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
						"%s: unable to create uid.",
						function);

					goto on_error;
				}
				if (narrow_string_copy(
					single_file_permission->uid,
					value_string,
					value_string_size - 1) == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set uid.",
						function);

					goto on_error;
				}
				single_file_permission->uid[value_string_size - 1] = 0;

				single_file_permission->uid_size = value_string_size;
			}
			else if (type_string[0] == (uint8_t) 'p')
			{
				/* p = 1 if directory
				 * p = empty if file
				 */
				if (value_string == NULL)
				{
					single_file_permission->type = LIBEWF_FILE_ENTRY_TYPE_FILE;
				}
				else if ((value_string_size == 2)
					&& (value_string[0] == (uint8_t) '1'))
				{
					single_file_permission->type = LIBEWF_FILE_ENTRY_TYPE_DIRECTORY;
				}
			}
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if (libcnotify_verbose != 0)
	{
		libcnotify_printf(
			"\n");
	}
#endif
	if (libfvalue_split_utf8_string_free(
		&values,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			"%s: unable to free split values.",
			function);

		goto on_error;
	}
	if (libcdata_tree_node_set_value(
		parent_file_permission_node,
		(intptr_t *)single_file_permission,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to set single file permission in node.",
			function);

		goto on_error;
	}
	single_file_permission = NULL;

	if (libfvalue_split_utf8_string_get_number_of_segments(
		lines,
		&number_of_lines,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve number of lines",
			function);

		goto on_error;
	}
	if ((*line_index + number_of_sub_entries) > (uint64_t)number_of_lines)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			"%s: number of sub entries exceed the number of available lines.",
			function);

		goto on_error;
	}
	while (number_of_sub_entries > 0)
	{
		if (tree_level == 0)
		{
			id++;
		}
		if (libcdata_tree_node_initialize(
			&file_permission_node,
			error) != 1)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_RUNTIME,
				LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				"%s: unable to create single file permission node.",
				function);

			goto on_error;
		}
		if (libewf_single_files_parse_file_permission(
			file_permission_node,
			lines,
			line_index,
			types,
			format,
			tree_level + 1,
			id,
			error) != 1)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_CONVERSION,
				LIBCERROR_CONVERSION_ERROR_GENERIC,
				"%s: unable to parse file permission.",
				function);

			goto on_error;
		}
		if (libcdata_tree_node_append_node(
			parent_file_permission_node,
			file_permission_node,
			error) != 1)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_RUNTIME,
				LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				"%s: unable to append single file permission node to parent.",
				function);

			goto on_error;
		}
		file_permission_node = NULL;

		number_of_sub_entries--;
	}
	return(1);

on_error:
	if (file_permission_node != NULL)
	{
		libcdata_tree_node_free(
			&file_permission_node,
			(int(*)(intptr_t **, libcerror_error_t **)) &libewf_single_file_permission_free,
			NULL);
	}
	if (single_file_permission != NULL)
	{
		libewf_single_file_permission_free(
			&single_file_permission,
			NULL);
	}
	if (values != NULL)
	{
		libfvalue_split_utf8_string_free(
			&values,
			NULL);
	}
	return(-1);
}

/* Parse a single file source string for the values
 * Returns 1 if successful or -1 on error
 */
int libewf_single_files_parse_file_source(
	libcdata_tree_node_t *parent_file_source_node,
	libfvalue_split_utf8_string_t *lines,
	int *line_index,
	libfvalue_split_utf8_string_t *types,
	uint8_t *format,
	libcerror_error_t **error)
{
	libewf_single_file_source_t *single_file_source = NULL;
	libfvalue_split_utf8_string_t *values = NULL;
	libcdata_tree_node_t *file_source_node = NULL;
	uint8_t *line_string = NULL;
	uint8_t *type_string = NULL;
	uint8_t *value_string = NULL;
	static char *function = "libewf_single_files_parse_file_source";
	size_t line_string_size = 0;
	size_t type_string_size = 0;
	size_t value_string_size = 0;
	uint64_t number_of_sub_entries = 0;
	uint64_t value_64bit = 0;
	int number_of_lines = 0;
	int number_of_types = 0;
	int number_of_values = 0;
	int value_index = 0;

	if (parent_file_source_node == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid parent file source node.",
			function);

		return(-1);
	}
	if (line_index == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid line index.",
			function);

		return(-1);
	}
	if (format == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid format.",
			function);

		return(-1);
	}
	if (libfvalue_split_utf8_string_get_number_of_segments(
		types,
		&number_of_types,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve number of types",
			function);

		goto on_error;
	}

	if (libewf_single_files_parse_file_entry_number_of_sub_entries(
		lines,
		line_index,
		&number_of_sub_entries,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_CONVERSION,
			LIBCERROR_CONVERSION_ERROR_GENERIC,
			"%s: unable to parse file entry number of sub entries.",
			function);

		goto on_error;
	}

	if (libfvalue_split_utf8_string_get_segment_by_index(
		lines,
		*line_index,
		&line_string,
		&line_string_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve line string: %d.",
			function,
			*line_index);

		goto on_error;
	}
	*line_index += 1;

	if (libfvalue_utf8_string_split(
		line_string,
		line_string_size,
		(uint8_t) '\t',
		&values,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			"%s: unable to split entries string into values.",
			function);

		goto on_error;
	}
	if (libfvalue_split_utf8_string_get_number_of_segments(
		values,
		&number_of_values,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve number of values",
			function);

		goto on_error;
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	if (number_of_types != number_of_values)
	{
		if (libcnotify_verbose != 0)
		{
			libcnotify_printf(
				"%s: mismatch in number of types and values.\n",
				function);
		}
	}
#endif
	if (libewf_single_file_source_initialize(
		&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			"%s: unable to create single file source.",
			function);

		goto on_error;
	}
	for (value_index = 0;
		value_index < number_of_types;
		value_index++)
	{
		if (libfvalue_split_utf8_string_get_segment_by_index(
			types,
			value_index,
			&type_string,
			&type_string_size,
			error) != 1)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_RUNTIME,
				LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				"%s: unable to retrieve type string: %d.",
				function,
				value_index);

			goto on_error;
		}
		if ((type_string == NULL)
			|| (type_string_size < 2)
			|| (type_string[0] == 0))
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_RUNTIME,
				LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				"%s: missing type string: %d.",
				function,
				value_index);

			goto on_error;
		}
		/* Remove trailing carriage return
		 */
		else if (type_string[type_string_size - 2] == (uint8_t) '\r')
		{
			type_string[type_string_size - 2] = 0;

			type_string_size -= 1;
		}
		if (value_index < number_of_values)
		{
			if (libfvalue_split_utf8_string_get_segment_by_index(
				values,
				value_index,
				&value_string,
				&value_string_size,
				error) != 1)
			{
				libcerror_error_set(
					error,
					LIBCERROR_ERROR_DOMAIN_RUNTIME,
					LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					"%s: unable to retrieve value string: %d.",
					function,
					value_index);

				goto on_error;
			}
			if ((value_string == NULL)
				|| (value_string_size < 2)
				|| (value_string[0] == 0))
			{
				value_string = NULL;
				value_string_size = 0;
			}
			/* Remove trailing carriage return
			 */
			else if (value_string[value_string_size - 2] == (uint8_t) '\r')
			{
				value_string[value_string_size - 2] = 0;

				value_string_size -= 1;
			}
		}
		else
		{
			value_string = NULL;
			value_string_size = 0;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if (libcnotify_verbose != 0)
		{
			libcnotify_printf(
				"%s: type: %s with value: %s.\n",
				function,
				(char *)type_string,
				(char *)value_string);
		}
#endif
		if (value_string == NULL)
		{
			/* Ignore empty values
			 */
		}
		else if (type_string_size == 4)
		{
			if ((type_string[0] == (uint8_t) 'p')
			 && (type_string[1] == (uint8_t) 'g')
			 && (type_string[2] == (uint8_t) 'u'))
			{
				single_file_source->primary_guid = (uint8_t *)memory_allocate(
					sizeof(uint8_t) * value_string_size);

				if (single_file_source->primary_guid == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
						"%s: unable to create primary GUID.",
						function);

					goto on_error;
				}

				if (libewf_single_files_read_hex_string(
					value_string, 
					value_string_size, 
					single_file_source->primary_guid, 
					&single_file_source->primary_guid_size,
					error) != 1)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set primary GUID.",
						function);

					goto on_error;
				}
			}
			else if ((type_string[0] == (uint8_t) 'm')
				  && (type_string[1] == (uint8_t) 'f')
				  && (type_string[2] == (uint8_t) 'r'))
			{
				single_file_source->manufacturer = (uint8_t *)memory_allocate(
					sizeof(uint8_t) * value_string_size);

				if (single_file_source->manufacturer == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
						"%s: unable to create manufacturer.",
						function);

					goto on_error;
				}
				if (narrow_string_copy(
					single_file_source->manufacturer,
					value_string,
					value_string_size - 1) == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set manufacturer.",
						function);

					goto on_error;
				}
				single_file_source->manufacturer[value_string_size - 1] = 0;

				single_file_source->manufacturer_size = value_string_size;
			}
			else if ((type_string[0] == (uint8_t) 'l')
				  && (type_string[1] == (uint8_t) 'o')
				  && (type_string[2] == (uint8_t) 'c'))
			{ 
				single_file_source->location = (uint8_t *)memory_allocate(
					sizeof(uint8_t) * value_string_size);

				if (single_file_source->location == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
						"%s: unable to create location.",
						function);

					goto on_error;
				}
				if (narrow_string_copy(
					single_file_source->location,
					value_string,
					value_string_size - 1) == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set location.",
						function);

					goto on_error;
				}
				single_file_source->location[value_string_size - 1] = 0;

				single_file_source->location_size = value_string_size;
			}
		}
		else if (type_string_size == 3)
		{
			if ((type_string[0] == (uint8_t) 'i')
			 && (type_string[1] == (uint8_t) 'd'))
			{
				if (libfvalue_utf8_string_copy_to_integer(
					value_string,
					value_string_size,
					&value_64bit,
					32,
					LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
					error) != 1)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set id.",
						function);

					goto on_error;
				}
				single_file_source->id = (uint32_t)value_64bit;
			}
			else if ((type_string[0] == (uint8_t) 'g')
				  && (type_string[1] == (uint8_t) 'u'))
			{
				single_file_source->guid = (uint8_t *)memory_allocate(
					sizeof(uint8_t) * value_string_size);

				if (single_file_source->guid == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
						"%s: unable to create GUID.",
						function);

					goto on_error;
				}

				if (libewf_single_files_read_hex_string(
					value_string,
					value_string_size,
					single_file_source->guid,
					&single_file_source->guid_size,
					error) != 1)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set GUID.",
						function);

					goto on_error;
				}
			}
			else if ((type_string[0] == (uint8_t) 'a')
				  && (type_string[1] == (uint8_t) 'h'))
			{
				single_file_source->md5_hash = (uint8_t *)memory_allocate(
					sizeof(uint8_t) * value_string_size);

				if (single_file_source->md5_hash == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
						"%s: unable to create MD5 hash.",
						function);

					goto on_error;
				}

				if (libewf_single_files_read_hex_string(
					value_string,
					value_string_size,
					single_file_source->md5_hash,
					&single_file_source->md5_hash_size,
					error) != 1)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set MD5 hash.",
						function);

					goto on_error;
				}
			}
			else if ((type_string[0] == (uint8_t) 's')
				  && (type_string[1] == (uint8_t) 'h'))
			{
				single_file_source->sha1_hash = (uint8_t *)memory_allocate(
					sizeof(uint8_t) * value_string_size);

				if (single_file_source->sha1_hash == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
						"%s: unable to create SHA1 hash.",
						function);

					goto on_error;
				}

				if (libewf_single_files_read_hex_string(
					value_string,
					value_string_size,
					single_file_source->sha1_hash,
					&single_file_source->sha1_hash_size,
					error) != 1)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set SHA1 hash.",
						function);

					goto on_error;
				}
			}
			else if ((type_string[0] == (uint8_t) 'p')
				  && (type_string[1] == (uint8_t) 'o'))
			{
				if (value_string_size > 0 && *value_string == '-')
					continue;
				if (libfvalue_utf8_string_copy_to_integer(
					value_string,
					value_string_size,
					&value_64bit,
					64,
					LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_SIGNED,
					error) != 1)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set physical offset.",
						function);

					goto on_error;
				}
				single_file_source->physical_offset = (off64_t)value_64bit;				
			}
			else if ((type_string[0] == (uint8_t) 'l')
				  && (type_string[1] == (uint8_t) 'o'))
			{
				if (value_string_size > 0 && *value_string == '-')
					continue;
				if (libfvalue_utf8_string_copy_to_integer(
					value_string,
					value_string_size,
					&value_64bit,
					64,
					LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_SIGNED,
					error) != 1)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set logical offset.",
						function);

					goto on_error;
				}
				single_file_source->logical_offset = (off64_t)value_64bit;
			}
			else if ((type_string[0] == (uint8_t) 't')
				  && (type_string[1] == (uint8_t) 'b'))
			{
				if (libfvalue_utf8_string_copy_to_integer(
					value_string,
					value_string_size,
					&value_64bit,
					64,
					LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
					error) != 1)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set total size.",
						function);

					goto on_error;
				}
				single_file_source->total_size = (off64_t)value_64bit;
			}
			else if ((type_string[0] == (uint8_t) 's')
				  && (type_string[1] == (uint8_t) 'e'))
			{
				single_file_source->serial_number = (uint8_t *)memory_allocate(
					sizeof(uint8_t) * value_string_size);

				if (single_file_source->serial_number == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
						"%s: unable to create serial number.",
						function);

					goto on_error;
				}
				if (narrow_string_copy(
					single_file_source->serial_number,
					value_string,
					value_string_size - 1) == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set serial number.",
						function);

					goto on_error;
				}
				single_file_source->serial_number[value_string_size - 1] = 0;

				single_file_source->serial_number_size = value_string_size;
			}
			else if ((type_string[0] == (uint8_t) 'm')
				  && (type_string[1] == (uint8_t) 'o'))
			{
				single_file_source->model = (uint8_t *)memory_allocate(
					sizeof(uint8_t) * value_string_size);

				if (single_file_source->model == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
						"%s: unable to create model.",
						function);

					goto on_error;
				}
				if (narrow_string_copy(
					single_file_source->model,
					value_string,
					value_string_size - 1) == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set model.",
						function);

					goto on_error;
				}
				single_file_source->model[value_string_size - 1] = 0;

				single_file_source->model_size = value_string_size;
			}
			else if ((type_string[0] == (uint8_t) 'd')
				  && (type_string[1] == (uint8_t) 't'))
			{
				if (value_string_size == 2)
				{
					single_file_source->drive_type = value_string[0];
				}
			}
			else if ((type_string[0] == (uint8_t) 'e')
				  && (type_string[1] == (uint8_t) 'v'))
			{ 
				single_file_source->evidence_number = (uint8_t *)memory_allocate(
					sizeof(uint8_t) * value_string_size);

				if (single_file_source->evidence_number == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
						"%s: unable to create evidence_number.",
						function);

					goto on_error;
				}
				if (narrow_string_copy(
					single_file_source->evidence_number,
					value_string,
					value_string_size - 1) == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set evidence_number.",
						function);

					goto on_error;
				}
				single_file_source->evidence_number[value_string_size - 1] = 0;

				single_file_source->evidence_number_size = value_string_size;
			}
			else if ((type_string[0] == (uint8_t) 'd')
				  && (type_string[1] == (uint8_t) 'o'))
			{ 
				single_file_source->domain = (uint8_t *)memory_allocate(
					sizeof(uint8_t) * value_string_size);

				if (single_file_source->domain == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
						"%s: unable to create domain.",
						function);

					goto on_error;
				}
				if (narrow_string_copy(
					single_file_source->domain,
					value_string,
					value_string_size - 1) == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set domain.",
						function);

					goto on_error;
				}
				single_file_source->domain[value_string_size - 1] = 0;

				single_file_source->domain_size = value_string_size;
			}
			else if ((type_string[0] == (uint8_t) 'a')
				  && (type_string[1] == (uint8_t) 'q'))
			{ 
				if (libfvalue_utf8_string_copy_to_integer(
					value_string,
					value_string_size,
					&value_64bit,
					32,
					LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
					error) != 1)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set acquisition time.",
						function);

					goto on_error;
				}
				single_file_source->acquisition_time = (uint32_t)value_64bit;
			}
			else if ((type_string[0] == (uint8_t) 'i')
				  && (type_string[1] == (uint8_t) 'p'))
			{ 
				single_file_source->ip_address = (uint8_t *)memory_allocate(
					sizeof(uint8_t) * value_string_size);

				if (single_file_source->ip_address == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
						"%s: unable to create ip_address.",
						function);

					goto on_error;
				}
				if (narrow_string_copy(
					single_file_source->ip_address,
					value_string,
					value_string_size - 1) == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set ip_address.",
						function);

					goto on_error;
				}
				single_file_source->ip_address[value_string_size - 1] = 0;

				single_file_source->ip_address_size = value_string_size;
			}
			else if ((type_string[0] == (uint8_t) 's')
				  && (type_string[1] == (uint8_t) 'i'))
			{ // Static IP (bool)?
				/* 1 if static IP
				 * empty otherwise
				 */
				 if (value_string == NULL)
				 {
					 single_file_source->static_ip = 0;
				 }
				 else if ((value_string_size == 2)
					 && (value_string[0] == (uint8_t) '1'))
				 {
					 single_file_source->static_ip = 1;
				 }

			}
			else if ((type_string[0] == (uint8_t) 'm')
				  && (type_string[1] == (uint8_t) 'a'))
			{
				single_file_source->mac_address = (uint8_t *)memory_allocate(
					sizeof(uint8_t) * value_string_size);

				if (single_file_source->mac_address == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
						"%s: unable to create mac_address.",
						function);

					goto on_error;
				}
				if (narrow_string_copy(
					single_file_source->mac_address,
					value_string,
					value_string_size - 1) == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set mac_address.",
						function);

					goto on_error;
				}
				single_file_source->mac_address[value_string_size - 1] = 0;

				single_file_source->mac_address_size = value_string_size;
			}
		}
		else if (type_string_size == 2)
		{
			if (type_string[0] == (uint8_t) 'n')
			{
				single_file_source->name = (uint8_t *)memory_allocate(
					sizeof(uint8_t) * value_string_size);

				if (single_file_source->name == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
						"%s: unable to create name.",
						function);

					goto on_error;
				}
				if (narrow_string_copy(
					single_file_source->name,
					value_string,
					value_string_size - 1) == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set name.",
						function);

					goto on_error;
				}
				single_file_source->name[value_string_size - 1] = 0;

				single_file_source->name_size = value_string_size;
			}
			else if (type_string[0] == (uint8_t) 'p')
			{
				/* p = 1 if directory
				 * p = empty if file
				 */
				if (value_string == NULL)
				{
					single_file_source->type = LIBEWF_FILE_ENTRY_TYPE_FILE;
				}
				else if ((value_string_size == 2)
					&& (value_string[0] == (uint8_t) '1'))
				{
					single_file_source->type = LIBEWF_FILE_ENTRY_TYPE_DIRECTORY;
				}
			}
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if (libcnotify_verbose != 0)
	{
		libcnotify_printf(
			"\n");
	}
#endif
	if (libfvalue_split_utf8_string_free(
		&values,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			"%s: unable to free split values.",
			function);

		goto on_error;
	}
	if (libcdata_tree_node_set_value(
		parent_file_source_node,
		(intptr_t *)single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to set single file source in node.",
			function);

		goto on_error;
	}
	single_file_source = NULL;

	if (libfvalue_split_utf8_string_get_number_of_segments(
		lines,
		&number_of_lines,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve number of lines",
			function);

		goto on_error;
	}
	if ((*line_index + number_of_sub_entries) > (uint64_t)number_of_lines)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			"%s: number of sub entries exceed the number of available lines.",
			function);

		goto on_error;
	}
	while (number_of_sub_entries > 0)
	{
		if (libcdata_tree_node_initialize(
			&file_source_node,
			error) != 1)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_RUNTIME,
				LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				"%s: unable to create single file source node.",
				function);

			goto on_error;
		}
		if (libewf_single_files_parse_file_source(
			file_source_node,
			lines,
			line_index,
			types,
			format,
			error) != 1)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_CONVERSION,
				LIBCERROR_CONVERSION_ERROR_GENERIC,
				"%s: unable to parse file source.",
				function);

			goto on_error;
		}
		if (libcdata_tree_node_append_node(
			parent_file_source_node,
			file_source_node,
			error) != 1)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_RUNTIME,
				LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				"%s: unable to append single file source node to parent.",
				function);

			goto on_error;
		}
		file_source_node = NULL;

		number_of_sub_entries--;
	}
	return(1);

on_error:
	if (file_source_node != NULL)
	{
		libcdata_tree_node_free(
			&file_source_node,
			(int(*)(intptr_t **, libcerror_error_t **)) &libewf_single_file_source_free,
			NULL);
	}
	if (single_file_source != NULL)
	{
		libewf_single_file_source_free(
			&single_file_source,
			NULL);
	}
	if (values != NULL)
	{
		libfvalue_split_utf8_string_free(
			&values,
			NULL);
	}
	return(-1);
}

/* Parse a single file subject string for the values
 * Returns 1 if successful or -1 on error
 */
int libewf_single_files_parse_file_subject(
	libcdata_tree_node_t *parent_file_subject_node,
	libfvalue_split_utf8_string_t *lines,
	int *line_index,
	libfvalue_split_utf8_string_t *types,
	uint8_t *format,
	libcerror_error_t **error)
{
	libewf_single_file_subject_t *single_file_subject = NULL;
	libfvalue_split_utf8_string_t *values = NULL;
	libcdata_tree_node_t *file_subject_node = NULL;
	uint8_t *line_string = NULL;
	uint8_t *type_string = NULL;
	uint8_t *value_string = NULL;
	static char *function = "libewf_single_files_parse_file_subject";
	size_t line_string_size = 0;
	size_t type_string_size = 0;
	size_t value_string_size = 0;
	uint64_t number_of_sub_entries = 0;
	uint64_t value_64bit = 0;
	int number_of_lines = 0;
	int number_of_types = 0;
	int number_of_values = 0;
	int value_index = 0;

	if (parent_file_subject_node == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid parent file subject node.",
			function);

		return(-1);
	}
	if (line_index == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid line index.",
			function);

		return(-1);
	}
	if (format == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid format.",
			function);

		return(-1);
	}
	if (libfvalue_split_utf8_string_get_number_of_segments(
		types,
		&number_of_types,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve number of types",
			function);

		goto on_error;
	}

	if (libewf_single_files_parse_file_entry_number_of_sub_entries(
		lines,
		line_index,
		&number_of_sub_entries,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_CONVERSION,
			LIBCERROR_CONVERSION_ERROR_GENERIC,
			"%s: unable to parse file entry number of sub entries.",
			function);

		goto on_error;
	}

	if (libfvalue_split_utf8_string_get_segment_by_index(
		lines,
		*line_index,
		&line_string,
		&line_string_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve line string: %d.",
			function,
			*line_index);

		goto on_error;
	}
	*line_index += 1;

	if (libfvalue_utf8_string_split(
		line_string,
		line_string_size,
		(uint8_t) '\t',
		&values,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			"%s: unable to split entries string into values.",
			function);

		goto on_error;
	}
	if (libfvalue_split_utf8_string_get_number_of_segments(
		values,
		&number_of_values,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve number of values",
			function);

		goto on_error;
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	if (number_of_types != number_of_values)
	{
		if (libcnotify_verbose != 0)
		{
			libcnotify_printf(
				"%s: mismatch in number of types and values.\n",
				function);
		}
	}
#endif
	if (libewf_single_file_subject_initialize(
		&single_file_subject,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			"%s: unable to create single file subject.",
			function);

		goto on_error;
	}
	for (value_index = 0;
		value_index < number_of_types;
		value_index++)
	{
		if (libfvalue_split_utf8_string_get_segment_by_index(
			types,
			value_index,
			&type_string,
			&type_string_size,
			error) != 1)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_RUNTIME,
				LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				"%s: unable to retrieve type string: %d.",
				function,
				value_index);

			goto on_error;
		}
		if ((type_string == NULL)
			|| (type_string_size < 2)
			|| (type_string[0] == 0))
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_RUNTIME,
				LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				"%s: missing type string: %d.",
				function,
				value_index);

			goto on_error;
		}
		/* Remove trailing carriage return
		 */
		else if (type_string[type_string_size - 2] == (uint8_t) '\r')
		{
			type_string[type_string_size - 2] = 0;

			type_string_size -= 1;
		}
		if (value_index < number_of_values)
		{
			if (libfvalue_split_utf8_string_get_segment_by_index(
				values,
				value_index,
				&value_string,
				&value_string_size,
				error) != 1)
			{
				libcerror_error_set(
					error,
					LIBCERROR_ERROR_DOMAIN_RUNTIME,
					LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					"%s: unable to retrieve value string: %d.",
					function,
					value_index);

				goto on_error;
			}
			if ((value_string == NULL)
				|| (value_string_size < 2)
				|| (value_string[0] == 0))
			{
				value_string = NULL;
				value_string_size = 0;
			}
			/* Remove trailing carriage return
			 */
			else if (value_string[value_string_size - 2] == (uint8_t) '\r')
			{
				value_string[value_string_size - 2] = 0;

				value_string_size -= 1;
			}
		}
		else
		{
			value_string = NULL;
			value_string_size = 0;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if (libcnotify_verbose != 0)
		{
			libcnotify_printf(
				"%s: type: %s with value: %s.\n",
				function,
				(char *)type_string,
				(char *)value_string);
		}
#endif
		if (value_string == NULL)
		{
			/* Ignore empty values
			 */
		}
		else if (type_string_size == 3)
		{
			if ((type_string[0] == (uint8_t) 'i')
		  	 && (type_string[1] == (uint8_t) 'd'))
			{
				if (libfvalue_utf8_string_copy_to_integer(
					value_string,
					value_string_size,
					&value_64bit,
					32,
					LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
					error) != 1)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set id.",
						function);

					goto on_error;
				}
				single_file_subject->id = (uint32_t)value_64bit;
			}
			else if ((type_string[0] == (uint8_t) 'g')
				  && (type_string[1] == (uint8_t) 'u'))
			{
				single_file_subject->guid = (uint8_t *)memory_allocate(
					sizeof(uint8_t) * value_string_size);

				if (single_file_subject->guid == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
						"%s: unable to create GUID.",
						function);

					goto on_error;
				}

				if (libewf_single_files_read_hex_string(
					value_string,
					value_string_size,
					single_file_subject->guid,
					&single_file_subject->guid_size,
					error) != 1)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set GUID.",
						function);

					goto on_error;
				}
			}
			else if ((type_string[0] == (uint8_t) 'n')
				  && (type_string[1] == (uint8_t) 'u'))
			{
				single_file_subject->number = (uint8_t *)memory_allocate(
					sizeof(uint8_t) * value_string_size);

				if (single_file_subject->number == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
						"%s: unable to create serial number.",
						function);

					goto on_error;
				}
				if (narrow_string_copy(
					single_file_subject->number,
					value_string,
					value_string_size - 1) == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set number.",
						function);

					goto on_error;
				}
				single_file_subject->number[value_string_size - 1] = 0;

				single_file_subject->number_size = value_string_size;
			}
			else if ((type_string[0] == (uint8_t) 'c')
				  && (type_string[1] == (uint8_t) 'o'))
			{
				// comments are formatted differently in the tree; len<space>text.
				uint8_t *space = (uint8_t*)strchr((char*)value_string, ' ');
				if (space)
				{
					value_string_size -= ++space - value_string;
					value_string = space;
				}
				single_file_subject->comment = (uint8_t *)memory_allocate(
					sizeof(uint8_t) * value_string_size);

				if (single_file_subject->comment == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
						"%s: unable to create comment.",
						function);

					goto on_error;
				}
				if (narrow_string_copy(
					single_file_subject->comment,
					value_string,
					value_string_size - 1) == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set comment.",
						function);

					goto on_error;
				}
				single_file_subject->comment[value_string_size - 1] = 0;

				single_file_subject->comment_size = value_string_size;
			}
		}
		else if (type_string_size == 2)
		{
			if (type_string[0] == (uint8_t) 'n')
			{
				single_file_subject->name = (uint8_t *)memory_allocate(
					sizeof(uint8_t) * value_string_size);

				if (single_file_subject->name == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
						"%s: unable to create name.",
						function);

					goto on_error;
				}
				if (narrow_string_copy(
					single_file_subject->name,
					value_string,
					value_string_size - 1) == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set name.",
						function);

					goto on_error;
				}
				single_file_subject->name[value_string_size - 1] = 0;

				single_file_subject->name_size = value_string_size;
			}
			else if (type_string[0] == (uint8_t) 'p')
			{
				/* p = 1 if directory
				 * p = empty if file
				 */
				if (value_string == NULL)
				{
					single_file_subject->type = LIBEWF_FILE_ENTRY_TYPE_FILE;
				}
				else if ((value_string_size == 2)
					&& (value_string[0] == (uint8_t) '1'))
				{
					single_file_subject->type = LIBEWF_FILE_ENTRY_TYPE_DIRECTORY;
				}
			}
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if (libcnotify_verbose != 0)
	{
		libcnotify_printf(
			"\n");
	}
#endif
	if (libfvalue_split_utf8_string_free(
		&values,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			"%s: unable to free split values.",
			function);

		goto on_error;
	}
	if (libcdata_tree_node_set_value(
		parent_file_subject_node,
		(intptr_t *)single_file_subject,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to set single file subject in node.",
			function);

		goto on_error;
	}
	single_file_subject = NULL;

	if (libfvalue_split_utf8_string_get_number_of_segments(
		lines,
		&number_of_lines,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve number of lines",
			function);

		goto on_error;
	}
	if ((*line_index + number_of_sub_entries) > (uint64_t)number_of_lines)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			"%s: number of sub entries exceed the number of available lines.",
			function);

		goto on_error;
	}
	while (number_of_sub_entries > 0)
	{
		if (libcdata_tree_node_initialize(
			&file_subject_node,
			error) != 1)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_RUNTIME,
				LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				"%s: unable to create single file subject node.",
				function);

			goto on_error;
		}
		if (libewf_single_files_parse_file_subject(
			file_subject_node,
			lines,
			line_index,
			types,
			format,
			error) != 1)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_CONVERSION,
				LIBCERROR_CONVERSION_ERROR_GENERIC,
				"%s: unable to parse file subject.",
				function);

			goto on_error;
		}
		if (libcdata_tree_node_append_node(
			parent_file_subject_node,
			file_subject_node,
			error) != 1)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_RUNTIME,
				LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				"%s: unable to append single file subject node to parent.",
				function);

			goto on_error;
		}
		file_subject_node = NULL;

		number_of_sub_entries--;
	}
	return(1);

on_error:
	if (file_subject_node != NULL)
	{
		libcdata_tree_node_free(
			&file_subject_node,
			(int(*)(intptr_t **, libcerror_error_t **)) &libewf_single_file_subject_free,
			NULL);
	}
	if (single_file_subject != NULL)
	{
		libewf_single_file_subject_free(
			&single_file_subject,
			NULL);
	}
	if (values != NULL)
	{
		libfvalue_split_utf8_string_free(
			&values,
			NULL);
	}
	return(-1);
}

/* Parse a single file extended_attribute string for the values
 * Returns 1 if successful or -1 on error
 */
int libewf_single_files_parse_extended_attribute(
	libcdata_tree_node_t *parent_file_extended_attribute_node,
	uint8_t *encoded_tree,
	size_t encoded_tree_size,
	libcerror_error_t **error)
{
	static char *function = "libewf_single_files_parse_file_extended_attribute";

	if (parent_file_extended_attribute_node == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid parent file extended_attribute node.",
			function);

		return(-1);
	}

	libewf_single_file_decode_extended_attribute(parent_file_extended_attribute_node,
		encoded_tree,
		encoded_tree_size,
		error);

	return(1);
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
	uint8_t *encoded_tree                         = NULL;
	static char *function                         = "libewf_single_files_parse_file_entry";
	size_t line_string_size                       = 0;
	size_t type_string_size                       = 0;
	size_t value_string_size                      = 0;
	uint64_t number_of_sub_entries                = 0;
	uint64_t value_64bit                          = 0;
	int number_of_lines                           = 0;
	int number_of_types                           = 0;
	int number_of_values                          = 0;
	int value_index                               = 0;

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
			/* Record type id
			*/
	    if ((type_string[0] == (uint8_t) 'c')
			&& (type_string[1] == (uint8_t) 'i')
			&& (type_string[2] == (uint8_t) 'd'))
		{
			if (libfvalue_utf8_string_copy_to_integer(
				value_string,
				value_string_size,
				&value_64bit,
				32,
				LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
				error) != 1)
			{
				libcerror_error_set(
					error,
					LIBCERROR_ERROR_DOMAIN_MEMORY,
					LIBCERROR_MEMORY_ERROR_SET_FAILED,
					"%s: unable to set cid.",
					function);

				goto on_error;
			}
			single_file_entry->record_type = (uint32_t)value_64bit;
		}
		else if( ( type_string[0] == (uint8_t) 'm' )
    			  && ( type_string[1] == (uint8_t) 'i' )
    			  && ( type_string[2] == (uint8_t) 'd' ) )
			{
				single_file_entry->guid = (uint8_t *)memory_allocate(
					sizeof(uint8_t) * value_string_size);

				if (single_file_entry->guid == NULL)
				{
					libcerror_error_set(
					error,
					LIBCERROR_ERROR_DOMAIN_MEMORY,
					LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
					"%s: unable to create GUID.",
					function);

					goto on_error;
				}

				if (libewf_single_files_read_hex_string(
					value_string,
					value_string_size,
					single_file_entry->guid,
					&single_file_entry->guid_size,
					error) != 1)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set GUID.",
						function);

					goto on_error;
				}
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
				if (libfvalue_utf8_string_copy_to_integer(
					value_string,
					value_string_size,
					&value_64bit,
					32,
					LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
					error) != 1)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set source id.",
						function);

					goto on_error;
				}
				single_file_entry->source_id = (uint32_t)value_64bit;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 's' )
			      && ( type_string[ 1 ] == (uint8_t) 'u' )
			      && ( type_string[ 2 ] == (uint8_t) 'b' ) )
			{
				if (libfvalue_utf8_string_copy_to_integer(
					value_string,
					value_string_size,
					&value_64bit,
					32,
					LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
					error) != 1)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set subject id.",
						function);

					goto on_error;
				}
				single_file_entry->subject_id = (uint32_t)value_64bit;
			}
			/* Short Name
			*/
			else if ((type_string[0] == (uint8_t) 's')
				  && (type_string[1] == (uint8_t) 'n')
				  && (type_string[2] == (uint8_t) 'h'))
			{
				// short names are formatted differently in the tree; len<space>text.
				uint8_t *space = (uint8_t*)strchr((char*)value_string, ' ');
				if (space)
				{
					value_string_size -= ++space - value_string;
					value_string = space;
				}
				single_file_entry->short_name = (uint8_t *)memory_allocate(
					sizeof(uint8_t) * value_string_size);

				if (single_file_entry->short_name == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
						"%s: unable to create short_name.",
						function);

					goto on_error;
				}
				if (narrow_string_copy(
					single_file_entry->short_name,
					value_string,
					value_string_size - 1) == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set short_name.",
						function);

					goto on_error;
				}
				single_file_entry->short_name[value_string_size - 1] = 0;

				single_file_entry->short_name_size = value_string_size;
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
					 "%s: unable to create SHA1 hash.",
					 function );

					goto on_error;
				}

				if (libewf_single_files_read_hex_string(
					value_string,
					value_string_size,
					single_file_entry->sha1_hash,
					&single_file_entry->sha1_hash_size,
					error) != 1)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set SHA1 hash.",
						function);

					goto on_error;
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
				     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_SIGNED,
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
				single_file_entry->access_time = (int32_t) value_64bit;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'a' )
			      && ( type_string[ 1 ] == (uint8_t) 'q' ) )
			{
			}
			/* Data offset
			 * consist of: count (always 1 for single files), offset and size
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
				     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_SIGNED,
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
				single_file_entry->creation_time = (int32_t) value_64bit;
			}
			/* deleted time
			*/
			else if( ( type_string[ 0 ] == (uint8_t) 'd' )
			      && ( type_string[ 1 ] == (uint8_t) 'l' ) )
			{
				if (libfvalue_utf8_string_copy_to_integer(
					value_string,
					value_string_size,
					&value_64bit,
					32,
					LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
					error) != 1)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set deleted time.",
						function);

					goto on_error;
				}
				single_file_entry->deleted_time = (uint32_t)value_64bit;
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

				if (libewf_single_files_read_hex_string(
					value_string,
					value_string_size,
					single_file_entry->md5_hash,
					&single_file_entry->md5_hash_size,
					error) != 1)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set MD5 hash.",
						function);

					goto on_error;
				}
			}
			/* File Identifier
			 */
			else if( ( type_string[ 0 ] == (uint8_t) 'i' )
			      && ( type_string[ 1 ] == (uint8_t) 'd' ) )
			{
				if (libfvalue_utf8_string_copy_to_integer(
					value_string,
					value_string_size,
					&value_64bit,
					64,
					LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
					error) != 1)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set size.",
						function);

					goto on_error;
				}
				single_file_entry->file_identifier = (uint64_t)value_64bit;
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
				     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_SIGNED,
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
				single_file_entry->entry_modification_time = (int32_t) value_64bit;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'p' )
			      && ( type_string[ 1 ] == (uint8_t) 'm' ) )
			{
				if (value_string_size > 0 && *value_string == '-')
					continue;
				if (libfvalue_utf8_string_copy_to_integer(
					value_string,
					value_string_size,
					&value_64bit,
					32,
					LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_SIGNED,
					error) != 1)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set permission id.",
						function);

					goto on_error;
				}
				single_file_entry->permission_id = (uint32_t)value_64bit;
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
				     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_SIGNED,
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
				single_file_entry->modification_time = (int32_t) value_64bit;
			}
			/* Extended attributes
			 */
			else if ((type_string[0] == (uint8_t) 'e')
				  && (type_string[1] == (uint8_t) 'a'))
			{
				encoded_tree = (uint8_t *)memory_allocate(
					sizeof(uint8_t) * value_string_size);

				if (encoded_tree == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
						"%s: unable to create encoded extended attributes tree.",
						function);

					goto on_error;
				}
				if (narrow_string_copy(
					encoded_tree,
					value_string,
					value_string_size - 1) == NULL)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_MEMORY,
						LIBCERROR_MEMORY_ERROR_SET_FAILED,
						"%s: unable to set encoded extended attributes tree.",
						function);

					goto on_error;
				}
				encoded_tree[value_string_size - 1] = 0;

				if (libcdata_tree_node_initialize(
					&(single_file_entry->root_extended_attribute_node),
					error) != 1)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_RUNTIME,
						LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
						"%s: unable to create root extended attribute node.",
						function);

					goto on_error;
				}

				//single_file_entry->name_size = value_string_size;
				// have the encoded data in a string, now need to parse it ...
				if (libewf_single_files_parse_extended_attribute(
					single_file_entry->root_extended_attribute_node,
					encoded_tree,
					value_string_size,
					error) != 1)
				{
					libcerror_error_set(
						error,
						LIBCERROR_ERROR_DOMAIN_CONVERSION,
						LIBCERROR_CONVERSION_ERROR_GENERIC,
						"%s: unable to parse extended attribute.",
						function);

					goto on_error;
				}
				if (encoded_tree != NULL)
				{
					memory_free(
						encoded_tree
					);
				}
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
	if (values != NULL)
	{
		libfvalue_split_utf8_string_free(
			&values,
			NULL);
	}
	if (encoded_tree != NULL)
	{
		memory_free(
			encoded_tree
		);
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

