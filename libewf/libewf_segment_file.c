/*
 * Segment file reading/writing functions
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
#include <endian.h>
#include <memory.h>
#include <types.h>

#include <liberror.h>

#include "libewf_definitions.h"
#include "libewf_hash_values.h"
#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_notify.h"
#include "libewf_section.h"
#include "libewf_segment_file.h"
#include "libewf_segment_table.h"
#include "libewf_string.h"
#include "libewf_system_string.h"

#include "ewf_definitions.h"
#include "ewf_file_header.h"
#include "ewfx_delta_chunk.h"

const uint8_t dvf_file_signature[ 8 ] = { 0x64, 0x76, 0x66, 0x09, 0x0D, 0x0A, 0xFF, 0x00 };
const uint8_t evf_file_signature[ 8 ] = { 0x45, 0x56, 0x46, 0x09, 0x0D, 0x0A, 0xFF, 0x00 };
const uint8_t lvf_file_signature[ 8 ] = { 0x4c, 0x56, 0x46, 0x09, 0x0D, 0x0A, 0xFF, 0x00 };

/* Reads the file header from a segment file
 * Returns the amount of bytes read if successful, or -1 on errror
 */
ssize_t libewf_segment_file_read_file_header(
         libewf_segment_file_handle_t *segment_file_handle,
         uint16_t *segment_number,
         libbfio_pool_t *file_io_pool,
         liberror_error_t **error )
{
	ewf_file_header_t file_header;

	static char *function = "libewf_segment_file_read_file_header";
	ssize_t read_count    = 0;

	if( segment_file_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file handle.",
		 function );

		return( -1 );
	}
	if( segment_number == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment number.",
		 function );

		return( -1 );
	}
	read_count = libbfio_pool_read(
	              file_io_pool,
	              segment_file_handle->file_io_pool_entry,
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
	/* The amount of EWF segment files will be the largest
	 */
	if( memory_compare(
	     evf_file_signature,
	     file_header.signature,
	     8 ) == 0 )
	{
		segment_file_handle->file_type = LIBEWF_SEGMENT_FILE_TYPE_EWF;
	}
	else if( memory_compare(
	          lvf_file_signature,
	          file_header.signature,
	          8 ) == 0 )
	{
		segment_file_handle->file_type = LIBEWF_SEGMENT_FILE_TYPE_LWF;
	}
	else if( memory_compare(
	          dvf_file_signature,
	          file_header.signature,
	          8 ) == 0 )
	{
		segment_file_handle->file_type = LIBEWF_SEGMENT_FILE_TYPE_DWF;
	}
	else
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_INPUT,
		 LIBERROR_INPUT_ERROR_SIGNATURE_MISMATCH,
		 "%s: file signature does not match.",
		 function );

		return( -1 );
	}
	endian_little_convert_16bit(
	 *segment_number,
	 file_header.fields_segment );

	return( read_count );
}

/* Reads all sections from a segment file into the section list specific
 * for the segment file in the segment table in the handle
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libewf_segment_file_read_sections(
     libewf_segment_file_handle_t *segment_file_handle,
     libewf_io_handle_t *io_handle,
     int *last_segment_file,
     libewf_header_sections_t *header_sections,
     libewf_hash_sections_t *hash_sections,
     libewf_media_values_t *media_values,
     libewf_offset_table_t *offset_table,
     libewf_sector_table_t *sessions,
     libewf_sector_table_t *acquiry_errors,
     liberror_error_t **error )
{
	ewf_section_t section;

	static char *function   = "libewf_segment_file_read_sections";
	off64_t previous_offset = 0;
	int result              = 0;

	if( segment_file_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file handle.",
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
	if( last_segment_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid last segment file.",
		 function );

		return( -1 );
	}
	*last_segment_file = 0;

	/* The first offset is directly after the file header (13 byte)
	 */
	previous_offset = (off64_t) sizeof( ewf_file_header_t );

	while( result != -1 )
	{
		result = libewf_section_read(
		          io_handle->file_io_pool,
		          segment_file_handle,
		          header_sections,
		          hash_sections,
		          media_values,
		          offset_table,
		          sessions,
		          acquiry_errors,
		          &( io_handle->compression_level ),
		          &( io_handle->format ),
		          &( io_handle->ewf_format ),
		          &section,
		          &previous_offset,
		          error );

		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read section.",
			 function );

			return( -1 );
		}
		/* The next and done sections point back at themselves
		 */
		if( memory_compare(
		     section.type,
		     "next",
		     5 ) == 0 )
		{
			return( 1 );
		}
		else if( memory_compare(
		          section.type,
		          "done",
		          5 ) == 0 )
		{
			*last_segment_file = 1;

			return( 1 );
		}
	}
	return( 0 );
}

/* Write the headers to file
 * Returns the amount of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_headers(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_io_handle_t *io_handle,
         libewf_header_sections_t *header_sections,
         liberror_error_t **error )
{
	static char *function     = "libewf_segment_file_write_headers";
	ssize_t write_count       = 0;
	ssize_t total_write_count = 0;
	size_t header_size        = 0;
	size_t header2_size       = 0;

	if( segment_file_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file handle.",
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
	/* The header size contains the end of string character
	 */
	header_size = header_sections->header_size - 1;

	if( ( io_handle->format == LIBEWF_FORMAT_EWF )
	 || ( io_handle->format == LIBEWF_FORMAT_SMART )
	 || ( io_handle->format == LIBEWF_FORMAT_ENCASE1 ) )
	{
		/* The header should be written only once
		 * and using the compression used in the file
		 */
		write_count = libewf_section_header_write(
		               io_handle->file_io_pool,
		               segment_file_handle,
		               header_sections->header,
		               header_size,
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

			return( -1 );
		}
		total_write_count += write_count;

		header_sections->amount_of_header_sections += 1;
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
		write_count = libewf_section_header_write(
		               io_handle->file_io_pool,
		               segment_file_handle,
		               header_sections->header,
		               header_size,
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

			return( -1 );
		}
		total_write_count += write_count;

		write_count = libewf_section_header_write(
		               io_handle->file_io_pool,
		               segment_file_handle,
		               header_sections->header,
		               header_size,
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

			return( -1 );
		}
		total_write_count += write_count;

		header_sections->amount_of_header_sections += 2;
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

			return( -1 );
		}
		/* The header2 size contains two end of string characters
		 */
		header2_size = header_sections->header2_size - 2;

		/* The header2 should be written twice
		 * the default compression is used
		 */
		write_count = libewf_section_header2_write(
		               io_handle->file_io_pool,
		               segment_file_handle,
		               header_sections->header2,
		               header2_size,
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

			return( -1 );
		}
		total_write_count += write_count;

		write_count = libewf_section_header2_write(
		               io_handle->file_io_pool,
		               segment_file_handle,
		               header_sections->header2,
		               header2_size,
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

			return( -1 );
		}
		total_write_count += write_count;

		/* The header should be written once
		 * the default compression is used
		 */
		write_count = libewf_section_header_write(
		               io_handle->file_io_pool,
		               segment_file_handle,
		               header_sections->header,
		               header_size,
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

			return( -1 );
		}
		total_write_count += write_count;

		header_sections->amount_of_header_sections += 3;
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

			return( -1 );
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

			return( -1 );
		}
		/* The header2 size contains two end of string characters
		 */
		header2_size = header_sections->header2_size - 2;

		/* The xheader should be written once
		 * the default compression is used
		 */
		write_count = libewf_section_xheader_write(
		               io_handle->file_io_pool,
		               segment_file_handle,
		               header_sections->xheader,
		               header_sections->xheader_size,
		               EWF_COMPRESSION_DEFAULT,
		               error );

		if( write_count == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write firts xheader section.",
			 function );

			return( -1 );
		}
		total_write_count += write_count;

		/* The header2 should be written once
		 * the default compression is used
		 */
		write_count = libewf_section_header2_write(
		               io_handle->file_io_pool,
		               segment_file_handle,
		               header_sections->header2,
		               header2_size,
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

			return( -1 );
		}
		total_write_count += write_count;

		/* The header should be written once
		 * the default compression is used
		 */
		write_count = libewf_section_header_write(
		               io_handle->file_io_pool,
		               segment_file_handle,
		               header_sections->header,
		               header_size,
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

			return( -1 );
		}
		total_write_count += write_count;

		header_sections->amount_of_header_sections += 3;
	}
	return( total_write_count );
}

/* Write the last section at the end of the segment file
 * Returns the amount of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_last_section(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_io_handle_t *io_handle,
         int last_segment_file,
         liberror_error_t **error )
{
	uint8_t *last_section_type = NULL;
	static char *function      = "libewf_segment_file_write_last_section";
	ssize_t write_count        = 0;

	if( segment_file_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file handle.",
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
	write_count = libewf_section_last_write(
		       io_handle->file_io_pool,
		       segment_file_handle,
		       last_section_type,
		       4,
		       io_handle->format,
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

		return( -1 );
	}
	return( write_count );
}

/* Write the necessary sections at the start of the segment file
 * Returns the amount of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_start(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_io_handle_t *io_handle,
         uint16_t segment_number,
         uint8_t segment_file_type,
         libewf_media_values_t *media_values,
         libewf_header_sections_t *header_sections,
         ewf_data_t **cached_data_section,
         liberror_error_t **error )
{
	ewf_file_header_t file_header;

	static char *function         = "libewf_segment_file_write_start";
	const uint8_t *file_signature = NULL;
	ssize_t total_write_count     = 0;
	ssize_t write_count           = 0;

	if( segment_file_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file handle.",
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
	if( segment_number == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid segment number.",
		 function );

		return( -1 );
	}
	if( segment_file_handle->section_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section list.",
		 function );

		return( -1 );
	}
	if( cached_data_section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid cached data section.",
		 function );

		return( -1 );
	}
	/* Determine the segment file signature
	 */
	if( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF )
	{
		file_signature = evf_file_signature;
	}
	else if( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_LWF )
	{
		file_signature = lvf_file_signature;
	}
	else if( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_DWF )
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
	segment_file_handle->file_type = segment_file_type;

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
	endian_little_revert_16bit(
	 file_header.fields_segment,
	 segment_number );

	file_header.fields_start    = 1;
	file_header.fields_end[ 0 ] = 0;
	file_header.fields_end[ 1 ] = 0;

	/* Write segment file header
	 */
	write_count = libbfio_pool_write(
	               io_handle->file_io_pool,
	               segment_file_handle->file_io_pool_entry,
	               (uint8_t *) &file_header,
	               sizeof( ewf_file_header_t ),
	               error );

	if( write_count != (ssize_t) sizeof( ewf_file_header_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write file header to file.",
		 function );

		return( -1 );
	}
	total_write_count += write_count;

	if( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF )
	{
		if( segment_number == 1 )
		{
			/* Write header section(s)
			 * The segment file offset is updated by the function
			 */
			write_count = libewf_segment_file_write_headers(
				       segment_file_handle,
				       io_handle,
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

			if( io_handle->ewf_format == EWF_FORMAT_S01 )
			{
				/* Write volume (SMART) section
				 */
				write_count = libewf_section_volume_s01_write(
					       io_handle->file_io_pool,
					       segment_file_handle,
					       media_values,
					       io_handle->format,
					       0,
				               error );
			}
			else if( io_handle->ewf_format == EWF_FORMAT_E01 )
			{
				/* Write volume section
				 */
				write_count = libewf_section_volume_e01_write(
					       io_handle->file_io_pool,
					       segment_file_handle,
					       media_values,
					       io_handle->compression_level,
					       io_handle->format,
					       0,
				               error );
			}
			else
			{
				/* Fail safe
				 */
				write_count = -1;
			}
			if( write_count == -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write volume section.",
				 function );

				return( -1 );
			}
			total_write_count += write_count;
		}
		else if( io_handle->ewf_format == EWF_FORMAT_E01 )
		{
			/* Write data section
			 */
			write_count = libewf_section_data_write(
				       io_handle->file_io_pool,
				       segment_file_handle,
				       media_values,
				       io_handle->compression_level,
				       io_handle->format,
				       cached_data_section,
				       0,
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
			total_write_count += write_count;
		}
	}
	return( total_write_count );
}

/* Write the necessary sections before the actual data chunks to file
 * Returns the amount of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_chunks_section_start(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_io_handle_t *io_handle,
         libewf_offset_table_t *offset_table,
         ewf_table_offset_t *table_offsets,
         uint32_t amount_of_table_offsets,
         size32_t chunk_size,
         uint32_t total_chunk_amount,
         uint32_t segment_chunk_amount,
         liberror_error_t **error )
{
	static char *function = "libewf_segment_file_write_chunks_section_start";
	ssize_t write_count   = 0;
	size_t section_size   = 0;

	if( segment_file_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file handle.",
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
	if( offset_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset table.",
		 function );

		return( -1 );
	}
	if( table_offsets == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table offsets.",
		 function );

		return( -1 );
	}
	if( amount_of_table_offsets < segment_chunk_amount )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: table offsets too small.",
		 function );

		return( -1 );
	}
	/* The segment_chunk_amount contains the estimated amount of chunks for this section
	 */
	if( ( offset_table->amount_of_chunk_offsets < ( total_chunk_amount + segment_chunk_amount ) )
	 && ( libewf_offset_table_resize(
	       offset_table,
	       total_chunk_amount + segment_chunk_amount,
	       error ) != 1 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize offset table.",
		 function );

		return( -1 );
	}
	if( ( io_handle->ewf_format == EWF_FORMAT_S01 )
	 || ( io_handle->format == LIBEWF_FORMAT_ENCASE1 ) )
	{
		/* Write table section start
		 */
		write_count = libewf_section_table_write(
		               io_handle->file_io_pool,
		               segment_file_handle,
		               0,
		               table_offsets,
		               segment_chunk_amount,
		               (uint8_t *) "table",
		               5,
		               0,
		               io_handle->format,
		               io_handle->ewf_format,
		               1,
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
		section_size = segment_chunk_amount
		             * ( chunk_size + sizeof( ewf_crc_t ) );

		/* Write sectors section start
		 */
		write_count = libewf_section_sectors_write(
		               io_handle->file_io_pool,
		               segment_file_handle,
		               section_size,
		               1,
		               error );

		if( write_count == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write sectors section.",
			 function );

			return( -1 );
		}
	}
	return( write_count );
}

/* Correct the sections before the actual data chunks
 * Also write the necessary sections after the actual data chunks to file (like table and table2 sections for EWF-E01 format)
 * Returns the amount of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_chunks_section_correction(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_io_handle_t *io_handle,
         libewf_offset_table_t *offset_table,
         ewf_table_offset_t *table_offsets,
         uint32_t amount_of_table_offsets,
         off64_t chunks_section_offset,
         size64_t chunks_section_size,
         uint32_t amount_of_chunks,
         uint32_t section_amount_of_chunks,
         liberror_error_t **error )
{
	uint8_t *table_section_string    = NULL;
	static char *function            = "libewf_segment_file_write_chunks_section_correction";
	off64_t last_segment_file_offset = 0;
	off64_t base_offset              = 0;
	ssize_t total_write_count        = 0;
	ssize_t write_count              = 0;

	if( segment_file_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file handle.",
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
	if( ( ( io_handle->format == LIBEWF_FORMAT_ENCASE6 )
	  && ( chunks_section_size >= (size64_t) INT64_MAX ) )
	 || ( chunks_section_size >= (size64_t) INT32_MAX ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid chunk section size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( io_handle->format == LIBEWF_FORMAT_ENCASE6 )
	 || ( io_handle->format == LIBEWF_FORMAT_LINEN6 ) )
	{
		base_offset = chunks_section_offset;
	}
	if( libewf_offset_table_fill_offsets(
	     offset_table,
	     amount_of_chunks - section_amount_of_chunks,
	     section_amount_of_chunks,
	     base_offset,
	     table_offsets,
	     amount_of_table_offsets,
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
	if( libbfio_pool_get_offset(
	     io_handle->file_io_pool,
	     segment_file_handle->file_io_pool_entry,
	     &last_segment_file_offset,
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
	/* Seek the start of the data chunks
	*/
#if defined( HAVE_VERBOSE_OUTPUT )
	libewf_notify_verbose_printf(
	 "%s: setting file descriptor to start of chunks section offset: %" PRIu32 ".\n",
	 function,
	 chunks_section_offset );
#endif

	if( libbfio_pool_seek_offset(
	     io_handle->file_io_pool,
	     segment_file_handle->file_io_pool_entry,
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
	if( ( io_handle->ewf_format == EWF_FORMAT_S01 )
	 || ( io_handle->format == LIBEWF_FORMAT_ENCASE1 ) )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		libewf_notify_verbose_printf(
		 "%s: correcting table section size: %" PRIu64 " offset: %" PRIjd ".\n",
		 function,
		 chunks_section_size,
		 chunks_section_offset );
#endif

		/* Rewrite table section start
		 */
		write_count = libewf_section_table_write(
		               io_handle->file_io_pool,
		               segment_file_handle,
		               0,
		               table_offsets,
		               section_amount_of_chunks,
		               (uint8_t *) "table",
		               5,
		               (size_t) chunks_section_size,
		               io_handle->format,
		               io_handle->ewf_format,
		               0,
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
	}
	else if( io_handle->ewf_format == EWF_FORMAT_E01 )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		libewf_notify_verbose_printf(
		 "%s: correcting sectors section size: %" PRIzu " offset: %" PRIjd ".\n",
		 function,
		 chunks_section_size,
		 chunks_section_offset );
#endif

		/* Rewrite sectors section start
		 */
		write_count = libewf_section_sectors_write(
		               io_handle->file_io_pool,
		               segment_file_handle,
		               chunks_section_size,
		               0,
		               error );

		if( write_count == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to rewrite sectors section.",
			 function );

			return( -1 );
		}
	}
	/* Seek the end of the chunks section
	 */
#if defined( HAVE_VERBOSE_OUTPUT )
	libewf_notify_verbose_printf(
	 "%s: setting file descriptor back to end of data at offset: %" PRIu32 ".\n",
	 function,
	 last_segment_file_offset );
#endif

	if( libbfio_pool_seek_offset(
	     io_handle->file_io_pool,
	     segment_file_handle->file_io_pool_entry,
	     last_segment_file_offset,
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
		/* Write table section start
		 */
		write_count = libewf_section_table_write(
		               io_handle->file_io_pool,
		               segment_file_handle,
		               base_offset,
		               table_offsets,
		               section_amount_of_chunks,
		               (uint8_t *) "table",
		               5,
		               0,
		               io_handle->format,
		               io_handle->ewf_format,
		               0,
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
		total_write_count += write_count;

		/* Write table2 section start
		 */
		write_count = libewf_section_table_write(
		               io_handle->file_io_pool,
		               segment_file_handle,
		               base_offset,
		               table_offsets,
		               section_amount_of_chunks,
		               (uint8_t *) "table2",
		               6,
		               0,
		               io_handle->format,
		               io_handle->ewf_format,
		               0,
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
		total_write_count += write_count;
	}
	return( total_write_count );
}

/* Write a chunk of data to a segment file and update the offset table
 * Set write_crc to a non 0 value if the CRC is not provided within the chunk data
 * Returns the amount of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_chunk(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_io_handle_t *io_handle,
         libewf_offset_table_t *offset_table,
         uint32_t chunk,
         uint8_t *chunk_buffer,
         size_t chunk_size,
         int8_t is_compressed,
         uint8_t *crc_buffer,
         ewf_crc_t *chunk_crc,
         int8_t write_crc,
         liberror_error_t **error )
{
#if defined( HAVE_VERBOSE_OUTPUT )
	char *chunk_type            = NULL;
#endif
	static char *function       = "libewf_segment_file_write_chunk";
	off64_t segment_file_offset = 0;
	size_t write_size           = 0;
	ssize_t write_count         = 0;
	ssize_t total_write_count   = 0;

	if( segment_file_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file handle.",
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
	if( offset_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset table.",
		 function );

		return( -1 );
	}
	if( chunk_crc == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk CRC.",
		 function );

		return( -1 );
	}
	if( chunk_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid chunk size value exceeds maximum.",
		 function );

		return( -1 );
	}
	/* Make sure the chunk is available in the offset table
	 */
	if( ( offset_table->amount_of_chunk_offsets < ( chunk + 1 ) )
	 && ( libewf_offset_table_resize(
	       offset_table,
	       chunk + 1,
	       error ) != 1 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize offset table.",
		 function );

		return( -1 );
	}
	if( libbfio_pool_get_offset(
	     io_handle->file_io_pool,
	     segment_file_handle->file_io_pool_entry,
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
	/* Set the values in the offset table
	 */
	offset_table->chunk_offset[ chunk ].segment_file_handle = segment_file_handle;
	offset_table->chunk_offset[ chunk ].file_offset         = segment_file_offset;
	offset_table->chunk_offset[ chunk ].size                = chunk_size;

	if( write_crc != 0 )
	{
		offset_table->chunk_offset[ chunk ].size += sizeof( ewf_crc_t );
	}
	if( is_compressed == 0 )
	{
		offset_table->chunk_offset[ chunk ].flags = 0;
	}
	else
	{
		offset_table->chunk_offset[ chunk ].flags |= LIBEWF_CHUNK_OFFSET_FLAGS_COMPRESSED;
	}

#if defined( HAVE_VERBOSE_OUTPUT )
	/* Print a verbose notification
	 */
	if( is_compressed == 0 )
	{
		chunk_type = "uncompressed";
	}
	else
	{
		chunk_type = "compressed";
	}
	libewf_notify_verbose_printf(
	 "%s: writing %s chunk: %" PRIu32 " at offset: %" PRIjd " with size: %" PRIzu ", with CRC: %" PRIu32 ".\n",
	 function,
	 chunk_type,
	 chunk + 1,
	 segment_file_offset,
	 offset_table->chunk_offset[ chunk ].size,
	 *chunk_crc );
#endif

	write_size = chunk_size;

	/* Write the CRC if necessary
	 */
	if( write_crc != 0 )
	{
		if( crc_buffer == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid CRC buffer.",
			 function );

			return( -1 );
		}
		endian_little_revert_32bit(
		 crc_buffer,
		 *chunk_crc );

		/* Check if the chunk and crc buffers are aligned
		 * if so write the chunk and crc at the same time
		 */
		if( ( is_compressed == 0 )
		 && ( &( chunk_buffer[ chunk_size ] ) == crc_buffer ) )
		{
			write_size += sizeof( ewf_crc_t );
		}
	}
	/* Write the chunk data to the segment file
	 */
	write_count = libbfio_pool_write(
	               io_handle->file_io_pool,
	               segment_file_handle->file_io_pool_entry,
	               chunk_buffer,
	               write_size,
	               error );

	if( write_count != (ssize_t) write_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write data.",
		 function );

		return( -1 );
	}
	total_write_count += write_count;

	/* Check if the chunk and crc buffers are aligned
	 * if not the chunk and crc need to be written separately
	 */
	if( ( write_crc != 0 )
	 && ( &( chunk_buffer[ chunk_size ] ) != crc_buffer ) )
	{
		write_count = libbfio_pool_write(
			       io_handle->file_io_pool,
			       segment_file_handle->file_io_pool_entry,
			       crc_buffer,
			       sizeof( ewf_crc_t ),
			       error );

		if( write_count != (ssize_t) sizeof( ewf_crc_t ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write CRC.",
			 function );

			return( -1 );
		}
		total_write_count += write_count;
	}
	return( total_write_count );
}

/* Write a delta chunk of data to a segment file and update the offset table
 * Returns the amount of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_delta_chunk(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_io_handle_t *io_handle,
         libewf_offset_table_t *offset_table,
         uint32_t chunk,
         uint8_t *chunk_buffer,
         size_t chunk_size,
         uint8_t *crc_buffer,
         ewf_crc_t *chunk_crc,
         uint8_t write_crc,
	 uint8_t no_section_append,
         liberror_error_t **error )
{
	static char *function       = "libewf_segment_file_write_delta_chunk";
	off64_t segment_file_offset = 0;
	ssize_t write_count         = 0;

	if( segment_file_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file handle.",
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
	if( offset_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset table.",
		 function );

		return( -1 );
	}
	/* Make sure the chunk is available in the offset table
	 */
	if( chunk >= offset_table->amount_of_chunk_offsets )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: chunk: %" PRIu32 " out of range [0,%" PRIu32 "].",
		 function,
		 chunk,
		 offset_table->amount_of_chunk_offsets - 1 );

		return( -1 );
	}
	if( libbfio_pool_get_offset(
	     io_handle->file_io_pool,
	     segment_file_handle->file_io_pool_entry,
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
	segment_file_offset += sizeof( ewfx_delta_chunk_header_t ) + sizeof( ewf_section_t );

#if defined( HAVE_VERBOSE_OUTPUT )
	libewf_notify_verbose_printf(
	 "%s: writing uncompressed delta chunk: %" PRIu32 " at offset: %" PRIjd " with size: %" PRIzu ", with CRC: %" PRIu32 ".\n",
	 function,
	 ( chunk + 1 ),
	 segment_file_offset,
	 chunk_size,
	 *chunk_crc );
#endif

	/* Write the chunk in the delta segment file
	 */
	write_count = libewf_section_delta_chunk_write(
	               io_handle->file_io_pool,
	               segment_file_handle,
	               chunk,
	               chunk_buffer,
	               chunk_size,
	               crc_buffer,
	               chunk_crc,
	               write_crc,
	               no_section_append,
	               error );

	/* Set the values in the offset table
	 * if the delta chunk was added newly
	 *  and the delta chunk was written correctly
	 */
	if( ( no_section_append == 0 )
	 && ( write_count > 0 ) )
	{
		offset_table->chunk_offset[ chunk ].segment_file_handle = segment_file_handle;
		offset_table->chunk_offset[ chunk ].file_offset         = segment_file_offset;
		offset_table->chunk_offset[ chunk ].size                = chunk_size + sizeof( ewf_crc_t );
		offset_table->chunk_offset[ chunk ].flags               = LIBEWF_CHUNK_OFFSET_FLAGS_DELTA_CHUNK;
	}
	else if( write_count < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write delta chunk.",
		 function );

		return( -1 );
	}
	return( write_count );
}

/* Closes the segment file, necessary sections at the end of the segment file will be written
 * Returns the amount of bytes written or -1 on error
 */
ssize_t libewf_segment_file_write_close(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_io_handle_t *io_handle,
         uint16_t segment_number,
         uint32_t segment_amount_of_chunks,
         int last_segment_file,
         libewf_hash_sections_t *hash_sections,
         libewf_values_table_t *hash_values,
         libewf_media_values_t *media_values,
         libewf_sector_table_t *sessions,
         libewf_sector_table_t *acquiry_errors,
         ewf_data_t **cached_data_section,
	 liberror_error_t **error )
{
	static char *function     = "libewf_segment_file_write_close";
	ssize_t total_write_count = 0;
	ssize_t write_count       = 0;

	if( segment_file_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file handle.",
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
		if( ( io_handle->ewf_format == EWF_FORMAT_E01 )
		 && ( segment_number == 1 ) )
		{
			write_count = libewf_section_data_write(
				       io_handle->file_io_pool,
				       segment_file_handle,
				       media_values,
				       io_handle->compression_level,
				       io_handle->format,
				       cached_data_section,
				       0,
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
			total_write_count += write_count;
		}
		/* Write the session section if required
		 */
		if( ( sessions->amount > 0 )
		 && ( ( io_handle->format == LIBEWF_FORMAT_ENCASE5 )
		  || ( io_handle->format == LIBEWF_FORMAT_ENCASE6 )
		  || ( io_handle->format == LIBEWF_FORMAT_LINEN5 )
		  || ( io_handle->format == LIBEWF_FORMAT_LINEN6 )
		  || ( io_handle->format == LIBEWF_FORMAT_EWFX ) ) )
		{
			write_count = libewf_section_session_write(
			               io_handle->file_io_pool,
			               segment_file_handle,
			               sessions,
			               error );

			if( write_count == -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write sessions section.",
				 function );

				return( -1 );
			}
			total_write_count += write_count;
		}
		/* Write the error2 section if required
		 */
		if( ( acquiry_errors->amount > 0 )
		 && ( ( io_handle->format == LIBEWF_FORMAT_ENCASE3 )
		  || ( io_handle->format == LIBEWF_FORMAT_ENCASE4 )
		  || ( io_handle->format == LIBEWF_FORMAT_ENCASE5 )
		  || ( io_handle->format == LIBEWF_FORMAT_ENCASE6 )
		  || ( io_handle->format == LIBEWF_FORMAT_LINEN5 )
		  || ( io_handle->format == LIBEWF_FORMAT_LINEN6 )
		  || ( io_handle->format == LIBEWF_FORMAT_EWFX ) ) )
		{
			write_count = libewf_section_error2_write(
			               io_handle->file_io_pool,
			               segment_file_handle,
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

				return( -1 );
			}
			total_write_count += write_count;
		}
		if( io_handle->format == LIBEWF_FORMAT_ENCASE6 )
		{
			/* Write the digest section if required
			 */
			if( hash_sections->sha1_digest_set != 0 )
			{
				write_count = libewf_section_digest_write(
					       io_handle->file_io_pool,
					       segment_file_handle,
					       hash_sections->md5_digest,
					       hash_sections->sha1_digest,
					       error );

				if( write_count == -1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_IO,
					 LIBERROR_IO_ERROR_WRITE_FAILED,
					 "%s: unable to write digest section.",
					 function );

					return( -1 );
				}
				total_write_count += write_count;
			}
		}
		/* Write the hash section if required
		 */
		if( hash_sections->md5_hash_set != 0 )
		{
			write_count = libewf_section_hash_write(
			               io_handle->file_io_pool,
			               segment_file_handle,
			               hash_sections->md5_hash,
			               error );

			if( write_count == -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write hash section.",
				 function );

				return( -1 );
			}
			total_write_count += write_count;
		}
		/* Write the xhash section
		 */
		if( io_handle->format == LIBEWF_FORMAT_EWFX )
		{
			if( hash_sections->xhash != NULL )
			{
#if defined( HAVE_VERBOSE_OUTPUT )
				libewf_notify_verbose_printf(
				 "%s: xhash already set - cleaning previous defintion.\n",
				 function );
#endif

				memory_free(
				 hash_sections->xhash );

				hash_sections->xhash = NULL;
			}
			if( libewf_hash_values_generate_xhash_string_ewfx(
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

				return( -1 );
			}
			write_count = libewf_section_xhash_write(
			               io_handle->file_io_pool,
			               segment_file_handle,
			               hash_sections->xhash,
			               hash_sections->xhash_size,
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

				return( -1 );
			}
			total_write_count += write_count;
		}
	}
	/* Write the done or next section
	 * The segment file offset is updated by the function
	 */
	write_count = libewf_segment_file_write_last_section(
		       segment_file_handle,
		       io_handle,
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

		return( -1 );
	}
	total_write_count += write_count;

	segment_file_handle->amount_of_chunks = segment_amount_of_chunks;

	if( libbfio_pool_close(
	     io_handle->file_io_pool,
	     segment_file_handle->file_io_pool_entry,
	     error ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close segment file: %" PRIu16 ".",
		 function,
		 segment_number );

		return( -1 );
	}
	segment_file_handle->write_open = 0;

	return( total_write_count );
}

/* Corrects sections after streamed write
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_file_write_sections_correction(
     libewf_segment_file_handle_t *segment_file_handle,
     libewf_io_handle_t *io_handle,
     uint16_t segment_number,
     uint32_t segment_amount_of_chunks,
     int last_segment_file,
     libewf_media_values_t *media_values,
     libewf_values_table_t *hash_values,
     libewf_hash_sections_t *hash_sections,
     libewf_sector_table_t *sessions,
     libewf_sector_table_t *acquiry_errors,
     ewf_data_t **cached_data_section,
     liberror_error_t **error )
{
	libewf_list_element_t *section_list_element       = NULL;
	libewf_section_list_values_t *section_list_values = NULL;
	static char *function                             = "libewf_segment_file_write_sections_correction";
	off64_t next_section_start_offset                 = 0;
	ssize_t write_count                               = 0;
	int correct_last_next_section                     = 0;

	if( segment_file_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file handle.",
		 function );

		return( -1 );
	}
	if( segment_file_handle->section_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment file: %" PRIu16 " - missing section list.",
		 function,
		 segment_number );

		return( -1 );
	}
	if( segment_file_handle->section_list->first == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment file: %" PRIu16 " - invalid section list - missing entries.",
		 function,
		 segment_number );

		return( -1 );
	}
	section_list_element = segment_file_handle->section_list->first;

	if( segment_file_handle->file_io_pool_entry == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file io pool entry for segment file: %" PRIu16 ".",
		 function,
		 segment_number );

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
	if( libbfio_pool_open(
	     io_handle->file_io_pool,
	     segment_file_handle->file_io_pool_entry,
	     LIBBFIO_OPEN_READ_WRITE,
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
#if defined( HAVE_VERBOSE_OUTPUT )
	libewf_notify_verbose_printf(
	 "%s: correcting sections in segment file: %" PRIu16 ".\n",
	 function,
	 segment_number );
#endif

	while( section_list_element != NULL )
	{
		section_list_values = (libewf_section_list_values_t *) section_list_element->value;

		if( section_list_values == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing section list values for segment file: %" PRIu16 ".",
			 function,
			 segment_number );

			return( -1 );
		}
		if( memory_compare(
		     section_list_values->type,
		     "volume",
		     6 ) == 0 )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			libewf_notify_verbose_printf(
			 "%s: correcting volume section.\n",
			 function );
#endif

			if( libbfio_pool_seek_offset(
			     io_handle->file_io_pool,
			     segment_file_handle->file_io_pool_entry,
			     section_list_values->start_offset,
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
			if( io_handle->ewf_format == EWF_FORMAT_S01 )
			{
				/* Write volume (SMART) section
				 */
				write_count = libewf_section_volume_s01_write(
					       io_handle->file_io_pool,
					       segment_file_handle,
					       media_values,
					       io_handle->format,
					       1,
					       error );
			}
			else if( io_handle->ewf_format == EWF_FORMAT_E01 )
			{
				/* Write volume section
				 */
				write_count = libewf_section_volume_e01_write(
					       io_handle->file_io_pool,
					       segment_file_handle,
					       media_values,
					       io_handle->compression_level,
					       io_handle->format,
					       1,
					       error );
			}
			else
			{
				write_count = -1;
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
		else if( memory_compare(
			  section_list_values->type,
			  "data",
			  4 ) == 0 )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			libewf_notify_verbose_printf(
			 "%s: correcting data section.\n",
			 function );
#endif

			if( libbfio_pool_seek_offset(
			     io_handle->file_io_pool,
			     segment_file_handle->file_io_pool_entry,
			     section_list_values->start_offset,
			     SEEK_SET,
			     error ) == -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_SEEK_FAILED,
				 "%s: unable to find offset to data volume section.",
				 function );

				return( -1 );
			}
			/* Write data section
			 */
			write_count = libewf_section_data_write(
				       io_handle->file_io_pool,
				       segment_file_handle,
				       media_values,
				       io_handle->compression_level,
				       io_handle->format,
				       cached_data_section,
				       1,
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
		else if( ( last_segment_file != 0 )
		      && ( memory_compare(
			    section_list_values->type,
			    "next",
			    4 ) == 0 ) )

		{
			correct_last_next_section = 1;
			next_section_start_offset = section_list_values->start_offset;
		}
		section_list_element = section_list_element->next;
	}
	if( correct_last_next_section != 0 )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		libewf_notify_verbose_printf(
		 "%s: correcting last next section.\n",
		 function );
#endif

		if( libbfio_pool_seek_offset(
		     io_handle->file_io_pool,
		     segment_file_handle->file_io_pool_entry,
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
			       segment_file_handle,
			       io_handle,
			       segment_number,
			       segment_amount_of_chunks,
			       1,
			       hash_sections,
			       hash_values,
			       media_values,
			       sessions,
			       acquiry_errors,
			       cached_data_section,
			       error );

		if( write_count == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to close last segment file: %" PRIu16 ".",
			 function,
			 segment_number );

			return( -1 );
		}
	}
	else
	{
		if( libbfio_pool_close(
		     io_handle->file_io_pool,
		     segment_file_handle->file_io_pool_entry,
		     error ) != 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close segment file: %" PRIu16 ".",
			 function,
			 segment_number );

			return( -1 );
		}
	}
	return( 1 );
}

