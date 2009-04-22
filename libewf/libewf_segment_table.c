/*
 * Segment table functions
 *
 * Copyright (c) 2006-2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
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

#include "libewf_definitions.h"
#include "libewf_filename.h"
#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_notify.h"
#include "libewf_segment_file.h"
#include "libewf_segment_table.h"
#include "libewf_system_string.h"

#include "ewf_data.h"

/* Initialize the hash sections
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_table_initialize(
     libewf_segment_table_t **segment_table,
     uint16_t amount,
     size64_t maximum_segment_size,
     liberror_error_t **error )
{
	static char *function     = "libewf_segment_table_initialize";
	size_t segment_table_size = 0;

	if( segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( amount == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid amount value cannot be zero.",
		 function );

		return( -1 );
	}
	if( *segment_table == NULL )
	{
		segment_table_size = sizeof( libewf_segment_file_handle_t * ) * amount;

		if( segment_table_size > (size_t) SSIZE_MAX )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid segment table size value exceeds maximum.",
			 function );

			return( -1 );
		}
		*segment_table = (libewf_segment_table_t *) memory_allocate(
		                                             sizeof( libewf_segment_table_t ) );

		if( *segment_table == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create segment table.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *segment_table,
		     0,
		     sizeof( libewf_segment_table_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear segment table.",
			 function );

			memory_free(
			 *segment_table );

			*segment_table = NULL;

			return( -1 );
		}
		( *segment_table )->segment_file_handle = (libewf_segment_file_handle_t **) memory_allocate(
		                                                                             segment_table_size );

		if( ( *segment_table )->segment_file_handle == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create segment file array.",
			 function );

			memory_free(
			 segment_table );

			*segment_table = NULL;

			return( -1 );
		}
		if( memory_set(
		     ( *segment_table )->segment_file_handle,
		     0,
		     segment_table_size ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear segment file array.",
			 function );

			memory_free(
			 ( *segment_table )->segment_file_handle );
			memory_free(
			 *segment_table );

			*segment_table = NULL;

			return( -1 );
		}
		( *segment_table )->amount               = amount;
		( *segment_table )->maximum_segment_size = maximum_segment_size;
	}
	return( 1 );
}

/* Frees the hash sections including elements
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_table_free(
     libewf_segment_table_t **segment_table,
     liberror_error_t **error )
{
	static char *function           = "libewf_segment_table_free";
	uint16_t segment_table_iterator = 0;

	if( segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( *segment_table != NULL )
	{
		for( segment_table_iterator = 0; segment_table_iterator < ( *segment_table )->amount; segment_table_iterator++ )
		{
			if( libewf_segment_file_handle_free(
			     &( ( *segment_table )->segment_file_handle[ segment_table_iterator ] ),
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free segment file handle: %" PRIu16 ".",
				 function,
				 segment_table_iterator + 1 );
			}
		}
		memory_free(
		 ( *segment_table )->segment_file_handle );

		if( ( *segment_table )->basename != NULL )
		{
			memory_free(
			 ( *segment_table )->basename );
		}
		memory_free(
		 *segment_table );

		*segment_table = NULL;
	}
	return( 1 );
}

/* Resizes the segment table
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_table_resize(
     libewf_segment_table_t *segment_table,
     uint16_t amount,
     liberror_error_t **error )
{
	void *reallocation        = NULL;
	static char *function     = "libewf_segment_table_resize";
	size_t segment_table_size = 0;

	if( segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( segment_table->amount < amount )
	{
		segment_table_size = sizeof( libewf_segment_file_handle_t * ) * amount;

		if( segment_table_size > (size_t) SSIZE_MAX )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid segment table size value exceeds maximum.",
			 function );

			return( -1 );
		}
		reallocation = memory_reallocate(
				segment_table->segment_file_handle,
				segment_table_size );

		if( reallocation == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to resize file handle array.",
			 function );

			return( -1 );
		}
		segment_table->segment_file_handle = (libewf_segment_file_handle_t **) reallocation;

		if( memory_set(
		     &( segment_table->segment_file_handle[ segment_table->amount ] ),
		     0,
		     ( ( amount - segment_table->amount ) * sizeof( libewf_segment_file_handle_t * ) ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear file handle array.",
			 function );

			return( 1 );
		}
		segment_table->amount = amount;
	}
	return( 1 );
}

/* Builds the segment table from all segment files
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libewf_segment_table_build(
     libewf_segment_table_t *segment_table,
     libewf_io_handle_t *io_handle,
     libewf_header_sections_t *header_sections,
     libewf_hash_sections_t *hash_sections,
     libewf_media_values_t *media_values,
     libewf_offset_table_t *offset_table,
     libewf_sector_table_t *sessions,
     libewf_sector_table_t *acquiry_errors,
     int *abort,
     liberror_error_t **error )
{
	static char *function   = "libewf_segment_table_build";
	uint16_t segment_number = 0;
	int last_segment_file   = 0;
	int result              = 0;

	if( segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( segment_table->segment_file_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment table - missing segment file handles.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid io handle.",
		 function );

		return( -1 );
	}
	/* If there are more than one segment files use the size of the 
	 * first as tehe maximum segment size
	 */
	if( segment_table->amount > 2 )
	{
		if( libbfio_pool_get_size(
		     io_handle->file_io_pool,
		     segment_table->segment_file_handle[ 1 ]->file_io_pool_entry,
		     &( segment_table->maximum_segment_size ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve segment file size.",
			 function );

			return( -1 );
		}
		/* Round size to nearest KiB
		 */
		segment_table->maximum_segment_size = ( segment_table->maximum_segment_size / 1024 ) * 1024;
	}
	/* Read the segment and offset table from the segment file(s)
	 */
	for( segment_number = 1; segment_number < segment_table->amount; segment_number++ )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		libewf_notify_verbose_printf(
		 "%s: reading section list for segment number: %" PRIu16 ".\n",
		 function,
		 segment_number );
#endif

		result = libewf_segment_file_read_sections(
		          segment_table->segment_file_handle[ segment_number ],
		          io_handle,
		          &last_segment_file,
		          header_sections,
		          hash_sections,
		          media_values,
		          offset_table,
		          sessions,
		          acquiry_errors,
		          error );

		if( result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read sections.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_INPUT,
			 LIBERROR_INPUT_ERROR_MISSING_LAST_SECTION,
			 "%s: missing next or done section.",
			 function );

			return( 0 );
		}
		if( *abort == 1 )
		{
			break;
		}
	}
	/* Check to see if the done section has been found in the last segment file
	 */
	if( last_segment_file != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_INPUT,
		 LIBERROR_INPUT_ERROR_MISSING_LAST_SEGMENT_FILE,
		 "%s: unable to find the last segment file.",
		 function );

		return( 0 );
	}
	return( 1 );
}

/* Retrieves the size of the basename in the segment table
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_segment_table_get_basename_size(
     libewf_segment_table_t *segment_table,
     size_t *basename_size,
     liberror_error_t **error )
{
	static char *function = "libewf_segment_table_get_basename_size";

	if( segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( basename_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid basename size.",
		 function );

		return( -1 );
	}
	if( segment_table->basename == NULL )
	{
		return( 0 );
	}
#if defined( LIBEWF_WIDE_SYSTEM_CHARACTER_TYPE )
	if( narrow_string_size_from_libewf_system_string(
	     segment_table->basename,
	     segment_table->basename_size,
	     basename_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine basename size.",
		 function );

		return( -1 );
	}
#else
	*basename_size = segment_table->basename_size;
#endif

	return( 1 );
}

/* Retrieves the basename in the segment table
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_segment_table_get_basename(
     libewf_segment_table_t *segment_table,
     char *basename,
     size_t basename_size,
     liberror_error_t **error )
{
	static char *function       = "libewf_segment_table_get_basename";
	size_t narrow_basename_size = 0;

	if( segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( basename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid basename.",
		 function );

		return( -1 );
	}
	if( segment_table->basename == NULL )
	{
		return( 0 );
	}
#if defined( LIBEWF_WIDE_SYSTEM_CHARACTER_TYPE )
	if( narrow_string_size_from_libewf_system_string(
	     segment_table->basename,
	     segment_table->basename_size,
	     &narrow_basename_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine basename size.",
		 function );

		return( -1 );
	}
#else
	narrow_basename_size = segment_table->basename_size;
#endif
	if( basename_size < narrow_basename_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: basename too small.",
		 function );

		return( -1 );
	}
#if defined( LIBEWF_WIDE_SYSTEM_CHARACTER_TYPE )
	if( narrow_string_copy_from_libewf_system_string(
	     basename,
	     basename_size,
	     segment_table->basename,
	     segment_table->basename_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set basename.",
		 function );

		return( -1 );
	}
#else
	if( libewf_system_string_copy(
	     basename,
	     segment_table->basename,
	     segment_table->basename_size ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set basename.",
		 function );

		return( -1 );
	}
	basename[ segment_table->basename_size - 1 ] = 0;
#endif
	return( 1 );
}

/* Sets the basename in the segment table
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_table_set_basename(
     libewf_segment_table_t *segment_table,
     const char *basename,
     size_t basename_size,
     liberror_error_t **error )
{
	static char *function = "libewf_segment_table_set_basename";

	if( segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( basename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid basename.",
		 function );

		return( -1 );
	}
	if( segment_table->basename != NULL )
	{
		memory_free(
		 segment_table->basename );

		segment_table->basename      = NULL;
		segment_table->basename_size = 0;
	}
#if defined( LIBEWF_WIDE_SYSTEM_CHARACTER_TYPE )
	if( libewf_system_string_size_from_narrow_string(
	     basename,
	     basename_size,
	     &( segment_table->basename_size ),
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine basename size.",
		 function );

		return( -1 );
	}
#else
	segment_table->basename_size = basename_size;
#endif
	segment_table->basename = (libewf_system_character_t *) memory_allocate(
	                                                         sizeof( libewf_system_character_t ) * segment_table->basename_size );

	if( segment_table->basename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create basename.",
		 function );

		segment_table->basename_size = 0;

		return( -1 );
	}
#if defined( LIBEWF_WIDE_SYSTEM_CHARACTER_TYPE )
	if( libewf_system_string_copy_from_narrow_string(
	     segment_table->basename,
	     segment_table->basename_size,
	     basename,
	     basename_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set basename.",
		 function );

		memory_free(
		 segment_table->basename );

		segment_table->basename      = NULL;
		segment_table->basename_size = 0;

		return( -1 );
	}
#else
	if( libewf_system_string_copy(
	     segment_table->basename,
	     basename,
	     basename_size ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set basename.",
		 function );

		memory_free(
		 segment_table->basename );

		segment_table->basename      = NULL;
		segment_table->basename_size = 0;

		return( -1 );
	}
	segment_table->basename[ basename_size - 1 ] = 0;
#endif
	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )
/* Retrieves the size of the basename in the segment table
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_segment_table_get_basename_size_wide(
     libewf_segment_table_t *segment_table,
     size_t *basename_size,
     liberror_error_t **error )
{
	static char *function = "libewf_segment_table_get_basename_size_wide";

	if( segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( basename_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid basename size.",
		 function );

		return( -1 );
	}
	if( segment_table->basename == NULL )
	{
		return( 0 );
	}
#if defined( LIBEWF_WIDE_SYSTEM_CHARACTER_TYPE )
	*basename_size = segment_table->basename_size;
#else
	if( wide_string_size_from_libewf_system_string(
	     segment_table->basename,
	     segment_table->basename_size,
	     basename_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine basename size.",
		 function );

		return( -1 );
	}
#endif

	return( 1 );
}

/* Retrieves the basename in the segment table
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_segment_table_get_basename_wide(
     libewf_segment_table_t *segment_table,
     wchar_t *basename,
     size_t basename_size,
     liberror_error_t **error )
{
	static char *function     = "libewf_segment_table_get_basename_wide";
	size_t wide_basename_size = 0;

	if( segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( basename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid basename.",
		 function );

		return( -1 );
	}
	if( segment_table->basename == NULL )
	{
		return( 0 );
	}
#if defined( LIBEWF_WIDE_SYSTEM_CHARACTER_TYPE )
	wide_basename_size = segment_table->basename_size;
#else
	if( wide_string_size_from_libewf_system_string(
	     segment_table->basename,
	     segment_table->basename_size,
	     &wide_basename_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine basename size.",
		 function );

		return( -1 );
	}
#endif
	if( basename_size < wide_basename_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: basename too small.",
		 function );

		return( -1 );
	}
#if defined( LIBEWF_WIDE_SYSTEM_CHARACTER_TYPE )
	if( libewf_system_string_copy(
	     basename,
	     segment_table->basename,
	     segment_table->basename_size ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set basename.",
		 function );

		return( -1 );
	}
	basename[ segment_table->basename_size - 1 ] = 0;
#else
	if( wide_string_copy_from_libewf_system_string(
	     basename,
	     basename_size,
	     segment_table->basename,
	     segment_table->basename_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set basename.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Sets the basename in the segment table
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_table_set_basename_wide(
     libewf_segment_table_t *segment_table,
     const wchar_t *basename,
     size_t basename_size,
     liberror_error_t **error )
{
	static char *function = "libewf_segment_table_set_basename_wide";

	if( segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( basename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid basename.",
		 function );

		return( -1 );
	}
	if( segment_table->basename != NULL )
	{
		memory_free(
		 segment_table->basename );

		segment_table->basename      = NULL;
		segment_table->basename_size = 0;
	}
#if defined( LIBEWF_WIDE_SYSTEM_CHARACTER_TYPE )
	segment_table->basename_size = basename_size;
#else
	if( libewf_system_string_size_from_wide_string(
	     basename,
	     basename_size,
	     &( segment_table->basename_size ),
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine basename size.",
		 function );

		return( -1 );
	}
#endif
	segment_table->basename = (libewf_system_character_t *) memory_allocate(
	                                                         sizeof( libewf_system_character_t ) * segment_table->basename_size );


	if( segment_table->basename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create basename.",
		 function );

		return( -1 );
	}
#if defined( LIBEWF_WIDE_SYSTEM_CHARACTER_TYPE )
	if( libewf_system_string_copy(
	     segment_table->basename,
	     basename,
	     basename_size ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set basename.",
		 function );

		memory_free(
		 segment_table->basename );

		segment_table->basename      = NULL;
		segment_table->basename_size = 0;

		return( -1 );
	}
	segment_table->basename[ basename_size - 1 ] = 0;
#else
	if( libewf_system_string_copy_from_wide_string(
	     segment_table->basename,
	     segment_table->basename_size,
	     basename,
	     basename_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set basename.",
		 function );

		memory_free(
		 segment_table->basename );

		segment_table->basename      = NULL;
		segment_table->basename_size = 0;

		return( -1 );
	}
#endif
	return( 1 );
}
#endif

/* Creates a new segment file and opens it for writing
 * The necessary sections at the start of the segment file are written
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_table_create_segment_file(
     libewf_segment_table_t *segment_table,
     uint16_t segment_number,
     libewf_io_handle_t *io_handle,
     int16_t maximum_amount_of_segments,
     uint8_t segment_file_type,
     liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle    = NULL;
	libewf_system_character_t *filename = NULL;
	static char *function               = "libewf_segment_table_create_segment_file";
	size_t filename_size                = 0;
	int file_io_pool_entry              = 0;
	int flags                           = 0;

	if( segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( segment_table->segment_file_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment table - missing segment file handles.",
		 function );

		return( -1 );
	}
	if( segment_number == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid segment number: 0.",
		 function );

		return( -1 );
	}
	if( segment_number > segment_table->amount )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_LARGE,
		 "%s: invalid segment number value out of range.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid io handle.",
		 function );

		return( -1 );
	}
	/* Check if one additional entries in the segment table are needed
	 */
	if( segment_number >= segment_table->amount )
	{
		/* Add one additional entry because the 0 entry is used for the basename
		 */
		if( libewf_segment_table_resize(
		     segment_table,
		     ( segment_number + 1 ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize segment table.",
			 function );

			return( -1 );
		}
	}
	/* Check if the entry has already been filled
	 */
	if( segment_table->segment_file_handle[ segment_number ] != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: segment file has already been created.",
		 function );

		return( -1 );
	}
	if( libewf_filename_create(
	     &filename,
	     &filename_size,
	     segment_table->basename,
	     segment_table->basename_size - 1,
	     segment_number,
	     maximum_amount_of_segments,
	     segment_file_type,
	     io_handle->format,
	     io_handle->ewf_format,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create segment file filename.",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: filename is empty.",
		 function );

		return( -1 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	libewf_notify_verbose_printf(
	 "%s: creating segment file: %" PRIu16 " with filename: %" PRIs_LIBEWF_SYSTEM ".\n",
	 function,
	 segment_number,
	 filename );
#endif

	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file io handle.",
		 function );

		memory_free(
		 filename );

		return( -1 );
	}
#if defined( LIBEWF_WIDE_SYSTEM_CHARACTER_TYPE )
	if( libbfio_file_set_name_wide(
	     file_io_handle,
	     filename,
	     filename_size,
	     error ) != 1 )
#else
	if( libbfio_file_set_name(
	     file_io_handle,
	     filename,
	     filename_size,
	     error ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set filename in file io handle.",
		 function );

		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
		memory_free(
		 filename );

		return( -1 );
	}
	memory_free(
	 filename );

	if( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_DWF )
	{
		flags = LIBBFIO_OPEN_READ_WRITE_TRUNCATE;
	}
	else
	{
		flags = LIBBFIO_OPEN_WRITE_TRUNCATE;
	}
	if( libbfio_pool_add_handle(
	     io_handle->file_io_pool,
	     &file_io_pool_entry,
	     file_io_handle,
	     flags,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to add file io handle to pool.",
		 function );

		libbfio_handle_free(
		 &file_io_handle,
		 NULL );

		return( -1 );
	}
	if( libbfio_pool_open(
	     io_handle->file_io_pool,
	     file_io_pool_entry,
	     flags,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open segment file: %" PRIu16 ".",
		 function,
		 segment_number );

		return( -1 );
	}
	if( libewf_segment_file_handle_initialize(
	     &( segment_table->segment_file_handle[ segment_number ] ),
	     file_io_pool_entry,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create segment file handle.",
		 function );

		return( -1 );
	}
	segment_table->segment_file_handle[ segment_number ]->write_open = 1;

	return( 1 );
}

/* Corrects sections after streamed write
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_table_write_sections_corrections(
     libewf_segment_table_t *segment_table,
     libewf_io_handle_t *io_handle,
     uint32_t last_segment_amount_of_chunks,
     libewf_media_values_t *media_values,
     libewf_values_table_t *hash_values,
     libewf_hash_sections_t *hash_sections,
     libewf_sector_table_t *sessions,
     libewf_sector_table_t *acquiry_errors,
     ewf_data_t **cached_data_section,
     liberror_error_t **error )
{
	static char *function           = "libewf_segment_table_write_sections_corrections";
	uint16_t segment_table_iterator = 0;
	int last_segment_file           = 0;

	if( segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( segment_table->segment_file_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment table - missing segment file handles.",
		 function );

		return( -1 );
	}
	for( segment_table_iterator = 1; segment_table_iterator < segment_table->amount; segment_table_iterator++ )
	{
		if( segment_table_iterator == ( segment_table->amount - 1 ) )
		{
			last_segment_file = 1;
		}
		if( libewf_segment_file_write_sections_correction(
		     segment_table->segment_file_handle[ segment_table_iterator ],
		     io_handle,
		     segment_table_iterator,
		     last_segment_amount_of_chunks,
		     last_segment_file,
		     media_values,
		     hash_values,
		     hash_sections,
		     sessions,
		     acquiry_errors,
		     cached_data_section,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write sections correction to segment file: %" PRIu16 ".",
			 function,
			 segment_table_iterator );

			return( -1 );
		}
	}
	return( 1 );
}

