/*
 * Media values functions
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

#include "libewf_definitions.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_media_values.h"

/* Creates media values
 * Make sure the value media_values is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_media_values_initialize(
     libewf_media_values_t **media_values,
     libcerror_error_t **error )
{
	static char *function = "libewf_media_values_initialize";

	if( media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	if( *media_values != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid media values value already set.",
		 function );

		return( -1 );
	}
	*media_values = memory_allocate_structure(
	                 libewf_media_values_t );

	if( *media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create media values.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *media_values,
	     0,
	     sizeof( libewf_media_values_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear media values.",
		 function );

		goto on_error;
	}
	( *media_values )->chunk_size        = LIBEWF_MINIMUM_CHUNK_SIZE;
	( *media_values )->sectors_per_chunk = 64;
	( *media_values )->bytes_per_sector  = 512;
	( *media_values )->media_flags       = 0x01;

	return( 1 );

on_error:
	if( *media_values != NULL )
	{
		memory_free(
		 *media_values );

		*media_values = NULL;
	}
	return( -1 );
}

/* Frees media values
 * Returns 1 if successful or -1 on error
 */
int libewf_media_values_free(
     libewf_media_values_t **media_values,
     libcerror_error_t **error )
{
        static char *function = "libewf_media_values_free";

	if( media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	if( *media_values != NULL )
	{
		memory_free(
		 *media_values );

		*media_values = NULL;
	}
	return( 1 );
}

/* Clones the media values
 * Returns 1 if successful or -1 on error
 */
int libewf_media_values_clear(
     libewf_media_values_t *media_values,
     libcerror_error_t **error )
{
        static char *function = "libewf_media_values_clear";

	if( media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     media_values,
	     0,
	     sizeof( libewf_media_values_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear media values.",
		 function );

		return( -1 );
	}
	media_values->chunk_size        = LIBEWF_MINIMUM_CHUNK_SIZE;
	media_values->sectors_per_chunk = 64;
	media_values->bytes_per_sector  = 512;
	media_values->media_flags       = 0x01;

	return( 1 );
}

/* Clones the media values
 * Returns 1 if successful or -1 on error
 */
int libewf_media_values_clone(
     libewf_media_values_t **destination_media_values,
     libewf_media_values_t *source_media_values,
     libcerror_error_t **error )
{
	static char *function = "libewf_media_values_clone";

	if( destination_media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination media values.",
		 function );

		return( -1 );
	}
	if( *destination_media_values != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination media values already set.",
		 function );

		return( -1 );
	}
	if( source_media_values == NULL )
	{
		*destination_media_values = NULL;

		return( 1 );
	}
	*destination_media_values = memory_allocate_structure(
		                     libewf_media_values_t );

	if( *destination_media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination media values.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_media_values,
	     source_media_values,
	     sizeof( libewf_media_values_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination media values.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_media_values != NULL )
	{
		memory_free(
		 *destination_media_values );

		*destination_media_values = NULL;
	}
	return( -1 );
}

/* Calculate the chunk size
 * Returns 1 if successful or -1 on error
 */
int libewf_media_values_calculate_chunk_size(
     libewf_media_values_t *media_values,
     libcerror_error_t **error )
{
        static char *function    = "libewf_media_values_calculate_chunk_size";
	size64_t bytes_per_chunk = 0;

	if( media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}	
	if( ( media_values->sectors_per_chunk == 0 )
	 || ( media_values->sectors_per_chunk > (uint32_t) INT32_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid sectors per chunk value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( media_values->bytes_per_sector == 0 )
	 || ( media_values->bytes_per_sector > (uint32_t) INT32_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid bytes per sector value out of bounds.",
		 function );

		return( -1 );
	}
	bytes_per_chunk = (size64_t) media_values->sectors_per_chunk * media_values->bytes_per_sector;

	if( bytes_per_chunk > (size64_t) INT32_MAX )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: chunk size value exceeds maximum defaulting to: %d.\n",
			 function,
			 LIBEWF_MINIMUM_CHUNK_SIZE );
		}
#endif
		bytes_per_chunk = (size64_t) LIBEWF_MINIMUM_CHUNK_SIZE;
	}
	media_values->chunk_size = (uint32_t) bytes_per_chunk;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: sectors per chunk\t\t: %" PRIu32 "\n",
		 function,
		 media_values->sectors_per_chunk );

		libcnotify_printf(
		 "%s: bytes per sector\t\t: %" PRIu32 "\n",
		 function,
		 media_values->bytes_per_sector );

		libcnotify_printf(
		 "%s: chunk size\t\t\t: %" PRIu32 "\n",
		 function,
		 media_values->chunk_size );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

