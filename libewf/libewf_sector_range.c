/*
 * Sector range functions
 *
 * Copyright (C) 2006-2020, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libewf_libcerror.h"
#include "libewf_sector_range.h"

/* Creates a sector range
 * Make sure the value sector_range is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_sector_range_initialize(
     libewf_sector_range_t **sector_range,
     libcerror_error_t **error )
{
	static char *function = "libewf_sector_range_initialize";

	if( sector_range == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector range.",
		 function );

		return( -1 );
	}
	if( *sector_range != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid sector range value already set.",
		 function );

		return( -1 );
	}
	*sector_range = memory_allocate_structure(
	                 libewf_sector_range_t );

	if( *sector_range == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create sector range.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *sector_range,
	     0,
	     sizeof( libewf_sector_range_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear sector range.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *sector_range != NULL )
	{
		memory_free(
		 *sector_range );

		*sector_range = NULL;
	}
	return( -1 );
}

/* Frees a sector range
 * Returns 1 if successful or -1 on error
 */
int libewf_sector_range_free(
     libewf_sector_range_t **sector_range,
     libcerror_error_t **error )
{
	static char *function = "libewf_sector_range_free";

	if( sector_range == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector range.",
		 function );

		return( -1 );
	}
	if( *sector_range != NULL )
	{
		memory_free(
		 *sector_range );

		*sector_range = NULL;
	}
	return( 1 );
}

/* Clones the sector range
 * Returns 1 if successful or -1 on error
 */
int libewf_sector_range_clone(
     libewf_sector_range_t **destination_sector_range,
     libewf_sector_range_t *source_sector_range,
     libcerror_error_t **error )
{
	static char *function = "libewf_sector_range_clone";

	if( destination_sector_range == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination sector range.",
		 function );

		return( -1 );
	}
	if( *destination_sector_range != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination sector range already set.",
		 function );

		return( -1 );
	}
	if( source_sector_range == NULL )
	{
		*destination_sector_range = NULL;

		return( 1 );
	}
	*destination_sector_range = memory_allocate_structure(
		                     libewf_sector_range_t );

	if( *destination_sector_range == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination sector range.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_sector_range,
	     source_sector_range,
	     sizeof( libewf_sector_range_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination sector range.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_sector_range != NULL )
	{
		memory_free(
		 *destination_sector_range );

		*destination_sector_range = NULL;
	}
	return( -1 );
}

/* Retrieves a sector range
 * Returns 1 if successful or -1 on error
 */
int libewf_sector_range_get(
     libewf_sector_range_t *sector_range,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     libcerror_error_t **error )
{
	static char *function = "libewf_sector_range_get";

	if( sector_range == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector range.",
		 function );

		return( -1 );
	}
	if( start_sector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid start sector.",
		 function );

		return( -1 );
	}
	if( number_of_sectors == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of sectors.",
		 function );

		return( -1 );
	}
	*start_sector      = sector_range->start_sector;
	*number_of_sectors = sector_range->number_of_sectors;

	return( 1 );
}

/* Sets a sector range
 * Returns 1 if successful or -1 on error
 */
int libewf_sector_range_set(
     libewf_sector_range_t *sector_range,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libcerror_error_t **error )
{
	static char *function = "libewf_sector_range_set";

	if( sector_range == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector range.",
		 function );

		return( -1 );
	}
	if( start_sector > (uint64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid start sector value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( number_of_sectors > (uint64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid number of sectors value exceeds maximum.",
		 function );

		return( -1 );
	}
	sector_range->start_sector      = start_sector;
	sector_range->end_sector        = start_sector + number_of_sectors;
	sector_range->number_of_sectors = number_of_sectors;

	return( 1 );
}

