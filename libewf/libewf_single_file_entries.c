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
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#include <liberror.h>
#include <libnotify.h>

#include "libewf_definitions.h"
#include "libewf_single_file_entries.h"
#include "libewf_libuna.h"
#include "libewf_split_values.h"
#include "libewf_string.h"

/* Parse a single file entries string for the values
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entries_parse_entries_string(
     libewf_character_t *entries_string,
     size_t entries_string_size,
     liberror_error_t **error )
{
	libewf_split_values_t *lines                 = NULL;
	libewf_split_values_t *types                 = NULL;
	libewf_split_values_t *values                = NULL;

	libewf_character_t *date_time_values_string  = NULL;
	static char *function                        = "libewf_single_file_entries_parse_entries_string";
	size_t date_time_values_string_size          = 0;
	size_t type_string_length                    = 0;
	size_t value_string_length                   = 0;
	int line_iterator                            = 0;
	int value_iterator                           = 0;
	int result                                   = 0;

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
		/* Find the line with: "entry"
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
			for( line_iterator += 1;
			     line_iterator < lines->amount_of_values;
			     line_iterator++ )
			{
				if( libewf_split_values_parse_string(
				     &values,
				     lines->values[ line_iterator ],
				     lines->sizes[ line_iterator ],
				     (libewf_character_t) '\t',
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to split entries string into values.",
					 function );

					libewf_split_values_free(
					 &types,
					 NULL );
					libewf_split_values_free(
					 &lines,
					 NULL );

					return( -1 );
				}
				if( ( values->sizes[ 0 ] == 2 )
				 && ( ( values->values[ 0 ] )[ 0 ] == '0' ) )
				{
fprintf( stderr, "\nChilds\t: %s\n", values->values[ 1 ] );
				}
				if( types->amount_of_values == values->amount_of_values )
				{
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

					libewf_split_values_free(
					 &values,
					 NULL );
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

/* Parse an EWF ltree for the values
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entries_parse_ltree(
     uint8_t *ltree,
     size_t ltree_size,
     liberror_error_t **error )
{
	libewf_character_t *entries_string = NULL;
	static char *function              = "libewf_single_file_entries_parse_ltree";
	size_t entries_string_size         = 0;
	int result                         = 0;

	if( ltree == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid ltree.",
		 function );

		return( -1 );
	}
	if( libewf_string_size_from_utf16_stream(
	     ltree,
	     ltree_size,
	     LIBUNA_ENDIAN_LITTLE,
	     &entries_string_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine entries string size.",
		 function );

		return( -1 );
	}
	entries_string = (libewf_character_t *) memory_allocate(
	                                         sizeof( libewf_character_t ) * (size_t) entries_string_size );

	if( entries_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create entries string.",
		 function );

		return( -1 );
	}
	if( libewf_string_copy_from_utf16_stream(
	     entries_string,
	     entries_string_size,
	     ltree,
	     ltree_size,
	     LIBUNA_ENDIAN_LITTLE,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set entries string.",
		 function );

		memory_free(
		 entries_string );

		return( -1 );
	}
	result = libewf_single_file_entries_parse_entries_string(
	          entries_string,
	          entries_string_size,
	          error );

	memory_free(
	 entries_string );

	if( result != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse entries string.",
		 function );
	}
	return( result );
}

