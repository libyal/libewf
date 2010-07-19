/*
 * Error sector table functions, for checksum and acquiry read errrors
 *
 * Copyright (c) 2006-2010, Joachim Metz <jbmetz@users.sourceforge.net>
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

#include <liberror.h>

#include "libewf_sector_table.h"

/* Initialize the sector table
 * Returns 1 if successful or -1 on error
 */
int libewf_sector_table_initialize(
     libewf_sector_table_t **sector_table,
     uint32_t number_of_sectors,
     liberror_error_t **error )
{
	static char *function    = "libewf_sector_table_initialize";
	size_t sector_table_size = 0;

	if( sector_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector table.",
		 function );

		return( -1 );
	}
	if( *sector_table == NULL )
	{
		sector_table_size = sizeof( libewf_sector_table_entry_t ) * number_of_sectors;

		if( sector_table_size > (size_t) SSIZE_MAX )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid sector table size value exceeds maximum.",
			 function );

			return( -1 );
		}
		*sector_table = (libewf_sector_table_t *) memory_allocate(
		                                           sizeof( libewf_sector_table_t ) );

		if( *sector_table == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create sector table.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *sector_table,
		     0,
		     sizeof( libewf_sector_table_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear sector table.",
			 function );

			memory_free(
			 *sector_table );

			*sector_table = NULL;

			return( -1 );
		}
		if( number_of_sectors > 0 )
		{
			( *sector_table )->sector = (libewf_sector_table_entry_t *) memory_allocate(
			                                                             sector_table_size );

			if( ( *sector_table )->sector == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create sector array.",
				 function );

				memory_free(
				 *sector_table );

				*sector_table = NULL;

				return( -1 );
			}
			if( memory_set(
			     ( *sector_table )->sector,
			     0,
			     sector_table_size ) == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear sector array.",
				 function );

				memory_free(
				 ( *sector_table )->sector );
				memory_free(
				 *sector_table );

				*sector_table = NULL;

				return( -1 );
			}
		}
		( *sector_table )->number_of_sectors = number_of_sectors;
	}
	return( 1 );
}

/* Frees the sector table including elements
 * Returns 1 if successful or -1 on error
 */
int libewf_sector_table_free(
     libewf_sector_table_t **sector_table,
     liberror_error_t **error )
{
	static char *function = "libewf_sector_table_free";

	if( sector_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector table.",
		 function );

		return( -1 );
	}
	if( *sector_table != NULL )
	{
		if( ( *sector_table )->sector != NULL )
		{
			memory_free(
			 ( *sector_table )->sector );
		}
		memory_free(
		 *sector_table );
	}
	return( 1 );
}

/* Resizes the sector table
 * Returns 1 if successful or -1 on error
 */
int libewf_sector_table_resize(
     libewf_sector_table_t *sector_table,
     uint32_t number_of_sectors,
     liberror_error_t **error )
{
	void *reallocation       = NULL;
	static char *function    = "libewf_sector_table_resize";
	size_t sector_table_size = 0;

	if( sector_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector table.",
		 function );

		return( -1 );
	}
	if( sector_table->number_of_sectors < number_of_sectors )
	{
		sector_table_size = sizeof( libewf_sector_table_entry_t ) * number_of_sectors;

		if( sector_table_size > (size_t) SSIZE_MAX )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid sector table size value exceeds maximum.",
			 function );

			return( -1 );
		}
		reallocation = memory_reallocate(
				sector_table->sector,
				sector_table_size );

		if( reallocation == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to resize sector array.",
			 function );

			return( -1 );
		}
		sector_table->sector = (libewf_sector_table_entry_t *) reallocation;

		if( memory_set(
		     &( sector_table->sector[ sector_table->number_of_sectors ] ),
		     0,
		     ( sizeof( libewf_sector_table_entry_t ) * ( number_of_sectors - sector_table->number_of_sectors ) ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear sector array.",
			 function );

			return( 1 );
		}
		sector_table->number_of_sectors = number_of_sectors;
	}
	return( 1 );
}

/* Retrieves the information of a sector
 * Returns 1 if successful, 0 if no sector could be found or -1 on error
 */
int libewf_sector_table_get_sector(
     libewf_sector_table_t *sector_table,
     uint32_t index,
     uint64_t *first_sector,
     uint64_t *number_of_sectors,
     liberror_error_t **error )
{
	static char *function = "libewf_sector_table_get_sector";

	if( sector_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector table.",
		 function );

		return( -1 );
	}
	if( sector_table->number_of_sectors == 0 )
	{
		return( 0 );
	}
	if( sector_table->sector == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid sector table - missing sectors.",
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
	if( index >= sector_table->number_of_sectors )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid index out of bounds.",
		 function );

		return( -1 );
	}
	*first_sector      = sector_table->sector[ index ].first_sector;
	*number_of_sectors = sector_table->sector[ index ].number_of_sectors;

	return( 1 );
}

/* Add a sector
 * Returns 1 if successful or -1 on error
 */
int libewf_sector_table_add_sector(
     libewf_sector_table_t *sector_table,
     uint64_t first_sector,
     uint64_t number_of_sectors,
     int merge_continious_entries,
     liberror_error_t **error )
{
	static char *function      = "libewf_sector_table_add_sector";
	uint64_t last_sector       = 0;
	uint64_t last_range_sector = 0;
	uint32_t iterator          = 0;

	if( sector_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector table.",
		 function );

		return( -1 );
	}
	if( ( merge_continious_entries != 0 )
	 && ( sector_table->sector != NULL ) )
	{
		/* Check if the sector is already in the table
		 */
		for( iterator = 0;
		     iterator < sector_table->number_of_sectors;
		     iterator++ )
		{
			last_range_sector = sector_table->sector[ iterator ].first_sector
			                  + sector_table->sector[ iterator ].number_of_sectors;

			if( ( first_sector >= sector_table->sector[ iterator ].first_sector )
			 && ( first_sector <= last_range_sector ) )
			{
				/* Merge current sector with existing
				 */
				last_sector = first_sector + number_of_sectors;

				if( last_sector > last_range_sector )
				{
					sector_table->sector[ iterator ].number_of_sectors += last_sector - last_range_sector;
				}
				return( 1 );
			}
		}
	}
	/* Create a new sector
	 */
	if( libewf_sector_table_resize(
	     sector_table,
	     sector_table->number_of_sectors + 1,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize sector table.",
		 function );

		return( -1 );
	}
	sector_table->sector[ sector_table->number_of_sectors - 1 ].first_sector      = first_sector;
	sector_table->sector[ sector_table->number_of_sectors - 1 ].number_of_sectors = number_of_sectors;

	return( 1 );
}

