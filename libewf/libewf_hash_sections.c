/*
 * Hash sections functions
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
#include <narrow_string.h>

#include "libewf_libcerror.h"
#include "libewf_hash_sections.h"
#include "libewf_hash_values.h"

/* Creates hash sections
 * Make sure the value hash_sections is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_sections_initialize(
     libewf_hash_sections_t **hash_sections,
     libcerror_error_t **error )
{
	static char *function = "libewf_hash_sections_initialize";

	if( hash_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash sections.",
		 function );

		return( -1 );
	}
	if( *hash_sections != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid hash sections value already set.",
		 function );

		return( -1 );
	}
	*hash_sections = memory_allocate_structure(
	                  libewf_hash_sections_t );

	if( *hash_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create hash sections.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *hash_sections,
	     0,
	     sizeof( libewf_hash_sections_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear hash sections.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *hash_sections != NULL )
	{
		memory_free(
		 *hash_sections );

		*hash_sections = NULL;
	}
	return( -1 );
}

/* Frees hash sections
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_sections_free(
     libewf_hash_sections_t **hash_sections,
     libcerror_error_t **error )
{
        static char *function = "libewf_hash_sections_free";

	if( hash_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash sections.",
		 function );

		return( -1 );
	}
	if( *hash_sections != NULL )
	{
		if( ( *hash_sections )->xhash != NULL )
		{
			memory_free(
			 ( *hash_sections )->xhash );
		}
		memory_free(
		 *hash_sections );

		*hash_sections = NULL;
	}
	return( 1 );
}

/* Clones the hash sections
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_sections_clone(
     libewf_hash_sections_t **destination_hash_sections,
     libewf_hash_sections_t *source_hash_sections,
     libcerror_error_t **error )
{
	static char *function = "libewf_hash_sections_clone";

	if( destination_hash_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination hash sections.",
		 function );

		return( -1 );
	}
	if( *destination_hash_sections != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination hash sections already set.",
		 function );

		return( -1 );
	}
	if( source_hash_sections == NULL )
	{
		*destination_hash_sections = NULL;

		return( 1 );
	}
	*destination_hash_sections = memory_allocate_structure(
	                              libewf_hash_sections_t );

	if( *destination_hash_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination hash sections.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_hash_sections,
	     source_hash_sections,
	     sizeof( libewf_hash_sections_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination hash sections.",
		 function );

		memory_free(
		 *destination_hash_sections );

		*destination_hash_sections = NULL;

		return( -1 );
	}
	( *destination_hash_sections )->xhash      = NULL;
	( *destination_hash_sections )->xhash_size = 0;

	if( source_hash_sections->xhash != NULL )
	{
		( *destination_hash_sections )->xhash = (uint8_t *) memory_allocate(
		                                                     sizeof( uint8_t ) * source_hash_sections->xhash_size );

		if( ( *destination_hash_sections )->xhash == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create destination xhash.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     ( *destination_hash_sections )->xhash,
		     source_hash_sections->xhash,
		     sizeof( uint8_t ) * source_hash_sections->xhash_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy source to destination xhash.",
			 function );

			goto on_error;
		}
		( *destination_hash_sections )->xhash_size = source_hash_sections->xhash_size;
	}
	return( 1 );

on_error:
	if( *destination_hash_sections != NULL )
	{
		if( ( *destination_hash_sections )->xhash != NULL )
		{
			memory_free(
			 ( *destination_hash_sections )->xhash );
		}
		memory_free(
		 *destination_hash_sections );

		*destination_hash_sections = NULL;
	}
	return( -1 );
}

/* Sets the digest specified by the identifier from the hash values
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_sections_set_digest_from_hash_values(
     libewf_hash_sections_t *hash_sections,
     const uint8_t *identifier,
     size_t identifier_length,
     libfvalue_table_t *hash_values,
     libcerror_error_t **error )
{
	static char *function = "libewf_hash_sections_set_digest_from_hash_values";

	if( hash_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash sections.",
		 function );

		return( -1 );
	}
	if( ( identifier_length == 3 )
	 && ( narrow_string_compare(
	       (char *) identifier,
	       "MD5",
	       identifier_length ) == 0 ) )
	{
		if( libewf_hash_values_generate_md5_hash(
		     hash_values,
		     hash_sections->md5_hash,
		     16,
		     &( hash_sections->md5_hash_set ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to parse MD5 hash value for its value.",
			 function );

			return( -1 );
		}
		if( libewf_hash_values_generate_md5_hash(
		     hash_values,
		     hash_sections->md5_digest,
		     16,
		     &( hash_sections->md5_digest_set ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to parse MD5 hash value for its value.",
			 function );

			return( -1 );
		}
	}
	else if( ( identifier_length == 4 )
	      && ( narrow_string_compare(
		    (char *) identifier,
		    "SHA1",
		    identifier_length ) == 0 ) )
	{
		if( libewf_hash_values_generate_sha1_hash(
		     hash_values,
		     hash_sections->sha1_hash,
		     20,
		     &( hash_sections->sha1_hash_set ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to parse SHA1 hash value for its value.",
			 function );

			return( -1 );
		}
		if( libewf_hash_values_generate_sha1_hash(
		     hash_values,
		     hash_sections->sha1_digest,
		     20,
		     &( hash_sections->sha1_digest_set ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to parse SHA1 hash value for its value.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

