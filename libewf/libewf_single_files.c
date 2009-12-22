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
		/* TODO free tree */

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
	libewf_split_values_t *lines        = NULL;
	libewf_split_values_t *types        = NULL;
	libewf_tree_node_t *file_entry_node = NULL;
	static char *function               = "libewf_single_files_parse_file_entries";
	int line_iterator                   = 0;

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

			while( line_iterator < lines->amount_of_values )
			{
				/* The single files entries should be followed by an emtpy line
				 */
				if( ( lines->sizes[ line_iterator ] == 1 )
				 && ( ( lines->values[ line_iterator ] )[ 0 ] == 0 ) )
				{
					break;
				}
				if( libewf_single_files_parse_file_entry(
				     file_entry_node,
				     lines,
				     &line_iterator,
				     types,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_CONVERSION,
					 LIBERROR_CONVERSION_ERROR_GENERIC,
					 "%s: unable to parse file entries string.",
					 function );

					libewf_split_values_free(
					 &types,
					 NULL );
					libewf_split_values_free(
					 &lines,
					 NULL );

					return( -1 );
				}
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
     libewf_tree_node_t *file_entry_node,
     libewf_split_values_t *lines,
     int *line_iterator,
     libewf_split_values_t *types,
     liberror_error_t **error )
{
	libewf_character_t *date_time_values_string  = NULL;
	libewf_split_values_t *values                = NULL;
	static char *function                        = "libewf_single_files_parse_file_entry";
	size_t date_time_values_string_size          = 0;
	size_t type_string_length                    = 0;
	size_t value_string_length                   = 0;
	int value_iterator                           = 0;
	int result                                   = 0;

/* TODO
	if( file_entry_node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry tree node.",
		 function );

		return( 1 );
	}
*/
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
	/* TODO determine amount of child values
	 */
fprintf( stderr, "\nChilds\t: %s\n", values->values[ 1 ] );

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
			if( libewf_string_compare(
			     types->values[ value_iterator ],
			     _LIBEWF_STRING( "ac" ),
			     type_string_length ) == 0 )
			{
			}
			else if( libewf_string_compare(
				  types->values[ value_iterator ],
				  _LIBEWF_STRING( "aq" ),
				  type_string_length ) == 0 )
			{
			}
			else if( libewf_string_compare(
				  types->values[ value_iterator ],
				  _LIBEWF_STRING( "be" ),
				  type_string_length ) == 0 )
			{
				/* data segment offset and size*/
			}
			else if( libewf_string_compare(
				  types->values[ value_iterator ],
				  _LIBEWF_STRING( "cr" ),
				  type_string_length ) == 0 )
			{
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
			else if( libewf_string_compare(
				  types->values[ value_iterator ],
				  _LIBEWF_STRING( "ha" ),
				  type_string_length ) == 0 )
			{
				/* MD5 hash */
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
			else if( libewf_string_compare(
				  types->values[ value_iterator ],
				  _LIBEWF_STRING( "ls" ),
				  type_string_length ) == 0 )
			{
				/* file size */
			}
			else if( libewf_string_compare(
				  types->values[ value_iterator ],
				  _LIBEWF_STRING( "mo" ),
				  type_string_length ) == 0 )
			{
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
			else if( libewf_string_compare(
				  types->values[ value_iterator ],
				  _LIBEWF_STRING( "wr" ),
				  type_string_length ) == 0 )
			{
			}
		}
		else if( type_string_length == 1 )
		{
			if( libewf_string_compare(
			     types->values[ value_iterator ],
			     _LIBEWF_STRING( "n" ),
			     type_string_length ) == 0 )
			{
				/* Filename */
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
		else
		{
			libnotify_verbose_printf(
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

		return( -1 );
	}
	*line_iterator += 1;

	return( 1 );
}

