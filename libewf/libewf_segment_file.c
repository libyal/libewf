/*
 * Segment file reading/writing functions
 *
 * Copyright (c) 2006-2013, Joachim Metz <joachim.metz@gmail.com>
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

#if defined( TIME_WITH_SYS_TIME )
#include <sys/time.h>
#include <time.h>
#elif defined( HAVE_SYS_TIME_H )
#include <sys/time.h>
#else
#include <time.h>
#endif

#include "libewf_case_data.h"
#include "libewf_chunk_table.h"
#include "libewf_debug.h"
#include "libewf_definitions.h"
#include "libewf_device_information.h"
#include "libewf_hash_values.h"
#include "libewf_header_values.h"
#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_libcdata.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libfvalue.h"
#include "libewf_libfcache.h"
#include "libewf_libmfdata.h"
#include "libewf_section.h"
#include "libewf_segment_file.h"
#include "libewf_segment_table.h"
#include "libewf_single_files.h"
#include "libewf_unused.h"

#include "ewf_definitions.h"
#include "ewf_file_header.h"
#include "ewf_section.h"
#include "ewf_volume.h"
#include "ewfx_delta_chunk.h"

const uint8_t ewf1_dvf_file_signature[ 8 ] = { 0x64, 0x76, 0x66, 0x09, 0x0d, 0x0a, 0xff, 0x00 };
const uint8_t ewf1_evf_file_signature[ 8 ] = { 0x45, 0x56, 0x46, 0x09, 0x0d, 0x0a, 0xff, 0x00 };
const uint8_t ewf1_lvf_file_signature[ 8 ] = { 0x4c, 0x56, 0x46, 0x09, 0x0d, 0x0a, 0xff, 0x00 };
const uint8_t ewf2_evf_file_signature[ 8 ] = { 0x45, 0x56, 0x46, 0x32, 0x0d, 0x0a, 0x81, 0x00 };
const uint8_t ewf2_lef_file_signature[ 8 ] = { 0x4c, 0x45, 0x46, 0x32, 0x0d, 0x0a, 0x81, 0x00 };

/* Initialize the segment file
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_file_initialize(
     libewf_segment_file_t **segment_file,
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

		goto on_error;
	}
	if( libcdata_list_initialize(
	     &( ( *segment_file )->section_list ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
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
		if( ( *segment_file )->section_list != NULL )
		{
			if( libcdata_list_free(
			     &( ( *segment_file )->section_list ),
			     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_section_free,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
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
     libcerror_error_t **error )
{
	static char *function = "libewf_segment_file_clone";

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

		goto on_error;
	}
	( *destination_segment_file )->section_list = NULL;

	if( libcdata_list_clone(
	     &( ( *destination_segment_file )->section_list ),
	     source_segment_file->section_list,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_section_free,
	     (int(*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libewf_section_clone,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
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
         libcerror_error_t **error )
{
	uint8_t *file_header_data    = NULL;
	static char *function        = "libewf_segment_file_read_file_header";
	size_t file_header_data_size = 0;
	ssize_t read_count           = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint16_t value_16bit         = 0;
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
	file_header_data = (uint8_t *) memory_allocate(
	                                sizeof( ewf_file_header_v2_t ) );

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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek file header offset: 0.",
		 function );

		goto on_error;
	}
	read_count = libbfio_pool_read_buffer(
	              file_io_pool,
	              file_io_pool_entry,
	              file_header_data,
	              8,
	              error );

	if( read_count != (ssize_t) 8 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header signature.",
		 function );

		goto on_error;
	}
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
	else if( memory_compare(
	          ewf1_dvf_file_signature,
	          file_header_data,
	          8 ) == 0 )
	{
		segment_file->type    = LIBEWF_SEGMENT_FILE_TYPE_EWF1_DELTA;
		file_header_data_size = sizeof( ewf_file_header_v1_t );
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_SIGNATURE_MISMATCH,
		 "%s: unsupported file header signature.",
		 function );

		goto on_error;
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

		goto on_error;
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
	if( file_header_data_size == sizeof( ewf_file_header_v1_t ) )
	{
		byte_stream_copy_to_uint16_little_endian(
		 ( (ewf_file_header_v1_t *) file_header_data )->segment_number,
		 segment_file->segment_number );

		segment_file->major_version      = 1;
		segment_file->minor_version      = 0;
		segment_file->compression_method = LIBEWF_COMPRESSION_METHOD_DEFLATE;
	}
	else if( file_header_data_size == sizeof( ewf_file_header_v2_t ) )
	{
		segment_file->major_version = ( (ewf_file_header_v2_t *) file_header_data )->major_version;
		segment_file->minor_version = ( (ewf_file_header_v2_t *) file_header_data )->minor_version;

		byte_stream_copy_to_uint16_little_endian(
		 ( (ewf_file_header_v2_t *) file_header_data )->compression_method,
		 segment_file->compression_method );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_file_header_v2_t *) file_header_data )->segment_number,
		 segment_file->segment_number );

		if( memory_copy(
		     segment_file->set_identifier,
		     ( (ewf_file_header_v2_t *) file_header_data )->set_identifier,
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
	 	 "%s: signature:\n",
		 function );
		libcnotify_print_data(
		 file_header_data,
		 8,
		 0 );

		if( file_header_data_size == sizeof( ewf_file_header_v1_t ) )
		{
			libcnotify_printf(
		 	 "%s: fields start\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (ewf_file_header_v1_t *) file_header_data )->fields_start );
		}
		else if( file_header_data_size == sizeof( ewf_file_header_v2_t ) )
		{
			libcnotify_printf(
		 	 "%s: major version\t\t\t: %" PRIu8 "\n",
			 function,
			 segment_file->major_version );

			libcnotify_printf(
		 	 "%s: minor version\t\t\t: %" PRIu8 "\n",
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

		if( file_header_data_size == sizeof( ewf_file_header_v1_t ) )
		{
			byte_stream_copy_to_uint16_little_endian(
			 ( (ewf_file_header_v1_t *) file_header_data )->fields_end,
			 value_16bit );
			libcnotify_printf(
		 	 "%s: fields end\t\t\t: 0x%04" PRIx16 "\n",
			 function,
			 value_16bit );
		}
		else if( file_header_data_size == sizeof( ewf_file_header_v2_t ) )
		{
/* TODO replace by GUID print */
			libcnotify_printf(
			 "%s: set identifier:\n",
			 function );
			libcnotify_print_data(
			 segment_file->set_identifier,
			 16,
			 0 );
		}
		libcnotify_printf(
	 	 "\n" );
	}
#endif
	memory_free(
	 file_header_data );

	return( (ssize_t) file_header_data_size );

on_error:
	if( file_header_data != NULL )
	{
		memory_free(
		 file_header_data );
	}
	return( -1 );
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
		else if( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_DELTA )
		{
			file_signature = ewf1_dvf_file_signature;
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
		 	 "%s: major version\t\t\t: %" PRIu8 "\n",
			 function,
			 segment_file->major_version );

			libcnotify_printf(
		 	 "%s: minor version\t\t\t: %" PRIu8 "\n",
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
/* TODO replace by GUID print */
			libcnotify_printf(
			 "%s: set identifier:\n",
			 function );
			libcnotify_print_data(
			 segment_file->set_identifier,
			 16,
			 0 );
		}
		libcnotify_printf(
	 	 "\n" );
	}
#endif
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

/* Reads a segment file
 * Callback function for the segment files list
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_file_read(
     intptr_t *io_handle LIBEWF_ATTRIBUTE_UNUSED,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     libmfdata_file_t *file,
     libfcache_cache_t *cache,
     uint8_t read_flags LIBEWF_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
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
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create section.",
			 function );

			goto on_error;
		}
/* TODO set version */
		read_count = libewf_section_descriptor_read(
		              section,
		              file_io_pool,
		              file_io_pool_entry,
		              section_offset,
		              1,
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
		if( section->type_string_length == 4 )
		{
			if( memory_compare(
			     (void *) section->type_string,
			     (void *) "done",
			     4 ) == 0 )
			{
				last_section = 1;
			}
			else if( memory_compare(
				  (void *) section->type_string,
				  (void *) "next",
				  4 ) == 0 )
			{
				last_section = 1;
			}
		}
		section_offset += section->size;

		if( libcdata_list_append_value(
		     segment_file->section_list,
		     (intptr_t *) section,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing next or done section.",
		 function );

		goto on_error;
	}
	if( libmfdata_file_set_file_value(
	     file,
	     cache,
	     (intptr_t *) segment_file,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_segment_file_free,
	     LIBMFDATA_FILE_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
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
         libcerror_error_t **error )
{
	uint8_t *section_data          = NULL;
	uint8_t *table_entries_data    = NULL;
	static char *function          = "libewf_segment_file_read_table_section";
	off64_t element_group_offset   = 0;
	size64_t element_group_size    = 0;
	ssize_t read_count             = 0;
	size_t section_data_size       = 0;
	size_t table_entries_data_size = 0;
	uint64_t base_offset           = 0;
	uint64_t first_chunk_index     = 0;
	uint32_t number_of_entries     = 0;
	uint8_t entries_corrupted      = 0;

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
	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
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
	if( chunk_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk table.",
		 function );

		return( -1 );
	}
	chunk_table->previous_last_chunk_filled = chunk_table->last_chunk_filled;

	read_count = libewf_section_table_read(
		      section,
		      io_handle,
		      file_io_pool,
		      file_io_pool_entry,
		      segment_file->major_version,
		      &section_data,
		      &section_data_size,
		      &first_chunk_index,
		      &base_offset,
		      &table_entries_data,
		      &table_entries_data_size,
		      &number_of_entries,
		      &entries_corrupted,
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
	if( segment_file->major_version == 1 )
	{
		/* For EWF version 1 the entire table section is considered the group
		 * because the section descriptor is need to determine the chunk data
		 * offset and size values
		 */
		element_group_offset = section->start_offset;
		element_group_size   = (size64_t) section->size;
	}
	else if( segment_file->major_version == 2 )
	{
		/* For EWF version 2 the table (section data) is considered the group
		 */
		element_group_offset = section->start_offset;
		element_group_size   = (size64_t) section->data_size;

		if( first_chunk_index != (uint64_t) chunk_table->last_chunk_filled )
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
/* TODO use entries_corrupted to mark group as tainted ? */
	if( number_of_entries > 0 )
	{
		/* The EWF-L01 does not define the number of chunks in the volume
		 */
		if( media_values->number_of_chunks == 0 )
		{
			if( libmfdata_list_append_group(
			     chunk_table_list,
			     &( chunk_table->last_chunk_filled ),
			     (int) number_of_entries,
			     file_io_pool_entry,
			     element_group_offset,
			     element_group_size,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append chunk group.",
				 function );

				goto on_error;
			}
		}
		else
		{
			if( libmfdata_list_set_group_by_index(
			     chunk_table_list,
			     chunk_table->last_chunk_filled,
			     (int) number_of_entries,
			     file_io_pool_entry,
			     element_group_offset,
			     element_group_size,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set chunk group: %d - %d.",
				 function,
				 chunk_table->last_chunk_filled,
				 chunk_table->last_chunk_filled + number_of_entries );

				goto on_error;
			}
		}
		chunk_table->last_chunk_filled += (int) number_of_entries;

		segment_file->number_of_chunks += (uint64_t) number_of_entries;
	}
	memory_free(
	 section_data );

	return( 1 );

on_error:
	if( section_data != NULL )
	{
		memory_free(
		 section_data );
	}
	return( -1 );
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
         libcerror_error_t **error )
{
	uint8_t *section_data          = NULL;
	uint8_t *table_entries_data    = NULL;
	static char *function          = "libewf_segment_file_read_table2_section";
	off64_t group_offset           = 0;
	size64_t group_size            = 0;
	size_t section_data_size       = 0;
	size_t table_entries_data_size = 0;
	ssize_t read_count             = 0;
	uint64_t base_offset           = 0;
	uint64_t first_chunk_index     = 0;
	uint32_t group_flags           = 0;
	uint32_t number_of_entries     = 0;
	uint8_t entries_corrupted      = 0;
	int group_number_of_entries    = 0;
	int group_file_io_pool_entry   = 0;

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
	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
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
	if( chunk_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk table.",
		 function );

		return( -1 );
	}
	read_count = libewf_section_table_read(
	              section,
	              io_handle,
	              file_io_pool,
	              file_io_pool_entry,
	              segment_file->major_version,
		      &section_data,
		      &section_data_size,
	              &first_chunk_index,
	              &base_offset,
		      &table_entries_data,
		      &table_entries_data_size,
	              &number_of_entries,
		      &entries_corrupted,
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
/* TODO use entries_corrupted ? */
	if( number_of_entries > 0 )
	{
		if( libmfdata_list_get_group_by_index(
		     chunk_table_list,
		     chunk_table->last_chunk_compared,
		     &group_number_of_entries,
		     &group_file_io_pool_entry,
		     &group_offset,
		     &group_size,
		     &group_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve chunk group: %d - %d.",
			 function,
			 chunk_table->last_chunk_compared,
			 chunk_table->last_chunk_compared + number_of_entries );

			goto on_error;
		}
		if( (int) number_of_entries != group_number_of_entries )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_INPUT,
			 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
			 "%s: mismatch between number of entries in table and table2.",
			 function );

			goto on_error;
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
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set backup data range of chunk group: %d - %d.",
			 function,
			 chunk_table->last_chunk_compared,
			 chunk_table->last_chunk_compared + number_of_entries );

			goto on_error;
		}
		chunk_table->last_chunk_compared += (int) number_of_entries;
	}
	memory_free(
	 section_data );

	return( 1 );

on_error:
	if( section_data != NULL )
	{
		memory_free(
		 section_data );
	}
	return( -1 );
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
	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
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
	if( section->data_size == (size64_t) sizeof( ewf_volume_t ) )
	{
		read_count = libewf_section_volume_e01_read(
		              section,
		              io_handle,
		              file_io_pool,
		              file_io_pool_entry,
		              media_values,
		              error );

		if( read_count != (ssize_t) section->data_size )
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
	else if( section->data_size == (size64_t) sizeof( ewf_volume_smart_t ) )
	{
		read_count = libewf_section_volume_s01_read(
		              section,
		              io_handle,
		              file_io_pool,
		              file_io_pool_entry,
		              media_values,
		              error );

		if( read_count != (ssize_t) section->data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read EWF-S01 volume section.",
			 function );

			return( -1 );
		}
		segment_file->type           = LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART;
		io_handle->segment_file_type = LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART;
		io_handle->format            = LIBEWF_FORMAT_SMART;
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
         libcerror_error_t **error )
{
	static char *function = "libewf_segment_file_read_delta_chunk_section";
	off64_t chunk_offset  = 0;
	ssize_t read_count    = 0;
	uint32_t chunk_index  = 0;
	uint32_t chunk_size   = 0;
	uint32_t range_flags  = 0;

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
	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	read_count = libewf_section_delta_chunk_read_header(
	              section,
	              file_io_pool,
	              file_io_pool_entry,
	              &chunk_index,
	              &chunk_size,
	              error );
	
	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read delta chunk section header.",
		 function );

		return( -1 );
	}
	chunk_offset = section->start_offset
	             + sizeof( ewf_section_descriptor_v1_t )
	             + sizeof( ewfx_delta_chunk_header_t );

	range_flags = LIBEWF_RANGE_FLAG_HAS_CHECKSUM
	            | LIBEWF_RANGE_FLAG_IS_DELTA;

	if( libmfdata_list_set_element_by_index(
	     chunk_table_list,
	     (int) chunk_index,
	     file_io_pool_entry,
	     chunk_offset,
	     (size64_t) chunk_size,
	     range_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set chunk: %" PRIu32 ".",
		 function,
		 chunk_index );

		return( -1 );
	}
	return( 1 );
}

/* Writes a device information section to file
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_device_information_section(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         uint8_t **device_information,
         size_t *device_information_size,
         libewf_media_values_t *media_values,
         libfvalue_table_t *header_values,
         libcerror_error_t **error )
{
	libewf_section_t *section = NULL;
	static char *function     = "libewf_segment_file_write_device_information_section";
	ssize_t write_count       = 0;

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
	if( libewf_section_initialize(
	     &section,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create section.",
		 function );

		goto on_error;
	}
	/* Do not include the end of string character in the compressed data
	 */
	write_count = libewf_section_write_compressed_string(
	               section,
	               io_handle,
	               file_io_pool,
	               file_io_pool_entry,
	               2,
	               LIBEWF_SECTION_TYPE_DEVICE_INFORMATION,
	               NULL,
	               0,
	               section_offset,
	               io_handle->compression_method,
	               LIBEWF_COMPRESSION_DEFAULT,
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
	if( libcdata_list_append_value(
	     segment_file->section_list,
	     (intptr_t *) section,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append section to list.",
		 function );

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

/* Writes a case data section to file
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_case_data_section(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         uint8_t **case_data,
         size_t *case_data_size,
         libewf_media_values_t *media_values,
         libfvalue_table_t *header_values,
         time_t timestamp,
         libcerror_error_t **error )
{
	libewf_section_t *section = NULL;
	static char *function     = "libewf_segment_file_write_case_data_section";
	ssize_t write_count       = 0;

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
		     io_handle->format,
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
	if( libewf_section_initialize(
	     &section,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create section.",
		 function );

		goto on_error;
	}
	/* Do not include the end of string character in the compressed data
	 */
	write_count = libewf_section_write_compressed_string(
	               section,
	               io_handle,
	               file_io_pool,
	               file_io_pool_entry,
	               2,
	               LIBEWF_SECTION_TYPE_CASE_DATA,
	               NULL,
	               0,
	               section_offset,
	               io_handle->compression_method,
	               LIBEWF_COMPRESSION_DEFAULT,
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
	if( libcdata_list_append_value(
	     segment_file->section_list,
	     (intptr_t *) section,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append section to list.",
		 function );

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

/* Writes a header section to file
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_header_section(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_header_sections_t *header_sections,
         int8_t compression_level,
         libcerror_error_t **error )
{
	libewf_section_t *section = NULL;
	static char *function     = "libewf_segment_file_write_header_section";
	ssize_t write_count       = 0;

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
	if( libewf_section_initialize(
	     &section,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create section.",
		 function );

		goto on_error;
	}
	/* Do not include the end of string character in the compressed data
	 */
	write_count = libewf_section_write_compressed_string(
	               section,
	               io_handle,
	               file_io_pool,
	               file_io_pool_entry,
	               1,
	               0,
	               (uint8_t *) "header",
	               6,
	               section_offset,
	               io_handle->compression_method,
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
	if( libcdata_list_append_value(
	     segment_file->section_list,
	     (intptr_t *) section,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append section to list.",
		 function );

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

/* Writes a header2 section to file
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_header2_section(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_header_sections_t *header_sections,
         libcerror_error_t **error )
{
	libewf_section_t *section = NULL;
	static char *function     = "libewf_segment_file_write_header2_section";
	ssize_t write_count       = 0;

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
	if( libewf_section_initialize(
	     &section,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create section.",
		 function );

		goto on_error;
	}
	/* Do not include the end of string character in the compressed data
	 */
	write_count = libewf_section_write_compressed_string(
	               section,
	               io_handle,
	               file_io_pool,
	               file_io_pool_entry,
	               1,
	               0,
	               (uint8_t *) "header2",
	               7,
	               section_offset,
	               io_handle->compression_method,
	               LIBEWF_COMPRESSION_DEFAULT,
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
	if( libcdata_list_append_value(
	     segment_file->section_list,
	     (intptr_t *) section,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append section to list.",
		 function );

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

/* Writes a xheader section to file
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_xheader_section(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_header_sections_t *header_sections,
         libcerror_error_t **error )
{
	libewf_section_t *section = NULL;
	static char *function     = "libewf_segment_file_write_xheader_section";
	ssize_t write_count       = 0;

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
	if( libewf_section_initialize(
	     &section,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create section.",
		 function );

		goto on_error;
	}
	/* Do not include the end of string character in the compressed data
	 */
	write_count = libewf_section_write_compressed_string(
	               section,
	               io_handle,
	               file_io_pool,
	               file_io_pool_entry,
	               1,
	               0,
	               (uint8_t *) "xheader",
	               7,
	               section_offset,
	               io_handle->compression_method,
	               LIBEWF_COMPRESSION_DEFAULT,
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
	if( libcdata_list_append_value(
	     segment_file->section_list,
	     (intptr_t *) section,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append section to list.",
		 function );

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

/* Writes the header sections to file
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_header_sections(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
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
	     io_handle->compression_level,
	     io_handle->format,
	     io_handle->header_codepage,
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
	if( ( io_handle->format == LIBEWF_FORMAT_EWF )
	 || ( io_handle->format == LIBEWF_FORMAT_SMART )
	 || ( io_handle->format == LIBEWF_FORMAT_ENCASE1 ) )
	{
		/* The header should be written only once
		 * and using the compression used in the file
		 */
		write_count = libewf_segment_file_write_header_section(
		               segment_file,
		               io_handle,
		               file_io_pool,
		               file_io_pool_entry,
		               section_offset,
		               header_sections,
		               io_handle->compression_level,
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
		section_offset    += write_count;
	}
	else if( ( io_handle->format == LIBEWF_FORMAT_ENCASE2 )
	      || ( io_handle->format == LIBEWF_FORMAT_ENCASE3 )
	      || ( io_handle->format == LIBEWF_FORMAT_FTK_IMAGER )
	      || ( io_handle->format == LIBEWF_FORMAT_LINEN5 )
	      || ( io_handle->format == LIBEWF_FORMAT_LINEN6 )
	      || ( io_handle->format == LIBEWF_FORMAT_LINEN7 ) )
	{
		/* The header should be written twice
		 * the default compression is used
		 */
		write_count = libewf_segment_file_write_header_section(
		               segment_file,
		               io_handle,
		               file_io_pool,
		               file_io_pool_entry,
		               section_offset,
		               header_sections,
		               LIBEWF_COMPRESSION_DEFAULT,
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
		section_offset    += write_count;

		write_count = libewf_segment_file_write_header_section(
		               segment_file,
		               io_handle,
		               file_io_pool,
		               file_io_pool_entry,
		               section_offset,
		               header_sections,
		               LIBEWF_COMPRESSION_DEFAULT,
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
		section_offset    += write_count;
	}
	else if( ( io_handle->format == LIBEWF_FORMAT_ENCASE4 )
	      || ( io_handle->format == LIBEWF_FORMAT_ENCASE5 )
	      || ( io_handle->format == LIBEWF_FORMAT_ENCASE6 )
	      || ( io_handle->format == LIBEWF_FORMAT_ENCASE7 ) )
	{
		/* The header2 should be written twice
		 * the default compression is used
		 */
		write_count = libewf_segment_file_write_header2_section(
		               segment_file,
		               io_handle,
		               file_io_pool,
		               file_io_pool_entry,
		               section_offset,
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
		section_offset    += write_count;

		write_count = libewf_segment_file_write_header2_section(
		               segment_file,
		               io_handle,
		               file_io_pool,
		               file_io_pool_entry,
		               section_offset,
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
		section_offset    += write_count;

		/* The header should be written once
		 * the default compression is used
		 */
		write_count = libewf_segment_file_write_header_section(
		               segment_file,
		               io_handle,
		               file_io_pool,
		               file_io_pool_entry,
		               section_offset,
		               header_sections,
		               LIBEWF_COMPRESSION_DEFAULT,
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
		section_offset    += write_count;
	}
	/* EWFX uses the header and header2 for backwards compatibility
	 */
	else if( io_handle->format == LIBEWF_FORMAT_EWFX )
	{
		/* The xheader should be written once
		 * the default compression is used
		 */
		write_count = libewf_segment_file_write_xheader_section(
		               segment_file,
		               io_handle,
		               file_io_pool,
		               file_io_pool_entry,
		               section_offset,
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
		section_offset    += write_count;

		/* The header2 should be written once
		 * the default compression is used
		 */
		write_count = libewf_segment_file_write_header2_section(
		               segment_file,
		               io_handle,
		               file_io_pool,
		               file_io_pool_entry,
		               section_offset,
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
		section_offset    += write_count;

		/* The header should be written once
		 * the default compression is used
		 */
		write_count = libewf_segment_file_write_header_section(
		               segment_file,
		               io_handle,
		               file_io_pool,
		               file_io_pool_entry,
		               section_offset,
		               header_sections,
		               LIBEWF_COMPRESSION_DEFAULT,
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
		section_offset    += write_count;
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
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         int last_segment_file,
         libcerror_error_t **error )
{
	libewf_section_t *section           = NULL;
	uint8_t *section_type_string        = NULL;
	static char *function               = "libewf_segment_file_write_last_section";
	size_t section_descriptor_data_size = 0;
	size_t section_size                 = 0;
	size_t section_type_string_length   = 0;
	ssize_t write_count                 = 0;
	uint32_t section_type               = 0;

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
	if( libewf_section_initialize(
	     &section,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create section.",
		 function );

		goto on_error;
	}
	if( libewf_section_set_values(
	     section,
	     section_type,
	     section_type_string,
	     section_type_string_length,
	     section_offset,
	     section_size,
	     0,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set section values.",
		 function );

		goto on_error;
	}
	write_count = libewf_section_descriptor_write(
	               section,
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
	if( libcdata_list_append_value(
	     segment_file->section_list,
	     (intptr_t *) section,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
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
         uint8_t **case_data,
         size_t *case_data_size,
         uint8_t **device_information,
         size_t *device_information_size,
         ewf_data_t **data_section,
         libewf_media_values_t *media_values,
         libfvalue_table_t *header_values,
         time_t timestamp,
         libcerror_error_t **error )
{
	libewf_section_t *section = NULL;
	static char *function     = "libewf_segment_file_write_start";
	off64_t section_offset    = 0;
	ssize_t total_write_count = 0;
	ssize_t write_count       = 0;

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
	section_offset    += write_count;
	total_write_count += write_count;

	if( ( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1 )
	 || ( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART ) )
	{
		if( segment_file->segment_number == 1 )
		{
			write_count = libewf_segment_file_write_header_sections(
				       segment_file,
				       io_handle,
				       file_io_pool,
				       file_io_pool_entry,
				       section_offset,
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
			section_offset    += write_count;
		}
		if( libewf_section_initialize(
		     &section,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create section.",
			 function );

			goto on_error;
		}
		if( segment_file->segment_number == 1 )
		{
			write_count = -1;

			if( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1 )
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
			else if( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
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
				       section,
				       io_handle,
				       file_io_pool,
				       file_io_pool_entry,
				       section_offset,
				       media_values,
				       data_section,
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
		total_write_count += write_count;
		section_offset    += write_count;

		if( libcdata_list_append_value(
		     segment_file->section_list,
		     (intptr_t *) section,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append section to list.",
			 function );

			goto on_error;
		}
		section = NULL;
	}
	else if( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF2 )
	{
		write_count = libewf_segment_file_write_device_information_section(
			       segment_file,
			       io_handle,
			       file_io_pool,
			       file_io_pool_entry,
			       section_offset,
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
		section_offset    += write_count;

		write_count = libewf_segment_file_write_case_data_section(
			       segment_file,
			       io_handle,
			       file_io_pool,
			       file_io_pool_entry,
			       section_offset,
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
		section_offset    += write_count;
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
         uint8_t *table_section_data,
         size_t table_section_data_size,
         uint8_t *table_entries_data,
         size_t table_entries_data_size,
         uint32_t number_of_table_entries,
         uint64_t number_of_chunks_written,
         uint32_t chunks_per_section,
         libcerror_error_t **error )
{	
	libewf_section_t *section           = NULL;
	static char *function               = "libewf_segment_file_write_chunks_section_start";
	ssize_t write_count                 = 0;
	uint64_t estimated_number_of_chunks = 0;
	int number_of_chunks                = 0;

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
	if( number_of_table_entries < chunks_per_section )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: table entries too small.",
		 function );

		return( -1 );
	}
	if( libmfdata_list_get_number_of_elements(
	     chunk_table_list,
	     &number_of_chunks,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the number of chunks in the chunk table list.",
		 function );

		return( -1 );
	}
	/* The chunks_per_section contains the estimated number of chunks for this section
	 */
	estimated_number_of_chunks = number_of_chunks_written + chunks_per_section;

	if( estimated_number_of_chunks > (uint64_t) INT_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: estimated number of chunks value out of bounds.",
		 function );

		return( -1 );
	}
	if( number_of_chunks < (int) estimated_number_of_chunks )
	{
		if( libmfdata_list_resize(
		     chunk_table_list,
		     (int) estimated_number_of_chunks,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize chunk table list.",
			 function );

			return( -1 );
		}
	}
	if( libewf_section_initialize(
	     &section,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create section.",
		 function );

		goto on_error;
	}
	if( ( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
	 || ( io_handle->format == LIBEWF_FORMAT_ENCASE1 ) )
	{
		/* Write table section descriptor
		 */
		write_count = libewf_section_table_write(
		               section,
		               io_handle,
		               file_io_pool,
		               file_io_pool_entry,
		               1,
		               (uint8_t *) "table",
		               5,
		               section_offset,
		               table_section_data,
		               table_section_data_size,
		               number_of_chunks_written,
		               0,
		               table_entries_data,
		               table_entries_data_size,
		               chunks_per_section,
		               0,
		               segment_file->type,
		               error );

		if( write_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write table section.",
			 function );

			return( -1 );
		}
	}
	else if( ( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1 )
	      || ( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_LOGICAL ) )
	{
		write_count = libewf_section_sectors_write(
		               section,
		               file_io_pool,
		               file_io_pool_entry,
		               1,
		               section_offset,
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
	}
	if( libewf_section_free(
	     &section,
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
	if( section != NULL )
	{
		libewf_section_free(
		 &section,
		 NULL );
	}
	return( -1 );
}

/* Write the necessary sections after the actual data chunks to file
 * If necessary the sections before the actual data chunks are corrected
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_chunks_section_final(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libmfdata_list_t *chunk_table_list,
         uint8_t *table_section_data,
         size_t table_section_data_size,
         uint8_t *table_entries_data,
         size_t table_entries_data_size,
         uint32_t number_of_table_entries,
         off64_t chunks_section_offset,
         size64_t chunks_section_size,
         uint32_t chunks_section_padding_size,
         uint64_t number_of_chunks_written,
         uint32_t section_number_of_chunks,
         libcerror_error_t **error )
{
	libewf_section_t *group_section = NULL;
	libewf_section_t *section       = NULL;
	static char *function           = "libewf_segment_file_write_chunks_section_final";
	uint64_t base_offset            = 0;
	uint64_t chunk_index            = 0;
	ssize_t total_write_count       = 0;
	ssize_t write_count             = 0;

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
/* TODO what about linen 7 */
	if( ( io_handle->format == LIBEWF_FORMAT_ENCASE6 )
	 || ( io_handle->format == LIBEWF_FORMAT_ENCASE7 )
	 || ( io_handle->format == LIBEWF_FORMAT_V2_ENCASE7 ) )
	{
		if( chunks_section_size >= (size64_t) INT64_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid chunk section size value exceeds maximum.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( chunks_section_size >= (size64_t) INT32_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid chunk section size value exceeds maximum.",
			 function );

			return( -1 );
		}
	}
	if( ( (uint64_t) section_number_of_chunks > number_of_chunks_written )
	 || ( section_number_of_chunks > number_of_table_entries ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: section number of chunks value out of bounds.",
		 function );

		return( -1 );
	}
	chunk_index = number_of_chunks_written - section_number_of_chunks;

	if( chunk_index > (uint64_t) INT_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: chunk index value out of bounds.",
		 function );

		return( -1 );
	}
/* TODO what about linen 7 */
	if( ( io_handle->format == LIBEWF_FORMAT_ENCASE6 )
	 || ( io_handle->format == LIBEWF_FORMAT_ENCASE7 ) )
	{
		base_offset = chunks_section_offset;
	}
	if( libewf_chunk_table_generate_table_entries_data(
	     chunk_table_list,
	     (int) chunk_index,
	     segment_file->major_version,
	     table_entries_data,
	     table_entries_data_size,
	     section_number_of_chunks,
	     base_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set table entries data.",
		 function );

		goto on_error;
	}
	if( libewf_section_initialize(
	     &section,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create section.",
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
		 || ( io_handle->format == LIBEWF_FORMAT_ENCASE1 ) )
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
			/* Rewrite table section descriptor
			 */
			write_count = libewf_section_table_write(
				       section,
				       io_handle,
				       file_io_pool,
				       file_io_pool_entry,
				       1,
				       (uint8_t *) "table",
				       5,
				       chunks_section_offset,
				       table_section_data,
				       table_section_data_size,
				       chunk_index,
				       0,
				       table_entries_data,
				       table_entries_data_size,
				       section_number_of_chunks,
				       chunks_section_size,
				       segment_file->type,
				       error );

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
			group_section = section;
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
				       section,
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
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: setting file descriptor back to end of data at offset: %" PRIi64 ".\n",
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
		               section,
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
		section_offset    += write_count;
		total_write_count += write_count;
	}
	if( libcdata_list_append_value(
	     segment_file->section_list,
	     (intptr_t *) section,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append section to list.",
		 function );

		goto on_error;
	}
	section = NULL;

	if( ( segment_file->type != LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
	 && ( io_handle->format != LIBEWF_FORMAT_ENCASE1 ) )
	{
		if( libewf_section_initialize(
		     &section,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create section.",
			 function );

			goto on_error;
		}
		write_count = libewf_section_table_write(
		               section,
		               io_handle,
		               file_io_pool,
		               file_io_pool_entry,
		               segment_file->major_version,
		               (uint8_t *) "table",
		               5,
		               section_offset,
		               table_section_data,
		               table_section_data_size,
		               chunk_index,
		               base_offset,
		               table_entries_data,
		               table_entries_data_size,
		               section_number_of_chunks,
		               0,
		               segment_file->type,
		               error );

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
		section_offset    += write_count;
		total_write_count += write_count;

		if( libcdata_list_append_value(
		     segment_file->section_list,
		     (intptr_t *) section,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append section to list.",
			 function );

			goto on_error;
		}
		group_section = section;

		section = NULL;
	}
	if( group_section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing group section.",
		 function );

		goto on_error;
	}
	/* Group the elements to reduce the memory usage
	 */
	if( libmfdata_list_set_group_by_index(
	     chunk_table_list,
	     (int) chunk_index,
	     (int) section_number_of_chunks,
	     file_io_pool_entry,
	     group_section->start_offset,
	     group_section->size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set chunk group: %d - %d.",
		 function,
		 (int) chunk_index,
		 (int) number_of_chunks_written );

		goto on_error;
	}
	if( ( ( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1 )
	  ||  ( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_LOGICAL ) )
	 && ( io_handle->format != LIBEWF_FORMAT_ENCASE1 ) )
	{
		if( libewf_section_initialize(
		     &section,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create section.",
			 function );

			goto on_error;
		}
		write_count = libewf_section_table_write(
		               section,
		               io_handle,
		               file_io_pool,
		               file_io_pool_entry,
		               1,
		               (uint8_t *) "table2",
		               6,
		               section_offset,
		               table_section_data,
		               table_section_data_size,
		               chunk_index,
		               base_offset,
		               table_entries_data,
		               table_entries_data_size,
		               section_number_of_chunks,
		               0,
		               segment_file->type,
		               error );

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
		section_offset    += write_count;
		total_write_count += write_count;

		if( libcdata_list_append_value(
		     segment_file->section_list,
		     (intptr_t *) section,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append section to list.",
			 function );

			goto on_error;
		}
		group_section = section;

		section = NULL;

		if( libmfdata_list_set_backup_data_range_by_index(
		     chunk_table_list,
		     (int) chunk_index,
		     file_io_pool_entry,
		     group_section->start_offset,
		     group_section->size,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set backup data range of chunk group: %d - %d.",
			 function,
			 (int) chunk_index,
			 (int) number_of_chunks_written );

			goto on_error;
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
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_chunk_data(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libmfdata_list_t *chunk_table_list,
         int chunk_index,
         libewf_chunk_data_t *chunk_data,
         libcerror_error_t **error )
{
	static char *function       = "libewf_segment_file_write_chunk_data";
	off64_t segment_file_offset = 0;
	size_t write_size           = 0;
	ssize_t total_write_count   = 0;
	ssize_t write_count         = 0;
	int number_of_chunks        = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t chunk_checksum     = 0;
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
	if( chunk_data->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid chunk data - mssing data.",
		 function );

		return( -1 );
	}
	if( chunk_data->allocated_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chunk data - allocated data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( chunk_data->data_size > (size_t) UINT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chunk data - data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( chunk_data->padding_size > (size_t) UINT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chunk data - padding size value out of bounds.",
		 function );

		return( -1 );
	}
	if( io_handle->format != LIBEWF_FORMAT_SMART )
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
	if( libmfdata_list_get_number_of_elements(
	     chunk_table_list,
	     &number_of_chunks,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
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
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve current offset in segment file.",
		 function );

		return( -1 );
	}
	write_size = chunk_data->data_size + chunk_data->padding_size;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: chunk: %d file IO pool entry\t\t: %d\n",
		 function,
		 chunk_index,
		 file_io_pool_entry );

		libcnotify_printf(
		 "%s: chunk: %d offset\t\t\t: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 chunk_index,
		 segment_file_offset,
		 segment_file_offset );

		libcnotify_printf(
		 "%s: chunk: %d write size\t\t\t: %" PRIzd "\n",
		 function,
		 chunk_index,
		 write_size );

		libcnotify_printf(
		 "%s: chunk: %d data size\t\t\t: %" PRIzd "\n",
		 function,
		 chunk_index,
		 chunk_data->data_size );

		libcnotify_printf(
		 "%s: chunk: %d padding size\t\t\t: %" PRIzd "\n",
		 function,
		 chunk_index,
		 chunk_data->padding_size );

		if( ( chunk_data->range_flags & LIBEWF_CHUNK_DATA_FLAG_IS_COMPRESSED ) != 0 )
		{
			if( chunk_data->data_size < 4 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid chunk data size value out of bounds.",
				 function );

				return( -1 );
			}
			if( io_handle->compression_method == LIBEWF_COMPRESSION_METHOD_DEFLATE )
			{
				byte_stream_copy_to_uint32_little_endian(
				 &( ( chunk_data->data )[ chunk_data->data_size - 4 ] ),
				 chunk_checksum );
			}
			else if( io_handle->compression_method == LIBEWF_COMPRESSION_METHOD_BZIP2 )
			{
/* TODO bzip2 support */
			}
		}
		else if( ( chunk_data->range_flags & LIBEWF_RANGE_FLAG_HAS_CHECKSUM ) != 0 )
		{
/* TODO chunk data rewrite:
 * Set LIBEWF_CHUNK_IO_FLAG_CHECKSUM_SET chunk_data->chunk_io_flags if the checksum is not provided within the chunk data
 */
			if( ( ( chunk_data->chunk_io_flags & LIBEWF_CHUNK_IO_FLAG_CHECKSUM_SET ) != 0 )
			 && ( chunk_data->checksum_buffer != NULL ) )
			{
				byte_stream_copy_to_uint32_little_endian(
				 chunk_data->checksum_buffer,
				 chunk_checksum );
			}
/* TODO chunk data rewrite */
			else
			{
				if( ( chunk_data->data_size + 4 ) > chunk_data->allocated_data_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid chunk data size value out of bounds.",
					 function );

					return( -1 );
				}
				byte_stream_copy_to_uint32_little_endian(
				 &( ( chunk_data->data )[ chunk_data->data_size ] ),
				 chunk_checksum );
			}
		}
		libcnotify_printf(
		 "%s: chunk: %d checksum\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 chunk_index,
		 chunk_checksum );

		libcnotify_printf(
		 "%s: chunk: %d flags:\n",
		 function,
		 chunk_index );

		if( ( chunk_data->range_flags & LIBMFDATA_RANGE_FLAG_IS_COMPRESSED ) != 0 )
		{
			libcnotify_printf(
			 "Is compressed\n" );
		}
		if( ( chunk_data->range_flags & LIBEWF_RANGE_FLAG_HAS_CHECKSUM ) != 0 )
		{
			libcnotify_printf(
			 "Has checksum\n" );
		}
		if( ( chunk_data->range_flags & LIBEWF_RANGE_FLAG_USES_PATTERN_FILL ) != 0 )
		{
			libcnotify_printf(
			 "Uses pattern fill\n" );
		}
		libcnotify_printf(
		 "\n" );
	}
#endif
	/* Write the chunk data to the segment file
	 */
	write_count = libbfio_pool_write_buffer(
	               file_io_pool,
	               file_io_pool_entry,
	               chunk_data->data,
	               write_size,
	               error );

	if( write_count != (ssize_t) write_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write chunk: %d data.",
		 function,
		 chunk_index );

		return( -1 );
	}
	total_write_count += write_count;

	/* Check if the chunk and checksum buffers are aligned
	 * if not the chunk and checksum need to be written separately
	 */
	if( ( chunk_data->range_flags & LIBEWF_RANGE_FLAG_HAS_CHECKSUM ) != 0 )
	{
/* TODO chunk data rewrite */
		if( ( ( chunk_data->chunk_io_flags & LIBEWF_CHUNK_IO_FLAG_CHECKSUM_SET ) != 0 )
		 && ( chunk_data->checksum_buffer != NULL ) )
		{
			write_count = libbfio_pool_write_buffer(
				       file_io_pool,
				       file_io_pool_entry,
				       chunk_data->checksum_buffer,
				       4,
				       error );

			if( write_count != (ssize_t) 4 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write chunk: %d checksum.",
				 function,
				 chunk_index );

				return( -1 );
			}
			total_write_count += write_count;
		}
	}
	if( libmfdata_list_set_element_by_index(
	     chunk_table_list,
	     chunk_index,
	     file_io_pool_entry,
	     segment_file_offset,
	     (size64_t) total_write_count - chunk_data->padding_size,
	     chunk_data->range_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
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
         libewf_chunk_data_t *chunk_data,
	 uint8_t no_section_append,
         libcerror_error_t **error )
{
	libewf_section_t *section = NULL;
	static char *function     = "libewf_segment_file_write_delta_chunk";
	off64_t chunk_offset      = 0;
	size_t chunk_data_size    = 0;
	ssize_t write_count       = 0;
	int chunk_exists          = 0;

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
	if( ( section_offset < 0 )
	 || ( section_offset > (off64_t) INT64_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section offset value out of bounds.",
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
	if( chunk_data->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid chunk data - mssing data.",
		 function );

		return( -1 );
	}
	if( chunk_data->data_size > (size_t) UINT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chunk data - data size value out of bounds.",
		 function );

		return( -1 );
	}
	chunk_exists = libmfdata_list_is_set(
			chunk_table_list,
			chunk_index,
			error );

	if( chunk_exists == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if the chunk: %d exists in the chunk table.",
		 function,
		 chunk_index );

		goto on_error;
	}
	else if( chunk_exists == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing chunk: %d.",
		 function,
		 chunk_index );

		goto on_error;
	}
	chunk_offset = section_offset
	             + sizeof( ewf_section_descriptor_v1_t )
	             + sizeof( ewfx_delta_chunk_header_t );

	if( libewf_section_initialize(
	     &section,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
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
	               chunk_data->data,
	               (uint32_t) chunk_data->data_size,
	               chunk_data->checksum_buffer,
	               chunk_data->chunk_io_flags,
	               error );

	if( write_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write delta chunk.",
		 function );

		goto on_error;
	}
	if( no_section_append == 0 )
	{
		if( libcdata_list_append_value(
		     segment_file->section_list,
		     (intptr_t *) section,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
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
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free section.",
			 function );

			section = NULL;

			goto on_error;
		}
	}
/* TODO chunk data rewrite  */
	chunk_data_size = chunk_data->data_size;

	if( ( ( chunk_data->chunk_io_flags & LIBEWF_CHUNK_IO_FLAG_CHECKSUM_SET ) != 0 )
	 && ( chunk_data->checksum_buffer != NULL ) )
	{
		chunk_data_size += 4;
	}
	chunk_data->range_flags |= LIBEWF_RANGE_FLAG_IS_DELTA;

	if( libmfdata_list_set_element_by_index(
	     chunk_table_list,
	     chunk_index,
	     file_io_pool_entry,
	     chunk_offset,
	     (size64_t) chunk_data_size,
	     chunk_data->range_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
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

/* Writes the hash sections to file
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_hash_sections(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_hash_sections_t *hash_sections,
         libfvalue_table_t *hash_values,
         libcerror_error_t **error )
{
	libewf_section_t *section = NULL;
	static char *function     = "libewf_segment_file_write_hash_sections";
	ssize_t total_write_count = 0;
	ssize_t write_count       = 0;

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
	if( ( io_handle->format == LIBEWF_FORMAT_ENCASE6 )
	 || ( io_handle->format == LIBEWF_FORMAT_ENCASE7 )
	 || ( io_handle->format == LIBEWF_FORMAT_LINEN6 )
	 || ( io_handle->format == LIBEWF_FORMAT_LINEN7 ) )
	{
		/* Write the digest section if required
		 */
		if( hash_sections->sha1_digest_set != 0 )
		{
			if( libewf_section_initialize(
			     &section,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create section.",
				 function );

				goto on_error;
			}
			write_count = libewf_section_digest_write(
				       section,
				       io_handle,
				       file_io_pool,
				       file_io_pool_entry,
				       section_offset,
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
			section_offset    += write_count;
			total_write_count += write_count;

			if( libcdata_list_append_value(
			     segment_file->section_list,
			     (intptr_t *) section,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append section to list.",
				 function );

				goto on_error;
			}
			section = NULL;
		}
	}
	/* Write the MD5 hash section if required
	 */
	if( hash_sections->md5_hash_set != 0 )
	{
		if( libewf_section_initialize(
		     &section,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create section.",
			 function );

			goto on_error;
		}
		write_count = libewf_section_md5_hash_write(
			       section,
			       io_handle,
			       file_io_pool,
			       file_io_pool_entry,
			       segment_file->major_version,
			       section_offset,
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
		section_offset    += write_count;
		total_write_count += write_count;

		if( libcdata_list_append_value(
		     segment_file->section_list,
		     (intptr_t *) section,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append section to list.",
			 function );

			goto on_error;
		}
		section = NULL;
	}
	if( io_handle->format == LIBEWF_FORMAT_V2_ENCASE7 )
	{
		/* Write the SHA1 hash section if required
		 */
		if( hash_sections->sha1_hash_set != 0 )
		{
			if( libewf_section_initialize(
			     &section,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create section.",
				 function );

				goto on_error;
			}
			write_count = libewf_section_sha1_hash_write(
				       section,
				       io_handle,
				       file_io_pool,
				       file_io_pool_entry,
				       segment_file->major_version,
				       section_offset,
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
			section_offset    += write_count;
			total_write_count += write_count;

			if( libcdata_list_append_value(
			     segment_file->section_list,
			     (intptr_t *) section,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append section to list.",
				 function );

				goto on_error;
			}
			section = NULL;
		}
	}
	if( io_handle->format == LIBEWF_FORMAT_EWFX )
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
		if( libewf_section_initialize(
		     &section,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create section.",
			 function );

			goto on_error;
		}
		/* Do not include the end of string character in the compressed data
		 */
		write_count = libewf_section_write_compressed_string(
			       section,
			       io_handle,
			       file_io_pool,
			       file_io_pool_entry,
			       1,
			       0,
			       (uint8_t *) "xhash",
			       5,
			       section_offset,
			       io_handle->compression_method,
			       LIBEWF_COMPRESSION_DEFAULT,
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
		section_offset    += write_count;
		total_write_count += write_count;

		if( libcdata_list_append_value(
		     segment_file->section_list,
		     (intptr_t *) section,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
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

/* Closes the segment file, necessary sections at the end of the segment file will be written
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_close(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         uint64_t number_of_chunks_written_to_segment_file,
         int last_segment_file,
         libewf_hash_sections_t *hash_sections,
         libfvalue_table_t *hash_values,
         libewf_media_values_t *media_values,
         libcdata_array_t *sessions,
         libcdata_array_t *tracks,
         libcdata_range_list_t *acquiry_errors,
         ewf_data_t **data_section,
	 libcerror_error_t **error )
{
	libewf_section_t *section    = NULL;
	static char *function        = "libewf_segment_file_write_close";
	ssize_t total_write_count    = 0;
	ssize_t write_count          = 0;
	int number_of_acquiry_errors = 0;
	int number_of_sessions       = 0;

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
				if( libewf_section_initialize(
				     &section,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
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
					       data_section,
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
				section_offset    += write_count;
				total_write_count += write_count;

				if( libcdata_list_append_value(
				     segment_file->section_list,
				     (intptr_t *) section,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
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
		 || ( io_handle->format == LIBEWF_FORMAT_ENCASE7 )
		 || ( io_handle->format == LIBEWF_FORMAT_LINEN5 )
		 || ( io_handle->format == LIBEWF_FORMAT_LINEN6 )
		 || ( io_handle->format == LIBEWF_FORMAT_LINEN7 )
		 || ( io_handle->format == LIBEWF_FORMAT_V2_ENCASE7 )
		 || ( io_handle->format == LIBEWF_FORMAT_EWFX ) )
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
				if( libewf_section_initialize(
				     &section,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create section.",
					 function );

					goto on_error;
				}
				write_count = libewf_section_session_write(
					       section,
					       io_handle,
					       file_io_pool,
					       file_io_pool_entry,
					       segment_file->major_version,
					       section_offset,
					       sessions,
					       tracks,
					       error );

				if( write_count == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_WRITE_FAILED,
					 "%s: unable to write sessions section.",
					 function );

					goto on_error;
				}
				section_offset    += write_count;
				total_write_count += write_count;

				if( libcdata_list_append_value(
				     segment_file->section_list,
				     (intptr_t *) section,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append section to list.",
					 function );

					goto on_error;
				}
				section = NULL;
			}
		}
		/* Write the error section if required
		 */
		if( ( io_handle->format == LIBEWF_FORMAT_ENCASE3 )
		 || ( io_handle->format == LIBEWF_FORMAT_ENCASE4 )
		 || ( io_handle->format == LIBEWF_FORMAT_ENCASE5 )
		 || ( io_handle->format == LIBEWF_FORMAT_ENCASE6 )
		 || ( io_handle->format == LIBEWF_FORMAT_ENCASE7 )
		 || ( io_handle->format == LIBEWF_FORMAT_LINEN5 )
		 || ( io_handle->format == LIBEWF_FORMAT_LINEN6 )
		 || ( io_handle->format == LIBEWF_FORMAT_LINEN7 )
		 || ( io_handle->format == LIBEWF_FORMAT_V2_ENCASE7 )
		 || ( io_handle->format == LIBEWF_FORMAT_EWFX ) )
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
				if( libewf_section_initialize(
				     &section,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create section.",
					 function );

					goto on_error;
				}
				write_count = libewf_section_error_write(
					       section,
					       io_handle,
					       file_io_pool,
					       file_io_pool_entry,
					       segment_file->major_version,
					       section_offset,
					       acquiry_errors,
					       error );

				if( write_count == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_WRITE_FAILED,
					 "%s: unable to write error section.",
					 function );

					goto on_error;
				}
				section_offset    += write_count;
				total_write_count += write_count;

				if( libcdata_list_append_value(
				     segment_file->section_list,
				     (intptr_t *) section,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append section to list.",
					 function );

					goto on_error;
				}
				section = NULL;
			}
		}
		/* Write the hash sections
		 */
		write_count = libewf_segment_file_write_hash_sections(
			       segment_file,
			       io_handle,
			       file_io_pool,
			       file_io_pool_entry,
			       section_offset,
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
		section_offset    += write_count;
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write end of segment file.",
		 function );

		goto on_error;
	}
	total_write_count += write_count;
	section_offset    += write_count;

	segment_file->number_of_chunks = number_of_chunks_written_to_segment_file;

	/* Make sure the next time the file is opened it is not truncated
	 */
	if( libbfio_pool_reopen(
	     file_io_pool,
	     file_io_pool_entry,
	     LIBBFIO_OPEN_READ_WRITE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
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
     ewf_data_t **data_section,
     libcerror_error_t **error )
{
	libcdata_list_element_t *section_list_element = NULL;
	libewf_section_t *section                     = NULL;
	static char *function                         = "libewf_segment_file_write_sections_correction";
	off64_t next_section_start_offset             = 0;
	ssize_t write_count                           = 0;
	int correct_last_next_section                 = 0;

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
	if( segment_file->section_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment file - missing section list.",
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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: correcting sections in segment file: %" PRIu16 ".\n",
		 function,
		 segment_file->segment_number );	
	}
#endif
	if( libcdata_list_get_element_by_index(
	     segment_file->section_list,
	     0,
	     &section_list_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve first element from section list.",
		 function );

		return( -1 );
	}
	if( section_list_element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing first section list element.",
		 function );

		return( -1 );
	}
	while( section_list_element != NULL )
	{
		if( libcdata_list_element_get_value(
		     section_list_element,
		     (intptr_t **) &section,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value from section list element.",
			 function );

			return( -1 );
		}
		if( section == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing section.",
			 function );

			return( -1 );
		}
		if( section->type != 0 )
		{
			switch( section->type )
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

							return( -1 );
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
					     section->start_offset,
					     SEEK_SET,
					     error ) == -1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_SEEK_FAILED,
						 "%s: unable to find offset to correct device information section.",
						 function );

						return( -1 );
					}
					/* Do not include the end of string character in the compressed data
					 */
					write_count = libewf_section_write_compressed_string(
						       section,
						       io_handle,
						       file_io_pool,
						       file_io_pool_entry,
						       2,
						       LIBEWF_SECTION_TYPE_DEVICE_INFORMATION,
						       NULL,
						       0,
						       section->start_offset,
						       io_handle->compression_method,
						       LIBEWF_COMPRESSION_DEFAULT,
						       *device_information,
						       *device_information_size - 2,
						       (size_t) section->data_size,
						       error );

					if( write_count == -1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_WRITE_FAILED,
						 "%s: unable to write device information section.",
						 function );

						return( -1 );
					}
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
						     io_handle->format,
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
							 "%s: unable to generate case data.",
							 function );

							return( -1 );
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
					     section->start_offset,
					     SEEK_SET,
					     error ) == -1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_SEEK_FAILED,
						 "%s: unable to find offset to correct case data section.",
						 function );

						return( -1 );
					}
					/* Do not include the end of string character in the compressed data
					 */
					write_count = libewf_section_write_compressed_string(
						       section,
						       io_handle,
						       file_io_pool,
						       file_io_pool_entry,
						       2,
						       LIBEWF_SECTION_TYPE_CASE_DATA,
						       NULL,
						       0,
						       section->start_offset,
						       io_handle->compression_method,
						       LIBEWF_COMPRESSION_DEFAULT,
						       *case_data,
						       *case_data_size - 2,
						       (size_t) section->data_size,
						       error );

					if( write_count == -1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_WRITE_FAILED,
						 "%s: unable to write case data section.",
						 function );

						return( -1 );
					}
					break;

				case LIBEWF_SECTION_TYPE_NEXT:
					/* The last segment file should be terminated with a done section and not with a next section
					 */
					if( last_segment_file != 0 )
					{
						correct_last_next_section = 1;
						next_section_start_offset = section->start_offset;
					}
					break;
			}
		}
		else if( section->type_string_length == 6 )
		{
			if( memory_compare(
			     section->type_string,
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
				     section->start_offset,
				     SEEK_SET,
				     error ) == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_SEEK_FAILED,
					 "%s: unable to find offset to correct volume section.",
					 function );

					return( -1 );
				}
				if( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1 )
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
				else if( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
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

					return( -1 );
				}
			}
		}
		else if( section->type_string_length == 4 )
		{
			if( memory_compare(
			     section->type_string,
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
				     section->start_offset,
				     SEEK_SET,
				     error ) == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_SEEK_FAILED,
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
					       data_section,
					       error );

				if( write_count == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_WRITE_FAILED,
					 "%s: unable to correct data section.",
					 function );

					return( -1 );
				}
			}
		}
		if( libcdata_list_element_get_next_element(
		     section_list_element,
		     &section_list_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve next element from section list element.",
			 function );

			return( -1 );
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

			return( -1 );
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

			return( -1 );
		}
		write_count = libewf_segment_file_write_close(
			       segment_file,
			       io_handle,
			       file_io_pool,
			       file_io_pool_entry,
			       next_section_start_offset,
			       number_of_chunks_written_to_segment_file,
			       1,
			       hash_sections,
			       hash_values,
			       media_values,
			       sessions,
			       tracks,
			       acquiry_errors,
			       data_section,
			       error );

		if( write_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to close segment file.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

