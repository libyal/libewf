/*
 * Volume section functions
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
#include <narrow_string.h>
#include <types.h>

#include "libewf_checksum.h"
#include "libewf_definitions.h"
#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_media_values.h"
#include "libewf_section.h"
#include "libewf_section_descriptor.h"
#include "libewf_volume_section.h"

#include "ewf_volume.h"

/* Reads an EWF-E01 (EnCase) volume section
 * Returns 1 if successful or -1 on error
 */
int libewf_volume_section_e01_read_data(
     const uint8_t *data,
     size_t data_size,
     libewf_io_handle_t *io_handle,
     libewf_media_values_t *media_values,
     libcerror_error_t **error )
{
	static char *function        = "libewf_volume_section_e01_read_data";
	uint32_t calculated_checksum = 0;
	uint32_t stored_checksum     = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit         = 0;
#endif

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
	if( data_size != (size_t) sizeof( ewf_volume_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
	 	 "%s: volume section data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	media_values->media_type = ( (ewf_volume_t *) data )->media_type;

	byte_stream_copy_to_uint32_little_endian(
	 ( (ewf_volume_t *) data )->number_of_chunks,
	 media_values->number_of_chunks );

	byte_stream_copy_to_uint32_little_endian(
	 ( (ewf_volume_t *) data )->sectors_per_chunk,
	 media_values->sectors_per_chunk );

	byte_stream_copy_to_uint32_little_endian(
	 ( (ewf_volume_t *) data )->bytes_per_sector,
	 media_values->bytes_per_sector );

	byte_stream_copy_to_uint64_little_endian(
	 ( (ewf_volume_t *) data )->number_of_sectors,
	 media_values->number_of_sectors );

	media_values->media_flags = ( (ewf_volume_t *) data )->media_flags;

	io_handle->compression_level = (int8_t) ( (ewf_volume_t *) data )->compression_level;

	byte_stream_copy_to_uint32_little_endian(
	 ( (ewf_volume_t *) data )->error_granularity,
	 media_values->error_granularity );

	if( memory_copy(
	     media_values->set_identifier,
	     ( (ewf_volume_t *) data )->set_identifier,
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
	byte_stream_copy_to_uint32_little_endian(
	 ( (ewf_volume_t *) data )->checksum,
	 stored_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: media type\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 media_values->media_type );

		libcnotify_printf(
		 "%s: unknown1:\n",
		 function );
		libcnotify_print_data(
		 ( (ewf_volume_t *) data )->unknown1,
		 3,
		 0 );

		libcnotify_printf(
		 "%s: number of chunks\t\t\t: %" PRIu64 "\n",
		 function,
		 media_values->number_of_chunks );

		libcnotify_printf(
		 "%s: sectors per chunk\t\t\t: %" PRIu32 "\n",
		 function,
		 media_values->sectors_per_chunk );

		libcnotify_printf(
		 "%s: bytes per sector\t\t\t: %" PRIu32 "\n",
		 function,
		 media_values->bytes_per_sector );

		libcnotify_printf(
		 "%s: number of sectors\t\t\t: %" PRIu64 "\n",
		 function,
		 media_values->number_of_sectors );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_volume_t *) data )->chs_cylinders,
		 value_32bit );
		libcnotify_printf(
		 "%s: CHS number of cylinders\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_volume_t *) data )->chs_heads,
		 value_32bit );
		libcnotify_printf(
		 "%s: CHS number of heads\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_volume_t *) data )->chs_sectors,
		 value_32bit );
		libcnotify_printf(
		 "%s: CHS number of sectors\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: media flags\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 media_values->media_flags );

		libcnotify_printf(
		 "%s: unknown2:\n",
		 function );
		libcnotify_print_data(
		 ( (ewf_volume_t *) data )->unknown2,
		 3,
		 0 );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_volume_t *) data )->palm_volume_start_sector,
		 value_32bit );
		libcnotify_printf(
		 "%s: PALM volume start sector\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: unknown3:\n",
		 function );
		libcnotify_print_data(
		 ( (ewf_volume_t *) data )->unknown3,
		 4,
		 0 );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_volume_t *) data )->smart_logs_start_sector,
		 value_32bit );
		libcnotify_printf(
		 "%s: SMART logs start sector\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: compression level\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 io_handle->compression_level );

		libcnotify_printf(
		 "%s: unknown4:\n",
		 function );
		libcnotify_print_data(
		 ( (ewf_volume_t *) data )->unknown4,
		 3,
		 0 );

		libcnotify_printf(
		 "%s: error granularity\t\t\t: %" PRIu32 "\n",
		 function,
		 media_values->error_granularity );

		libcnotify_printf(
		 "%s: unknown5:\n",
		 function );
		libcnotify_print_data(
		 ( (ewf_volume_t *) data )->unknown5,
		 4,
		 0 );

		libcnotify_printf(
		 "%s: set identifier:\n",
		 function );
		libcnotify_print_data(
		 ( (ewf_volume_t *) data )->set_identifier,
		 16,
		 0 );

		libcnotify_printf(
		 "%s: unknown6:\n",
		 function );
		libcnotify_print_data(
		 ( (ewf_volume_t *) data )->unknown6,
		 963,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

		libcnotify_printf(
		 "%s: signature:\n",
		 function );
		libcnotify_print_data(
		 ( (ewf_volume_t *) data )->signature,
		 5,
		 0 );

		libcnotify_printf(
		 "%s: checksum\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_checksum );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( libewf_checksum_calculate_adler32(
	     &calculated_checksum,
	     data,
	     data_size - 4,
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to calculate checksum.",
		 function );

		return( -1 );
	}
	if( stored_checksum != calculated_checksum )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
		 "%s: checksum does not match (stored: 0x%08" PRIx32 ", calculated: 0x%08" PRIx32 ").",
		 function,
		 stored_checksum,
		 calculated_checksum );

		return( -1 );
	}
	return( 1 );
}

/* Reads an EWF-E01 (EnCase) volume section
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_volume_section_e01_read_file_io_pool(
         libewf_section_descriptor_t *section_descriptor,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_media_values_t *media_values,
         libcerror_error_t **error )
{
	uint8_t *section_data    = NULL;
	static char *function    = "libewf_volume_section_e01_read_file_io_pool";
	size_t section_data_size = 0;
	ssize_t read_count       = 0;

	if( section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section descriptor.",
		 function );

		return( -1 );
	}
	read_count = libewf_section_read_data(
	              section_descriptor,
	              io_handle,
	              file_io_pool,
	              file_io_pool_entry,
	              &section_data,
	              &section_data_size,
	              error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read section data.",
		 function );

		goto on_error;
	}
	else if( read_count != 0 )
	{
		if( libewf_volume_section_e01_read_data(
		     section_data,
		     section_data_size,
		     io_handle,
		     media_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read EWF-E01 volume section.",
			 function );

			goto on_error;
		}
		memory_free(
		 section_data );
	}
	return( read_count );

on_error:
	if( section_data != NULL )
	{
		memory_free(
		 section_data );
	}
	return( -1 );
}

/* Reads an EWF-E01 (EnCase) volume section
 * Returns 1 if successful or -1 on error
 */
int libewf_volume_section_e01_write_data(
     uint8_t *data,
     size_t data_size,
     libewf_io_handle_t *io_handle,
     libewf_media_values_t *media_values,
     libcerror_error_t **error )
{
	static char *function        = "libewf_volume_section_e01_write_data";
	uint32_t calculated_checksum = 0;

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
	if( data_size != (size_t) sizeof( ewf_volume_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
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
	if( memory_set(
	     data,
	     0,
	     data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear data.",
		 function );

		return( 1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: media type\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 media_values->media_type );

		libcnotify_printf(
		 "%s: number of chunks\t\t\t: %" PRIu64 "\n",
		 function,
		 media_values->number_of_chunks );

		libcnotify_printf(
		 "%s: sectors per chunk\t\t\t: %" PRIu32 "\n",
		 function,
		 media_values->sectors_per_chunk );

		libcnotify_printf(
		 "%s: bytes per sector\t\t\t: %" PRIu32 "\n",
		 function,
		 media_values->bytes_per_sector );

		libcnotify_printf(
		 "%s: number of sectors\t\t\t: %" PRIu64 "\n",
		 function,
		 media_values->number_of_sectors );

		libcnotify_printf(
		 "%s: media flags\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 media_values->media_flags );

		libcnotify_printf(
		 "%s: compression level\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 io_handle->compression_level );

		libcnotify_printf(
		 "%s: error granularity\t\t\t: %" PRIu32 "\n",
		 function,
		 media_values->error_granularity );

		libcnotify_printf(
		 "%s: set identifier:\n",
		 function );
		libcnotify_print_data(
		 media_values->set_identifier,
		 16,
		 0 );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	( (ewf_volume_t *) data )->media_type = media_values->media_type;
	( (ewf_volume_t *) data )->media_flags = media_values->media_flags;

	byte_stream_copy_from_uint32_little_endian(
	 ( (ewf_volume_t *) data )->number_of_chunks,
	 (uint32_t) media_values->number_of_chunks );

	byte_stream_copy_from_uint32_little_endian(
	 ( (ewf_volume_t *) data )->sectors_per_chunk,
	 media_values->sectors_per_chunk );

	byte_stream_copy_from_uint32_little_endian(
	 ( (ewf_volume_t *) data )->bytes_per_sector,
	 media_values->bytes_per_sector );

	byte_stream_copy_from_uint64_little_endian(
	 ( (ewf_volume_t *) data )->number_of_sectors,
	 media_values->number_of_sectors );

	if( ( io_handle->format == LIBEWF_FORMAT_ENCASE5 )
	 || ( io_handle->format == LIBEWF_FORMAT_ENCASE6 )
	 || ( io_handle->format == LIBEWF_FORMAT_ENCASE7 )
	 || ( io_handle->format == LIBEWF_FORMAT_LINEN5 )
	 || ( io_handle->format == LIBEWF_FORMAT_LINEN6 )
	 || ( io_handle->format == LIBEWF_FORMAT_LINEN7 )
	 || ( io_handle->format == LIBEWF_FORMAT_EWFX ) )
	{
		( (ewf_volume_t *) data )->compression_level = (uint8_t) io_handle->compression_level;

		if( memory_copy(
		     ( (ewf_volume_t *) data )->set_identifier,
		     media_values->set_identifier,
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy set identifier.",
			 function );

			return( 1 );
		}
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_volume_t *) data )->error_granularity,
		 media_values->error_granularity );
	}
	if( libewf_checksum_calculate_adler32(
	     &calculated_checksum,
	     data,
	     data_size - 4,
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to calculate checksum.",
		 function );

		return( 1 );
	}
	byte_stream_copy_from_uint32_little_endian(
	 ( (ewf_volume_t *) data )->checksum,
	 calculated_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: volume has %" PRIu64 " chunks of %" PRIi32 " bytes (%" PRIi32 " sectors) each.\n",
		 function,
		 media_values->number_of_chunks,
		 media_values->chunk_size,
		 media_values->sectors_per_chunk );

		libcnotify_printf(
		 "%s: volume has %" PRIu64 " sectors of %" PRIi32 " bytes each.\n",
		 function,
		 media_values->number_of_sectors,
		 media_values->bytes_per_sector );
	}
#endif
	return( 1 );
}

/* Writes an EWF-E01 (EnCase) volume section
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_volume_section_e01_write_file_io_pool(
         libewf_section_descriptor_t *section_descriptor,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_media_values_t *media_values,
         libcerror_error_t **error )
{
	uint8_t *section_data     = NULL;
	static char *function     = "libewf_volume_section_e01_write_file_io_pool";
	ssize_t total_write_count = 0;
	ssize_t write_count       = 0;

	if( section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section descriptor.",
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
	if( media_values->number_of_chunks > (uint64_t) UINT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid media values - number of chunks value out of bounds.",
		 function );

		return( -1 );
	}
	if( libewf_section_descriptor_set(
	     section_descriptor,
	     0,
	     (uint8_t *) "volume",
	     6,
	     section_offset,
	     (size64_t) ( sizeof( ewf_section_descriptor_v1_t ) + sizeof( ewf_volume_t ) ),
	     (size64_t) sizeof( ewf_volume_t ),
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
	               1,
	               error );

	if( write_count != (ssize_t) sizeof( ewf_section_descriptor_v1_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section descriptor.",
		 function );

		goto on_error;
	}
	total_write_count += write_count;

	section_data = (uint8_t *) memory_allocate(
	                            sizeof( ewf_volume_t ) );

	if( section_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create section data.",
		 function );

		goto on_error;
	}
	if( libewf_volume_section_e01_write_data(
	     section_data,
	     sizeof( ewf_volume_t ),
	     io_handle,
	     media_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to write section data.",
		 function );

		goto on_error;
	}
	write_count = libewf_section_write_data(
	               section_descriptor,
	               io_handle,
	               file_io_pool,
	               file_io_pool_entry,
	               section_data,
	               sizeof( ewf_volume_t ),
	               error );

	if( write_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section data.",
		 function );

		return( -1 );
	}
	total_write_count += write_count;

	memory_free(
	 section_data );

	return( total_write_count );

on_error:
	if( section_data != NULL )
	{
		memory_free(
		 section_data );
	}
	return( -1 );
}

/* Reads an EWF-S01 (SMART) volume section
 * Returns 1 if successful or -1 on error
 */
int libewf_volume_section_s01_read_data(
     const uint8_t *data,
     size_t data_size,
     libewf_io_handle_t *io_handle,
     libewf_media_values_t *media_values,
     libcerror_error_t **error )
{
	static char *function        = "libewf_volume_section_s01_read_data";
	uint32_t calculated_checksum = 0;
	uint32_t stored_checksum     = 0;

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
	if( data_size != (size_t) sizeof( ewf_volume_smart_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: volume data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (ewf_volume_smart_t *) data )->number_of_chunks,
	 media_values->number_of_chunks );

	byte_stream_copy_to_uint32_little_endian(
	 ( (ewf_volume_smart_t *) data )->sectors_per_chunk,
	 media_values->sectors_per_chunk );

	byte_stream_copy_to_uint32_little_endian(
	 ( (ewf_volume_smart_t *) data )->bytes_per_sector,
	 media_values->bytes_per_sector );

	byte_stream_copy_to_uint32_little_endian(
	 ( (ewf_volume_smart_t *) data )->number_of_sectors,
	 media_values->number_of_sectors );

	byte_stream_copy_to_uint32_little_endian(
	 ( (ewf_volume_smart_t *) data )->checksum,
	 stored_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: unknown1:\n",
		 function );
		libcnotify_print_data(
		 ( (ewf_volume_smart_t *) data )->unknown1,
		 4,
		 0 );

		libcnotify_printf(
		 "%s: number of chunks\t\t: %" PRIu64 "\n",
		 function,
		 media_values->number_of_chunks );

		libcnotify_printf(
		 "%s: sectors per chunk\t\t: %" PRIu32 "\n",
		 function,
		 media_values->sectors_per_chunk );

		libcnotify_printf(
		 "%s: bytes per sector\t\t: %" PRIu32 "\n",
		 function,
		 media_values->bytes_per_sector );

		libcnotify_printf(
		 "%s: number of sectors\t\t: %" PRIu64 "\n",
		 function,
		 media_values->number_of_sectors );

		libcnotify_printf(
		 "%s: unknown2:\n",
		 function );
		libcnotify_print_data(
		 ( (ewf_volume_smart_t *) data )->unknown2,
		 20,
		 0 );

		libcnotify_printf(
		 "%s: unknown3:\n",
		 function );
		libcnotify_print_data(
		 ( (ewf_volume_smart_t *) data )->unknown3,
		 45,
		 0 );

		libcnotify_printf(
		 "%s: signature:\n",
		 function );
		libcnotify_print_data(
		 ( (ewf_volume_smart_t *) data )->signature,
		 5,
		 0 );

		libcnotify_printf(
		 "%s: checksum\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_checksum );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( memory_compare(
	     (void *) ( (ewf_volume_smart_t *) data )->signature,
	     (void *) "SMART",
	     5 ) == 0 )
	{
		io_handle->format = LIBEWF_FORMAT_SMART;
	}
	else
	{
		io_handle->format = LIBEWF_FORMAT_EWF;
	}
	if( libewf_checksum_calculate_adler32(
	     &calculated_checksum,
	     data,
	     data_size - 4,
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to calculate checksum.",
		 function );

		return( -1 );
	}
	if( stored_checksum != calculated_checksum )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
		 "%s: checksum does not match (stored: 0x%08" PRIx32 ", calculated: 0x%08" PRIx32 ").",
		 function,
		 stored_checksum,
		 calculated_checksum );

		return( -1 );
	}
	return( 1 );
}

/* Reads an EWF-S01 (SMART) volume section
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_volume_section_s01_read_file_io_pool(
         libewf_section_descriptor_t *section_descriptor,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_media_values_t *media_values,
         libcerror_error_t **error )
{
	uint8_t *section_data    = NULL;
	static char *function    = "libewf_volume_section_s01_read_file_io_pool";
	size_t section_data_size = 0;
	ssize_t read_count       = 0;

	if( section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section descriptor.",
		 function );

		return( -1 );
	}
	read_count = libewf_section_read_data(
	              section_descriptor,
	              io_handle,
	              file_io_pool,
	              file_io_pool_entry,
	              &section_data,
	              &section_data_size,
	              error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read section data.",
		 function );

		goto on_error;
	}
	else if( read_count == 0 )
	{
		return( 0 );
	}
	if( libewf_volume_section_s01_read_data(
	     section_data,
	     section_data_size,
	     io_handle,
	     media_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read EWF-S01 volume section.",
		 function );

		goto on_error;
	}
	memory_free(
	 section_data );

	return( read_count );

on_error:
	if( section_data != NULL )
	{
		memory_free(
		 section_data );
	}
	return( -1 );
}

/* Writes an EWF-S01 (SMART) volume section
 * Returns 1 if successful or -1 on error
 */
int libewf_volume_section_s01_write_data(
     uint8_t *data,
     size_t data_size,
     libewf_io_handle_t *io_handle,
     libewf_media_values_t *media_values,
     libcerror_error_t **error )
{
	static char *function        = "libewf_volume_section_s01_write_data";
	uint32_t calculated_checksum = 0;

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
	if( data_size != (size_t) sizeof( ewf_volume_smart_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
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
	if( memory_set(
	     data,
	     0,
	     data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear data.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: media type\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 media_values->media_type );

		libcnotify_printf(
		 "%s: number of chunks\t\t\t: %" PRIu64 "\n",
		 function,
		 media_values->number_of_chunks );

		libcnotify_printf(
		 "%s: sectors per chunk\t\t\t: %" PRIu32 "\n",
		 function,
		 media_values->sectors_per_chunk );

		libcnotify_printf(
		 "%s: bytes per sector\t\t\t: %" PRIu32 "\n",
		 function,
		 media_values->bytes_per_sector );

		libcnotify_printf(
		 "%s: number of sectors\t\t\t: %" PRIu64 "\n",
		 function,
		 media_values->number_of_sectors );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	( (ewf_volume_smart_t *) data )->unknown1[ 0 ] = 1;

	byte_stream_copy_from_uint32_little_endian(
	 ( (ewf_volume_smart_t *) data )->number_of_chunks,
	 (uint32_t) media_values->number_of_chunks );

	byte_stream_copy_from_uint32_little_endian(
	 ( (ewf_volume_smart_t *) data )->sectors_per_chunk,
	 media_values->sectors_per_chunk );

	byte_stream_copy_from_uint32_little_endian(
	 ( (ewf_volume_smart_t *) data )->bytes_per_sector,
	 media_values->bytes_per_sector );

	byte_stream_copy_from_uint32_little_endian(
	 ( (ewf_volume_smart_t *) data )->number_of_sectors,
	 media_values->number_of_sectors );

	if( io_handle->format == LIBEWF_FORMAT_SMART )
	{
		( (ewf_volume_smart_t *) data )->signature[ 0 ] = (uint8_t) 'S';
		( (ewf_volume_smart_t *) data )->signature[ 1 ] = (uint8_t) 'M';
		( (ewf_volume_smart_t *) data )->signature[ 2 ] = (uint8_t) 'A';
		( (ewf_volume_smart_t *) data )->signature[ 3 ] = (uint8_t) 'R';
		( (ewf_volume_smart_t *) data )->signature[ 4 ] = (uint8_t) 'T';
	}
	if( libewf_checksum_calculate_adler32(
	     &calculated_checksum,
	     data,
	     data_size - 4,
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to calculate checksum.",
		 function );

		return( -1 );
	}
	byte_stream_copy_from_uint32_little_endian(
	 ( (ewf_volume_smart_t *) data )->checksum,
	 calculated_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: volume has %" PRIu64 " chunks of %" PRIi32 " bytes (%" PRIi32 " sectors) each.\n",
		 function,
		 media_values->number_of_chunks,
		 media_values->chunk_size,
		 media_values->sectors_per_chunk );

		libcnotify_printf(
		 "%s: volume has %" PRIu64 " sectors of %" PRIi32 " bytes each.\n",
		 function,
		 media_values->number_of_sectors,
		 media_values->bytes_per_sector );
	}
#endif
	return( 1 );
}

/* Writes an EWF-S01 (SMART) volume section
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_volume_section_s01_write_file_io_pool(
         libewf_section_descriptor_t *section_descriptor,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_media_values_t *media_values,
         libcerror_error_t **error )
{
	uint8_t *section_data     = NULL;
	static char *function     = "libewf_volume_section_s01_write_file_io_pool";
	ssize_t total_write_count = 0;
	ssize_t write_count       = 0;

	if( section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section descriptor.",
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
	if( media_values->number_of_chunks > (uint64_t) UINT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid media values - number of chunks value out of bounds.",
		 function );

		return( -1 );
	}
	if( libewf_section_descriptor_set(
	     section_descriptor,
	     0,
	     (uint8_t *) "volume",
	     6,
	     section_offset,
	     (size64_t) ( sizeof( ewf_section_descriptor_v1_t ) + sizeof( ewf_volume_smart_t ) ),
	     (size64_t) sizeof( ewf_volume_smart_t ),
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
	               1,
	               error );

	if( write_count != (ssize_t) sizeof( ewf_section_descriptor_v1_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section descriptor.",
		 function );

		goto on_error;
	}
	total_write_count += write_count;

	section_data = (uint8_t *) memory_allocate(
	                            sizeof( ewf_volume_smart_t ) );

	if( section_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create section data.",
		 function );

		goto on_error;
	}
	if( libewf_volume_section_s01_write_data(
	     section_data,
	     sizeof( ewf_volume_smart_t ),
	     io_handle,
	     media_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to write section data.",
		 function );

		goto on_error;
	}
	write_count = libewf_section_write_data(
	               section_descriptor,
	               io_handle,
	               file_io_pool,
	               file_io_pool_entry,
	               section_data,
	               sizeof( ewf_volume_smart_t ),
	               error );

	if( write_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section data.",
		 function );

		return( -1 );
	}
	total_write_count += write_count;

	memory_free(
	 section_data );

	return( total_write_count );

on_error:
	if( section_data != NULL )
	{
		memory_free(
		 section_data );
	}
	return( -1 );
}

