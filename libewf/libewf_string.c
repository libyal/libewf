/*
 * Character type string functions
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

#include "libewf_string.h"

/* Split a string into elements using a delimiter character
 * Returns 1 if successful or -1 on error
 */
int libewf_string_split(
     libewf_character_t *string,
     size_t string_size,
     libewf_character_t delimiter,
     libewf_character_t ***split_values,
     size_t *amount_of_split_values,
     liberror_error_t **error )
{
	libewf_character_t *split_value_start = NULL;
	libewf_character_t *split_value_end   = NULL;
	libewf_character_t *string_end        = NULL;
	static char *function                 = "libewf_string_split";
	size_t remaining_string_size          = 0;
	size_t split_value_iterator           = 0;
	ssize_t split_value_size              = 0;

	if( string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( string_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( split_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid split values.",
		 function );

		return( -1 );
	}
	if( *split_values != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: split values already set.",
		 function );

		return( -1 );
	}
	if( amount_of_split_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid amount of split values.",
		 function );

		return( -1 );
	}
	/* Include the last character if not the end of string character
	 */
	if( string[ string_size - 1 ] != 0 )
	{
		string_size += 1;
	}
	/* Do not bother with empty strings
	 */
	if( ( string_size == 0 )
	 || ( string[ 0 ] == 0 ) )
	{
		*split_values           = NULL;
		*amount_of_split_values = 0;

		return( 1 );
	}
	/* Determine the amount of split values
	 */
	remaining_string_size = string_size;
	split_value_start     = string;
	split_value_end       = string;
	string_end            = &string[ string_size - 1 ];

	do
	{
		split_value_end = libewf_string_search(
		                   split_value_start,
		                   delimiter,
		                   remaining_string_size );

		if( split_value_end > string_end )
		{
			break;
		}
		split_value_iterator++;

		if( split_value_end == NULL )
		{
			break;
		}
		/* Include delimiter character
		 */
		remaining_string_size -= (size_t) ( split_value_end - split_value_start ) + 1;

		if( split_value_end == split_value_start )
		{
			split_value_start += 1;
		}
		else if( split_value_end != string )
		{
			split_value_start = split_value_end + 1;
		}
	}
	while( split_value_end != NULL );

	*amount_of_split_values = split_value_iterator;

	if( *amount_of_split_values == 0 )
	{
		*split_values = NULL;

		return( 1 );
	}
	*split_values = (libewf_character_t **) memory_allocate(
	                                         sizeof( libewf_character_t * ) * *amount_of_split_values );

	if( *split_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: unable to create split values.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     *split_values,
	     0,
	     sizeof( libewf_character_t * ) * *amount_of_split_values ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: unable to clear split values.",
		 function );

		memory_free(
		 *split_values );

		*split_values = NULL;

		return( -1 );
	}
	/* Determine the split values
	 */
	remaining_string_size = string_size;
	split_value_start     = string;
	split_value_end       = string;

	/* Empty values are stored as strings only containing the end of character
	 */
	for( split_value_iterator = 0;
	     split_value_iterator < *amount_of_split_values;
	     split_value_iterator++ )
	{
		if( split_value_end != string )
		{
			split_value_start = split_value_end + 1;
		}
		split_value_end = libewf_string_search(
		                   split_value_start,
		                   delimiter,
		                   remaining_string_size );

		/* Check for last value
		 */
		if( split_value_end == NULL )
		{
			split_value_size = (ssize_t) ( string_end - split_value_start );
		}
		else
		{
			split_value_size = (ssize_t) ( split_value_end - split_value_start );
		}
		if( split_value_size >= 0 )
		{
			/* Add 1 additional byte required for the end of string character
			 */
			split_value_size += 1;

			( *split_values )[ split_value_iterator ] = (libewf_character_t *) memory_allocate(
										            sizeof( libewf_character_t ) * split_value_size );

			if( ( *split_values )[ split_value_iterator ] == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
				 "%s: unable to create split value: %" PRIzd ".",
				 function,
				 split_value_iterator );

				libewf_string_split_values_free(
				 *split_values,
				 ( split_value_iterator - 1 ),
				 NULL );

				*split_values = NULL;

				return( -1 );
			}
			if( libewf_string_copy(
			     ( *split_values )[ split_value_iterator ],
			     split_value_start,
			     split_value_size ) == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
				 "%s: unable to set split value: %" PRIzd ".",
				 function,
				 split_value_iterator );

				libewf_string_split_values_free(
				 *split_values,
				 split_value_iterator,
				 NULL );

				*split_values = NULL;

				return( -1 );
			}
			( ( *split_values )[ split_value_iterator ] )[ split_value_size - 1 ] = 0;
		}
		/* Include delimiter character
		 */
		remaining_string_size -= (size_t) ( split_value_end - split_value_start ) + 1;

		/* Correct if first value is empty
		 */
		if( split_value_end == string )
		{
			split_value_start += 1;
		}
	}
	return( 1 );
}

/* Clears a split values array
 * Returns 1 if successful or -1 on error
 */
int libewf_string_split_values_free(
     libewf_character_t **split_values,
     size_t amount_of_split_values,
     liberror_error_t **error )
{
	static char *function       = "libewf_string_split_values_free";
	size_t split_value_iterator = 0;

	if( split_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid split values array.",
		 function );

		return( -1 );
	}
	for( split_value_iterator = 0;
	     split_value_iterator < amount_of_split_values;
	     split_value_iterator++ )
	{
		if( split_values[ split_value_iterator ] != NULL )
		{
			memory_free(
			 split_values[ split_value_iterator ] );
		}
#if defined( HAVE_VERBOSE_OUTPUT )
		else
		{
			libnotify_verbose_printf(
			 "%s: empty split value: %" PRIzd " out of: %" PRIzd ".\n",
			 function,
			 split_value_iterator,
			 amount_of_split_values );
		}
#endif
	}
	memory_free(
	 split_values );

	return( 1 );
}

