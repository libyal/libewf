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
     uint32_t amount_of_values )
{
	static char *function    = "libewf_values_table_initialize";
	size_t values_table_size = 0;

	if( values_table == NULL )
	{
		notify_warning_printf( "%s: invalid values table.\n",
		 function );

		return( 1 );
	}
	if( amount_of_values > (uint32_t) INT32_MAX )
	{
		notify_warning_printf( "%s: invalid amount of values exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( *values_table == NULL )
	{
		values_table_size = amount_of_values * sizeof( character_t * );

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
		( *values_table )->identifiers = (character_t **) memory_allocate(
		                                                   values_table_size );

		if( ( *values_table )->identifiers == NULL )
		{
			notify_warning_printf( "%s: unable to create identifiers.\n",
			 function );

			memory_free(
			 *values_table );

			return( -1 );
		}
		if( memory_set(
		     ( *values_table )->identifiers,
		     0,
		     values_table_size ) == NULL )
		{
			notify_warning_printf( "%s: unable to clear identifiers.\n",
			 function );

			memory_free(
			 ( *values_table )->identifiers );
			memory_free(
			 *values_table );

			return( -1 );
		}
		( *values_table )->values = (character_t **) memory_allocate(
		                                              values_table_size );

		if( ( *values_table )->values == NULL )
		{
			notify_warning_printf( "%s: unable to create values.\n",
			 function );

			memory_free(
			 ( *values_table )->identifiers );
			memory_free(
			 *values_table );

			return( -1 );
		}
		if( memory_set(
		     ( *values_table )->values,
		     0,
		     values_table_size ) == NULL )
		{
			notify_warning_printf( "%s: unable to clear values.\n",
			 function );

			memory_free(
			 ( *values_table )->identifiers );
			memory_free(
			 ( *values_table )->values );
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
	static char *function = "libewf_values_table_free";
	uint32_t iterator     = 0;

	if( values_table == NULL )
	{
		notify_warning_printf( "%s: invalid values table.\n",
		 function );

		return( 1 );
	}
	if( *values_table != NULL )
	{
		for( iterator = 0; iterator < ( *values_table )->amount_of_values; iterator++ )
		{
			if( ( *values_table )->identifiers[ iterator ] != NULL )
			{
				memory_free(
				 ( *values_table )->identifiers[ iterator ] );
			}
			if( ( *values_table )->values[ iterator ] != NULL )
			{
				memory_free(
				 ( *values_table )->values[ iterator ] );
			}
		}
		if( ( *values_table )->identifiers != NULL )
		{
			memory_free(
			 ( *values_table )->identifiers );
		}
		if( ( *values_table )->values != NULL )
		{
			memory_free(
			 ( *values_table )->values );
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
     uint32_t amount_of_values )
{
	character_t **reallocation = NULL;
	static char *function      = "libewf_values_table_resize";
	size_t values_table_size   = 0;

	if( values_table == NULL )
	{
		notify_warning_printf( "%s: invalid values table.\n",
		 function );

		return( -1 );
	}
	if( amount_of_values > (uint32_t) INT32_MAX )
	{
		notify_warning_printf( "%s: invalid amount of values exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( values_table->amount_of_values < amount_of_values )
	{
		values_table_size = amount_of_values * sizeof( character_t * );

		if( values_table_size > (ssize_t) SSIZE_MAX )
		{
			notify_warning_printf( "%s: invalid values table size value exceeds maximum.\n",
			 function );

			return( -1 );
		}
		reallocation = (character_t **) memory_reallocate(
		                                 values_table->identifiers,
		                                 values_table_size );

		if( reallocation == NULL )
		{
			notify_warning_printf( "%s: unable to resize identifiers.\n",
			 function );

			return( -1 );
		}
		values_table->identifiers = reallocation;

		if( memory_set(
		     &( values_table->identifiers[ values_table->amount_of_values ] ),
		     0,
		     sizeof( character_t * ) * ( amount_of_values - values_table->amount_of_values ) ) == NULL )
		{
			notify_warning_printf( "%s: unable to clear identifiers.\n",
			 function );

			return( -1 );
		}
		reallocation = (character_t **) memory_reallocate(
		                                 values_table->values,
		                                 values_table_size );

		if( reallocation == NULL )
		{
			notify_warning_printf( "%s: unable to resize values.\n",
			 function );

			return( -1 );
		}
		values_table->values = reallocation;

		if( memory_set(
		     &( values_table->values[ values_table->amount_of_values ] ),
		     0,
		     sizeof( character_t * ) * ( amount_of_values - values_table->amount_of_values ) ) == NULL )
		{
			notify_warning_printf( "%s: unable to clear values.\n",
			 function );

			return( -1 );
		}
		values_table->amount_of_values = amount_of_values;
	}
	return( 1 );
}

/* Retrieves the value index number or -1 on error
 * The index number will be larger than the amount when the identifier is not present in the values table
 */
int32_t libewf_values_table_get_index(
         libewf_values_table_t *values_table,
         character_t *identifier )
{
	static char *function    = "libewf_values_table_get_index";
	size_t string_length     = 0;
	size_t identifier_length = 0;
	int32_t iterator         = 0;

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
	identifier_length = string_length(
	                     identifier );

	if( identifier_length == 0 )
	{
		notify_warning_printf( "%s: invalid identifier.\n",
		 function );

		return( -1 );
	}
	if( values_table->amount_of_values > (uint32_t) INT32_MAX )
	{
		notify_warning_printf( "%s: invalid values table amount value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	for( iterator = 0; iterator < (int32_t) values_table->amount_of_values; iterator++ )
	{
		if( values_table->identifiers[ iterator ] == NULL )
		{
			notify_warning_printf( "%s: missing identifier for index: %" PRIi32 ".\n",
			 function, iterator );

			continue;
		}
		string_length = string_length(
		                 values_table->identifiers[ iterator ] );

		if( string_length == 0 )
		{
			notify_warning_printf( "%s: unable to determine length of identifier of index: %" PRIi32 ".\n",
			 function, iterator );

			continue;
		}
		if( string_length != identifier_length )
		{
			continue;
		}
		if( string_compare(
		     identifier,
		     values_table->identifiers[ iterator ],
		     identifier_length ) == 0 )
		{
			return( iterator );
		}
	}
	return( iterator );
}

/* Retrieves an identifier in the values table
 * Length should contain the amount of characters in the string
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_values_table_get_identifier(
     libewf_values_table_t *values_table,
     uint32_t index,
     character_t *identifier,
     size_t length )
{
	static char *function    = "libewf_values_table_get_identifier";
	size_t identifier_length = 0;

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
	if( values_table->amount_of_values == 0 )
	{
		return( 0 );
	}
	if( index >= values_table->amount_of_values )
	{
		notify_warning_printf( "%s: invalid index out of range.\n",
		 function );

		return( -1 );
	}
	if( values_table->identifiers[ index ] == NULL )
	{
		notify_warning_printf( "%s: missing identifier for index: %" PRIu32 ".\n",
		 function, index );

		return( -1 );
	}
	identifier_length = string_length(
	                     values_table->identifiers[ index ] );

	/* Don't bother with empty values
	 */
	if( identifier_length == 0 )
	{
		return( 0 );
	}
	/* Add 1 additional byte required for the end of string character
	 */
	identifier_length += 1;

	if( identifier_length > length )
	{
		notify_warning_printf( "%s: identifier too small.\n",
		 function );

		return( -1 );
	}
	if( string_copy(
	     identifier,
	     values_table->identifiers[ index ],
	     identifier_length ) == NULL )
	{
		notify_warning_printf( "%s: unable to set identifier.\n",
		 function );

		return( -1 );
	}
	identifier[ identifier_length - 1 ] = (character_t) '\0';

	return( 1 );
}

/* Retrieves a value in the values table
 * Length should contain the amount of characters in the string
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_values_table_get_value(
     libewf_values_table_t *values_table,
     character_t *identifier,
     character_t *value,
     size_t length )
{
	static char *function = "libewf_values_table_get_value";
	size_t value_length   = 0;
	int32_t index         = 0;

	if( value == NULL )
	{
		notify_warning_printf( "%s: invalid value.\n",
		 function );

		return( -1 );
	}
	index = libewf_values_table_get_index(
	         values_table,
	         identifier );

	if( index <= -1 )
	{
		notify_warning_printf( "%s: unable to find index for: %" PRIs ".\n",
		 function, identifier );

		return( -1 );
	}
	if( (uint32_t) index >= values_table->amount_of_values )
	{
		return( 0 );
	}
	if( values_table->values[ index ] == NULL )
	{
		return( 0 );
	}
	value_length = string_length(
	                values_table->values[ index ] );

	/* Don't bother with empty values
	 */
	if( value_length == 0 )
	{
		return( 0 );
	}
	/* Add 1 additional byte required for the end of string character
	 */
	value_length += 1;

	if( value_length > length )
	{
		notify_warning_printf( "%s: value too small.\n",
		 function );

		return( -1 );
	}
	if( string_copy(
	     value,
	     values_table->values[ index ],
	     value_length ) == NULL )
	{
		notify_warning_printf( "%s: unable to set value.\n",
		 function );

		return( -1 );
	}
	value[ value_length - 1 ] = (character_t) '\0';

	return( 1 );
}

/* Set a value in the values table
 * Length should contain the amount of characters in the string
 * Frees the previous value if necessary
 * Returns 1 if successful or -1 on error
 */
int libewf_values_table_set_value(
     libewf_values_table_t *values_table,
     character_t *identifier,
     character_t *value,
     size_t length )
{
	static char *function = "libewf_values_table_set_value";
	size_t string_length  = 0;
	int32_t index         = 0;

	index = libewf_values_table_get_index(
	         values_table,
	         identifier );

	if( index <= -1 )
	{
		notify_warning_printf( "%s: unable to find index for: %" PRIs ".\n",
		 function, identifier );

		return( -1 );
	}
	if( (uint32_t) index >= values_table->amount_of_values )
	{
		string_length = string_length(
		                 identifier );

		if( string_length == 0 )
		{
			notify_warning_printf( "%s: unable to determine length of identifier.\n",
			 function );

			return( -1 );
		}
		if( libewf_values_table_resize(
		     values_table,
		     ( index + 1 ) ) != 1 )
		{
			notify_warning_printf( "%s: unable to resize values table.\n",
			 function );

			return( -1 );
		}
		values_table->identifiers[ index ] = string_duplicate(
		                                      identifier,
		                                      string_length + 1 );

		if( values_table->identifiers[ index ] == NULL )
		{
			notify_warning_printf( "%s: unable to set identifier.\n",
			 function );

			memory_free(
			 values_table->identifiers[ index ] );

			values_table->identifiers[ index ] = NULL;

			return( -1 );
		}
	}
	/* Clear the buffer of the previous value
	 */
	if( values_table->values[ index ] != NULL )
	{
		memory_free(
		 values_table->values[ index ] );

		values_table->values[ index ] = NULL;
	}
	/* Don't bother with empty values
	 */
	if( value == NULL )
	{
		return( 1 );
	}
	if( length == 0 )
	{
		return( 1 );
	}
	values_table->values[ index ] = string_duplicate(
	                                 value,
	                                 length + 1 );

	if( values_table->values[ index ] == NULL )
	{
		notify_warning_printf( "%s: unable to set value.\n",
		 function );

		memory_free(
		 values_table->values[ index ] );

		values_table->values[ index ] = NULL;

		return( -1 );
	}
	return( 1 );
}

