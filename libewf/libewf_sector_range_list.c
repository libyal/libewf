/*
 * Sector range list functions
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
#include <types.h>

#include "libewf_libcdata.h"
#include "libewf_libcerror.h"
#include "libewf_sector_range.h"
#include "libewf_sector_range_list.h"

/* Retrieves a sector range from the list
 * Returns 1 if successful or -1 on error
 */
int libewf_sector_range_list_get_range(
     libcdata_array_t *sector_range_list,
     uint32_t range_index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     libcerror_error_t **error )
{
	libewf_sector_range_t *sector_range = NULL;
	static char *function               = "libewf_sector_range_list_get_range";

	if( libcdata_array_get_entry_by_index(
	     sector_range_list,
	     (int) range_index,
	     (intptr_t **) &sector_range,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sector range: %" PRIu32 " from array.",
		 function,
		 range_index );

		return( -1 );
	}
	if( libewf_sector_range_get(
	     sector_range,
	     start_sector,
	     number_of_sectors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sector range: %" PRIu32 " values.",
		 function,
		 range_index );

		return( -1 );
	}
	return( 1 );
}

/* Appends a sector range to the list
 * Returns 1 if successful or -1 on error
 */
int libewf_sector_range_list_append_range(
     libcdata_array_t *sector_range_list,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libcerror_error_t **error )
{
	libewf_sector_range_t *sector_range = NULL;
	static char *function               = "libewf_sector_range_list_append_range";
	int entry_index                     = 0;

	if( libewf_sector_range_initialize(
	     &sector_range,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create session sector range.",
		 function );

		goto on_error;
	}
	if( libewf_sector_range_set(
	     sector_range,
	     start_sector,
	     number_of_sectors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set session sector range.",
		 function );

		goto on_error;
	}
	if( libcdata_array_append_entry(
	     sector_range_list,
	     &entry_index,
	     (intptr_t *) sector_range,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append session sector range to array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( sector_range != NULL )
	{
		libewf_sector_range_free(
		 &sector_range,
		 NULL );
	}
	return( -1 );
}

