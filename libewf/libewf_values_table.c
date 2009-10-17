/*
 * Values table functions
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
#include <narrow_string.h>
#include <memory.h>
#include <types.h>
#include <wide_string.h>

#include "libewf_definitions.h"
#include "libewf_values_table.h"

/* Initializes the values table
 * Returns 1 if successful or -1 on error
 */
int libewf_values_table_initialize(
     libewf_values_table_t **values_table,
     int amount_of_values,
     liberror_error_t **error )
{
	static char *function           = "libewf_values_table_initialize";
	size_t values_table_size        = 0;
	size_t values_table_string_size = 0;

	if( values_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values table.",
		 function );

		return( 1 );
	}
	if( amount_of_values < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid amount of values less than zero.",
		 function );

		return( -1 );
	}
	if( *values_table == NULL )
	{
		values_table_string_size = amount_of_values * sizeof( uint8_t * );

		if( values_table_string_size > (size_t) SSIZE_MAX )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid values table string size value exceeds maximum.",
			 function );

			return( -1 );
		}
		values_table_size = amount_of_values * sizeof( size_t );

		if( values_table_size > (size_t) SSIZE_MAX )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid values table size value exceeds maximum.",
			 function );

			return( -1 );
		}
		*values_table = (libewf_values_table_t *) memory_allocate(
		                                           sizeof( libewf_values_table_t ) );

		if( *values_table == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create values table.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *values_table,
		     0,
		     sizeof( libewf_values_table_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear values table.",
			 function );

			memory_free(
			 *values_table );

			*values_table = NULL;

			return( -1 );
		}
		if( amount_of_values > 0 )
		{
			( *values_table )->identifier = (uint8_t **) memory_allocate(
								      values_table_string_size );

			if( ( *values_table )->identifier == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create identifiers.",
				 function );

				memory_free(
				 *values_table );

				*values_table = NULL;

				return( -1 );
			}
			if( memory_set(
			     ( *values_table )->identifier,
			     0,
			     values_table_string_size ) == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear identifiers.",
				 function );

				memory_free(
				 ( *values_table )->identifier );
				memory_free(
				 *values_table );

				*values_table = NULL;

				return( -1 );
			}
			( *values_table )->identifier_length = (size_t *) memory_allocate(
									   values_table_size );

			if( ( *values_table )->identifier_length == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create identifier lengths.",
				 function );

				memory_free(
				 ( *values_table )->identifier );
				memory_free(
				 *values_table );

				*values_table = NULL;

				return( -1 );
			}
			if( memory_set(
			     ( *values_table )->identifier_length,
			     0,
			     values_table_size ) == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear identifier lengths.",
				 function );

				memory_free(
				 ( *values_table )->identifier_length );
				memory_free(
				 ( *values_table )->identifier );
				memory_free(
				 *values_table );

				*values_table = NULL;

				return( -1 );
			}
			( *values_table )->value = (uint8_t **) memory_allocate(
								 values_table_string_size );

			if( ( *values_table )->value == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create values.",
				 function );

				memory_free(
				 ( *values_table )->identifier_length );
				memory_free(
				 ( *values_table )->identifier );
				memory_free(
				 *values_table );

				*values_table = NULL;

				return( -1 );
			}
			if( memory_set(
			     ( *values_table )->value,
			     0,
			     values_table_string_size ) == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear values.",
				 function );

				memory_free(
				 ( *values_table )->value );
				memory_free(
				 ( *values_table )->identifier_length );
				memory_free(
				 ( *values_table )->identifier );
				memory_free(
				 *values_table );

				*values_table = NULL;

				return( -1 );
			}
			( *values_table )->value_length = (size_t *) memory_allocate(
			                                              values_table_size );

			if( ( *values_table )->value_length == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create value lengths.",
				 function );

				memory_free(
				 ( *values_table )->value );
				memory_free(
				 ( *values_table )->identifier_length );
				memory_free(
				 ( *values_table )->identifier );
				memory_free(
				 *values_table );

				*values_table = NULL;

				return( -1 );
			}
			if( memory_set(
			     ( *values_table )->value_length,
			     0,
			     values_table_size ) == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear value lengths.",
				 function );

				memory_free(
				 ( *values_table )->value_length );
				memory_free(
				 ( *values_table )->value );
				memory_free(
				 ( *values_table )->identifier_length );
				memory_free(
				 ( *values_table )->identifier );
				memory_free(
				 *values_table );

				*values_table = NULL;

				return( -1 );
			}
		}
		( *values_table )->amount_of_values = amount_of_values;
	}
	return( 1 );
}

/* Frees the values table including elements
 * Returns 1 if successful or -1 on error
 */
int libewf_values_table_free(
     libewf_values_table_t **values_table,
     liberror_error_t **error )
{
	static char *function     = "libewf_values_table_free";
	int values_table_iterator = 0;

	if( values_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values table.",
		 function );

		return( 1 );
	}
	if( *values_table != NULL )
	{
		for( values_table_iterator = 0;
		     values_table_iterator < ( *values_table )->amount_of_values;
		     values_table_iterator++ )
		{
			if( ( *values_table )->identifier[ values_table_iterator ] != NULL )
			{
				memory_free(
				 ( *values_table )->identifier[ values_table_iterator ] );
			}
			if( ( *values_table )->value[ values_table_iterator ] != NULL )
			{
				memory_free(
				 ( *values_table )->value[ values_table_iterator ] );
			}
		}
		if( ( *values_table )->identifier != NULL )
		{
			memory_free(
			 ( *values_table )->identifier );
		}
		if( ( *values_table )->identifier_length != NULL )
		{
			memory_free(
			 ( *values_table )->identifier_length );
		}
		if( ( *values_table )->value != NULL )
		{
			memory_free(
			 ( *values_table )->value );
		}
		if( ( *values_table )->value_length != NULL )
		{
			memory_free(
			 ( *values_table )->value_length );
		}
		memory_free(
		 *values_table );

		*values_table = NULL;
	}
	return( 1 );
}

/* Resizes the values table
 * Returns 1 if successful or -1 on error
 */
int libewf_values_table_resize(
     libewf_values_table_t *values_table,
     int amount_of_values,
     liberror_error_t **error )
{
	static char *function           = "libewf_values_table_resize";
	void *reallocation              = NULL;
	size_t values_table_size        = 0;
	size_t values_table_string_size = 0;

	if( values_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values table.",
		 function );

		return( -1 );
	}
	if( amount_of_values < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid amount of values less than zero.",
		 function );

		return( -1 );
	}
	if( values_table->amount_of_values < amount_of_values )
	{
		values_table_string_size = amount_of_values * sizeof( uint8_t * );

		if( values_table_string_size > (ssize_t) SSIZE_MAX )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid values table string size value exceeds maximum.",
			 function );

			return( -1 );
		}
		values_table_size = amount_of_values * sizeof( size_t );

		if( values_table_size > (ssize_t) SSIZE_MAX )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid values table size value exceeds maximum.",
			 function );

			return( -1 );
		}
		reallocation = memory_reallocate(
		                values_table->identifier,
		                values_table_string_size );

		if( reallocation == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to resize identifiers.",
			 function );

			return( -1 );
		}
		values_table->identifier = (uint8_t **) reallocation;

		if( memory_set(
		     &( values_table->identifier[ values_table->amount_of_values ] ),
		     0,
		     sizeof( uint8_t * ) * ( amount_of_values - values_table->amount_of_values ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear identifiers.",
			 function );

			return( -1 );
		}
		reallocation = memory_reallocate(
		                values_table->identifier_length,
		                values_table_size );

		if( reallocation == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to resize identifier lengths.",
			 function );

			return( -1 );
		}
		values_table->identifier_length = (size_t *) reallocation;

		if( memory_set(
		     &( values_table->identifier_length[ values_table->amount_of_values ] ),
		     0,
		     sizeof( size_t ) * ( amount_of_values - values_table->amount_of_values ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear identifier lengths.",
			 function );

			return( -1 );
		}
		reallocation = memory_reallocate(
		                values_table->value,
		                values_table_string_size );

		if( reallocation == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to resize values.",
			 function );

			return( -1 );
		}
		values_table->value = (uint8_t **) reallocation;

		if( memory_set(
		     &( values_table->value[ values_table->amount_of_values ] ),
		     0,
		     sizeof( uint8_t * ) * ( amount_of_values - values_table->amount_of_values ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear values.",
			 function );

			return( -1 );
		}
		reallocation = memory_reallocate(
		                values_table->value_length,
		                values_table_size );

		if( reallocation == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to resize value lengths.",
			 function );

			return( -1 );
		}
		values_table->value_length = (size_t *) reallocation;

		if( memory_set(
		     &( values_table->value_length[ values_table->amount_of_values ] ),
		     0,
		     sizeof( size_t ) * ( amount_of_values - values_table->amount_of_values ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear value sizes.",
			 function );

			return( -1 );
		}
		values_table->amount_of_values = amount_of_values;
	}
	return( 1 );
}

/* Retrieves the amount of values
 * Returns 1 if successful or -1 on error
 */
int libewf_values_table_get_amount_of_values(
     libewf_values_table_t *values_table,
     int *amount_of_values,
     liberror_error_t **error )
{
	static char *function = "libewf_values_table_get_amount_of_values";

	if( values_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values table.",
		 function );

		return( -1 );
	}
	if( amount_of_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid amount of values.",
		 function );

		return( -1 );
	}
	*amount_of_values = values_table->amount_of_values;

	return( 1 );
}

/* Retrieves the index for a certain identifier
 * Returns 1 if successful, 0 if no index was found or -1 on error
 */
int libewf_values_table_get_index(
     libewf_values_table_t *values_table,
     const uint8_t *identifier,
     size_t identifier_length,
     int *index,
     liberror_error_t **error )
{
	static char *function     = "libewf_values_table_get_index";
	int values_table_iterator = 0;

	if( values_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values table.",
		 function );

		return( -1 );
	}
	if( values_table->amount_of_values < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid values table amount value less than zero.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	if( identifier_length == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid identifier length value is zero.",
		 function );

		return( -1 );
	}
	if( identifier_length > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid identifier length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( index == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index.",
		 function );

		return( -1 );
	}
	for( values_table_iterator = 0;
	     values_table_iterator < values_table->amount_of_values;
	     values_table_iterator++ )
	{
		if( values_table->identifier[ values_table_iterator ] == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing identifier for index: %d.",
			 function,
			 values_table_iterator );

			continue;
		}
		if( values_table->identifier_length[ values_table_iterator ] == 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing identifier length for index: %d.",
			 function,
			 values_table_iterator );

			continue;
		}
		if( values_table->identifier_length[ values_table_iterator ] != identifier_length )
		{
			continue;
		}
		if( narrow_string_compare(
		     (char *) identifier,
		     (char *) values_table->identifier[ values_table_iterator ],
		     identifier_length ) == 0 )
		{
			*index = values_table_iterator;

			return( 1 );
		}
	}
	return( 0 );
}

/* Retrieves an identifiers size in the values table
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_values_table_get_identifier_size(
     libewf_values_table_t *values_table,
     int index,
     size_t *identifier_size,
     liberror_error_t **error )
{
	static char *function = "libewf_values_table_get_identifier_size";

	if( values_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values table.",
		 function );

		return( -1 );
	}
	if( identifier_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier size.",
		 function );

		return( -1 );
	}
	if( values_table->amount_of_values == 0 )
	{
		return( 0 );
	}
	if( ( index < 0 )
	 || ( index >= values_table->amount_of_values ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: index out of range.",
		 function );

		return( -1 );
	}
	if( values_table->identifier[ index ] == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing identifier for index: %d.",
		 function,
		 index );

		return( -1 );
	}
	if( values_table->identifier_length[ index ] == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing identifier size for length: %d.",
		 function,
		 index );

		return( -1 );
	}
	*identifier_size = values_table->identifier_length[ index ] + 1;

	return( 1 );
}

/* Retrieves an identifier in the values table
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_values_table_get_identifier(
     libewf_values_table_t *values_table,
     int index,
     uint8_t *identifier,
     size_t identifier_size,
     liberror_error_t **error )
{
	static char *function         = "libewf_values_table_get_identifier";
	size_t narrow_identifier_size = 0;

	if( values_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values table.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	if( identifier_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid identifier size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( values_table->amount_of_values == 0 )
	{
		return( 0 );
	}
	if( ( index < 0 )
	 || ( index >= values_table->amount_of_values ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: index out of range.",
		 function );

		return( -1 );
	}
	if( values_table->identifier[ index ] == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing identifier for index: %d.",
		 function,
		 index );

		return( -1 );
	}
	if( values_table->identifier_length[ index ] == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing identifier size for length: %d.",
		 function,
		 index );

		return( -1 );
	}
	narrow_identifier_size = values_table->identifier_length[ index ] + 1;

	if( identifier_size < narrow_identifier_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: identifier too small.",
		 function );

		return( -1 );
	}
	if( narrow_string_copy(
	     (char *) identifier,
	     (char *) values_table->identifier[ index ],
	     values_table->identifier_length[ index ] ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set identifier.",
		 function );

		return( -1 );
	}
	identifier[ values_table->identifier_length[ index ] ] = 0;

	return( 1 );
}

/* Set a value in the values table
 * Frees the previous value if necessary
 * Returns 1 if successful or -1 on error
 */
int libewf_values_table_set_identifier(
     libewf_values_table_t *values_table,
     int index,
     const uint8_t *identifier,
     size_t identifier_length,
     liberror_error_t **error )
{
	static char *function = "libewf_values_table_set_identifier";

	if( values_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values table.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	if( identifier_length == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid identifier length value is zero.",
		 function );

		return( -1 );
	}
	if( identifier_length > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid identifier length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( values_table->amount_of_values == 0 )
	 || ( index < 0 )
	 || ( index >= values_table->amount_of_values ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid index out of range.",
		 function );

		return( -1 );
	}
	/* Remove exisiting identifiers
	 */
	if( values_table->identifier[ index ] != NULL )
	{
		memory_free(
		 values_table->identifier[ index ] );

		values_table->identifier[ index ] = NULL;
	}
	/* Do not include the end of string character in the identifier length
	 */
	if( identifier[ identifier_length - 1 ] == 0 )
	{
		identifier_length -= 1;
	}
	values_table->identifier_length[ index ] = identifier_length;

	values_table->identifier[ index ] = (uint8_t *) memory_allocate(
							 sizeof( uint8_t ) * ( values_table->identifier_length[ index ] + 1 ) );

	if( values_table->identifier[ index ] == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create identifier.",
		 function );

		return( -1 );
	}
	if( narrow_string_copy(
	     (char *) values_table->identifier[ index ],
	     (char *) identifier,
	     identifier_length ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set identifier.",
		 function );

		memory_free(
		 values_table->identifier[ index ] );

		values_table->identifier[ index ]        = NULL;
		values_table->identifier_length[ index ] = 0;

		return( -1 );
	}
	( values_table->identifier[ index ] )[ identifier_length ] = 0;

	return( 1 );
}

/* Retrieves a values size in the values table
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_values_table_get_value_size(
     libewf_values_table_t *values_table,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *value_size,
     liberror_error_t **error )
{
	static char *function = "libewf_values_table_get_value_size";
	int index             = 0;
	int result            = 0;

	if( value_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value size.",
		 function );

		return( -1 );
	}
	result = libewf_values_table_get_index(
	          values_table,
	          identifier,
	          identifier_length,
	          &index,
	          error );

	if( result <= -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to find index for: %s.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	if( ( values_table->value[ index ] == NULL )
	 || ( values_table->value_length[ index ] == 0 ) )
	{
		return( 0 );
	}
	*value_size = values_table->value_length[ index ] + 1;

	return( 1 );
}

/* Retrieves a value in the values table
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_values_table_get_value(
     libewf_values_table_t *values_table,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *value,
     size_t value_size,
     liberror_error_t **error )
{
	static char *function    = "libewf_values_table_get_value";
	size_t narrow_value_size = 0;
	int index                = 0;
	int result               = 0;

	if( value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	if( value_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid value size value exceeds maximum.",
		 function );

		return( -1 );
	}
	result = libewf_values_table_get_index(
	          values_table,
	          identifier,
	          identifier_length,
	          &index,
	          error );

	if( result <= -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to find index for: %s.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	if( ( values_table->value[ index ] == NULL )
	 || ( values_table->value_length[ index ] == 0 ) )
	{
		return( 0 );
	}
	narrow_value_size = values_table->value_length[ index ] + 1;

	if( value_size < narrow_value_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: value too small.",
		 function );

		return( -1 );
	}
	if( narrow_string_copy(
	     (char *) value,
	     (char *) values_table->value[ index ],
	     values_table->value_length[ index ] ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set value.",
		 function );

		return( -1 );
	}
	value[ values_table->value_length[ index ] ] = 0;

	return( 1 );
}

/* Set a value in the values table
 * Frees the previous value if necessary
 * Returns 1 if successful or -1 on error
 */
int libewf_values_table_set_value(
     libewf_values_table_t *values_table,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *value,
     size_t value_length,
     liberror_error_t **error )
{
	static char *function = "libewf_values_table_set_value";
	int index             = 0;
	int result            = 0;

	result = libewf_values_table_get_index(
	          values_table,
	          identifier,
	          identifier_length,
	          &index,
	          error );

	if( result <= -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to find index for: %s.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	else if( result == 0 )
	{
		index = values_table->amount_of_values;

		if( libewf_values_table_resize(
		     values_table,
		     index + 1,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize values table.",
			 function );

			return( -1 );
		}
		if( libewf_values_table_set_identifier(
		     values_table,
		     index,
		     identifier,
		     identifier_length,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set identifier.",
			 function );

			return( -1 );
		}
	}
	/* Remove exisiting values
	 */
	if( values_table->value[ index ] != NULL )
	{
		memory_free(
		 values_table->value[ index ] );

		values_table->value[ index ] = NULL;
	}
	if( ( value != NULL )
	 && ( value[ 0 ] != 0 )
	 && ( value_length > 0 ) )
	{
		/* Do not include the end of string character in the value length
		 */
		if( value[ value_length - 1 ] == 0 )
		{
			value_length -= 1;
		}
		values_table->value_length[ index ] = value_length;

		values_table->value[ index ] = (uint8_t *) memory_allocate(
		                                            sizeof( uint8_t ) * ( values_table->value_length[ index ] + 1 ) );

		if( values_table->value[ index ] == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create value.",
			 function );

			return( -1 );
		}
		if( narrow_string_copy(
		     (char *) values_table->value[ index ],
		     (char *) value,
		     value_length ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set value.",
			 function );

			memory_free(
			 values_table->value[ index ] );

			values_table->value[ index ]        = NULL;
			values_table->value_length[ index ] = 0;

			return( -1 );
		}
		( values_table->value[ index ] )[ value_length ] = 0;
	}
	return( 1 );
}

