/*
 * Values table functions
 *
 * Copyright (c) 2006-2008, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
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
#include <character_string.h>
#include <memory.h>
#include <notify.h>
#include <types.h>

#include <libewf/definitions.h>

#include "libewf_values_table.h"
#include "libewf_string.h"

/* Initializes the values table
 * Returns 1 if successful or -1 on error
 */
int libewf_values_table_initialize(
     libewf_values_table_t **values_table,
     int amount_of_values )
{
	static char *function           = "libewf_values_table_initialize";
	size_t values_table_size        = 0;
	size_t values_table_string_size = 0;

	if( values_table == NULL )
	{
		notify_warning_printf( "%s: invalid values table.\n",
		 function );

		return( 1 );
	}
	if( amount_of_values < 0 )
	{
		notify_warning_printf( "%s: invalid amount of values less than zero.\n",
		 function );

		return( -1 );
	}
	if( *values_table == NULL )
	{
		values_table_string_size = amount_of_values * sizeof( character_t * );

		if( values_table_string_size > (size_t) SSIZE_MAX )
		{
			notify_warning_printf( "%s: invalid values table string size value exceeds maximum.\n",
			 function );

			return( -1 );
		}
		values_table_size = amount_of_values * sizeof( size_t );

		if( values_table_size > (size_t) SSIZE_MAX )
		{
			notify_warning_printf( "%s: invalid values table size value exceeds maximum.\n",
			 function );

			return( -1 );
		}
		*values_table = (libewf_values_table_t *) memory_allocate(
		                                           sizeof( libewf_values_table_t ) );

		if( *values_table == NULL )
		{
			notify_warning_printf( "%s: unable to create values table.\n",
			 function );

			return( -1 );
		}
		( *values_table )->identifier = (character_t **) memory_allocate(
		                                                  values_table_string_size );

		if( ( *values_table )->identifier == NULL )
		{
			notify_warning_printf( "%s: unable to create identifiers.\n",
			 function );

			memory_free(
			 *values_table );

			return( -1 );
		}
		if( memory_set(
		     ( *values_table )->identifier,
		     0,
		     values_table_string_size ) == NULL )
		{
			notify_warning_printf( "%s: unable to clear identifiers.\n",
			 function );

			memory_free(
			 ( *values_table )->identifier );
			memory_free(
			 *values_table );

			return( -1 );
		}
		( *values_table )->identifier_length = (size_t *) memory_allocate(
		                                                   values_table_size );

		if( ( *values_table )->identifier_length == NULL )
		{
			notify_warning_printf( "%s: unable to create identifier lengths.\n",
			 function );

			memory_free(
			 ( *values_table )->identifier );
			memory_free(
			 *values_table );

			return( -1 );
		}
		if( memory_set(
		     ( *values_table )->identifier_length,
		     0,
		     values_table_size ) == NULL )
		{
			notify_warning_printf( "%s: unable to clear identifier lengths.\n",
			 function );

			memory_free(
			 ( *values_table )->identifier_length );
			memory_free(
			 ( *values_table )->identifier );
			memory_free(
			 *values_table );

			return( -1 );
		}
		( *values_table )->value = (character_t **) memory_allocate(
		                                             values_table_string_size );

		if( ( *values_table )->value == NULL )
		{
			notify_warning_printf( "%s: unable to create values.\n",
			 function );

			memory_free(
			 ( *values_table )->identifier_length );
			memory_free(
			 ( *values_table )->identifier );
			memory_free(
			 *values_table );

			return( -1 );
		}
		if( memory_set(
		     ( *values_table )->value,
		     0,
		     values_table_string_size ) == NULL )
		{
			notify_warning_printf( "%s: unable to clear values.\n",
			 function );

			memory_free(
			 ( *values_table )->value );
			memory_free(
			 ( *values_table )->identifier_length );
			memory_free(
			 ( *values_table )->identifier );
			memory_free(
			 *values_table );

			return( -1 );
		}
		( *values_table )->value_length = (size_t *) memory_allocate(
		                                            values_table_size );

		if( ( *values_table )->value_length == NULL )
		{
			notify_warning_printf( "%s: unable to create value lengths.\n",
			 function );

			memory_free(
			 ( *values_table )->value );
			memory_free(
			 ( *values_table )->identifier_length );
			memory_free(
			 ( *values_table )->identifier );
			memory_free(
			 *values_table );

			return( -1 );
		}
		if( memory_set(
		     ( *values_table )->value_length,
		     0,
		     values_table_size ) == NULL )
		{
			notify_warning_printf( "%s: unable to clear value lengths.\n",
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

			return( -1 );
		}
		( *values_table )->amount_of_values = amount_of_values;
	}
	return( 1 );
}

/* Frees the values table including elements
 * Returns 1 if successful or -1 on error
 */
int libewf_values_table_free(
     libewf_values_table_t **values_table )
{
	static char *function     = "libewf_values_table_free";
	int values_table_iterator = 0;

	if( values_table == NULL )
	{
		notify_warning_printf( "%s: invalid values table.\n",
		 function );

		return( 1 );
	}
	if( *values_table != NULL )
	{
		for( values_table_iterator = 0; values_table_iterator < ( *values_table )->amount_of_values; values_table_iterator++ )
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
     int amount_of_values )
{
	static char *function           = "libewf_values_table_resize";
	void *reallocation              = NULL;
	size_t values_table_size        = 0;
	size_t values_table_string_size = 0;

	if( values_table == NULL )
	{
		notify_warning_printf( "%s: invalid values table.\n",
		 function );

		return( -1 );
	}
	if( amount_of_values < 0 )
	{
		notify_warning_printf( "%s: invalid amount of values less than zero.\n",
		 function );

		return( -1 );
	}
	if( values_table->amount_of_values < amount_of_values )
	{
		values_table_string_size = amount_of_values * sizeof( character_t * );

		if( values_table_string_size > (ssize_t) SSIZE_MAX )
		{
			notify_warning_printf( "%s: invalid values table string size value exceeds maximum.\n",
			 function );

			return( -1 );
		}
		values_table_size = amount_of_values * sizeof( size_t );

		if( values_table_size > (ssize_t) SSIZE_MAX )
		{
			notify_warning_printf( "%s: invalid values table size value exceeds maximum.\n",
			 function );

			return( -1 );
		}
		reallocation = memory_reallocate(
		                values_table->identifier,
		                values_table_string_size );

		if( reallocation == NULL )
		{
			notify_warning_printf( "%s: unable to resize identifiers.\n",
			 function );

			return( -1 );
		}
		values_table->identifier = (character_t **) reallocation;

		if( memory_set(
		     &( values_table->identifier[ values_table->amount_of_values ] ),
		     0,
		     sizeof( character_t * ) * ( amount_of_values - values_table->amount_of_values ) ) == NULL )
		{
			notify_warning_printf( "%s: unable to clear identifiers.\n",
			 function );

			return( -1 );
		}
		reallocation = memory_reallocate(
		                values_table->identifier_length,
		                values_table_size );

		if( reallocation == NULL )
		{
			notify_warning_printf( "%s: unable to resize identifier lengths.\n",
			 function );

			return( -1 );
		}
		values_table->identifier_length = (size_t *) reallocation;

		if( memory_set(
		     &( values_table->identifier_length[ values_table->amount_of_values ] ),
		     0,
		     sizeof( size_t ) * ( amount_of_values - values_table->amount_of_values ) ) == NULL )
		{
			notify_warning_printf( "%s: unable to clear identifier lengths.\n",
			 function );

			return( -1 );
		}
		reallocation = memory_reallocate(
		                values_table->value,
		                values_table_string_size );

		if( reallocation == NULL )
		{
			notify_warning_printf( "%s: unable to resize values.\n",
			 function );

			return( -1 );
		}
		values_table->value = (character_t **) reallocation;

		if( memory_set(
		     &( values_table->value[ values_table->amount_of_values ] ),
		     0,
		     sizeof( character_t * ) * ( amount_of_values - values_table->amount_of_values ) ) == NULL )
		{
			notify_warning_printf( "%s: unable to clear values.\n",
			 function );

			return( -1 );
		}
		reallocation = memory_reallocate(
		                values_table->value_length,
		                values_table_size );

		if( reallocation == NULL )
		{
			notify_warning_printf( "%s: unable to resize value lengths.\n",
			 function );

			return( -1 );
		}
		values_table->value_length = (size_t *) reallocation;

		if( memory_set(
		     &( values_table->value_length[ values_table->amount_of_values ] ),
		     0,
		     sizeof( size_t ) * ( amount_of_values - values_table->amount_of_values ) ) == NULL )
		{
			notify_warning_printf( "%s: unable to clear value sizes.\n",
			 function );

			return( -1 );
		}
		values_table->amount_of_values = amount_of_values;
	}
	return( 1 );
}

/* Retrieves the index for a certain identifier
 * Returns 1 if successful, 0 if no index was found or -1 on error
 */
int libewf_values_table_get_index(
     libewf_values_table_t *values_table,
     character_t *identifier,
     size_t identifier_length,
     int *index )
{
	static char *function     = "libewf_values_table_get_index";
	int values_table_iterator = 0;

	if( values_table == NULL )
	{
		notify_warning_printf( "%s: invalid values table.\n",
		 function );

		return( -1 );
	}
	if( values_table->amount_of_values < 0 )
	{
		notify_warning_printf( "%s: invalid values table amount value less than zero.\n",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		notify_warning_printf( "%s: invalid identifier.\n",
		 function );

		return( -1 );
	}
	if( identifier_length == 0 )
	{
		notify_warning_printf( "%s: invalid identifier length value is zero.\n",
		 function );

		return( -1 );
	}
	if( identifier_length > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid identifier length value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( index == NULL )
	{
		notify_warning_printf( "%s: invalid index.\n",
		 function );

		return( -1 );
	}
	for( values_table_iterator = 0; values_table_iterator < values_table->amount_of_values; values_table_iterator++ )
	{
		if( values_table->identifier[ values_table_iterator ] == NULL )
		{
			notify_warning_printf( "%s: missing identifier for index: %d.\n",
			 function, values_table_iterator );

			continue;
		}
		if( values_table->identifier_length[ values_table_iterator ] == 0 )
		{
			notify_warning_printf( "%s: missing identifier length for index: %d.\n",
			 function, values_table_iterator );

			continue;
		}
		if( values_table->identifier_length[ values_table_iterator ] != identifier_length )
		{
			continue;
		}
		if( string_compare(
		     identifier,
		     values_table->identifier[ values_table_iterator ],
		     identifier_length ) == 0 )
		{
			*index = values_table_iterator;

			return( 1 );
		}
	}
	return( 0 );
}

/* Retrieves an identifier in the values table
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_values_table_get_identifier(
     libewf_values_table_t *values_table,
     int index,
     character_t *identifier,
     size_t identifier_size )
{
	static char *function = "libewf_values_table_get_identifier";

	if( values_table == NULL )
	{
		notify_warning_printf( "%s: invalid values table.\n",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		notify_warning_printf( "%s: invalid identifier.\n",
		 function );

		return( -1 );
	}
	if( identifier_size > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid identifier size value exceeds maximum.\n",
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
		return( 0 );
	}
	if( values_table->identifier[ index ] == NULL )
	{
		notify_warning_printf( "%s: missing identifier for index: %d.\n",
		 function, index );

		return( -1 );
	}
	if( values_table->identifier_length[ index ] == 0 )
	{
		notify_warning_printf( "%s: missing identifier size for length: %d.\n",
		 function, index );

		return( -1 );
	}
	if( identifier_size < ( values_table->identifier_length[ index ] + 1 ) )
	{
		notify_warning_printf( "%s: identifier too small.\n",
		 function );

		return( -1 );
	}
	if( string_copy(
	     identifier,
	     values_table->identifier[ index ],
	     values_table->identifier_length[ index ] ) == NULL )
	{
		notify_warning_printf( "%s: unable to set identifier.\n",
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
     character_t *identifier,
     size_t identifier_length )
{
	static char *function = "libewf_values_table_set_identifier";

	if( values_table == NULL )
	{
		notify_warning_printf( "%s: invalid values table.\n",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		notify_warning_printf( "%s: invalid identifier.\n",
		 function );

		return( -1 );
	}
	if( identifier_length == 0 )
	{
		notify_warning_printf( "%s: invalid identifier length value is zero.\n",
		 function );

		return( -1 );
	}
	if( identifier_length > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid identifier length value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( ( values_table->amount_of_values == 0 )
	 || ( index < 0 )
	 || ( index >= values_table->amount_of_values ) )
	{
		notify_warning_printf( "%s: invalid index.\n",
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
	values_table->identifier[ index ] = (character_t *) memory_allocate(
							     sizeof( character_t ) * ( identifier_length + 1 ) );

	if( values_table->identifier[ index ] == NULL )
	{
		notify_warning_printf( "%s: unable to create identifier.\n",
		 function );

		return( -1 );
	}
	if( string_copy(
	     values_table->identifier[ index ],
	     identifier,
	     identifier_length ) == NULL )
	{
		notify_warning_printf( "%s: unable to set identifier.\n",
		 function );

		memory_free(
		 values_table->identifier[ index ] );

		values_table->identifier[ index ] = NULL;

		return( -1 );
	}
	( values_table->identifier[ index ] )[ identifier_length ] = 0;

	values_table->identifier_length[ index ] = identifier_length;

	return( 1 );
}

/* Retrieves a value in the values table
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_values_table_get_value(
     libewf_values_table_t *values_table,
     character_t *identifier,
     size_t identifier_length,
     character_t *value,
     size_t value_size )
{
	static char *function = "libewf_values_table_get_value";
	int index             = 0;
	int result            = 0;

	if( value == NULL )
	{
		notify_warning_printf( "%s: invalid value.\n",
		 function );

		return( -1 );
	}
	if( value_size > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid value size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	result = libewf_values_table_get_index(
	          values_table,
	          identifier,
	          identifier_length,
	          &index ) ;

	if( result <= -1 )
	{
		notify_warning_printf( "%s: unable to find index for: %" PRIs ".\n",
		 function, identifier );

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
	if( value_size < ( values_table->value_length[ index ] + 1 ) )
	{
		notify_warning_printf( "%s: value too small.\n",
		 function );

		return( -1 );
	}
	if( string_copy(
	     value,
	     values_table->value[ index ],
	     values_table->value_length[ index ] ) == NULL )
	{
		notify_warning_printf( "%s: unable to set value.\n",
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
     character_t *identifier,
     size_t identifier_length,
     character_t *value,
     size_t value_length )
{
	static char *function = "libewf_values_table_set_value";
	int index             = 0;
	int result            = 0;

	result = libewf_values_table_get_index(
	          values_table,
	          identifier,
	          identifier_length,
	          &index );

	if( result <= -1 )
	{
		notify_warning_printf( "%s: unable to find index for: %" PRIs ".\n",
		 function, identifier );

		return( -1 );
	}
	else if( result == 0 )
	{
		index = values_table->amount_of_values;

		if( libewf_values_table_resize(
		     values_table,
		     ( index + 1 ) ) != 1 )
		{
			notify_warning_printf( "%s: unable to resize values table.\n",
			 function );

			return( -1 );
		}
		if( libewf_values_table_set_identifier(
		     values_table,
		     index,
		     identifier,
		     identifier_length ) != 1 )
		{
			notify_warning_printf( "%s: unable to set identifier.\n",
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
		values_table->value[ index ] = (character_t *) memory_allocate(
		                                                sizeof( character_t ) * ( value_length + 1 ) );

		if( values_table->value[ index ] == NULL )
		{
			notify_warning_printf( "%s: unable to create value.\n",
			 function );

			return( -1 );
		}
		if( string_copy(
		     values_table->value[ index ],
		     value,
		     value_length ) == NULL )
		{
			notify_warning_printf( "%s: unable to set value.\n",
			 function );

			memory_free(
			 values_table->value[ index ] );

			values_table->value[ index ] = NULL;

			return( -1 );
		}
		( values_table->value[ index ] )[ value_length ] = 0;

		values_table->value_length[ index ] = value_length;
	}
	return( 1 );
}

