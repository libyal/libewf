/*
 * Segment file reading/writing functions
 *
 * Copyright (c) 2006-2012, Joachim Metz <jbmetz@users.sourceforge.net>
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include <liberror.h>
#include <libnotify.h>

#include "libewf_chunk_table.h"
#include "libewf_definitions.h"
#include "libewf_hash_values.h"
#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_libmfcache.h"
#include "libewf_libmfdata.h"
#include "libewf_list_type.h"
#include "libewf_section.h"
#include "libewf_sector_list.h"
#include "libewf_segment_file.h"
#include "libewf_segment_table.h"
#include "libewf_single_files.h"
#include "libewf_unused.h"

#include "ewf_definitions.h"
#include "ewf_file_header.h"
#include "ewf_section.h"
#include "ewfx_delta_chunk.h"

const uint8_t dvf_file_signature[ 8 ] = { 0x64, 0x76, 0x66, 0x09, 0x0d, 0x0a, 0xff, 0x00 };
const uint8_t evf_file_signature[ 8 ] = { 0x45, 0x56, 0x46, 0x09, 0x0d, 0x0a, 0xff, 0x00 };
const uint8_t lvf_file_signature[ 8 ] = { 0x4c, 0x56, 0x46, 0x09, 0x0d, 0x0a, 0xff, 0x00 };

/* Initialize the segment file
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_file_initialize(
     libewf_segment_file_t **segment_file,
     liberror_error_t **error )
{
	static char *function = "libewf_segment_file_initialize";

	if( segment_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( *segment_file != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid segment file value already set.",
		 function );

		return( -1 );
	}
	*segment_file = memory_allocate_structure(
	                 libewf_segment_file_t );

	if( *segment_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create segment file.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *segment_file,
	     0,
	     sizeof( libewf_segment_file_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear segment file.",
		 function );

		goto on_error;
	}
	if( libewf_list_initialize(
	     &( ( *segment_file )->section_list ),
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create section list.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *segment_file != NULL )
	{
		memory_free(
		 *segment_file );

		*segment_file = NULL;
	}
	return( -1 );
}

/* Frees the segment file including elements
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_file_free(
     libewf_segment_file_t **segment_file,
     liberror_error_t **error )
{
	static char *function = "libewf_segment_file_free";
	int result            = 1;

	if( segment_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( *segment_file != NULL )
	{
		if( ( *segment_file )->section_list != NULL )
		{
			if( libewf_list_free(
			     &( ( *segment_file )->section_list ),
			     (int (*)(intptr_t **, liberror_error_t **)) &libewf_section_free,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free section list.",
				 function );

				result = -1;
			}
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
     liberror_error_t **error )
{
	static char *function = "libewf_segment_file_clone";

	if( destination_segment_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination segment file.",
		 function );

		return( -1 );
	}
	if( *destination_segment_file != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination segment file.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_segment_file,
	     source_segment_file,
	     sizeof( libewf_segment_file_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination segment file.",
		 function );

		goto on_error;
	}
	( *destination_segment_file )->section_list = NULL;

	if( libewf_list_clone(
	     &( ( *destination_segment_file )->section_list ),
	     source_segment_file->section_list,
	     (int (*)(intptr_t **, liberror_error_t **)) &libewf_section_free,
	     (int(*)(intptr_t **, intptr_t *, liberror_error_t **)) &libewf_section_clone,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination section list.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_segment_file != NULL )
	{
		memory_free(
		 *destination_segment_file );

		*destination_segment_file = NULL;
	}
	return( -1 );
}

/* Reads the segment file header
 * Returns the number of bytes read if successful, or -1 on error
 */
ssize_t libewf_segment_file_read_file_header(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         liberror_error_t **error )
{
	ewf_file_header_t file_header;

	static char *function = "libewf_segment_file_read_file_header";
	ssize_t read_count    = 0;

	if( segment_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
	 	"%s: reading file header at offset: 0 (0x00000000)\n",
		 function );
	}
#endif
	if( libbfio_pool_seek_offset(
	     file_io_pool,
	     file_io_pool_entry,
	     0,
	     SEEK_SET,
	     error ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek file header offset: 0.",
		 function );

		return( -1 );
	}
	read_count = libbfio_pool_read(
	              file_io_pool,
	              file_io_pool_entry,
	              (uint8_t *) &file_header,
	              sizeof( ewf_file_header_t ),
	              error );

	if( read_count != (ssize_t) sizeof( ewf_file_header_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
	 	 "%s: file header:\n",
		 function );
		libnotify_print_data(
		 (uint8_t *) &file_header,
		 sizeof( ewf_file_header_t ),
		 0 );
	}
#endif
	byte_stream_copy_to_uint16_little_endian(
	 file_header.fields_segment,
	 segment_file->segment_number );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
	 	 "%s: signature:\n",
		 function );
		libnotify_print_data(
		 file_header.signature,
		 8,
		 0 );

		libnotify_printf(
	 	 "%s: segment number\t\t\t: %" PRIu16 "\n",
		 function,
		 segment_file->segment_number );

		libnotify_printf(
	 	 "\n" );
	}
#endif
	/* Compare the most common signature first
	 */
	if( memory_compare(
	     evf_file_signature,
	     file_header.signature,
	     8 ) == 0 )
	{
		segment_file->type = LIBEWF_SEGMENT_FILE_TYPE_EWF;
	}
	else if( memory_compare(
	          lvf_file_signature,
	          file_header.signature,
	          8 ) == 0 )
	{
		segment_file->type = LIBEWF_SEGMENT_FILE_TYPE_LWF;
	}
	else if( memory_compare(
	          dvf_file_signature,
	          file_header.signature,
	          8 ) == 0 )
	{
		segment_file->type = LIBEWF_SEGMENT_FILE_TYPE_DWF;
	}
	else
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_INPUT,
		 LIBERROR_INPUT_ERROR_SIGNATURE_MISMATCH,
		 "%s: unsupported segment file signature.",
		 function );

		return( -1 );
	}
	return( read_count );
}

/* Writes the segment file header
 * Returns the number of bytes written if successful, or -1 on error
 */
ssize_t libewf_segment_file_write_file_header(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         liberror_error_t **error )
{
	ewf_file_header_t file_header;

	static char *function         = "libewf_segment_file_write_file_header";
	const uint8_t *file_signature = NULL;
	ssize_t write_count           = 0;

	if( segment_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( segment_file->segment_number == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid segment file - segment number value out of bounds.",
		 function );

		return( -1 );
	}
	if( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF )
	{
		file_signature = evf_file_signature;
	}
	else if( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_LWF )
	{
		file_signature = lvf_file_signature;
	}
	else if( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_DWF )
	{
		file_signature = dvf_file_signature;
	}
	else
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported segment file type.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     file_header.signature,
	     file_signature,
	     8 ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set file signature.",
		 function );

		return( -1 );
	}
	byte_stream_copy_from_uint16_little_endian(
	 file_header.fields_segment,
	 segment_file->segment_number );

	file_header.fields_start    = 1;
	file_header.fields_end[ 0 ] = 0;
	file_header.fields_end[ 1 ] = 0;

	write_count = libbfio_pool_write(
	               file_io_pool,
	               file_io_pool_entry,
	               (uint8_t *) &file_header,
	               sizeof( ewf_file_header_t ),
	               error );

	if( write_count != (ssize_t) sizeof( ewf_file_header_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write file header.",
		 function );

		return( -1 );
	}
	return( write_count );
}

/* Reads a segment file
 * Callback function for the segment files list
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_file_read(
     intptr_t *io_handle LIBEWF_ATTRIBUTE_UNUSED,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     libmfdata_file_t *file,
     libmfcache_cache_t *cache,
     uint8_t read_flags LIBEWF_ATTRIBUTE_UNUSED,
     liberror_error_t **error )
{
	libewf_section_t *section           = NULL;
	libewf_segment_file_t *segment_file = NULL;
	static char *function               = "libewf_segment_file_read";
	off64_t section_offset              = 0;
	size64_t segment_file_size          = 0;
	ssize_t read_count                  = 0;
	int last_section                    = 0;

	LIBEWF_UNREFERENCED_PARAMETER( io_handle )
	LIBEWF_UNREFERENCED_PARAMETER( read_flags )

	if( libewf_segment_file_initialize(
	     &segment_file,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create segment file.",
		 function );

		goto on_error;
	}
	if( libbfio_pool_get_size(
	     file_io_pool,
	     file_io_pool_entry,
	     &segment_file_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of file IO pool entry: %d.",
		 function,
		 file_io_pool_entry );

		goto on_error;
	}
	read_count = libewf_segment_file_read_file_header(
		      segment_file,
		      file_io_pool,
		      file_io_pool_entry,
		      error );

	if( read_count == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read segment file header.",
		 function );

		goto on_error;
	}
	section_offset = (off64_t) read_count;

	while( (size64_t) section_offset < segment_file_size )
	{
		if( libewf_section_initialize(
		     &section,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create section.",
			 function );

			goto on_error;
		}
		read_count = libewf_section_start_read(
		              section,
		              file_io_pool,
		              file_io_pool_entry,
		              section_offset,
		              error );

		if( read_count == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read section start.",
			 function );

			goto on_error;
		}
		if( section->type_length == 4 )
		{
			if( memory_compare(
			     (void *) section->type,
			     (void *) "done",
			     4 ) == 0 )
			{
				last_section = 1;
			}
			else if( memory_compare(
				  (void *) section->type,
				  (void *) "next",
				  4 ) == 0 )
			{
				last_section = 1;
			}
		}
		section_offset += section->size;

		if( libewf_list_append_value(
		     segment_file->section_list,
		     (intptr_t *) section,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append section to list.",
			 function );

			goto on_error;
		}
		section = NULL;

		if( last_section != 0 )
		{
			break;
		}
	}
	if( last_section == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing next or done section.",
		 function );

		goto on_error;
	}
	if( libmfdata_file_set_file_value(
	     file,
	     cache,
	     (intptr_t *) segment_file,
	     (int (*)(intptr_t **, liberror_error_t **)) &libewf_segment_file_free,
	     LIBMFDATA_FILE_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set segment file as file value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( section != NULL )
	{
		libewf_section_free(
		 &section,
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

/* Reads the table section
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_segment_file_read_table_section(
         libewf_segment_file_t *segment_file,
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_media_values_t *media_values,
         libewf_chunk_table_t *chunk_table,
         libmfdata_list_t *chunk_table_list,
         liberror_error_t **error )
{
	static char *function      = "libewf_segment_file_read_table_section";
	ssize_t read_count         = 0;
	uint64_t base_offset       = 0;
	uint32_t number_of_offsets = 0;

	if( segment_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( chunk_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk table.",
		 function );

		return( -1 );
	}
	chunk_table->previous_last_chunk_filled = chunk_table->last_chunk_filled;

	read_count = libewf_section_table_header_read(
	              section,
	              file_io_pool,
	              file_io_pool_entry,
	              io_handle->format,
	              &number_of_offsets,
	              &base_offset,
	              error );
	
	if( read_count == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read table section header.",
		 function );

		return( -1 );
	}
	if( number_of_offsets > 0 )
	{
		/* The EWF-L01 does not define the number of chunks in the volume
		 */
		if( media_values->number_of_chunks == 0 )
		{
			if( libmfdata_list_append_group(
			     chunk_table_list,
			     &( chunk_table->last_chunk_filled ),
			     (int) number_of_offsets,
			     file_io_pool_entry,
			     section->start_offset,
			     section->size,
			     0,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append chunk group.",
				 function );

				return( -1 );
			}
		}
		else
		{
			if( libmfdata_list_set_group_by_index(
			     chunk_table_list,
			     chunk_table->last_chunk_filled,
			     (int) number_of_offsets,
			     file_io_pool_entry,
			     section->start_offset,
			     section->size,
			     0,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set chunk group: %d - %d.",
				 function,
				 chunk_table->last_chunk_filled,
				 chunk_table->last_chunk_filled + number_of_offsets );

				return( -1 );
			}
		}
		chunk_table->last_chunk_filled += (int) number_of_offsets;

		segment_file->number_of_chunks += number_of_offsets;
	}
	return( 1 );
}

/* Reads the table2 section
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_segment_file_read_table2_section(
         libewf_segment_file_t *segment_file,
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_chunk_table_t *chunk_table,
         libmfdata_list_t *chunk_table_list,
         liberror_error_t **error )
{
	static char *function        = "libewf_segment_file_read_table2_section";
	off64_t group_offset         = 0;
	size64_t group_size          = 0;
	ssize_t read_count           = 0;
	uint64_t base_offset         = 0;
	uint32_t group_flags         = 0;
	uint32_t number_of_offsets   = 0;
	int group_number_of_offsets  = 0;
	int group_file_io_pool_entry = 0;

	if( segment_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( chunk_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk table.",
		 function );

		return( -1 );
	}
	read_count = libewf_section_table_header_read(
	              section,
	              file_io_pool,
	              file_io_pool_entry,
	              io_handle->format,
	              &number_of_offsets,
	              &base_offset,
	              error );
	
	if( read_count == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read table2 section header.",
		 function );

		return( -1 );
	}
	if( number_of_offsets > 0 )
	{
		if( libmfdata_list_get_group_by_index(
		     chunk_table_list,
		     chunk_table->last_chunk_compared,
		     &group_number_of_offsets,
		     &group_file_io_pool_entry,
		     &group_offset,
		     &group_size,
		     &group_flags,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve chunk group: %d - %d.",
			 function,
			 chunk_table->last_chunk_compared,
			 chunk_table->last_chunk_compared + number_of_offsets );

			return( -1 );
		}
		if( (int) number_of_offsets != group_number_of_offsets )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_INPUT,
			 LIBERROR_INPUT_ERROR_VALUE_MISMATCH,
			 "%s: mismatch between number of offsets in table and table2.",
			 function );

			return( -1 );
		}
		if( libmfdata_list_set_backup_data_range_by_index(
		     chunk_table_list,
		     chunk_table->last_chunk_compared,
		     file_io_pool_entry,
		     section->start_offset,
		     section->size,
		     0,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set backup data range of chunk group: %d - %d.",
			 function,
			 chunk_table->last_chunk_compared,
			 chunk_table->last_chunk_compared + number_of_offsets );

			return( -1 );
		}
		chunk_table->last_chunk_compared += (int) number_of_offsets;
	}
	return( 1 );
}

/* Reads a volume section
 * Returns the number of bytes read if successful or -1 on error
 */
ssize_t libewf_segment_file_read_volume_section(
         libewf_segment_file_t *segment_file,
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_media_values_t *media_values,
         libmfdata_list_t *chunk_table_list,
         liberror_error_t **error )
{
	static char *function = "libewf_segment_file_read_volume_section";
	ssize_t read_count    = 0;

	if( segment_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	read_count = libewf_section_volume_read(
		      section,
		      io_handle,
		      file_io_pool,
		      file_io_pool_entry,
		      media_values,
		      error );

	if( read_count == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume section.",
		 function );

		return( -1 );
	}
	if( media_values->number_of_chunks > 0 )
	{
		if( libmfdata_list_resize(
		     chunk_table_list,
		     (int) media_values->number_of_chunks,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize chunk table list.",
			 function );

			return( -1 );
		}
	}
	return( read_count );
}

/* Reads the delta chunk section
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_segment_file_read_delta_chunk_section(
         libewf_segment_file_t *segment_file,
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libmfdata_list_t *chunk_table_list,
         liberror_error_t **error )
{
	static char *function = "libewf_segment_file_read_delta_chunk_section";
	off64_t chunk_offset  = 0;
	ssize_t read_count    = 0;
	uint32_t chunk_index  = 0;
	uint32_t chunk_size   = 0;

	if( segment_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	read_count = libewf_section_delta_chunk_read(
	              section,
	              file_io_pool,
	              file_io_pool_entry,
	              &chunk_index,
	              &chunk_size,
	              error );
	
	if( read_count == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read delta chunk section.",
		 function );

		return( -1 );
	}
	chunk_offset = section->start_offset
	             + sizeof( ewf_section_start_t )
	             + sizeof( ewfx_delta_chunk_header_t );

	if( libmfdata_list_set_element_by_index(
	     chunk_table_list,
	     (int) chunk_index,
	     file_io_pool_entry,
	     chunk_offset,
	     (size64_t) chunk_size,
	     0,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set chunk: %" PRIu32 ".",
		 function,
		 chunk_index );

		return( -1 );
	}
	return( 1 );
}

/* Write the headers to file
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_headers(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_header_sections_t *header_sections,
         liberror_error_t **error )
{
	libewf_section_t *section = NULL;
	static char *function     = "libewf_segment_file_write_headers";
	ssize_t write_count       = 0;
	ssize_t total_write_count = 0;

	if( segment_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( header_sections == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header sections.",
		 function );

		return( -1 );
	}
	if( ( header_sections->header == NULL )
	 || ( header_sections->header_size == 0 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid header.",
		 function );

		return( -1 );
	}
	if( ( io_handle->format == LIBEWF_FORMAT_EWF )
	 || ( io_handle->format == LIBEWF_FORMAT_SMART )
	 || ( io_handle->format == LIBEWF_FORMAT_ENCASE1 ) )
	{
		/* The header should be written only once
		 * and using the compression used in the file
		 */
		if( libewf_section_initialize(
		     &section,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create section.",
			 function );

			goto on_error;
		}
		write_count = libewf_section_header_write(
		               section,
		               file_io_pool,
		               file_io_pool_entry,
		               section_offset,
		               header_sections,
		               io_handle->compression_level,
		               error );

		if( write_count == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write single header section.",
			 function );

			goto on_error;
		}
		total_write_count += write_count;
		section_offset    += write_count;

		if( libewf_list_append_value(
		     segment_file->section_list,
		     (intptr_t *) section,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append section to list.",
			 function );

			goto on_error;
		}
		section = NULL;
	}
	else if( ( io_handle->format == LIBEWF_FORMAT_ENCASE2 )
	      || ( io_handle->format == LIBEWF_FORMAT_ENCASE3 )
	      || ( io_handle->format == LIBEWF_FORMAT_LINEN5 )
	      || ( io_handle->format == LIBEWF_FORMAT_LINEN6 )
	      || ( io_handle->format == LIBEWF_FORMAT_FTK ) )
	{
		/* The header should be written twice
		 * the default compression is used
		 */
		if( libewf_section_initialize(
		     &section,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create section.",
			 function );

			goto on_error;
		}
		write_count = libewf_section_header_write(
		               section,
		               file_io_pool,
		               file_io_pool_entry,
		               section_offset,
		               header_sections,
		               EWF_COMPRESSION_DEFAULT,
		               error );

		if( write_count == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write first header section.",
			 function );

			goto on_error;
		}
		total_write_count += write_count;
		section_offset    += write_count;

		if( libewf_list_append_value(
		     segment_file->section_list,
		     (intptr_t *) section,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append section to list.",
			 function );

			goto on_error;
		}
		section = NULL;

		if( libewf_section_initialize(
		     &section,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create section.",
			 function );

			goto on_error;
		}
		write_count = libewf_section_header_write(
		               section,
		               file_io_pool,
		               file_io_pool_entry,
		               section_offset,
		               header_sections,
		               EWF_COMPRESSION_DEFAULT,
		               error );

		if( write_count == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write second header section.",
			 function );

			goto on_error;
		}
		total_write_count += write_count;
		section_offset    += write_count;

		if( libewf_list_append_value(
		     segment_file->section_list,
		     (intptr_t *) section,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append section to list.",
			 function );

			goto on_error;
		}
		section = NULL;
	}
	else if( ( io_handle->format == LIBEWF_FORMAT_ENCASE4 )
	      || ( io_handle->format == LIBEWF_FORMAT_ENCASE5 )
	      || ( io_handle->format == LIBEWF_FORMAT_ENCASE6 ) )
	{
		if( ( header_sections->header2 == NULL )
		 && ( header_sections->header2_size == 0 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid header2.",
			 function );

			goto on_error;
		}
		/* The header2 should be written twice
		 * the default compression is used
		 */
		if( libewf_section_initialize(
		     &section,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create section.",
			 function );

			goto on_error;
		}
		write_count = libewf_section_header2_write(
		               section,
		               file_io_pool,
		               file_io_pool_entry,
		               section_offset,
		               header_sections,
		               EWF_COMPRESSION_DEFAULT,
		               error );

		if( write_count == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write first header2 section.",
			 function );

			goto on_error;
		}
		total_write_count += write_count;
		section_offset    += write_count;

		if( libewf_list_append_value(
		     segment_file->section_list,
		     (intptr_t *) section,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append section to list.",
			 function );

			goto on_error;
		}
		section = NULL;

		if( libewf_section_initialize(
		     &section,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create section.",
			 function );

			goto on_error;
		}
		write_count = libewf_section_header2_write(
		               section,
		               file_io_pool,
		               file_io_pool_entry,
		               section_offset,
		               header_sections,
		               EWF_COMPRESSION_DEFAULT,
		               error );

		if( write_count == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write second header2 section.",
			 function );

			goto on_error;
		}
		total_write_count += write_count;
		section_offset    += write_count;

		if( libewf_list_append_value(
		     segment_file->section_list,
		     (intptr_t *) section,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append section to list.",
			 function );

			goto on_error;
		}
		section = NULL;

		/* The header should be written once
		 * the default compression is used
		 */
		if( libewf_section_initialize(
		     &section,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create section.",
			 function );

			goto on_error;
		}
		write_count = libewf_section_header_write(
		               section,
		               file_io_pool,
		               file_io_pool_entry,
		               section_offset,
		               header_sections,
		               EWF_COMPRESSION_DEFAULT,
		               error );

		if( write_count == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write third header section.",
			 function );

			goto on_error;
		}
		total_write_count += write_count;
		section_offset    += write_count;

		if( libewf_list_append_value(
		     segment_file->section_list,
		     (intptr_t *) section,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append section to list.",
			 function );

			goto on_error;
		}
		section = NULL;
	}
	/* EWFX uses the header and header2 for backwards compatibility
	 */
	else if( io_handle->format == LIBEWF_FORMAT_EWFX )
	{
		if( ( header_sections->xheader == NULL )
		 && ( header_sections->xheader_size == 0 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid xheader.",
			 function );

			goto on_error;
		}
		if( ( header_sections->header2 == NULL )
		 && ( header_sections->header2_size == 0 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid header2.",
			 function );

			goto on_error;
		}
		/* The xheader should be written once
		 * the default compression is used
		 */
		if( libewf_section_initialize(
		     &section,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create section.",
			 function );

			goto on_error;
		}
		write_count = libewf_section_xheader_write(
		               section,
		               file_io_pool,
		               file_io_pool_entry,
		               section_offset,
		               header_sections,
		               EWF_COMPRESSION_DEFAULT,
		               error );

		if( write_count == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write xheader section.",
			 function );

			goto on_error;
		}
		total_write_count += write_count;
		section_offset    += write_count;

		if( libewf_list_append_value(
		     segment_file->section_list,
		     (intptr_t *) section,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append section to list.",
			 function );

			goto on_error;
		}
		section = NULL;

		/* The header2 should be written once
		 * the default compression is used
		 */
		if( libewf_section_initialize(
		     &section,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create section.",
			 function );

			goto on_error;
		}
		write_count = libewf_section_header2_write(
		               section,
		               file_io_pool,
		               file_io_pool_entry,
		               section_offset,
		               header_sections,
		               EWF_COMPRESSION_DEFAULT,
		               error );

		if( write_count == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write second header2 section.",
			 function );

			goto on_error;
		}
		total_write_count += write_count;
		section_offset    += write_count;

		if( libewf_list_append_value(
		     segment_file->section_list,
		     (intptr_t *) section,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append section to list.",
			 function );

			goto on_error;
		}
		section = NULL;

		/* The header should be written once
		 * the default compression is used
		 */
		if( libewf_section_initialize(
		     &section,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create section.",
			 function );

			goto on_error;
		}
		write_count = libewf_section_header_write(
		               section,
		               file_io_pool,
		               file_io_pool_entry,
		               section_offset,
		               header_sections,
		               EWF_COMPRESSION_DEFAULT,
		               error );

		if( write_count == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write third header section.",
			 function );

			goto on_error;
		}
		total_write_count += write_count;
		section_offset    += write_count;

		if( libewf_list_append_value(
		     segment_file->section_list,
		     (intptr_t *) section,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append section to list.",
			 function );

			goto on_error;
		}
		section = NULL;
	}
	return( total_write_count );

on_error:
	if( section != NULL )
	{
		libewf_section_free(
		 &section,
		 NULL );
	}
	return( -1 );
}

/* Write the last section at the end of the segment file
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_last_section(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         int last_segment_file,
         liberror_error_t **error )
{
	libewf_section_t *section  = NULL;
	uint8_t *last_section_type = NULL;
	static char *function      = "libewf_segment_file_write_last_section";
	ssize_t write_count        = 0;

	if( segment_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( last_segment_file == 0 )
	{
		last_section_type = (uint8_t *) "next";
	}
	else
	{
		last_section_type = (uint8_t *) "done";
	}
	/* Write next or done section
	 */
	if( libewf_section_initialize(
	     &section,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create section.",
		 function );

		goto on_error;
	}
	write_count = libewf_section_last_write(
		       section,
		       file_io_pool,
		       file_io_pool_entry,
		       last_section_type,
		       4,
		       section_offset,
		       io_handle->ewf_format,
	               error );

	if( write_count == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write %s section.",
		 function,
		 (char *) last_section_type );

		goto on_error;
	}
	if( libewf_list_append_value(
	     segment_file->section_list,
	     (intptr_t *) section,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append section to list.",
		 function );

		goto on_error;
	}
	section = NULL;

	return( write_count );

on_error:
	if( section != NULL )
	{
		libewf_section_free(
		 &section,
		 NULL );
	}
	return( -1 );
}

/* Write the necessary sections at the start of the segment file
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_start(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_media_values_t *media_values,
         libewf_header_sections_t *header_sections,
         ewf_data_t **cached_data_section,
         liberror_error_t **error )
{
	libewf_section_t *section = NULL;
	static char *function     = "libewf_segment_file_write_start";
	off64_t section_offset    = 0;
	ssize_t total_write_count = 0;
	ssize_t write_count       = 0;

	if( segment_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	write_count = libewf_segment_file_write_file_header(
	               segment_file,
	               file_io_pool,
	               file_io_pool_entry,
	               error );

	if( write_count != (ssize_t) sizeof( ewf_file_header_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write file header.",
		 function );

		return( -1 );
	}
	section_offset    += write_count;
	total_write_count += write_count;

	if( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF )
	{
		if( segment_file->segment_number == 1 )
		{
			write_count = libewf_segment_file_write_headers(
				       segment_file,
				       io_handle,
				       file_io_pool,
				       file_io_pool_entry,
				       section_offset,
				       header_sections,
			               error );

			if( write_count == -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write header sections.",
				 function );

				return( -1 );
			}
			total_write_count += write_count;
			section_offset    += write_count;
		}
		if( libewf_section_initialize(
		     &section,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create section.",
			 function );

			goto on_error;
		}
		if( segment_file->segment_number == 1 )
		{
			write_count = -1;

			if( io_handle->ewf_format == EWF_FORMAT_E01 )
			{
				write_count = libewf_section_volume_e01_write(
					       section,
					       io_handle,
					       file_io_pool,
					       file_io_pool_entry,
					       section_offset,
					       media_values,
				               error );
			}
			else if( io_handle->ewf_format == EWF_FORMAT_S01 )
			{
				write_count = libewf_section_volume_s01_write(
					       section,
					       io_handle,
					       file_io_pool,
					       file_io_pool_entry,
					       section_offset,
					       media_values,
				               error );
			}
			if( write_count == -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write volume section.",
				 function );

				goto on_error;
			}
		}
		else if( io_handle->ewf_format == EWF_FORMAT_E01 )
		{
			write_count = libewf_section_data_write(
				       section,
				       io_handle,
				       file_io_pool,
				       file_io_pool_entry,
				       section_offset,
				       media_values,
				       cached_data_section,
			               error );

			if( write_count == -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write data section.",
				 function );

				return( -1 );
			}
		}
		total_write_count += write_count;
		section_offset    += write_count;

		if( libewf_list_append_value(
		     segment_file->section_list,
		     (intptr_t *) section,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append section to list.",
			 function );

			goto on_error;
		}
		section = NULL;
	}
	return( total_write_count );

on_error:
	if( section != NULL )
	{
		libewf_section_free(
		 &section,
		 NULL );
	}
	return( -1 );
}

/* Write the necessary sections before the actual data chunks to file
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_chunks_section_start(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libmfdata_list_t *chunk_table_list,
         ewf_table_offset_t *table_offsets,
         uint32_t number_of_table_offsets,
         uint32_t number_of_chunks_written,
         uint32_t chunks_per_section,
         liberror_error_t **error )
{	
	libewf_section_t *section = NULL;
	static char *function     = "libewf_segment_file_write_chunks_section_start";
	ssize_t write_count       = 0;
	int number_of_chunks      = 0;

	if( segment_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( number_of_table_offsets < chunks_per_section )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: table offsets too small.",
		 function );

		return( -1 );
	}
	if( libmfdata_list_get_number_of_elements(
	     chunk_table_list,
	     &number_of_chunks,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the number of chunks in the chunk table list.",
		 function );

		return( -1 );
	}
	/* The chunks_per_section contains the estimated number of chunks for this section
	 */
	if( (uint32_t) number_of_chunks < ( number_of_chunks_written + chunks_per_section ) )
	{
		if( libmfdata_list_resize(
		     chunk_table_list,
		     (int) ( number_of_chunks_written + chunks_per_section ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize chunk table list.",
			 function );

			return( -1 );
		}
	}
	if( libewf_section_initialize(
	     &section,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create section.",
		 function );

		goto on_error;
	}
	if( ( io_handle->ewf_format == EWF_FORMAT_S01 )
	 || ( io_handle->format == LIBEWF_FORMAT_ENCASE1 ) )
	{
		/* Write table section start
		 */
		write_count = libewf_section_table_write(
		               section,
		               file_io_pool,
		               file_io_pool_entry,
		               (uint8_t *) "table",
		               5,
		               section_offset,
		               0,
		               table_offsets,
		               chunks_per_section,
		               0,
		               io_handle->ewf_format,
		               error );

		if( write_count == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write table section.",
			 function );

			return( -1 );
		}
	}
	else if( io_handle->ewf_format == EWF_FORMAT_E01 )
	{
		write_count = libewf_section_sectors_write(
		               section,
		               file_io_pool,
		               file_io_pool_entry,
		               section_offset,
		               0,
		               error );

		if( write_count == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write sectors section.",
			 function );

			goto on_error;
		}
	}
	if( libewf_section_free(
	     &section,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free section.",
		 function );

		section = NULL;

		goto on_error;
	}
	return( write_count );

on_error:
	if( section != NULL )
	{
		libewf_section_free(
		 &section,
		 NULL );
	}
	return( -1 );
}

/* Correct the sections before the actual data chunks
 * Also write the necessary sections after the actual data chunks to file (like table and table2 sections for EWF-E01 format)
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_chunks_section_correction(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libmfdata_list_t *chunk_table_list,
         ewf_table_offset_t *table_offsets,
         uint32_t number_of_table_offsets,
         off64_t chunks_section_offset,
         size64_t chunks_section_size,
         uint32_t number_of_chunks,
         uint32_t section_number_of_chunks,
         liberror_error_t **error )
{
	libewf_section_t *backup_group_section = NULL;
	libewf_section_t *group_section        = NULL;
	libewf_section_t *section              = NULL;
	uint8_t *table_section_string          = NULL;
	static char *function                  = "libewf_segment_file_write_chunks_section_correction";
	off64_t base_offset                    = 0;
	ssize_t total_write_count              = 0;
	ssize_t write_count                    = 0;

	if( segment_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( io_handle->format == LIBEWF_FORMAT_ENCASE6 )
	{
		if( chunks_section_size >= (size64_t) INT64_MAX )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid chunk section size value exceeds maximum.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( chunks_section_size >= (size64_t) INT32_MAX )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid chunk section size value exceeds maximum.",
			 function );

			return( -1 );
		}
	}
	if( ( section_number_of_chunks > number_of_chunks )
	 || ( section_number_of_chunks > number_of_table_offsets ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: section number of chunks value out of bounds.",
		 function );

		return( -1 );
	}
	if( io_handle->format == LIBEWF_FORMAT_ENCASE6 )
	{
		base_offset = chunks_section_offset;
	}
	if( libewf_chunk_table_fill_offsets(
	     chunk_table_list,
	     number_of_chunks - section_number_of_chunks,
	     base_offset,
	     table_offsets,
	     section_number_of_chunks,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to fill table offsets.",
		 function );

		return( -1 );
	}
	/* Seek the start of the data chunks
	*/
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
	 	"%s: setting file descriptor to start of chunks section offset: %" PRIu32 ".\n",
		 function,
		 chunks_section_offset );
	}
#endif
	if( libbfio_pool_seek_offset(
	     file_io_pool,
	     file_io_pool_entry,
	     chunks_section_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to find offset to correct sectors size.",
		 function );

		return( -1 );
	}
	if( libewf_section_initialize(
	     &section,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create section.",
		 function );

		goto on_error;
	}
	if( ( io_handle->ewf_format == EWF_FORMAT_S01 )
	 || ( io_handle->format == LIBEWF_FORMAT_ENCASE1 ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: correcting table section offset: %" PRIi64 " size: %" PRIu64 ".\n",
			 function,
			 chunks_section_offset,
			 chunks_section_size );
		}
#endif
		/* Rewrite table section start
		 */
		write_count = libewf_section_table_write(
		               section,
		               file_io_pool,
		               file_io_pool_entry,
		               (uint8_t *) "table",
		               5,
		               chunks_section_offset,
		               0,
		               table_offsets,
		               section_number_of_chunks,
		               chunks_section_size,
		               io_handle->ewf_format,
		               error );

		if( write_count == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to rewrite %s section.",
			 function,
			 (char *) table_section_string );

			return( -1 );
		}
		group_section = section;
	}
	else if( io_handle->ewf_format == EWF_FORMAT_E01 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
		 	"%s: correcting sectors section offset: %" PRIi64 " size: %" PRIu64 ".\n",
			 function,
			 chunks_section_offset,
			 chunks_section_size );
		}
#endif
		write_count = libewf_section_sectors_write(
		               section,
		               file_io_pool,
		               file_io_pool_entry,
		               chunks_section_offset,
		               chunks_section_size,
		               error );

		if( write_count == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to rewrite sectors section.",
			 function );

			goto on_error;
		}
	}
	if( libewf_list_append_value(
	     segment_file->section_list,
	     (intptr_t *) section,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append section to list.",
		 function );

		goto on_error;
	}
	section = NULL;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: setting file descriptor back to end of data at offset: %" PRIu32 ".\n",
		 function,
		 section_offset );
	}
#endif
	if( libbfio_pool_seek_offset(
	     file_io_pool,
	     file_io_pool_entry,
	     section_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to find offset to continue.",
		 function );

		return( -1 );
	}
	if( ( io_handle->ewf_format == EWF_FORMAT_E01 )
	 && ( io_handle->format != LIBEWF_FORMAT_ENCASE1 ) )
	{
		if( libewf_section_initialize(
		     &section,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create section.",
			 function );

			goto on_error;
		}
		write_count = libewf_section_table_write(
		               section,
		               file_io_pool,
		               file_io_pool_entry,
		               (uint8_t *) "table",
		               5,
		               section_offset,
		               base_offset,
		               table_offsets,
		               section_number_of_chunks,
		               0,
		               io_handle->ewf_format,
		               error );

		if( write_count == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to rewrite table section.",
			 function );

			return( -1 );
		}
		section_offset    += write_count;
		total_write_count += write_count;

		if( libewf_list_append_value(
		     segment_file->section_list,
		     (intptr_t *) section,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append section to list.",
			 function );

			goto on_error;
		}
		group_section = section;

		section = NULL;

		if( libewf_section_initialize(
		     &section,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create section.",
			 function );

			goto on_error;
		}
		write_count = libewf_section_table_write(
		               section,
		               file_io_pool,
		               file_io_pool_entry,
		               (uint8_t *) "table2",
		               6,
		               section_offset,
		               base_offset,
		               table_offsets,
		               section_number_of_chunks,
		               0,
		               io_handle->format,
		               error );

		if( write_count == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to rewrite table2 section.",
			 function );

			return( -1 );
		}
		section_offset    += write_count;
		total_write_count += write_count;

		if( libewf_list_append_value(
		     segment_file->section_list,
		     (intptr_t *) section,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append section to list.",
			 function );

			goto on_error;
		}
		backup_group_section = section;
	
		section = NULL;
	}
	/* Group the elements to reduce the memory usage
	 */
	if( libmfdata_list_set_group_by_index(
	     chunk_table_list,
	     number_of_chunks - section_number_of_chunks,
	     (int) section_number_of_chunks,
	     file_io_pool_entry,
	     group_section->start_offset,
	     group_section->size,
	     0,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set chunk group: %d - %d.",
		 function,
		 number_of_chunks - section_number_of_chunks,
		 number_of_chunks );

		return( -1 );
	}
	if( backup_group_section != NULL )
	{
		if( libmfdata_list_set_backup_data_range_by_index(
		     chunk_table_list,
		     number_of_chunks - section_number_of_chunks,
		     file_io_pool_entry,
		     backup_group_section->start_offset,
		     backup_group_section->size,
		     0,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set backup data range of chunk group: %d - %d.",
			 function,
			 number_of_chunks - section_number_of_chunks,
			 number_of_chunks );

			return( -1 );
		}
	}
	return( total_write_count );

on_error:
	if( section != NULL )
	{
		libewf_section_free(
		 &section,
		 NULL );
	}
	return( -1 );
}

/* Write a chunk of data to a segment file and update the chunk table
 * Set write_checksum to a non 0 value if the checksum is not provided within the chunk data
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_chunk(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libmfdata_list_t *chunk_table_list,
         int chunk_index,
         uint8_t *chunk_buffer,
         size_t chunk_buffer_size,
         size_t chunk_data_size LIBEWF_ATTRIBUTE_UNUSED,
         int8_t is_compressed,
         uint8_t *checksum_buffer,
         uint32_t *chunk_checksum,
         int8_t write_checksum,
         liberror_error_t **error )
{
	static char *function       = "libewf_segment_file_write_chunk";
	off64_t segment_file_offset = 0;
	size_t write_size           = 0;
	ssize_t write_count         = 0;
	ssize_t total_write_count   = 0;
	uint8_t chunk_flags         = 0;
	int number_of_chunks        = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	char *chunk_type            = NULL;
#endif

	LIBEWF_UNREFERENCED_PARAMETER( chunk_data_size )

	if( segment_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( chunk_checksum == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk checksum.",
		 function );

		return( -1 );
	}
	if( chunk_buffer_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid chunk buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( libmfdata_list_get_number_of_elements(
	     chunk_table_list,
	     &number_of_chunks,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the number of chunks in the chunk table list.",
		 function );

		return( -1 );
	}
	/* Make sure the chunk is available in the chunk table
	 */
	if( number_of_chunks < ( chunk_index + 1 ) )
	{
		if( libmfdata_list_resize(
		     chunk_table_list,
		     chunk_index + 1,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize chunk table list.",
			 function );

			return( -1 );
		}
	}
	if( libbfio_pool_get_offset(
	     file_io_pool,
	     file_io_pool_entry,
	     &segment_file_offset,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve current offset in segment file.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		write_size = chunk_buffer_size;

		if( write_checksum != 0 )
		{
			write_size += sizeof( uint32_t );
		}
		if( is_compressed == 0 )
		{
			chunk_type = "uncompressed";
		}
		else
		{
			chunk_type = "compressed";
		}
		libnotify_printf(
		 "%s: writing %s chunk: %d at offset: %" PRIi64 " with size: %" PRIzd ", with checksum: 0x%08" PRIx32 ".\n",
		 function,
		 chunk_type,
		 chunk_index,
		 segment_file_offset,
		 write_size,
		 *chunk_checksum );
	}
#endif
	write_size = chunk_buffer_size;

	/* Write the checksum if necessary
	 */
	if( write_checksum != 0 )
	{
		if( checksum_buffer == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid checksum buffer.",
			 function );

			return( -1 );
		}
		byte_stream_copy_from_uint32_little_endian(
		 checksum_buffer,
		 *chunk_checksum );

		/* Check if the chunk and checksum buffers are aligned
		 * if so write the chunk and checksum at the same time
		 */
		if( ( is_compressed == 0 )
		 && ( &( chunk_buffer[ chunk_buffer_size ] ) == checksum_buffer ) )
		{
			write_size += sizeof( uint32_t );

			write_checksum = 0;
		}
	}
	/* Write the chunk data to the segment file
	 */
	write_count = libbfio_pool_write(
	               file_io_pool,
	               file_io_pool_entry,
	               chunk_buffer,
	               write_size,
	               error );

	if( write_count != (ssize_t) write_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write chunk: %d data.",
		 function,
		 chunk_index );

		return( -1 );
	}
	total_write_count += write_count;

	/* Check if the chunk and checksum buffers are aligned
	 * if not the chunk and checksum need to be written separately
	 */
	if( write_checksum != 0 )
	{
		write_count = libbfio_pool_write(
			       file_io_pool,
			       file_io_pool_entry,
			       checksum_buffer,
			       sizeof( uint32_t ),
			       error );

		if( write_count != (ssize_t) sizeof( uint32_t ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write chunk: %d checksum.",
			 function,
			 chunk_index );

			return( -1 );
		}
		total_write_count += write_count;
	}
	if( is_compressed != 0 )
	{
		chunk_flags = LIBMFDATA_RANGE_FLAG_IS_COMPRESSED;
	}
	else
	{
		chunk_flags = 0;
	}
	if( libmfdata_list_set_element_by_index(
	     chunk_table_list,
	     chunk_index,
	     file_io_pool_entry,
	     segment_file_offset,
	     (size64_t) total_write_count,
	     chunk_flags,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set chunk: %d.",
		 function,
		 chunk_index );

		return( -1 );
	}
	return( total_write_count );
}

/* Write a delta chunk of data to a segment file and update the chunk table
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_delta_chunk(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libmfdata_list_t *chunk_table_list,
         int chunk_index,
         uint8_t *chunk_buffer,
         size_t chunk_size,
         uint8_t *checksum_buffer,
         uint32_t *chunk_checksum,
         uint8_t write_checksum,
	 uint8_t no_section_append,
         liberror_error_t **error )
{
	libewf_section_t *section = NULL;
	static char *function     = "libewf_segment_file_write_delta_chunk";
	off64_t chunk_offset      = 0;
	ssize_t write_count       = 0;
	int chunk_exists          = 0;

	if( segment_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( ( section_offset < 0 )
	 || ( section_offset > (off64_t) INT64_MAX ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section offset value out of bounds.",
		 function );

		return( -1 );
	}
	chunk_exists = libmfdata_list_is_set(
			chunk_table_list,
			chunk_index,
			error );

	if( chunk_exists == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if the chunk: %d exists in the chunk table.",
		 function,
		 chunk_index );

		goto on_error;
	}
	else if( chunk_exists == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing chunk: %d.",
		 function,
		 chunk_index );

		goto on_error;
	}
	chunk_offset = section_offset
	             + sizeof( ewf_section_start_t )
	             + sizeof( ewfx_delta_chunk_header_t );

	if( libewf_section_initialize(
	     &section,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create section.",
		 function );

		goto on_error;
	}
	write_count = libewf_section_delta_chunk_write(
	               section,
	               file_io_pool,
	               file_io_pool_entry,
	               section_offset,
	               (uint32_t) chunk_index,
	               chunk_buffer,
	               (uint32_t) chunk_size,
	               checksum_buffer,
	               chunk_checksum,
	               write_checksum,
	               error );

	if( write_count == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write delta chunk.",
		 function );

		goto on_error;
	}
	if( no_section_append == 0 )
	{
		if( libewf_list_append_value(
		     segment_file->section_list,
		     (intptr_t *) section,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append section to list.",
			 function );

			goto on_error;
		}
		section = NULL;
	}
	else
	{
		if( libewf_section_free(
		     &section,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free section.",
			 function );

			section = NULL;

			goto on_error;
		}
	}
	if( write_checksum != 0 )
	{
		chunk_size += sizeof( uint32_t );
	}
	if( libmfdata_list_set_element_by_index(
	     chunk_table_list,
	     chunk_index,
	     file_io_pool_entry,
	     chunk_offset,
	     (size64_t) chunk_size,
	     LIBEWF_CHUNK_FLAG_IS_DELTA,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set chunk: %d.",
		 function,
		 chunk_index );

		goto on_error;
	}
	return( write_count );

on_error:
	if( section != NULL )
	{
		libewf_section_free(
		 &section,
		 NULL );
	}
	return( -1 );
}

/* Closes the segment file, necessary sections at the end of the segment file will be written
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_close(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         uint32_t number_of_chunks_written_to_segment,
         int last_segment_file,
         libewf_hash_sections_t *hash_sections,
         libfvalue_table_t *hash_values,
         libewf_media_values_t *media_values,
         libewf_sector_list_t *sessions,
         libewf_sector_list_t *tracks,
         libewf_sector_list_t *acquiry_errors,
         ewf_data_t **cached_data_section,
	 liberror_error_t **error )
{
	libewf_section_t *section = NULL;
	static char *function     = "libewf_segment_file_write_close";
	ssize_t total_write_count = 0;
	ssize_t write_count       = 0;
	int number_of_elements    = 0;

	if( segment_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( hash_sections == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash sections.",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	if( sessions == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sessions.",
		 function );

		return( -1 );
	}
	if( acquiry_errors == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid acquiry errors.",
		 function );

		return( -1 );
	}
	if( last_segment_file != 0 )
	{
		/* Write the data section for a single segment file only for EWF-E01
		 */
		if( io_handle->ewf_format == EWF_FORMAT_E01 )
		{
			if( segment_file->segment_number == 1 )
			{
				if( libewf_section_initialize(
				     &section,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create section.",
					 function );

					goto on_error;
				}
				write_count = libewf_section_data_write(
					       section,
					       io_handle,
					       file_io_pool,
					       file_io_pool_entry,
					       section_offset,
					       media_values,
					       cached_data_section,
					       error );

				if( write_count == -1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_IO,
					 LIBERROR_IO_ERROR_WRITE_FAILED,
					 "%s: unable to write data section.",
					 function );

					goto on_error;
				}
				section_offset    += write_count;
				total_write_count += write_count;

				if( libewf_list_append_value(
				     segment_file->section_list,
				     (intptr_t *) section,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append section to list.",
					 function );

					goto on_error;
				}
				section = NULL;
			}
		}
		/* Write the session section if required
		 */
		if( ( io_handle->format == LIBEWF_FORMAT_ENCASE5 )
		 || ( io_handle->format == LIBEWF_FORMAT_ENCASE6 )
		 || ( io_handle->format == LIBEWF_FORMAT_LINEN5 )
		 || ( io_handle->format == LIBEWF_FORMAT_LINEN6 )
		 || ( io_handle->format == LIBEWF_FORMAT_EWFX ) )
		{
			if( libewf_sector_list_get_number_of_elements(
			     sessions,
			     &number_of_elements,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve number of elements from sessions sector list.",
				 function );

				goto on_error;
			}
			if( number_of_elements > 0 )
			{
				if( libewf_section_initialize(
				     &section,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create section.",
					 function );

					goto on_error;
				}
				write_count = libewf_section_session_write(
					       section,
					       file_io_pool,
					       file_io_pool_entry,
					       section_offset,
					       sessions,
					       tracks,
					       error );

				if( write_count == -1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_IO,
					 LIBERROR_IO_ERROR_WRITE_FAILED,
					 "%s: unable to write sessions section.",
					 function );

					goto on_error;
				}
				section_offset    += write_count;
				total_write_count += write_count;

				if( libewf_list_append_value(
				     segment_file->section_list,
				     (intptr_t *) section,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append section to list.",
					 function );

					goto on_error;
				}
				section = NULL;
			}
		}
		/* Write the error2 section if required
		 */
		if( ( io_handle->format == LIBEWF_FORMAT_ENCASE3 )
		 || ( io_handle->format == LIBEWF_FORMAT_ENCASE4 )
		 || ( io_handle->format == LIBEWF_FORMAT_ENCASE5 )
		 || ( io_handle->format == LIBEWF_FORMAT_ENCASE6 )
		 || ( io_handle->format == LIBEWF_FORMAT_LINEN5 )
		 || ( io_handle->format == LIBEWF_FORMAT_LINEN6 )
		 || ( io_handle->format == LIBEWF_FORMAT_EWFX ) )
		{
			if( libewf_sector_list_get_number_of_elements(
			     acquiry_errors,
			     &number_of_elements,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve number of elements from acquiry errors sector list.",
				 function );

				goto on_error;
			}
			if( number_of_elements > 0 )
			{
				if( libewf_section_initialize(
				     &section,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create section.",
					 function );

					goto on_error;
				}
				write_count = libewf_section_error2_write(
					       section,
					       file_io_pool,
					       file_io_pool_entry,
					       section_offset,
					       acquiry_errors,
					       error );

				if( write_count == -1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_IO,
					 LIBERROR_IO_ERROR_WRITE_FAILED,
					 "%s: unable to write error2 section.",
					 function );

					goto on_error;
				}
				section_offset    += write_count;
				total_write_count += write_count;

				if( libewf_list_append_value(
				     segment_file->section_list,
				     (intptr_t *) section,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append section to list.",
					 function );

					goto on_error;
				}
				section = NULL;
			}
		}
		if( ( io_handle->format == LIBEWF_FORMAT_ENCASE6 )
		 || ( io_handle->format == LIBEWF_FORMAT_LINEN6 ) )
		{
			/* Write the digest section if required
			 */
			if( hash_sections->sha1_digest_set != 0 )
			{
				if( libewf_section_initialize(
				     &section,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create section.",
					 function );

					goto on_error;
				}
				write_count = libewf_section_digest_write(
					       section,
					       file_io_pool,
					       file_io_pool_entry,
					       section_offset,
					       hash_sections,
					       error );

				if( write_count == -1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_IO,
					 LIBERROR_IO_ERROR_WRITE_FAILED,
					 "%s: unable to write digest section.",
					 function );

					goto on_error;
				}
				section_offset    += write_count;
				total_write_count += write_count;

				if( libewf_list_append_value(
				     segment_file->section_list,
				     (intptr_t *) section,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append section to list.",
					 function );

					goto on_error;
				}
				section = NULL;
			}
		}
		/* Write the hash section if required
		 */
		if( hash_sections->md5_hash_set != 0 )
		{
			if( libewf_section_initialize(
			     &section,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create section.",
				 function );

				goto on_error;
			}
			write_count = libewf_section_hash_write(
			               section,
			               file_io_pool,
			               file_io_pool_entry,
			               section_offset,
			               hash_sections,
			               error );

			if( write_count == -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write hash section.",
				 function );

				goto on_error;
			}
			section_offset    += write_count;
			total_write_count += write_count;

			if( libewf_list_append_value(
			     segment_file->section_list,
			     (intptr_t *) section,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append section to list.",
				 function );

				goto on_error;
			}
			section = NULL;
		}
		/* Write the xhash section
		 */
		if( io_handle->format == LIBEWF_FORMAT_EWFX )
		{
			if( hash_sections->xhash != NULL )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libnotify_verbose != 0 )
				{
					libnotify_printf(
				 	"%s: xhash already set - cleaning previous defintion.\n",
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
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to generate xhash.",
				 function );

				goto on_error;
			}
			if( libewf_section_initialize(
			     &section,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create section.",
				 function );

				goto on_error;
			}
			write_count = libewf_section_xhash_write(
			               section,
			               file_io_pool,
			               file_io_pool_entry,
			               section_offset,
			               hash_sections,
			               EWF_COMPRESSION_DEFAULT,
			               error );

			if( write_count == -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write xhash section.",
				 function );

				goto on_error;
			}
			section_offset    += write_count;
			total_write_count += write_count;

			if( libewf_list_append_value(
			     segment_file->section_list,
			     (intptr_t *) section,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append section to list.",
				 function );

				goto on_error;
			}
			section = NULL;
		}
	}
	/* Write the done or next section
	 * The segment file offset is updated by the function
	 */
	write_count = libewf_segment_file_write_last_section(
		       segment_file,
		       io_handle,
		       file_io_pool,
		       file_io_pool_entry,
	               section_offset,
	               last_segment_file,
	               error );

	if( write_count == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write end of segment file.",
		 function );

		goto on_error;
	}
	total_write_count += write_count;
	section_offset    += write_count;

	segment_file->number_of_chunks = number_of_chunks_written_to_segment;

	/* Make sure the next time the file is opened
	 * it is not truncated
	 */
	if( libbfio_pool_reopen(
	     file_io_pool,
	     file_io_pool_entry,
	     LIBBFIO_OPEN_READ_WRITE,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to reopen segment file: %" PRIu16 ".",
		 function,
		 segment_file->segment_number );

		goto on_error;
	}
	if( libbfio_pool_close(
	     file_io_pool,
	     file_io_pool_entry,
	     error ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close segment file: %" PRIu16 ".",
		 function,
		 segment_file->segment_number );

		goto on_error;
	}
	segment_file->flags &= ~( LIBEWF_SEGMENT_FILE_FLAG_WRITE_OPEN );

	return( total_write_count );

on_error:
	if( section != NULL )
	{
		libewf_section_free(
		 &section,
		 NULL );
	}
	return( -1 );
}

/* Corrects sections after streamed write
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_file_write_sections_correction(
     libewf_segment_file_t *segment_file,
     libewf_io_handle_t *io_handle,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     uint32_t number_of_chunks_written_to_segment,
     int last_segment_file,
     libewf_media_values_t *media_values,
     libfvalue_table_t *hash_values,
     libewf_hash_sections_t *hash_sections,
     libewf_sector_list_t *sessions,
     libewf_sector_list_t *tracks,
     libewf_sector_list_t *acquiry_errors,
     ewf_data_t **cached_data_section,
     liberror_error_t **error )
{
	libewf_list_element_t *section_list_element = NULL;
	libewf_section_t *section                   = NULL;
	static char *function                       = "libewf_segment_file_write_sections_correction";
	off64_t next_section_start_offset           = 0;
	ssize_t write_count                         = 0;
	int correct_last_next_section               = 0;

	if( segment_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( segment_file->section_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment file - missing section list.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: correcting sections in segment file: %" PRIu16 ".\n",
		 function,
		 segment_file->segment_number );	
	}
#endif
	section_list_element = segment_file->section_list->first_element;

	if( section_list_element == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing first section list element.",
		 function );

		return( -1 );
	}
	while( section_list_element != NULL )
	{
		section = (libewf_section_t *) section_list_element->value;

		if( section == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing section.",
			 function );

			return( -1 );
		}
		if( section->type_length == 6 )
		{
			if( memory_compare(
			     section->type,
			     "volume",
			     6 ) == 0 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libnotify_verbose != 0 )
				{
					libnotify_printf(
					 "%s: correcting volume section.\n",
					 function );
				}
#endif
				if( libbfio_pool_seek_offset(
				     file_io_pool,
				     file_io_pool_entry,
				     section->start_offset,
				     SEEK_SET,
				     error ) == -1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_IO,
					 LIBERROR_IO_ERROR_SEEK_FAILED,
					 "%s: unable to find offset to correct volume section.",
					 function );

					return( -1 );
				}
				write_count = -1;

				if( io_handle->ewf_format == EWF_FORMAT_E01 )
				{
					write_count = libewf_section_volume_e01_write(
						       section,
						       io_handle,
						       file_io_pool,
						       file_io_pool_entry,
						       section->start_offset,
						       media_values,
						       error );
				}
				else if( io_handle->ewf_format == EWF_FORMAT_S01 )
				{
					write_count = libewf_section_volume_s01_write(
						       section,
						       io_handle,
						       file_io_pool,
						       file_io_pool_entry,
						       section->start_offset,
						       media_values,
						       error );
				}
				if( write_count == -1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_IO,
					 LIBERROR_IO_ERROR_WRITE_FAILED,
					 "%s: unable to correct volume section.",
					 function );

					return( -1 );
				}
			}
		}
		else if( section->type_length == 4 )
		{
			if( memory_compare(
			     section->type,
			     "data",
			     4 ) == 0 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libnotify_verbose != 0 )
				{
					libnotify_printf(
					 "%s: correcting data section.\n",
					 function );
				}
#endif
				if( libbfio_pool_seek_offset(
				     file_io_pool,
				     file_io_pool_entry,
				     section->start_offset,
				     SEEK_SET,
				     error ) == -1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_IO,
					 LIBERROR_IO_ERROR_SEEK_FAILED,
					 "%s: unable to seek offset of data section.",
					 function );

					return( -1 );
				}
				write_count = libewf_section_data_write(
					       section,
					       io_handle,
					       file_io_pool,
					       file_io_pool_entry,
					       section->start_offset,
					       media_values,
					       cached_data_section,
					       error );

				if( write_count == -1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_IO,
					 LIBERROR_IO_ERROR_WRITE_FAILED,
					 "%s: unable to correct data section.",
					 function );

					return( -1 );
				}
			}
			/* The last segment file should be terminated with a done section and not with a next section
			 */
			else if( last_segment_file != 0 )
			{
				if( memory_compare(
				     section->type,
				     "next",
				     4 ) == 0 )
				{
					correct_last_next_section = 1;
					next_section_start_offset = section->start_offset;
				}
			}
		}
		section_list_element = section_list_element->next_element;
	}
	if( correct_last_next_section == 0 )
	{
		if( libbfio_pool_close(
		     file_io_pool,
		     file_io_pool_entry,
		     error ) != 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close file IO pool entry: %d.",
			 function,
			 file_io_pool_entry );

			return( -1 );
		}
	}
	else
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to find offset to last next section.",
			 function );

			return( -1 );
		}
		write_count = libewf_segment_file_write_close(
			       segment_file,
			       io_handle,
			       file_io_pool,
			       file_io_pool_entry,
			       next_section_start_offset,
			       number_of_chunks_written_to_segment,
			       1,
			       hash_sections,
			       hash_values,
			       media_values,
			       sessions,
			       tracks,
			       acquiry_errors,
			       cached_data_section,
			       error );

		if( write_count == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to close segment file.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

