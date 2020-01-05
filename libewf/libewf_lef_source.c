/*
 * Logical Evidence File (LEF) source functions
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

#include "libewf_lef_source.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libfvalue.h"
#include "libewf_serialized_string.h"

/* Creates a source
 * Make sure the value lef_source is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_initialize(
     libewf_lef_source_t **lef_source,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_initialize";

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	if( *lef_source != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid source value already set.",
		 function );

		return( -1 );
	}
	*lef_source = memory_allocate_structure(
	               libewf_lef_source_t );

	if( *lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create source.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *lef_source,
	     0,
	     sizeof( libewf_lef_source_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear source.",
		 function );

		memory_free(
		 *lef_source );

		*lef_source = NULL;

		return( -1 );
	}
	if( libewf_serialized_string_initialize(
	     &( ( *lef_source )->name ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create name string.",
		 function );

		goto on_error;
	}
	if( libewf_serialized_string_initialize(
	     &( ( *lef_source )->evidence_number ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create evidence number string.",
		 function );

		goto on_error;
	}
	if( libewf_serialized_string_initialize(
	     &( ( *lef_source )->location ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create location string.",
		 function );

		goto on_error;
	}
	if( libewf_serialized_string_initialize(
	     &( ( *lef_source )->device_guid ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create device GUID string.",
		 function );

		goto on_error;
	}
	if( libewf_serialized_string_initialize(
	     &( ( *lef_source )->primary_device_guid ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create primary device GUID string.",
		 function );

		goto on_error;
	}
	if( libewf_serialized_string_initialize(
	     &( ( *lef_source )->manufacturer ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create manufacturer string.",
		 function );

		goto on_error;
	}
	if( libewf_serialized_string_initialize(
	     &( ( *lef_source )->model ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create model string.",
		 function );

		goto on_error;
	}
	if( libewf_serialized_string_initialize(
	     &( ( *lef_source )->serial_number ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create serial number string.",
		 function );

		goto on_error;
	}
	if( libewf_serialized_string_initialize(
	     &( ( *lef_source )->domain ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create domain string.",
		 function );

		goto on_error;
	}
	if( libewf_serialized_string_initialize(
	     &( ( *lef_source )->ip_address ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create IP address string.",
		 function );

		goto on_error;
	}
	if( libewf_serialized_string_initialize(
	     &( ( *lef_source )->mac_address ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create MAC address string.",
		 function );

		goto on_error;
	}
	if( libewf_serialized_string_initialize(
	     &( ( *lef_source )->md5_hash ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create MD5 hash string.",
		 function );

		goto on_error;
	}
	if( libewf_serialized_string_initialize(
	     &( ( *lef_source )->sha1_hash ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create SHA1 hash string.",
		 function );

		goto on_error;
	}
	( *lef_source )->logical_offset  = -1;
	( *lef_source )->physical_offset = -1;

	return( 1 );

on_error:
	if( *lef_source != NULL )
	{
		if( ( *lef_source )->md5_hash != NULL )
		{
			libewf_serialized_string_free(
			 &( ( *lef_source )->md5_hash ),
			 NULL );
		}
		if( ( *lef_source )->mac_address != NULL )
		{
			libewf_serialized_string_free(
			 &( ( *lef_source )->mac_address ),
			 NULL );
		}
		if( ( *lef_source )->ip_address != NULL )
		{
			libewf_serialized_string_free(
			 &( ( *lef_source )->ip_address ),
			 NULL );
		}
		if( ( *lef_source )->domain != NULL )
		{
			libewf_serialized_string_free(
			 &( ( *lef_source )->domain ),
			 NULL );
		}
		if( ( *lef_source )->serial_number != NULL )
		{
			libewf_serialized_string_free(
			 &( ( *lef_source )->serial_number ),
			 NULL );
		}
		if( ( *lef_source )->model != NULL )
		{
			libewf_serialized_string_free(
			 &( ( *lef_source )->model ),
			 NULL );
		}
		if( ( *lef_source )->manufacturer != NULL )
		{
			libewf_serialized_string_free(
			 &( ( *lef_source )->manufacturer ),
			 NULL );
		}
		if( ( *lef_source )->primary_device_guid != NULL )
		{
			libewf_serialized_string_free(
			 &( ( *lef_source )->primary_device_guid ),
			 NULL );
		}
		if( ( *lef_source )->device_guid != NULL )
		{
			libewf_serialized_string_free(
			 &( ( *lef_source )->device_guid ),
			 NULL );
		}
		if( ( *lef_source )->location != NULL )
		{
			libewf_serialized_string_free(
			 &( ( *lef_source )->location ),
			 NULL );
		}
		if( ( *lef_source )->evidence_number != NULL )
		{
			libewf_serialized_string_free(
			 &( ( *lef_source )->evidence_number ),
			 NULL );
		}
		if( ( *lef_source )->name != NULL )
		{
			libewf_serialized_string_free(
			 &( ( *lef_source )->name ),
			 NULL );
		}
		memory_free(
		 *lef_source );

		*lef_source = NULL;
	}
	return( -1 );
}

/* Frees a source
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_free(
     libewf_lef_source_t **lef_source,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_free";
	int result            = 1;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	if( *lef_source != NULL )
	{
		if( ( *lef_source )->name != NULL )
		{
			if( libewf_serialized_string_free(
			     &( ( *lef_source )->name ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free name string.",
				 function );

				result = -1;
			}
		}
		if( ( *lef_source )->evidence_number != NULL )
		{
			if( libewf_serialized_string_free(
			     &( ( *lef_source )->evidence_number ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free evidence number string.",
				 function );

				result = -1;
			}
		}
		if( ( *lef_source )->location != NULL )
		{
			if( libewf_serialized_string_free(
			     &( ( *lef_source )->location ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free location string.",
				 function );

				result = -1;
			}
		}
		if( ( *lef_source )->device_guid != NULL )
		{
			if( libewf_serialized_string_free(
			     &( ( *lef_source )->device_guid ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free device GUID string.",
				 function );

				result = -1;
			}
		}
		if( ( *lef_source )->primary_device_guid != NULL )
		{
			if( libewf_serialized_string_free(
			     &( ( *lef_source )->primary_device_guid ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free primary device GUID string.",
				 function );

				result = -1;
			}
		}
		if( ( *lef_source )->manufacturer != NULL )
		{
			if( libewf_serialized_string_free(
			     &( ( *lef_source )->manufacturer ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free manufacturer string.",
				 function );

				result = -1;
			}
		}
		if( ( *lef_source )->model != NULL )
		{
			if( libewf_serialized_string_free(
			     &( ( *lef_source )->model ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free model string.",
				 function );

				result = -1;
			}
		}
		if( ( *lef_source )->serial_number != NULL )
		{
			if( libewf_serialized_string_free(
			     &( ( *lef_source )->serial_number ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free serial number string.",
				 function );

				result = -1;
			}
		}
		if( ( *lef_source )->domain != NULL )
		{
			if( libewf_serialized_string_free(
			     &( ( *lef_source )->domain ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free domain string.",
				 function );

				result = -1;
			}
		}
		if( ( *lef_source )->ip_address != NULL )
		{
			if( libewf_serialized_string_free(
			     &( ( *lef_source )->ip_address ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free IP address string.",
				 function );

				result = -1;
			}
		}
		if( ( *lef_source )->mac_address != NULL )
		{
			if( libewf_serialized_string_free(
			     &( ( *lef_source )->mac_address ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free MAC address string.",
				 function );

				result = -1;
			}
		}
		if( ( *lef_source )->md5_hash != NULL )
		{
			if( libewf_serialized_string_free(
			     &( ( *lef_source )->md5_hash ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free MD5 hash string.",
				 function );

				result = -1;
			}
		}
		if( ( *lef_source )->sha1_hash != NULL )
		{
			if( libewf_serialized_string_free(
			     &( ( *lef_source )->sha1_hash ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free SHA1 hash string.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *lef_source );

		*lef_source = NULL;
	}
	return( result );
}

/* Clones the source
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_clone(
     libewf_lef_source_t **destination_lef_source,
     libewf_lef_source_t *source_lef_source,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_clone";

	if( destination_lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination source.",
		 function );

		return( -1 );
	}
	if( *destination_lef_source != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination source value already set.",
		 function );

		return( -1 );
	}
	if( source_lef_source == NULL )
	{
		*destination_lef_source = NULL;

		return( 1 );
	}
	*destination_lef_source = memory_allocate_structure(
	                           libewf_lef_source_t );

	if( *destination_lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination source.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_lef_source,
	     source_lef_source,
	     sizeof( libewf_lef_source_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination source.",
		 function );

		memory_free(
		 *destination_lef_source );

		*destination_lef_source = NULL;

		return( -1 );
	}
	( *destination_lef_source )->name                = NULL;
	( *destination_lef_source )->evidence_number     = NULL;
	( *destination_lef_source )->location            = NULL;
	( *destination_lef_source )->device_guid         = NULL;
	( *destination_lef_source )->primary_device_guid = NULL;
	( *destination_lef_source )->manufacturer        = NULL;
	( *destination_lef_source )->model               = NULL;
	( *destination_lef_source )->serial_number       = NULL;
	( *destination_lef_source )->domain              = NULL;
	( *destination_lef_source )->ip_address          = NULL;
	( *destination_lef_source )->mac_address         = NULL;
	( *destination_lef_source )->md5_hash            = NULL;
	( *destination_lef_source )->sha1_hash           = NULL;

	if( libewf_serialized_string_clone(
	     &( ( *destination_lef_source )->name ),
	     source_lef_source->name,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to clone destination name string.",
		 function );

		goto on_error;
	}
	if( libewf_serialized_string_clone(
	     &( ( *destination_lef_source )->evidence_number ),
	     source_lef_source->evidence_number,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to clone destination evidence number string.",
		 function );

		goto on_error;
	}
	if( libewf_serialized_string_clone(
	     &( ( *destination_lef_source )->location ),
	     source_lef_source->location,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to clone destination location string.",
		 function );

		goto on_error;
	}
	if( libewf_serialized_string_clone(
	     &( ( *destination_lef_source )->device_guid ),
	     source_lef_source->device_guid,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to clone destination device GUID string.",
		 function );

		goto on_error;
	}
	if( libewf_serialized_string_clone(
	     &( ( *destination_lef_source )->primary_device_guid ),
	     source_lef_source->primary_device_guid,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to clone destination primary device GUID string.",
		 function );

		goto on_error;
	}
	if( libewf_serialized_string_clone(
	     &( ( *destination_lef_source )->manufacturer ),
	     source_lef_source->manufacturer,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to clone destination manufacturer string.",
		 function );

		goto on_error;
	}
	if( libewf_serialized_string_clone(
	     &( ( *destination_lef_source )->model ),
	     source_lef_source->model,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to clone destination model string.",
		 function );

		goto on_error;
	}
	if( libewf_serialized_string_clone(
	     &( ( *destination_lef_source )->serial_number ),
	     source_lef_source->serial_number,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to clone destination serial number string.",
		 function );

		goto on_error;
	}
	if( libewf_serialized_string_clone(
	     &( ( *destination_lef_source )->domain ),
	     source_lef_source->domain,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to clone destination domain string.",
		 function );

		goto on_error;
	}
	if( libewf_serialized_string_clone(
	     &( ( *destination_lef_source )->ip_address ),
	     source_lef_source->ip_address,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to clone destination IP address string.",
		 function );

		goto on_error;
	}
	if( libewf_serialized_string_clone(
	     &( ( *destination_lef_source )->mac_address ),
	     source_lef_source->mac_address,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to clone destination MAC address string.",
		 function );

		goto on_error;
	}
	if( libewf_serialized_string_clone(
	     &( ( *destination_lef_source )->md5_hash ),
	     source_lef_source->md5_hash,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to clone destination MD5 hash string.",
		 function );

		goto on_error;
	}
	if( libewf_serialized_string_clone(
	     &( ( *destination_lef_source )->sha1_hash ),
	     source_lef_source->sha1_hash,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to clone destination SHA1 hash string.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_lef_source != NULL )
	{
		libewf_lef_source_free(
		 destination_lef_source,
		 NULL );
	}
	return( -1 );
}

/* Reads a source
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_read_data(
     libewf_lef_source_t *lef_source,
     libfvalue_split_utf8_string_t *types,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfvalue_split_utf8_string_t *values = NULL;
	uint8_t *type_string                  = NULL;
	uint8_t *value_string                 = NULL;
	static char *function                 = "libewf_lef_source_read_data";
	size_t type_string_size               = 0;
	size_t value_string_size              = 0;
	uint64_t value_64bit                  = 0;
	int number_of_types                   = 0;
	int number_of_values                  = 0;
	int value_index                       = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	if( libfvalue_split_utf8_string_get_number_of_segments(
	     types,
	     &number_of_types,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of types",
		 function );

		goto on_error;
	}
	if( libfvalue_utf8_string_split(
	     data,
	     data_size,
	     (uint8_t) '\t',
	     &values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split data into string values.",
		 function );

		goto on_error;
	}
	if( libfvalue_split_utf8_string_get_number_of_segments(
	     values,
	     &number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of values",
		 function );

		goto on_error;
	}
	if( number_of_types != number_of_values )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: mismatch in number of types and values.",
		 function );

		goto on_error;
	}
	for( value_index = 0;
	     value_index < number_of_types;
	     value_index++ )
	{
		if( libfvalue_split_utf8_string_get_segment_by_index(
		     types,
		     value_index,
		     &type_string,
		     &type_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve type string: %d.",
			 function,
			 value_index );

			goto on_error;
		}
		if( ( type_string == NULL )
		 || ( type_string_size < 2 )
		 || ( type_string[ 0 ] == 0 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing type string: %d.",
			 function,
			 value_index );

			goto on_error;
		}
		if( value_index >= number_of_values )
		{
			value_string      = NULL;
			value_string_size = 0;
		}
		else
		{
			if( libfvalue_split_utf8_string_get_segment_by_index(
			     values,
			     value_index,
			     &value_string,
			     &value_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value string: %d.",
				 function,
				 value_index );

				goto on_error;
			}
			if( ( value_string == NULL )
			 || ( value_string_size < 2 )
			 || ( value_string[ 0 ] == 0 ) )
			{
				value_string      = NULL;
				value_string_size = 0;
			}
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: type: %s with value: %s\n",
			 function,
			 (char *) type_string,
			 (char *) value_string );
		}
#endif
		if( ( value_string == NULL )
		 || ( value_string_size == 0 ) )
		{
			/* Ignore empty values
			 */
		}
		else if( type_string_size == 4 )
		{
			if( ( type_string[ 0 ] == (uint8_t) 'l' )
			 && ( type_string[ 1 ] == (uint8_t) 'o' )
			 && ( type_string[ 2 ] == (uint8_t) 'c' ) )
			{
				if( libewf_serialized_string_read_data(
				     lef_source->location,
				     value_string,
				     value_string_size - 1,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read location string.",
					 function );

					goto on_error;
				}
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'm' )
			      && ( type_string[ 1 ] == (uint8_t) 'f' )
			      && ( type_string[ 2 ] == (uint8_t) 'r' ) )
			{
				if( libewf_serialized_string_read_data(
				     lef_source->manufacturer,
				     value_string,
				     value_string_size - 1,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read manufacturer string.",
					 function );

					goto on_error;
				}
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'p' )
			      && ( type_string[ 1 ] == (uint8_t) 'g' )
			      && ( type_string[ 2 ] == (uint8_t) 'u' ) )
			{
				if( libewf_serialized_string_read_hexadecimal_data(
				     lef_source->primary_device_guid,
				     value_string,
				     value_string_size - 1,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read primary device GUID string.",
					 function );

					goto on_error;
				}
			}
		}
		else if( type_string_size == 3 )
		{
			if( ( type_string[ 0 ] == (uint8_t) 'a' )
			 && ( type_string[ 1 ] == (uint8_t) 'h' ) )
			{
				if( libewf_serialized_string_read_hexadecimal_data(
				     lef_source->md5_hash,
				     value_string,
				     value_string_size - 1,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read MD5 hash.",
					 function );

					goto on_error;
				}
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'a' )
			      && ( type_string[ 1 ] == (uint8_t) 'q' ) )
			{
				if( libfvalue_utf8_string_copy_to_integer(
				     value_string,
				     value_string_size,
				     &value_64bit,
				     64,
				     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_SIGNED,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set acquisition time.",
					 function );

					goto on_error;
				}
				lef_source->acquisition_time = (int64_t) value_64bit;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'd' )
			      && ( type_string[ 1 ] == (uint8_t) 'o' ) )
			{
				if( libewf_serialized_string_read_data(
				     lef_source->domain,
				     value_string,
				     value_string_size - 1,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read domain string.",
					 function );

					goto on_error;
				}
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'd' )
			      && ( type_string[ 1 ] == (uint8_t) 't' ) )
			{
				if( value_string_size != 2 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read drive type string.",
					 function );

					goto on_error;
				}
				lef_source->drive_type = value_string[ 0 ];
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'e' )
			      && ( type_string[ 1 ] == (uint8_t) 'v' ) )
			{
				if( libewf_serialized_string_read_data(
				     lef_source->evidence_number,
				     value_string,
				     value_string_size - 1,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read evidence number string.",
					 function );

					goto on_error;
				}
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'i' )
			      && ( type_string[ 1 ] == (uint8_t) 'd' ) )
			{
				if( libfvalue_utf8_string_copy_to_integer(
				     value_string,
				     value_string_size,
				     &value_64bit,
				     64,
				     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set identifier.",
					 function );

					goto on_error;
				}
				if( ( (int64_t) value_64bit < (int64_t) 0 )
				 || ( (int64_t) value_64bit > (int64_t) INT_MAX ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid identifier value out of bounds.",
					 function );

					goto on_error;
				}
				lef_source->identifier = (uint32_t) value_64bit;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'i' )
			      && ( type_string[ 1 ] == (uint8_t) 'p' ) )
			{
				if( libewf_serialized_string_read_data(
				     lef_source->ip_address,
				     value_string,
				     value_string_size - 1,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read IP address string.",
					 function );

					goto on_error;
				}
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'g' )
			      && ( type_string[ 1 ] == (uint8_t) 'u' ) )
			{
				if( libewf_serialized_string_read_hexadecimal_data(
				     lef_source->device_guid,
				     value_string,
				     value_string_size - 1,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read device GUID string.",
					 function );

					goto on_error;
				}
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'l' )
			      && ( type_string[ 1 ] == (uint8_t) 'o' ) )
			{
				if( libfvalue_utf8_string_copy_to_integer(
				     value_string,
				     value_string_size,
				     &value_64bit,
				     64,
				     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_SIGNED,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set logical offset.",
					 function );

					goto on_error;
				}
				lef_source->logical_offset = (off64_t) value_64bit;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'm' )
			      && ( type_string[ 1 ] == (uint8_t) 'a' ) )
			{
				if( libewf_serialized_string_read_hexadecimal_data(
				     lef_source->mac_address,
				     value_string,
				     value_string_size - 1,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read MAC address string.",
					 function );

					goto on_error;
				}
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'm' )
			      && ( type_string[ 1 ] == (uint8_t) 'o' ) )
			{
				if( libewf_serialized_string_read_data(
				     lef_source->model,
				     value_string,
				     value_string_size - 1,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read model string.",
					 function );

					goto on_error;
				}
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'p' )
			      && ( type_string[ 1 ] == (uint8_t) 'o' ) )
			{
				if( libfvalue_utf8_string_copy_to_integer(
				     value_string,
				     value_string_size,
				     &value_64bit,
				     64,
				     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_SIGNED,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set physical offset.",
					 function );

					goto on_error;
				}
				lef_source->physical_offset = (off64_t) value_64bit;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 's' )
			      && ( type_string[ 1 ] == (uint8_t) 'e' ) )
			{
				if( libewf_serialized_string_read_data(
				     lef_source->serial_number,
				     value_string,
				     value_string_size - 1,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read serial number string.",
					 function );

					goto on_error;
				}
			}
			else if( ( type_string[ 0 ] == (uint8_t) 's' )
			      && ( type_string[ 1 ] == (uint8_t) 'h' ) )
			{
				if( libewf_serialized_string_read_hexadecimal_data(
				     lef_source->sha1_hash,
				     value_string,
				     value_string_size - 1,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read SHA1 hash.",
					 function );

					goto on_error;
				}
			}
			else if( ( type_string[ 0 ] == (uint8_t) 's' )
			      && ( type_string[ 1 ] == (uint8_t) 'i' ) )
			{
/* TODO implement */
			}
			else if( ( type_string[ 0 ] == (uint8_t) 't' )
			      && ( type_string[ 1 ] == (uint8_t) 'b' ) )
			{
				if( libfvalue_utf8_string_copy_to_integer(
				     value_string,
				     value_string_size,
				     &value_64bit,
				     64,
				     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set size.",
					 function );

					goto on_error;
				}
				lef_source->size = (size64_t) value_64bit;
			}
		}
		else if( type_string_size == 2 )
		{
			if( type_string[ 0 ] == (uint8_t) 'n' )
			{
				if( libewf_serialized_string_read_data(
				     lef_source->name,
				     value_string,
				     value_string_size - 1,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read name string.",
					 function );

					goto on_error;
				}
			}
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	if( libfvalue_split_utf8_string_free(
	     &values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free split values.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( values != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &values,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the identifier
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_identifier(
     libewf_lef_source_t *lef_source,
     int *identifier,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_identifier";

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	*identifier = lef_source->identifier;

	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf8_name_size(
     libewf_lef_source_t *lef_source,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf8_name_size";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf8_string_size(
	          lef_source->name,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve name UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf8_name(
     libewf_lef_source_t *lef_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf8_name";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf8_string(
	          lef_source->name,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy name to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf16_name_size(
     libewf_lef_source_t *lef_source,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf16_name_size";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf16_string_size(
	          lef_source->name,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve name UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf16_name(
     libewf_lef_source_t *lef_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf16_name";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf16_string(
	          lef_source->name,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy name to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded evidence number
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf8_evidence_number_size(
     libewf_lef_source_t *lef_source,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf8_evidence_number_size";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf8_string_size(
	          lef_source->evidence_number,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve evidence number UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded evidence number
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf8_evidence_number(
     libewf_lef_source_t *lef_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf8_evidence_number";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf8_string(
	          lef_source->evidence_number,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy evidence number to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded evidence number
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf16_evidence_number_size(
     libewf_lef_source_t *lef_source,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf16_evidence_number_size";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf16_string_size(
	          lef_source->evidence_number,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve evidence number UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded evidence number
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf16_evidence_number(
     libewf_lef_source_t *lef_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf16_evidence_number";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf16_string(
	          lef_source->evidence_number,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy evidence number to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded location
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf8_location_size(
     libewf_lef_source_t *lef_source,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf8_location_size";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf8_string_size(
	          lef_source->location,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve location UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded location
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf8_location(
     libewf_lef_source_t *lef_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf8_location";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf8_string(
	          lef_source->location,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy location to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded location
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf16_location_size(
     libewf_lef_source_t *lef_source,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf16_location_size";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf16_string_size(
	          lef_source->location,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve location UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded location
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf16_location(
     libewf_lef_source_t *lef_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf16_location";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf16_string(
	          lef_source->location,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy location to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded device GUID
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf8_device_guid_size(
     libewf_lef_source_t *lef_source,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf8_device_guid_size";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf8_string_size(
	          lef_source->device_guid,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve device GUID UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded device GUID
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf8_device_guid(
     libewf_lef_source_t *lef_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf8_device_guid";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf8_string(
	          lef_source->device_guid,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy device GUID to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded device GUID
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf16_device_guid_size(
     libewf_lef_source_t *lef_source,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf16_device_guid_size";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf16_string_size(
	          lef_source->device_guid,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve device GUID UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded device GUID
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf16_device_guid(
     libewf_lef_source_t *lef_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf16_device_guid";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf16_string(
	          lef_source->device_guid,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy device GUID to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded primary device GUID
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf8_primary_device_guid_size(
     libewf_lef_source_t *lef_source,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf8_primary_device_guid_size";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf8_string_size(
	          lef_source->primary_device_guid,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve primary device GUID UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded primary device GUID
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf8_primary_device_guid(
     libewf_lef_source_t *lef_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf8_primary_device_guid";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf8_string(
	          lef_source->primary_device_guid,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy primary device GUID to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded primary device GUID
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf16_primary_device_guid_size(
     libewf_lef_source_t *lef_source,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf16_primary_device_guid_size";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf16_string_size(
	          lef_source->primary_device_guid,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve primary device GUID UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded primary device GUID
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf16_primary_device_guid(
     libewf_lef_source_t *lef_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf16_primary_device_guid";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf16_string(
	          lef_source->primary_device_guid,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy primary device GUID to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the drive type
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_drive_type(
     libewf_lef_source_t *lef_source,
     uint8_t *drive_type,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_drive_type";

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	if( drive_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid drive type.",
		 function );

		return( -1 );
	}
	*drive_type = lef_source->drive_type;

	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded manufacturer
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf8_manufacturer_size(
     libewf_lef_source_t *lef_source,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf8_manufacturer_size";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf8_string_size(
	          lef_source->manufacturer,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve manufacturer UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded manufacturer
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf8_manufacturer(
     libewf_lef_source_t *lef_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf8_manufacturer";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf8_string(
	          lef_source->manufacturer,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy manufacturer to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded manufacturer
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf16_manufacturer_size(
     libewf_lef_source_t *lef_source,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf16_manufacturer_size";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf16_string_size(
	          lef_source->manufacturer,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve manufacturer UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded manufacturer
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf16_manufacturer(
     libewf_lef_source_t *lef_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf16_manufacturer";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf16_string(
	          lef_source->manufacturer,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy manufacturer to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded model
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf8_model_size(
     libewf_lef_source_t *lef_source,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf8_model_size";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf8_string_size(
	          lef_source->model,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve model UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded model
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf8_model(
     libewf_lef_source_t *lef_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf8_model";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf8_string(
	          lef_source->model,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy model to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded model
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf16_model_size(
     libewf_lef_source_t *lef_source,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf16_model_size";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf16_string_size(
	          lef_source->model,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve model UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded model
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf16_model(
     libewf_lef_source_t *lef_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf16_model";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf16_string(
	          lef_source->model,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy model to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded serial number
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf8_serial_number_size(
     libewf_lef_source_t *lef_source,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf8_serial_number_size";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf8_string_size(
	          lef_source->serial_number,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve serial number UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded serial number
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf8_serial_number(
     libewf_lef_source_t *lef_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf8_serial_number";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf8_string(
	          lef_source->serial_number,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy serial number to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded serial number
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf16_serial_number_size(
     libewf_lef_source_t *lef_source,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf16_serial_number_size";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf16_string_size(
	          lef_source->serial_number,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve serial number UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded serial number
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf16_serial_number(
     libewf_lef_source_t *lef_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf16_serial_number";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf16_string(
	          lef_source->serial_number,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy serial number to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded domain
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf8_domain_size(
     libewf_lef_source_t *lef_source,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf8_domain_size";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf8_string_size(
	          lef_source->domain,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve domain UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded domain
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf8_domain(
     libewf_lef_source_t *lef_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf8_domain";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf8_string(
	          lef_source->domain,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy domain to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded domain
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf16_domain_size(
     libewf_lef_source_t *lef_source,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf16_domain_size";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf16_string_size(
	          lef_source->domain,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve domain UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded domain
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf16_domain(
     libewf_lef_source_t *lef_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf16_domain";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf16_string(
	          lef_source->domain,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy domain to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded IP address
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf8_ip_address_size(
     libewf_lef_source_t *lef_source,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf8_ip_address_size";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf8_string_size(
	          lef_source->ip_address,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve IP address UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded IP address
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf8_ip_address(
     libewf_lef_source_t *lef_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf8_ip_address";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf8_string(
	          lef_source->ip_address,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy IP address to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded IP address
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf16_ip_address_size(
     libewf_lef_source_t *lef_source,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf16_ip_address_size";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf16_string_size(
	          lef_source->ip_address,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve IP address UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded IP address
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf16_ip_address(
     libewf_lef_source_t *lef_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf16_ip_address";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf16_string(
	          lef_source->ip_address,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy IP address to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded MAC address
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf8_mac_address_size(
     libewf_lef_source_t *lef_source,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf8_mac_address_size";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf8_string_size(
	          lef_source->mac_address,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve MAC address UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded MAC address
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf8_mac_address(
     libewf_lef_source_t *lef_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf8_mac_address";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf8_string(
	          lef_source->mac_address,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy MAC address to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded MAC address
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf16_mac_address_size(
     libewf_lef_source_t *lef_source,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf16_mac_address_size";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf16_string_size(
	          lef_source->mac_address,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve MAC address UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded MAC address
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_utf16_mac_address(
     libewf_lef_source_t *lef_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf16_mac_address";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf16_string(
	          lef_source->mac_address,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy MAC address to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_size(
     libewf_lef_source_t *lef_source,
     size64_t *size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_size";

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	if( size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid size.",
		 function );

		return( -1 );
	}
	*size = lef_source->size;

	return( 1 );
}

/* Retrieves the logical offset
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_lef_source_get_logical_offset(
     libewf_lef_source_t *lef_source,
     off64_t *logical_offset,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_logical_offset";

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	if( logical_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical offset.",
		 function );

		return( -1 );
	}
	if( lef_source->logical_offset == -1 )
	{
		return( 0 );
	}
	*logical_offset = lef_source->logical_offset;

	return( 1 );
}

/* Retrieves the physical offset
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_lef_source_get_physical_offset(
     libewf_lef_source_t *lef_source,
     off64_t *physical_offset,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_physical_offset";

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	if( physical_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid physical offset.",
		 function );

		return( -1 );
	}
	if( lef_source->physical_offset == -1 )
	{
		return( 0 );
	}
	*physical_offset = lef_source->physical_offset;

	return( 1 );
}

/* Retrieves the acquisition date and time
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_source_get_acquisition_time(
     libewf_lef_source_t *lef_source,
     int64_t *posix_time,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_acquisition_time";

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	if( posix_time == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid POSIX time.",
		 function );

		return( -1 );
	}
	*posix_time = lef_source->acquisition_time;

	return( 1 );
}

/* Retrieves the UTF-8 encoded MD5 hash value
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_lef_source_get_utf8_hash_value_md5(
     libewf_lef_source_t *lef_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf8_hash_value_md5";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf8_string(
	          lef_source->md5_hash,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy MD5 hash to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the UTF-16 encoded MD5 hash value
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_lef_source_get_utf16_hash_value_md5(
     libewf_lef_source_t *lef_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf16_hash_value_md5";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf16_string(
	          lef_source->md5_hash,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy MD5 hash to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the UTF-8 encoded SHA1 hash value
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_lef_source_get_utf8_hash_value_sha1(
     libewf_lef_source_t *lef_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf8_hash_value_sha1";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf8_string(
	          lef_source->sha1_hash,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy SHA1 hash to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the UTF-16 encoded SHA1 hash value
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_lef_source_get_utf16_hash_value_sha1(
     libewf_lef_source_t *lef_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_source_get_utf16_hash_value_sha1";
	int result            = 0;

	if( lef_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf16_string(
	          lef_source->sha1_hash,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy SHA1 hash to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( result );
}

