/*
 * Session section functions
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
#include "libewf_hash_sections.h"
#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_libcdata.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_section.h"
#include "libewf_section_descriptor.h"
#include "libewf_sector_range.h"
#include "libewf_sector_range_list.h"
#include "libewf_session_section.h"

#include "ewf_session.h"

/* Reads a version 1 session section or version 2 session table section
 * Returns 1 if successful or -1 on error
 */
int libewf_session_section_read_data(
     const uint8_t *data,
     size_t data_size,
     uint8_t format_version,
     libewf_media_values_t *media_values,
     libcdata_array_t *sessions,
     libcdata_array_t *tracks,
     libcerror_error_t **error )
{
	const uint8_t *session_data        = NULL;
	const uint8_t *session_entry_data  = NULL;
	static char *function              = "libewf_session_section_read_data";
	size_t session_entries_data_size   = 0;
	size_t session_entry_data_size     = 0;
	size_t session_footer_data_size    = 0;
	size_t session_header_data_size    = 0;
	uint64_t number_of_sectors         = 0;
	uint64_t previous_start_sector     = 0;
	uint64_t session_start_sector      = 0;
	uint64_t start_sector              = 0;
	uint64_t track_start_sector        = 0;
	uint32_t calculated_checksum       = 0;
	uint32_t flags                     = 0;
	uint32_t number_of_session_entries = 0;
	uint32_t previous_flags            = 0;
	uint32_t session_entry_index       = 0;
	uint32_t stored_checksum           = 0;

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
	if( format_version == 1 )
	{
		session_header_data_size = sizeof( ewf_session_header_v1_t );
		session_entry_data_size  = sizeof( ewf_session_entry_v1_t );
		session_footer_data_size = 4;
	}
	else if( format_version == 2 )
	{
		session_header_data_size = sizeof( ewf_session_header_v2_t );
		session_entry_data_size  = sizeof( ewf_session_entry_v2_t );
		session_footer_data_size = 16;
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
	if( ( data_size < session_header_data_size )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
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
	session_data = data;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: session header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 session_header_data_size,
		 0 );
	}
#endif
	if( format_version == 1 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_session_header_v1_t *) data )->number_of_entries,
		 number_of_session_entries );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_session_header_v1_t *) data )->checksum,
		 stored_checksum );
	}
	else if( format_version == 2 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_session_header_v2_t *) data )->number_of_entries,
		 number_of_session_entries );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_session_header_v2_t *) data )->checksum,
		 stored_checksum );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: number of entries\t\t\t: %" PRIu32 "\n",
		 function,
		 number_of_session_entries );

		libcnotify_printf(
		 "%s: unknown1:\n",
		 function );

		if( format_version == 1 )
		{
			libcnotify_print_data(
			 ( (ewf_session_header_v1_t *) data )->unknown1,
			 28,
			 0 );
		}
		else if( format_version == 2 )
		{
			libcnotify_print_data(
			 ( (ewf_session_header_v2_t *) data )->unknown1,
			 12,
			 0 );
		}
		libcnotify_printf(
		 "%s: checksum\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_checksum );

		if( format_version == 1 )
		{
			libcnotify_printf(
			 "\n" );
		}
		else if( format_version == 2 )
		{
			libcnotify_printf(
			 "%s: padding:\n",
			 function );
			libcnotify_print_data(
			 ( (ewf_session_header_v2_t *) data )->padding,
			 12,
			 0 );
		}
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( format_version == 2 )
	{
		session_header_data_size -= 12;
	}
	if( libewf_checksum_calculate_adler32(
	     &calculated_checksum,
	     data,
	     session_header_data_size - 4,
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to calculate header checksum.",
		 function );

		goto on_error;
	}
	if( format_version == 2 )
	{
		session_header_data_size += 12;
	}
	session_data = &( data[ session_header_data_size ] );
	data_size   -= session_header_data_size;

	if( stored_checksum != calculated_checksum )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
		 "%s: header checksum does not match (stored: 0x%08" PRIx32 ", calculated: 0x%08" PRIx32 ").",
		 function,
		 stored_checksum,
		 calculated_checksum );

		goto on_error;
	}
	if( number_of_session_entries > 0 )
	{
		session_entry_data = session_data;

		session_entries_data_size = number_of_session_entries * session_entry_data_size;

		if( session_entries_data_size > (size_t) SSIZE_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid session entries data size value exceeds maximum.",
			 function );

			goto on_error;
		}
		if( data_size < session_entries_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid section size value out of bounds - insufficient space for entries.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: session entries data:\n",
			 function );
			libcnotify_print_data(
			 session_data,
			 session_entries_data_size,
			 0 );
		}
#endif
		if( libewf_checksum_calculate_adler32(
		     &calculated_checksum,
		     session_data,
		     session_entries_data_size,
		     1,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to calculate entries checksum.",
			 function );

			goto on_error;
		}
		session_data += session_entries_data_size;
		data_size    -= session_entries_data_size;

		if( data_size < session_footer_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid section size value out of bounds - insufficient space for footer.",
			 function );

			goto on_error;
		}
		byte_stream_copy_to_uint32_little_endian(
		 session_data,
		 stored_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: session entries checksum\t\t: 0x%08" PRIx32 "\n",
			 function,
			 stored_checksum );

			if( format_version == 1 )
			{
				libcnotify_printf(
				 "\n" );
			}
			else if( format_version == 2 )
			{
				libcnotify_printf(
				 "%s: padding:\n",
				 function );
				libcnotify_print_data(
				 &( session_data[ 4 ] ),
				 12,
				 0 );
			}
		}
#endif
		session_data      += session_footer_data_size;
		data_size -= session_footer_data_size;

		if( stored_checksum != calculated_checksum )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_INPUT,
			 LIBCERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
			 "%s: entries checksum does not match (stored: 0x%08" PRIx32 ", calculated: 0x%08" PRIx32 ").",
			 function,
			 stored_checksum,
			 calculated_checksum );

			goto on_error;
		}
#if defined( HAVE_VERBOSE_OUTPUT ) || defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( data_size > 0 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				libcnotify_printf(
				 "%s: trailing data:\n",
				 function );
				libcnotify_print_data(
				 session_data,
				 data_size,
				 0 );

#elif defined( HAVE_VERBOSE_OUTPUT )
				libcnotify_printf(
				 "%s: section has trailing data.\n",
				 function );
#endif
			}
		}
#endif
		if( libcdata_array_empty(
		     sessions,
		     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_sector_range_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to empty sessions array.",
			 function );

			goto on_error;
		}
		if( format_version == 1 )
		{
			byte_stream_copy_to_uint32_little_endian(
			 ( (ewf_session_entry_v1_t *) session_entry_data )->flags,
			 previous_flags );

			byte_stream_copy_to_uint32_little_endian(
			 ( (ewf_session_entry_v1_t *) session_entry_data )->start_sector,
			 previous_start_sector );
		}
		else if( format_version == 2 )
		{
			byte_stream_copy_to_uint64_little_endian(
			 ( (ewf_session_entry_v2_t *) session_entry_data )->start_sector,
			 previous_start_sector );

			byte_stream_copy_to_uint32_little_endian(
			 ( (ewf_session_entry_v2_t *) session_entry_data )->flags,
			 previous_flags );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( format_version == 1 )
			{
				libcnotify_printf(
				 "%s: entry: %02" PRIu32 " flags\t\t\t: 0x%08" PRIx32 "\n",
				 function,
				 session_entry_index,
				 previous_flags );

				libcnotify_printf(
				 "%s: entry: %02" PRIu32 " start sector\t\t: 0 (%" PRIu64 ")\n",
				 function,
				 session_entry_index,
				 previous_start_sector );
			}
			else if( format_version == 2 )
			{
				libcnotify_printf(
				 "%s: entry: %02" PRIu32 " start sector\t\t: 0 (%" PRIu64 ")\n",
				 function,
				 session_entry_index,
				 previous_start_sector );

				libcnotify_printf(
				 "%s: entry: %02" PRIu32 " flags\t\t\t: 0x%08" PRIx32 "\n",
				 function,
				 session_entry_index,
				 previous_flags );
			}
			libcnotify_printf(
			 "%s: entry: %02" PRIu32 " unknown1:\n",
			 function,
			 session_entry_index );

			if( format_version == 1 )
			{
				libcnotify_print_data(
				 ( (ewf_session_entry_v1_t *) session_entry_data )->unknown1,
				 24,
				 0 );
			}
			else if( format_version == 2 )
			{
				libcnotify_print_data(
				 ( (ewf_session_entry_v2_t *) session_entry_data )->unknown1,
				 20,
				 0 );
			}
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		session_entry_data += session_entry_data_size;

		/* Note that EnCase says the first session starts at session 16
		 * This is either some EnCase specific behavior or the value is used for
		 * other purposes.
		 */
		session_start_sector = 0;
		track_start_sector   = 0;

		for( session_entry_index = 1;
		     session_entry_index < number_of_session_entries;
		     session_entry_index++ )
		{
			if( format_version == 1 )
			{
				byte_stream_copy_to_uint32_little_endian(
				 ( (ewf_session_entry_v1_t *) session_entry_data )->flags,
				 flags );

				byte_stream_copy_to_uint32_little_endian(
				 ( (ewf_session_entry_v1_t *) session_entry_data )->start_sector,
				 start_sector );
			}
			else if( format_version == 2 )
			{
				byte_stream_copy_to_uint64_little_endian(
				 ( (ewf_session_entry_v2_t *) session_entry_data )->start_sector,
				 start_sector );

				byte_stream_copy_to_uint32_little_endian(
				 ( (ewf_session_entry_v2_t *) session_entry_data )->flags,
				 flags );
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				if( format_version == 1 )
				{
					libcnotify_printf(
					 "%s: entry: %02" PRIu32 " flags\t\t\t: 0x%08" PRIx32 "\n",
					 function,
					 session_entry_index,
					 flags );

					libcnotify_printf(
					 "%s: entry: %02" PRIu32 " start sector\t\t: %" PRIu64 "\n",
					 function,
					 session_entry_index,
					 start_sector );
				}
				else if( format_version == 2 )
				{
					libcnotify_printf(
					 "%s: entry: %02" PRIu32 " start sector\t\t: %" PRIu64 "\n",
					 function,
					 session_entry_index,
					 start_sector );

					libcnotify_printf(
					 "%s: entry: %02" PRIu32 " flags\t\t\t: 0x%08" PRIx32 "\n",
					 function,
					 session_entry_index,
					 flags );
				}
				libcnotify_printf(
				 "%s: entry: %02" PRIu32 " unknown1:\n",
				 function,
				 session_entry_index );

				if( format_version == 1 )
				{
					libcnotify_print_data(
					 ( (ewf_session_entry_v1_t *) session_entry_data )->unknown1,
					 24,
					 0 );
				}
				else if( format_version == 2 )
				{
					libcnotify_print_data(
					 ( (ewf_session_entry_v2_t *) session_entry_data )->unknown1,
					 20,
					 0 );
				}
			}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

			session_entry_data += session_entry_data_size;

			if( start_sector < previous_start_sector )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid start sector: %" PRIu32 " value out of bounds.",
				 function,
				 start_sector );

				goto on_error;
			}
			if( ( flags & LIBEWF_SESSION_ENTRY_FLAGS_IS_AUDIO_TRACK ) == 0 )
			{
/* TODO bounds check */
				number_of_sectors = start_sector - session_start_sector;

				if( libewf_sector_range_list_append_range(
				     sessions,
				     session_start_sector,
				     (uint64_t) number_of_sectors,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append session sector range.",
					 function );

					goto on_error;
				}
				session_start_sector = start_sector;
			}
			if( ( previous_flags & LIBEWF_SESSION_ENTRY_FLAGS_IS_AUDIO_TRACK ) != 0 )
			{
/* TODO bounds check */
				number_of_sectors = start_sector - track_start_sector;

				if( libewf_sector_range_list_append_range(
				     tracks,
				     track_start_sector,
				     (uint64_t) number_of_sectors,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append track sector range.",
					 function );

					goto on_error;
				}
				track_start_sector = start_sector;
			}
			previous_start_sector = start_sector;
			previous_flags        = flags;
		}
		if( media_values->number_of_sectors > session_start_sector )
		{
			number_of_sectors = (uint32_t) ( media_values->number_of_sectors - session_start_sector );
		}
		else
		{
			number_of_sectors = 0;
		}
		if( libewf_sector_range_list_append_range(
		     sessions,
		     session_start_sector,
		     (uint64_t) number_of_sectors,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append session sector range.",
			 function );

			goto on_error;
		}
		if( ( flags & LIBEWF_SESSION_ENTRY_FLAGS_IS_AUDIO_TRACK ) != 0 )
		{
			if( media_values->number_of_sectors > track_start_sector )
			{
				number_of_sectors = (uint32_t) ( media_values->number_of_sectors - track_start_sector );
			}
			else
			{
				number_of_sectors = 0;
			}
			if( libewf_sector_range_list_append_range(
			     tracks,
			     track_start_sector,
			     (uint64_t) number_of_sectors,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append track sector range.",
				 function );

				goto on_error;
			}
		}
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	else if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: session section contains no entries.\n",
		 function );
	}
#endif
	return( 1 );

on_error:
	libcdata_array_empty(
	 tracks,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libewf_sector_range_free,
	 error );

	libcdata_array_empty(
	 sessions,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libewf_sector_range_free,
	 error );

	return( -1 );
}

/* Reads a version 1 session section or version 2 session table section
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_session_section_read_file_io_pool(
         libewf_section_descriptor_t *section_descriptor,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         libewf_media_values_t *media_values,
         libcdata_array_t *sessions,
         libcdata_array_t *tracks,
         libcerror_error_t **error )
{
	uint8_t *section_data    = NULL;
	static char *function    = "libewf_session_section_read_file_io_pool";
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
		if( libewf_session_section_read_data(
		     section_data,
		     section_data_size,
		     format_version,
		     media_values,
		     sessions,
		     tracks,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read section data.",
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

/* Writes a session section
 * Returns 1 if successful or -1 on error
 */
int libewf_session_section_write_data(
     uint8_t *data,
     size_t data_size,
     uint8_t format_version,
     libcdata_array_t *sessions,
     libcdata_array_t *tracks,
     int number_of_session_entries,
     libcerror_error_t **error )
{
	uint8_t *session_data              = NULL;
	uint8_t *session_entry_data        = NULL;
	static char *function              = "libewf_session_section_write_data";
	size_t required_data_size          = 0;
	size_t session_entries_data_size   = 0;
	size_t session_entry_data_size     = 0;
	size_t session_footer_data_size    = 0;
	size_t session_header_data_size    = 0;
	uint64_t current_sector            = 0;
	uint64_t session_last_sector       = 0;
	uint64_t session_number_of_sectors = 0;
	uint64_t session_start_sector      = 0;
	uint64_t track_last_sector         = 0;
	uint64_t track_number_of_sectors   = 0;
	uint64_t track_start_sector        = 0;
	uint32_t calculated_checksum       = 0;
	uint32_t session_entry_index       = 0;
	int number_of_sessions             = 0;
	int number_of_tracks               = 0;
	int session_index                  = 0;
	int track_index                    = 0;

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
	if( format_version == 1 )
	{
		session_header_data_size = sizeof( ewf_session_header_v1_t );
		session_entry_data_size  = sizeof( ewf_session_entry_v1_t );
		session_footer_data_size = 4;
	}
	else if( format_version == 2 )
	{
		session_header_data_size = sizeof( ewf_session_header_v2_t );
		session_entry_data_size  = sizeof( ewf_session_entry_v2_t );
		session_footer_data_size = 16;
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
	session_entries_data_size = number_of_session_entries * session_entry_data_size;

	required_data_size = session_header_data_size
	                   + session_entries_data_size
	                   + session_footer_data_size;

	if( ( data_size < required_data_size )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
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
	session_data = data;

/* TODO check if number_of_session_entries is in bounds */
	if( format_version == 1 )
	{
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_session_header_v1_t *) session_data )->number_of_entries,
		 (uint32_t) number_of_session_entries );
	}
	else if( format_version == 2 )
	{
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_session_header_v2_t *) session_data )->number_of_entries,
		 (uint32_t) number_of_session_entries );

		session_header_data_size -= 12;
	}
	if( libewf_checksum_calculate_adler32(
	     &calculated_checksum,
	     session_data,
	     session_header_data_size - 4,
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to calculate header checksum.",
		 function );

		return( -1 );
	}
	if( format_version == 1 )
	{
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_session_header_v1_t *) session_data )->checksum,
		 calculated_checksum );
	}
	else if( format_version == 2 )
	{
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_session_header_v2_t *) session_data )->checksum,
		 calculated_checksum );

		session_header_data_size += 12;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: session header data:\n",
		 function );
		libcnotify_print_data(
		 session_data,
		 session_header_data_size,
		 0 );
	}
#endif
	session_data += session_header_data_size;

	session_entry_data   = session_data;
	current_sector       = 0;
	session_start_sector = 0;
	session_index        = 0;
	session_last_sector  = 0;
	track_start_sector   = 0;
	track_index          = 0;
	track_last_sector    = 0;

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

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     tracks,
	     &number_of_tracks,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from tracks array.",
		 function );

		return( -1 );
	}
	do
	{
		if( ( session_index < number_of_sessions )
		 && ( current_sector >= session_last_sector ) )
		{
			if( libewf_sector_range_list_get_range(
			     sessions,
			     session_index,
			     &session_start_sector,
			     &session_number_of_sectors,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve session: %d sector range.",
				 function,
				 session_index );

				return( -1 );
			}
			session_last_sector = session_start_sector
					    + session_number_of_sectors;

			session_index++;
		}
		if( ( track_index < number_of_tracks )
		 && ( current_sector >= track_last_sector ) )
		{
			if( libewf_sector_range_list_get_range(
			     tracks,
			     track_index,
			     &track_start_sector,
			     &track_number_of_sectors,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve track: %d sector range.",
				 function,
				 track_index );

				return( -1 );
			}
			track_last_sector = track_start_sector
					  + track_number_of_sectors;

			track_index++;
		}
		if( ( number_of_tracks > 0 )
		 && ( current_sector >= track_start_sector )
		 && ( current_sector < track_last_sector ) )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: entry: %02" PRIu32 " flags\t\t\t\t: 1\n",
				 function,
				 session_entry_index );

				libcnotify_printf(
				 "%s: entry: %02" PRIu32 " start sector\t\t\t: %" PRIu32 "\n",
				 function,
				 session_entry_index,
				 track_start_sector );

				libcnotify_printf(
				 "%s: entry: %02" PRIu32 " last sector\t\t\t: %" PRIu32 "\n",
				 function,
				 session_entry_index,
				 track_last_sector );

				libcnotify_printf(
				 "\n" );
			}
#endif
			/* Note that EnCase says the first track starts at sector 16
			 * This is either some EnCase specific behavior or the value is used for
			 * other purposes.
			 */
			if( ( session_entry_index == 0 )
			 && ( track_start_sector == 0 ) )
			{
				track_start_sector = 16;
			}
			if( format_version == 1 )
			{
				byte_stream_copy_from_uint32_little_endian(
				 ( (ewf_session_entry_v1_t *) session_entry_data )->flags,
				 LIBEWF_SESSION_ENTRY_FLAGS_IS_AUDIO_TRACK );

				byte_stream_copy_from_uint32_little_endian(
				 ( (ewf_session_entry_v1_t *) session_entry_data )->start_sector,
				 (uint32_t) track_start_sector );
			}
			else if( format_version == 2 )
			{
				byte_stream_copy_from_uint64_little_endian(
				 ( (ewf_session_entry_v2_t *) session_entry_data )->start_sector,
				 track_start_sector );

				byte_stream_copy_from_uint32_little_endian(
				 ( (ewf_session_entry_v2_t *) session_entry_data )->flags,
				 LIBEWF_SESSION_ENTRY_FLAGS_IS_AUDIO_TRACK );
			}
			session_entry_data += session_entry_data_size;

			session_entry_index++;

			current_sector = track_last_sector;
		}
		else if( ( number_of_sessions > 0 )
		      && ( current_sector >= session_start_sector )
		      && ( current_sector < session_last_sector ) )
		{
			if( ( track_last_sector == 0 )
			 || ( track_last_sector < session_start_sector ) )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: entry: %02" PRIu32 " flags\t\t\t\t: 0\n",
					 function,
					 session_entry_index );

					libcnotify_printf(
					 "%s: entry: %02" PRIu32 " start sector\t\t\t: %" PRIu32 "\n",
					 function,
					 session_entry_index,
					 session_start_sector );

					libcnotify_printf(
					 "%s: entry: %02" PRIu32 " last sector\t\t\t: %" PRIu32 "\n",
					 function,
					 session_entry_index,
					 session_last_sector );

					libcnotify_printf(
					 "\n" );
				}
#endif
				/* Note that EnCase says the first session starts at sector 16
				 * This is either some EnCase specific behavior or the value is used for
				 * other purposes.
				 */
				if( ( session_entry_index == 0 )
				 && ( session_start_sector == 0 ) )
				{
					session_start_sector = 16;
				}
				if( format_version == 1 )
				{
					byte_stream_copy_from_uint32_little_endian(
					 ( (ewf_session_entry_v1_t *) session_entry_data )->start_sector,
					 (uint32_t) session_start_sector );
				}
				else if( format_version == 2 )
				{
					byte_stream_copy_from_uint64_little_endian(
					 ( (ewf_session_entry_v2_t *) session_entry_data )->start_sector,
					 session_start_sector );
				}
				session_entry_data += session_entry_data_size;

				session_entry_index++;
			}
			current_sector = session_last_sector;
		}
		if( session_entry_index >= (uint32_t) number_of_session_entries )
		{
			break;
		}
	}
	while( ( session_index < number_of_sessions )
	    || ( track_index < number_of_tracks ) );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: session entries data:\n",
		 function );
		libcnotify_print_data(
		 session_data,
		 session_entries_data_size,
		 0 );
	}
#endif
	if( libewf_checksum_calculate_adler32(
	     &calculated_checksum,
	     session_data,
	     session_entries_data_size,
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to calculate entries checksum.",
		 function );

		return( -1 );
	}
	session_data += session_entries_data_size;

	byte_stream_copy_from_uint32_little_endian(
	 session_data,
	 calculated_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: session footer data:\n",
		 function );
		libcnotify_print_data(
		 session_data,
		 session_footer_data_size,
		 0 );
	}
#endif
	return( 1 );
}

/* Writes a session section
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_session_section_write_file_io_pool(
         libewf_section_descriptor_t *section_descriptor,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         off64_t section_offset,
         libcdata_array_t *sessions,
         libcdata_array_t *tracks,
         libcerror_error_t **error )
{
	uint8_t *section_data               = NULL;
	static char *function               = "libewf_session_section_write_file_io_pool";
	size_t section_data_size            = 0;
	size_t section_descriptor_data_size = 0;
	size_t session_entries_data_size    = 0;
	size_t session_entry_data_size      = 0;
	size_t session_footer_data_size     = 0;
	size_t session_header_data_size     = 0;
	ssize_t total_write_count           = 0;
	ssize_t write_count                 = 0;
	uint64_t current_sector             = 0;
	uint64_t session_last_sector        = 0;
	uint64_t session_number_of_sectors  = 0;
	uint64_t session_start_sector       = 0;
	uint64_t track_last_sector          = 0;
	uint64_t track_number_of_sectors    = 0;
	uint64_t track_start_sector         = 0;
	uint32_t section_padding_size       = 0;
	int number_of_session_entries       = 0;
	int number_of_sessions              = 0;
	int number_of_tracks                = 0;
	int session_index                   = 0;
	int track_index                     = 0;

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
	if( format_version == 1 )
	{
		section_descriptor_data_size = sizeof( ewf_section_descriptor_v1_t );
		session_header_data_size     = sizeof( ewf_session_header_v1_t );
		session_entry_data_size      = sizeof( ewf_session_entry_v1_t );
		session_footer_data_size     = 4;
	}
	else if( format_version == 2 )
	{
		section_descriptor_data_size = sizeof( ewf_section_descriptor_v2_t );
		session_header_data_size     = sizeof( ewf_session_header_v2_t );
		session_entry_data_size      = sizeof( ewf_session_entry_v2_t );
		session_footer_data_size     = 16;
		section_padding_size         = 24;
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
	if( number_of_sessions < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of sessions value out of bounds.",
		 function );

		goto on_error;
	}
	if( libcdata_array_get_number_of_entries(
	     tracks,
	     &number_of_tracks,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from tracks array.",
		 function );

		goto on_error;
	}
	if( number_of_tracks < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of tracks value out of bounds.",
		 function );

		goto on_error;
	}
	if( ( number_of_sessions != 0 )
	 && ( number_of_tracks == 0 ) )
	{
		number_of_session_entries = number_of_sessions;
	}
	else if( ( number_of_sessions == 0 )
	      && ( number_of_tracks != 0 ) )
	{
		number_of_session_entries = number_of_tracks;
	}
	else if( ( number_of_sessions != 0 )
	      && ( number_of_tracks != 0 ) )
	{
		/* Encase does not store sessions containing tracks
		 * therefore the number of sessions entries needs to be determined
		 * from the run-time tracks and session information
		 */
		do
		{
			if( ( session_index < number_of_sessions )
			 && ( current_sector >= session_last_sector ) )
			{
				if( libewf_sector_range_list_get_range(
				     sessions,
				     session_index,
				     &session_start_sector,
				     &session_number_of_sectors,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve session: %d sector range.",
					 function,
					 session_index );

					goto on_error;
				}
				session_last_sector = session_start_sector
						    + session_number_of_sectors;

				session_index++;
			}
			if( ( track_index < number_of_tracks )
			 && ( current_sector >= track_last_sector ) )
			{
				if( libewf_sector_range_list_get_range(
				     tracks,
				     track_index,
				     &track_start_sector,
				     &track_number_of_sectors,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve track: %d sector range.",
					 function,
					 track_index );

					goto on_error;
				}
				track_last_sector = track_start_sector
						  + track_number_of_sectors;

				track_index++;
			}
			if( ( number_of_tracks > 0 )
			 && ( current_sector >= track_start_sector )
			 && ( current_sector < track_last_sector ) )
			{
				number_of_session_entries++;

				current_sector = track_last_sector;
			}
			else if( ( number_of_sessions > 0 )
			      && ( current_sector >= session_start_sector )
			      && ( current_sector < session_last_sector ) )
			{
				if( ( track_last_sector == 0 )
				 || ( track_last_sector < session_start_sector ) )
				{
					number_of_session_entries++;
				}
				current_sector = session_last_sector;
			}
		}
		while( ( session_index < number_of_sessions )
		    || ( track_index < number_of_tracks ) );
	}
	if( number_of_session_entries == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of sessions entries value out of bounds.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: number of entries\t\t\t: %" PRIu32 "\n",
		 function,
		 number_of_session_entries );

		libcnotify_printf(
		 "\n" );
	}
#endif
	session_entries_data_size = number_of_session_entries * session_entry_data_size;

	section_data_size = session_header_data_size
	                  + session_entries_data_size
	                  + session_footer_data_size;

	if( libewf_section_descriptor_set(
	     section_descriptor,
	     LIBEWF_SECTION_TYPE_SESSION_TABLE,
	     (uint8_t *) "session",
	     7,
	     section_offset,
	     section_descriptor_data_size + section_data_size,
	     section_data_size,
	     section_padding_size,
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
	if( format_version == 1 )
	{
		write_count = libewf_section_descriptor_write_file_io_pool(
			       section_descriptor,
			       file_io_pool,
			       file_io_pool_entry,
			       format_version,
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
		total_write_count += write_count;
	}
	if( ( section_data_size == 0 )
	 || ( section_data_size > MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section data size value out of bounds.",
		 function );

		goto on_error;
	}
	section_data = (uint8_t *) memory_allocate(
	                            section_data_size );

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
	if( libewf_session_section_write_data(
	     section_data,
	     section_data_size,
	     format_version,
	     sessions,
	     tracks,
	     number_of_session_entries,
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
	               section_data_size,
	               error );

	if( write_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section data.",
		 function );

		goto on_error;
	}
	total_write_count += write_count;

	memory_free(
	 section_data );

	section_data = NULL;

	if( format_version == 2 )
	{
		write_count = libewf_section_descriptor_write_file_io_pool(
			       section_descriptor,
			       file_io_pool,
			       file_io_pool_entry,
			       format_version,
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
		total_write_count += write_count;
	}
	return( total_write_count );

on_error:
	if( section_data != NULL )
	{
		memory_free(
		 section_data );
	}
	return( -1 );
}

