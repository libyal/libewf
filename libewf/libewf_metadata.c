/*
 * Metadata functions
 *
 * Copyright (C) 2006-2016, Joachim Metz <joachim.metz@gmail.com>
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

#include "libewf_codepage.h"
#include "libewf_date_time_values.h"
#include "libewf_definitions.h"
#include "libewf_handle.h"
#include "libewf_hash_values.h"
#include "libewf_header_values.h"
#include "libewf_libcerror.h"
#include "libewf_libcthreads.h"
#include "libewf_libcdata.h"
#include "libewf_libcnotify.h"
#include "libewf_libcstring.h"
#include "libewf_libfvalue.h"
#include "libewf_metadata.h"
#include "libewf_sector_range.h"
#include "libewf_types.h"

/* Retrieves the number of sectors per chunk
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_sectors_per_chunk(
     libewf_handle_t *handle,
     uint32_t *sectors_per_chunk,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_sectors_per_chunk";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( sectors_per_chunk == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sectors per chunk.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->media_values->sectors_per_chunk > (uint32_t) INT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid sectors per chunk value exceeds maximum.",
		 function );

		goto on_error;
	}
	*sectors_per_chunk = internal_handle->media_values->sectors_per_chunk;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Sets the number of sectors per chunk
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_sectors_per_chunk(
     libewf_handle_t *handle,
     uint32_t sectors_per_chunk,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_sectors_per_chunk";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_handle->read_io_handle != NULL )
	 || ( internal_handle->write_io_handle == NULL )
	 || ( internal_handle->write_io_handle->values_initialized != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: sectors per chunk cannot be changed.",
		 function );

		goto on_error;
	}
	if( libewf_internal_handle_set_media_values(
	     internal_handle,
	     sectors_per_chunk,
	     internal_handle->media_values->bytes_per_sector,
	     internal_handle->media_values->media_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set media values.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the number of bytes per sector
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_bytes_per_sector(
     libewf_handle_t *handle,
     uint32_t *bytes_per_sector,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_bytes_per_sector";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( bytes_per_sector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bytes per sector.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->media_values->bytes_per_sector > (uint32_t) INT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid bytes per sector value exceeds maximum.",
		 function );

		goto on_error;
	}
	*bytes_per_sector = internal_handle->media_values->bytes_per_sector;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Sets the number of bytes per sector
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_bytes_per_sector(
     libewf_handle_t *handle,
     uint32_t bytes_per_sector,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_bytes_per_sector";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_handle->read_io_handle != NULL )
	 || ( internal_handle->write_io_handle == NULL )
	 || ( internal_handle->write_io_handle->values_initialized != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: bytes per sector cannot be changed.",
		 function );

		goto on_error;
	}
	if( libewf_internal_handle_set_media_values(
	     internal_handle,
	     internal_handle->media_values->sectors_per_chunk,
	     bytes_per_sector,
	     internal_handle->media_values->media_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set media values.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the number of sectors
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_number_of_sectors(
     libewf_handle_t *handle,
     uint64_t *number_of_sectors,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_number_of_sectors";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( number_of_sectors == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bytes per sector.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->media_values->number_of_sectors > (uint64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid number of sectors value exceeds maximum.",
		 function );

		goto on_error;
	}
	*number_of_sectors = internal_handle->media_values->number_of_sectors;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the chunk size
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_chunk_size(
     libewf_handle_t *handle,
     size32_t *chunk_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_chunk_size";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( chunk_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->media_values->chunk_size > (size32_t) INT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid chunk size value exceeds maximum.",
		 function );

		goto on_error;
	}
	*chunk_size = internal_handle->media_values->chunk_size;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the error granularity
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_error_granularity(
     libewf_handle_t *handle,
     uint32_t *error_granularity,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_error_granularity";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( error_granularity == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid error granularity.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->media_values->error_granularity > (uint32_t) INT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid error granularity value exceeds maximum.",
		 function );

		goto on_error;
	}
	*error_granularity = internal_handle->media_values->error_granularity;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Sets the error granularity
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_error_granularity(
     libewf_handle_t *handle,
     uint32_t error_granularity,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_error_granularity";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_handle->write_io_handle == NULL )
	 || ( internal_handle->write_io_handle->values_initialized != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: error granularity cannot be changed.",
		 function );

		goto on_error;
	}
	internal_handle->media_values->error_granularity = error_granularity;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the compression method
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_compression_method(
     libewf_handle_t *handle,
     uint16_t *compression_method,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_compression_method";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( compression_method == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compression method.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*compression_method = internal_handle->io_handle->compression_method;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Sets the compression method
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_compression_method(
     libewf_handle_t *handle,
     uint16_t compression_method,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_compression_method";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ( compression_method != LIBEWF_COMPRESSION_METHOD_DEFLATE )
	 && ( compression_method != LIBEWF_COMPRESSION_METHOD_BZIP2 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported compression method.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_handle->write_io_handle == NULL )
	 || ( internal_handle->write_io_handle->values_initialized != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: compression values cannot be changed.",
		 function );

		goto on_error;
	}
	if( ( compression_method == LIBEWF_COMPRESSION_METHOD_BZIP2 )
	 && ( internal_handle->io_handle->segment_file_type != LIBEWF_SEGMENT_FILE_TYPE_EWF2 )
	 && ( internal_handle->io_handle->segment_file_type != LIBEWF_SEGMENT_FILE_TYPE_EWF2_LOGICAL ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: compression method not supported by format.",
		 function );

		goto on_error;
	}
	internal_handle->io_handle->compression_method = compression_method;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the compression values
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_compression_values(
     libewf_handle_t *handle,
     int8_t *compression_level,
     uint8_t *compression_flags,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_compression_values";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( compression_level == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compression level.",
		 function );

		return( -1 );
	}
	if( compression_flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compression flags.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*compression_level = internal_handle->io_handle->compression_level;
	*compression_flags = internal_handle->io_handle->compression_flags;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Sets the compression values
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_compression_values(
     libewf_handle_t *handle,
     int8_t compression_level,
     uint8_t compression_flags,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_compression_values";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ( compression_level != LIBEWF_COMPRESSION_NONE )
	 && ( compression_level != LIBEWF_COMPRESSION_FAST )
	 && ( compression_level != LIBEWF_COMPRESSION_BEST ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported compression level.",
		 function );

		return( -1 );
	}
	if( ( compression_flags & ~( LIBEWF_COMPRESS_FLAG_USE_EMPTY_BLOCK_COMPRESSION ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported compression flags.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_handle->write_io_handle == NULL )
	 || ( internal_handle->write_io_handle->values_initialized != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: compression values cannot be changed.",
		 function );

		goto on_error;
	}
	if( ( internal_handle->io_handle->segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF2 )
	 && ( internal_handle->io_handle->segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF2_LOGICAL ) )
	{
		compression_flags |= LIBEWF_COMPRESS_FLAG_USE_PATTERN_FILL_COMPRESSION;
	}
	internal_handle->io_handle->compression_level = compression_level;
	internal_handle->io_handle->compression_flags = compression_flags;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the size of the contained media data
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_media_size(
     libewf_handle_t *handle,
     size64_t *media_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_media_size";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( media_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*media_size = internal_handle->media_values->media_size;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Sets the media size
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_media_size(
     libewf_handle_t *handle,
     size64_t media_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_media_size";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_handle->read_io_handle != NULL )
	 || ( internal_handle->write_io_handle == NULL )
	 || ( internal_handle->write_io_handle->values_initialized != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: media size cannot be changed.",
		 function );

		goto on_error;
	}
	if( libewf_internal_handle_set_media_values(
	     internal_handle,
	     internal_handle->media_values->sectors_per_chunk,
	     internal_handle->media_values->bytes_per_sector,
	     media_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set media values.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the media type value
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_media_type(
     libewf_handle_t *handle,
     uint8_t *media_type,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_media_type";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values->media_type > (uint8_t) INT8_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid media type value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( media_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media type.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*media_type = internal_handle->media_values->media_type;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Sets the media type
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_media_type(
     libewf_handle_t *handle,
     uint8_t media_type,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_media_type";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_handle->read_io_handle != NULL )
	 || ( internal_handle->write_io_handle == NULL )
	 || ( internal_handle->write_io_handle->values_initialized != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: media type cannot be changed.",
		 function );

		goto on_error;
	}
	internal_handle->media_values->media_type = media_type;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the media flags
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_media_flags(
     libewf_handle_t *handle,
     uint8_t *media_flags,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_media_flags";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( media_flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media flags.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->media_values->media_flags > (uint8_t) INT8_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid media flags value exceeds maximum.",
		 function );

		goto on_error;
	}
	*media_flags = internal_handle->media_values->media_flags;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Sets the media flags
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_media_flags(
     libewf_handle_t *handle,
     uint8_t media_flags,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_media_flags";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_handle->read_io_handle != NULL )
	 || ( internal_handle->write_io_handle == NULL )
	 || ( internal_handle->write_io_handle->values_initialized != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: media flags cannot be changed.",
		 function );

		goto on_error;
	}
	internal_handle->media_values->media_flags = media_flags;

	/* Make sure the lowest bit is always set
	 */
	internal_handle->media_values->media_flags |= 0x01;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the format type value
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_format(
     libewf_handle_t *handle,
     uint8_t *format,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_format";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( format == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid format.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->io_handle->format > (uint8_t) INT8_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid format value exceeds maximum.",
		 function );

		goto on_error;
	}
	*format = internal_handle->io_handle->format;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Sets the output format
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_format(
     libewf_handle_t *handle,
     uint8_t format,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_format";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ( format != LIBEWF_FORMAT_ENCASE1 )
	 && ( format != LIBEWF_FORMAT_ENCASE2 )
	 && ( format != LIBEWF_FORMAT_ENCASE3 )
	 && ( format != LIBEWF_FORMAT_ENCASE4 )
	 && ( format != LIBEWF_FORMAT_ENCASE5 )
	 && ( format != LIBEWF_FORMAT_ENCASE6 )
	 && ( format != LIBEWF_FORMAT_ENCASE7 )
	 && ( format != LIBEWF_FORMAT_SMART )
	 && ( format != LIBEWF_FORMAT_FTK_IMAGER )
	 && ( format != LIBEWF_FORMAT_LINEN5 )
	 && ( format != LIBEWF_FORMAT_LINEN6 )
	 && ( format != LIBEWF_FORMAT_LINEN7 )
	 && ( format != LIBEWF_FORMAT_V2_ENCASE7 )
/* TODO add support for: L01, Lx01:
	 && ( format != LIBEWF_FORMAT_LOGICAL_ENCASE5 )
	 && ( format != LIBEWF_FORMAT_LOGICAL_ENCASE6 )
	 && ( format != LIBEWF_FORMAT_LOGICAL_ENCASE7 )
	 && ( format != LIBEWF_FORMAT_V2_LOGICAL_ENCASE7 )
*/
	 && ( format != LIBEWF_FORMAT_EWF )
	 && ( format != LIBEWF_FORMAT_EWFX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format: %d.",
		 function,
	         format );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_handle->read_io_handle != NULL )
	 || ( internal_handle->write_io_handle == NULL )
	 || ( internal_handle->write_io_handle->values_initialized != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: format cannot be changed.",
		 function );

		goto on_error;
	}
/* TODO refactor into separate function */
	internal_handle->io_handle->format = format;

	if( format == LIBEWF_FORMAT_V2_ENCASE7 )
	{
		internal_handle->io_handle->major_version = 2;
		internal_handle->io_handle->minor_version = 1;
	}
	else
	{
		internal_handle->io_handle->major_version = 1;
		internal_handle->io_handle->minor_version = 0;
	}
	if( ( format == LIBEWF_FORMAT_EWF )
	 || ( format == LIBEWF_FORMAT_SMART ) )
	{
		/* Wraps .s01 to .s99 and then to .saa up to .zzz
		 * ( ( ( 's' to 'z' = 8 ) * 26 * 26 ) + 99 ) = 5507
		 */
		internal_handle->write_io_handle->maximum_number_of_segments = (uint32_t) 5507;
		internal_handle->io_handle->segment_file_type                = LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART;
	}
	else if( format == LIBEWF_FORMAT_V2_ENCASE7 )
	{
		/* Wraps .Ex01 to .Ex99 and then to .ExAA up to .EzZZ
		 * ( ( ( 'x' to 'z' = 3 ) * 26 * 26 ) + 99 ) = 2127
		 */
		internal_handle->write_io_handle->maximum_number_of_segments = (uint32_t) 2127;
		internal_handle->io_handle->segment_file_type                = LIBEWF_SEGMENT_FILE_TYPE_EWF2;
	}
	else
	{
		/* Wraps .E01 to .E99 and then to .EAA up to .ZZZ
		 * ( ( ( 'E' to 'Z' or 'e' to 'z' = 22 ) * 26 * 26 ) + 99 ) = 14971
		 */
		internal_handle->write_io_handle->maximum_number_of_segments = (uint32_t) 14971;
		internal_handle->io_handle->segment_file_type                = LIBEWF_SEGMENT_FILE_TYPE_EWF1;
	}
	/* Determine the maximum number of table entries
	 */
	if( ( format == LIBEWF_FORMAT_ENCASE6 )
	 || ( format == LIBEWF_FORMAT_ENCASE7 ) )
	{
		internal_handle->write_io_handle->maximum_segment_file_size  = INT64_MAX;
		internal_handle->write_io_handle->maximum_chunks_per_section = LIBEWF_MAXIMUM_TABLE_ENTRIES_ENCASE6;
	}
	else if( format == LIBEWF_FORMAT_V2_ENCASE7 )
	{
		internal_handle->write_io_handle->maximum_segment_file_size  = INT64_MAX;
		internal_handle->write_io_handle->maximum_chunks_per_section = LIBEWF_MAXIMUM_TABLE_ENTRIES;
	}
	else if( format == LIBEWF_FORMAT_EWFX )
	{
		internal_handle->write_io_handle->maximum_segment_file_size  = INT32_MAX;
		internal_handle->write_io_handle->maximum_chunks_per_section = LIBEWF_MAXIMUM_TABLE_ENTRIES;
	}
	else
	{
		internal_handle->write_io_handle->maximum_segment_file_size  = INT32_MAX;
		internal_handle->write_io_handle->maximum_chunks_per_section = LIBEWF_MAXIMUM_TABLE_ENTRIES_EWF;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the segment file version
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_segment_file_version(
     libewf_handle_t *handle,
     uint8_t *major_version,
     uint8_t *minor_version,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_segment_file_version";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( major_version == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid major version.",
		 function );

		return( -1 );
	}
	if( minor_version == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid minor version.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*major_version = internal_handle->io_handle->major_version;
	*minor_version = internal_handle->io_handle->minor_version;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Retrieves the segment file set identifier
 * The identifier is a GUID and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_segment_file_set_identifier(
     libewf_handle_t *handle,
     uint8_t *set_identifier,
     size_t size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_segment_file_set_identifier";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( set_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid set identifier.",
		 function );

		return( -1 );
	}
	if( size < 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: set identifier too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( memory_copy(
	     set_identifier,
	     internal_handle->media_values->set_identifier,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set set identifier.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Sets the segment file set identifier
 * The identifier is a GUID and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_segment_file_set_identifier(
     libewf_handle_t *handle,
     const uint8_t *set_identifier,
     size_t size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_segment_file_set_identifier";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( set_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid set identifier.",
		 function );

		return( -1 );
	}
	if( size < 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: set identifier too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_handle->read_io_handle != NULL )
	 || ( internal_handle->write_io_handle == NULL )
	 || ( internal_handle->write_io_handle->values_initialized != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: set identifier cannot be changed.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     internal_handle->media_values->set_identifier,
	     set_identifier,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy set indentifier.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the MD5 hash
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_handle_get_md5_hash(
     libewf_handle_t *handle,
     uint8_t *md5_hash,
     size_t size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_md5_hash";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->hash_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing hash sections.",
		 function );

		return( -1 );
	}
	if( md5_hash == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MD5 hash.",
		 function );

		return( -1 );
	}
	if( size < 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: MD5 hash too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_handle->hash_sections->md5_hash_set == 0 )
	 && ( internal_handle->hash_values != NULL ) )
	{
		if( libewf_hash_values_generate_md5_hash(
		     internal_handle->hash_values,
		     internal_handle->hash_sections->md5_hash,
		     16,
		     &( internal_handle->hash_sections->md5_hash_set ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to parse MD5 hash value for its value.",
			 function );

			goto on_error;
		}
	}
	if( internal_handle->hash_sections->md5_digest_set != 0 )
	{
		if( memory_copy(
		     md5_hash,
		     internal_handle->hash_sections->md5_digest,
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy MD5 digest.",
			 function );

			goto on_error;
		}
		result = 1;
	}
	else if( internal_handle->hash_sections->md5_hash_set != 0 )
	{
		if( memory_copy(
		     md5_hash,
		     internal_handle->hash_sections->md5_hash,
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy MD5 hash.",
			 function );

			goto on_error;
		}
		result = 1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Sets the MD5 hash
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_md5_hash(
     libewf_handle_t *handle,
     const uint8_t *md5_hash,
     size_t size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_md5_hash";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->hash_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing hash sections.",
		 function );

		return( -1 );
	}
	if( md5_hash == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MD5 hash.",
		 function );

		return( -1 );
	}
	if( size < 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: MD5 hash too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_handle->read_io_handle != NULL )
	 || ( internal_handle->hash_sections->md5_hash_set != 0 )
	 || ( internal_handle->hash_sections->md5_digest_set != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: MD5 hash cannot be changed.",
		 function );

		goto on_error;
	}
	if( internal_handle->hash_values == NULL )
	{
		if( libewf_hash_values_initialize(
		     &( internal_handle->hash_values ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create hash values.",
			 function );

			goto on_error;
		}
		internal_handle->hash_values_parsed = 1;
	}
	if( memory_copy(
	     internal_handle->hash_sections->md5_hash,
	     md5_hash,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set MD5 hash.",
		 function );

		goto on_error;
	}
	internal_handle->hash_sections->md5_hash_set = 1;

	if( memory_copy(
	     internal_handle->hash_sections->md5_digest,
	     md5_hash,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set MD5 hash.",
		 function );

		goto on_error;
	}
	internal_handle->hash_sections->md5_digest_set = 1;

	if( libewf_hash_values_parse_md5_hash(
	     internal_handle->hash_values,
	     md5_hash,
	     16,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to parse MD5 hash for its value.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the SHA1 hash
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_handle_get_sha1_hash(
     libewf_handle_t *handle,
     uint8_t *sha1_hash,
     size_t size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_sha1_hash";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->hash_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing hash sections.",
		 function );

		return( -1 );
	}
	if( sha1_hash == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid SHA1 hash.",
		 function );

		return( -1 );
	}
	if( size < 20 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: SHA1 hash too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_handle->hash_sections->sha1_digest_set == 0 )
	 && ( internal_handle->hash_values != NULL ) )
	{
		if( libewf_hash_values_generate_sha1_hash(
		     internal_handle->hash_values,
		     internal_handle->hash_sections->sha1_digest,
		     20,
		     &( internal_handle->hash_sections->sha1_digest_set ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to parse MD5 hash value for its value.",
			 function );

			goto on_error;
		}
	}
	if( internal_handle->hash_sections->sha1_digest_set != 0 )
	{
		if( memory_copy(
		     sha1_hash,
		     internal_handle->hash_sections->sha1_digest,
		     20 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy SHA1 digest.",
			 function );

			goto on_error;
		}
		result = 1;
	}
	else if( internal_handle->hash_sections->sha1_hash_set != 0 )
	{
		if( memory_copy(
		     sha1_hash,
		     internal_handle->hash_sections->sha1_hash,
		     20 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy SHA1 hash.",
			 function );

			goto on_error;
		}
		result = 1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Sets the SHA1 hash
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_sha1_hash(
     libewf_handle_t *handle,
     const uint8_t *sha1_hash,
     size_t size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_sha1_hash";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->hash_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing hash sections.",
		 function );

		return( -1 );
	}
	if( sha1_hash == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid SHA1 hash.",
		 function );

		return( -1 );
	}
	if( size < 20 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: SHA1 hash too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_handle->read_io_handle != NULL )
	 || ( internal_handle->hash_sections->sha1_hash_set != 0 )
	 || ( internal_handle->hash_sections->sha1_digest_set != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: SHA1 hash cannot be changed.",
		 function );

		goto on_error;
	}
	if( internal_handle->hash_values == NULL )
	{
		if( libewf_hash_values_initialize(
		     &( internal_handle->hash_values ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create hash values.",
			 function );

			goto on_error;
		}
		internal_handle->hash_values_parsed = 1;
	}
	if( memory_copy(
	     internal_handle->hash_sections->sha1_hash,
	     sha1_hash,
	     20 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set SHA1 hash.",
		 function );

		goto on_error;
	}
	internal_handle->hash_sections->sha1_hash_set = 1;

	if( memory_copy(
	     internal_handle->hash_sections->sha1_digest,
	     sha1_hash,
	     20 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set SHA1 hash.",
		 function );

		goto on_error;
	}
	internal_handle->hash_sections->sha1_digest_set = 1;

	if( libewf_hash_values_parse_sha1_hash(
	     internal_handle->hash_values,
	     sha1_hash,
	     20,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to parse SHA1 hash for its value.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Sets the read zero chunk on error
 * The chunk is not zeroed if read raw is used
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_read_zero_chunk_on_error(
     libewf_handle_t *handle,
     uint8_t zero_on_error,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_read_zero_chunk_on_error";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	internal_handle->io_handle->zero_on_error = zero_on_error;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Copies the media values from the source to the destination handle
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_copy_media_values(
     libewf_handle_t *destination_handle,
     libewf_handle_t *source_handle,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_destination_handle = NULL;
	libewf_internal_handle_t *internal_source_handle      = NULL;
	static char *function                                 = "libewf_handle_copy_media_values";

	if( destination_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination handle.",
		 function );

		return( -1 );
	}
	if( source_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source handle.",
		 function );

		return( -1 );
	}
	internal_destination_handle = (libewf_internal_handle_t *) destination_handle;
	internal_source_handle      = (libewf_internal_handle_t *) source_handle;

	if( internal_source_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid source handle - missing media values.",
		 function );

		return( -1 );
	}
	if( internal_destination_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid destination handle - missing media values.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     internal_destination_handle->media_values,
	     internal_source_handle->media_values,
	     sizeof( libewf_media_values_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy media values.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of acquiry errors
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_number_of_acquiry_errors(
     libewf_handle_t *handle,
     uint32_t *number_of_errors,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_number_of_acquiry_errors";
	int number_of_elements                    = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( number_of_errors == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of errors.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->acquiry_errors != NULL )
	{
		if( libcdata_range_list_get_number_of_elements(
		     internal_handle->acquiry_errors,
		     &number_of_elements,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of elements from acquiry errors range list.",
			 function );

			goto on_error;
		}
	}
	if( number_of_elements < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of elements value out of bounds.",
		 function );

		goto on_error;
	}
	*number_of_errors = (uint32_t) number_of_elements;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves an acquiry error
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_acquiry_error(
     libewf_handle_t *handle,
     uint32_t index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_acquiry_error";
	intptr_t *value                           = NULL;
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libcdata_range_list_get_range_by_index(
	          internal_handle->acquiry_errors,
	          (int) index,
	          start_sector,
	          number_of_sectors,
	          &value,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve acquiry error: %" PRIu32 ".",
		 function,
		 index );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Append an acquiry error
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_append_acquiry_error(
     libewf_handle_t *handle,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_append_acquiry_error";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libcdata_range_list_insert_range(
	          internal_handle->acquiry_errors,
	          start_sector,
	          number_of_sectors,
	          NULL,
	          NULL,
	          NULL,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to insert acquiry error in range list.",
		 function );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the number of checksum errors
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_number_of_checksum_errors(
     libewf_handle_t *handle,
     uint32_t *number_of_errors,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_number_of_checksum_errors";
	uint32_t number_of_elements               = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( number_of_errors == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of errors.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->chunk_table != NULL )
	{
		if( libewf_chunk_table_get_number_of_checksum_errors(
		     internal_handle->chunk_table,
		     &number_of_elements,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of checksum errors.",
			 function );

			goto on_error;
		}
	}
	*number_of_errors = number_of_elements;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves a checksum error
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_checksum_error(
     libewf_handle_t *handle,
     uint32_t error_index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_checksum_error";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->chunk_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing chunk table.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libewf_chunk_table_get_checksum_error(
	          internal_handle->chunk_table,
	          error_index,
	          start_sector,
	          number_of_sectors,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve checksum error: %" PRIu32 ".",
		 function,
		 error_index );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Appends a checksum error
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_append_checksum_error(
     libewf_handle_t *handle,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_append_checksum_error";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libewf_chunk_table_append_checksum_error(
	          internal_handle->chunk_table,
	          start_sector,
	          number_of_sectors,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append checksum error.",
		 function );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the number of sessions
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_number_of_sessions(
     libewf_handle_t *handle,
     uint32_t *number_of_sessions,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_number_of_sessions";
	int number_of_entries                     = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( number_of_sessions == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of sessions.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->sessions != NULL )
	{
		if( libcdata_array_get_number_of_entries(
		     internal_handle->sessions,
		     &number_of_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of entries from sessions array.",
			 function );

			goto on_error;
		}
	}
	if( number_of_entries < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of entries value out of bounds.",
		 function );

		goto on_error;
	}
	*number_of_sessions = (uint32_t) number_of_entries;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves a session
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_session(
     libewf_handle_t *handle,
     uint32_t index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	libewf_sector_range_t *sector_range       = NULL;
	static char *function                     = "libewf_handle_get_session";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libcdata_array_get_entry_by_index(
	     internal_handle->sessions,
	     (int) index,
	     (intptr_t **) &sector_range,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve session sector range: %" PRIu32 " from array.",
		 function,
		 index );

		goto on_error;
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
		 "%s: unable to retrieve session: %" PRIu32 " sector range.",
		 function,
		 index );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Appends a session
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_append_session(
     libewf_handle_t *handle,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	libewf_sector_range_t *sector_range       = NULL;
	static char *function                     = "libewf_handle_append_session";
	int entry_index                           = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
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
	     internal_handle->sessions,
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
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
	if( sector_range != NULL )
	{
		libewf_sector_range_free(
		 &sector_range,
		 NULL );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the number of tracks
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_number_of_tracks(
     libewf_handle_t *handle,
     uint32_t *number_of_tracks,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_number_of_tracks";
	int number_of_entries                     = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( number_of_tracks == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of tracks.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->tracks != NULL )
	{
		if( libcdata_array_get_number_of_entries(
		     internal_handle->tracks,
		     &number_of_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of entries from tracks array.",
			 function );

			goto on_error;
		}
	}
	if( number_of_entries < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of entries value out of bounds.",
		 function );

		goto on_error;
	}
	*number_of_tracks = (uint32_t) number_of_entries;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves a track
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_track(
     libewf_handle_t *handle,
     uint32_t index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	libewf_sector_range_t *sector_range       = NULL;
	static char *function                     = "libewf_handle_get_track";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libcdata_array_get_entry_by_index(
	     internal_handle->tracks,
	     (int) index,
	     (intptr_t **) &sector_range,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve track sector range: %" PRIu32 " from array.",
		 function,
		 index );

		goto on_error;
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
		 "%s: unable to retrieve track: %" PRIu32 " sector range.",
		 function,
		 index );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Appends a track
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_append_track(
     libewf_handle_t *handle,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	libewf_sector_range_t *sector_range       = NULL;
	static char *function                     = "libewf_handle_append_track";
	int entry_index                           = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libewf_sector_range_initialize(
	     &sector_range,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create track sector range.",
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
		 "%s: unable to set track sector range.",
		 function );

		goto on_error;
	}
	if( libcdata_array_append_entry(
	     internal_handle->tracks,
	     &entry_index,
	     (intptr_t *) sector_range,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append track sector range to array.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
	if( sector_range != NULL )
	{
		libewf_sector_range_free(
		 &sector_range,
		 NULL );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the header codepage
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_header_codepage(
     libewf_handle_t *handle,
     int *header_codepage,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_header_codepage";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( header_codepage == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header codepage.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*header_codepage = internal_handle->io_handle->header_codepage;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Sets the header codepage
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_header_codepage(
     libewf_handle_t *handle,
     int header_codepage,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_header_codepage";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ( header_codepage != LIBEWF_CODEPAGE_ASCII )
	 && ( header_codepage != LIBEWF_CODEPAGE_WINDOWS_874 )
	 && ( header_codepage != LIBEWF_CODEPAGE_WINDOWS_932 )
	 && ( header_codepage != LIBEWF_CODEPAGE_WINDOWS_936 )
	 && ( header_codepage != LIBEWF_CODEPAGE_WINDOWS_1250 )
	 && ( header_codepage != LIBEWF_CODEPAGE_WINDOWS_1251 )
	 && ( header_codepage != LIBEWF_CODEPAGE_WINDOWS_1252 )
	 && ( header_codepage != LIBEWF_CODEPAGE_WINDOWS_1253 )
	 && ( header_codepage != LIBEWF_CODEPAGE_WINDOWS_1254 )
	 && ( header_codepage != LIBEWF_CODEPAGE_WINDOWS_1255 )
	 && ( header_codepage != LIBEWF_CODEPAGE_WINDOWS_1256 )
	 && ( header_codepage != LIBEWF_CODEPAGE_WINDOWS_1257 )
	 && ( header_codepage != LIBEWF_CODEPAGE_WINDOWS_1258 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported header codepage.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	internal_handle->io_handle->header_codepage = header_codepage;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Retrieves the header value date format
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_header_values_date_format(
     libewf_handle_t *handle,
     int *date_format,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_header_values_date_format";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( date_format == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid date format.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*date_format = internal_handle->date_format;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Sets the header values date format
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_header_values_date_format(
     libewf_handle_t *handle,
     int date_format,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_header_values_date_format";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( ( date_format != LIBEWF_DATE_FORMAT_CTIME )
	 && ( date_format != LIBEWF_DATE_FORMAT_DAYMONTH )
	 && ( date_format != LIBEWF_DATE_FORMAT_MONTHDAY )
	 && ( date_format != LIBEWF_DATE_FORMAT_ISO8601 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported date format.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	internal_handle->date_format = date_format;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Retrieves the number of header values
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_number_of_header_values(
     libewf_handle_t *handle,
     uint32_t *number_of_values,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_number_of_header_values";
	int number_of_header_values               = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( number_of_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of values.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->header_values != NULL )
	{
		if( libfvalue_table_get_number_of_values(
		     internal_handle->header_values,
		     &number_of_header_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of header values.",
			 function );

			goto on_error;
		}
	}
	if( number_of_header_values < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of header values value out of bounds.",
		 function );

		goto on_error;
	}
	*number_of_values = (uint32_t) number_of_header_values;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the size of the value identifier of a specific index
 * The identifier size includes the end of string character
 * Returns 1 if successful, 0 if no header values are present or -1 on error
 */
int libewf_handle_get_header_value_identifier_size(
     libewf_handle_t *handle,
     uint32_t index,
     size_t *identifier_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_header_value_identifier_size";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->header_values != NULL )
	{
		result = libewf_header_values_get_identifier_size(
		          internal_handle->header_values,
		          index,
		          identifier_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: %" PRIu32 " identifier size.",
			 function,
			 index );
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the header value identifier of a specific index
 * The identifier size should include the end of string character
 * Returns 1 if successful, 0 if no header values are present or -1 on error
 */
int libewf_handle_get_header_value_identifier(
     libewf_handle_t *handle,
     uint32_t index,
     uint8_t *identifier,
     size_t identifier_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_header_value_identifier";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->header_values != NULL )
	{
		result = libewf_header_values_get_identifier(
		          internal_handle->header_values,
		          index,
		          identifier,
		          identifier_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: %" PRIu32 " identifier.",
			 function,
			 index );
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-8 encoded header value of an identifier
 * The string size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_handle_get_utf8_header_value_size(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_utf8_header_value_size";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->header_values != NULL )
	{
		result = libewf_header_values_get_utf8_value_size(
		          internal_handle->header_values,
		          identifier,
		          identifier_length,
		          internal_handle->date_format,
		          utf8_string_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value size.",
			 function );
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-8 encoded header value of an identifier
 * The string size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_handle_get_utf8_header_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_utf8_header_value";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->header_values != NULL )
	{
		result = libewf_header_values_get_utf8_value(
		          internal_handle->header_values,
		          identifier,
		          identifier_length,
		          internal_handle->date_format,
		          utf8_string,
		          utf8_string_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value.",
			 function );
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Sets the UTF-8 encoded header value specified by the identifier
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_utf8_header_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_utf8_header_value";
	int result                                = 1;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_handle->read_io_handle != NULL )
	 || ( internal_handle->write_io_handle == NULL )
	 || ( internal_handle->write_io_handle->values_initialized != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: header value cannot be changed.",
		 function );

		result = -1;
	}
	else if( internal_handle->header_values == NULL )
	{
		result = libewf_header_values_initialize(
		          &( internal_handle->header_values ),
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create header values.",
			 function );
		}
	}
	if( result == 1 )
	{
		result = libewf_header_values_set_utf8_value(
			  internal_handle->header_values,
			  identifier,
			  identifier_length,
			  utf8_string,
			  utf8_string_length,
			  error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set header value.",
			 function );
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-16 encoded header value of an identifier
 * The string size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_handle_get_utf16_header_value_size(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_utf8_header_value_size";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->header_values != NULL )
	{
		result = libewf_header_values_get_utf16_value_size(
		          internal_handle->header_values,
		          identifier,
		          identifier_length,
		          internal_handle->date_format,
		          utf16_string_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value size.",
			 function );
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-16 encoded header value of an identifier
 * The string size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_handle_get_utf16_header_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_utf16_header_value";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->header_values != NULL )
	{
		result = libewf_header_values_get_utf16_value(
		          internal_handle->header_values,
		          identifier,
		          identifier_length,
		          internal_handle->date_format,
		          utf16_string,
		          utf16_string_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value.",
			 function );
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Sets the UTF-16 encoded header value specified by the identifier
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_utf16_header_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_utf16_header_value";
	int result                                = 1;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_handle->read_io_handle != NULL )
	 || ( internal_handle->write_io_handle == NULL )
	 || ( internal_handle->write_io_handle->values_initialized != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: header value cannot be changed.",
		 function );

		result = -1;
	}
	else if( internal_handle->header_values == NULL )
	{
		result = libewf_header_values_initialize(
		          &( internal_handle->header_values ),
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create header values.",
			 function );
		}
	}
	if( result == 1 )
	{
		result = libewf_header_values_set_utf16_value(
		          internal_handle->header_values,
		          identifier,
		          identifier_length,
		          utf16_string,
		          utf16_string_length,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set header value.",
			 function );
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Copies the header values from the source to the destination handle
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_copy_header_values(
     libewf_handle_t *destination_handle,
     libewf_handle_t *source_handle,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_destination_handle = NULL;
	libewf_internal_handle_t *internal_source_handle      = NULL;
	static char *function                                 = "libewf_handle_copy_header_values";
	int result                                            = 1;

	if( destination_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination handle.",
		 function );

		return( -1 );
	}
	if( source_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source handle.",
		 function );

		return( -1 );
	}
	internal_destination_handle = (libewf_internal_handle_t *) destination_handle;
	internal_source_handle      = (libewf_internal_handle_t *) source_handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_source_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_source_handle->header_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid source handle - missing header values.",
		 function );

		result = -1;
	}
	else
	{
/* TODO add destination handle write lock */
		if( internal_destination_handle->header_values == NULL )
		{
			if( libewf_header_values_initialize(
			     &( internal_destination_handle->header_values ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create header values.",
				 function );

				result = -1;
			}
		}
		if( result == 1 )
		{
			result = libewf_header_values_copy(
			          internal_destination_handle->header_values,
			          internal_source_handle->header_values,
			          error );

			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy header values.",
				 function );
			}
			else
			{
				internal_destination_handle->header_values_parsed = 1;
			}
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_source_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Parses the hash values from the hash, digest and/or xhash section
 * Returns 1 if successful or -1 on error
 */
int libewf_internal_handle_parse_hash_values(
     libewf_internal_handle_t *internal_handle,
     libcerror_error_t **error )
{
	static char *function = "libewf_internal_handle_parse_hash_values";
	int result            = 1;

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->hash_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing hash sections.",
		 function );

		return( -1 );
	}
	if( internal_handle->hash_values != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - hash sections already set.",
		 function );

		return( -1 );
	}
	if( libewf_hash_values_initialize(
	     &( internal_handle->hash_values ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create hash values.",
		 function );

		return( -1 );
	}
	if( ( internal_handle->hash_sections->md5_hash_set != 0 )
	 && ( libewf_hash_values_parse_md5_hash(
	       internal_handle->hash_values,
	       internal_handle->hash_sections->md5_hash,
	       16,
	       error ) != 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to parse MD5 hash for its value.",
		 function );

		result = -1;
	}
	else if( ( internal_handle->hash_sections->md5_digest_set != 0 )
	      && ( libewf_hash_values_parse_md5_hash(
	            internal_handle->hash_values,
	            internal_handle->hash_sections->md5_digest,
	            16,
	            error ) != 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to parse MD5 digest for its value.",
		 function );

		result = -1;
	}
	if( ( internal_handle->hash_sections->sha1_hash_set != 0 )
	 && ( libewf_hash_values_parse_sha1_hash(
	       internal_handle->hash_values,
	       internal_handle->hash_sections->sha1_hash,
	       20,
	       error ) != 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to parse SHA1 hash for its value.",
		 function );

		result = -1;
	}
	else if( ( internal_handle->hash_sections->sha1_digest_set != 0 )
	      && ( libewf_hash_values_parse_sha1_hash(
	            internal_handle->hash_values,
	            internal_handle->hash_sections->sha1_digest,
	            20,
	            error ) != 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to parse SHA1 digest for its value.",
		 function );

		result = -1;
	}
	if( ( internal_handle->hash_sections->xhash != NULL )
	 && ( libewf_hash_values_parse_xhash(
	       internal_handle->hash_values,
	       internal_handle->hash_sections->xhash,
	       internal_handle->hash_sections->xhash_size,
	       error ) != 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to parse xhash for values.",
		 function );

		result = -1;
	}
	return( result );
}

/* Retrieves the number of hash values
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_number_of_hash_values(
     libewf_handle_t *handle,
     uint32_t *number_of_values,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_number_of_hash_values";
	int number_of_hash_values                 = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( number_of_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of values.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->hash_values_parsed == 0 )
	{
		if( libewf_internal_handle_parse_hash_values(
		     internal_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to parse hash values.",
			 function );

			goto on_error;
		}
		internal_handle->hash_values_parsed = 1;
	}
	if( internal_handle->hash_values != NULL )
	{
		if( libfvalue_table_get_number_of_values(
		     internal_handle->hash_values,
		     &number_of_hash_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of hash values.",
			 function );

			goto on_error;
		}
	}
	if( number_of_hash_values < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of hash values value out of bounds.",
		 function );

		goto on_error;
	}
	*number_of_values = (uint32_t) number_of_hash_values;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the size of the hash value identifier of a specific index
 * The identifier size includes the end of string character
 * Returns 1 if successful, 0 if if no hash values are present or -1 on error
 */
int libewf_handle_get_hash_value_identifier_size(
     libewf_handle_t *handle,
     uint32_t index,
     size_t *identifier_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_hash_value_identifier_size";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

/* TODO add write lock */
	if( internal_handle->hash_values_parsed == 0 )
	{
		if( libewf_internal_handle_parse_hash_values(
		     internal_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to parse hash values.",
			 function );

			return( -1 );
		}
		internal_handle->hash_values_parsed = 1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->hash_values != NULL )
	{
		result = libewf_hash_values_get_identifier_size(
		          internal_handle->hash_values,
		          index,
		          identifier_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve hash value: %" PRIu32 " identifier size.",
			 function,
			 index );
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the hash value identifier of a specific index
 * The identifier size should include the end of string character
 * Returns 1 if successful, 0 if no hash values are present or -1 on error
 */
int libewf_handle_get_hash_value_identifier(
     libewf_handle_t *handle,
     uint32_t index,
     uint8_t *identifier,
     size_t identifier_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_hash_value_identifier";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

/* TODO add write lock */
	if( internal_handle->hash_values_parsed == 0 )
	{
		if( libewf_internal_handle_parse_hash_values(
		     internal_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to parse hash values.",
			 function );

			return( -1 );
		}
		internal_handle->hash_values_parsed = 1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->hash_values != NULL )
	{
		result = libewf_hash_values_get_identifier(
		          internal_handle->hash_values,
		          index,
		          identifier,
		          identifier_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve hash value: %" PRIu32 " identifier.",
			 function,
			 index );
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-8 encoded hash value of an identifier
 * The string size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_handle_get_utf8_hash_value_size(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_utf8_hash_value_size";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

/* TODO add write lock */
	if( internal_handle->hash_values_parsed == 0 )
	{
		if( libewf_internal_handle_parse_hash_values(
		     internal_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to parse hash values.",
			 function );

			return( -1 );
		}
		internal_handle->hash_values_parsed = 1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->hash_values != NULL )
	{
		result = libewf_hash_values_get_utf8_value_size(
		          internal_handle->hash_values,
		          identifier,
		          identifier_length,
		          utf8_string_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve hash value size.",
			 function );
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-8 encoded hash value of an identifier
 * The string size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_handle_get_utf8_hash_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_utf8_hash_value";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

/* TODO add write lock */
	if( internal_handle->hash_values_parsed == 0 )
	{
		if( libewf_internal_handle_parse_hash_values(
		     internal_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to parse hash values.",
			 function );

			return( -1 );
		}
		internal_handle->hash_values_parsed = 1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->hash_values != NULL )
	{
		result = libewf_hash_values_get_utf8_value(
		          internal_handle->hash_values,
		          identifier,
		          identifier_length,
		          utf8_string,
		          utf8_string_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve hash value.",
			 function );
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Sets the UTF-8 encoded hash value specified by the identifier
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_utf8_hash_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_utf8_hash_value";
	int result                                = 1;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( ( internal_handle->io_handle->access_flags & LIBEWF_ACCESS_FLAG_READ ) != 0 )
	 && ( ( internal_handle->io_handle->access_flags & LIBEWF_ACCESS_FLAG_RESUME ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: hash value cannot be changed.",
		 function );

		result = -1;
	}
	else if( internal_handle->hash_values == NULL )
	{
		if( libewf_hash_values_initialize(
		     &( internal_handle->hash_values ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create hash values.",
			 function );

			result = -1;
		}
		else
		{
			internal_handle->hash_values_parsed = 1;
		}
	}
	if( result == 1 )
	{
		result = libewf_hash_values_set_utf8_value(
		          internal_handle->hash_values,
		          identifier,
		          identifier_length,
		          utf8_string,
		          utf8_string_length,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set hash value.",
			 function );
		}
		else if( internal_handle->hash_sections != NULL )
		{
			result = libewf_hash_sections_set_digest_from_hash_values(
				  internal_handle->hash_sections,
				  identifier,
				  identifier_length,
				  internal_handle->hash_values,
				  error );

			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set digest in hash sections.",
				 function );
			}
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-16 encoded hash value of an identifier
 * The string size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_handle_get_utf16_hash_value_size(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_utf16_hash_value_size";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

/* TODO add write lock */
	if( internal_handle->hash_values_parsed == 0 )
	{
		if( libewf_internal_handle_parse_hash_values(
		     internal_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to parse hash values.",
			 function );

			return( -1 );
		}
		internal_handle->hash_values_parsed = 1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->hash_values != NULL )
	{
		result = libewf_hash_values_get_utf16_value_size(
		          internal_handle->hash_values,
		          identifier,
		          identifier_length,
		          utf16_string_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve hash value size.",
			 function );
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-16 encoded hash value of an identifier
 * The string size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_handle_get_utf16_hash_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_utf16_hash_value";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

/* TODO add write lock */
	if( internal_handle->hash_values_parsed == 0 )
	{
		if( libewf_internal_handle_parse_hash_values(
		     internal_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to parse hash values.",
			 function );

			return( -1 );
		}
		internal_handle->hash_values_parsed = 1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->hash_values != NULL )
	{
		result = libewf_hash_values_get_utf16_value(
		          internal_handle->hash_values,
		          identifier,
		          identifier_length,
		          utf16_string,
		          utf16_string_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve hash value.",
			 function );
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Sets the UTF-16 encoded hash value specified by the identifier
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_utf16_hash_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_utf16_hash_value";
	int result                                = 1;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( ( internal_handle->io_handle->access_flags & LIBEWF_ACCESS_FLAG_READ ) != 0 )
	 && ( ( internal_handle->io_handle->access_flags & LIBEWF_ACCESS_FLAG_RESUME ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: hash value cannot be changed.",
		 function );

		result = -1;
	}
	else if( internal_handle->hash_values == NULL )
	{
		if( libewf_hash_values_initialize(
		     &( internal_handle->hash_values ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create hash values.",
			 function );

			result = -1;
		}
		else
		{
			internal_handle->hash_values_parsed = 1;
		}
	}
	if( result == 1 )
	{
		result = libewf_hash_values_set_utf16_value(
		          internal_handle->hash_values,
		          identifier,
		          identifier_length,
		          utf16_string,
		          utf16_string_length,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set hash value.",
			 function );
		}
		else if( internal_handle->hash_sections != NULL )
		{
			result = libewf_hash_sections_set_digest_from_hash_values(
				  internal_handle->hash_sections,
				  identifier,
				  identifier_length,
				  internal_handle->hash_values,
				  error );

			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set digest in hash sections.",
				 function );
			}
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

