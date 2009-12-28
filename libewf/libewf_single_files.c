/*
 * Single file entries functions
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
#include <types.h>

#include <liberror.h>
#include <libnotify.h>

#include "libewf_libuna.h"
#include "libewf_single_file_entry.h"
#include "libewf_single_files.h"
#include "libewf_split_values.h"
#include "libewf_string.h"
#include "libewf_tree_type.h"

/* Initialize the single files
 * Returns 1 if successful or -1 on error
 */
int libewf_single_files_initialize(
     libewf_single_files_t **single_files,
     liberror_error_t **error )
{
	static char *function = "libewf_single_files_initialize";

	if( single_files == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single files.",
		 function );

		return( -1 );
	}
	if( *single_files == NULL )
	{
		*single_files = (libewf_single_files_t *) memory_allocate(
		                                           sizeof( libewf_single_files_t ) );

		if( *single_files == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create single files.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *single_files,
		     0,
		     sizeof( libewf_single_files_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear single files.",
			 function );

			memory_free(
			 *single_files );

			*single_files = NULL;

			return( -1 );
		}
	}
	return( 1 );
}

/* Frees the single files including elements
 * Returns 1 if successful or -1 on error
 */
int libewf_single_files_free(
     libewf_single_files_t **single_files,
     liberror_error_t **error )
{
	static char *function = "libewf_single_files_free";
	int result            = 1;

	if( single_files == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single files.",
		 function );

		return( 1 );
	}
	if( *single_files != NULL )
	{
		if( ( *single_files )->ltree_data != NULL )
		{
			memory_free(
			 ( *single_files )->ltree_data );
		}
		if( ( *single_files )->root_file_entry_node != NULL )
		{
			if( libewf_tree_node_free(
			     &( ( *single_files )->root_file_entry_node ),
			     &libewf_single_file_entry_free,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
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
     liberror_error_t **error )
{
	libewf_character_t *file_entries_string = NULL;
	static char *function                   = "libewf_single_files_parse";
	size_t file_entries_string_size         = 0;

	if( single_files == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single files.",
		 function );

		return( 1 );
	}
	if( single_files->ltree_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid single files - missing ltree data.",
		 function );

		return( -1 );
	}
	if( libewf_string_size_from_utf16_stream(
	     single_files->ltree_data,
	     single_files->ltree_data_size,
	     LIBUNA_ENDIAN_LITTLE,
	     &file_entries_string_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine file entries string size.",
		 function );

		return( -1 );
	}
	file_entries_string = (libewf_character_t *) memory_allocate(
	                                              sizeof( libewf_character_t ) * (size_t) file_entries_string_size );

	if( file_entries_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file entries string.",
		 function );

		return( -1 );
	}
	if( libewf_string_copy_from_utf16_stream(
	     file_entries_string,
	     file_entries_string_size,
	     single_files->ltree_data,
	     single_files->ltree_data_size,
	     LIBUNA_ENDIAN_LITTLE,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set file entries string.",
		 function );

		memory_free(
		 file_entries_string );

		return( -1 );
	}
	if( libewf_single_files_parse_file_entries(
	     single_files,
	     file_entries_string,
	     file_entries_string_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse file entries string.",
		 function );

		memory_free(
		 file_entries_string );

		return( -1 );
	}
	memory_free(
	 file_entries_string );

	return( 1 );
}

/* Parse a single file entries string for the values
 * Returns 1 if successful or -1 on error
 */
int libewf_single_files_parse_file_entries(
     libewf_single_files_t *single_files,
     libewf_character_t *entries_string,
     size_t entries_string_size,
     liberror_error_t **error )
{
	libewf_split_values_t *lines = NULL;
	libewf_split_values_t *types = NULL;
	static char *function        = "libewf_single_files_parse_file_entries";
	int line_iterator            = 0;

	if( single_files == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single files.",
		 function );

		return( 1 );
	}
	if( entries_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entries string.",
		 function );

		return( -1 );
	}
	if( libewf_split_values_parse_string(
	     &lines,
	     entries_string,
	     entries_string_size,
	     (libewf_character_t) '\n',
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split entries string into lines.",
		 function );

		return( -1 );
	}
	if( lines->amount_of_values > 0 )
	{
		if( ( ( lines->values[ 0 ] )[ 0 ] < (libewf_character_t) '0' )
		 || ( ( lines->values[ 0 ] )[ 0 ] > (libewf_character_t) '9' ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: single file entries string not supported.",
			 function );

			libewf_split_values_free(
			 &lines,
			 NULL );

			return( -1 );
		}
		/* Find the line containing: "entry"
		 */
		for( line_iterator = 0;
		     line_iterator < lines->amount_of_values;
		     line_iterator++ )
		{
			if( lines->sizes[ line_iterator ] == 6 )
			{
				if( libewf_string_compare(
				     lines->values[ line_iterator ],
				     _LIBEWF_STRING( "entry" ),
				     5 ) == 0 )
				{
					line_iterator += 2;

					break;
				}
			}
		}
		if( line_iterator < lines->amount_of_values )
		{
			if( libewf_split_values_parse_string(
			     &types,
			     lines->values[ line_iterator ],
			     lines->sizes[ line_iterator ],
			     (libewf_character_t) '\t',
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to split entries string into types.",
				 function );

				libewf_split_values_free(
				 &lines,
				 NULL );

				return( -1 );
			}
			line_iterator += 1;

			if( libewf_tree_node_initialize(
			     &( single_files->root_file_entry_node ),
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create root single file entry node.",
				 function );

				libewf_split_values_free(
				 &lines,
				 NULL );

				return( -1 );
			}
			/* TODO parse first entries differently ?
			 */
			if( libewf_single_files_parse_file_entry(
			     single_files->root_file_entry_node,
			     lines,
			     &line_iterator,
			     types,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_CONVERSION,
				 LIBERROR_CONVERSION_ERROR_GENERIC,
				 "%s: unable to parse file entry.",
				 function );

				libewf_split_values_free(
				 &types,
				 NULL );
				libewf_split_values_free(
				 &lines,
				 NULL );

				return( -1 );
			}
			/* The single files entries should be followed by an emtpy line
			 */
			if( ( lines->sizes[ line_iterator ] == 1 )
			 && ( ( lines->values[ line_iterator ] )[ 0 ] == 0 ) )
			{
				/* TODO error if line is not empty */
			}
		}
		if( libewf_split_values_free(
		     &types,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free split types.",
			 function );

			libewf_split_values_free(
			 &lines,
			 NULL );

			return( -1 );
		}
	}
	if( libewf_split_values_free(
	     &lines,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free split lines.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Parse a single file entry string for the values
 * Returns 1 if successful or -1 on error
 */
int libewf_single_files_parse_file_entry(
     libewf_tree_node_t *parent_file_entry_node,
     libewf_split_values_t *lines,
     int *line_iterator,
     libewf_split_values_t *types,
     liberror_error_t **error )
{
	libewf_single_file_entry_t *single_file_entry = NULL;
	libewf_split_values_t *offset_values          = NULL;
	libewf_split_values_t *values                 = NULL;
	libewf_tree_node_t *file_entry_node           = NULL;
	static char *function                         = "libewf_single_files_parse_file_entry";
	size_t type_string_length                     = 0;
	size_t value_string_length                    = 0;
	uint64_t amount_of_child_entries              = 0;
	uint64_t value_64bit                          = 0;
	int value_iterator                            = 0;

	if( parent_file_entry_node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid parent file entry node.",
		 function );

		return( 1 );
	}
	if( lines == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid lines.",
		 function );

		return( 1 );
	}
	if( line_iterator == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid line iterator.",
		 function );

		return( 1 );
	}
	if( ( *line_iterator < 0 )
	 || ( *line_iterator >= lines->amount_of_values ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: line iterator out of range.",
		 function );

		return( -1 );
	}
	/* Make sure there are at least 2 lines
	 */
	if( ( *line_iterator + 1 ) >= lines->amount_of_values )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: lines too small.",
		 function );

		return( -1 );
	}
	if( types == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid types.",
		 function );

		return( 1 );
	}
	if( libewf_split_values_parse_string(
	     &values,
	     lines->values[ *line_iterator ],
	     lines->sizes[ *line_iterator ],
	     (libewf_character_t) '\t',
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split entries string into values.",
		 function );

		return( -1 );
	}
	if( ( values->amount_of_values != 2 )
	 || ( values->sizes[ 0 ] != 2 )
	 || ( ( values->values[ 0 ] )[ 0 ] != '0' ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported single file entry first line.",
		 function );

		libewf_split_values_free(
		 &values,
		 NULL );

		return( -1 );
	}
	if( libewf_string_copy_to_64bit(
	     values->values[ 1 ],
	     values->sizes[ 1 ],
	     &amount_of_child_entries,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to set amount fo child entries.",
		 function );

		libewf_single_file_entry_free(
		 (intptr_t *) single_file_entry,
		 NULL );
		libewf_split_values_free(
		 &values,
		 NULL );

		return( -1 );
	}
	if( libewf_split_values_free(
	     &values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free split values.",
		 function );

		return( -1 );
	}
	*line_iterator += 1;

	if( libewf_split_values_parse_string(
	     &values,
	     lines->values[ *line_iterator ],
	     lines->sizes[ *line_iterator ],
	     (libewf_character_t) '\t',
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split entries string into values.",
		 function );

		return( -1 );
	}
	if( types->amount_of_values != values->amount_of_values )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported single file entry second line.",
		 function );

		libewf_split_values_free(
		 &values,
		 NULL );

		return( -1 );
	}
	if( libewf_single_file_entry_initialize(
	     &single_file_entry,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create single file entry.",
		 function );

		libewf_split_values_free(
		 &values,
		 NULL );

		return( -1 );
	}
	for( value_iterator = 0;
	     value_iterator < values->amount_of_values;
	     value_iterator++ )
	{
		type_string_length  = types->sizes[ value_iterator ] - 1;
		value_string_length = values->sizes[ value_iterator ] - 1;

		/* Remove trailing carriage return
		 */
		if( ( type_string_length > 0 )
		 && ( ( types->values[ value_iterator ] )[ type_string_length - 1 ] == (libewf_character_t) '\r' ) )
		{
			type_string_length -= 1;
		}
		if( ( value_string_length > 0 )
		 && ( ( types->values[ value_iterator ] )[ value_string_length - 1 ] == (libewf_character_t) '\r' ) )
		{
			value_string_length -= 1;
		}
fprintf( stderr, "%s\t: %s\n", types->values[ value_iterator ], values->values[ value_iterator ] );

		/* Ignore empty values
		 */
		if( value_string_length == 0 )
		{
			continue;
		}
		if( type_string_length == 3 )
		{
			if( libewf_string_compare(
			     types->values[ value_iterator ],
			     _LIBEWF_STRING( "cid" ),
			     type_string_length ) == 0 )
			{
			}
			else if( libewf_string_compare(
				  types->values[ value_iterator ],
				  _LIBEWF_STRING( "opr" ),
				  type_string_length ) == 0 )
			{
			}
			else if( libewf_string_compare(
				  types->values[ value_iterator ],
				  _LIBEWF_STRING( "src" ),
				  type_string_length ) == 0 )
			{
			}
			else if( libewf_string_compare(
				  types->values[ value_iterator ],
				  _LIBEWF_STRING( "sub" ),
				  type_string_length ) == 0 )
			{
			}
		}
		else if( type_string_length == 2 )
		{
			/* Access time
			 */
			if( libewf_string_compare(
			     types->values[ value_iterator ],
			     _LIBEWF_STRING( "ac" ),
			     type_string_length ) == 0 )
			{
				if( libewf_string_copy_to_64bit(
				     values->values[ value_iterator ],
				     value_string_length + 1,
				     &value_64bit,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_MEMORY,
					 LIBERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set access time.",
					 function );

					libewf_single_file_entry_free(
					 (intptr_t *) single_file_entry,
					 NULL );
					libewf_split_values_free(
					 &values,
					 NULL );

					return( -1 );
				}
				single_file_entry->access_time = (time_t) value_64bit;
			}
			else if( libewf_string_compare(
				  types->values[ value_iterator ],
				  _LIBEWF_STRING( "aq" ),
				  type_string_length ) == 0 )
			{
			}
			/* Data offset
			 * consist of: segment number, offset and size
			 */
			else if( libewf_string_compare(
				  types->values[ value_iterator ],
				  _LIBEWF_STRING( "be" ),
				  type_string_length ) == 0 )
			{
				if( libewf_split_values_parse_string(
				     &offset_values,
				     values->values[ value_iterator ],
				     value_string_length + 1,
				     (libewf_character_t) ' ',
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to split value string into offset values.",
					 function );

					libewf_single_file_entry_free(
					 (intptr_t *) single_file_entry,
					 NULL );
					libewf_split_values_free(
					 &values,
					 NULL );

					return( -1 );
				}
				if( offset_values->amount_of_values == 3 )
				{
					if( libewf_string_copy_to_64bit(
					     offset_values->values[ 0 ],
					     offset_values->sizes[ 0 ],
					     &value_64bit,
					     error ) != 1 )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_MEMORY,
						 LIBERROR_MEMORY_ERROR_SET_FAILED,
						 "%s: unable to set data segment number.",
						 function );

						libewf_single_file_entry_free(
						 (intptr_t *) single_file_entry,
						 NULL );
						libewf_split_values_free(
						 &values,
						 NULL );

						return( -1 );
					}
					/* TODO check value range */
					single_file_entry->data_segment_number = (uint16_t) value_64bit;

					if( libewf_string_copy_to_64bit(
					     offset_values->values[ 1 ],
					     offset_values->sizes[ 1 ],
					     &value_64bit,
					     error ) != 1 )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_MEMORY,
						 LIBERROR_MEMORY_ERROR_SET_FAILED,
						 "%s: unable to set data offset.",
						 function );

						libewf_single_file_entry_free(
						 (intptr_t *) single_file_entry,
						 NULL );
						libewf_split_values_free(
						 &values,
						 NULL );

						return( -1 );
					}
					single_file_entry->data_offset = (off64_t) value_64bit;

					if( libewf_string_copy_to_64bit(
					     offset_values->values[ 2 ],
					     offset_values->sizes[ 2 ],
					     &value_64bit,
					     error ) != 1 )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_MEMORY,
						 LIBERROR_MEMORY_ERROR_SET_FAILED,
						 "%s: unable to set data size.",
						 function );

						libewf_single_file_entry_free(
						 (intptr_t *) single_file_entry,
						 NULL );
						libewf_split_values_free(
						 &values,
						 NULL );

						return( -1 );
					}
					single_file_entry->data_size = (size64_t) value_64bit;
				}
				if( libewf_split_values_free(
				     &offset_values,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free split offset values.",
					 function );

					libewf_single_file_entry_free(
					 (intptr_t *) single_file_entry,
					 NULL );
					libewf_split_values_free(
					 &values,
					 NULL );

					return( -1 );
				}
			}
			/* Creation time
			 */
			else if( libewf_string_compare(
				  types->values[ value_iterator ],
				  _LIBEWF_STRING( "cr" ),
				  type_string_length ) == 0 )
			{
				if( libewf_string_copy_to_64bit(
				     values->values[ value_iterator ],
				     value_string_length + 1,
				     &value_64bit,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_MEMORY,
					 LIBERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set creation time.",
					 function );

					libewf_single_file_entry_free(
					 (intptr_t *) single_file_entry,
					 NULL );
					libewf_split_values_free(
					 &values,
					 NULL );

					return( -1 );
				}
				single_file_entry->creation_time = (time_t) value_64bit;
			}
			else if( libewf_string_compare(
				  types->values[ value_iterator ],
				  _LIBEWF_STRING( "dl" ),
				  type_string_length ) == 0 )
			{
			}
			else if( libewf_string_compare(
				  types->values[ value_iterator ],
				  _LIBEWF_STRING( "du" ),
				  type_string_length ) == 0 )
			{
			}
			/* MD5 digest hash
			 */
			else if( libewf_string_compare(
				  types->values[ value_iterator ],
				  _LIBEWF_STRING( "ha" ),
				  type_string_length ) == 0 )
			{
				single_file_entry->md5_hash = (libewf_character_t *) memory_allocate(
				                                                      sizeof( libewf_character_t ) * ( value_string_length + 1 ) );

				if( single_file_entry->md5_hash == NULL )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_MEMORY,
					 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
					 "%s: unable to create MD5 hash.",
					 function );

					libewf_single_file_entry_free(
					 (intptr_t *) single_file_entry,
					 NULL );
					libewf_split_values_free(
					 &values,
					 NULL );

					return( -1 );
				}
				if( libewf_string_copy(
				     single_file_entry->md5_hash,
				     values->values[ value_iterator ],
				     value_string_length ) == NULL )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_MEMORY,
					 LIBERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set MD5 hash.",
					 function );

					libewf_single_file_entry_free(
					 (intptr_t *) single_file_entry,
					 NULL );
					libewf_split_values_free(
					 &values,
					 NULL );

					return( -1 );
				}
				single_file_entry->md5_hash[ value_string_length ] = 0;

				single_file_entry->md5_hash_size = value_string_length + 1;
			}
			else if( libewf_string_compare(
				  types->values[ value_iterator ],
				  _LIBEWF_STRING( "id" ),
				  type_string_length ) == 0 )
			{
			}
			else if( libewf_string_compare(
				  types->values[ value_iterator ],
				  _LIBEWF_STRING( "jq" ),
				  type_string_length ) == 0 )
			{
			}
			else if( libewf_string_compare(
				  types->values[ value_iterator ],
				  _LIBEWF_STRING( "lo" ),
				  type_string_length ) == 0 )
			{
			}
			/* Size
			 */
			else if( libewf_string_compare(
				  types->values[ value_iterator ],
				  _LIBEWF_STRING( "ls" ),
				  type_string_length ) == 0 )
			{
				if( libewf_string_copy_to_64bit(
				     values->values[ value_iterator ],
				     value_string_length + 1,
				     &value_64bit,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_MEMORY,
					 LIBERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set size.",
					 function );

					libewf_single_file_entry_free(
					 (intptr_t *) single_file_entry,
					 NULL );
					libewf_split_values_free(
					 &values,
					 NULL );

					return( -1 );
				}
				single_file_entry->size = (size64_t) value_64bit;
			}
			else if( libewf_string_compare(
				  types->values[ value_iterator ],
				  _LIBEWF_STRING( "mo" ),
				  type_string_length ) == 0 )
			{
				if( libewf_string_copy_to_64bit(
				     values->values[ value_iterator ],
				     value_string_length + 1,
				     &value_64bit,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_MEMORY,
					 LIBERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set entry modification time.",
					 function );

					libewf_single_file_entry_free(
					 (intptr_t *) single_file_entry,
					 NULL );
					libewf_split_values_free(
					 &values,
					 NULL );

					return( -1 );
				}
				single_file_entry->entry_modification_time = (time_t) value_64bit;
			}
			else if( libewf_string_compare(
				  types->values[ value_iterator ],
				  _LIBEWF_STRING( "pm" ),
				  type_string_length ) == 0 )
			{
			}
			else if( libewf_string_compare(
				  types->values[ value_iterator ],
				  _LIBEWF_STRING( "po" ),
				  type_string_length ) == 0 )
			{
			}
			/* Modification time
			 */
			else if( libewf_string_compare(
				  types->values[ value_iterator ],
				  _LIBEWF_STRING( "wr" ),
				  type_string_length ) == 0 )
			{
				if( libewf_string_copy_to_64bit(
				     values->values[ value_iterator ],
				     value_string_length + 1,
				     &value_64bit,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_MEMORY,
					 LIBERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set modification time.",
					 function );

					libewf_single_file_entry_free(
					 (intptr_t *) single_file_entry,
					 NULL );
					libewf_split_values_free(
					 &values,
					 NULL );

					return( -1 );
				}
				single_file_entry->modification_time = (time_t) value_64bit;
			}
		}
		else if( type_string_length == 1 )
		{
			/* Name
			 */
			if( libewf_string_compare(
			     types->values[ value_iterator ],
			     _LIBEWF_STRING( "n" ),
			     type_string_length ) == 0 )
			{
				single_file_entry->name = (libewf_character_t *) memory_allocate(
				                                                  sizeof( libewf_character_t ) * ( value_string_length + 1 ) );

				if( single_file_entry->name == NULL )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_MEMORY,
					 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
					 "%s: unable to create name.",
					 function );

					libewf_single_file_entry_free(
					 (intptr_t *) single_file_entry,
					 NULL );
					libewf_split_values_free(
					 &values,
					 NULL );

					return( -1 );
				}
				if( libewf_string_copy(
				     single_file_entry->name,
				     values->values[ value_iterator ],
				     value_string_length ) == NULL )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_MEMORY,
					 LIBERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set name.",
					 function );

					libewf_single_file_entry_free(
					 (intptr_t *) single_file_entry,
					 NULL );
					libewf_split_values_free(
					 &values,
					 NULL );

					return( -1 );
				}
				single_file_entry->name[ value_string_length ] = 0;

				single_file_entry->name_size = value_string_length + 1;
			}
			else if( libewf_string_compare(
				  types->values[ value_iterator ],
				  _LIBEWF_STRING( "p" ),
				  type_string_length ) == 0 )
			{
				/* p = 0 if directory
				 * p = emtpy if file
				 */
			}
		}
#if defined( HAVE_VERBOSE_OUTPUT )
		else if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: unsupported type: %s with value: %s.\n",
			 function,
			 (char *) types->values[ value_iterator ],
			 (char *) values->values[ value_iterator ] );
		}
#endif
	}
	if( libewf_split_values_free(
	     &values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free split values.",
		 function );

		libewf_single_file_entry_free(
		 (intptr_t *) single_file_entry,
		 NULL );

		return( -1 );
	}
	*line_iterator += 1;

	if( libewf_tree_node_set_value(
	     parent_file_entry_node,
	     (intptr_t *) single_file_entry,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set single file entry in node.",
		 function );

		libewf_single_file_entry_free(
		 (intptr_t *) single_file_entry,
		 NULL );

		return( -1 );
	}
	if( ( *line_iterator + amount_of_child_entries ) > (uint64_t) lines->amount_of_values )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: amount of child entries exceed the amount of available lines.",
		 function );

		return( -1 );
	}
	while( amount_of_child_entries > 0 )
	{
		if( libewf_tree_node_initialize(
		     &file_entry_node,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create single file entry node.",
			 function );

			return( -1 );
		}
		if( libewf_single_files_parse_file_entry(
		     file_entry_node,
		     lines,
		     line_iterator,
		     types,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_CONVERSION,
			 LIBERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to parse file entry.",
			 function );

			libewf_tree_node_free(
			 &file_entry_node,
			 &libewf_single_file_entry_free,
			 NULL );

			return( -1 );
		}
		if( libewf_tree_node_append_node(
		     parent_file_entry_node,
		     file_entry_node,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append single file entry node to parent.",
			 function );

			libewf_tree_node_free(
			 &file_entry_node,
			 &libewf_single_file_entry_free,
			 NULL );

			return( -1 );
		}
		file_entry_node = NULL;

		amount_of_child_entries--;
	}
	return( 1 );
}

