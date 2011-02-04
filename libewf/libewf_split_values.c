/*
 * Split string values functions
 *
 * Copyright (c) 2006-2011, Joachim Metz <jbmetz@users.sourceforge.net>
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

#include <libcstring.h>
#include <liberror.h>

#include "libewf_split_values.h"

/* Initializes the split values
 * Returns 1 if successful or -1 on error
 */
int libewf_split_values_initialize(
     libewf_split_values_t **split_values,
     const uint8_t *string,
     size_t string_length,
     int number_of_values,
     liberror_error_t **error )
{
	static char *function = "libewf_split_values_initialize";

	if( split_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid split values.",
		 function );

		return( 1 );
	}
	if( number_of_values < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid number of values less than zero.",
		 function );

		return( -1 );
	}
	if( *split_values == NULL )
	{
		*split_values = memory_allocate_structure(
		                 libewf_split_values_t );

		if( *split_values == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create split values.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     *split_values,
		     0,
		     sizeof( libewf_split_values_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear split values.",
			 function );

			memory_free(
			 *split_values );

			*split_values = NULL;

			return( -1 );
		}
		if( ( string != NULL )
		 && ( string_length > 0 ) )
		{
			( *split_values )->string = (uint8_t *) memory_allocate(
			                                         sizeof( uint8_t ) * ( string_length + 1 ) );

			if( ( *split_values )->string == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create string.",
				 function );

				goto on_error;
			}
			if( memory_copy(
			     ( *split_values )->string,
			     string,
			     sizeof( uint8_t ) * string_length ) == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to copy string.",
				 function );

				goto on_error;
			}
			( *split_values )->string[ string_length ] = 0;
		}
		if( number_of_values > 0 )
		{
			( *split_values )->values = (uint8_t **) memory_allocate(
			                                          sizeof( uint8_t * ) * number_of_values );

			if( ( *split_values )->values == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create values.",
				 function );

				goto on_error;
			}
			if( memory_set(
			     ( *split_values )->values,
			     0,
			     sizeof( uint8_t * ) * number_of_values ) == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear values.",
				 function );

				goto on_error;
			}
			( *split_values )->sizes = (size_t *) memory_allocate(
			                                       sizeof( size_t ) * number_of_values );

			if( ( *split_values )->sizes == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create sizes.",
				 function );

				goto on_error;
			}
			if( memory_set(
			     ( *split_values )->sizes,
			     0,
			     sizeof( size_t ) * number_of_values ) == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear sizes.",
				 function );

				goto on_error;
			}
		}
		( *split_values )->number_of_values = number_of_values;
	}
	return( 1 );

on_error:
	if( *split_values != NULL )
	{
		if( ( *split_values )->sizes != NULL )
		{
			memory_free(
			 ( *split_values )->sizes );
		}
		if( ( *split_values )->values != NULL )
		{
			memory_free(
			 ( *split_values )->values );
		}
		if( ( *split_values )->string != NULL )
		{
			memory_free(
			 ( *split_values )->string );
		}
		memory_free(
		 *split_values );

		*split_values = NULL;
	}
	return( -1 );
}

/* Frees the split values including elements
 * Returns 1 if successful or -1 on error
 */
int libewf_split_values_free(
     libewf_split_values_t **split_values,
     liberror_error_t **error )
{
	static char *function = "libewf_split_values_free";

	if( split_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid split values.",
		 function );

		return( 1 );
	}
	if( *split_values != NULL )
	{
		if( ( *split_values )->string != NULL )
		{
			memory_free(
			 ( *split_values )->string );
		}
		if( ( *split_values )->values != NULL )
		{
			memory_free(
			 ( *split_values )->values );
		}
		if( ( *split_values )->sizes != NULL )
		{
			memory_free(
			 ( *split_values )->sizes );
		}
		memory_free(
		 *split_values );

		*split_values = NULL;
	}
	return( 1 );
}

/* Determines the split values in a string using a delimiter character
 * Returns 1 if successful or -1 on error
 */
int libewf_split_values_parse_string(
     libewf_split_values_t **split_values,
     const uint8_t *string,
     size_t string_length,
     uint8_t delimiter,
     liberror_error_t **error )
{
	uint8_t *split_value_start = NULL;
	uint8_t *split_value_end   = NULL;
	uint8_t *string_end        = NULL;
	static char *function      = "libewf_split_values_parse_string";
	ssize_t split_value_length = 0;
	int number_of_split_values = 0;
	int split_value_iterator   = 0;

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
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid split values already set.",
		 function );

		return( -1 );
	}
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
	if( string_length > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid string length value exceeds maximum.",
		 function );

		return( -1 );
	}
	/* An empty string has no split values
	 */
	if( ( string[ 0 ] == 0 )
	 || ( string_length == 0 ) )
	{
		return( 1 );
	}
	/* Determine the number of split values
	 */
	split_value_start = (uint8_t *) string;
	string_end        = (uint8_t *) &( string[ string_length ] );

	do
	{
		split_value_end = split_value_start;

		while( split_value_end <= string_end )
		{
			if( ( split_value_end == string_end )
			 || ( *split_value_end == 0 ) )
			{
				split_value_end = NULL;

				break;
			}
			else if( *split_value_end == (char) delimiter )
			{
				break;
			}
			split_value_end++;
		}
		if( split_value_end > string_end )
		{
			break;
		}
		split_value_iterator++;

		if( split_value_end == NULL )
		{
			break;
		}
		if( split_value_end == split_value_start )
		{
			split_value_start++;
		}
		else if( split_value_end != string )
		{
			split_value_start = split_value_end + 1;
		}
	}
	while( split_value_end != NULL );

	number_of_split_values = split_value_iterator;

	if( libewf_split_values_initialize(
	     split_values,
	     string,
	     string_length,
	     number_of_split_values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to intialize split values.",
		 function );

		return( -1 );
	}
	/* Do not bother splitting empty strings
	 */
	if( number_of_split_values == 0 )
	{
		return( 1 );
	}
	/* Determine the split values
	 * empty values are stored as strings only containing the end of character
	 */
	split_value_start = ( *split_values )->string;
	split_value_end   = ( *split_values )->string;
	string_end        = &( ( *split_values )->string[ string_length ] );

	for( split_value_iterator = 0;
	     split_value_iterator < number_of_split_values;
	     split_value_iterator++ )
	{
		split_value_end = split_value_start;

		while( split_value_end <= string_end )
		{
			if( ( split_value_end == string_end )
			 || ( *split_value_end == 0 ) )
			{
				split_value_end = NULL;

				break;
			}
			else if( *split_value_end == (char) delimiter )
			{
				break;
			}
			split_value_end++;
		}
		if( split_value_end == NULL )
		{
			split_value_length = (ssize_t) ( string_end - split_value_start );
		}
		else
		{
			split_value_length = (ssize_t) ( split_value_end - split_value_start );
		}
		if( split_value_length >= 0 )
		{
			( *split_values )->values[ split_value_iterator ] = split_value_start;
			( *split_values )->sizes[ split_value_iterator ]  = split_value_length + 1;

			( ( *split_values )->values[ split_value_iterator ] )[ split_value_length ] = 0;
		}
		if( split_value_end == NULL )
		{
			break;
		}
		if( split_value_end == ( *split_values )->string )
		{
			split_value_start++;
		}
		if( split_value_end != ( *split_values )->string )
		{
			split_value_start = split_value_end + 1;
		}
	}
	return( 1 );
}

