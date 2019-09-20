/*
 * Single file source functions
 *
 */

#include <common.h>
#include <memory.h>
#include <narrow_string.h>
#include <types.h>

#include "libewf_libcerror.h"
#include "libewf_libfvalue.h"
#include "libewf_libuna.h"
#include "libewf_single_file_source.h"
#include "libewf_definitions.h"
#include "libfguid_definitions.h"

/* Creates a single file source
 * Make sure the value single_file_source is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_initialize(
     libewf_single_file_source_t **single_file_source,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_source_initialize";

	if( single_file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file source.",
		 function );

		return( -1 );
	}
	if( *single_file_source != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid single file source value already set.",
		 function );

		return( -1 );
	}
	*single_file_source = memory_allocate_structure(
	                      libewf_single_file_source_t );

	if( *single_file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create single file source.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *single_file_source,
	     0,
	     sizeof( libewf_single_file_source_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear single file source.",
		 function );

		goto on_error;
	}

	( *single_file_source )->physical_offset = -1;
	( *single_file_source )->logical_offset  = -1;

	return( 1 );

on_error:
	if( *single_file_source != NULL )
	{
		memory_free(
		 *single_file_source );

		*single_file_source = NULL;
	}
	return( -1 );
}

/* Frees a single file source
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_free(
     libewf_single_file_source_t **single_file_source,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_source_free";

	if( single_file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file source.",
		 function );

		return( -1 );
	}
	if( *single_file_source != NULL )
	{
		if( ( *single_file_source )->name != NULL )
		{
			memory_free(
			 ( *single_file_source )->name );
		}
		if( ( *single_file_source )->model != NULL )
		{
			memory_free(
			 ( *single_file_source )->model );
		}
		if( ( *single_file_source )->serial_number != NULL )
		{
			memory_free(
			 ( *single_file_source )->serial_number );
		}
		if ((*single_file_source)->manufacturer != NULL)
		{
			memory_free(
				(*single_file_source)->manufacturer);
		}		
		if ((*single_file_source)->evidence_number != NULL)
		{
			memory_free(
				(*single_file_source)->evidence_number);
		}
		if( ( *single_file_source )->md5_hash != NULL )
		{
			memory_free(
			 ( *single_file_source )->md5_hash );
		}
		if( ( *single_file_source )->sha1_hash != NULL )
		{
			memory_free(
			 ( *single_file_source )->sha1_hash );
		}
		if( ( *single_file_source )->primary_guid != NULL )
		{
			memory_free(
			 ( *single_file_source )->primary_guid );
		}
        if( ( *single_file_source )->guid != NULL )
        {
            memory_free(
             ( *single_file_source )->guid );
        }       
		if( ( *single_file_source )->location != NULL )
		{
			memory_free(
			 ( *single_file_source )->location );
		}
		if( ( *single_file_source )->domain != NULL )
		{
			memory_free(
			 ( *single_file_source )->domain );
		}
		if( ( *single_file_source )->ip_address != NULL )
		{
			memory_free(
			 ( *single_file_source )->ip_address );
		}
		if( ( *single_file_source )->mac_address != NULL )
		{
			memory_free(
			 ( *single_file_source )->mac_address );
		}
		memory_free(
		 *single_file_source );

		*single_file_source = NULL;
	}
	return( 1 );
}

/* Clones the single file source
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_clone(
     libewf_single_file_source_t **destination_single_file_source,
     libewf_single_file_source_t *source_single_file_source,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_source_clone";

	if( destination_single_file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination single file source.",
		 function );

		return( -1 );
	}
	if( *destination_single_file_source != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination single file source value already set.",
		 function );

		return( -1 );
	}
	if( source_single_file_source == NULL )
	{
		*destination_single_file_source = NULL;

		return( 1 );
	}
	*destination_single_file_source = memory_allocate_structure(
			                  libewf_single_file_source_t );

	if( *destination_single_file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination single file source.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_single_file_source,
	     source_single_file_source,
	     sizeof( libewf_single_file_source_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination single file source.",
		 function );

		memory_free(
		 *destination_single_file_source );

		*destination_single_file_source = NULL;

		return( -1 );
	}
	( *destination_single_file_source )->name            = NULL;
	( *destination_single_file_source )->serial_number   = NULL;
	( *destination_single_file_source )->manufacturer    = NULL;
	( *destination_single_file_source )->evidence_number = NULL;
	( *destination_single_file_source )->model           = NULL;
    ( *destination_single_file_source )->primary_guid    = NULL;
    ( *destination_single_file_source )->md5_hash        = NULL;
    ( *destination_single_file_source )->sha1_hash       = NULL;
    ( *destination_single_file_source )->guid            = NULL;
	( *destination_single_file_source )->location        = NULL;
    ( *destination_single_file_source )->domain          = NULL;
    ( *destination_single_file_source )->ip_address      = NULL;
    ( *destination_single_file_source )->mac_address     = NULL;
	
	if( source_single_file_source->name != NULL )
	{
		( *destination_single_file_source )->name = (uint8_t *) memory_allocate(
		                                                        sizeof( uint8_t ) * source_single_file_source->name_size );

		if( ( *destination_single_file_source )->name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create destination name.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     ( *destination_single_file_source )->name,
		     source_single_file_source->name,
		     source_single_file_source->name_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy source to destination name.",
			 function );

			goto on_error;
		}
		( *destination_single_file_source )->name_size = source_single_file_source->name_size;
	}
	if( source_single_file_source->serial_number != NULL )
	{
		( *destination_single_file_source )->serial_number = (uint8_t *) memory_allocate(
		                                                        sizeof( uint8_t ) * source_single_file_source->serial_number_size );

		if( ( *destination_single_file_source )->serial_number == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create destination serial_number.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     ( *destination_single_file_source )->serial_number,
		     source_single_file_source->serial_number,
		     source_single_file_source->serial_number_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy source to destination serial_number.",
			 function );

			goto on_error;
		}
		( *destination_single_file_source )->serial_number_size = source_single_file_source->serial_number_size;
	}
	if( source_single_file_source->manufacturer != NULL )
	{
		( *destination_single_file_source )->manufacturer = (uint8_t *) memory_allocate(
		                                                        sizeof( uint8_t ) * source_single_file_source->manufacturer_size );

		if( ( *destination_single_file_source )->manufacturer == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create destination manufacturer.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     ( *destination_single_file_source )->manufacturer,
		     source_single_file_source->manufacturer,
		     source_single_file_source->manufacturer_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy source to destination manufacturer.",
			 function );

			goto on_error;
		}
		( *destination_single_file_source )->manufacturer_size = source_single_file_source->manufacturer_size;
	}
	if (source_single_file_source->evidence_number != NULL)
	{
		(*destination_single_file_source)->evidence_number = (uint8_t *)memory_allocate(
			sizeof(uint8_t) * source_single_file_source->evidence_number_size);

		if ((*destination_single_file_source)->evidence_number == NULL)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_MEMORY,
				LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				"%s: unable to create destination evidence_number.",
				function);

			goto on_error;
		}
		if (memory_copy(
			(*destination_single_file_source)->evidence_number,
			source_single_file_source->evidence_number,
			source_single_file_source->evidence_number_size) == NULL)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_MEMORY,
				LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				"%s: unable to copy source to destination evidence_number.",
				function);

			goto on_error;
		}
		(*destination_single_file_source)->evidence_number_size = source_single_file_source->evidence_number_size;
	}

	if( source_single_file_source->model != NULL )
	{
		( *destination_single_file_source )->model = (uint8_t *) memory_allocate(
		                                                            sizeof( uint8_t ) * source_single_file_source->model_size );

		if( ( *destination_single_file_source )->model == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create destination model.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     ( *destination_single_file_source )->model,
		     source_single_file_source->model,
		     source_single_file_source->model_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy source to destination model.",
			 function );

			goto on_error;
		}
		( *destination_single_file_source )->model_size = source_single_file_source->model_size;
	}
	if( source_single_file_source->md5_hash != NULL )
	{
		( *destination_single_file_source )->md5_hash = (uint8_t *) memory_allocate(
		                                                            sizeof( uint8_t ) * source_single_file_source->md5_hash_size );

		if( ( *destination_single_file_source )->md5_hash == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create destination MD5 hash.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     ( *destination_single_file_source )->md5_hash,
		     source_single_file_source->md5_hash,
		     source_single_file_source->md5_hash_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy source to destination MD5 hash.",
			 function );

			goto on_error;
		}
		( *destination_single_file_source )->md5_hash_size = source_single_file_source->md5_hash_size;
	}
	if( source_single_file_source->sha1_hash != NULL )
	{
		( *destination_single_file_source )->sha1_hash = (uint8_t *) memory_allocate(
		                                                             sizeof( uint8_t ) * source_single_file_source->sha1_hash_size );

		if( ( *destination_single_file_source )->sha1_hash == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create destination SHA1 hash.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     ( *destination_single_file_source )->sha1_hash,
		     source_single_file_source->sha1_hash,
		     source_single_file_source->sha1_hash_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy source to destination SHA1 hash.",
			 function );

			goto on_error;
		}
		( *destination_single_file_source )->sha1_hash_size = source_single_file_source->sha1_hash_size;
	}
	if( source_single_file_source->primary_guid != NULL )
	{
		( *destination_single_file_source )->primary_guid = (uint8_t *) memory_allocate(
		                                                             sizeof( uint8_t ) * source_single_file_source->primary_guid_size );

		if( ( *destination_single_file_source )->primary_guid == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create destination primary guid.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     ( *destination_single_file_source )->primary_guid,
		     source_single_file_source->primary_guid,
		     source_single_file_source->primary_guid_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy source to destination primary guid.",
			 function );

			goto on_error;
		}
		( *destination_single_file_source )->primary_guid_size = source_single_file_source->primary_guid_size;
	}
	if( source_single_file_source->guid != NULL)
	{
		( *destination_single_file_source )->guid = ( uint8_t *)memory_allocate(
			                                                sizeof( uint8_t ) * source_single_file_source->guid_size );

		if( ( *destination_single_file_source )->guid == NULL )
		{
			libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_MEMORY,
			LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			"%s: unable to create destination GUID.",
			function );

			goto on_error;
		}
		if( memory_copy(
			( *destination_single_file_source )->guid,
			source_single_file_source->guid,
			source_single_file_source->guid_size ) == NULL )
		{
			libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_MEMORY,
			LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			"%s: unable to copy source to destination GUID.",
			function );

			goto on_error;
		}
		( *destination_single_file_source )->guid_size = source_single_file_source->guid_size;
	}
	if( source_single_file_source->location != NULL)
	{
		( *destination_single_file_source )->location = ( uint8_t *)memory_allocate(
			                                                sizeof( uint8_t ) * source_single_file_source->location_size );

		if( ( *destination_single_file_source )->location == NULL )
		{
			libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_MEMORY,
			LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			"%s: unable to create destination GUID.",
			function );

			goto on_error;
		}
		if( memory_copy(
			( *destination_single_file_source )->location,
			source_single_file_source->location,
			source_single_file_source->location_size ) == NULL )
		{
			libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_MEMORY,
			LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			"%s: unable to copy source to destination GUID.",
			function );

			goto on_error;
		}
		( *destination_single_file_source )->location_size = source_single_file_source->location_size;
	}
	if( source_single_file_source->domain != NULL)
	{
		( *destination_single_file_source )->domain = ( uint8_t *)memory_allocate(
			                                                sizeof( uint8_t ) * source_single_file_source->domain_size );

		if( ( *destination_single_file_source )->domain == NULL )
		{
			libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_MEMORY,
			LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			"%s: unable to create destination GUID.",
			function );

			goto on_error;
		}
		if( memory_copy(
			( *destination_single_file_source )->domain,
			source_single_file_source->domain,
			source_single_file_source->domain_size ) == NULL )
		{
			libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_MEMORY,
			LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			"%s: unable to copy source to destination GUID.",
			function );

			goto on_error;
		}
		( *destination_single_file_source )->domain_size = source_single_file_source->domain_size;
	}
	if( source_single_file_source->ip_address != NULL)
	{
		( *destination_single_file_source )->ip_address = ( uint8_t *)memory_allocate(
			                                                sizeof( uint8_t ) * source_single_file_source->ip_address_size );

		if( ( *destination_single_file_source )->ip_address == NULL )
		{
			libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_MEMORY,
			LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			"%s: unable to create destination GUID.",
			function );

			goto on_error;
		}
		if( memory_copy(
			( *destination_single_file_source )->ip_address,
			source_single_file_source->ip_address,
			source_single_file_source->ip_address_size ) == NULL )
		{
			libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_MEMORY,
			LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			"%s: unable to copy source to destination GUID.",
			function );

			goto on_error;
		}
		( *destination_single_file_source )->ip_address_size = source_single_file_source->ip_address_size;
	}
	if( source_single_file_source->mac_address != NULL)
	{
		( *destination_single_file_source )->mac_address = ( uint8_t *)memory_allocate(
			                                                sizeof( uint8_t ) * source_single_file_source->mac_address_size );

		if( ( *destination_single_file_source )->mac_address == NULL )
		{
			libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_MEMORY,
			LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			"%s: unable to create destination GUID.",
			function );

			goto on_error;
		}
		if( memory_copy(
			( *destination_single_file_source )->mac_address,
			source_single_file_source->mac_address,
			source_single_file_source->mac_address_size ) == NULL )
		{
			libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_MEMORY,
			LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			"%s: unable to copy source to destination GUID.",
			function );

			goto on_error;
		}
		( *destination_single_file_source )->mac_address_size = source_single_file_source->mac_address_size;
	}
	return( 1 );

on_error:
	if( *destination_single_file_source != NULL )
	{
		if( ( *destination_single_file_source )->primary_guid != NULL )
		{
			memory_free(
			 ( *destination_single_file_source )->primary_guid );
		}
		if( ( *destination_single_file_source )->model != NULL )
		{
			memory_free(
			 ( *destination_single_file_source )->model );
		}
		if( ( *destination_single_file_source )->sha1_hash != NULL )
		{
			memory_free(
			 ( *destination_single_file_source )->sha1_hash );
		}
		if( ( *destination_single_file_source )->md5_hash != NULL )
		{
			memory_free(
			 ( *destination_single_file_source )->md5_hash );
		}
        if( ( *destination_single_file_source)->guid != NULL )
        {
            memory_free(
             ( *destination_single_file_source )->guid );
        }
		if( ( *destination_single_file_source )->name != NULL )
		{
			memory_free(
			 ( *destination_single_file_source )->name );
		}
		if( ( *destination_single_file_source )->serial_number != NULL )
		{
			memory_free(
			 ( *destination_single_file_source )->serial_number );
		}
		if( ( *destination_single_file_source)->manufacturer != NULL)
		{
			memory_free(
			 ( *destination_single_file_source)->manufacturer );
		}
		if( ( *destination_single_file_source)->evidence_number != NULL)
		{
			memory_free(
			 ( *destination_single_file_source)->evidence_number);
		}				
		if( ( *destination_single_file_source)->location != NULL)
		{
			memory_free(
			 ( *destination_single_file_source)->location);
		}
		if( ( *destination_single_file_source)->domain != NULL)
		{
			memory_free(
			 ( *destination_single_file_source)->domain);
		}
		if( ( *destination_single_file_source)->ip_address != NULL)
		{
			memory_free(
			 ( *destination_single_file_source)->ip_address);
		}
		if ( ( *destination_single_file_source)->mac_address != NULL)
		{
			memory_free(
			 ( *destination_single_file_source)->mac_address);
		}
		memory_free(
		 *destination_single_file_source );

		*destination_single_file_source = NULL;
	}
	return( -1 );
}

/* Retrieves the id
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_id(
     libewf_single_file_source_t *single_file_source,
     uint32_t *id,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_source_get_id";

	if( single_file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file source.",
		 function );

		return( -1 );
	}
	if( id == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid access time.",
		 function );

		return( -1 );
	}
	*id = single_file_source->id;

	return( 1 );
}

/* Retrieves the type
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_type(
     libewf_single_file_source_t *single_file_source,
     uint8_t *type,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_source_get_type";

	if( single_file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file source.",
		 function );

		return( -1 );
	}
	if( type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid type.",
		 function );

		return( -1 );
	}
	*type = single_file_source->type;

	return( 1 );
}

/* Retrieves the drive type
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_drive_type(
     libewf_single_file_source_t *single_file_source,
     uint8_t *drive_type,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_source_get_drive_type";

	if( single_file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file source.",
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
	*drive_type = single_file_source->drive_type;

	return( 1 );
}

/* Retrieves the physical offset
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_physical_offset(
     libewf_single_file_source_t *single_file_source,
     off64_t *physical_offset,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_source_get_physical_offset";

	if( single_file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file source.",
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
	*physical_offset = single_file_source->physical_offset;

	return( 1 );
}

/* Retrieves the logical offset
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_logical_offset(
     libewf_single_file_source_t *single_file_source,
     off64_t *logical_offset,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_source_get_logical_offset";

	if( single_file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file source.",
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
	*logical_offset = single_file_source->logical_offset;

	return( 1 );
}

/* Retrieves the total size
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_total_size(
     libewf_single_file_source_t *single_file_source,
     size64_t *total_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_source_get_total_size";

	if( single_file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file source.",
		 function );

		return( -1 );
	}
	if( total_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid total size.",
		 function );

		return( -1 );
	}
	*total_size = single_file_source->total_size;

	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_utf8_name_size(
     libewf_single_file_source_t *single_file_source,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_source_get_utf8_name_size";

	if( single_file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file source.",
		 function );

		return( -1 );
	}
	if( utf8_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string size.",
		 function );

		return( -1 );
	}
	*utf8_string_size = single_file_source->name_size;

	return( 1 );
}

/* Retrieves the UTF-8 encoded name value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_utf8_name(
     libewf_single_file_source_t *single_file_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_source_get_utf8_name";

	if( single_file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file source.",
		 function );

		return( -1 );
	}
	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid UTF-8 string size value too small.",
		 function );

		return( -1 );
	}
	if( utf8_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( single_file_source->name == NULL )
	 || ( single_file_source->name_size == 0 ) )
	{
		utf8_string[ 0 ] = 0;
	}
	else
	{
		if( utf8_string_size < single_file_source->name_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: invalid UTF-8 string size value too small.",
			 function );

			return( -1 );
		}
		if( narrow_string_copy(
		     (char *) utf8_string,
		     (char *) single_file_source->name,
		     single_file_source->name_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set UTF-8 string.",
			 function );

			return( -1 );
		}
		utf8_string[ single_file_source->name_size - 1 ] = 0;
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_utf16_name_size(
     libewf_single_file_source_t *single_file_source,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_source_get_utf16_name_size";

	if( single_file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file source.",
		 function );

		return( -1 );
	}
	if( ( single_file_source->name == NULL )
	 || ( single_file_source->name_size == 0 ) )
	{
		if( utf16_string_size == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-16 string size.",
			 function );

			return( -1 );
		}
		*utf16_string_size = 0;
	}
	else
	{
		if( libuna_utf16_string_size_from_utf8(
		     single_file_source->name,
		     single_file_source->name_size,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-16 string size.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded name value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_utf16_name(
     libewf_single_file_source_t *single_file_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_source_get_utf16_name";

	if( single_file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file source.",
		 function );

		return( -1 );
	}
	if( utf16_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( utf16_string_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid UTF-16 string size value too small.",
		 function );

		return( -1 );
	}
	if( utf16_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( single_file_source->name == NULL )
	 || ( single_file_source->name_size == 0 ) )
	{
		utf16_string[ 0 ] = 0;
	}
	else
	{
		if( libuna_utf16_string_copy_from_utf8(
		     utf16_string,
		     utf16_string_size,
		     single_file_source->name,
		     single_file_source->name_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy name to UTF-16 string.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded serial_number
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_utf8_serial_number_size(
     libewf_single_file_source_t *single_file_source,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_source_get_utf8_serial_number_size";

	if( single_file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file source.",
		 function );

		return( -1 );
	}
	if( utf8_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string size.",
		 function );

		return( -1 );
	}
	*utf8_string_size = single_file_source->serial_number_size;

	return( 1 );
}

/* Retrieves the UTF-8 encoded serial_number value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_utf8_serial_number(
     libewf_single_file_source_t *single_file_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_source_get_utf8_serial_number";

	if( single_file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file source.",
		 function );

		return( -1 );
	}
	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid UTF-8 string size value too small.",
		 function );

		return( -1 );
	}
	if( utf8_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( single_file_source->serial_number == NULL )
	 || ( single_file_source->serial_number_size == 0 ) )
	{
		utf8_string[ 0 ] = 0;
	}
	else
	{
		if( utf8_string_size < single_file_source->serial_number_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: invalid UTF-8 string size value too small.",
			 function );

			return( -1 );
		}
		if( narrow_string_copy(
		     (char *) utf8_string,
		     (char *) single_file_source->serial_number,
		     single_file_source->serial_number_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set UTF-8 string.",
			 function );

			return( -1 );
		}
		utf8_string[ single_file_source->serial_number_size - 1 ] = 0;
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded serial_number
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_utf16_serial_number_size(
     libewf_single_file_source_t *single_file_source,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_source_get_utf16_serial_number_size";

	if( single_file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file source.",
		 function );

		return( -1 );
	}
	if( ( single_file_source->serial_number == NULL )
	 || ( single_file_source->serial_number_size == 0 ) )
	{
		if( utf16_string_size == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-16 string size.",
			 function );

			return( -1 );
		}
		*utf16_string_size = 0;
	}
	else
	{
		if( libuna_utf16_string_size_from_utf8(
		     single_file_source->serial_number,
		     single_file_source->serial_number_size,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-16 string size.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded serial_number value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_utf16_serial_number(
     libewf_single_file_source_t *single_file_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_source_get_utf16_serial_number";

	if( single_file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file source.",
		 function );

		return( -1 );
	}
	if( utf16_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( utf16_string_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid UTF-16 string size value too small.",
		 function );

		return( -1 );
	}
	if( utf16_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( single_file_source->serial_number == NULL )
	 || ( single_file_source->serial_number_size == 0 ) )
	{
		utf16_string[ 0 ] = 0;
	}
	else
	{
		if( libuna_utf16_string_copy_from_utf8(
		     utf16_string,
		     utf16_string_size,
		     single_file_source->serial_number,
		     single_file_source->serial_number_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy serial_number to UTF-16 string.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded manufacturer
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_utf8_manufacturer_size(
     libewf_single_file_source_t *single_file_source,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_source_get_utf8_manufacturer_size";

	if( single_file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file source.",
		 function );

		return( -1 );
	}
	if( utf8_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string size.",
		 function );

		return( -1 );
	}
	*utf8_string_size = single_file_source->manufacturer_size;

	return( 1 );
}

/* Retrieves the UTF-8 encoded manufacturer value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_utf8_manufacturer(
     libewf_single_file_source_t *single_file_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_source_get_utf8_manufacturer";

	if( single_file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file source.",
		 function );

		return( -1 );
	}
	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid UTF-8 string size value too small.",
		 function );

		return( -1 );
	}
	if( utf8_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( single_file_source->manufacturer == NULL )
	 || ( single_file_source->manufacturer_size == 0 ) )
	{
		utf8_string[ 0 ] = 0;
	}
	else
	{
		if( utf8_string_size < single_file_source->manufacturer_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: invalid UTF-8 string size value too small.",
			 function );

			return( -1 );
		}
		if( narrow_string_copy(
		     (char *) utf8_string,
		     (char *) single_file_source->manufacturer,
		     single_file_source->manufacturer_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set UTF-8 string.",
			 function );

			return( -1 );
		}
		utf8_string[ single_file_source->manufacturer_size - 1 ] = 0;
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded manufacturer
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_utf16_manufacturer_size(
     libewf_single_file_source_t *single_file_source,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_source_get_utf16_manufacturer_size";

	if( single_file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file source.",
		 function );

		return( -1 );
	}
	if( ( single_file_source->manufacturer == NULL )
	 || ( single_file_source->manufacturer_size == 0 ) )
	{
		if( utf16_string_size == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-16 string size.",
			 function );

			return( -1 );
		}
		*utf16_string_size = 0;
	}
	else
	{
		if( libuna_utf16_string_size_from_utf8(
		     single_file_source->manufacturer,
		     single_file_source->manufacturer_size,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-16 string size.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded manufacturer value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_utf16_manufacturer(
     libewf_single_file_source_t *single_file_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_source_get_utf16_manufacturer";

	if( single_file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file source.",
		 function );

		return( -1 );
	}
	if( utf16_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( utf16_string_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid UTF-16 string size value too small.",
		 function );

		return( -1 );
	}
	if( utf16_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( single_file_source->manufacturer == NULL )
	 || ( single_file_source->manufacturer_size == 0 ) )
	{
		utf16_string[ 0 ] = 0;
	}
	else
	{
		if( libuna_utf16_string_copy_from_utf8(
		     utf16_string,
		     utf16_string_size,
		     single_file_source->manufacturer,
		     single_file_source->manufacturer_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy manufacturer to UTF-16 string.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded evidence_number
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_utf8_evidence_number_size(
	libewf_single_file_source_t *single_file_source,
	size_t *utf8_string_size,
	libcerror_error_t **error)
{
	static char *function = "libewf_single_file_source_get_utf8_evidence_number_size";

	if (single_file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid single file source.",
			function);

		return(-1);
	}
	if (utf8_string_size == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid UTF-8 string size.",
			function);

		return(-1);
	}
	*utf8_string_size = single_file_source->evidence_number_size;

	return(1);
}

/* Retrieves the UTF-8 encoded evidence_number value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_utf8_evidence_number(
	libewf_single_file_source_t *single_file_source,
	uint8_t *utf8_string,
	size_t utf8_string_size,
	libcerror_error_t **error)
{
	static char *function = "libewf_single_file_source_get_utf8_evidence_number";

	if (single_file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid single file source.",
			function);

		return(-1);
	}
	if (utf8_string == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid UTF-8 string.",
			function);

		return(-1);
	}
	if (utf8_string_size == 0)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			"%s: invalid UTF-8 string size value too small.",
			function);

		return(-1);
	}
	if (utf8_string_size > (size_t)SSIZE_MAX)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			"%s: invalid UTF-8 string size value exceeds maximum.",
			function);

		return(-1);
	}
	if ((single_file_source->evidence_number == NULL)
		|| (single_file_source->evidence_number_size == 0))
	{
		utf8_string[0] = 0;
	}
	else
	{
		if (utf8_string_size < single_file_source->evidence_number_size)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				"%s: invalid UTF-8 string size value too small.",
				function);

			return(-1);
		}
		if (narrow_string_copy(
			(char *)utf8_string,
			(char *)single_file_source->evidence_number,
			single_file_source->evidence_number_size) == NULL)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_MEMORY,
				LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				"%s: unable to set UTF-8 string.",
				function);

			return(-1);
		}
		utf8_string[single_file_source->evidence_number_size - 1] = 0;
	}
	return(1);
}

/* Retrieves the size of the UTF-16 encoded evidence_number
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_utf16_evidence_number_size(
	libewf_single_file_source_t *single_file_source,
	size_t *utf16_string_size,
	libcerror_error_t **error)
{
	static char *function = "libewf_single_file_source_get_utf16_evidence_number_size";

	if (single_file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid single file source.",
			function);

		return(-1);
	}
	if ((single_file_source->evidence_number == NULL)
		|| (single_file_source->evidence_number_size == 0))
	{
		if (utf16_string_size == NULL)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
				"%s: invalid UTF-16 string size.",
				function);

			return(-1);
		}
		*utf16_string_size = 0;
	}
	else
	{
		if (libuna_utf16_string_size_from_utf8(
			single_file_source->evidence_number,
			single_file_source->evidence_number_size,
			utf16_string_size,
			error) != 1)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_RUNTIME,
				LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				"%s: unable to retrieve UTF-16 string size.",
				function);

			return(-1);
		}
	}
	return(1);
}

/* Retrieves the UTF-16 encoded evidence_number value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_utf16_evidence_number(
	libewf_single_file_source_t *single_file_source,
	uint16_t *utf16_string,
	size_t utf16_string_size,
	libcerror_error_t **error)
{
	static char *function = "libewf_single_file_source_get_utf16_evidence_number";

	if (single_file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid single file source.",
			function);

		return(-1);
	}
	if (utf16_string == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid UTF-16 string.",
			function);

		return(-1);
	}
	if (utf16_string_size == 0)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			"%s: invalid UTF-16 string size value too small.",
			function);

		return(-1);
	}
	if (utf16_string_size > (size_t)SSIZE_MAX)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			"%s: invalid UTF-16 string size value exceeds maximum.",
			function);

		return(-1);
	}
	if ((single_file_source->evidence_number == NULL)
		|| (single_file_source->evidence_number_size == 0))
	{
		utf16_string[0] = 0;
	}
	else
	{
		if (libuna_utf16_string_copy_from_utf8(
			utf16_string,
			utf16_string_size,
			single_file_source->evidence_number,
			single_file_source->evidence_number_size,
			error) != 1)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_RUNTIME,
				LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				"%s: unable to copy evidence_number to UTF-16 string.",
				function);

			return(-1);
		}
	}
	return(1);
}

/* Retrieves the size of the UTF-8 encoded model
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_utf8_model_size(
     libewf_single_file_source_t *single_file_source,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_source_get_utf8_model_size";

	if( single_file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file source.",
		 function );

		return( -1 );
	}
	if( utf8_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string size.",
		 function );

		return( -1 );
	}
	*utf8_string_size = single_file_source->model_size;

	return( 1 );
}

/* Retrieves the UTF-8 encoded model
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_single_file_source_get_utf8_model(
     libewf_single_file_source_t *single_file_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_source_get_utf8_model";

	if( single_file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file source.",
		 function );

		return( -1 );
	}
	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf8_string_size < single_file_source->model_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-8 string too small.",
		 function );

		return( -1 );
	}
	if( single_file_source->model_size == 0 )
	{
		return( 0 );
	}
	if( memory_copy(
	     utf8_string,
	     single_file_source->model,
	     single_file_source->model_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
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
int libewf_single_file_source_get_utf16_model_size(
     libewf_single_file_source_t *single_file_source,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_source_get_utf16_model_size";

	if( single_file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file source.",
		 function );

		return( -1 );
	}
	if( ( single_file_source->model == NULL )
	 || ( single_file_source->model_size == 0 ) )
	{
		if( utf16_string_size == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-16 string size.",
			 function );

			return( -1 );
		}
		*utf16_string_size = 0;
	}
	else
	{
		if( libuna_utf16_string_size_from_utf8(
		     single_file_source->model,
		     single_file_source->model_size,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-16 string size.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded model
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_single_file_source_get_utf16_model(
     libewf_single_file_source_t *single_file_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_source_get_utf16_model";

	if( single_file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file source.",
		 function );

		return( -1 );
	}
	if( utf16_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( utf16_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf16_string_size < single_file_source->model_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-16 string too small.",
		 function );

		return( -1 );
	}
	if( single_file_source->model_size == 0 )
	{
		return( 0 );
	}
	if( libuna_utf16_string_copy_from_utf8(
	     utf16_string,
	     utf16_string_size,
	     single_file_source->model,
	     single_file_source->model_size,
	     error ) != 1 )
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

/* Retrieves the UTF-8 encoded MD5 hash value
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_single_file_source_get_utf8_hash_value_md5(
     libewf_single_file_source_t *single_file_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_source_get_utf8_hash_value_md5";

	if( single_file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file source.",
		 function );

		return( -1 );
	}
	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf8_string_size < single_file_source->md5_hash_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-8 string too small.",
		 function );

		return( -1 );
	}
	if( single_file_source->md5_hash_size == 0 )
	{
		return( 0 );
	}
	if( memory_copy(
	     utf8_string,
	     single_file_source->md5_hash,
	     single_file_source->md5_hash_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy MD5 hash to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded MD5 hash value
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_single_file_source_get_utf16_hash_value_md5(
     libewf_single_file_source_t *single_file_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_source_get_utf16_hash_value_md5";

	if( single_file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file source.",
		 function );

		return( -1 );
	}
	if( utf16_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( utf16_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf16_string_size < single_file_source->md5_hash_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-16 string too small.",
		 function );

		return( -1 );
	}
	if( single_file_source->md5_hash_size == 0 )
	{
		return( 0 );
	}
	if( libuna_utf16_string_copy_from_utf8(
	     utf16_string,
	     utf16_string_size,
	     single_file_source->md5_hash,
	     single_file_source->md5_hash_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy MD5 hash to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded SHA1 hash value
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_single_file_source_get_utf8_hash_value_sha1(
     libewf_single_file_source_t *single_file_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_source_get_utf8_hash_value_sha1";

	if( single_file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file source.",
		 function );

		return( -1 );
	}
	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf8_string_size < single_file_source->sha1_hash_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-8 string too small.",
		 function );

		return( -1 );
	}
	if( single_file_source->sha1_hash_size == 0 )
	{
		return( 0 );
	}
	if( memory_copy(
	     utf8_string,
	     single_file_source->sha1_hash,
	     single_file_source->sha1_hash_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy SHA1 hash to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded SHA1 hash value
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_single_file_source_get_utf16_hash_value_sha1(
     libewf_single_file_source_t *single_file_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_source_get_utf16_hash_value_sha1";

	if( single_file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file source.",
		 function );

		return( -1 );
	}
	if( utf16_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( utf16_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf16_string_size < single_file_source->sha1_hash_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-16 string too small.",
		 function );

		return( -1 );
	}
	if( single_file_source->sha1_hash_size == 0 )
	{
		return( 0 );
	}
	if( libuna_utf16_string_copy_from_utf8(
	     utf16_string,
	     utf16_string_size,
	     single_file_source->sha1_hash,
	     single_file_source->sha1_hash_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy SHA1 hash to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded primary guid value
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_single_file_source_get_utf8_primary_guid(
     libewf_single_file_source_t *single_file_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_source_get_utf8_primary_guid_value";

	if( single_file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file source.",
		 function );

		return( -1 );
	}
	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf8_string_size < single_file_source->primary_guid_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-8 string too small.",
		 function );

		return( -1 );
	}
	if( single_file_source->primary_guid_size == 0 )
	{
		return( 0 );
	}
	if( memory_copy(
	     utf8_string,
	     single_file_source->primary_guid,
	     single_file_source->primary_guid_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy primary guid to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded primary guid value
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_single_file_source_get_utf16_primary_guid(
     libewf_single_file_source_t *single_file_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_source_get_utf16_primary_guid_value";

	if( single_file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file source.",
		 function );

		return( -1 );
	}
	if( utf16_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( utf16_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf16_string_size < single_file_source->primary_guid_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-16 string too small.",
		 function );

		return( -1 );
	}
	if( single_file_source->primary_guid_size == 0 )
	{
		return( 0 );
	}
	if( libuna_utf16_string_copy_from_utf8(
	     utf16_string,
	     utf16_string_size,
	     single_file_source->primary_guid,
	     single_file_source->primary_guid_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy primary guid to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded GUID value
* Returns 1 if successful, 0 if value not present or -1 on error
*/
int libewf_single_file_source_get_utf8_guid(
  libewf_single_file_source_t *single_file_source,
  uint8_t *utf8_string,
  size_t utf8_string_size,
  libcerror_error_t **error)
{
  static char *function = "libewf_single_file_source_get_utf8_guid";

  if (single_file_source == NULL)
  {
    libcerror_error_set(
      error,
      LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
      LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
      "%s: invalid single file source.",
      function);

    return(-1);
  }
  if (utf8_string == NULL)
  {
    libcerror_error_set(
      error,
      LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
      LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
      "%s: invalid UTF-8 string.",
      function);

    return(-1);
  }
  if (utf8_string_size > (size_t)SSIZE_MAX)
  {
    libcerror_error_set(
      error,
      LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
      LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
      "%s: invalid UTF-8 string size value exceeds maximum.",
      function);

    return(-1);
  }
  if (utf8_string_size < single_file_source->guid_size)
  {
    libcerror_error_set(
      error,
      LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
      LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
      "%s: UTF-8 string too small.",
      function);

    return(-1);
  }
  if (single_file_source->guid_size == 0)
  {
    return(0);
  }
  if (memory_copy(
    utf8_string,
    single_file_source->guid,
    single_file_source->guid_size) == NULL)
  {
    libcerror_error_set(
      error,
      LIBCERROR_ERROR_DOMAIN_MEMORY,
      LIBCERROR_MEMORY_ERROR_COPY_FAILED,
      "%s: unable to copy GUID to UTF-8 string.",
      function);

    return(-1);
  }
  return(1);
}

/* Retrieves the UTF-16 encoded GUID value
* Returns 1 if successful, 0 if value not present or -1 on error
*/
int libewf_single_file_source_get_utf16_guid(
  libewf_single_file_source_t *single_file_source,
  uint16_t *utf16_string,
  size_t utf16_string_size,
  libcerror_error_t **error)
{
  static char *function = "libewf_single_file_source_get_utf16_guid";

  if (single_file_source == NULL)
  {
    libcerror_error_set(
      error,
      LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
      LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
      "%s: invalid single file source.",
      function);

    return(-1);
  }
  if (utf16_string == NULL)
  {
    libcerror_error_set(
      error,
      LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
      LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
      "%s: invalid UTF-16 string.",
      function);

    return(-1);
  }
  if (utf16_string_size > (size_t)SSIZE_MAX)
  {
    libcerror_error_set(
      error,
      LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
      LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
      "%s: invalid UTF-16 string size value exceeds maximum.",
      function);

    return(-1);
  }
  if (utf16_string_size < single_file_source->guid_size)
  {
    libcerror_error_set(
      error,
      LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
      LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
      "%s: UTF-16 string too small.",
      function);

    return(-1);
  }
  if (single_file_source->guid_size == 0)
  {
    return(0);
  }
  if (libuna_utf16_string_copy_from_utf8(
    utf16_string,
    utf16_string_size,
    single_file_source->guid,
    single_file_source->guid_size,
    error) != 1)
  {
    libcerror_error_set(
      error,
      LIBCERROR_ERROR_DOMAIN_RUNTIME,
      LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
      "%s: unable to copy GUID to UTF-16 string.",
      function);

    return(-1);
  }
  return(1);
}

/* Retrieves the acqisition time
*/
int libewf_single_file_source_get_acquisition_time(
	libewf_single_file_source_t *single_file_source,
	uint32_t *acquisition_time,
	libcerror_error_t **error)
{
	static char *function = "libewf_single_file_source_get_acquisition_time";

	if (single_file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid single file source.",
			function);

		return(-1);
	}
	if (acquisition_time == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid acquisition time.",
			function);

		return(-1);
	}
	*acquisition_time = single_file_source->acquisition_time;

	return(1);
}

/* Retrieves the size of the UTF-8 encoded location
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_utf8_location_size(
	libewf_single_file_source_t *single_file_source,
	size_t *utf8_string_size,
	libcerror_error_t **error)
{
	static char *function = "libewf_single_file_source_get_utf8_location_size";

	if (single_file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid single file source.",
			function);

		return(-1);
	}
	if (utf8_string_size == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid UTF-8 string size.",
			function);

		return(-1);
	}
	*utf8_string_size = single_file_source->location_size;

	return(1);
}

/* Retrieves the UTF-8 encoded location value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_utf8_location(
	libewf_single_file_source_t *single_file_source,
	uint8_t *utf8_string,
	size_t utf8_string_size,
	libcerror_error_t **error)
{
	static char *function = "libewf_single_file_source_get_utf8_location";

	if (single_file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid single file source.",
			function);

		return(-1);
	}
	if (utf8_string == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid UTF-8 string.",
			function);

		return(-1);
	}
	if (utf8_string_size == 0)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			"%s: invalid UTF-8 string size value too small.",
			function);

		return(-1);
	}
	if (utf8_string_size > (size_t)SSIZE_MAX)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			"%s: invalid UTF-8 string size value exceeds maximum.",
			function);

		return(-1);
	}
	if ((single_file_source->location == NULL)
		|| (single_file_source->location_size == 0))
	{
		utf8_string[0] = 0;
	}
	else
	{
		if (utf8_string_size < single_file_source->location_size)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				"%s: invalid UTF-8 string size value too small.",
				function);

			return(-1);
		}
		if (narrow_string_copy(
			(char *)utf8_string,
			(char *)single_file_source->location,
			single_file_source->location_size) == NULL)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_MEMORY,
				LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				"%s: unable to set UTF-8 string.",
				function);

			return(-1);
		}
		utf8_string[single_file_source->location_size - 1] = 0;
	}
	return(1);
}

/* Retrieves the size of the UTF-16 encoded location
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_utf16_location_size(
	libewf_single_file_source_t *single_file_source,
	size_t *utf16_string_size,
	libcerror_error_t **error)
{
	static char *function = "libewf_single_file_source_get_utf16_location_size";

	if (single_file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid single file source.",
			function);

		return(-1);
	}
	if ((single_file_source->location == NULL)
		|| (single_file_source->location_size == 0))
	{
		if (utf16_string_size == NULL)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
				"%s: invalid UTF-16 string size.",
				function);

			return(-1);
		}
		*utf16_string_size = 0;
	}
	else
	{
		if (libuna_utf16_string_size_from_utf8(
			single_file_source->location,
			single_file_source->location_size,
			utf16_string_size,
			error) != 1)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_RUNTIME,
				LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				"%s: unable to retrieve UTF-16 string size.",
				function);

			return(-1);
		}
	}
	return(1);
}

/* Retrieves the UTF-16 encoded location value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_utf16_location(
	libewf_single_file_source_t *single_file_source,
	uint16_t *utf16_string,
	size_t utf16_string_size,
	libcerror_error_t **error)
{
	static char *function = "libewf_single_file_source_get_utf16_location";

	if (single_file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid single file source.",
			function);

		return(-1);
	}
	if (utf16_string == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid UTF-16 string.",
			function);

		return(-1);
	}
	if (utf16_string_size == 0)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			"%s: invalid UTF-16 string size value too small.",
			function);

		return(-1);
	}
	if (utf16_string_size > (size_t)SSIZE_MAX)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			"%s: invalid UTF-16 string size value exceeds maximum.",
			function);

		return(-1);
	}
	if ((single_file_source->location == NULL)
		|| (single_file_source->location_size == 0))
	{
		utf16_string[0] = 0;
	}
	else
	{
		if (libuna_utf16_string_copy_from_utf8(
			utf16_string,
			utf16_string_size,
			single_file_source->location,
			single_file_source->location_size,
			error) != 1)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_RUNTIME,
				LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				"%s: unable to copy location to UTF-16 string.",
				function);

			return(-1);
		}
	}
	return(1);
}

/* Retrieves the size of the UTF-8 encoded domain
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_utf8_domain_size(
	libewf_single_file_source_t *single_file_source,
	size_t *utf8_string_size,
	libcerror_error_t **error)
{
	static char *function = "libewf_single_file_source_get_utf8_domain_size";

	if (single_file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid single file source.",
			function);

		return(-1);
	}
	if (utf8_string_size == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid UTF-8 string size.",
			function);

		return(-1);
	}
	*utf8_string_size = single_file_source->domain_size;

	return(1);
}

/* Retrieves the UTF-8 encoded domain value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_utf8_domain(
	libewf_single_file_source_t *single_file_source,
	uint8_t *utf8_string,
	size_t utf8_string_size,
	libcerror_error_t **error)
{
	static char *function = "libewf_single_file_source_get_utf8_domain";

	if (single_file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid single file source.",
			function);

		return(-1);
	}
	if (utf8_string == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid UTF-8 string.",
			function);

		return(-1);
	}
	if (utf8_string_size == 0)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			"%s: invalid UTF-8 string size value too small.",
			function);

		return(-1);
	}
	if (utf8_string_size > (size_t)SSIZE_MAX)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			"%s: invalid UTF-8 string size value exceeds maximum.",
			function);

		return(-1);
	}
	if ((single_file_source->domain == NULL)
		|| (single_file_source->domain_size == 0))
	{
		utf8_string[0] = 0;
	}
	else
	{
		if (utf8_string_size < single_file_source->domain_size)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				"%s: invalid UTF-8 string size value too small.",
				function);

			return(-1);
		}
		if (narrow_string_copy(
			(char *)utf8_string,
			(char *)single_file_source->domain,
			single_file_source->domain_size) == NULL)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_MEMORY,
				LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				"%s: unable to set UTF-8 string.",
				function);

			return(-1);
		}
		utf8_string[single_file_source->domain_size - 1] = 0;
	}
	return(1);
}

/* Retrieves the size of the UTF-16 encoded domain
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_utf16_domain_size(
	libewf_single_file_source_t *single_file_source,
	size_t *utf16_string_size,
	libcerror_error_t **error)
{
	static char *function = "libewf_single_file_source_get_utf16_domain_size";

	if (single_file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid single file source.",
			function);

		return(-1);
	}
	if ((single_file_source->domain == NULL)
		|| (single_file_source->domain_size == 0))
	{
		if (utf16_string_size == NULL)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
				"%s: invalid UTF-16 string size.",
				function);

			return(-1);
		}
		*utf16_string_size = 0;
	}
	else
	{
		if (libuna_utf16_string_size_from_utf8(
			single_file_source->domain,
			single_file_source->domain_size,
			utf16_string_size,
			error) != 1)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_RUNTIME,
				LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				"%s: unable to retrieve UTF-16 string size.",
				function);

			return(-1);
		}
	}
	return(1);
}

/* Retrieves the UTF-16 encoded domain value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_utf16_domain(
	libewf_single_file_source_t *single_file_source,
	uint16_t *utf16_string,
	size_t utf16_string_size,
	libcerror_error_t **error)
{
	static char *function = "libewf_single_file_source_get_utf16_domain";

	if (single_file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid single file source.",
			function);

		return(-1);
	}
	if (utf16_string == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid UTF-16 string.",
			function);

		return(-1);
	}
	if (utf16_string_size == 0)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			"%s: invalid UTF-16 string size value too small.",
			function);

		return(-1);
	}
	if (utf16_string_size > (size_t)SSIZE_MAX)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			"%s: invalid UTF-16 string size value exceeds maximum.",
			function);

		return(-1);
	}
	if ((single_file_source->domain == NULL)
		|| (single_file_source->domain_size == 0))
	{
		utf16_string[0] = 0;
	}
	else
	{
		if (libuna_utf16_string_copy_from_utf8(
			utf16_string,
			utf16_string_size,
			single_file_source->domain,
			single_file_source->domain_size,
			error) != 1)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_RUNTIME,
				LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				"%s: unable to copy domain to UTF-16 string.",
				function);

			return(-1);
		}
	}
	return(1);
}

/* Retrieves the size of the UTF-8 encoded ip_address
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_utf8_ip_address_size(
	libewf_single_file_source_t *single_file_source,
	size_t *utf8_string_size,
	libcerror_error_t **error)
{
	static char *function = "libewf_single_file_source_get_utf8_ip_address_size";

	if (single_file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid single file source.",
			function);

		return(-1);
	}
	if (utf8_string_size == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid UTF-8 string size.",
			function);

		return(-1);
	}
	*utf8_string_size = single_file_source->ip_address_size;

	return(1);
}

/* Retrieves the UTF-8 encoded ip_address value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_utf8_ip_address(
	libewf_single_file_source_t *single_file_source,
	uint8_t *utf8_string,
	size_t utf8_string_size,
	libcerror_error_t **error)
{
	static char *function = "libewf_single_file_source_get_utf8_ip_address";

	if (single_file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid single file source.",
			function);

		return(-1);
	}
	if (utf8_string == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid UTF-8 string.",
			function);

		return(-1);
	}
	if (utf8_string_size == 0)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			"%s: invalid UTF-8 string size value too small.",
			function);

		return(-1);
	}
	if (utf8_string_size > (size_t)SSIZE_MAX)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			"%s: invalid UTF-8 string size value exceeds maximum.",
			function);

		return(-1);
	}
	if ((single_file_source->ip_address == NULL)
		|| (single_file_source->ip_address_size == 0))
	{
		utf8_string[0] = 0;
	}
	else
	{
		if (utf8_string_size < single_file_source->ip_address_size)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				"%s: invalid UTF-8 string size value too small.",
				function);

			return(-1);
		}
		if (narrow_string_copy(
			(char *)utf8_string,
			(char *)single_file_source->ip_address,
			single_file_source->ip_address_size) == NULL)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_MEMORY,
				LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				"%s: unable to set UTF-8 string.",
				function);

			return(-1);
		}
		utf8_string[single_file_source->ip_address_size - 1] = 0;
	}
	return(1);
}

/* Retrieves the size of the UTF-16 encoded ip_address
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_utf16_ip_address_size(
	libewf_single_file_source_t *single_file_source,
	size_t *utf16_string_size,
	libcerror_error_t **error)
{
	static char *function = "libewf_single_file_source_get_utf16_ip_address_size";

	if (single_file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid single file source.",
			function);

		return(-1);
	}
	if ((single_file_source->ip_address == NULL)
		|| (single_file_source->ip_address_size == 0))
	{
		if (utf16_string_size == NULL)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
				"%s: invalid UTF-16 string size.",
				function);

			return(-1);
		}
		*utf16_string_size = 0;
	}
	else
	{
		if (libuna_utf16_string_size_from_utf8(
			single_file_source->ip_address,
			single_file_source->ip_address_size,
			utf16_string_size,
			error) != 1)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_RUNTIME,
				LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				"%s: unable to retrieve UTF-16 string size.",
				function);

			return(-1);
		}
	}
	return(1);
}

/* Retrieves the UTF-16 encoded ip_address value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_utf16_ip_address(
	libewf_single_file_source_t *single_file_source,
	uint16_t *utf16_string,
	size_t utf16_string_size,
	libcerror_error_t **error)
{
	static char *function = "libewf_single_file_source_get_utf16_ip_address";

	if (single_file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid single file source.",
			function);

		return(-1);
	}
	if (utf16_string == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid UTF-16 string.",
			function);

		return(-1);
	}
	if (utf16_string_size == 0)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			"%s: invalid UTF-16 string size value too small.",
			function);

		return(-1);
	}
	if (utf16_string_size > (size_t)SSIZE_MAX)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			"%s: invalid UTF-16 string size value exceeds maximum.",
			function);

		return(-1);
	}
	if ((single_file_source->ip_address == NULL)
		|| (single_file_source->ip_address_size == 0))
	{
		utf16_string[0] = 0;
	}
	else
	{
		if (libuna_utf16_string_copy_from_utf8(
			utf16_string,
			utf16_string_size,
			single_file_source->ip_address,
			single_file_source->ip_address_size,
			error) != 1)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_RUNTIME,
				LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				"%s: unable to copy ip_address to UTF-16 string.",
				function);

			return(-1);
		}
	}
	return(1);
}

/* Retrieves the static_ip value
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_static_ip(
	libewf_single_file_source_t *single_file_source,
	uint8_t *static_ip,
	libcerror_error_t **error)
{
	static char *function = "libewf_single_file_source_get_static_ip";

	if (single_file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid single file source.",
			function);

		return(-1);
	}
	if (static_ip == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid static_ip value.",
			function);

		return(-1);
	}

	*static_ip = single_file_source->static_ip;

	return(1);
}

/* Retrieves the size of the UTF-8 encoded mac_address
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_utf8_mac_address_size(
	libewf_single_file_source_t *single_file_source,
	size_t *utf8_string_size,
	libcerror_error_t **error)
{
	static char *function = "libewf_single_file_source_get_utf8_mac_address_size";

	if (single_file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid single file source.",
			function);

		return(-1);
	}
	if (utf8_string_size == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid UTF-8 string size.",
			function);

		return(-1);
	}
	*utf8_string_size = single_file_source->mac_address_size;

	return(1);
}

/* Retrieves the UTF-8 encoded mac_address value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_utf8_mac_address(
	libewf_single_file_source_t *single_file_source,
	uint8_t *utf8_string,
	size_t utf8_string_size,
	libcerror_error_t **error)
{
	static char *function = "libewf_single_file_source_get_utf8_mac_address";

	if (single_file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid single file source.",
			function);

		return(-1);
	}
	if (utf8_string == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid UTF-8 string.",
			function);

		return(-1);
	}
	if (utf8_string_size == 0)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			"%s: invalid UTF-8 string size value too small.",
			function);

		return(-1);
	}
	if (utf8_string_size > (size_t)SSIZE_MAX)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			"%s: invalid UTF-8 string size value exceeds maximum.",
			function);

		return(-1);
	}
	if ((single_file_source->mac_address == NULL)
		|| (single_file_source->mac_address_size == 0))
	{
		utf8_string[0] = 0;
	}
	else
	{
		if (utf8_string_size < single_file_source->mac_address_size)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				"%s: invalid UTF-8 string size value too small.",
				function);

			return(-1);
		}
		if (narrow_string_copy(
			(char *)utf8_string,
			(char *)single_file_source->mac_address,
			single_file_source->mac_address_size) == NULL)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_MEMORY,
				LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				"%s: unable to set UTF-8 string.",
				function);

			return(-1);
		}
		utf8_string[single_file_source->mac_address_size - 1] = 0;
	}
	return(1);
}

/* Retrieves the size of the UTF-16 encoded mac_address
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_utf16_mac_address_size(
	libewf_single_file_source_t *single_file_source,
	size_t *utf16_string_size,
	libcerror_error_t **error)
{
	static char *function = "libewf_single_file_source_get_utf16_mac_address_size";

	if (single_file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid single file source.",
			function);

		return(-1);
	}
	if ((single_file_source->mac_address == NULL)
		|| (single_file_source->mac_address_size == 0))
	{
		if (utf16_string_size == NULL)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
				"%s: invalid UTF-16 string size.",
				function);

			return(-1);
		}
		*utf16_string_size = 0;
	}
	else
	{
		if (libuna_utf16_string_size_from_utf8(
			single_file_source->mac_address,
			single_file_source->mac_address_size,
			utf16_string_size,
			error) != 1)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_RUNTIME,
				LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				"%s: unable to retrieve UTF-16 string size.",
				function);

			return(-1);
		}
	}
	return(1);
}

/* Retrieves the UTF-16 encoded mac_address value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_source_get_utf16_mac_address(
	libewf_single_file_source_t *single_file_source,
	uint16_t *utf16_string,
	size_t utf16_string_size,
	libcerror_error_t **error)
{
	static char *function = "libewf_single_file_source_get_utf16_mac_address";

	if (single_file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid single file source.",
			function);

		return(-1);
	}
	if (utf16_string == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid UTF-16 string.",
			function);

		return(-1);
	}
	if (utf16_string_size == 0)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			"%s: invalid UTF-16 string size value too small.",
			function);

		return(-1);
	}
	if (utf16_string_size > (size_t)SSIZE_MAX)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			"%s: invalid UTF-16 string size value exceeds maximum.",
			function);

		return(-1);
	}
	if ((single_file_source->mac_address == NULL)
		|| (single_file_source->mac_address_size == 0))
	{
		utf16_string[0] = 0;
	}
	else
	{
		if (libuna_utf16_string_copy_from_utf8(
			utf16_string,
			utf16_string_size,
			single_file_source->mac_address,
			single_file_source->mac_address_size,
			error) != 1)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_RUNTIME,
				LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				"%s: unable to copy mac_address to UTF-16 string.",
				function);

			return(-1);
		}
	}
	return(1);
}

