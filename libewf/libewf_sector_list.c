/*
 * Offset list
 *
 * Copyright (c) 2006-2012, Joachim Metz <jbmetz@users.sourceforge.net>
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

#include "libewf_list_type.h"
#include "libewf_sector_list.h"

/* Creates a sector list value
 * Returns 1 if successful or -1 on error
 */
int libewf_sector_list_value_initialize(
     libewf_sector_list_value_t **sector_list_value,
     liberror_error_t **error )
{
	static char *function = "libewf_sector_list_value_initialize";

	if( sector_list_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector list value.",
		 function );

		return( -1 );
	}
	if( *sector_list_value != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid sector list value value already set.",
		 function );

		return( -1 );
	}
	*sector_list_value = memory_allocate_structure(
	                      libewf_sector_list_value_t );

	if( *sector_list_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create sector list value.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *sector_list_value,
	     0,
	     sizeof( libewf_sector_list_value_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear sector list value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *sector_list_value != NULL )
	{
		memory_free(
		 *sector_list_value );

		*sector_list_value = NULL;
	}
	return( -1 );
}

/* Frees a sector list value
 */
int libewf_sector_list_value_free(
     libewf_sector_list_value_t **sector_list_value,
     liberror_error_t **error )
{
	static char *function = "libewf_sector_list_value_free";

	if( sector_list_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector list value.",
		 function );

		return( -1 );
	}
	if( *sector_list_value != NULL )
	{
		memory_free(
		 *sector_list_value );

		*sector_list_value = NULL;
	}
	return( 1 );
}

/* Clones the sector list value
 * Returns 1 if successful or -1 on error
 */
int libewf_sector_list_value_clone(
     libewf_sector_list_value_t **destination_sector_list_value,
     libewf_sector_list_value_t *source_sector_list_value,
     liberror_error_t **error )
{
	static char *function = "libewf_sector_list_value_clone";

	if( destination_sector_list_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination sector list value.",
		 function );

		return( -1 );
	}
	if( *destination_sector_list_value != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination sector list value value already set.",
		 function );

		return( -1 );
	}
	if( source_sector_list_value == NULL )
	{
		*destination_sector_list_value = NULL;

		return( 1 );
	}
	*destination_sector_list_value = memory_allocate_structure(
	                                  libewf_sector_list_value_t );

	if( *destination_sector_list_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination sector list value.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_sector_list_value,
	     source_sector_list_value,
	     sizeof( libewf_sector_list_value_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination sector list value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_sector_list_value != NULL )
	{
		memory_free(
		 *destination_sector_list_value );

		*destination_sector_list_value = NULL;
	}
	return( -1 );
}

/* Creates a sector list
 * Returns 1 if successful or -1 on error
 */
int libewf_sector_list_initialize(
     libewf_sector_list_t **sector_list,
     liberror_error_t **error )
{
	static char *function = "libewf_sector_list_initialize";

	if( sector_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector list.",
		 function );

		return( -1 );
	}
	if( *sector_list == NULL )
	{
		*sector_list = memory_allocate_structure(
		                libewf_sector_list_t );

		if( *sector_list == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create sector list.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     *sector_list,
		     0,
		     sizeof( libewf_sector_list_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear sector list.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( *sector_list != NULL )
	{
		memory_free(
		 *sector_list );

		*sector_list = NULL;
	}
	return( -1 );
}

/* Frees an sector list including the elements
 * Returns 1 if successful or -1 on error
 */
int libewf_sector_list_free(
     libewf_sector_list_t **sector_list,
     liberror_error_t **error )
{
	static char *function = "libewf_sector_list_free";
	int result            = 1;

	if( sector_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector list.",
		 function );

		return( -1 );
	}
	if( *sector_list != NULL )
	{
		result = libewf_sector_list_empty(
		          *sector_list,
		          error );

		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to empty sector list.",
			 function );
		}
		memory_free(
		 *sector_list );

		*sector_list = NULL;
	}
	return( result );
}

/* Empties an sector list and frees the elements
 * Returns 1 if successful or -1 on error
 */
int libewf_sector_list_empty(
     libewf_sector_list_t *sector_list,
     liberror_error_t **error )
{
	libewf_list_element_t *list_element = NULL;
	static char *function               = "libewf_sector_list_empty";
	int element_index                   = 0;
	int number_of_elements              = 0;
	int result                          = 1;

	if( sector_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector list.",
		 function );

		return( -1 );
	}
	if( sector_list->number_of_elements > 0 )
	{
		number_of_elements = sector_list->number_of_elements;

		for( element_index = 0;
		     element_index < number_of_elements;
		     element_index++ )
		{
			list_element = sector_list->first_element;

			if( list_element == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: corruption detected in element: %d.",
				 function,
				 element_index );

				return( -1 );
			}
			sector_list->first_element = list_element->next_element;

			if( sector_list->last_element == list_element )
			{
				sector_list->last_element = list_element->next_element;
			}
			sector_list->number_of_elements -= 1;

			if( list_element->next_element != NULL )
			{
				list_element->next_element->previous_element = NULL;
			}
			list_element->next_element = NULL;

			if( libewf_list_element_free(
			     &list_element,
			     (int (*)(intptr_t **, liberror_error_t **)) &libewf_sector_list_value_free,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free element: %d.",
				 function,
				 element_index );

				result = -1;
			}
		}
	}
	return( result );
}

/* Clones the sector list
 * Returns 1 if successful or -1 on error
 */
int libewf_sector_list_clone(
     libewf_sector_list_t **destination_sector_list,
     libewf_sector_list_t *source_sector_list,
     liberror_error_t **error )
{
	libewf_list_element_t *destination_list_element = NULL;
	libewf_list_element_t *source_list_element      = NULL;
	libewf_sector_list_value_t *destination_value   = NULL;
	static char *function                           = "libewf_sector_list_clone";
	int element_index                               = 0;

	if( destination_sector_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination sector list.",
		 function );

		return( -1 );
	}
	if( *destination_sector_list != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination sector list value already set.",
		 function );

		return( -1 );
	}
	if( source_sector_list == NULL )
	{
		*destination_sector_list = NULL;

		return( 1 );
	}
	if( libewf_sector_list_initialize(
	     destination_sector_list,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination sector list.",
		 function );

		goto on_error;
	}
	if( *destination_sector_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing destination sector list.",
		 function );

		goto on_error;
	}
	source_list_element = source_sector_list->first_element;

	for( element_index = 0;
	     element_index < source_sector_list->number_of_elements;
	     element_index++ )
	{
		if( source_list_element == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected in source sector list element: %d.",
			 function,
			 element_index );

			goto on_error;
		}
		if( libewf_list_element_initialize(
		     &destination_list_element,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create destination list element: %d.",
			 function,
			 element_index );

			goto on_error;
		}
		if( libewf_sector_list_value_clone(
		     &destination_value,
		     (libewf_sector_list_value_t *) source_list_element->value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to clone value of sector list element: %d.",
			 function,
			 element_index );

			goto on_error;
		}
		if( libewf_list_element_set_value(
		     destination_list_element,
		     (intptr_t *) destination_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to set value of destination list element: %d.",
			 function,
			 element_index );

			goto on_error;
		}
		destination_value = NULL;

		if( ( *destination_sector_list )->first_element == NULL )
		{
			( *destination_sector_list )->first_element = destination_list_element;
		}
		if( ( *destination_sector_list )->last_element != NULL )
		{
			( *destination_sector_list )->last_element->next_element = destination_list_element;
			destination_list_element->previous_element               = ( *destination_sector_list )->last_element;
		}
		( *destination_sector_list )->last_element        = destination_list_element;
		( *destination_sector_list )->number_of_elements += 1;

		destination_list_element = NULL;

		source_list_element = source_list_element->next_element;
	}
	return( 1 );

on_error:
	if( destination_value != NULL )
	{
		libewf_sector_list_value_free(
		 &destination_value,
		 NULL );
	}
	if( destination_list_element != NULL )
	{
		libewf_list_element_free(
		 &destination_list_element,
		 (int (*)(intptr_t **, liberror_error_t **)) &libewf_sector_list_value_free,
		 NULL );
	}
	if( *destination_sector_list != NULL )
	{
		libewf_sector_list_free(
		 destination_sector_list,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the number of elements in the sector list
 * Returns 1 if successful or -1 on error
 */
int libewf_sector_list_get_number_of_elements(
     libewf_sector_list_t *sector_list,
     int *number_of_elements,
     liberror_error_t **error )
{
	static char *function = "libewf_sector_list_get_number_of_elements";

	if( sector_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector list.",
		 function );

		return( -1 );
	}
	if( number_of_elements == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of elements.",
		 function );

		return( -1 );
	}
	*number_of_elements = sector_list->number_of_elements;

	return( 1 );
}

/* Appends a sector
 * Returns 1 if successful, or -1 on error
 */
int libewf_sector_list_append_sector(
     libewf_sector_list_t *sector_list,
     uint64_t first_sector,
     uint64_t number_of_sectors,
     uint8_t merge_ranges,
     liberror_error_t **error )
{
	libewf_list_element_t *last_list_element      = NULL;
	libewf_list_element_t *list_element           = NULL;
	libewf_list_element_t *remove_element         = NULL;
	libewf_sector_list_value_t *sector_list_value = NULL;
	static char *function                         = "libewf_sector_list_append_sector";
	uint64_t last_range_sector                    = 0;
	uint64_t last_sector                          = 0;
	int create_list_element                       = 0;
	int element_index                             = 0;
	int merge_next_list_element_check             = 0;
	int merge_previous_list_element_check         = 0;

	if( sector_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector list.",
		 function );

		return( -1 );
	}
	if( first_sector > (uint64_t) INT64_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid first sector value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( number_of_sectors > (uint64_t) INT64_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid number of sectors value exceeds maximum.",
		 function );

		return( -1 );
	}
	create_list_element = 1;

	/* Check if new range should be merged with an existing range
	 */
	if( merge_ranges == 0 )
	{
		last_list_element = sector_list->last_element;
	}
	else if( sector_list->number_of_elements > 0 )
	{
		last_sector = first_sector + number_of_sectors;

		/* Check the last element first, most often the list will be filled linear 
		 */
		list_element = sector_list->last_element;

		if( list_element == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected for list element: %d.",
			 function,
			 sector_list->number_of_elements - 1 );

			return( -1 );
		}
		if( list_element->value == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing sector list value for list element: %d.",
			 function,
			 sector_list->number_of_elements - 1 );

			return( -1 );
		}
		sector_list_value = (libewf_sector_list_value_t *) list_element->value;

		last_range_sector = sector_list_value->first_sector + sector_list_value->number_of_sectors;

		/* Check if the sector range overlaps at the end of the last sector range
		 */
		if( ( first_sector >= sector_list_value->first_sector )
		 && ( first_sector <= last_range_sector ) )
		{
			if( last_sector > last_range_sector )
			{
				sector_list_value->number_of_sectors += (uint64_t) ( last_sector - last_range_sector );
			}
			create_list_element           = 0;
			merge_next_list_element_check = 1;
		}
		/* Check if the sector range overlaps at the beginning of the last sector range
		 */
		else if( ( last_sector >= sector_list_value->first_sector )
		      && ( last_sector <= last_range_sector ) )
		{
			if( first_sector < sector_list_value->first_sector )
			{
				sector_list_value->first_sector       = first_sector;
				sector_list_value->number_of_sectors += (uint64_t) ( sector_list_value->first_sector - first_sector );
			}
			create_list_element               = 0;
			merge_previous_list_element_check = 1;
		}
		/* Check if the sector range overlaps the last sector range entirely
		 */
		else if( ( first_sector < sector_list_value->first_sector )
		      && ( last_sector > last_range_sector ) )
		{
			sector_list_value->first_sector      = first_sector;
			sector_list_value->number_of_sectors = number_of_sectors;

			create_list_element               = 0;
			merge_previous_list_element_check = 1;
		}
		/* Check if the sector range is beyond the last range
		 */
		else if( last_sector > last_range_sector )
		{
			last_list_element = list_element;
		}
		else if( sector_list->number_of_elements > 1 )
		{
			if( last_sector > ( last_range_sector / 2 ) )
			{
				list_element = list_element->previous_element;

				for( element_index = ( sector_list->number_of_elements - 2 );
				     element_index >= 0;
				     element_index-- )
				{
					if( list_element == NULL )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_RUNTIME,
						 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
						 "%s: corruption detected for list element: %d.",
						 function,
						 element_index );

						return( -1 );
					}
					if( list_element->value == NULL )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_RUNTIME,
						 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
						 "%s: missing sector list value for list element: %d.",
						 function,
						 element_index );

						return( -1 );
					}
					sector_list_value = (libewf_sector_list_value_t *) list_element->value;

					last_range_sector = sector_list_value->first_sector + sector_list_value->number_of_sectors;

					/* Check if the sector range overlaps at the end of an existing sector range
					 */
					if( ( first_sector >= sector_list_value->first_sector )
					 && ( first_sector <= last_range_sector ) )
					{
						if( last_sector > last_range_sector )
						{
							sector_list_value->number_of_sectors += (uint64_t) ( last_sector - last_range_sector );
						}
						create_list_element           = 0;
						merge_next_list_element_check = 1;
					}
					/* Check if the sector range overlaps at the beginning of an existing sector range
					 */
					else if( ( last_sector >= sector_list_value->first_sector )
					      && ( last_sector <= last_range_sector ) )
					{
						if( first_sector < sector_list_value->first_sector )
						{
							sector_list_value->first_sector       = first_sector;
							sector_list_value->number_of_sectors += (uint64_t) ( sector_list_value->first_sector - first_sector );
						}
						create_list_element               = 0;
						merge_previous_list_element_check = 1;
					}
					/* Check if the sector range overlaps an existing sector range entirely
					 */
					else if( ( first_sector < sector_list_value->first_sector )
					      && ( last_sector > last_range_sector ) )
					{
						sector_list_value->first_sector      = first_sector;
						sector_list_value->number_of_sectors = number_of_sectors;

						create_list_element               = 0;
						merge_next_list_element_check     = 1;
						merge_previous_list_element_check = 1;
					}
					if( create_list_element == 0 )
					{
						break;
					}
					/* Check if the sector range belongs after the exising sector range
					 */
					if( last_sector > last_range_sector )
					{
						last_list_element = list_element;

						break;
					}
					last_list_element = list_element;
					list_element      = list_element->previous_element;
				}
			}
			else
			{
				list_element = sector_list->first_element;

				for( element_index = 0;
				     element_index < ( sector_list->number_of_elements - 1 );
				     element_index++ )
				{
					if( list_element == NULL )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_RUNTIME,
						 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
						 "%s: corruption detected for list element: %d.",
						 function,
						 element_index );

						return( -1 );
					}
					if( list_element->value == NULL )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_RUNTIME,
						 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
						 "%s: missing sector list value for list element: %d.",
						 function,
						 element_index );

						return( -1 );
					}
					sector_list_value = (libewf_sector_list_value_t *) list_element->value;

					last_range_sector = sector_list_value->first_sector + sector_list_value->number_of_sectors;

					/* Check if the sector range overlaps at the end of an existing sector range
					 */
					if( ( first_sector >= sector_list_value->first_sector )
					 && ( first_sector <= last_range_sector ) )
					{
						if( last_sector > last_range_sector )
						{
							sector_list_value->number_of_sectors += (uint64_t) ( last_sector - last_range_sector );
						}
						create_list_element           = 0;
						merge_next_list_element_check = 1;
					}
					/* Check if the sector range overlaps at the beginning of an existing sector range
					 */
					else if( ( last_sector >= sector_list_value->first_sector )
					      && ( last_sector <= last_range_sector ) )
					{
						if( first_sector < sector_list_value->first_sector )
						{
							sector_list_value->first_sector       = first_sector;
							sector_list_value->number_of_sectors += (uint64_t) ( sector_list_value->first_sector - first_sector );
						}
						create_list_element               = 0;
						merge_previous_list_element_check = 1;
					}
					/* Check if the sector range overlaps an existing sector range entirely
					 */
					else if( ( first_sector < sector_list_value->first_sector )
					      && ( last_sector > last_range_sector ) )
					{
						sector_list_value->first_sector      = first_sector;
						sector_list_value->number_of_sectors = number_of_sectors;

						create_list_element               = 0;
						merge_next_list_element_check     = 1;
						merge_previous_list_element_check = 1;
					}
					if( create_list_element == 0 )
					{
						break;
					}
					/* Check if the sector range belongs before the current sector range
					 */
					if( last_sector < last_range_sector )
					{
                                        	last_list_element = list_element->previous_element;

						break;
					}
                                        last_list_element = list_element;
                                        list_element      = list_element->next_element;
				}
			}
		}
		/* Check if the current range should be merged with the previous range
		 */
		if( merge_previous_list_element_check != 0 )
		{
			if( list_element == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid list element.",
				 function );

				return( -1 );
			}
			if( list_element->previous_element != NULL )
			{
				if( list_element->previous_element->value == NULL )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: missing sector list value for previous list element.",
					 function );

					return( -1 );
				}
				last_sector = ( (libewf_sector_list_value_t *) list_element->previous_element->value )->first_sector
				            + ( (libewf_sector_list_value_t *) list_element->previous_element->value )->number_of_sectors;

				if( last_sector == sector_list_value->first_sector )
				{
					/* Merge sector range with previous
					 */
					sector_list_value->first_sector       = ( (libewf_sector_list_value_t *) list_element->previous_element->value )->first_sector;
					sector_list_value->number_of_sectors += ( (libewf_sector_list_value_t *) list_element->previous_element->value )->number_of_sectors;

					/* Remove previous list element
					 */
					remove_element = list_element->previous_element;

					if( remove_element == sector_list->first_element )
					{
						sector_list->first_element = remove_element->next_element;
					}
					if( remove_element == sector_list->last_element )
					{
						sector_list->last_element = remove_element->previous_element;
					}
					if( remove_element->next_element != NULL )
					{
						remove_element->next_element->previous_element = remove_element->previous_element;
					}
					if( remove_element->previous_element != NULL )
					{
						remove_element->previous_element->next_element = remove_element->next_element;
					}
					remove_element->next_element     = NULL;
					remove_element->previous_element = NULL;
					sector_list->number_of_elements -= 1;

					if( libewf_list_element_free(
					     &remove_element,
					     (int (*)(intptr_t **, liberror_error_t **)) &libewf_sector_list_value_free,
					     error ) != 1 )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_RUNTIME,
						 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
						 "%s: unable to free element: %d.",
						 function,
						 element_index );

						return( -1 );
					}
				}
			}
		}
		/* Check if the current range should be merged with the next range
		 */
		if( merge_next_list_element_check != 0 )
		{
			if( list_element == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid list element.",
				 function );

				return( -1 );
			}
			if( list_element->next_element != NULL )
			{
				if( list_element->next_element->value == NULL )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: missing sector list value for next list element.",
					 function );

					return( -1 );
				}
				last_sector = sector_list_value->first_sector + sector_list_value->number_of_sectors;

				if( last_sector == ( (libewf_sector_list_value_t *) list_element->next_element->value )->first_sector )
				{
					/* Merge sector range with next
					 */
					sector_list_value->number_of_sectors += ( (libewf_sector_list_value_t *) list_element->next_element->value )->number_of_sectors;

					/* Remove next list element
					 */
					remove_element = list_element->next_element;

					if( remove_element == sector_list->first_element )
					{
						sector_list->first_element = remove_element->next_element;
					}
					if( remove_element == sector_list->last_element )
					{
						sector_list->last_element = remove_element->previous_element;
					}
					if( remove_element->next_element != NULL )
					{
						remove_element->next_element->previous_element = remove_element->previous_element;
					}
					if( remove_element->previous_element != NULL )
					{
						remove_element->previous_element->next_element = remove_element->next_element;
					}
					remove_element->next_element     = NULL;
					remove_element->previous_element = NULL;
					sector_list->number_of_elements -= 1;

					if( libewf_list_element_free(
					     &remove_element,
					     (int (*)(intptr_t **, liberror_error_t **)) &libewf_sector_list_value_free,
					     error ) != 1 )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_RUNTIME,
						 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
						 "%s: unable to free element: %d.",
						 function,
						 element_index );

						return( -1 );
					}
				}
			}
		}
	}
	if( create_list_element != 0 )
	{
		sector_list_value = NULL;

		if( libewf_sector_list_value_initialize(
		     &sector_list_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create sector list value.",
			 function );

			return( -1 );
		}
		if( sector_list_value == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing sector list value.",
			 function );

			return( -1 );
		}
		sector_list_value->first_sector      = first_sector;
		sector_list_value->number_of_sectors = number_of_sectors;

		list_element = NULL;

		if( libewf_list_element_initialize(
		     &list_element,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create list element.",
			 function );

			libewf_sector_list_value_free(
			 &sector_list_value,
			 NULL );

			return( -1 );
		}
		if( list_element == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing list element.",
			 function );

			libewf_sector_list_value_free(
			 &sector_list_value,
			 NULL );

			return( -1 );
		}
		list_element->value = (intptr_t *) sector_list_value;

		if( sector_list->number_of_elements == 0 )
		{
			if( sector_list->first_element != NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: corruption detected - first element already set.",
				 function );

				libewf_list_element_free(
				 &list_element,
				 (int (*)(intptr_t **, liberror_error_t **)) &libewf_sector_list_value_free,
				 NULL );

				return( -1 );
			}
			if( sector_list->last_element != NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: corruption detected - last element already set.",
				 function );

				libewf_list_element_free(
				 &list_element,
				 (int (*)(intptr_t **, liberror_error_t **)) &libewf_sector_list_value_free,
				 NULL );

				return( -1 );
			}
			sector_list->first_element = list_element;
			sector_list->last_element  = list_element;
		}
		else
		{
			if( sector_list->first_element == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: corruption detected - missing first.",
				 function );

				libewf_list_element_free(
				 &list_element,
				 (int (*)(intptr_t **, liberror_error_t **)) &libewf_sector_list_value_free,
				 NULL );

				return( -1 );
			}
			if( sector_list->last_element == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: corruption detected - missing last.",
				 function );

				libewf_list_element_free(
				 &list_element,
				 (int (*)(intptr_t **, liberror_error_t **)) &libewf_sector_list_value_free,
				 NULL );

				return( -1 );
			}
			if( last_list_element == NULL )
			{
				sector_list->first_element->previous_element = list_element;
				list_element->next_element                   = sector_list->first_element;

				sector_list->first_element = list_element;
			}
			else
			{
				list_element->previous_element = last_list_element;
				list_element->next_element     = last_list_element->next_element;

				if( last_list_element == sector_list->last_element )
				{
					sector_list->last_element = list_element;
				}
				else if( last_list_element->next_element == NULL )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: corruption detected - missing next in last list element.",
					 function );

					return( -1 );
				}
				else
				{
					last_list_element->next_element->previous_element = list_element;
				}
				last_list_element->next_element = list_element;
			}
		}
		sector_list->number_of_elements++;
	}
	return( 1 );
}

/* Retrieves a specific sector
 * Returns 1 if successful or -1 on error
 */
int libewf_sector_list_get_sector(
     libewf_sector_list_t *sector_list,
     int index,
     uint64_t *first_sector,
     uint64_t *number_of_sectors,
     liberror_error_t **error )
{
	libewf_sector_list_value_t *sector_list_value = NULL;
	static char *function                         = "libewf_sector_list_get_sector";

	if( sector_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector list.",
		 function );

		return( -1 );
	}
	if( ( index < 0 )
	 || ( index >= sector_list->number_of_elements ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid index value out of bounds.",
		 function );

		return( -1 );
	}
	if( first_sector == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid first sector.",
		 function );

		return( -1 );
	}
	if( number_of_sectors == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of sectors.",
		 function );

		return( -1 );
	}
	if( ( sector_list->current_element != NULL )
	 && ( sector_list->current_element_index != index ) )
	{
		if( index < sector_list->current_element_index )
		{
			if( ( sector_list->current_element_index - index ) < ( sector_list->number_of_elements / 2 ) )
			{
				while( sector_list->current_element_index > index )
				{
					if( sector_list->current_element == NULL )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_RUNTIME,
						 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
						 "%s: corruption detected in element: %d.",
						 function,
						 sector_list->current_element_index );

						return( -1 );
					}
					sector_list->current_element = sector_list->current_element->next_element;

					sector_list->current_element_index--;
				}
			}
		}
		else
		{
			if( ( index - sector_list->current_element_index ) < ( sector_list->number_of_elements / 2 ) )
			{
				while( sector_list->current_element_index < index )
				{
					if( sector_list->current_element == NULL )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_RUNTIME,
						 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
						 "%s: corruption detected in element: %d.",
						 function,
						 sector_list->current_element_index );

						return( -1 );
					}
					sector_list->current_element = sector_list->current_element->next_element;

					sector_list->current_element_index++;
				}
			}
		}
	}
	if( ( sector_list->current_element == NULL )
	 || ( sector_list->current_element_index != index ) )
	{
		if( index < ( sector_list->number_of_elements / 2 ) )
		{
			sector_list->current_element = sector_list->first_element;

			for( sector_list->current_element_index = 0;
			     sector_list->current_element_index < index;
			     sector_list->current_element_index++ )
			{
				if( sector_list->current_element == NULL )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: corruption detected in element: %d.",
					 function,
					 sector_list->current_element_index );

					return( -1 );
				}
				sector_list->current_element = sector_list->current_element->next_element;
			}
		}
		else
		{
			sector_list->current_element = sector_list->last_element;

			for( sector_list->current_element_index = ( sector_list->number_of_elements - 1 );
			     sector_list->current_element_index > index;
			     sector_list->current_element_index-- )
			{
				if( sector_list->current_element == NULL )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: corruption detected in element: %d.",
					 function,
					 sector_list->current_element_index );

					return( -1 );
				}
				sector_list->current_element = sector_list->current_element->previous_element;
			}
		}
	}
	if( sector_list->current_element == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing sector list element: %d.",
		 function,
		 sector_list->current_element_index );

		return( -1 );
	}
	if( sector_list->current_element->value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing sector list value for list element: %d.",
		 function,
		 sector_list->current_element_index );

		return( -1 );
	}
	sector_list_value = (libewf_sector_list_value_t *) sector_list->current_element->value;

	*first_sector      = sector_list_value->first_sector;
	*number_of_sectors = sector_list_value->number_of_sectors;

	return( 1 );
}

/* Retrieves a specific sector by its value
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libewf_sector_list_get_sector_by_value(
     libewf_sector_list_t *sector_list,
     uint64_t sector_value,
     uint64_t *first_sector,
     uint64_t *number_of_sectors,
     liberror_error_t **error )
{
	libewf_list_element_t *list_element           = NULL;
	libewf_sector_list_value_t *sector_list_value = NULL;
	static char *function                         = "libewf_sector_list_get_sector_by_value";
	int element_index                             = 0;

	if( sector_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector list.",
		 function );

		return( -1 );
	}
	if( first_sector == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid first sector.",
		 function );

		return( -1 );
	}
	if( number_of_sectors == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of sectors.",
		 function );

		return( -1 );
	}
	list_element = sector_list->first_element;

	for( element_index = 0;
	     element_index < sector_list->number_of_elements;
	     element_index++ )
	{
		if( list_element == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected for list element: %d.",
			 function,
			 element_index );

			return( -1 );
		}
		if( list_element->value == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing sector list value for list element: %d.",
			 function,
			 element_index );

			return( -1 );
		}
		sector_list_value = (libewf_sector_list_value_t *) list_element->value;

		if( ( sector_value >= sector_list_value->first_sector )
		 && ( sector_value < (uint64_t) ( sector_list_value->first_sector + sector_list_value->number_of_sectors ) ) )
		{
			*first_sector      = sector_list_value->first_sector;
			*number_of_sectors = sector_list_value->number_of_sectors;

			return( 1 );
		}
		list_element = list_element->next_element;
	}
	return( 0 );
}

/* Determines if a certain sector range is present in the list
 * Returns 1 if present, 0 if not present or -1 on error
 */
int libewf_sector_list_range_is_present(
     libewf_sector_list_t *sector_list,
     uint64_t first_sector,
     uint64_t number_of_sectors,
     liberror_error_t **error )
{
	libewf_list_element_t *list_element           = NULL;
	libewf_sector_list_value_t *sector_list_value = NULL;
	static char *function                         = "libewf_sector_list_range_is_present";
	uint64_t last_sector                          = 0;
	int element_index                             = 0;

	if( sector_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector list.",
		 function );

		return( -1 );
	}
	if( first_sector > (uint64_t) INT64_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid first sector value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( number_of_sectors > (uint64_t) INT64_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid number of sectors value exceeds maximum.",
		 function );

		return( -1 );
	}
	last_sector  = first_sector + number_of_sectors;
	list_element = sector_list->first_element;

	for( element_index = 0;
	     element_index < sector_list->number_of_elements;
	     element_index++ )
	{
		if( list_element == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected for list element: %d.",
			 function,
			 element_index );

			return( -1 );
		}
		if( list_element->value == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing sector list value for list element: %d.",
			 function,
			 element_index );

			return( -1 );
		}
		sector_list_value = (libewf_sector_list_value_t *) list_element->value;

		if( ( first_sector >= sector_list_value->first_sector )
		 && ( first_sector < (uint64_t) ( sector_list_value->first_sector + sector_list_value->number_of_sectors ) ) )
		{
			return( 1 );
		}
		if( ( sector_list_value->first_sector >= first_sector )
		 && ( sector_list_value->first_sector < last_sector ) )
		{
			return( 1 );
		}
		list_element = list_element->next_element;
	}
	return( 0 );
}

