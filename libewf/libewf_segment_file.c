/*
 * libewf segment file
 *
 * Copyright (c) 2006-2007, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of the creator, related organisations, nor the names of
 *   its contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER, COMPANY AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "libewf_includes.h"

#include <errno.h>

#include <libewf/libewf_definitions.h>

#include "libewf_common.h"
#include "libewf_endian.h"
#include "libewf_hash_values.h"
#include "libewf_notify.h"
#include "libewf_section.h"
#include "libewf_segment_file.h"
#include "libewf_segment_table.h"
#include "libewf_string.h"

#include "ewf_definitions.h"
#include "ewf_file_header.h"

const uint8_t dvf_file_signature[] = { 0x64, 0x76, 0x66, 0x09, 0x0D, 0x0A, 0xFF, 0x00 };
const uint8_t evf_file_signature[] = { 0x45, 0x56, 0x46, 0x09, 0x0D, 0x0A, 0xFF, 0x00 };
const uint8_t lvf_file_signature[] = { 0x4c, 0x56, 0x46, 0x09, 0x0D, 0x0A, 0xFF, 0x00 };

/* Detects if a file is an EWF file (check for the EWF file signature)
 * Returns 1 if true, 0 if not, or -1 on error
 */
int libewf_segment_file_check_file_signature( int file_descriptor )
{
	uint8_t signature[ 8 ];

	static char *function = "libewf_segment_file_check_file_signature";
	ssize_t read_count    = 0;

	if( file_descriptor == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid file descriptor.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_common_read( file_descriptor, signature, 8 );

	if( read_count <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: error reading signature from file.\n",
		 function );

		return( -1 );
	}
	else if( read_count != 8 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read signature from file.\n",
		 function );

		return( -1 );
	}
	/* The amount of EWF segment files will be the largest
	 */
	if( libewf_common_memcmp( evf_file_signature, signature, sizeof( evf_file_signature ) ) == 0 )
	{
		return( 1 );
	}
	else if( libewf_common_memcmp( lvf_file_signature, signature, sizeof( lvf_file_signature ) ) == 0 )
	{
		return( 1 );
	}
	else if( libewf_common_memcmp( dvf_file_signature, signature, sizeof( dvf_file_signature ) ) == 0 )
	{
		return( 1 );
	}
	return( 0 );
}

/* Reads the file header from a segment file
 * Returns the amount of bytes read if successful, or -1 on errror
 */
ssize_t libewf_segment_file_read_file_header( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, uint16_t *segment_number, uint8_t *segment_file_type )
{
	EWF_FILE_HEADER file_header;

	static char *function = "libewf_segment_file_read_file_header";
	ssize_t read_count    = 0;

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file handle.\n",
		 function );

		return( -1 );
	}
	if( segment_number == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment number.\n",
		 function );

		return( -1 );
	}
	if( segment_file_type == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file type.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_segment_file_handle_read(
	              segment_file_handle,
	              &file_header,
	              EWF_FILE_HEADER_SIZE );

	if( read_count != (ssize_t) EWF_FILE_HEADER_SIZE )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read file header.\n",
		 function );

		return( -1 );
	}
	/* The amount of EWF segment files will be the largest
	 */
	if( libewf_common_memcmp(
	     evf_file_signature,
	     file_header.signature,
	     sizeof( evf_file_signature ) ) == 0 )
	{
		*segment_file_type = LIBEWF_SEGMENT_FILE_TYPE_EWF;
	}
	else if( libewf_common_memcmp(
	          lvf_file_signature,
	          file_header.signature,
	          sizeof( lvf_file_signature ) ) == 0 )
	{
		*segment_file_type = LIBEWF_SEGMENT_FILE_TYPE_LWF;
	}
	else if( libewf_common_memcmp(
	          dvf_file_signature,
	          file_header.signature,
	          sizeof( dvf_file_signature ) ) == 0 )
	{
		*segment_file_type = LIBEWF_SEGMENT_FILE_TYPE_DWF;
	}
	else
	{
		LIBEWF_WARNING_PRINT( "%s: file signature does not match.\n",
		 function );

		return( -1 );
	}
	if( libewf_endian_convert_16bit(
	     segment_number,
	     file_header.fields_segment ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert fields segment number value.\n",
		 function );

		return( -1 );
	}
	return( read_count );
}

/* Reads all sections from a segment file into the section list specific
 * for the segment file in the segment table in the handle
 * Returns 1 if successful, 0 if not, or -1 on error
 */
int libewf_segment_file_read_sections( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, int *last_segment_file, LIBEWF_HEADER_SECTIONS *header_sections, LIBEWF_HASH_SECTIONS *hash_sections, LIBEWF_MEDIA_VALUES *media_values, LIBEWF_OFFSET_TABLE *offset_table, LIBEWF_OFFSET_TABLE *secondary_offset_table, LIBEWF_SECTOR_TABLE *acquiry_errors, int8_t *compression_level, uint8_t *format, uint8_t *ewf_format, size64_t *segment_file_size, uint8_t error_tollerance  )
{
	EWF_SECTION section;

	static char *function   = "libewf_segment_file_read_sections";
	off64_t previous_offset = 0;
	int result              = 0;

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file handle.\n",
		 function );

		return( -1 );
	}
	if( last_segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid last segment file.\n",
		 function );

		return( -1 );
	}
	*last_segment_file = 0;

	/* The first offset is directly after the file header (13 byte)
	 */
	previous_offset = (off64_t) EWF_FILE_HEADER_SIZE;

	while( result != -1 )
	{
		result = libewf_section_read(
		          segment_file_handle,
		          header_sections,
		          hash_sections,
		          media_values,
		          offset_table,
		          secondary_offset_table,
		          acquiry_errors,
		          compression_level,
		          format,
		          ewf_format,
		          segment_file_size,
		          &section,
		          &previous_offset,
		          error_tollerance );

		if( result != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to read section.\n",
			 function );

			return( -1 );
		}
		/* The next and done sections point back at themselves
		 */
		if( ewf_string_compare( section.type, "next", 5 ) == 0 )
		{
			return( 1 );
		}
		else if( ewf_string_compare( section.type, "done", 5 ) == 0 )
		{
			*last_segment_file = 1;

			return( 1 );
		}
	}
	return( 0 );
}

/* Write the headers to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_segment_file_write_headers( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, LIBEWF_HEADER_SECTIONS *header_sections, int8_t compression_level, uint8_t format )
{
	static char *function     = "libewf_segment_file_write_headers";
	ssize_t write_count       = 0;
	ssize_t total_write_count = 0;
	size_t header_size        = 0;
	size_t header2_size       = 0;

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file handle.\n",
		 function );

		return( -1 );
	}
	if( header_sections == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header sections.\n",
		 function );

		return( -1 );
	}
	if( ( header_sections->header == NULL )
	 || ( header_sections->header_size == 0 ) )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header.\n",
		 function );

		return( -1 );
	}
	/* The header size contains the end of string character
	 */
	header_size = header_sections->header_size - 1;

	if( ( format == LIBEWF_FORMAT_EWF )
	 || ( format == LIBEWF_FORMAT_SMART )
	 || ( format == LIBEWF_FORMAT_ENCASE1 ) )
	{
		/* The header should be written only once
		 * and using the compression used in the file
		 */
		write_count = libewf_section_header_write(
		               segment_file_handle,
		               header_sections->header,
		               header_size,
		               compression_level );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to write single header section.\n",
			 function );

			return( -1 );
		}
		total_write_count += write_count;

		header_sections->amount_of_header_sections += 1;
	}
	else if( ( format == LIBEWF_FORMAT_ENCASE2 )
	 || ( format == LIBEWF_FORMAT_ENCASE3 )
	 || ( format == LIBEWF_FORMAT_LINEN5 )
	 || ( format == LIBEWF_FORMAT_LINEN6 )
	 || ( format == LIBEWF_FORMAT_FTK ) )
	{
		/* The header should be written twice
		 * the default compression is used
		 */
		write_count = libewf_section_header_write(
		               segment_file_handle,
		               header_sections->header,
		               header_size,
		               EWF_COMPRESSION_DEFAULT );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to write first header section.\n",
			 function );

			return( -1 );
		}
		total_write_count += write_count;

		write_count = libewf_section_header_write(
		               segment_file_handle,
		               header_sections->header,
		               header_size,
		               EWF_COMPRESSION_DEFAULT );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to write second header section.\n",
			 function );

			return( -1 );
		}
		total_write_count += write_count;

		header_sections->amount_of_header_sections += 2;
	}
	else if( ( format == LIBEWF_FORMAT_ENCASE4 )
	 || ( format == LIBEWF_FORMAT_ENCASE5 )
	 || ( format == LIBEWF_FORMAT_ENCASE6 ) )
	{
		if( ( header_sections->header2 == NULL )
		 && ( header_sections->header2_size == 0 ) )
		{
			LIBEWF_WARNING_PRINT( "%s: invalid header2.\n",
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
		               segment_file_handle,
		               header_sections->header2,
		               header2_size,
		               EWF_COMPRESSION_DEFAULT );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to write first header2 section.\n",
			 function );

			return( -1 );
		}
		total_write_count += write_count;

		write_count = libewf_section_header2_write(
		               segment_file_handle,
		               header_sections->header2,
		               header2_size,
		               EWF_COMPRESSION_DEFAULT );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to write second header2 section.\n",
			 function );

			return( -1 );
		}
		total_write_count += write_count;

		/* The header should be written once
		 * the default compression is used
		 */
		write_count = libewf_section_header_write(
		               segment_file_handle,
		               header_sections->header,
		               header_size,
		               EWF_COMPRESSION_DEFAULT );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to write third header section.\n",
			 function );

			return( -1 );
		}
		total_write_count += write_count;

		header_sections->amount_of_header_sections += 3;
	}
	/* EWFX uses the header and header2 for backwards compatibility
	 */
	else if( format == LIBEWF_FORMAT_EWFX )
	{
		if( ( header_sections->xheader == NULL )
		 && ( header_sections->xheader_size == 0 ) )
		{
			LIBEWF_WARNING_PRINT( "%s: invalid xheader.\n",
			 function );

			return( -1 );
		}
		if( ( header_sections->header2 == NULL )
		 && ( header_sections->header2_size == 0 ) )
		{
			LIBEWF_WARNING_PRINT( "%s: invalid header2.\n",
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
		               segment_file_handle,
		               header_sections->xheader,
		               header_sections->xheader_size,
		               EWF_COMPRESSION_DEFAULT );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to write firts xheader section.\n",
			 function );

			return( -1 );
		}
		total_write_count += write_count;

		/* The header2 should be written once
		 * the default compression is used
		 */
		write_count = libewf_section_header2_write(
		               segment_file_handle,
		               header_sections->header2,
		               header2_size,
		               EWF_COMPRESSION_DEFAULT );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to write second header2 section.\n",
			 function );

			return( -1 );
		}
		total_write_count += write_count;

		/* The header should be written once
		 * the default compression is used
		 */
		write_count = libewf_section_header_write(
		               segment_file_handle,
		               header_sections->header,
		               header_size,
		               EWF_COMPRESSION_DEFAULT );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to write third header section.\n",
			 function );

			return( -1 );
		}
		total_write_count += write_count;

		header_sections->amount_of_header_sections += 3;
	}
	return( total_write_count );
}

/* Write the last section at the end of the segment file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_segment_file_write_last_section( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, int last_segment_file, uint8_t format, uint8_t ewf_format )
{
	EWF_CHAR *last_section_type = NULL;
	static char *function       = "libewf_segment_file_write_last_section";
	ssize_t write_count         = 0;

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file handle.\n",
		 function );

		return( -1 );
	}
	if( last_segment_file == 0 )
	{
		last_section_type = (EWF_CHAR *) "next";
	}
	else
	{
		last_section_type = (EWF_CHAR *) "done";
	}
	/* Write next or done section
	 */
	write_count = libewf_section_last_write(
		       segment_file_handle,
		       last_section_type,
		       4,
		       format,
		       ewf_format );

	if( write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write %s section.\n",
		 function, (char *) last_section_type );

		return( -1 );
	}
	return( write_count );
}

/* Write the necessary sections at the start of the segment file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_segment_file_write_start( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, uint16_t segment_number, uint8_t segment_file_type, LIBEWF_MEDIA_VALUES *media_values, LIBEWF_HEADER_SECTIONS *header_sections, int8_t compression_level, uint8_t format, uint8_t ewf_format, EWF_DATA **cached_data_section )
{
	EWF_FILE_HEADER file_header;

	static char *function         = "libewf_segment_file_write_start";
	const uint8_t *file_signature = NULL;
	ssize_t total_write_count     = 0;
	ssize_t write_count           = 0;

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file handle.\n",
		 function );

		return( -1 );
	}
	if( segment_number == 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment number.\n",
		 function );

		return( -1 );
	}
	if( segment_file_handle->file_descriptor == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid file descriptor.\n",
		 function );

		return( -1 );
	}
	if( segment_file_handle->section_list == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid section list.\n",
		 function );

		return( -1 );
	}
	if( cached_data_section == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid cached data section.\n",
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
		LIBEWF_WARNING_PRINT( "%s: unsupported segment file type.\n",
		 function );

		return( -1 );
	}
	if( libewf_common_memcpy(
	     file_header.signature,
	     file_signature,
	     8 ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set file signature.\n",
		 function );

		return( -1 );
	}
	if( libewf_endian_revert_16bit(
	     segment_number,
	     file_header.fields_segment ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert segment number.\n",
		 function );

		return( -1 );
	}
	file_header.fields_start    = 1;
	file_header.fields_end[ 0 ] = 0;
	file_header.fields_end[ 1 ] = 0;

	/* Write segment file header
	 */
	write_count = libewf_segment_file_handle_write(
	               segment_file_handle,
	               &file_header,
	               EWF_FILE_HEADER_SIZE );

	if( write_count != (ssize_t) EWF_FILE_HEADER_SIZE )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write file header to file.\n",
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
				       header_sections,
				       compression_level,
				       format );

			if( write_count == -1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to write header sections.\n",
				 function );

				return( -1 );
			}
			total_write_count += write_count;

			if( ewf_format == EWF_FORMAT_S01 )
			{
				/* Write volume (SMART) section
				 */
				write_count = libewf_section_volume_s01_write(
					       segment_file_handle,
					       media_values,
					       format,
					       0 );
			}
			else if( ewf_format == EWF_FORMAT_E01 )
			{
				/* Write volume section
				 */
				write_count = libewf_section_volume_e01_write(
					       segment_file_handle,
					       media_values,
					       compression_level,
					       format,
					       0 );
			}
			else
			{
				/* Fail safe
				 */
				write_count = -1;
			}
			if( write_count == -1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to write volume section.\n",
				 function );

				return( -1 );
			}
			total_write_count += write_count;
		}
		else if( ewf_format == EWF_FORMAT_E01 )
		{
			/* Write data section
			 */
			write_count = libewf_section_data_write(
				       segment_file_handle,
				       media_values,
				       compression_level,
				       format,
				       cached_data_section,
				       0 );

			if( write_count == -1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to write data section.\n",
				 function );

				return( -1 );
			}
			total_write_count += write_count;
		}
	}
	return( total_write_count );
}

/* Write the necessary sections before the actual data chunks to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_segment_file_write_chunks_section_start( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, LIBEWF_OFFSET_TABLE *offset_table, size32_t chunk_size, uint32_t total_chunk_amount, uint32_t segment_chunk_amount, uint8_t format, uint8_t ewf_format )
{
	static char *function = "libewf_segment_file_write_chunks_section_start";
	ssize_t write_count   = 0;
	size_t section_size   = 0;

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file handle.\n",
		 function );

		return( -1 );
	}
	if( offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid offset table.\n",
		 function );

		return( -1 );
	}

	/* The segment_chunk_amount contains the estimated amount of chunks for this section
	 */
	if( offset_table->amount < ( total_chunk_amount + segment_chunk_amount ) )
	{
		if( libewf_offset_table_realloc(
		     offset_table,
		     ( total_chunk_amount + segment_chunk_amount ) ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to reallocate offset table.\n",
			 function );

			return( -1 );
		}
	}
	if( ( ewf_format == EWF_FORMAT_S01 )
	 || ( format == LIBEWF_FORMAT_ENCASE1 ) )
	{
		/* Write table section start
		 */
		write_count = libewf_section_table_write(
		               segment_file_handle,
		               0,
		               offset_table,
		               total_chunk_amount,
		               segment_chunk_amount,
		               (EWF_CHAR *) "table",
		               5,
		               0,
		               format,
		               ewf_format,
		               1 );

		if( write_count == -1 )
		{
			LIBEWF_VERBOSE_PRINT( "%s: unable to write table section.\n",
			 function );

			return( -1 );
		}
	}
	else if( ewf_format == EWF_FORMAT_E01 )
	{
		section_size = segment_chunk_amount 
		             * ( chunk_size + EWF_CRC_SIZE );

		/* Write sectors section start
		 */
		write_count = libewf_section_sectors_write(
		               segment_file_handle,
		               section_size,
		               1 );

		if( write_count == -1 )
		{
			LIBEWF_VERBOSE_PRINT( "%s: unable to write sectors section.\n",
			 function );

			return( -1 );
		}
	}
	return( write_count );
}

/* Write a chunk of data to a segment file and update the offset table
 * Set write_crc to a non 0 value if the CRC is not provided within the chunk data
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_segment_file_write_chunks_data( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, LIBEWF_OFFSET_TABLE *offset_table, uint32_t chunk, EWF_CHAR *chunk_data, size_t size, int8_t is_compressed, EWF_CRC *chunk_crc, int8_t write_crc )
{
	uint8_t calculated_crc_buffer[ 4 ];

#if defined( HAVE_VERBOSE_OUTPUT )
	char *chunk_type          = NULL;
#endif
	static char *function     = "libewf_segment_file_write_chunks_data";
	ssize_t write_count       = 0;
	ssize_t total_write_count = 0;
	size_t chunk_size         = size;

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file handle.\n",
		 function );

		return( -1 );
	}
	if( offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid offset table.\n",
		 function );

		return( -1 );
	}
	if( chunk_data == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid chunk data.\n",
		 function );

		return( -1 );
	}
	if( chunk_crc == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid chunk CRC.\n",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( segment_file_handle->file_descriptor == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid file descriptor.\n",
		 function );

		return( -1 );
	}
	if( write_crc != 0 )
	{
		chunk_size += EWF_CRC_SIZE;
	}
	/* Make sure the chunk is available in the offset table
	 */
	if( offset_table->amount < ( chunk + 1 ) )
	{
		if( libewf_offset_table_realloc(
		     offset_table,
		     ( chunk + 1 ) ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to reallocate offset table.\n",
			 function );

			return( -1 );
		}
	}
	/* Set the values in the offset table
	 */
	offset_table->chunk_offset[ chunk ].segment_file_handle = segment_file_handle;
	offset_table->chunk_offset[ chunk ].file_offset         = segment_file_handle->file_offset;
	offset_table->chunk_offset[ chunk ].size                = chunk_size;
	offset_table->chunk_offset[ chunk ].compressed          = is_compressed;

#if defined( HAVE_VERBOSE_OUTPUT )
	/* Print a verbose notification
	 */
	if( is_compressed == 0 )
	{
		chunk_type = "UNCOMPRESSED";
	}
	else
	{
		chunk_type = "COMPRESSED";
	}
	LIBEWF_VERBOSE_PRINT( "%s: writing %s chunk: %" PRIu32 " at offset: %jd with size: %zu, with CRC: %" PRIu32 ".\n",
	 function, chunk_type, ( chunk + 1 ), segment_file_handle->file_offset, chunk_size, *chunk_crc );
#endif

	/* Write the chunk data to the segment file
	 */
	write_count = libewf_segment_file_handle_write(
	               segment_file_handle,
	               chunk_data,
	               size );

	if( write_count != (ssize_t) size )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write data.\n",
		 function );

		return( -1 );
	}
	total_write_count += write_count;

	/* Write the CRC if necessary
	 */
	if( write_crc != 0 )
	{
		if( libewf_endian_revert_32bit( *chunk_crc, calculated_crc_buffer ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to revert CRC value.\n",
			 function );

			return( -1 );
		}
		write_count = libewf_segment_file_handle_write(
		               segment_file_handle,
		               calculated_crc_buffer,
		               EWF_CRC_SIZE );

		if( write_count != (ssize_t) EWF_CRC_SIZE )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to write CRC.\n",
			 function );

			return( -1 );
		}
		total_write_count += write_count;
	}
	return( total_write_count );
}

/* Correct the sections before the actual data chunks
 * Also write the necessary sections after the actual data chunks to file (like table and table2 sections for EWF-E01 format)
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_segment_file_write_chunks_correction( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, LIBEWF_OFFSET_TABLE *offset_table, off64_t chunks_section_offset, size64_t chunks_section_size, uint32_t amount_of_chunks, uint32_t section_amount_of_chunks, uint8_t format, uint8_t ewf_format )
{
	EWF_CHAR *table_section_string   = NULL;
	static char *function            = "libewf_segment_file_write_chunks_correction";
	off64_t last_segment_file_offset = 0;
	off64_t base_offset              = 0;
	ssize_t total_write_count        = 0;
	ssize_t write_count              = 0;

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file handle.\n",
		 function );

		return( -1 );
	}
	if( format == LIBEWF_FORMAT_ENCASE6 )
	{
		if( chunks_section_size >= (size64_t) INT64_MAX )
		{
			LIBEWF_WARNING_PRINT( "%s: invalid chunk section size value exceeds maximum.\n",
			 function );

			return( -1 );
		}
	}
	else if( chunks_section_size >= (size64_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid chunk section size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	last_segment_file_offset = segment_file_handle->file_offset;

	/* Seek the start of the data chunks
	*/
	LIBEWF_VERBOSE_PRINT( "%s: setting file descriptor to start of chunks section offset: %" PRIu32 ".\n",
	 function, chunks_section_offset );

	if( libewf_segment_file_handle_seek_offset(
	     segment_file_handle,
	     chunks_section_offset ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to find offset to correct sectors size.\n",
		 function );

		return( -1 );
	}
	if( ( ewf_format == EWF_FORMAT_S01 )
	 || ( format == LIBEWF_FORMAT_ENCASE1 ) )
	{
		LIBEWF_VERBOSE_PRINT( "%s: correcting table section size: %" PRIu64 " offset: %jd.\n",
		 function, chunks_section_size, chunks_section_offset );

		/* Rewrite table section start
		 */
		write_count = libewf_section_table_write(
		               segment_file_handle,
		               0,
		               offset_table,
		               ( amount_of_chunks - section_amount_of_chunks ),
		               section_amount_of_chunks,
		               (EWF_CHAR *) "table",
		               5,
		               (size_t) chunks_section_size,
		               format,
		               ewf_format,
		               0 );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to rewrite %" PRIs_EWF " section.\n",
			 function, table_section_string );

			return( -1 );
		}
	}
	else if( ewf_format == EWF_FORMAT_E01 )
	{
		LIBEWF_VERBOSE_PRINT( "%s: correcting sectors section size: %zu offset: %jd.\n",
		 function, chunks_section_size, chunks_section_offset );

		/* Rewrite sectors section start
		 */
		write_count = libewf_section_sectors_write(
		               segment_file_handle,
		               chunks_section_size,
		               0 );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to rewrite sectors section.\n",
			 function );

			return( -1 );
		}
	}
	/* Seek the end of the chunks section
	*/
	LIBEWF_VERBOSE_PRINT( "%s: setting file descriptor back to end of data at offset: %" PRIu32 ".\n",
	 function, last_segment_file_offset );

	if( libewf_segment_file_handle_seek_offset(
	     segment_file_handle,
	     last_segment_file_offset ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to find offset to continue.\n",
		 function );

		return( -1 );
	}
	if( ( ewf_format == EWF_FORMAT_E01 )
	 && ( format != LIBEWF_FORMAT_ENCASE1 ) )
	{
		if( ( format == LIBEWF_FORMAT_ENCASE6 )
		 || ( format == LIBEWF_FORMAT_LINEN6 ) )
		{
			base_offset = chunks_section_offset;
		}
		/* Write table section start
		 */
		write_count = libewf_section_table_write(
		               segment_file_handle,
		               base_offset,
		               offset_table,
		               ( amount_of_chunks - section_amount_of_chunks ),
		               section_amount_of_chunks,
		               (EWF_CHAR *) "table",
		               5,
		               0,
		               format,
		               ewf_format,
		               0 );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to rewrite table section.\n",
			 function );

			return( -1 );
		}
		total_write_count += write_count;

		/* Write table2 section start
		 */
		write_count = libewf_section_table_write(
		               segment_file_handle,
		               base_offset,
		               offset_table,
		               ( amount_of_chunks - section_amount_of_chunks ),
		               section_amount_of_chunks,
		               (EWF_CHAR *) "table2",
		               6,
		               0,
		               format,
		               ewf_format,
		               0 );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to rewrite table2 section.\n",
			 function );

			return( -1 );
		}
		total_write_count += write_count;
	}
	return( total_write_count );
}

/* Write a delta chunk of data to a segment file and update the offset table
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_segment_file_write_delta_chunk( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, uint32_t chunk, EWF_CHAR *chunk_data, size_t chunk_size, EWF_CRC *chunk_crc, uint8_t write_crc )
{
	static char *function = "libewf_segment_file_write_delta_chunk";
	ssize_t write_count   = 0;

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file handle.\n",
		 function );

		return( -1 );
	}
	if( chunk_data == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid chunk data.\n",
		 function );

		return( -1 );
	}
	if( chunk_crc == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid chunk CRC.\n",
		 function );

		return( -1 );
	}

	/* Write the chunk in the delta segment file
	 */
	write_count = libewf_section_delta_chunk_write(
		       segment_file_handle,
		       chunk, 
		       chunk_data, 
		       chunk_size, 
		       chunk_crc,
	               write_crc,
	               0 );

	/* refactor */

	if( write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write delta chunk data.\n",
		 function );

		return( -1 );
	}
	return( write_count );
}

/* Closes the segment file, necessary sections at the end of the segment file will be written
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_segment_file_write_close( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, uint16_t segment_number, uint32_t segment_amount_of_chunks, int last_segment_file, LIBEWF_HASH_SECTIONS *hash_sections, LIBEWF_VALUES_TABLE *hash_values, LIBEWF_MEDIA_VALUES *media_values, LIBEWF_SECTOR_TABLE *acquiry_errors, int8_t compression_level, uint8_t format, uint8_t ewf_format, EWF_DATA **cached_data_section )
{
	static char *function     = "libewf_segment_file_write_close";
	ssize_t total_write_count = 0;
	ssize_t write_count       = 0;

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file handle.\n",
		 function );

		return( -1 );
	}
	if( hash_sections == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid hash sections.\n",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid media values.\n",
		 function );

		return( -1 );
	}
	if( acquiry_errors == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid acquiry errors.\n",
		 function );

		return( -1 );
	}
	if( last_segment_file != 0 )
	{
		/* Write the data section for a single segment file only for EWF-E01
		 */
		if( ( ewf_format == EWF_FORMAT_E01 )
		 && ( segment_number == 1 ) )
		{
			write_count = libewf_section_data_write(
				       segment_file_handle,
				       media_values,
				       compression_level,
				       format,
				       cached_data_section,
				       0 );

			if( write_count == -1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to write data section.\n",
				 function );

				return( -1 );
			}
			total_write_count += write_count;
		}
		/* Write the error2 section if required 
		 */
		if( ( acquiry_errors->amount > 0 )
		 && ( ( format == LIBEWF_FORMAT_ENCASE3 )
		  || ( format == LIBEWF_FORMAT_ENCASE4 )
		  || ( format == LIBEWF_FORMAT_ENCASE5 )
		  || ( format == LIBEWF_FORMAT_ENCASE6 )
		  || ( format == LIBEWF_FORMAT_LINEN5 )
		  || ( format == LIBEWF_FORMAT_LINEN6 )
		  || ( format == LIBEWF_FORMAT_EWFX ) ) )
		{
			write_count = libewf_section_error2_write(
			               segment_file_handle,
			               acquiry_errors );

			if( write_count == -1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to write error2 section.\n",
				 function );

				return( -1 );
			}
			total_write_count += write_count;
		}
		/* Write the hash section if required
		 */
		if( hash_sections->md5_hash_set != 0 )
		{
			write_count = libewf_section_hash_write(
			               segment_file_handle,
			               hash_sections->md5_hash );

			if( write_count == -1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to write hash section.\n",
				 function );

				return( -1 );
			}
			total_write_count += write_count;
		}

		/* Write the xhash section
		 */
		if( format == LIBEWF_FORMAT_EWFX )
		{
			if( hash_sections->xhash != NULL )
			{
				LIBEWF_WARNING_PRINT( "%s: xhash already set - cleaning previous defintion.\n",
				 function );

				libewf_common_free( hash_sections->xhash );
			}
			hash_sections->xhash = libewf_hash_values_generate_xhash_string_ewfx(
			                        hash_values,
			                        &( hash_sections->xhash_size ) );

			if( hash_sections->xhash == NULL )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to generate xhash.\n",
				 function );

				return( -1 );
			}
			write_count = libewf_section_xhash_write(
			               segment_file_handle,
			               hash_sections->xhash,
			               hash_sections->xhash_size,
			               EWF_COMPRESSION_DEFAULT );

			if( write_count == -1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to write xhash section.\n",
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
	               last_segment_file,
	               format,
	               ewf_format );

	if( write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write end of segment file.\n",
		 function );

		return( -1 );
	}
	total_write_count += write_count;

	segment_file_handle->amount_of_chunks = segment_amount_of_chunks;

	if( libewf_segment_file_handle_close( segment_file_handle ) != 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to close segment file handle.\n",
		 function );

		return( -1 );
	}
	return( total_write_count );
}

