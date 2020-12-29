/*
 * Segment file reading/writing functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#if defined( TIME_WITH_SYS_TIME )
#include <sys/time.h>
#include <time.h>
#elif defined( HAVE_SYS_TIME_H )
#include <sys/time.h>
#else
#include <time.h>
#endif

#include "libewf_case_data.h"
#include "libewf_chunk_data.h"
#include "libewf_debug.h"
#include "libewf_definitions.h"
#include "libewf_device_information.h"
#include "libewf_digest_section.h"
#include "libewf_error2_section.h"
#include "libewf_hash_values.h"
#include "libewf_header_values.h"
#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_libcdata.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libfcache.h"
#include "libewf_libfdata.h"
#include "libewf_libfguid.h"
#include "libewf_libfvalue.h"
#include "libewf_md5_hash_section.h"
#include "libewf_section.h"
#include "libewf_section_descriptor.h"
#include "libewf_segment_file.h"
#include "libewf_segment_table.h"
#include "libewf_session_section.h"
#include "libewf_sha1_hash_section.h"
#include "libewf_single_files.h"
#include "libewf_table_section.h"
#include "libewf_unused.h"
#include "libewf_volume_section.h"

#include "ewf_file_header.h"
#include "ewf_section.h"
#include "ewf_volume.h"

const uint8_t ewf1_dvf_file_signature[ 8 ] = { 0x64, 0x76, 0x66, 0x09, 0x0d, 0x0a, 0xff, 0x00 };
const uint8_t ewf1_evf_file_signature[ 8 ] = { 0x45, 0x56, 0x46, 0x09, 0x0d, 0x0a, 0xff, 0x00 };
const uint8_t ewf1_lvf_file_signature[ 8 ] = { 0x4c, 0x56, 0x46, 0x09, 0x0d, 0x0a, 0xff, 0x00 };
const uint8_t ewf2_evf_file_signature[ 8 ] = { 0x45, 0x56, 0x46, 0x32, 0x0d, 0x0a, 0x81, 0x00 };
const uint8_t ewf2_lef_file_signature[ 8 ] = { 0x4c, 0x45, 0x46, 0x32, 0x0d, 0x0a, 0x81, 0x00 };

/* Creates a segment file
 * Make sure the value segment_file is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_file_initialize(
     libewf_segment_file_t **segment_file,
     libewf_io_handle_t *io_handle,
     libcerror_error_t **error )
{
	static char *function = "libewf_segment_file_initialize";

	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( *segment_file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid segment file value already set.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	*segment_file = memory_allocate_structure(
	                 libewf_segment_file_t );

	if( *segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create segment file.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *segment_file,
	     0,
	     sizeof( libewf_segment_file_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear segment file.",
		 function );

		memory_free(
		 *segment_file );

		*segment_file = NULL;

		return( -1 );
	}
	if( libfdata_list_initialize(
	     &( ( *segment_file )->sections_list ),
	     (intptr_t *) *segment_file,
	     NULL,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, libfdata_list_element_t *, libfdata_cache_t *, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libewf_segment_file_read_section_element_data,
	     NULL,
	     LIBFDATA_DATA_HANDLE_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sections list.",
		 function );

		goto on_error;
	}
	if( libfdata_list_initialize(
	     &( ( *segment_file )->chunk_groups_list ),
	     (intptr_t *) *segment_file,
	     NULL,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, libfdata_list_element_t *, libfdata_cache_t *, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libewf_segment_file_read_chunk_group_element_data,
	     NULL,
	     LIBFDATA_DATA_HANDLE_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create chunk groups list.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &( ( *segment_file )->chunk_groups_cache ),
	     LIBEWF_MAXIMUM_CACHE_ENTRIES_CHUNK_GROUPS,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create chunk groups cache.",
		 function );

		goto on_error;
	}
	( *segment_file )->io_handle                        = io_handle;
	( *segment_file )->device_information_section_index = -1;
	( *segment_file )->previous_last_chunk_filled       = -1;
	( *segment_file )->last_chunk_filled                = -1;
	( *segment_file )->last_chunk_compared              = -1;

	return( 1 );

on_error:
	if( *segment_file != NULL )
	{
		if( ( *segment_file )->chunk_groups_list != NULL )
		{
			libfdata_list_free(
			 &( ( *segment_file )->chunk_groups_list ),
			 NULL );
		}
		if( ( *segment_file )->sections_list != NULL )
		{
			libfdata_list_free(
			 &( ( *segment_file )->sections_list ),
			 NULL );
		}
		memory_free(
		 *segment_file );

		*segment_file = NULL;
	}
	return( -1 );
}

/* Frees a segment file
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_file_free(
     libewf_segment_file_t **segment_file,
     libcerror_error_t **error )
{
	static char *function = "libewf_segment_file_free";
	int result            = 1;

	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( *segment_file != NULL )
	{
		/* The io_handle reference is freed elsewhere
		 */
		if( libfdata_list_free(
		     &( ( *segment_file )->sections_list ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free sections list.",
			 function );

			result = -1;
		}
		if( libfdata_list_free(
		     &( ( *segment_file )->chunk_groups_list ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free chunk groups list.",
			 function );

			result = -1;
		}
		if( libfcache_cache_free(
		     &( ( *segment_file )->chunk_groups_cache ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free chunk groups cache.",
			 function );

			result = -1;
		}
		memory_free(
		 *segment_file );

		*segment_file = NULL;
	}
	return( result );
}

/* Clones the segment file
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_file_clone(
     libewf_segment_file_t **destination_segment_file,
     libewf_segment_file_t *source_segment_file,
     libcerror_error_t **error )
{
	static char *function       = "libewf_segment_file_clone";
	size64_t list_element_size  = 0;
	size64_t mapped_size        = 0;
	off64_t list_element_offset = 0;
	uint32_t list_element_flags = 0;
	int element_index           = 0;
	int list_element_file_index = 0;
	int list_element_index      = 0;
	int number_of_list_elements = 0;

	if( destination_segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination segment file.",
		 function );

		return( -1 );
	}
	if( *destination_segment_file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination segment file value already set.",
		 function );

		return( -1 );
	}
	if( source_segment_file == NULL )
	{
		*destination_segment_file = NULL;

		return( 1 );
	}
	*destination_segment_file = memory_allocate_structure(
	                             libewf_segment_file_t );

	if( *destination_segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination segment file.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_segment_file,
	     source_segment_file,
	     sizeof( libewf_segment_file_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination segment file.",
		 function );

		memory_free(
		 *destination_segment_file );

		*destination_segment_file = NULL;

		return( -1 );
	}
	( *destination_segment_file )->sections_list             = NULL;
	( *destination_segment_file )->chunk_groups_list         = NULL;
	( *destination_segment_file )->chunk_groups_cache        = NULL;
	( *destination_segment_file )->current_chunk_group_index = 0;

	if( libfdata_list_initialize(
	     &( ( *destination_segment_file )->sections_list ),
	     (intptr_t *) *destination_segment_file,
	     NULL,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, libfdata_list_element_t *, libfdata_cache_t *, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libewf_segment_file_read_section_element_data,
	     NULL,
	     LIBFDATA_DATA_HANDLE_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination sections list.",
		 function );

		goto on_error;
	}
	if( libfdata_list_get_number_of_elements(
	     source_segment_file->sections_list,
	     &number_of_list_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the number of elements from source sections list.",
		 function );

		goto on_error;
	}
	for( list_element_index = 0;
	     list_element_index < number_of_list_elements;
	     list_element_index++ )
	{
		if( libfdata_list_get_element_by_index(
		     source_segment_file->sections_list,
		     list_element_index,
		     &list_element_file_index,
		     &list_element_offset,
		     &list_element_size,
		     &list_element_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve element: %d from source sections list.",
			 function,
			 list_element_index );

			goto on_error;
		}
		if( libfdata_list_append_element(
		     ( *destination_segment_file )->sections_list,
		     &element_index,
		     list_element_file_index,
		     list_element_offset,
		     list_element_size,
		     list_element_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append element: %d to destination sections list.",
			 function,
			 list_element_index );

			goto on_error;
		}
	}
	if( libfdata_list_initialize(
	     &( ( *destination_segment_file )->chunk_groups_list ),
	     (intptr_t *) *destination_segment_file,
	     NULL,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, libfdata_list_element_t *, libfdata_cache_t *, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libewf_segment_file_read_chunk_group_element_data,
	     NULL,
	     LIBFDATA_DATA_HANDLE_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination chunk groups list.",
		 function );

		goto on_error;
	}
	if( libfdata_list_get_number_of_elements(
	     source_segment_file->chunk_groups_list,
	     &number_of_list_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the number of elements from source chunk groups list.",
		 function );

		goto on_error;
	}
	for( list_element_index = 0;
	     list_element_index < number_of_list_elements;
	     list_element_index++ )
	{
		if( libfdata_list_get_element_by_index(
		     source_segment_file->chunk_groups_list,
		     list_element_index,
		     &list_element_file_index,
		     &list_element_offset,
		     &list_element_size,
		     &list_element_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve element: %d from source chunk groups list.",
			 function,
			 list_element_index );

			goto on_error;
		}
		if( libfdata_list_get_mapped_size_by_index(
		     source_segment_file->chunk_groups_list,
		     list_element_index,
		     &mapped_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve mappped size: %d from source chunk groups list.",
			 function,
			 list_element_index );

			goto on_error;
		}
		if( libfdata_list_append_element_with_mapped_size(
		     ( *destination_segment_file )->chunk_groups_list,
		     &element_index,
		     list_element_file_index,
		     list_element_offset,
		     list_element_size,
		     list_element_flags,
		     mapped_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append element: %d to destination chunk groups list.",
			 function,
			 list_element_index );

			goto on_error;
		}
	}
	if( libfcache_cache_clone(
	     &( ( *destination_segment_file )->chunk_groups_cache ),
	     source_segment_file->chunk_groups_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination chunk groups cache.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_segment_file != NULL )
	{
		if( ( *destination_segment_file )->chunk_groups_list != NULL )
		{
			libfdata_list_free(
			 &( ( *destination_segment_file )->chunk_groups_list ),
			 NULL );
		}
		if( ( *destination_segment_file )->sections_list != NULL )
		{
			libfdata_list_free(
			 &( ( *destination_segment_file )->sections_list ),
			 NULL );
		}
		memory_free(
		 *destination_segment_file );

		*destination_segment_file = NULL;
	}
	return( -1 );
}

/* Retrieves the number of segments
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_file_get_number_of_sections(
     libewf_segment_file_t *segment_file,
     int *number_of_sections,
     libcerror_error_t **error )
{
	static char *function = "libewf_segment_file_get_number_of_sections";

	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( libfdata_list_get_number_of_elements(
	     segment_file->sections_list,
	     number_of_sections,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the number of elements from sections list.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific section from the segment file
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_file_get_section_by_index(
     libewf_segment_file_t *segment_file,
     int section_index,
     libbfio_pool_t *file_io_pool,
     libfcache_cache_t *sections_cache,
     libewf_section_descriptor_t **section_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libewf_segment_file_get_section_by_index";

	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( libfdata_list_get_element_value_by_index(
	     segment_file->sections_list,
	     (intptr_t *) file_io_pool,
	     (libfdata_cache_t *) sections_cache,
	     section_index,
	     (intptr_t **) section_descriptor,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve element: %d from sections list.",
		 function,
		 section_index );

		return( -1 );
	}
	return( 1 );
}

/* Reads the segment file header
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_file_read_file_header_data(
     libewf_segment_file_t *segment_file,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_segment_file_read_file_header_data";

#if defined( HAVE_DEBUG_OUTPUT )
	uint16_t value_16bit  = 0;
#endif

	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing data.",
		 function );

		return( -1 );
	}
	if( ( data_size != sizeof( ewf_file_header_v1_t ) )
	 && ( data_size != sizeof( ewf_file_header_v2_t ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: file header:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 0 );
	}
#endif
	if( data_size == sizeof( ewf_file_header_v1_t ) )
	{
		byte_stream_copy_to_uint16_little_endian(
		 ( (ewf_file_header_v1_t *) data )->segment_number,
		 segment_file->segment_number );

		segment_file->major_version      = 1;
		segment_file->minor_version      = 0;
		segment_file->compression_method = LIBEWF_COMPRESSION_METHOD_DEFLATE;
	}
	else if( data_size == sizeof( ewf_file_header_v2_t ) )
	{
		segment_file->major_version = ( (ewf_file_header_v2_t *) data )->major_version;
		segment_file->minor_version = ( (ewf_file_header_v2_t *) data )->minor_version;

		byte_stream_copy_to_uint16_little_endian(
		 ( (ewf_file_header_v2_t *) data )->compression_method,
		 segment_file->compression_method );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_file_header_v2_t *) data )->segment_number,
		 segment_file->segment_number );

		if( memory_copy(
		     segment_file->set_identifier,
		     ( (ewf_file_header_v2_t *) data )->set_identifier,
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy set identifier.",
			 function );

			return( -1 );
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature:\n",
		 function );
		libcnotify_print_data(
		 data,
		 8,
		 0 );

		if( data_size == sizeof( ewf_file_header_v1_t ) )
		{
			libcnotify_printf(
			 "%s: fields start\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (ewf_file_header_v1_t *) data )->fields_start );
		}
		else if( data_size == sizeof( ewf_file_header_v2_t ) )
		{
			libcnotify_printf(
			 "%s: major version\t\t: %" PRIu8 "\n",
			 function,
			 segment_file->major_version );

			libcnotify_printf(
			 "%s: minor version\t\t: %" PRIu8 "\n",
			 function,
			 segment_file->minor_version );

			libcnotify_printf(
			 "%s: compression method\t\t: %" PRIu16 " (",
			 function,
			 segment_file->compression_method );
			libewf_debug_print_compression_method(
			 segment_file->compression_method );
			libcnotify_printf(
			 ")\n" );
		}
		libcnotify_printf(
		 "%s: segment number\t\t: %" PRIu32 "\n",
		 function,
		 segment_file->segment_number );

		if( data_size == sizeof( ewf_file_header_v1_t ) )
		{
			byte_stream_copy_to_uint16_little_endian(
			 ( (ewf_file_header_v1_t *) data )->fields_end,
			 value_16bit );
			libcnotify_printf(
			 "%s: fields end\t\t\t: 0x%04" PRIx16 "\n",
			 function,
			 value_16bit );
		}
		else if( data_size == sizeof( ewf_file_header_v2_t ) )
		{
			if( libewf_debug_print_guid_value(
			     function,
			     "set identifier\t\t",
			     segment_file->set_identifier,
			     16,
			     LIBFGUID_ENDIAN_LITTLE,
			     LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print GUID value.",
				 function );

				return( -1 );
			}
		}
		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

/* Reads the segment file header
 * Returns the number of bytes read if successful, or -1 on error
 */
ssize_t libewf_segment_file_read_file_header_file_io_pool(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libcerror_error_t **error )
{
	uint8_t file_header_data[ sizeof( ewf_file_header_v2_t ) ];

	static char *function        = "libewf_segment_file_read_file_header_file_io_pool";
	size_t file_header_data_size = 0;
	ssize_t read_count           = 0;

	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading file header at offset: 0 (0x00000000)\n",
		 function );
	}
#endif
	read_count = libbfio_pool_read_buffer_at_offset(
	              file_io_pool,
	              file_io_pool_entry,
	              file_header_data,
	              8,
	              0,
	              error );

	if( read_count != (ssize_t) 8 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header signature at offset 0 (0x00000000).",
		 function );

		return( -1 );
	}
	segment_file->current_offset = read_count;

	if( memory_compare(
	     ewf1_evf_file_signature,
	     file_header_data,
	     8 ) == 0 )
	{
		segment_file->type    = LIBEWF_SEGMENT_FILE_TYPE_EWF1;
		file_header_data_size = sizeof( ewf_file_header_v1_t );
	}
	else if( memory_compare(
	          ewf1_lvf_file_signature,
	          file_header_data,
	          8 ) == 0 )
	{
		segment_file->type    = LIBEWF_SEGMENT_FILE_TYPE_EWF1_LOGICAL;
		file_header_data_size = sizeof( ewf_file_header_v1_t );
	}
	else if( memory_compare(
	          ewf2_evf_file_signature,
	          file_header_data,
	          8 ) == 0 )
	{
		segment_file->type    = LIBEWF_SEGMENT_FILE_TYPE_EWF2;
		file_header_data_size = sizeof( ewf_file_header_v2_t );
	}
	else if( memory_compare(
	          ewf2_lef_file_signature,
	          file_header_data,
	          8 ) == 0 )
	{
		segment_file->type    = LIBEWF_SEGMENT_FILE_TYPE_EWF2_LOGICAL;
		file_header_data_size = sizeof( ewf_file_header_v2_t );
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_SIGNATURE_MISMATCH,
		 "%s: unsupported file header signature.",
		 function );

		return( -1 );
	}
	read_count = libbfio_pool_read_buffer(
	              file_io_pool,
	              file_io_pool_entry,
	              &( file_header_data[ 8 ] ),
	              file_header_data_size - 8,
	              error );

	if( read_count != (ssize_t) ( file_header_data_size - 8 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header data.",
		 function );

		return( -1 );
	}
	segment_file->current_offset += read_count;

	if( libewf_segment_file_read_file_header_data(
	     segment_file,
	     file_header_data,
	     file_header_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header.",
		 function );

		return( -1 );
	}
	return( (ssize_t) file_header_data_size );
}

/* Writes the segment file header
 * Returns the number of bytes written if successful, or -1 on error
 */
ssize_t libewf_segment_file_write_file_header(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libcerror_error_t **error )
{
	uint8_t *file_header_data     = NULL;
	static char *function         = "libewf_segment_file_write_file_header";
	const uint8_t *file_signature = NULL;
	size_t file_header_data_size  = 0;
	ssize_t write_count           = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint16_t value_16bit          = 0;
#endif

	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( segment_file->major_version == 1 )
	{
		file_header_data_size = sizeof( ewf_file_header_v1_t );
	}
	else if( segment_file->major_version == 2 )
	{
		file_header_data_size = sizeof( ewf_file_header_v2_t );
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format version.",
		 function );

		return( -1 );
	}
	if( segment_file->segment_number == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid segment file - segment number value out of bounds.",
		 function );

		return( -1 );
	}
	if( segment_file->major_version == 1 )
	{
		if( ( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1 )
		 || ( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART ) )
		{
			file_signature = ewf1_evf_file_signature;
		}
		else if( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_LOGICAL )
		{
			file_signature = ewf1_lvf_file_signature;
		}
	}
	else if( segment_file->major_version == 2 )
	{
		if( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF2 )
		{
			file_signature = ewf2_evf_file_signature;
		}
		else if( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF2_LOGICAL )
		{
			file_signature = ewf2_lef_file_signature;
		}
	}
	if( file_signature == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported segment file type.",
		 function );

		goto on_error;
	}
	file_header_data = (uint8_t *) memory_allocate(
	                                file_header_data_size );

	if( file_header_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file header data.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     file_header_data,
	     0,
	     file_header_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file header data.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     file_header_data,
	     0,
	     file_header_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file header data.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     file_header_data,
	     file_signature,
	     8 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy file signature.",
		 function );

		goto on_error;
	}
	if( segment_file->major_version == 1 )
	{
		( (ewf_file_header_v1_t *) file_header_data )->fields_start = 1;

		byte_stream_copy_from_uint16_little_endian(
		 ( (ewf_file_header_v1_t *) file_header_data )->segment_number,
		 segment_file->segment_number );
	}
	else if( segment_file->major_version == 2 )
	{
		( (ewf_file_header_v2_t *) file_header_data )->major_version = segment_file->major_version;
		( (ewf_file_header_v2_t *) file_header_data )->minor_version = segment_file->minor_version;

		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_file_header_v2_t *) file_header_data )->segment_number,
		 segment_file->segment_number );

		byte_stream_copy_from_uint16_little_endian(
		 ( (ewf_file_header_v2_t *) file_header_data )->compression_method,
		 segment_file->compression_method );

		if( memory_copy(
		     ( (ewf_file_header_v2_t *) file_header_data )->set_identifier,
		     segment_file->set_identifier,
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy set identifier.",
			 function );

			goto on_error;
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: file header:\n",
		 function );
		libcnotify_print_data(
		 file_header_data,
		 file_header_data_size,
		 0 );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature:\n",
		 function );
		libcnotify_print_data(
		 file_header_data,
		 8,
		 0 );

		if( segment_file->major_version == 1 )
		{
			libcnotify_printf(
			 "%s: fields start\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (ewf_file_header_v1_t *) file_header_data )->fields_start );
		}
		else if( segment_file->major_version == 2 )
		{
			libcnotify_printf(
			 "%s: major version\t\t: %" PRIu8 "\n",
			 function,
			 segment_file->major_version );

			libcnotify_printf(
			 "%s: minor version\t\t: %" PRIu8 "\n",
			 function,
			 segment_file->minor_version );

			libcnotify_printf(
			 "%s: compression method\t\t: %" PRIu16 " (",
			 function,
			 segment_file->compression_method );
			libewf_debug_print_compression_method(
			 segment_file->compression_method );
			libcnotify_printf(
			 ")\n" );
		}
		libcnotify_printf(
		 "%s: segment number\t\t\t: %" PRIu32 "\n",
		 function,
		 segment_file->segment_number );

		if( segment_file->major_version == 1 )
		{
			byte_stream_copy_to_uint16_little_endian(
			 ( (ewf_file_header_v1_t *) file_header_data )->fields_end,
			 value_16bit );
			libcnotify_printf(
			 "%s: fields end\t\t\t: 0x%04" PRIx16 "\n",
			 function,
			 value_16bit );
		}
		else if( segment_file->major_version == 2 )
		{
			if( libewf_debug_print_guid_value(
			     function,
			     "set identifier\t\t",
			     segment_file->set_identifier,
			     16,
			     LIBFGUID_ENDIAN_LITTLE,
			     LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print GUID value.",
				 function );

				return( -1 );
			}
		}
		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	write_count = libbfio_pool_write_buffer(
	               file_io_pool,
	               file_io_pool_entry,
	               file_header_data,
	               file_header_data_size,
	               error );

	if( write_count != (ssize_t) file_header_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write file header.",
		 function );

		goto on_error;
	}
	segment_file->current_offset += write_count;

	memory_free(
	 file_header_data );

	return( write_count );

on_error:
	if( file_header_data != NULL )
	{
		memory_free(
		 file_header_data );
	}
	return( -1 );
}

/* Seeks the offset in the segment file
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libewf_segment_file_seek_offset(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t offset,
         libcerror_error_t **error )
{
	static char *function = "libewf_segment_file_seek_offset";

	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( libbfio_pool_seek_offset(
	     file_io_pool,
	     file_io_pool_entry,
	     offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to seek section data offset: %" PRIi64 ".",
		 function,
		 offset );

		return( -1 );
	}
	segment_file->current_offset = offset;

	return( offset );
}

/* Reads the table section
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_segment_file_read_table_section(
         libewf_segment_file_t *segment_file,
         libewf_section_descriptor_t *section_descriptor,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         size32_t chunk_size,
         libcerror_error_t **error )
{
	libewf_table_section_t *table_section = NULL;
	static char *function                 = "libewf_segment_file_read_table_section";
	size64_t chunk_group_data_size        = 0;
	size64_t storage_media_size           = 0;
	ssize_t read_count                    = 0;
	off64_t chunk_group_data_offset       = 0;
	uint32_t range_flags                  = 0;

	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( segment_file->chunk_groups_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment file - missing chunk groups list.",
		 function );

		return( -1 );
	}
	if( section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( libewf_table_section_initialize(
	     &table_section,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create table section.",
		 function );

		goto on_error;
	}
	segment_file->previous_last_chunk_filled = segment_file->last_chunk_filled;

	read_count = libewf_table_section_read_file_io_pool(
	              table_section,
	              segment_file->io_handle,
	              file_io_pool,
	              file_io_pool_entry,
	              segment_file->major_version,
	              segment_file->type,
	              section_descriptor->data_size,
	              section_descriptor->data_flags,
	              error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read table section.",
		 function );

		goto on_error;
	}
	segment_file->current_offset += read_count;

	if( segment_file->major_version == 1 )
	{
		/* For EWF version 1 the entire table section is considered the group
		 * because the section descriptor is need to determine the chunk data
		 * offset and size values
		 */
		chunk_group_data_offset = section_descriptor->start_offset;
		chunk_group_data_size   = (size64_t) section_descriptor->size;
	}
	else if( segment_file->major_version == 2 )
	{
		/* For EWF version 2 the table (section data) is considered the group
		 */
		chunk_group_data_offset = section_descriptor->start_offset;
		chunk_group_data_size   = (size64_t) section_descriptor->data_size;

		if( segment_file->last_chunk_filled == -1 )
		{
			segment_file->last_chunk_filled = (int64_t) table_section->first_chunk_index;
		}
		else if( (int64_t) table_section->first_chunk_index != segment_file->last_chunk_filled )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: out of order first chunk number not yet supported.",
			 function );

			return( -1 );
		}
	}
	if( table_section->entries_corrupted != 0 )
	{
		segment_file->flags |= LIBEWF_SEGMENT_FILE_FLAG_IS_CORRUPTED;

		range_flags = LIBEWF_RANGE_FLAG_IS_TAINTED;
	}
	if( table_section->number_of_entries > 0 )
	{
		storage_media_size = (size64_t) chunk_size * table_section->number_of_entries;

		if( libfdata_list_append_element_with_mapped_size(
		     segment_file->chunk_groups_list,
		     &( segment_file->current_chunk_group_index ),
		     file_io_pool_entry,
		     chunk_group_data_offset,
		     chunk_group_data_size,
		     range_flags,
		     storage_media_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append element with mapped size to chunk groups list.",
			 function );

			goto on_error;
		}
		segment_file->storage_media_size += storage_media_size;
		segment_file->number_of_chunks   += (uint64_t) table_section->number_of_entries;
		segment_file->last_chunk_filled  += (int64_t) table_section->number_of_entries;
	}
	if( libewf_table_section_free(
	     &table_section,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free table section.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( table_section != NULL )
	{
		libewf_table_section_free(
		 &table_section,
		 NULL );
	}
	return( -1 );
}

/* Reads the table2 section
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_segment_file_read_table2_section(
         libewf_segment_file_t *segment_file,
         libewf_section_descriptor_t *section_descriptor,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libcerror_error_t **error )
{
	libewf_table_section_t *table_section = NULL;
	static char *function                 = "libewf_segment_file_read_table2_section";
	size64_t chunk_group_data_size        = 0;
	ssize_t read_count                    = 0;
	off64_t chunk_group_data_offset       = 0;
	int64_t chunk_group_number_of_entries = 0;
	uint32_t chunk_group_range_flags      = 0;
	uint8_t number_of_entries_mismatch    = 0;
	int chunk_group_file_io_pool_entry    = 0;
	int result                            = 0;

	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( segment_file->chunk_groups_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment file - missing chunk groups list.",
		 function );

		return( -1 );
	}
	if( segment_file->major_version != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid segment file - unsupported major version.",
		 function );

		return( -1 );
	}
	if( section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( segment_file->last_chunk_filled < segment_file->previous_last_chunk_filled )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chunk group - last chunk filled value out of bounds.",
		 function );

		return( -1 );
	}
	if( libewf_table_section_initialize(
	     &table_section,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create table section.",
		 function );

		goto on_error;
	}
	read_count = libewf_table_section_read_file_io_pool(
	              table_section,
	              segment_file->io_handle,
	              file_io_pool,
	              file_io_pool_entry,
	              segment_file->major_version,
	              segment_file->type,
	              section_descriptor->data_size,
	              section_descriptor->data_flags,
	              error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read table2 section.",
		 function );

		goto on_error;
	}
	segment_file->current_offset += read_count;

	chunk_group_number_of_entries = segment_file->last_chunk_filled
	                              - segment_file->previous_last_chunk_filled;

	if( (int64_t) table_section->number_of_entries != chunk_group_number_of_entries )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: mismatch between number of entries in table and table2\n.",
			 function );
		}
#endif
		if( ( chunk_group_range_flags & LIBEWF_RANGE_FLAG_IS_TAINTED ) == 0 )
		{
			number_of_entries_mismatch = 1;

			segment_file->flags |= LIBEWF_SEGMENT_FILE_FLAG_IS_CORRUPTED;
		}
	}
	if( libfdata_list_get_element_by_index(
	     segment_file->chunk_groups_list,
	     segment_file->current_chunk_group_index,
	     &chunk_group_file_io_pool_entry,
	     &chunk_group_data_offset,
	     &chunk_group_data_size,
	     &chunk_group_range_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve element: %d from chunk groups list.",
		 function,
		 segment_file->current_chunk_group_index );

		goto on_error;
	}
	result = 1;

	if( table_section->entries_corrupted != 0 )
	{
		segment_file->flags |= LIBEWF_SEGMENT_FILE_FLAG_IS_CORRUPTED;

		table_section->number_of_entries = chunk_group_number_of_entries;
	}
	else if( number_of_entries_mismatch != 0 )
	{
/* TODO improve to handle this corruption */
		if( chunk_group_number_of_entries != 0 )
		{
			/* If the number of entries in the table section are not 0 prefer the table section
			 */
			table_section->number_of_entries = chunk_group_number_of_entries;
		}
		else
		{
			/* If the number of entries in the table section are 0 prefer the table2 section
			 */
			chunk_group_range_flags = LIBEWF_RANGE_FLAG_IS_TAINTED;
		}
	}
	if( ( chunk_group_range_flags & LIBEWF_RANGE_FLAG_IS_TAINTED ) != 0 )
	{
		if( table_section->entries_corrupted == 0 )
		{
			segment_file->last_chunk_filled = segment_file->previous_last_chunk_filled + table_section->number_of_entries;

			/* For EWF version 1 the entire table2 section is considered the group
			 * because the section descriptor is need to determine the chunk data
			 * offset and size values
			 */
			chunk_group_data_offset = section_descriptor->start_offset;
			chunk_group_data_size   = (size64_t) section_descriptor->size;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: table section was corrupted using table2 section.\n",
				 function );
			}
#endif
			result = libfdata_list_set_element_by_index(
				  segment_file->chunk_groups_list,
				  segment_file->current_chunk_group_index,
				  file_io_pool_entry,
				  chunk_group_data_offset,
				  chunk_group_data_size,
				  0,
				  error );
		}
		else
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: both the table and table2 sections are corrupted.\n",
				 function );
			}
#endif
			result = libfdata_list_set_element_by_index(
				  segment_file->chunk_groups_list,
				  segment_file->current_chunk_group_index,
				  chunk_group_file_io_pool_entry,
				  chunk_group_data_offset,
				  chunk_group_data_size,
				  LIBEWF_RANGE_FLAG_IS_CORRUPTED,
				  error );
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to set element: %d in chunk groups list.",
			 function,
			 segment_file->current_chunk_group_index );

			goto on_error;
		}
	}
	segment_file->last_chunk_compared += (int64_t) table_section->number_of_entries;

	if( libewf_table_section_free(
	     &table_section,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free table section.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( table_section != NULL )
	{
		libewf_table_section_free(
		 &table_section,
		 NULL );
	}
	return( -1 );
}

/* Reads a volume section
 * Returns the number of bytes read if successful or -1 on error
 */
ssize_t libewf_segment_file_read_volume_section(
         libewf_segment_file_t *segment_file,
         libewf_section_descriptor_t *section_descriptor,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_media_values_t *media_values,
         libcerror_error_t **error )
{
	static char *function = "libewf_segment_file_read_volume_section";
	ssize_t read_count    = 0;

	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
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
	if( section_descriptor->data_size == (size64_t) sizeof( ewf_volume_t ) )
	{
		read_count = libewf_volume_section_e01_read_file_io_pool(
		              section_descriptor,
		              segment_file->io_handle,
		              file_io_pool,
		              file_io_pool_entry,
		              media_values,
		              error );

		if( read_count != (ssize_t) section_descriptor->data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read EWF-E01 volume section.",
			 function );

			return( -1 );
		}
	}
	else if( section_descriptor->data_size == (size64_t) sizeof( ewf_volume_smart_t ) )
	{
		read_count = libewf_volume_section_s01_read_file_io_pool(
		              section_descriptor,
		              segment_file->io_handle,
		              file_io_pool,
		              file_io_pool_entry,
		              media_values,
		              error );

		if( read_count != (ssize_t) section_descriptor->data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read EWF-S01 volume section.",
			 function );

			return( -1 );
		}
		segment_file->type = LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART;
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section size value out of bounds.",
		 function );

		return( -1 );
	}
	segment_file->current_offset += read_count;

	return( read_count );
}

/* Writes a device information section to file
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_device_information_section(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t **device_information,
         size_t *device_information_size,
         libewf_media_values_t *media_values,
         libfvalue_table_t *header_values,
         libcerror_error_t **error )
{
	libewf_section_descriptor_t *section_descriptor = NULL;
	static char *function                           = "libewf_segment_file_write_device_information_section";
	ssize_t write_count                             = 0;
	int element_index                               = 0;

	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( segment_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( device_information == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device information.",
		 function );

		return( -1 );
	}
	if( device_information_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device information size.",
		 function );

		return( -1 );
	}
	if( *device_information == NULL )
	{
		if( libewf_device_information_generate(
		     device_information,
		     device_information_size,
		     media_values,
		     header_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to generate device information.",
			 function );

			goto on_error;
		}
	}
	if( libewf_section_descriptor_initialize(
	     &section_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create section descriptor.",
		 function );

		goto on_error;
	}
	/* Do not include the end of string character in the compressed data
	 */
	write_count = libewf_section_write_compressed_string(
	               section_descriptor,
	               segment_file->io_handle,
	               file_io_pool,
	               file_io_pool_entry,
	               2,
	               LIBEWF_SECTION_TYPE_DEVICE_INFORMATION,
	               NULL,
	               0,
	               segment_file->current_offset,
	               segment_file->io_handle->compression_method,
	               LIBEWF_COMPRESSION_LEVEL_DEFAULT,
	               *device_information,
	               *device_information_size - 2,
	               0,
	               error );

	if( write_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write device information section.",
		 function );

		goto on_error;
	}
	segment_file->current_offset += write_count;

	if( libewf_section_descriptor_free(
	     &section_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free section.",
		 function );

		goto on_error;
	}
	if( libfdata_list_append_element(
	     segment_file->sections_list,
	     &element_index,
	     file_io_pool_entry,
	     segment_file->current_offset - sizeof( ewf_section_descriptor_v2_t ),
	     sizeof( ewf_section_descriptor_v2_t ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append element to sections list.",
		 function );

		goto on_error;
	}
	return( write_count );

on_error:
	if( section_descriptor != NULL )
	{
		libewf_section_descriptor_free(
		 &section_descriptor,
		 NULL );
	}
	return( -1 );
}

/* Writes a case data section to file
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_case_data_section(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t **case_data,
         size_t *case_data_size,
         libewf_media_values_t *media_values,
         libfvalue_table_t *header_values,
         time_t timestamp,
         libcerror_error_t **error )
{
	libewf_section_descriptor_t *section_descriptor = NULL;
	static char *function                           = "libewf_segment_file_write_case_data_section";
	ssize_t write_count                             = 0;
	int element_index                               = 0;

	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( segment_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( case_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid case data.",
		 function );

		return( -1 );
	}
	if( case_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid case data size.",
		 function );

		return( -1 );
	}
	if( *case_data == NULL )
	{
		if( libewf_case_data_generate(
		     case_data,
		     case_data_size,
		     media_values,
		     header_values,
		     timestamp,
		     segment_file->io_handle->format,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to generate case data.",
			 function );

			goto on_error;
		}
	}
	if( libewf_section_descriptor_initialize(
	     &section_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create section descriptor.",
		 function );

		goto on_error;
	}
	/* Do not include the end of string character in the compressed data
	 */
	write_count = libewf_section_write_compressed_string(
	               section_descriptor,
	               segment_file->io_handle,
	               file_io_pool,
	               file_io_pool_entry,
	               2,
	               LIBEWF_SECTION_TYPE_CASE_DATA,
	               NULL,
	               0,
	               segment_file->current_offset,
	               segment_file->io_handle->compression_method,
	               LIBEWF_COMPRESSION_LEVEL_DEFAULT,
	               *case_data,
	               *case_data_size - 2,
	               0,
	               error );

	if( write_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write case data section.",
		 function );

		goto on_error;
	}
	segment_file->current_offset += write_count;

	if( libewf_section_descriptor_free(
	     &section_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free section.",
		 function );

		goto on_error;
	}
	if( libfdata_list_append_element(
	     segment_file->sections_list,
	     &element_index,
	     file_io_pool_entry,
	     segment_file->current_offset - sizeof( ewf_section_descriptor_v2_t ),
	     sizeof( ewf_section_descriptor_v2_t ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append element to sections list.",
		 function );

		goto on_error;
	}
	return( write_count );

on_error:
	if( section_descriptor != NULL )
	{
		libewf_section_descriptor_free(
		 &section_descriptor,
		 NULL );
	}
	return( -1 );
}

/* Writes a header section to file
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_header_section(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_header_sections_t *header_sections,
         int8_t compression_level,
         libcerror_error_t **error )
{
	libewf_section_descriptor_t *section_descriptor = NULL;
	static char *function                           = "libewf_segment_file_write_header_section";
	ssize_t write_count                             = 0;
	int element_index                               = 0;

	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( segment_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( header_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header sections.",
		 function );

		return( -1 );
	}
	if( ( header_sections->header == NULL )
	 || ( header_sections->header_size == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid header sections - missing header.",
		 function );

		return( -1 );
	}
	if( libewf_section_descriptor_initialize(
	     &section_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create section descriptor.",
		 function );

		goto on_error;
	}
	/* Do not include the end of string character in the compressed data
	 */
	write_count = libewf_section_write_compressed_string(
	               section_descriptor,
	               segment_file->io_handle,
	               file_io_pool,
	               file_io_pool_entry,
	               1,
	               0,
	               (uint8_t *) "header",
	               6,
	               segment_file->current_offset,
	               segment_file->io_handle->compression_method,
	               compression_level,
	               header_sections->header,
	               header_sections->header_size - 1,
	               0,
	               error );

	if( write_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write header section.",
		 function );

		goto on_error;
	}
	if( libfdata_list_append_element(
	     segment_file->sections_list,
	     &element_index,
	     file_io_pool_entry,
	     segment_file->current_offset,
	     sizeof( ewf_section_descriptor_v1_t ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append element to sections list.",
		 function );

		goto on_error;
	}
	segment_file->current_offset += write_count;

	if( libewf_section_descriptor_free(
	     &section_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free section.",
		 function );

		goto on_error;
	}
	header_sections->number_of_header_sections += 1;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libewf_debug_byte_stream_print(
		     "Header",
		     header_sections->header,
		     header_sections->header_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print header.",
			 function );

			goto on_error;
		}
	}
#endif
	return( write_count );

on_error:
	if( section_descriptor != NULL )
	{
		libewf_section_descriptor_free(
		 &section_descriptor,
		 NULL );
	}
	return( -1 );
}

/* Writes a header2 section to file
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_header2_section(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_header_sections_t *header_sections,
         libcerror_error_t **error )
{
	libewf_section_descriptor_t *section_descriptor = NULL;
	static char *function                           = "libewf_segment_file_write_header2_section";
	ssize_t write_count                             = 0;
	int element_index                               = 0;

	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( segment_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( header_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header sections.",
		 function );

		return( -1 );
	}
	if( ( header_sections->header2 == NULL )
	 || ( header_sections->header2_size == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid header sections - missing header2.",
		 function );

		return( -1 );
	}
	if( libewf_section_descriptor_initialize(
	     &section_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create section descriptor.",
		 function );

		goto on_error;
	}
	/* Do not include the end of string character in the compressed data
	 */
	write_count = libewf_section_write_compressed_string(
	               section_descriptor,
	               segment_file->io_handle,
	               file_io_pool,
	               file_io_pool_entry,
	               1,
	               0,
	               (uint8_t *) "header2",
	               7,
	               segment_file->current_offset,
	               segment_file->io_handle->compression_method,
	               LIBEWF_COMPRESSION_LEVEL_DEFAULT,
	               header_sections->header2,
	               header_sections->header2_size - 2,
	               0,
	               error );

	if( write_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write header2 section.",
		 function );

		goto on_error;
	}
	if( libfdata_list_append_element(
	     segment_file->sections_list,
	     &element_index,
	     file_io_pool_entry,
	     segment_file->current_offset,
	     sizeof( ewf_section_descriptor_v1_t ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append element to sections list.",
		 function );

		goto on_error;
	}
	segment_file->current_offset += write_count;

	if( libewf_section_descriptor_free(
	     &section_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free section.",
		 function );

		goto on_error;
	}
	header_sections->number_of_header_sections += 1;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libewf_debug_utf16_stream_print(
		     "Header2",
		     header_sections->header2,
		     header_sections->header2_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print header2.",
			 function );

			goto on_error;
		}
	}
#endif
	return( write_count );

on_error:
	if( section_descriptor != NULL )
	{
		libewf_section_descriptor_free(
		 &section_descriptor,
		 NULL );
	}
	return( -1 );
}

/* Writes a xheader section to file
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_xheader_section(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_header_sections_t *header_sections,
         libcerror_error_t **error )
{
	libewf_section_descriptor_t *section_descriptor = NULL;
	static char *function                           = "libewf_segment_file_write_xheader_section";
	ssize_t write_count                             = 0;
	int element_index                               = 0;

	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( segment_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( header_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header sections.",
		 function );

		return( -1 );
	}
	if( ( header_sections->xheader == NULL )
	 || ( header_sections->xheader_size == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid header sections - missing xheader.",
		 function );

		return( -1 );
	}
	if( libewf_section_descriptor_initialize(
	     &section_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create section descriptor.",
		 function );

		goto on_error;
	}
	/* Do not include the end of string character in the compressed data
	 */
	write_count = libewf_section_write_compressed_string(
	               section_descriptor,
	               segment_file->io_handle,
	               file_io_pool,
	               file_io_pool_entry,
	               1,
	               0,
	               (uint8_t *) "xheader",
	               7,
	               segment_file->current_offset,
	               segment_file->io_handle->compression_method,
	               LIBEWF_COMPRESSION_LEVEL_DEFAULT,
	               header_sections->xheader,
	               header_sections->xheader_size - 1,
	               0,
	               error );

	if( write_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write xheader section.",
		 function );

		goto on_error;
	}
	if( libfdata_list_append_element(
	     segment_file->sections_list,
	     &element_index,
	     file_io_pool_entry,
	     segment_file->current_offset,
	     sizeof( ewf_section_descriptor_v1_t ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append element to sections list.",
		 function );

		goto on_error;
	}
	segment_file->current_offset += write_count;

	if( libewf_section_descriptor_free(
	     &section_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free section.",
		 function );

		goto on_error;
	}
	header_sections->number_of_header_sections += 1;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libewf_debug_utf8_stream_print(
		     "XHeader",
		     header_sections->xheader,
		     header_sections->xheader_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print xheader.",
			 function );

			goto on_error;
		}
	}
#endif
	return( write_count );

on_error:
	if( section_descriptor != NULL )
	{
		libewf_section_descriptor_free(
		 &section_descriptor,
		 NULL );
	}
	return( -1 );
}

/* Writes the header sections to file
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_header_sections(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libfvalue_table_t *header_values,
         time_t timestamp,
         libcerror_error_t **error )
{
	libewf_header_sections_t *header_sections = NULL;
	static char *function                     = "libewf_segment_file_write_header_sections";
	ssize_t write_count                       = 0;
	ssize_t total_write_count                 = 0;

	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( segment_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( libewf_header_sections_initialize(
	     &header_sections,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header sections.",
		 function );

		goto on_error;
	}
	if( libewf_header_sections_generate(
	     header_sections,
	     header_values,
	     timestamp,
	     segment_file->io_handle->compression_level,
	     segment_file->io_handle->format,
	     segment_file->io_handle->header_codepage,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header sections.",
		 function );

		goto on_error;
	}
	if( ( segment_file->io_handle->format == LIBEWF_FORMAT_EWF )
	 || ( segment_file->io_handle->format == LIBEWF_FORMAT_SMART )
	 || ( segment_file->io_handle->format == LIBEWF_FORMAT_ENCASE1 ) )
	{
		/* The header should be written only once
		 * and using the compression used in the file
		 */
		write_count = libewf_segment_file_write_header_section(
		               segment_file,
		               file_io_pool,
		               file_io_pool_entry,
		               header_sections,
		               segment_file->io_handle->compression_level,
		               error );

		if( write_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write single header section.",
			 function );

			goto on_error;
		}
		total_write_count += write_count;
	}
	else if( ( segment_file->io_handle->format == LIBEWF_FORMAT_ENCASE2 )
	      || ( segment_file->io_handle->format == LIBEWF_FORMAT_ENCASE3 )
	      || ( segment_file->io_handle->format == LIBEWF_FORMAT_FTK_IMAGER )
	      || ( segment_file->io_handle->format == LIBEWF_FORMAT_LINEN5 )
	      || ( segment_file->io_handle->format == LIBEWF_FORMAT_LINEN6 )
	      || ( segment_file->io_handle->format == LIBEWF_FORMAT_LINEN7 ) )
	{
		/* The header should be written twice
		 * the default compression is used
		 */
		write_count = libewf_segment_file_write_header_section(
		               segment_file,
		               file_io_pool,
		               file_io_pool_entry,
		               header_sections,
		               LIBEWF_COMPRESSION_LEVEL_DEFAULT,
		               error );

		if( write_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write first header section.",
			 function );

			goto on_error;
		}
		total_write_count += write_count;

		write_count = libewf_segment_file_write_header_section(
		               segment_file,
		               file_io_pool,
		               file_io_pool_entry,
		               header_sections,
		               LIBEWF_COMPRESSION_LEVEL_DEFAULT,
		               error );

		if( write_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write second header section.",
			 function );

			goto on_error;
		}
		total_write_count += write_count;
	}
	else if( ( segment_file->io_handle->format == LIBEWF_FORMAT_ENCASE4 )
	      || ( segment_file->io_handle->format == LIBEWF_FORMAT_ENCASE5 )
	      || ( segment_file->io_handle->format == LIBEWF_FORMAT_ENCASE6 )
	      || ( segment_file->io_handle->format == LIBEWF_FORMAT_ENCASE7 ) )
	{
		/* The header2 should be written twice
		 * the default compression is used
		 */
		write_count = libewf_segment_file_write_header2_section(
		               segment_file,
		               file_io_pool,
		               file_io_pool_entry,
		               header_sections,
		               error );

		if( write_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write first header2 section.",
			 function );

			goto on_error;
		}
		total_write_count += write_count;

		write_count = libewf_segment_file_write_header2_section(
		               segment_file,
		               file_io_pool,
		               file_io_pool_entry,
		               header_sections,
		               error );

		if( write_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write second header2 section.",
			 function );

			goto on_error;
		}
		total_write_count += write_count;

		/* The header should be written once
		 * the default compression is used
		 */
		write_count = libewf_segment_file_write_header_section(
		               segment_file,
		               file_io_pool,
		               file_io_pool_entry,
		               header_sections,
		               LIBEWF_COMPRESSION_LEVEL_DEFAULT,
		               error );

		if( write_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write third header section.",
			 function );

			goto on_error;
		}
		total_write_count += write_count;
	}
	/* EWFX uses the header and header2 for backwards compatibility
	 */
	else if( segment_file->io_handle->format == LIBEWF_FORMAT_EWFX )
	{
		/* The xheader should be written once
		 * the default compression is used
		 */
		write_count = libewf_segment_file_write_xheader_section(
		               segment_file,
		               file_io_pool,
		               file_io_pool_entry,
		               header_sections,
		               error );

		if( write_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write xheader section.",
			 function );

			goto on_error;
		}
		total_write_count += write_count;

		/* The header2 should be written once
		 * the default compression is used
		 */
		write_count = libewf_segment_file_write_header2_section(
		               segment_file,
		               file_io_pool,
		               file_io_pool_entry,
		               header_sections,
		               error );

		if( write_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write second header2 section.",
			 function );

			goto on_error;
		}
		total_write_count += write_count;

		/* The header should be written once
		 * the default compression is used
		 */
		write_count = libewf_segment_file_write_header_section(
		               segment_file,
		               file_io_pool,
		               file_io_pool_entry,
		               header_sections,
		               LIBEWF_COMPRESSION_LEVEL_DEFAULT,
		               error );

		if( write_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write third header section.",
			 function );

			goto on_error;
		}
		total_write_count += write_count;
	}
	if( libewf_header_sections_free(
	     &header_sections,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free header sections.",
		 function );

		goto on_error;
	}
	return( total_write_count );

on_error:
	if( header_sections != NULL )
	{
		libewf_header_sections_free(
		 &header_sections,
		 NULL );
	}
	return( -1 );
}

/* Write the last section at the end of the segment file
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_last_section(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         int last_segment_file,
         libcerror_error_t **error )
{
	libewf_section_descriptor_t *section_descriptor = NULL;
	uint8_t *section_type_string                    = NULL;
	static char *function                           = "libewf_segment_file_write_last_section";
	size_t section_descriptor_data_size             = 0;
	size_t section_size                             = 0;
	size_t section_type_string_length               = 0;
	ssize_t write_count                             = 0;
	uint32_t section_type                           = 0;
	int element_index                               = 0;

	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( ( segment_file->type != LIBEWF_SEGMENT_FILE_TYPE_EWF2 )
	 && ( segment_file->type != LIBEWF_SEGMENT_FILE_TYPE_EWF2_LOGICAL ) )
	{
		section_descriptor_data_size = sizeof( ewf_section_descriptor_v1_t );
	}
	else
	{
		section_descriptor_data_size = sizeof( ewf_section_descriptor_v2_t );
	}
	if( last_segment_file == 0 )
	{
		section_type               = LIBEWF_SECTION_TYPE_NEXT;
		section_type_string        = (uint8_t *) "next";
		section_type_string_length = 4;
	}
	else
	{
		section_type               = LIBEWF_SECTION_TYPE_DONE;
		section_type_string        = (uint8_t *) "done";
		section_type_string_length = 4;
	}
	/* The version 1 EWF-E01 and EWF-L01 formats leave the size of this section empty
	 */
	if( ( segment_file->type != LIBEWF_SEGMENT_FILE_TYPE_EWF1 )
	 && ( segment_file->type != LIBEWF_SEGMENT_FILE_TYPE_EWF1_LOGICAL ) )
	{
		section_size = section_descriptor_data_size;
	}
	/* Write next or done section
	 */
	if( libewf_section_descriptor_initialize(
	     &section_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create section descriptor.",
		 function );

		goto on_error;
	}
	if( libewf_section_descriptor_set(
	     section_descriptor,
	     section_type,
	     section_type_string,
	     section_type_string_length,
	     segment_file->current_offset,
	     section_size,
	     0,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set section descriptor.",
		 function );

		goto on_error;
	}
	write_count = libewf_section_descriptor_write_file_io_pool(
	               section_descriptor,
	               file_io_pool,
	               file_io_pool_entry,
	               segment_file->major_version,
	               error );

	if( write_count != (ssize_t) section_descriptor_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section descriptor data.",
		 function );

		goto on_error;
	}
	if( libfdata_list_append_element(
	     segment_file->sections_list,
	     &element_index,
	     file_io_pool_entry,
	     segment_file->current_offset,
	     (size64_t) write_count,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append element to sections list.",
		 function );

		goto on_error;
	}
	segment_file->current_offset += write_count;

	if( libewf_section_descriptor_free(
	     &section_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free section.",
		 function );

		goto on_error;
	}
	return( write_count );

on_error:
	if( section_descriptor != NULL )
	{
		libewf_section_descriptor_free(
		 &section_descriptor,
		 NULL );
	}
	return( -1 );
}

/* Write the necessary sections at the start of the segment file
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_start(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t **case_data,
         size_t *case_data_size,
         uint8_t **device_information,
         size_t *device_information_size,
         ewf_data_t **data_section_descriptor,
         libewf_media_values_t *media_values,
         libfvalue_table_t *header_values,
         time_t timestamp,
         libcerror_error_t **error )
{
	libewf_section_descriptor_t *section_descriptor = NULL;
	static char *function                           = "libewf_segment_file_write_start";
	ssize_t total_write_count                       = 0;
	ssize_t write_count                             = 0;
	int element_index                               = 0;

	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	write_count = libewf_segment_file_write_file_header(
	               segment_file,
	               file_io_pool,
	               file_io_pool_entry,
	               error );

	if( write_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write file header.",
		 function );

		goto on_error;
	}
	total_write_count += write_count;

	if( ( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1 )
	 || ( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART ) )
	{
		if( segment_file->segment_number == 1 )
		{
			write_count = libewf_segment_file_write_header_sections(
				       segment_file,
				       file_io_pool,
				       file_io_pool_entry,
				       header_values,
				       timestamp,
			               error );

			if( write_count == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write header sections.",
				 function );

				goto on_error;
			}
			total_write_count += write_count;
		}
		if( libewf_section_descriptor_initialize(
		     &section_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create section descriptor.",
			 function );

			goto on_error;
		}
		if( segment_file->segment_number == 1 )
		{
			write_count = -1;

			if( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1 )
			{
				write_count = libewf_volume_section_e01_write_file_io_pool(
					       section_descriptor,
					       segment_file->io_handle,
					       file_io_pool,
					       file_io_pool_entry,
					       segment_file->current_offset,
					       media_values,
				               error );
			}
			else if( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
			{
				write_count = libewf_volume_section_s01_write_file_io_pool(
					       section_descriptor,
					       segment_file->io_handle,
					       file_io_pool,
					       file_io_pool_entry,
					       segment_file->current_offset,
					       media_values,
				               error );
			}
			if( write_count == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write volume section.",
				 function );

				goto on_error;
			}
		}
		else if( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1 )
		{
			write_count = libewf_section_data_write(
				       section_descriptor,
				       segment_file->io_handle,
				       file_io_pool,
				       file_io_pool_entry,
				       segment_file->current_offset,
				       media_values,
				       data_section_descriptor,
			               error );

			if( write_count == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write data section.",
				 function );

				goto on_error;
			}
		}
		else
		{
			write_count = 0;
		}
		if( write_count > 0 )
		{
			if( libfdata_list_append_element(
			     segment_file->sections_list,
			     &element_index,
			     file_io_pool_entry,
			     segment_file->current_offset,
			     sizeof( ewf_section_descriptor_v1_t ),
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append element to sections list.",
				 function );

				goto on_error;
			}
			segment_file->current_offset += write_count;
			total_write_count            += write_count;
		}
		if( libewf_section_descriptor_free(
		     &section_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free section.",
			 function );

			goto on_error;
		}
	}
	else if( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF2 )
	{
		write_count = libewf_segment_file_write_device_information_section(
			       segment_file,
			       file_io_pool,
			       file_io_pool_entry,
			       device_information,
			       device_information_size,
			       media_values,
			       header_values,
			       error );

		if( write_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write device information section.",
			 function );

			goto on_error;
		}
		total_write_count += write_count;

		write_count = libewf_segment_file_write_case_data_section(
			       segment_file,
			       file_io_pool,
			       file_io_pool_entry,
			       case_data,
			       case_data_size,
			       media_values,
			       header_values,
			       timestamp,
			       error );

		if( write_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write case data section.",
			 function );

			goto on_error;
		}
		total_write_count += write_count;
	}
	return( total_write_count );

on_error:
	if( section_descriptor != NULL )
	{
		libewf_section_descriptor_free(
		 &section_descriptor,
		 NULL );
	}
	return( -1 );
}

/* Write the necessary sections before the actual data chunks to file
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_chunks_section_start(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t *table_section_data,
         size_t table_section_data_size,
         uint8_t *table_entries_data,
         size_t table_entries_data_size,
         uint32_t number_of_table_entries,
         uint64_t number_of_chunks_written,
         libcerror_error_t **error )
{
	libewf_section_descriptor_t *section_descriptor = NULL;
	libewf_table_section_t *table_section           = NULL;
	static char *function                           = "libewf_segment_file_write_chunks_section_start";
	ssize_t write_count                             = 0;

	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( segment_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ( segment_file->type != LIBEWF_SEGMENT_FILE_TYPE_EWF1 )
	 && ( segment_file->type != LIBEWF_SEGMENT_FILE_TYPE_EWF1_LOGICAL )
	 && ( segment_file->type != LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid segment file - unsupported type.",
		 function );

		return( -1 );
	}
	if( ( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
	 || ( segment_file->io_handle->format == LIBEWF_FORMAT_ENCASE1 ) )
	{
		if( libewf_table_section_initialize(
		     &table_section,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create table section.",
			 function );

			goto on_error;
		}
		table_section->first_chunk_index = number_of_chunks_written;
		table_section->base_offset       = 0;
		table_section->number_of_entries = number_of_table_entries;
		table_section->section_data      = table_section_data;
		table_section->section_data_size = table_section_data_size;

		/* Write table section descriptor
		 */
		write_count = libewf_table_section_write_file_io_pool(
		               table_section,
		               segment_file->io_handle,
		               file_io_pool,
		               file_io_pool_entry,
		               1,
		               segment_file->type,
		               (uint8_t *) "table",
		               5,
		               segment_file->current_offset,
		               table_entries_data,
		               table_entries_data_size,
		               0,
		               error );

		table_section->section_data      = NULL;
		table_section->section_data_size = 0;

		if( write_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write table section.",
			 function );

			goto on_error;
		}
		if( libewf_table_section_free(
		     &table_section,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free table section.",
			 function );

			goto on_error;
		}
	}
	else if( ( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1 )
	      || ( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_LOGICAL ) )
	{
		if( libewf_section_descriptor_initialize(
		     &section_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create section descriptor.",
			 function );

			goto on_error;
		}
		write_count = libewf_section_sectors_write(
		               section_descriptor,
		               file_io_pool,
		               file_io_pool_entry,
		               1,
		               segment_file->current_offset,
		               0,
		               0,
		               error );

		if( write_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write sectors section.",
			 function );

			goto on_error;
		}
		if( libewf_section_descriptor_free(
		     &section_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free section.",
			 function );

			goto on_error;
		}
	}
	segment_file->current_offset += write_count;

	return( write_count );

on_error:
	if( section_descriptor != NULL )
	{
		libewf_section_descriptor_free(
		 &section_descriptor,
		 NULL );
	}
	if( table_section != NULL )
	{
		libewf_table_section_free(
		 &table_section,
		 NULL );
	}
	return( -1 );
}

/* Write the necessary sections after the actual data chunks to file
 * If necessary the sections before the actual data chunks are corrected
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_chunks_section_end(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t *table_section_data,
         size_t table_section_data_size,
         uint8_t *table_entries_data,
         size_t table_entries_data_size,
         uint32_t number_of_table_entries,
         off64_t chunks_section_offset,
         size64_t chunks_section_size,
         uint32_t chunks_section_padding_size,
         uint64_t first_chunk_index,
         uint64_t base_offset,
         libcerror_error_t **error )
{
	libewf_section_descriptor_t *section_descriptor = NULL;
	libewf_table_section_t *table_section           = NULL;
	static char *function                           = "libewf_segment_file_write_chunks_section_end";
	size64_t maximum_chunks_section_size            = 0;
	ssize_t total_write_count                       = 0;
	ssize_t write_count                             = 0;
	int element_index                               = 0;
	int result                                      = 0;

	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( segment_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment file - missing IO handle.",
		 function );

		return( -1 );
	}
/* TODO what about linen 7 */
	if( ( segment_file->io_handle->format == LIBEWF_FORMAT_ENCASE6 )
	 || ( segment_file->io_handle->format == LIBEWF_FORMAT_ENCASE7 )
	 || ( segment_file->io_handle->format == LIBEWF_FORMAT_V2_ENCASE7 ) )
	{
		maximum_chunks_section_size = (size64_t) INT64_MAX;
	}
	else
	{
		maximum_chunks_section_size = (size64_t) INT32_MAX;
	}
	if( chunks_section_size >= maximum_chunks_section_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid chunk section size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( libewf_section_descriptor_initialize(
	     &section_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create section descriptor.",
		 function );

		goto on_error;
	}
	/* The EWF version 1 format requires the chunks section descriptor to
	 * be corrected in EWF version 2 the section descriptor is added to
	 * the end of the section
	 */
	if( ( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1 )
	 || ( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_LOGICAL )
	 || ( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: setting file descriptor to start of chunks section offset: %" PRIi64 ".\n",
			 function,
			 chunks_section_offset );
		}
#endif
		/* Seek the start of the chunks section
		 */
		if( libbfio_pool_seek_offset(
		     file_io_pool,
		     file_io_pool_entry,
		     chunks_section_offset,
		     SEEK_SET,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to find offset to correct sectors size.",
			 function );

			goto on_error;
		}
		if( ( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
		 || ( segment_file->io_handle->format == LIBEWF_FORMAT_ENCASE1 ) )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: correcting table section offset: %" PRIi64 " size: %" PRIu64 ".\n",
				 function,
				 chunks_section_offset,
				 chunks_section_size );
			}
#endif
			if( libewf_table_section_initialize(
			     &table_section,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create table section.",
				 function );

				goto on_error;
			}
			table_section->first_chunk_index = first_chunk_index;
			table_section->base_offset       = 0;
			table_section->number_of_entries = number_of_table_entries;
			table_section->section_data      = table_section_data;
			table_section->section_data_size = table_section_data_size;

			/* Rewrite table section descriptor
			 */
			write_count = libewf_table_section_write_file_io_pool(
				       table_section,
				       segment_file->io_handle,
				       file_io_pool,
				       file_io_pool_entry,
				       1,
				       segment_file->type,
				       (uint8_t *) "table",
				       5,
				       chunks_section_offset,
				       table_entries_data,
				       table_entries_data_size,
				       chunks_section_size,
				       error );

			table_section->section_data      = NULL;
			table_section->section_data_size = 0;

			if( write_count == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write table section.",
				 function );

				goto on_error;
			}
			if( libewf_table_section_free(
			     &table_section,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free table section.",
				 function );

				goto on_error;
			}
		}
		else if( ( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1 )
		      || ( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_LOGICAL ) )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				"%s: correcting sectors section offset: %" PRIi64 " size: %" PRIu64 ".\n",
				 function,
				 chunks_section_offset,
				 chunks_section_size );
			}
#endif
			write_count = libewf_section_sectors_write(
				       section_descriptor,
				       file_io_pool,
				       file_io_pool_entry,
				       1,
				       chunks_section_offset,
				       chunks_section_size,
				       0,
				       error );

			if( write_count == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write sectors section.",
				 function );

				goto on_error;
			}
		}
		if( libfdata_list_append_element(
		     segment_file->sections_list,
		     &element_index,
		     file_io_pool_entry,
		     chunks_section_offset,
		     sizeof( ewf_section_descriptor_v1_t ),
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append element to sections list.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: setting file descriptor back to end of data at offset: 0x%08" PRIx64 ".\n",
			 function,
			 segment_file->current_offset );
		}
#endif
		if( libbfio_pool_seek_offset(
		     file_io_pool,
		     file_io_pool_entry,
		     segment_file->current_offset,
		     SEEK_SET,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to find offset to continue.",
			 function );

			goto on_error;
		}
	}
	else
	{
		write_count = libewf_section_sectors_write(
		               section_descriptor,
		               file_io_pool,
		               file_io_pool_entry,
		               2,
		               chunks_section_offset,
		               chunks_section_size,
		               chunks_section_padding_size,
		               error );

		if( write_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write sectors data section.",
			 function );

			goto on_error;
		}
		segment_file->current_offset += write_count;
		total_write_count            += write_count;

		if( libfdata_list_append_element(
		     segment_file->sections_list,
		     &element_index,
		     file_io_pool_entry,
		     segment_file->current_offset - sizeof( ewf_section_descriptor_v2_t ),
		     sizeof( ewf_section_descriptor_v2_t ),
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append element to sections list.",
			 function );

			goto on_error;
		}
	}
	if( libewf_section_descriptor_free(
	     &section_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free section.",
		 function );

		goto on_error;
	}
	if( ( segment_file->type != LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
	 && ( segment_file->io_handle->format != LIBEWF_FORMAT_ENCASE1 ) )
	{
		if( libewf_table_section_initialize(
		     &table_section,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create table section.",
			 function );

			goto on_error;
		}
		table_section->first_chunk_index = first_chunk_index;
		table_section->base_offset       = base_offset;
		table_section->number_of_entries = number_of_table_entries;
		table_section->section_data      = table_section_data;
		table_section->section_data_size = table_section_data_size;

		write_count = libewf_table_section_write_file_io_pool(
		               table_section,
		               segment_file->io_handle,
		               file_io_pool,
		               file_io_pool_entry,
		               segment_file->major_version,
		               segment_file->type,
		               (uint8_t *) "table",
		               5,
		               segment_file->current_offset,
		               table_entries_data,
		               table_entries_data_size,
		               0,
		               error );

		table_section->section_data      = NULL;
		table_section->section_data_size = 0;

		if( write_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write sector table section.",
			 function );

			goto on_error;
		}
		if( libewf_table_section_free(
		     &table_section,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free table section.",
			 function );

			goto on_error;
		}
		if( segment_file->major_version == 1 )
		{
			result = libfdata_list_append_element(
			          segment_file->sections_list,
			          &element_index,
			          file_io_pool_entry,
			          segment_file->current_offset,
			          sizeof( ewf_section_descriptor_v1_t ),
			          0,
			          error );
		}
		segment_file->current_offset += write_count;
		total_write_count            += write_count;

		if( segment_file->major_version == 2 )
		{
			result = libfdata_list_append_element(
			          segment_file->sections_list,
			          &element_index,
			          file_io_pool_entry,
			          segment_file->current_offset - sizeof( ewf_section_descriptor_v2_t ),
			          sizeof( ewf_section_descriptor_v2_t ),
			          0,
			          error );
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append element to sections list.",
			 function );

			goto on_error;
		}
	}
	if( ( ( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1 )
	  ||  ( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_LOGICAL ) )
	 && ( segment_file->io_handle->format != LIBEWF_FORMAT_ENCASE1 ) )
	{
		if( libewf_table_section_initialize(
		     &table_section,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create table section.",
			 function );

			goto on_error;
		}
		table_section->first_chunk_index = first_chunk_index;
		table_section->base_offset       = base_offset;
		table_section->number_of_entries = number_of_table_entries;
		table_section->section_data      = table_section_data;
		table_section->section_data_size = table_section_data_size;

		write_count = libewf_table_section_write_file_io_pool(
		               table_section,
		               segment_file->io_handle,
		               file_io_pool,
		               file_io_pool_entry,
		               1,
		               segment_file->type,
		               (uint8_t *) "table2",
		               6,
		               segment_file->current_offset,
		               table_entries_data,
		               table_entries_data_size,
		               0,
		               error );

		table_section->section_data      = NULL;
		table_section->section_data_size = 0;

		if( write_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write table2 section.",
			 function );

			goto on_error;
		}
		if( libewf_table_section_free(
		     &table_section,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free table section.",
			 function );

			goto on_error;
		}
		if( libfdata_list_append_element(
		     segment_file->sections_list,
		     &element_index,
		     file_io_pool_entry,
		     segment_file->current_offset,
		     sizeof( ewf_section_descriptor_v1_t ),
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append element to sections list.",
			 function );

			goto on_error;
		}
		segment_file->current_offset += write_count;
		total_write_count            += write_count;
	}
	return( total_write_count );

on_error:
	if( table_section != NULL )
	{
		libewf_table_section_free(
		 &table_section,
		 NULL );
	}
	if( section_descriptor != NULL )
	{
		libewf_section_descriptor_free(
		 &section_descriptor,
		 NULL );
	}
	return( -1 );
}

/* Write a chunk of data to a segment file and update the chunk table
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_chunk_data(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint64_t chunk_index,
         libewf_chunk_data_t *chunk_data,
         libcerror_error_t **error )
{
	static char *function     = "libewf_segment_file_write_chunk_data";
	ssize_t write_count       = 0;
	uint32_t chunk_write_size = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t chunk_checksum   = 0;
	int result                = 0;
#endif

	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( segment_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( chunk_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk data.",
		 function );

		return( -1 );
	}
/* TODO chunk data rewrite */
	if( segment_file->io_handle->format != LIBEWF_FORMAT_SMART )
	{
		if( chunk_data->data_size > chunk_data->allocated_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid chunk data size value out of bounds.",
			 function );

			return( -1 );
		}
	}
	if( libewf_chunk_data_get_write_size(
	     chunk_data,
	     &chunk_write_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve chunk write size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: chunk: %05" PRIu64 " file IO pool entry\t: %d\n",
		 function,
		 chunk_index,
		 file_io_pool_entry );

		libcnotify_printf(
		 "%s: chunk: %05" PRIu64 " offset\t\t: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 chunk_index,
		 segment_file->current_offset,
		 segment_file->current_offset );

		libcnotify_printf(
		 "%s: chunk: %05" PRIu64 " write size\t\t: %" PRIzd "\n",
		 function,
		 chunk_index,
		 chunk_write_size );

		libcnotify_printf(
		 "%s: chunk: %05" PRIu64 " data size\t\t: %" PRIzd "\n",
		 function,
		 chunk_index,
		 chunk_data->data_size );

		libcnotify_printf(
		 "%s: chunk: %05" PRIu64 " padding size\t\t: %" PRIzd "\n",
		 function,
		 chunk_index,
		 chunk_data->padding_size );

		result = libewf_chunk_data_get_checksum(
		          chunk_data,
		          segment_file->io_handle->compression_method,
		          &chunk_checksum,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve chunk checksum.",
			 function );

			return( -1 );
		}
		libcnotify_printf(
		 "%s: chunk: %05" PRIu64 " checksum\t\t: ",
		 function,
		 chunk_index );

		if( result != 0 )
		{
			libcnotify_printf(
			 "0x%08" PRIx32 "",
			 chunk_checksum );
		}
		else
		{
			libcnotify_printf(
			 "N/A" );
		}
		libcnotify_printf(
		 "\n" );

		libcnotify_printf(
		 "%s: chunk: %05" PRIu64 " flags:\n",
		 function,
		 chunk_index );

		if( ( chunk_data->range_flags & LIBEWF_RANGE_FLAG_IS_COMPRESSED ) != 0 )
		{
			libcnotify_printf(
			 "\tIs compressed\n" );
		}
		if( ( chunk_data->range_flags & LIBEWF_RANGE_FLAG_HAS_CHECKSUM ) != 0 )
		{
			libcnotify_printf(
			 "\tHas checksum\n" );
		}
		if( ( chunk_data->range_flags & LIBEWF_RANGE_FLAG_USES_PATTERN_FILL ) != 0 )
		{
			libcnotify_printf(
			 "\tUses pattern fill\n" );
		}
		libcnotify_printf(
		 "\n" );
	}
#endif
	write_count = libewf_chunk_data_write(
	               chunk_data,
	               file_io_pool,
	               file_io_pool_entry,
	               error );

	if( write_count != (ssize_t) chunk_write_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write chunk data.",
		 function );

		return( -1 );
	}
	segment_file->current_offset += write_count;

	return( write_count );
}

/* Writes the hash sections to file
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_hash_sections(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_hash_sections_t *hash_sections,
         libfvalue_table_t *hash_values,
         libcerror_error_t **error )
{
	libewf_section_descriptor_t *section_descriptor = NULL;
	static char *function                           = "libewf_segment_file_write_hash_sections";
	ssize_t total_write_count                       = 0;
	ssize_t write_count                             = 0;
	int element_index                               = 0;
	int result                                      = 0;

	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( segment_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment file - missing IO handle.",
		 function );

		return( -1 );
	}
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
	if( ( segment_file->io_handle->format == LIBEWF_FORMAT_ENCASE6 )
	 || ( segment_file->io_handle->format == LIBEWF_FORMAT_ENCASE7 )
	 || ( segment_file->io_handle->format == LIBEWF_FORMAT_LINEN6 )
	 || ( segment_file->io_handle->format == LIBEWF_FORMAT_LINEN7 ) )
	{
		/* Write the digest section if required
		 */
		if( hash_sections->sha1_digest_set != 0 )
		{
			if( libewf_section_descriptor_initialize(
			     &section_descriptor,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create section descriptor.",
				 function );

				goto on_error;
			}
			write_count = libewf_digest_section_write_file_io_pool(
				       section_descriptor,
				       segment_file->io_handle,
				       file_io_pool,
				       file_io_pool_entry,
				       segment_file->current_offset,
				       hash_sections,
				       error );

			if( write_count == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write digest section.",
				 function );

				goto on_error;
			}
			if( libfdata_list_append_element(
			     segment_file->sections_list,
			     &element_index,
			     file_io_pool_entry,
			     segment_file->current_offset,
			     sizeof( ewf_section_descriptor_v1_t ),
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append element to sections list.",
				 function );

				goto on_error;
			}
			segment_file->current_offset += write_count;
			total_write_count            += write_count;

			if( libewf_section_descriptor_free(
			     &section_descriptor,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free section.",
				 function );

				goto on_error;
			}
		}
	}
	/* Write the MD5 hash section if required
	 */
	if( hash_sections->md5_hash_set != 0 )
	{
		if( libewf_section_descriptor_initialize(
		     &section_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create section descriptor.",
			 function );

			goto on_error;
		}
		write_count = libewf_md5_hash_section_write_file_io_pool(
			       section_descriptor,
			       segment_file->io_handle,
			       file_io_pool,
			       file_io_pool_entry,
			       segment_file->major_version,
			       segment_file->current_offset,
			       hash_sections,
			       error );

		if( write_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write MD5 hash section.",
			 function );

			goto on_error;
		}
		if( segment_file->major_version == 1 )
		{
			result = libfdata_list_append_element(
			          segment_file->sections_list,
			          &element_index,
			          file_io_pool_entry,
			          segment_file->current_offset,
			          sizeof( ewf_section_descriptor_v1_t ),
			          0,
			          error );
		}
		segment_file->current_offset += write_count;
		total_write_count            += write_count;

		if( segment_file->major_version == 2 )
		{
			result = libfdata_list_append_element(
			          segment_file->sections_list,
			          &element_index,
			          file_io_pool_entry,
			          segment_file->current_offset - sizeof( ewf_section_descriptor_v2_t ),
			          sizeof( ewf_section_descriptor_v2_t ),
			          0,
			          error );
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append element to sections list.",
			 function );

			goto on_error;
		}
		if( libewf_section_descriptor_free(
		     &section_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free section.",
			 function );

			goto on_error;
		}
	}
	if( segment_file->io_handle->format == LIBEWF_FORMAT_V2_ENCASE7 )
	{
		/* Write the SHA1 hash section if required
		 */
		if( hash_sections->sha1_hash_set != 0 )
		{
			if( libewf_section_descriptor_initialize(
			     &section_descriptor,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create section descriptor.",
				 function );

				goto on_error;
			}
			write_count = libewf_sha1_hash_section_write_file_io_pool(
				       section_descriptor,
				       segment_file->io_handle,
				       file_io_pool,
				       file_io_pool_entry,
				       segment_file->major_version,
				       segment_file->current_offset,
				       hash_sections,
				       error );

			if( write_count == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write SHA1 hash section.",
				 function );

				goto on_error;
			}
			if( segment_file->major_version == 1 )
			{
				result = libfdata_list_append_element(
					  segment_file->sections_list,
					  &element_index,
					  file_io_pool_entry,
					  segment_file->current_offset,
					  sizeof( ewf_section_descriptor_v1_t ),
					  0,
					  error );
			}
			segment_file->current_offset += write_count;
			total_write_count            += write_count;

			if( segment_file->major_version == 2 )
			{
				result = libfdata_list_append_element(
					  segment_file->sections_list,
					  &element_index,
					  file_io_pool_entry,
					  segment_file->current_offset - sizeof( ewf_section_descriptor_v2_t ),
					  sizeof( ewf_section_descriptor_v2_t ),
					  0,
					  error );
			}
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append element to sections list.",
				 function );

				goto on_error;
			}
			if( libewf_section_descriptor_free(
			     &section_descriptor,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free section.",
				 function );

				goto on_error;
			}
		}
	}
	if( segment_file->io_handle->format == LIBEWF_FORMAT_EWFX )
	{
		/* Write the xhash section
		 */
		if( hash_sections->xhash != NULL )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				"%s: xhash already set - removing previous version.\n",
				 function );
			}
#endif
			memory_free(
			 hash_sections->xhash );

			hash_sections->xhash = NULL;
		}
		if( libewf_hash_values_generate_xhash(
		     hash_values,
		     &( hash_sections->xhash ),
		     &( hash_sections->xhash_size ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to generate xhash.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( libewf_debug_utf8_stream_print(
			     "XHash",
			     hash_sections->xhash,
			     hash_sections->xhash_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print xhash.",
				 function );

				goto on_error;
			}
		}
#endif
		if( libewf_section_descriptor_initialize(
		     &section_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create section descriptor.",
			 function );

			goto on_error;
		}
		/* Do not include the end of string character in the compressed data
		 */
		write_count = libewf_section_write_compressed_string(
			       section_descriptor,
			       segment_file->io_handle,
			       file_io_pool,
			       file_io_pool_entry,
			       1,
			       0,
			       (uint8_t *) "xhash",
			       5,
			       segment_file->current_offset,
			       segment_file->io_handle->compression_method,
			       LIBEWF_COMPRESSION_LEVEL_DEFAULT,
			       hash_sections->xhash,
			       hash_sections->xhash_size - 1,
			       0,
			       error );

		if( write_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write xhash section.",
			 function );

			goto on_error;
		}
		if( libfdata_list_append_element(
		     segment_file->sections_list,
		     &element_index,
		     file_io_pool_entry,
		     segment_file->current_offset,
		     sizeof( ewf_section_descriptor_v1_t ),
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append element to sections list.",
			 function );

			goto on_error;
		}
		segment_file->current_offset += write_count;
		total_write_count            += write_count;

		if( libewf_section_descriptor_free(
		     &section_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free section.",
			 function );

			goto on_error;
		}
	}
	return( total_write_count );

on_error:
	if( section_descriptor != NULL )
	{
		libewf_section_descriptor_free(
		 &section_descriptor,
		 NULL );
	}
	return( -1 );
}

/* Closes the segment file, necessary sections at the end of the segment file will be written
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_close(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint64_t number_of_chunks_written_to_segment_file,
         int last_segment_file,
         libewf_hash_sections_t *hash_sections,
         libfvalue_table_t *hash_values,
         libewf_media_values_t *media_values,
         libcdata_array_t *sessions,
         libcdata_array_t *tracks,
         libcdata_range_list_t *acquiry_errors,
         ewf_data_t **data_section_descriptor,
	 libcerror_error_t **error )
{
	libewf_section_descriptor_t *section_descriptor = NULL;
	static char *function                           = "libewf_segment_file_write_close";
	ssize_t total_write_count                       = 0;
	ssize_t write_count                             = 0;
	int element_index                               = 0;
	int number_of_acquiry_errors                    = 0;
	int number_of_sessions                          = 0;
	int result                                      = 0;

	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( segment_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment file - missing IO handle.",
		 function );

		return( -1 );
	}
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
	if( sessions == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sessions.",
		 function );

		return( -1 );
	}
	if( acquiry_errors == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid acquiry errors.",
		 function );

		return( -1 );
	}
	if( last_segment_file != 0 )
	{
		/* Write the data section for a single segment file only for EWF-E01
		 */
		if( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1 )
		{
			if( segment_file->segment_number == 1 )
			{
				if( libewf_section_descriptor_initialize(
				     &section_descriptor,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create section descriptor.",
					 function );

					goto on_error;
				}
				write_count = libewf_section_data_write(
					       section_descriptor,
					       segment_file->io_handle,
					       file_io_pool,
					       file_io_pool_entry,
					       segment_file->current_offset,
					       media_values,
					       data_section_descriptor,
					       error );

				if( write_count == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_WRITE_FAILED,
					 "%s: unable to write data section.",
					 function );

					goto on_error;
				}
				if( libfdata_list_append_element(
				     segment_file->sections_list,
				     &element_index,
				     file_io_pool_entry,
				     segment_file->current_offset,
				     sizeof( ewf_section_descriptor_v1_t ),
				     0,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append element to sections list.",
					 function );

					goto on_error;
				}
				segment_file->current_offset += write_count;
				total_write_count            += write_count;

				if( libewf_section_descriptor_free(
				     &section_descriptor,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free section.",
					 function );

					goto on_error;
				}
			}
		}
		/* Write the session section if required
		 */
		if( ( segment_file->io_handle->format == LIBEWF_FORMAT_ENCASE5 )
		 || ( segment_file->io_handle->format == LIBEWF_FORMAT_ENCASE6 )
		 || ( segment_file->io_handle->format == LIBEWF_FORMAT_ENCASE7 )
		 || ( segment_file->io_handle->format == LIBEWF_FORMAT_LINEN5 )
		 || ( segment_file->io_handle->format == LIBEWF_FORMAT_LINEN6 )
		 || ( segment_file->io_handle->format == LIBEWF_FORMAT_LINEN7 )
		 || ( segment_file->io_handle->format == LIBEWF_FORMAT_V2_ENCASE7 )
		 || ( segment_file->io_handle->format == LIBEWF_FORMAT_EWFX ) )
		{
			if( libcdata_array_get_number_of_entries(
			     sessions,
			     &number_of_sessions,
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
			if( number_of_sessions > 0 )
			{
				if( libewf_section_descriptor_initialize(
				     &section_descriptor,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create section descriptor.",
					 function );

					goto on_error;
				}
				write_count = libewf_session_section_write_file_io_pool(
					       section_descriptor,
					       segment_file->io_handle,
					       file_io_pool,
					       file_io_pool_entry,
					       segment_file->major_version,
					       segment_file->current_offset,
					       sessions,
					       tracks,
					       error );

				if( write_count == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_WRITE_FAILED,
					 "%s: unable to write session section.",
					 function );

					goto on_error;
				}
				if( segment_file->major_version == 1 )
				{
					result = libfdata_list_append_element(
					          segment_file->sections_list,
					          &element_index,
					          file_io_pool_entry,
					          segment_file->current_offset,
					          sizeof( ewf_section_descriptor_v1_t ),
					          0,
					          error );
				}
				segment_file->current_offset += write_count;
				total_write_count            += write_count;

				if( segment_file->major_version == 2 )
				{
					result = libfdata_list_append_element(
					          segment_file->sections_list,
					          &element_index,
					          file_io_pool_entry,
					          segment_file->current_offset - sizeof( ewf_section_descriptor_v2_t ),
					          sizeof( ewf_section_descriptor_v2_t ),
					          0,
					          error );
				}
				if( result != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append element to sections list.",
					 function );

					goto on_error;
				}
				if( libewf_section_descriptor_free(
				     &section_descriptor,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free section.",
					 function );

					goto on_error;
				}
			}
		}
		/* Write the error section if required
		 */
		if( ( segment_file->io_handle->format == LIBEWF_FORMAT_ENCASE3 )
		 || ( segment_file->io_handle->format == LIBEWF_FORMAT_ENCASE4 )
		 || ( segment_file->io_handle->format == LIBEWF_FORMAT_ENCASE5 )
		 || ( segment_file->io_handle->format == LIBEWF_FORMAT_ENCASE6 )
		 || ( segment_file->io_handle->format == LIBEWF_FORMAT_ENCASE7 )
		 || ( segment_file->io_handle->format == LIBEWF_FORMAT_LINEN5 )
		 || ( segment_file->io_handle->format == LIBEWF_FORMAT_LINEN6 )
		 || ( segment_file->io_handle->format == LIBEWF_FORMAT_LINEN7 )
		 || ( segment_file->io_handle->format == LIBEWF_FORMAT_V2_ENCASE7 )
		 || ( segment_file->io_handle->format == LIBEWF_FORMAT_EWFX ) )
		{
			if( libcdata_range_list_get_number_of_elements(
			     acquiry_errors,
			     &number_of_acquiry_errors,
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
			if( number_of_acquiry_errors > 0 )
			{
				if( libewf_section_descriptor_initialize(
				     &section_descriptor,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create section descriptor.",
					 function );

					goto on_error;
				}
				write_count = libewf_error2_section_write_file_io_pool(
					       section_descriptor,
					       segment_file->io_handle,
					       file_io_pool,
					       file_io_pool_entry,
					       segment_file->major_version,
					       segment_file->current_offset,
					       acquiry_errors,
					       error );

				if( write_count == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_WRITE_FAILED,
					 "%s: unable to write error2 section.",
					 function );

					goto on_error;
				}
				if( segment_file->major_version == 1 )
				{
					result = libfdata_list_append_element(
					          segment_file->sections_list,
					          &element_index,
					          file_io_pool_entry,
					          segment_file->current_offset,
					          sizeof( ewf_section_descriptor_v1_t ),
					          0,
					          error );
				}
				segment_file->current_offset += write_count;
				total_write_count            += write_count;

				if( segment_file->major_version == 2 )
				{
					result = libfdata_list_append_element(
					          segment_file->sections_list,
					          &element_index,
					          file_io_pool_entry,
					          segment_file->current_offset - sizeof( ewf_section_descriptor_v2_t ),
					          sizeof( ewf_section_descriptor_v2_t ),
					          0,
					          error );
				}
				if( result != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append element to sections list.",
					 function );

					goto on_error;
				}
				if( libewf_section_descriptor_free(
				     &section_descriptor,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free section.",
					 function );

					goto on_error;
				}
			}
		}
		/* Write the hash sections
		 */
		write_count = libewf_segment_file_write_hash_sections(
			       segment_file,
			       file_io_pool,
			       file_io_pool_entry,
			       hash_sections,
			       hash_values,
			       error );

		if( write_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write hash sections.",
			 function );

			goto on_error;
		}
		total_write_count += write_count;
	}
	/* Write the done or next section
	 * The segment file offset is updated by the function
	 */
	write_count = libewf_segment_file_write_last_section(
		       segment_file,
		       file_io_pool,
		       file_io_pool_entry,
	               last_segment_file,
	               error );

	if( write_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write end of segment file.",
		 function );

		goto on_error;
	}
	total_write_count += write_count;

	segment_file->number_of_chunks = number_of_chunks_written_to_segment_file;

	if( libbfio_pool_close(
	     file_io_pool,
	     file_io_pool_entry,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close segment file: %" PRIu16 ".",
		 function,
		 segment_file->segment_number );

		goto on_error;
	}
	segment_file->flags &= ~( LIBEWF_SEGMENT_FILE_FLAG_WRITE_OPEN );

	return( total_write_count );

on_error:
	if( section_descriptor != NULL )
	{
		libewf_section_descriptor_free(
		 &section_descriptor,
		 NULL );
	}
	return( -1 );
}

/* Reopens the segment file for resume writing
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_file_reopen(
     libewf_segment_file_t *segment_file,
     int last_section_index,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     libfcache_cache_t *sections_cache,
     libcerror_error_t **error )
{
	libewf_section_descriptor_t *last_section = NULL;
	static char *function                     = "libewf_segment_file_reopen";
	size64_t storage_media_size               = 0;
	uint64_t number_of_chunks                 = 0;

	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( segment_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( segment_file->io_handle->chunk_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment file - invalid IO handle - missing chunk size.",
		 function );

		return( -1 );
	}
	if( libfdata_list_get_element_value_by_index(
	     segment_file->sections_list,
	     (intptr_t *) file_io_pool,
	     (libfdata_cache_t *) sections_cache,
	     last_section_index,
	     (intptr_t **) &last_section,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve section: %d from sections list.",
		 function,
		 last_section_index );

		return( -1 );
	}
	if( last_section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing section: %d.",
		 function,
		 last_section_index );

		return( -1 );
	}
        segment_file->current_offset      = last_section->end_offset;
	segment_file->last_section_offset = last_section->end_offset;

	if( libfdata_list_resize(
	     segment_file->sections_list,
	     last_section_index + 1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize sections list.",
		 function );

		return( -1 );
	}
	if( segment_file->number_of_chunks > 0 )
	{
		if( libfdata_list_get_mapped_size_by_index(
		     segment_file->chunk_groups_list,
		     segment_file->current_chunk_group_index,
		     &storage_media_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve chunk group: %d mapped size.",
			 function,
			 segment_file->current_chunk_group_index );

			return( -1 );
		}
		if( libfdata_list_resize(
		     segment_file->chunk_groups_list,
		     segment_file->current_chunk_group_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize chunk groups list.",
			 function );

			return( -1 );
		}
		segment_file->current_chunk_group_index -= 1;

		if( storage_media_size > segment_file->storage_media_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid storage media size value out of bounds.",
			 function );

			return( -1 );
		}
		segment_file->storage_media_size -= storage_media_size;

		number_of_chunks = storage_media_size;

		if( ( number_of_chunks % segment_file->io_handle->chunk_size ) != 0 )
		{
			number_of_chunks /= (uint64_t) segment_file->io_handle->chunk_size;
			number_of_chunks += 1;
		}
		else
		{
			number_of_chunks /= (uint64_t) segment_file->io_handle->chunk_size;
		}
		if( number_of_chunks > segment_file->number_of_chunks )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid number of chunks value out of bounds.",
			 function );

			return( -1 );
		}
		segment_file->number_of_chunks -= (uint64_t) number_of_chunks;
	}
	if( libbfio_pool_reopen(
	     file_io_pool,
	     file_io_pool_entry,
	     LIBBFIO_OPEN_READ_WRITE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to re-open file IO pool entry: %d.",
		 function,
		 file_io_pool_entry );

		return( -1 );
	}
	segment_file->flags |= LIBEWF_SEGMENT_FILE_FLAG_WRITE_OPEN;

	return( 1 );
}

/* Corrects sections after streamed write
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_file_write_sections_correction(
     libewf_segment_file_t *segment_file,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     uint64_t number_of_chunks_written_to_segment_file,
     int last_segment_file,
     libewf_media_values_t *media_values,
     libfvalue_table_t *header_values,
     time_t timestamp,
     libfvalue_table_t *hash_values,
     libewf_hash_sections_t *hash_sections,
     libcdata_array_t *sessions,
     libcdata_array_t *tracks,
     libcdata_range_list_t *acquiry_errors,
     uint8_t **case_data,
     size_t *case_data_size,
     uint8_t **device_information,
     size_t *device_information_size,
     ewf_data_t **data_section_descriptor,
     libcerror_error_t **error )
{
	libewf_section_descriptor_t *section_descriptor = NULL;
	libfcache_cache_t *sections_cache               = NULL;
	static char *function                           = "libewf_segment_file_write_sections_correction";
	ssize_t write_count                             = 0;
	off64_t next_section_start_offset               = 0;
	int correct_last_next_section                   = 0;
	int number_of_sections                          = 0;
	int section_index                               = 0;

	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( segment_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( segment_file->sections_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment file - missing sections list.",
		 function );

		return( -1 );
	}
	if( device_information == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device information.",
		 function );

		return( -1 );
	}
	if( device_information_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device information size.",
		 function );

		return( -1 );
	}
	if( case_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid case data.",
		 function );

		return( -1 );
	}
	if( case_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid case data size.",
		 function );

		return( -1 );
	}
	if( libfcache_cache_initialize(
	     &sections_cache,
	     LIBEWF_MAXIMUM_CACHE_ENTRIES_SECTIONS,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create section descriptor. cache.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: correcting sections in segment file: %" PRIu16 ".\n",
		 function,
		 segment_file->segment_number );
	}
#endif
	if( libfdata_list_get_number_of_elements(
	     segment_file->sections_list,
	     &number_of_sections,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the number of sections in the sections list.",
		 function );

		goto on_error;
	}
	for( section_index = 0;
	     section_index < number_of_sections;
	     section_index++ )
	{
		if( libfdata_list_get_element_value_by_index(
		     segment_file->sections_list,
		     (intptr_t *) file_io_pool,
		     (libfdata_cache_t *) sections_cache,
		     section_index,
		     (intptr_t **) &section_descriptor,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve section: %d from sections list.",
			 function,
			 section_index );

			goto on_error;
		}
		if( section_descriptor == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing section: %d.",
			 function,
			 section_index );

			goto on_error;
		}
		if( section_descriptor->type != 0 )
		{
			switch( section_descriptor->type )
			{
				case LIBEWF_SECTION_TYPE_DEVICE_INFORMATION:
					if( *device_information == NULL )
					{
						if( libewf_device_information_generate(
						     device_information,
						     device_information_size,
						     media_values,
						     header_values,
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
							 "%s: unable to generate device information.",
							 function );

							goto on_error;
						}
					}
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: correcting device information section.\n",
						 function );
					}
#endif
					if( libbfio_pool_seek_offset(
					     file_io_pool,
					     file_io_pool_entry,
					     section_descriptor->start_offset,
					     SEEK_SET,
					     error ) == -1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_SEEK_FAILED,
						 "%s: unable to find offset to correct device information section.",
						 function );

						goto on_error;
					}
					segment_file->current_offset = section_descriptor->start_offset;

					/* Do not include the end of string character in the compressed data
					 */
					write_count = libewf_section_write_compressed_string(
						       section_descriptor,
						       segment_file->io_handle,
						       file_io_pool,
						       file_io_pool_entry,
						       2,
						       LIBEWF_SECTION_TYPE_DEVICE_INFORMATION,
						       NULL,
						       0,
						       section_descriptor->start_offset,
						       segment_file->io_handle->compression_method,
						       LIBEWF_COMPRESSION_LEVEL_DEFAULT,
						       *device_information,
						       *device_information_size - 2,
						       (size_t) section_descriptor->data_size,
						       error );

					if( write_count == -1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_WRITE_FAILED,
						 "%s: unable to write device information section.",
						 function );

						goto on_error;
					}
					segment_file->current_offset += write_count;

					break;

				case LIBEWF_SECTION_TYPE_CASE_DATA:
					if( *case_data == NULL )
					{
						if( libewf_case_data_generate(
						     case_data,
						     case_data_size,
						     media_values,
						     header_values,
						     timestamp,
						     segment_file->io_handle->format,
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
							 "%s: unable to generate case data.",
							 function );

							goto on_error;
						}
					}
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: correcting case data section.\n",
						 function );
					}
#endif
					if( libbfio_pool_seek_offset(
					     file_io_pool,
					     file_io_pool_entry,
					     section_descriptor->start_offset,
					     SEEK_SET,
					     error ) == -1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_SEEK_FAILED,
						 "%s: unable to find offset to correct case data section.",
						 function );

						goto on_error;
					}
					segment_file->current_offset = section_descriptor->start_offset;

					/* Do not include the end of string character in the compressed data
					 */
					write_count = libewf_section_write_compressed_string(
						       section_descriptor,
						       segment_file->io_handle,
						       file_io_pool,
						       file_io_pool_entry,
						       2,
						       LIBEWF_SECTION_TYPE_CASE_DATA,
						       NULL,
						       0,
						       section_descriptor->start_offset,
						       segment_file->io_handle->compression_method,
						       LIBEWF_COMPRESSION_LEVEL_DEFAULT,
						       *case_data,
						       *case_data_size - 2,
						       (size_t) section_descriptor->data_size,
						       error );

					if( write_count == -1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_WRITE_FAILED,
						 "%s: unable to write case data section.",
						 function );

						goto on_error;
					}
					segment_file->current_offset += write_count;

					break;

				case LIBEWF_SECTION_TYPE_NEXT:
					/* The last segment file should be terminated with a done section and not with a next section
					 */
					if( last_segment_file != 0 )
					{
						correct_last_next_section = 1;
						next_section_start_offset = section_descriptor->start_offset;
					}
					break;
			}
		}
		else if( section_descriptor->type_string_length == 6 )
		{
			if( memory_compare(
			     section_descriptor->type_string,
			     "volume",
			     6 ) == 0 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: correcting volume section.\n",
					 function );
				}
#endif
				if( libbfio_pool_seek_offset(
				     file_io_pool,
				     file_io_pool_entry,
				     section_descriptor->start_offset,
				     SEEK_SET,
				     error ) == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_SEEK_FAILED,
					 "%s: unable to find offset to correct volume section.",
					 function );

					goto on_error;
				}
				segment_file->current_offset = section_descriptor->start_offset;

				if( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1 )
				{
					write_count = libewf_volume_section_e01_write_file_io_pool(
						       section_descriptor,
						       segment_file->io_handle,
						       file_io_pool,
						       file_io_pool_entry,
						       section_descriptor->start_offset,
						       media_values,
						       error );
				}
				else if( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
				{
					write_count = libewf_volume_section_s01_write_file_io_pool(
						       section_descriptor,
						       segment_file->io_handle,
						       file_io_pool,
						       file_io_pool_entry,
						       section_descriptor->start_offset,
						       media_values,
						       error );
				}
				else
				{
					write_count = -1;
				}
				if( write_count == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_WRITE_FAILED,
					 "%s: unable to correct volume section.",
					 function );

					goto on_error;
				}
				segment_file->current_offset += write_count;
			}
		}
		else if( section_descriptor->type_string_length == 4 )
		{
			if( memory_compare(
			     section_descriptor->type_string,
			     "data",
			     4 ) == 0 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: correcting data section.\n",
					 function );
				}
#endif
				if( libbfio_pool_seek_offset(
				     file_io_pool,
				     file_io_pool_entry,
				     section_descriptor->start_offset,
				     SEEK_SET,
				     error ) == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_SEEK_FAILED,
					 "%s: unable to seek offset of data section.",
					 function );

					goto on_error;
				}
				segment_file->current_offset = section_descriptor->start_offset;

				write_count = libewf_section_data_write(
					       section_descriptor,
					       segment_file->io_handle,
					       file_io_pool,
					       file_io_pool_entry,
					       section_descriptor->start_offset,
					       media_values,
					       data_section_descriptor,
					       error );

				if( write_count == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_WRITE_FAILED,
					 "%s: unable to correct data section.",
					 function );

					goto on_error;
				}
				segment_file->current_offset += write_count;
			}
		}
	}
	if( correct_last_next_section == 0 )
	{
		if( libbfio_pool_close(
		     file_io_pool,
		     file_io_pool_entry,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close file IO pool entry: %d.",
			 function,
			 file_io_pool_entry );

			goto on_error;
		}
	}
	else
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: correcting last next section.\n",
			 function );
		}
#endif
		if( libbfio_pool_seek_offset(
		     file_io_pool,
		     file_io_pool_entry,
		     next_section_start_offset,
		     SEEK_SET,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to find offset to last next section.",
			 function );

			goto on_error;
		}
		segment_file->current_offset = next_section_start_offset;

		write_count = libewf_segment_file_write_close(
			       segment_file,
			       file_io_pool,
			       file_io_pool_entry,
			       number_of_chunks_written_to_segment_file,
			       1,
			       hash_sections,
			       hash_values,
			       media_values,
			       sessions,
			       tracks,
			       acquiry_errors,
			       data_section_descriptor,
			       error );

		if( write_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to close segment file.",
			 function );

			goto on_error;
		}
	}
	if( libfcache_cache_free(
	     &sections_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free sections cache.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( sections_cache != NULL )
	{
		libfcache_cache_free(
		 &sections_cache,
		 NULL );
	}
	return( -1 );
}

/* Reads a segment file
 * Callback function for the segment files list
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_file_read_element_data(
     libewf_io_handle_t *io_handle,
     libbfio_pool_t *file_io_pool,
     libfdata_list_element_t *element,
     libfdata_cache_t *segment_file_cache,
     int file_io_pool_entry,
     off64_t segment_file_offset,
     size64_t segment_file_size,
     uint32_t element_flags LIBEWF_ATTRIBUTE_UNUSED,
     uint8_t read_flags LIBEWF_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libewf_section_descriptor_t *section_descriptor = NULL;
	libewf_segment_file_t *segment_file             = NULL;
	libfcache_cache_t *sections_cache               = NULL;
	static char *function                           = "libewf_segment_file_read_element_data";
	ssize_t read_count                              = 0;
	off64_t section_data_offset                     = 0;
	int element_index                               = 0;
	int last_section                                = 0;
	int number_of_sections                          = 0;
	int result                                      = 0;
	int section_index                               = 0;

	LIBEWF_UNREFERENCED_PARAMETER( element_flags )
	LIBEWF_UNREFERENCED_PARAMETER( read_flags )

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( libewf_segment_file_initialize(
	     &segment_file,
	     io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create segment file.",
		 function );

		goto on_error;
	}
	if( segment_file_size == 0 )
	{
		/* segment_file_size is 0 on write correction
		 */
		if( libbfio_pool_get_size(
		     file_io_pool,
		     file_io_pool_entry,
		     &segment_file_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve segment file size.",
			 function );

			goto on_error;
		}
	}
	read_count = libewf_segment_file_read_file_header_file_io_pool(
		      segment_file,
		      file_io_pool,
		      file_io_pool_entry,
		      error );

/* TODO deal with corrupted header ? */
	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read segment file header.",
		 function );

		goto on_error;
	}
	if( ( segment_file->type != LIBEWF_SEGMENT_FILE_TYPE_EWF1 )
	 && ( segment_file->type != LIBEWF_SEGMENT_FILE_TYPE_EWF1_LOGICAL )
	 && ( segment_file->type != LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
	 && ( segment_file->type != LIBEWF_SEGMENT_FILE_TYPE_EWF2 )
	 && ( segment_file->type != LIBEWF_SEGMENT_FILE_TYPE_EWF2_LOGICAL ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported segment file type.",
		 function );

		goto on_error;
	}
	if( ( io_handle->segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
	 && ( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1 ) )
	{
		segment_file->type = LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART;
	}
	else if( ( io_handle->segment_file_type != LIBEWF_SEGMENT_FILE_TYPE_UNDEFINED )
	      && ( io_handle->segment_file_type != segment_file->type ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: segment file type value mismatch.",
		 function );

		goto on_error;
	}
	if( segment_file->major_version == 2 )
	{
		if( ( segment_file->compression_method != LIBEWF_COMPRESSION_METHOD_DEFLATE )
		 && ( segment_file->compression_method != LIBEWF_COMPRESSION_METHOD_BZIP2 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported compression method.",
			 function );

			goto on_error;
		}
	}
	/* Read the section descriptors:
	 * EWF version 1 read from front to back
	 * EWF version 2 read from back to front
	 */
	if( segment_file->major_version == 1 )
	{
		segment_file_offset = (off64_t) read_count;
	}
	else if( segment_file->major_version == 2 )
	{
		segment_file_offset = (off64_t) segment_file_size - sizeof( ewf_section_descriptor_v2_t );
	}
	last_section = 0;

	if( libewf_section_descriptor_initialize(
	     &section_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create section descriptor.",
		 function );

		goto on_error;
	}
	while( ( segment_file_offset > 0 )
	    && ( (size64_t) segment_file_offset < segment_file_size ) )
	{
		read_count = libewf_section_descriptor_read_file_io_pool(
			      section_descriptor,
			      file_io_pool,
			      file_io_pool_entry,
			      segment_file_offset,
			      segment_file->major_version,
			      error );

		if( read_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read section descriptor.",
			 function );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				if( ( error != NULL )
				 && ( *error != NULL ) )
				{
					libcnotify_print_error_backtrace(
					 *error );
				}
			}
#endif
			libcerror_error_free(
			 error );

			segment_file->flags |= LIBEWF_SEGMENT_FILE_FLAG_IS_CORRUPTED;

			break;
		}
		segment_file->current_offset = segment_file_offset + read_count;

		if( segment_file->major_version == 1 )
		{
			if( section_descriptor->type == LIBEWF_SECTION_TYPE_NEXT )
			{
				last_section = 1;
			}
			else if( section_descriptor->type == LIBEWF_SECTION_TYPE_DONE )
			{
				last_section = 1;

				segment_file->flags |= LIBEWF_SEGMENT_FILE_FLAG_IS_LAST;
			}
			segment_file->last_section_offset = segment_file_offset;

			if( libfdata_list_append_element(
			     segment_file->sections_list,
			     &element_index,
			     file_io_pool_entry,
			     segment_file_offset,
			     sizeof( ewf_section_descriptor_v1_t ),
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append element to sections list.",
				 function );

				goto on_error;
			}
			segment_file_offset += section_descriptor->size;

/* TODO move into section descriptor read ? */
			if( ( last_section != 0 )
			 && ( section_descriptor->size == 0 ) )
			{
				segment_file_offset += sizeof( ewf_section_descriptor_v1_t );
			}
		}
		else if( segment_file->major_version == 2 )
		{
			if( section_descriptor->type == LIBEWF_SECTION_TYPE_DEVICE_INFORMATION )
			{
				segment_file->device_information_section_index = section_index;
			}
			if( section_descriptor->type == LIBEWF_SECTION_TYPE_ENCRYPTION_KEYS )
			{
				segment_file->flags |= LIBEWF_SEGMENT_FILE_FLAG_IS_ENCRYPTED;
/* TODO get key info ? */
			}
			if( segment_file->last_section_offset == 0 )
			{
				if( section_descriptor->type == LIBEWF_SECTION_TYPE_NEXT )
				{
					last_section = 1;
				}
				else if( section_descriptor->type == LIBEWF_SECTION_TYPE_DONE )
				{
					last_section = 1;

					segment_file->flags |= LIBEWF_SEGMENT_FILE_FLAG_IS_LAST;
				}
				segment_file->last_section_offset = segment_file_offset;
			}
			if( libfdata_list_append_element(
			     segment_file->sections_list,
			     &element_index,
			     file_io_pool_entry,
			     segment_file_offset,
			     sizeof( ewf_section_descriptor_v2_t ),
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append element to sections list.",
				 function );

				goto on_error;
			}
			segment_file_offset -= section_descriptor->size;
		}
		section_index++;

		if( ( segment_file->major_version == 1 )
		 && ( last_section != 0 ) )
		{
			break;
		}
	}
	number_of_sections = section_index;

	if( libewf_section_descriptor_free(
	     &section_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free section.",
		 function );

		goto on_error;
	}
	if( ( segment_file->flags & LIBEWF_SEGMENT_FILE_FLAG_IS_CORRUPTED ) == 0 )
	{
		if( last_section == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing next or done section.",
			 function );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				if( ( error != NULL )
				 && ( *error != NULL ) )
				{
					libcnotify_print_error_backtrace(
					 *error );
				}
			}
#endif
			libcerror_error_free(
			 error );

			segment_file->flags |= LIBEWF_SEGMENT_FILE_FLAG_IS_CORRUPTED;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		else if( libcnotify_verbose != 0 )
		{
			if( (size64_t) segment_file_offset < segment_file_size )
			{
				libcnotify_printf(
				 "%s: trailing data in segment file: %" PRIu16 "\n",
				 function,
				 segment_file->segment_number );
			}
		}
#endif
	}
	if( segment_file->major_version == 2 )
	{
		/* Reversing the list after the last append is more efficient than using prepend
		 */
		if( libfdata_list_reverse(
		     segment_file->sections_list,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to reverse sections list.",
			 function );

			goto on_error;
		}
		if( segment_file->device_information_section_index != -1 )
		{
			segment_file->device_information_section_index = ( section_index - 1 )
			                                               - segment_file->device_information_section_index;
		}
	}
	if( io_handle->chunk_size != 0 )
	{
		if( libfcache_cache_initialize(
		     &sections_cache,
		     LIBEWF_MAXIMUM_CACHE_ENTRIES_SECTIONS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create section descriptor cache.",
			 function );

			goto on_error;
		}
		for( section_index = 0;
		     section_index < number_of_sections;
		     section_index++ )
		{
			if( libfdata_list_get_element_value_by_index(
			     segment_file->sections_list,
			     (intptr_t *) file_io_pool,
			     (libfdata_cache_t *) sections_cache,
			     section_index,
			     (intptr_t **) &section_descriptor,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve section: %d from sections list.",
				 function,
				 section_index );

				goto on_error;
			}
			result = libewf_section_get_data_offset(
			          section_descriptor,
			          segment_file->major_version,
			          &section_data_offset,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve section: %d data offset.",
				 function,
				 section_index );

				goto on_error;
			}
			if( section_descriptor->type == LIBEWF_SECTION_TYPE_SECTOR_TABLE )
			{
				if( result == 0 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: missing section: %d data offset.",
					 function,
					 section_index );

					goto on_error;
				}
				if( libewf_segment_file_seek_offset(
				     segment_file,
				     file_io_pool,
				     file_io_pool_entry,
				     section_data_offset,
				     error ) == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_OPEN_FAILED,
					 "%s: unable to seek section: %d data offset: %" PRIu64 ".",
					 function,
					 section_index,
					 section_data_offset );

					goto on_error;
				}
				read_count = libewf_segment_file_read_table_section(
					      segment_file,
					      section_descriptor,
					      file_io_pool,
					      file_io_pool_entry,
					      io_handle->chunk_size,
					      error );

				if( read_count == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read section: 0x%08" PRIx32 ".",
					 function,
					 section_descriptor->type );

					section_descriptor = NULL;

					goto on_error;
				}
			}
			else if( ( segment_file->major_version == 1 )
			      && ( ( section_descriptor->type_string_length == 6 )
			      && ( memory_compare(
			            (void *) section_descriptor->type_string,
			            (void *) "table2",
			            6 ) == 0 ) ) )
			{
				if( result == 0 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: missing section: %d data offset.",
					 function,
					 section_index );

					goto on_error;
				}
				if( libewf_segment_file_seek_offset(
				     segment_file,
				     file_io_pool,
				     file_io_pool_entry,
				     section_data_offset,
				     error ) == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_OPEN_FAILED,
					 "%s: unable to seek section: %d data offset: %" PRIu64 ".",
					 function,
					 section_index,
					 section_data_offset );

					goto on_error;
				}
				read_count = libewf_segment_file_read_table2_section(
					      segment_file,
					      section_descriptor,
					      file_io_pool,
					      file_io_pool_entry,
					      error );

				if( read_count == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read section: %s.",
					 function,
					 section_descriptor->type_string );

					section_descriptor = NULL;

					goto on_error;
				}
			}
			section_descriptor = NULL;
		}
		if( libfcache_cache_free(
		     &sections_cache,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free sections cache.",
			 function );

			goto on_error;
		}
	}
	if( libfdata_list_element_set_element_value(
	     element,
	     (intptr_t *) file_io_pool,
	     (libfdata_cache_t *) segment_file_cache,
	     (intptr_t *) segment_file,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_segment_file_free,
	     LIBFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set segment file as element value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( sections_cache != NULL )
	{
		libfcache_cache_free(
		 &sections_cache,
		 NULL );
	}
	if( section_descriptor != NULL )
	{
		libewf_section_descriptor_free(
		 &section_descriptor,
		 NULL );
	}
	if( segment_file != NULL )
	{
		libewf_segment_file_free(
		 &segment_file,
		 NULL );
	}
	return( -1 );
}

/* Reads a section
 * Callback function for the sections list
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_file_read_section_element_data(
     libewf_segment_file_t *segment_file,
     libbfio_pool_t *file_io_pool,
     libfdata_list_element_t *element,
     libfdata_cache_t *cache,
     int file_io_pool_entry,
     off64_t section_data_offset,
     size64_t section_data_size LIBEWF_ATTRIBUTE_UNUSED,
     uint32_t element_flags LIBEWF_ATTRIBUTE_UNUSED,
     uint8_t read_flags LIBEWF_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libewf_section_descriptor_t *section_descriptor = NULL;
	static char *function                           = "libewf_segment_file_read_section_element_data";
	ssize_t read_count                              = 0;

	LIBEWF_UNREFERENCED_PARAMETER( section_data_size )
	LIBEWF_UNREFERENCED_PARAMETER( element_flags )
	LIBEWF_UNREFERENCED_PARAMETER( read_flags )

	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
/* TODO validate section_data_size ? */
	if( libewf_section_descriptor_initialize(
	     &section_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create section descriptor.",
		 function );

		goto on_error;
	}
	read_count = libewf_section_descriptor_read_file_io_pool(
		      section_descriptor,
		      file_io_pool,
		      file_io_pool_entry,
		      section_data_offset,
		      segment_file->major_version,
		      error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read section descriptor.",
		 function );

		goto on_error;
	}
	segment_file->current_offset = section_data_offset + read_count;

	if( libfdata_list_element_set_element_value(
	     element,
	     (intptr_t *) file_io_pool,
	     cache,
	     (intptr_t *) section_descriptor,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_section_descriptor_free,
	     LIBFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set section as element value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( section_descriptor != NULL )
	{
		libewf_section_descriptor_free(
		 &section_descriptor,
		 NULL );
	}
	return( -1 );
}

/* Reads a chunk group
 * Callback function for the chunk groups list
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_file_read_chunk_group_element_data(
     libewf_segment_file_t *segment_file,
     libbfio_pool_t *file_io_pool,
     libfdata_list_element_t *element,
     libfdata_cache_t *cache,
     int file_io_pool_entry,
     off64_t chunk_group_data_offset,
     size64_t chunk_group_data_size,
     uint32_t element_flags LIBEWF_ATTRIBUTE_UNUSED,
     uint8_t read_flags LIBEWF_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libewf_chunk_group_t *chunk_group               = NULL;
	libewf_section_descriptor_t *section_descriptor = NULL;
	libewf_table_section_t *table_section           = NULL;
	static char *function                           = "libewf_segment_file_read_chunk_group_element_data";
	ssize_t read_count                              = 0;
	uint64_t chunk_index                            = 0;
	int result                                      = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	size64_t chunk_group_range_size                 = 0;
	off64_t chunk_group_range_offset                = 0;
	int element_index                               = 0;
#endif

	LIBEWF_UNREFERENCED_PARAMETER( element_flags )
	LIBEWF_UNREFERENCED_PARAMETER( read_flags )

	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( segment_file->chunk_groups_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment file - missing chunk groups list.",
		 function );

		return( -1 );
	}
	if( segment_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( segment_file->io_handle->chunk_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment file - invalid IO handle - missing chunk size.",
		 function );

		return( -1 );
	}
	if( libewf_section_descriptor_initialize(
	     &section_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create section descriptor.",
		 function );

		goto on_error;
	}
	if( segment_file->major_version == 1 )
	{
		read_count = libewf_section_descriptor_read_file_io_pool(
			      section_descriptor,
			      file_io_pool,
			      file_io_pool_entry,
			      chunk_group_data_offset,
			      segment_file->major_version,
			      error );

		if( read_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read section descriptor.",
			 function );

			goto on_error;
		}
		segment_file->current_offset = chunk_group_data_offset + read_count;

		if( chunk_group_data_size != section_descriptor->size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid chunk group data size value out of bounds.",
			 function );

			goto on_error;
		}
		chunk_group_data_size -= read_count;
	}
	else if( segment_file->major_version == 2 )
	{
		if( chunk_group_data_size > (size64_t) UINT32_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid chunk group data size value out of bounds.",
			 function );

			goto on_error;
		}
		if( libbfio_pool_seek_offset(
		     file_io_pool,
		     file_io_pool_entry,
		     chunk_group_data_offset,
		     SEEK_SET,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek chunk table offset: %" PRIi64 " in file IO pool entry: %d.",
			 function,
			 chunk_group_data_offset,
			 file_io_pool_entry );

			goto on_error;
		}
		segment_file->current_offset     = chunk_group_data_offset;
		section_descriptor->start_offset = chunk_group_data_offset;
		section_descriptor->data_size    = (uint32_t) chunk_group_data_size;
	}
	if( libewf_table_section_initialize(
	     &table_section,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create table section.",
		 function );

		goto on_error;
	}
	read_count = libewf_table_section_read_file_io_pool(
	              table_section,
	              segment_file->io_handle,
	              file_io_pool,
	              file_io_pool_entry,
	              segment_file->major_version,
	              segment_file->type,
	              section_descriptor->data_size,
	              section_descriptor->data_flags,
	              error );

	if( read_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read table section.",
		 function );

		goto on_error;
	}
	segment_file->current_offset += read_count;
	chunk_group_data_size        -= read_count;

	if( table_section->number_of_entries == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_INVALID_DATA,
		 "%s: invalid number of entries.",
		 function );

		goto on_error;
	}
	if( libewf_chunk_group_initialize(
	     &chunk_group,
	     segment_file->io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create chunk group.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libfdata_list_element_get_element_index(
	     element,
	     &element_index,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve chunk group list element index.",
		 function );

		goto on_error;
	}
	if( libfdata_list_get_element_mapped_range(
	     segment_file->chunk_groups_list,
	     element_index,
	     &chunk_group_range_offset,
	     &chunk_group_range_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve chunk group list element: %d mapped range.",
		 function,
		 element_index );

		goto on_error;
	}
	chunk_index = ( segment_file->range_start_offset + chunk_group_range_offset ) / segment_file->io_handle->chunk_size;

#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( segment_file->major_version == 1 )
	{
		result = libewf_chunk_group_fill_v1(
			  chunk_group,
			  chunk_index,
			  segment_file->io_handle->chunk_size,
			  file_io_pool_entry,
			  section_descriptor,
			  (off64_t) table_section->base_offset,
			  table_section->number_of_entries,
			  &( table_section->section_data[ table_section->entries_offset ] ),
			  table_section->entries_size,
			  table_section->entries_corrupted,
			  error );
	}
	else if( segment_file->major_version == 2 )
	{
		result = libewf_chunk_group_fill_v2(
			  chunk_group,
			  chunk_index,
			  segment_file->io_handle->chunk_size,
			  file_io_pool_entry,
			  section_descriptor,
			  table_section->number_of_entries,
			  &( table_section->section_data[ table_section->entries_offset ] ),
			  table_section->entries_size,
			  table_section->entries_corrupted,
			  error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to fill chunks list.",
		 function );

		goto on_error;
	}
	if( libewf_table_section_free(
	     &table_section,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free table section.",
		 function );

		goto on_error;
	}
	if( libewf_section_descriptor_free(
	     &section_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free section.",
		 function );

		goto on_error;
	}
/* TODO read and process backup range if available and neccessary
	if( table_section->entries_corrupted != 0 )
	{
		if( segment_file->major_version == 1 )
		{
			result = libewf_chunk_group_correct_v1(
				  chunk_group,
			          chunk_index,
			          segment_file->io_handle->chunk_size,
				  file_io_pool_entry,
				  section_descriptor,
				  (off64_t) table_section->base_offset,
				  table_section->number_of_entries,
				  &( table_section->section_data[ table_section->entries_offset ] ),
				  table_section->entries_size,
				  table_section->entries_corrupted,
				  error );
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to correct chunks list.",
			 function );

			goto on_error;
		}
	}
*/
	if( libfdata_list_element_set_element_value(
	     element,
	     (intptr_t *) file_io_pool,
	     cache,
	     (intptr_t *) chunk_group,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_chunk_group_free,
	     LIBFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set section as element value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( chunk_group != NULL )
	{
		libewf_chunk_group_free(
		 &chunk_group,
		 NULL );
	}
	if( table_section != NULL )
	{
		libewf_table_section_free(
		 &table_section,
		 NULL );
	}
	if( section_descriptor != NULL )
	{
		libewf_section_descriptor_free(
		 &section_descriptor,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the chunk group at a specific offset
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libewf_segment_file_get_chunk_group_by_offset(
     libewf_segment_file_t *segment_file,
     libbfio_pool_t *file_io_pool,
     off64_t offset,
     int *chunk_group_index,
     off64_t *chunk_group_data_offset,
     libewf_chunk_group_t **chunk_group,
     libcerror_error_t **error )
{
	libewf_chunk_group_t *safe_chunk_group = NULL;
	static char *function                  = "libewf_segment_file_get_chunk_group_by_offset";
	off64_t safe_chunk_group_data_offset   = 0;
	int result                             = 0;
	int safe_chunk_group_index             = 0;

	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( chunk_group_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk group index.",
		 function );

		return( -1 );
	}
	if( chunk_group_data_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk group data offset.",
		 function );

		return( -1 );
	}
	if( chunk_group == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk group.",
		 function );

		return( -1 );
	}
	result = libfdata_list_get_element_value_at_offset(
		  segment_file->chunk_groups_list,
		  (intptr_t *) file_io_pool,
		  (libfdata_cache_t *) segment_file->chunk_groups_cache,
		  offset,
		  &safe_chunk_group_index,
		  &safe_chunk_group_data_offset,
		  (intptr_t **) &safe_chunk_group,
		  0,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve chunks list at offset: 0x%08" PRIx64 ".",
		 function,
		 offset );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( safe_chunk_group == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing chunk group.",
			 function );

			return( -1 );
		}
		segment_file->current_chunk_group_index = safe_chunk_group_index;

		if( libfdata_list_get_element_mapped_range(
		     segment_file->chunk_groups_list,
		     safe_chunk_group_index,
		     &( safe_chunk_group->range_start_offset ),
		     (size64_t *) &( safe_chunk_group->range_end_offset ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve chunks list element: %d mapped range.",
			 function,
			 safe_chunk_group_index );

			return( -1 );
		}
		safe_chunk_group->range_start_offset += segment_file->range_start_offset;
		safe_chunk_group->range_end_offset   += safe_chunk_group->range_start_offset;

		*chunk_group_index       = safe_chunk_group_index;
		*chunk_group_data_offset = safe_chunk_group_data_offset;
		*chunk_group             = safe_chunk_group;
	}
	return( result );
}

