/*
 * Single file entry functions
 *
 * Copyright (C) 2006-2017, Joachim Metz <joachim.metz@gmail.com>
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
#include <narrow_string.h>
#include <types.h>

#include "libewf_libcerror.h"
#include "libewf_libfvalue.h"
#include "libewf_libuna.h"
#include "libewf_single_file_entry.h"

/* Creates a single file entry
 * Make sure the value single_file_entry is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_initialize(
     libewf_single_file_entry_t **single_file_entry,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_initialize";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
		 function );

		return( -1 );
	}
	if( *single_file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid single file entry value already set.",
		 function );

		return( -1 );
	}
	*single_file_entry = memory_allocate_structure(
	                      libewf_single_file_entry_t );

	if( *single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create single file entry.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *single_file_entry,
	     0,
	     sizeof( libewf_single_file_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear single file entry.",
		 function );

		goto on_error;
	}
	( *single_file_entry )->data_offset           = -1;
	( *single_file_entry )->duplicate_data_offset = -1;

	return( 1 );

on_error:
	if( *single_file_entry != NULL )
	{
		memory_free(
		 *single_file_entry );

		*single_file_entry = NULL;
	}
	return( -1 );
}

/* Frees a single file entry
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_free(
     libewf_single_file_entry_t **single_file_entry,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_free";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
		 function );

		return( -1 );
	}
	if( *single_file_entry != NULL )
	{
		if( ( *single_file_entry )->name != NULL )
		{
			memory_free(
			 ( *single_file_entry )->name );
		}
		if( ( *single_file_entry )->md5_hash != NULL )
		{
			memory_free(
			 ( *single_file_entry )->md5_hash );
		}
		if( ( *single_file_entry )->sha1_hash != NULL )
		{
			memory_free(
			 ( *single_file_entry )->sha1_hash );
		}
		memory_free(
		 *single_file_entry );

		*single_file_entry = NULL;
	}
	return( 1 );
}

/* Clones the single file entry
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_clone(
     libewf_single_file_entry_t **destination_single_file_entry,
     libewf_single_file_entry_t *source_single_file_entry,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_clone";

	if( destination_single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination single file entry.",
		 function );

		return( -1 );
	}
	if( *destination_single_file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination single file entry value already set.",
		 function );

		return( -1 );
	}
	if( source_single_file_entry == NULL )
	{
		*destination_single_file_entry = NULL;

		return( 1 );
	}
	*destination_single_file_entry = memory_allocate_structure(
			                  libewf_single_file_entry_t );

	if( *destination_single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination single file entry.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_single_file_entry,
	     source_single_file_entry,
	     sizeof( libewf_single_file_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination single file entry.",
		 function );

		memory_free(
		 *destination_single_file_entry );

		*destination_single_file_entry = NULL;

		return( -1 );
	}
	( *destination_single_file_entry )->name      = NULL;
	( *destination_single_file_entry )->md5_hash  = NULL;
	( *destination_single_file_entry )->sha1_hash = NULL;

	if( source_single_file_entry->name != NULL )
	{
		( *destination_single_file_entry )->name = (uint8_t *) memory_allocate(
		                                                        sizeof( uint8_t ) * source_single_file_entry->name_size );

		if( ( *destination_single_file_entry )->name == NULL )
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
		     ( *destination_single_file_entry )->name,
		     source_single_file_entry->name,
		     source_single_file_entry->name_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy source to destination name.",
			 function );

			goto on_error;
		}
		( *destination_single_file_entry )->name_size = source_single_file_entry->name_size;
	}
	if( source_single_file_entry->md5_hash != NULL )
	{
		( *destination_single_file_entry )->md5_hash = (uint8_t *) memory_allocate(
		                                                            sizeof( uint8_t ) * source_single_file_entry->md5_hash_size );

		if( ( *destination_single_file_entry )->md5_hash == NULL )
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
		     ( *destination_single_file_entry )->md5_hash,
		     source_single_file_entry->md5_hash,
		     source_single_file_entry->md5_hash_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy source to destination MD5 hash.",
			 function );

			goto on_error;
		}
		( *destination_single_file_entry )->md5_hash_size = source_single_file_entry->md5_hash_size;
	}
	if( source_single_file_entry->sha1_hash != NULL )
	{
		( *destination_single_file_entry )->sha1_hash = (uint8_t *) memory_allocate(
		                                                             sizeof( uint8_t ) * source_single_file_entry->sha1_hash_size );

		if( ( *destination_single_file_entry )->sha1_hash == NULL )
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
		     ( *destination_single_file_entry )->sha1_hash,
		     source_single_file_entry->sha1_hash,
		     source_single_file_entry->sha1_hash_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy source to destination SHA1 hash.",
			 function );

			goto on_error;
		}
		( *destination_single_file_entry )->sha1_hash_size = source_single_file_entry->sha1_hash_size;
	}
	return( 1 );

on_error:
	if( *destination_single_file_entry != NULL )
	{
		if( ( *destination_single_file_entry )->sha1_hash != NULL )
		{
			memory_free(
			 ( *destination_single_file_entry )->sha1_hash );
		}
		if( ( *destination_single_file_entry )->md5_hash != NULL )
		{
			memory_free(
			 ( *destination_single_file_entry )->md5_hash );
		}
		if( ( *destination_single_file_entry )->name != NULL )
		{
			memory_free(
			 ( *destination_single_file_entry )->name );
		}
		memory_free(
		 *destination_single_file_entry );

		*destination_single_file_entry = NULL;
	}
	return( -1 );
}

/* Retrieves the type
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_get_type(
     libewf_single_file_entry_t *single_file_entry,
     uint8_t *type,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_type";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
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
	*type = single_file_entry->type;

	return( 1 );
}

/* Retrieves the flags
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_get_flags(
     libewf_single_file_entry_t *single_file_entry,
     uint32_t *flags,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_flags";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
		 function );

		return( -1 );
	}
	if( flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid flags.",
		 function );

		return( -1 );
	}
	*flags = single_file_entry->flags;

	return( 1 );
}

/* Retrieves the data offset
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_get_data_offset(
     libewf_single_file_entry_t *single_file_entry,
     off64_t *data_offset,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_data_offset";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
		 function );

		return( -1 );
	}
	if( data_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data offset.",
		 function );

		return( -1 );
	}
	*data_offset = single_file_entry->data_offset;

	return( 1 );
}

/* Retrieves the data size
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_get_data_size(
     libewf_single_file_entry_t *single_file_entry,
     size64_t *data_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_data_size";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
		 function );

		return( -1 );
	}
	if( data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data size.",
		 function );

		return( -1 );
	}
	*data_size = single_file_entry->data_size;

	return( 1 );
}

/* Retrieves the duplicate data offset
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_get_duplicate_data_offset(
     libewf_single_file_entry_t *single_file_entry,
     off64_t *duplicate_data_offset,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_duplicate_data_offset";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
		 function );

		return( -1 );
	}
	if( duplicate_data_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid duplicate data offset.",
		 function );

		return( -1 );
	}
	*duplicate_data_offset = single_file_entry->duplicate_data_offset;

	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_get_utf8_name_size(
     libewf_single_file_entry_t *single_file_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_utf8_name_size";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
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
	*utf8_string_size = single_file_entry->name_size;

	return( 1 );
}

/* Retrieves the UTF-8 encoded name value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_get_utf8_name(
     libewf_single_file_entry_t *single_file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_utf8_name";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
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
	if( ( single_file_entry->name == NULL )
	 || ( single_file_entry->name_size == 0 ) )
	{
		utf8_string[ 0 ] = 0;
	}
	else
	{
		if( utf8_string_size < single_file_entry->name_size )
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
		     (char *) single_file_entry->name,
		     single_file_entry->name_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set UTF-8 string.",
			 function );

			return( -1 );
		}
		utf8_string[ single_file_entry->name_size - 1 ] = 0;
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_get_utf16_name_size(
     libewf_single_file_entry_t *single_file_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_utf16_name_size";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
		 function );

		return( -1 );
	}
	if( ( single_file_entry->name == NULL )
	 || ( single_file_entry->name_size == 0 ) )
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
		     single_file_entry->name,
		     single_file_entry->name_size,
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
int libewf_single_file_entry_get_utf16_name(
     libewf_single_file_entry_t *single_file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_utf16_name";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
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
	if( ( single_file_entry->name == NULL )
	 || ( single_file_entry->name_size == 0 ) )
	{
		utf16_string[ 0 ] = 0;
	}
	else
	{
		if( libuna_utf16_string_copy_from_utf8(
		     utf16_string,
		     utf16_string_size,
		     single_file_entry->name,
		     single_file_entry->name_size,
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

/* Retrieves the size
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_get_size(
     libewf_single_file_entry_t *single_file_entry,
     size64_t *size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_size";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
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
	*size = single_file_entry->size;

	return( 1 );
}

/* Retrieves the creation date and time
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_get_creation_time(
     libewf_single_file_entry_t *single_file_entry,
     uint32_t *creation_time,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_creation_time";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
		 function );

		return( -1 );
	}
	if( creation_time == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid creation time.",
		 function );

		return( -1 );
	}
	*creation_time = single_file_entry->creation_time;

	return( 1 );
}

/* Retrieves the (file) modification (last written) date and time
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_get_modification_time(
     libewf_single_file_entry_t *single_file_entry,
     uint32_t *modification_time,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_modification_time";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
		 function );

		return( -1 );
	}
	if( modification_time == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid modification time.",
		 function );

		return( -1 );
	}
	*modification_time = single_file_entry->modification_time;

	return( 1 );
}

/* Retrieves the access date and time
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_get_access_time(
     libewf_single_file_entry_t *single_file_entry,
     uint32_t *access_time,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_access_time";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
		 function );

		return( -1 );
	}
	if( access_time == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid access time.",
		 function );

		return( -1 );
	}
	*access_time = single_file_entry->access_time;

	return( 1 );
}

/* Retrieves the (file system entry) modification date and time
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_get_entry_modification_time(
     libewf_single_file_entry_t *single_file_entry,
     uint32_t *entry_modification_time,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_entry_modification_time";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
		 function );

		return( -1 );
	}
	if( entry_modification_time == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry modification time.",
		 function );

		return( -1 );
	}
	*entry_modification_time = single_file_entry->entry_modification_time;

	return( 1 );
}

/* Retrieves the UTF-8 encoded MD5 hash value
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_single_file_entry_get_utf8_hash_value_md5(
     libewf_single_file_entry_t *single_file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_utf8_hash_value_md5";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
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
	if( utf8_string_size < single_file_entry->md5_hash_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-8 string too small.",
		 function );

		return( -1 );
	}
	if( single_file_entry->md5_hash_size == 0 )
	{
		return( 0 );
	}
	if( memory_copy(
	     utf8_string,
	     single_file_entry->md5_hash,
	     single_file_entry->md5_hash_size ) == NULL )
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
int libewf_single_file_entry_get_utf16_hash_value_md5(
     libewf_single_file_entry_t *single_file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_utf16_hash_value_md5";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
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
	if( utf16_string_size < single_file_entry->md5_hash_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-16 string too small.",
		 function );

		return( -1 );
	}
	if( single_file_entry->md5_hash_size == 0 )
	{
		return( 0 );
	}
	if( libuna_utf16_string_copy_from_utf8(
	     utf16_string,
	     utf16_string_size,
	     single_file_entry->md5_hash,
	     single_file_entry->md5_hash_size,
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
int libewf_single_file_entry_get_utf8_hash_value_sha1(
     libewf_single_file_entry_t *single_file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_utf8_hash_value_sha1";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
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
	if( utf8_string_size < single_file_entry->sha1_hash_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-8 string too small.",
		 function );

		return( -1 );
	}
	if( single_file_entry->sha1_hash_size == 0 )
	{
		return( 0 );
	}
	if( memory_copy(
	     utf8_string,
	     single_file_entry->sha1_hash,
	     single_file_entry->sha1_hash_size ) == NULL )
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
int libewf_single_file_entry_get_utf16_hash_value_sha1(
     libewf_single_file_entry_t *single_file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_utf16_hash_value_sha1";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
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
	if( utf16_string_size < single_file_entry->sha1_hash_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-16 string too small.",
		 function );

		return( -1 );
	}
	if( single_file_entry->sha1_hash_size == 0 )
	{
		return( 0 );
	}
	if( libuna_utf16_string_copy_from_utf8(
	     utf16_string,
	     utf16_string_size,
	     single_file_entry->sha1_hash,
	     single_file_entry->sha1_hash_size,
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

