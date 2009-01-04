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

/* Allocates memory for a segment file struct
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_SEGMENT_FILE *libewf_segment_file_alloc( void )
{
	LIBEWF_SEGMENT_FILE *segment_file = NULL;
	static char *function             = "libewf_segment_file_alloc";

	segment_file = (LIBEWF_SEGMENT_FILE *) libewf_common_alloc( LIBEWF_SEGMENT_FILE_SIZE );

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create segment file.\n",
		 function );

		return( NULL );
	}
	segment_file->section_list = (LIBEWF_SECTION_LIST *) libewf_common_alloc( LIBEWF_SECTION_LIST_SIZE );

	if( segment_file->section_list == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create section list.\n",
		 function );

		libewf_common_free( segment_file );

		return( NULL );
	}
	segment_file->file_descriptor     = -1;
	segment_file->filename            = NULL;
	segment_file->length_filename     = 0;
	segment_file->file_offset         = 0;
	segment_file->amount_of_chunks    = 0;
	segment_file->section_list->first = NULL;
	segment_file->section_list->last  = NULL;

	return( segment_file );
}

/* Opens a segment file
 * Sets the filename and the file descriptor in the segment file struct
 * Returns 1 if successful, or -1 on error
 */
int libewf_segment_file_open( LIBEWF_SEGMENT_FILE *segment_file, uint8_t flags )
{
	static char *function = "libewf_segment_file_open";

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( segment_file->filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file - missing filename.\n",
		 function );

		return( -1 );
	}
	segment_file->file_descriptor = libewf_filename_open(
	                                 segment_file->filename,
	                                 flags );

	if( segment_file->file_descriptor == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to open segment file: %" PRIs_EWF_filename ".\n",
		 function, segment_file->filename );

		return( -1 );
	}
	return( 1 );
}

/* Re-opens a segment file
 * Returns 1 if successful, or -1 on error
 */
int libewf_segment_file_reopen( LIBEWF_SEGMENT_FILE *segment_file, uint8_t flags )
{
	static char *function = "libewf_segment_file_reopen";

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( segment_file->filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file - missing filename.\n",
		 function );

		return( -1 );
	}
	if( libewf_common_close( segment_file->file_descriptor ) != 0 )
	{
		LIBEWF_VERBOSE_PRINT( "%s: unable to close segment file: %" PRIs_EWF_filename ".\n",
		 function, segment_file->filename );
	}
	segment_file->file_descriptor = libewf_filename_open(
	                                 segment_file->filename,
	                                 flags );

	if( segment_file->file_descriptor == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to open file: %" PRIs_EWF_filename ".\n",
		 function, segment_file->filename );

		return( -1 );
	}
	/* Seek the previous file offset
	 */
	if( libewf_common_lseek(
	     segment_file->file_descriptor,
	     segment_file->file_offset,
	     SEEK_CUR ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to seek in file: %" PRIs_EWF_filename ".\n",
		 function, segment_file->filename );

		return( -1 );
	}
	return( 1 );
}

/* Reads a buffer from a segment file
 * Updates the segment file offset
 * Returns the amount of bytes read if successful, or -1 on errror
 */
ssize_t libewf_segment_file_read( LIBEWF_SEGMENT_FILE *segment_file, void *buffer, size_t size )
{
	static char *function = "libewf_segment_file_read";
	ssize_t read_count    = 0;

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( segment_file->filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file - missing filename.\n",
		 function );

		return( -1 );
	}
	if( segment_file->file_descriptor == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file - invalid file descriptor.\n",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid buffer.\n",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_common_read(
	              segment_file->file_descriptor,
	              buffer,
	              size );

	if( read_count > 0 )
	{
		segment_file->file_offset += (off64_t) read_count;
	}
	if( read_count != (ssize_t) size )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read from segment file: %" PRIs_EWF_filename ".\n",
		 function, segment_file->filename );
	}
	return( read_count );
}

/* Writes a buffer to a segment file
 * Updates the segment file offset
 * Returns the amount of bytes written if successful, or -1 on errror
 */
ssize_t libewf_segment_file_write( LIBEWF_SEGMENT_FILE *segment_file, void *buffer, size_t size )
{
	static char *function = "libewf_segment_file_write";
	ssize_t write_count   = 0;

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( segment_file->filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file - missing filename.\n",
		 function );

		return( -1 );
	}
	if( segment_file->file_descriptor == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file - invalid file descriptor.\n",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid buffer.\n",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	write_count = libewf_common_write(
	               segment_file->file_descriptor,
	               buffer,
	               size );

	if( write_count > 0 )
	{
		segment_file->file_offset += (off64_t) write_count;
	}
	if( write_count != (ssize_t) size )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write to segment file: %" PRIs_EWF_filename ".\n",
		 function, segment_file->filename );
	}
	return( write_count );
}

/* Seeks a certain offset within the a segment file
 * Returns 1 if the seek is successful, or -1 on error
 */
off64_t libewf_segment_file_seek_offset( LIBEWF_SEGMENT_FILE *segment_file, off64_t offset )
{
	static char *function = "libewf_segment_file_seek_offset";

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( segment_file->file_descriptor == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file - invalid file descriptor.\n",
		 function );

		return( -1 );
	}
	if( segment_file->filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file - missing filename.\n",
		 function );

		return( -1 );
	}
	if( offset > (off64_t) INT64_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid offset value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( segment_file->file_offset != offset )
	{
		LIBEWF_VERBOSE_PRINT( "%s: seeking offset: %jd in segment file: %" PRIs_EWF_filename " with file descriptor: %d.\n",
		 function, offset, segment_file->filename, segment_file->file_descriptor );

		if( libewf_common_lseek(
		     segment_file->file_descriptor,
		     offset,
		     SEEK_SET ) == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to find offset: %jd in segment file: %" PRIs_EWF_filename ".\n",
			 function, offset, segment_file->filename );

			return( -1 );
		}
		segment_file->file_offset = offset;
	}
	return( offset );
}

/* Reads the file header from a segment file
 * Returns the amount of bytes read if successful, or -1 on errror
 */
ssize_t libewf_segment_file_read_file_header( LIBEWF_SEGMENT_FILE *segment_file, uint16_t *segment_number, uint8_t *segment_file_type )
{
	EWF_FILE_HEADER file_header;

	static char *function = "libewf_segment_file_read_file_header";
	ssize_t read_count    = 0;

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
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
	read_count = libewf_segment_file_read(
	              segment_file,
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

/* Creates a new segment file and opens it for writing
 * The necessary sections at the start of the segment file are written
 * Returns 1 if successful, or -1 on error
 */
int libewf_segment_file_create( LIBEWF_SEGMENT_TABLE *segment_table, uint16_t segment_number, int16_t maximum_amount_of_segments, uint8_t segment_file_type, uint8_t ewf_format, uint8_t format )
{
	LIBEWF_SEGMENT_FILE *segment_file = NULL;
	static char *function             = "libewf_segment_file_create";

	if( segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment table.\n",
		 function );

		return( -1 );
	}
	if( segment_table->segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment table - missing segment files.\n",
		 function );

		return( -1 );
	}
	if( segment_number == 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment number: 0.\n",
		 function );

		return( -1 );
	}
	if( segment_number > segment_table->amount )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment number value out of range.\n",
		 function );

		return( -1 );
	}

	/* Check if one additional entries in the segment table are needed
	 */
	if( segment_number >= segment_table->amount )
	{
		/* Add one additional entry because the 0 entry is used for the basename
		 */
		if( libewf_segment_table_realloc( segment_table, ( segment_number + 1 ) ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to reallocate segment table.\n",
			 function );

			return( -1 );
		}
	}
	/* Check if the entry has already been filled
	 */
	else if( segment_table->segment_file[ segment_number ] != NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: segment file has already been created.\n",
		 function );

		return( -1 );
	}
	segment_file = libewf_segment_file_alloc();

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create segment file.\n",
		 function );

		return( -1 );
	}
	segment_table->segment_file[ segment_number ] = segment_file;

	if( libewf_filename_create(
	     &( segment_file->filename ),
	     &( segment_file->length_filename ),
	     segment_table->segment_file[ 0 ]->filename,
	     segment_table->segment_file[ 0 ]->length_filename,
	     segment_number,
	     maximum_amount_of_segments,
	     segment_file_type,
	     ewf_format,
	     format ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create segment file filename.\n",
		 function );

		return( -1 );
	}
	if( ( segment_file->filename == NULL )
	 || ( segment_file->length_filename == 0 ) )
	{
		LIBEWF_WARNING_PRINT( "%s: filename is empty.\n",
		 function );

		return( -1 );
	}
	LIBEWF_VERBOSE_PRINT( "%s: segment file created: %" PRIu32 " with name: %" PRIs_EWF_filename ".\n",
	 function, segment_number, segment_file->filename );

	if( libewf_segment_file_open(
	     segment_file,
	     LIBEWF_OPEN_WRITE ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to open segment file: %" PRIs_EWF_filename ".\n",
		 function, segment_file->filename );

		return( -1 );
	}
	return( 1 );
}

/* Builds the segment table from all segment files
 * Returns 1 if successful, 0 if not, or -1 on error
 */
int libewf_segment_file_build_segment_table( LIBEWF_INTERNAL_HANDLE *internal_handle )
{
	static char *function   = "libewf_segment_file_build_segment_table";
	uint16_t segment_number = 0;
	int last_segment_file   = 0;
	int result              = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing segment table.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->segment_table->segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - invalid segment table - missing segment files.\n",
		 function );

		return( -1 );
	}
	/* Read the segment and offset table from the segment file(s)
	 */
	for( segment_number = 1; segment_number < internal_handle->segment_table->amount; segment_number++ )
	{
		LIBEWF_VERBOSE_PRINT( "%s: reading section list for segment number: %" PRIu16 ".\n",
		 function, segment_number );

		result = libewf_segment_file_read_sections(
		          internal_handle,
		          internal_handle->segment_table->segment_file[ segment_number ],
		          &last_segment_file );

		if( result == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to read sections.\n",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to missing next or done section.\n",
			 function );

			return( 0 );
		}
	}
	/* Check to see if the done section has been found in the last segment file
	 */
	if( last_segment_file != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to find the last segment file.\n",
		 function );

		return( 0 );
	}
	return( 1 );
}

/* Builds the delta segment table from all delta segment files
 * Returns 1 if successful, 0 if not, or -1 on error
 */
int libewf_segment_file_build_delta_segment_table( LIBEWF_INTERNAL_HANDLE *internal_handle )
{
	static char *function   = "libewf_segment_file_build_delta_segment_table";
	uint16_t segment_number = 0;
	int last_segment_file   = 0;
	int result              = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->delta_segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing delta segment table.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->delta_segment_table->segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - invalid delta segment table - missing segment files.\n",
		 function );

		return( -1 );
	}
	/* Read the segment and offset table from the segment file(s)
	 */
	for( segment_number = 1; segment_number < internal_handle->delta_segment_table->amount; segment_number++ )
	{
		LIBEWF_VERBOSE_PRINT( "%s: reading section list for delta segment number: %" PRIu16 ".\n",
		 function, segment_number );

		result = libewf_segment_file_read_sections(
		          internal_handle,
		          internal_handle->delta_segment_table->segment_file[ segment_number ],
		          &last_segment_file );

		if( result == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to read sections.\n",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to missing next or done section.\n",
			 function );

			return( 0 );
		}
	}
	/* Check to see if the done section has been found in the last delta segment file
	 */
	if( last_segment_file != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to find the last delta segment file.\n",
		 function );

		return( 0 );
	}
	return( 1 );
}

/* Reads all sections from a segment file into the section list specific
 * for the segment file in the segment table in the handle
 * Returns 1 if successful, 0 if not, or -1 on error
 */
int libewf_segment_file_read_sections( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_SEGMENT_FILE *segment_file, int *last_segment_file )
{
	EWF_SECTION section;

	static char *function   = "libewf_segment_file_read_sections";
	off64_t previous_offset = 0;
	int result              = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
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
		          internal_handle,
		          segment_file,
		          &section,
		          &previous_offset );

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
ssize_t libewf_segment_file_write_headers( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_SEGMENT_FILE *segment_file )
{
	static char *function     = "libewf_segment_file_write_headers";
	size_t header_size        = 0;
	size_t header2_size       = 0;
	size_t xheader_size       = 0;
	ssize_t write_count       = 0;
	ssize_t total_write_count = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( ( internal_handle->header == NULL )
	 || ( internal_handle->header_size == 0 ) )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header.\n",
		 function );

		return( -1 );
	}
	header_size = internal_handle->header_size - 1;

	if( ( internal_handle->format == LIBEWF_FORMAT_EWF )
	 || ( internal_handle->format == LIBEWF_FORMAT_SMART )
	 || ( internal_handle->format == LIBEWF_FORMAT_ENCASE1 ) )
	{
		/* The header should be written only once
		 * and using the compression used in the file
		 */
		write_count = libewf_section_header_write(
		               segment_file,
		               internal_handle->header,
		               header_size,
		               internal_handle->compression_level );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to write single header section.\n",
			 function );

			return( -1 );
		}
		total_write_count += write_count;

		internal_handle->amount_of_header_sections += 1;
	}
	else if( ( internal_handle->format == LIBEWF_FORMAT_ENCASE2 )
	 || ( internal_handle->format == LIBEWF_FORMAT_ENCASE3 )
	 || ( internal_handle->format == LIBEWF_FORMAT_LINEN5 )
	 || ( internal_handle->format == LIBEWF_FORMAT_LINEN6 )
	 || ( internal_handle->format == LIBEWF_FORMAT_FTK ) )
	{
		/* The header should be written twice
		 * the default compression is used
		 */
		write_count = libewf_section_header_write(
		               segment_file,
		               internal_handle->header,
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
		               segment_file,
		               internal_handle->header,
		               header_size,
		               EWF_COMPRESSION_DEFAULT );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to write second header section.\n",
			 function );

			return( -1 );
		}
		total_write_count += write_count;

		internal_handle->amount_of_header_sections += 2;
	}
	else if( ( internal_handle->format == LIBEWF_FORMAT_ENCASE4 )
	 || ( internal_handle->format == LIBEWF_FORMAT_ENCASE5 )
	 || ( internal_handle->format == LIBEWF_FORMAT_ENCASE6 ) )
	{
		if( ( internal_handle->header2 == NULL )
		 && ( internal_handle->header2_size == 0 ) )
		{
			LIBEWF_WARNING_PRINT( "%s: invalid header2.\n",
			 function );

			return( -1 );
		}
		header2_size = internal_handle->header2_size - 2;

		/* The header2 should be written twice
		 * the default compression is used
		 */
		write_count = libewf_section_header2_write(
		               segment_file,
		               internal_handle->header2,
		               internal_handle->header2_size,
		               EWF_COMPRESSION_DEFAULT );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to write first header2 section.\n",
			 function );

			return( -1 );
		}
		total_write_count += write_count;

		write_count = libewf_section_header2_write(
		               segment_file,
		               internal_handle->header2,
		               internal_handle->header2_size,
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
		               segment_file,
		               internal_handle->header,
		               header_size,
		               EWF_COMPRESSION_DEFAULT );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to write third header section.\n",
			 function );

			return( -1 );
		}
		total_write_count += write_count;

		internal_handle->amount_of_header_sections += 3;
	}
	/* EWFX uses the header and header2 for backwards compatibility
	 */
	else if( internal_handle->format == LIBEWF_FORMAT_EWFX )
	{
		if( ( internal_handle->xheader == NULL )
		 && ( internal_handle->xheader_size == 0 ) )
		{
			LIBEWF_WARNING_PRINT( "%s: invalid xheader.\n",
			 function );

			return( -1 );
		}
		xheader_size = internal_handle->xheader_size;

		if( ( internal_handle->header2 == NULL )
		 && ( internal_handle->header2_size == 0 ) )
		{
			LIBEWF_WARNING_PRINT( "%s: invalid header2.\n",
			 function );

			return( -1 );
		}
		header2_size = internal_handle->header2_size - 2;

		/* The xheader should be written once
		 * the default compression is used
		 */
		write_count = libewf_section_xheader_write(
		               segment_file,
		               internal_handle->xheader,
		               xheader_size,
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
		               segment_file,
		               internal_handle->header2,
		               internal_handle->header2_size,
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
		               segment_file,
		               internal_handle->header,
		               header_size,
		               EWF_COMPRESSION_DEFAULT );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to write third header section.\n",
			 function );

			return( -1 );
		}
		total_write_count += write_count;

		internal_handle->amount_of_header_sections += 3;
	}
	return( total_write_count );
}

/* Write the last section at the end of the segment file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_segment_file_write_last_section( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_SEGMENT_FILE *segment_file, int last_segment_file )
{
	EWF_CHAR *last_section_type = NULL;
	static char *function       = "libewf_segment_file_write_last_section";
	ssize_t write_count         = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
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
		       segment_file,
		       last_section_type,
		       4,
		       internal_handle->format,
		       internal_handle->ewf_format );

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
ssize_t libewf_segment_file_write_start( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_SEGMENT_FILE *segment_file, uint16_t segment_number, uint8_t segment_file_type )
{
	EWF_FILE_HEADER file_header;

	static char *function         = "libewf_segment_file_write_start";
	const uint8_t *file_signature = NULL;
	ssize_t total_write_count     = 0;
	ssize_t write_count           = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media values.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->write == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing subhandle write.\n",
		 function );

		return( -1 );
	}
	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( segment_number == 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment number.\n",
		 function );

		return( -1 );
	}
	if( segment_file->file_descriptor == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid file descriptor.\n",
		 function );

		return( -1 );
	}
	if( segment_file->section_list == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid section list.\n",
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
	if( libewf_common_memcpy( file_header.signature, file_signature, 8 ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set file signature.\n",
		 function );

		return( -1 );
	}
	if( libewf_endian_revert_16bit( segment_number, file_header.fields_segment ) != 1 )
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
	write_count = libewf_segment_file_write(
	               segment_file,
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
				       internal_handle,
				       segment_file );

			if( write_count == -1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to write header sections.\n",
				 function );

				return( -1 );
			}
			total_write_count += write_count;

			if( internal_handle->ewf_format == EWF_FORMAT_S01 )
			{
				/* Write volume (SMART) section
				 */
				write_count = libewf_section_volume_s01_write(
					       segment_file,
					       internal_handle->media_values,
					       internal_handle->format,
					       0 );
			}
			else if( internal_handle->ewf_format == EWF_FORMAT_E01 )
			{
				/* Write volume section
				 */
				write_count = libewf_section_volume_e01_write(
					       segment_file,
					       internal_handle->media_values,
					       internal_handle->format,
					       internal_handle->compression_level,
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
		else if( internal_handle->ewf_format == EWF_FORMAT_E01 )
		{
			/* Write data section
			 */
			write_count = libewf_section_data_write(
				       segment_file,
				       internal_handle->media_values->amount_of_chunks,
				       internal_handle->media_values->sectors_per_chunk,
				       internal_handle->media_values->bytes_per_sector,
				       internal_handle->media_values->amount_of_sectors,
				       internal_handle->media_values->error_granularity,
				       internal_handle->media_values->media_type,
				       internal_handle->media_values->media_flags,
				       internal_handle->compression_level,
				       internal_handle->media_values->guid,
				       internal_handle->format,
				       &( internal_handle->write->data_section ),
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
ssize_t libewf_segment_file_write_chunks_section_start( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_SEGMENT_FILE *segment_file, size32_t chunk_size, uint32_t total_chunk_amount, uint32_t segment_chunk_amount )
{
	static char *function = "libewf_segment_file_write_chunks_section_start";
	ssize_t write_count   = 0;
	size_t section_size   = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing offset table.\n",
		 function );

		return( -1 );
	}
	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}

	/* The segment_chunk_amount contains the estimated amount of chunks for this section
	 */
	if( internal_handle->offset_table->amount < ( total_chunk_amount + segment_chunk_amount ) )
	{
		if( libewf_offset_table_realloc(
		     internal_handle->offset_table,
		     ( total_chunk_amount + segment_chunk_amount ) ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to reallocate offset table.\n",
			 function );

			return( -1 );
		}
	}
	if( ( internal_handle->ewf_format == EWF_FORMAT_S01 )
	 || ( internal_handle->format == LIBEWF_FORMAT_ENCASE1 ) )
	{
		/* Write table section start
		 */
		write_count = libewf_section_table_write(
		               segment_file,
		               0,
		               internal_handle->offset_table,
		               total_chunk_amount,
		               segment_chunk_amount,
		               (EWF_CHAR *) "table",
		               5,
		               0,
		               internal_handle->format,
		               internal_handle->ewf_format,
		               1 );

		if( write_count == -1 )
		{
			LIBEWF_VERBOSE_PRINT( "%s: unable to write table section.\n",
			 function );

			return( -1 );
		}
	}
	else if( internal_handle->ewf_format == EWF_FORMAT_E01 )
	{
		section_size = segment_chunk_amount 
		             * ( chunk_size + EWF_CRC_SIZE );

		/* Write sectors section start
		 */
		write_count = libewf_section_start_write(
		               segment_file,
		               (EWF_CHAR *) "sectors",
		               7,
		               section_size );

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
ssize_t libewf_segment_file_write_chunks_data( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_SEGMENT_FILE *segment_file, uint32_t chunk, EWF_CHAR *chunk_data, size_t size, int8_t is_compressed, EWF_CRC *chunk_crc, int8_t write_crc )
{
	uint8_t calculated_crc_buffer[ 4 ];

#if defined( HAVE_VERBOSE_OUTPUT )
	char *chunk_type          = NULL;
#endif
	static char *function     = "libewf_segment_file_write_chunks_data";
	ssize_t write_count       = 0;
	ssize_t total_write_count = 0;
	size_t chunk_size         = size;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing offset table.\n",
		 function );

		return( -1 );
	}
	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
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
	if( segment_file->file_descriptor == -1 )
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
	if( internal_handle->offset_table->amount < ( chunk + 1 ) )
	{
		if( libewf_offset_table_realloc(
		     internal_handle->offset_table,
		     ( chunk + 1 ) ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to reallocate offset table.\n",
			 function );

			return( -1 );
		}
	}
	/* Set the values in the offset table
	 */
	internal_handle->offset_table->chunk_offset[ chunk ].segment_file = segment_file;
	internal_handle->offset_table->chunk_offset[ chunk ].file_offset  = segment_file->file_offset;
	internal_handle->offset_table->chunk_offset[ chunk ].size         = chunk_size;
	internal_handle->offset_table->chunk_offset[ chunk ].compressed   = is_compressed;

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
	 function, chunk_type, ( chunk + 1 ), segment_file->file_offset, chunk_size, *chunk_crc );
#endif

	/* Write the chunk data to the segment file
	 */
	write_count = libewf_segment_file_write(
	               segment_file,
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
		write_count = libewf_segment_file_write(
		               segment_file,
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
ssize_t libewf_segment_file_write_chunks_correction( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_SEGMENT_FILE *segment_file, off64_t chunks_section_offset, size64_t chunks_section_size, uint32_t amount_of_chunks, uint32_t section_amount_of_chunks )
{
	EWF_CHAR *table_section_string   = NULL;
	static char *function            = "libewf_segment_file_write_chunks_correction";
	off64_t last_segment_file_offset = 0;
	off64_t base_offset              = 0;
	ssize_t total_write_count        = 0;
	ssize_t write_count              = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->format == LIBEWF_FORMAT_ENCASE6 )
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
	last_segment_file_offset = segment_file->file_offset;

	/* Seek the start of the data chunks
	*/
	LIBEWF_VERBOSE_PRINT( "%s: setting file descriptor to start of chunks section offset: %" PRIu32 ".\n",
	 function, chunks_section_offset );

	if( libewf_segment_file_seek_offset(
	     segment_file,
	     chunks_section_offset ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to find offset to correct sectors size.\n",
		 function );

		return( -1 );
	}
	if( ( internal_handle->ewf_format == EWF_FORMAT_S01 )
	 || ( internal_handle->format == LIBEWF_FORMAT_ENCASE1 ) )
	{
		LIBEWF_VERBOSE_PRINT( "%s: correcting table section size: %" PRIu64 " offset: %jd.\n",
		 function, chunks_section_size, chunks_section_offset );

		/* Rewrite table section start
		 */
		write_count = libewf_section_table_write(
		               segment_file,
		               0,
		               internal_handle->offset_table,
		               ( amount_of_chunks - section_amount_of_chunks ),
		               section_amount_of_chunks,
		               (EWF_CHAR *) "table",
		               5,
		               (size_t) chunks_section_size,
		               internal_handle->format,
		               internal_handle->ewf_format,
		               0 );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to rewrite %" PRIs_EWF " section.\n",
			 function, table_section_string );

			return( -1 );
		}
	}
	else if( internal_handle->ewf_format == EWF_FORMAT_E01 )
	{
		LIBEWF_VERBOSE_PRINT( "%s: correcting sectors section size: %zu offset: %jd.\n",
		 function, chunks_section_size, chunks_section_offset );

		/* Rewrite sectors section start
		 */
		write_count = libewf_section_start_write(
		               segment_file,
		               (EWF_CHAR *) "sectors",
		               7,
		               chunks_section_size );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to rewrite sectors section.\n",
			 function );

			return( -1 );
		}
		if( libewf_section_list_append(
		     segment_file->section_list,
		     (EWF_CHAR *) "sectors",
		     7,
		     chunks_section_offset,
		     ( chunks_section_offset + write_count ) ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to append sectors section to section list.\n",
			 function );

			return( -1 );
		}
	}
	/* Seek the end of the chunks section
	*/
	LIBEWF_VERBOSE_PRINT( "%s: setting file descriptor back to end of data at offset: %" PRIu32 ".\n",
	 function, last_segment_file_offset );

	if( libewf_segment_file_seek_offset(
	     segment_file,
	     last_segment_file_offset ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to find offset to continue.\n",
		 function );

		return( -1 );
	}
	if( ( internal_handle->ewf_format == EWF_FORMAT_E01 )
	 && ( internal_handle->format != LIBEWF_FORMAT_ENCASE1 ) )
	{
		if( ( internal_handle->format == LIBEWF_FORMAT_ENCASE6 )
		 || ( internal_handle->format == LIBEWF_FORMAT_LINEN6 ) )
		{
			base_offset = chunks_section_offset;
		}
		/* Write table section start
		 */
		write_count = libewf_section_table_write(
		               segment_file,
		               base_offset,
		               internal_handle->offset_table,
		               ( amount_of_chunks - section_amount_of_chunks ),
		               section_amount_of_chunks,
		               (EWF_CHAR *) "table",
		               5,
		               0,
		               internal_handle->format,
		               internal_handle->ewf_format,
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
		               segment_file,
		               base_offset,
		               internal_handle->offset_table,
		               ( amount_of_chunks - section_amount_of_chunks ),
		               section_amount_of_chunks,
		               (EWF_CHAR *) "table2",
		               6,
		               0,
		               internal_handle->format,
		               internal_handle->ewf_format,
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
ssize_t libewf_segment_file_write_delta_chunk( LIBEWF_SEGMENT_FILE *segment_file, uint32_t chunk, EWF_CHAR *chunk_data, size_t chunk_size, EWF_CRC *chunk_crc, uint8_t write_crc )
{
	static char *function = "libewf_segment_file_write_delta_chunk";
	ssize_t write_count   = 0;

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
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
		       segment_file,
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
ssize_t libewf_segment_file_write_close( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_SEGMENT_FILE *segment_file, uint16_t segment_number, uint32_t segment_amount_of_chunks, int last_segment_file )
{
	static char *function     = "libewf_segment_file_write_close";
	ssize_t total_write_count = 0;
	ssize_t write_count       = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media values.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->write == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing subhandle write.\n",
		 function );

		return( -1 );
	}
	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( last_segment_file != 0 )
	{
		/* Write the data section for a single segment file only for EWF-E01
		 */
		if( ( internal_handle->ewf_format == EWF_FORMAT_E01 )
		 && ( segment_number == 1 ) )
		{
			write_count = libewf_section_data_write(
				       segment_file,
				       internal_handle->media_values->amount_of_chunks,
				       internal_handle->media_values->sectors_per_chunk,
				       internal_handle->media_values->bytes_per_sector,
				       internal_handle->media_values->amount_of_sectors,
				       internal_handle->media_values->error_granularity,
				       internal_handle->media_values->media_type,
				       internal_handle->media_values->media_flags,
				       internal_handle->compression_level,
				       internal_handle->media_values->guid,
				       internal_handle->format,
				       &( internal_handle->write->data_section ),
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
		if( ( internal_handle->amount_of_acquiry_errors > 0 )
		 && ( ( internal_handle->format == LIBEWF_FORMAT_ENCASE3 )
		  || ( internal_handle->format == LIBEWF_FORMAT_ENCASE4 )
		  || ( internal_handle->format == LIBEWF_FORMAT_ENCASE5 )
		  || ( internal_handle->format == LIBEWF_FORMAT_ENCASE6 )
		  || ( internal_handle->format == LIBEWF_FORMAT_LINEN5 )
		  || ( internal_handle->format == LIBEWF_FORMAT_LINEN6 )
		  || ( internal_handle->format == LIBEWF_FORMAT_EWFX ) ) )
		{
			write_count = libewf_section_error2_write(
			               segment_file,
			               internal_handle->acquiry_error_sectors,
			               internal_handle->amount_of_acquiry_errors );

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
		if( internal_handle->md5_hash_set != 0 )
		{
			write_count = libewf_section_hash_write(
			               segment_file,
			               internal_handle->md5_hash );

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
		if( internal_handle->format == LIBEWF_FORMAT_EWFX )
		{
			if( internal_handle->xhash != NULL )
			{
				LIBEWF_WARNING_PRINT( "%s: xhash already set - cleaning previous defintion.\n",
				 function );

				libewf_common_free( internal_handle->xhash );
			}
			internal_handle->xhash = libewf_hash_values_generate_xhash_string_ewfx(
			                          internal_handle->hash_values,
			                          &internal_handle->xhash_size );

			if( internal_handle->xhash == NULL )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to generate xhash.\n",
				 function );

				return( -1 );
			}
			write_count = libewf_section_xhash_write(
			               segment_file,
			               internal_handle->xhash,
			               internal_handle->xhash_size,
			               EWF_COMPRESSION_DEFAULT );

			if( write_count == -1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to write xhash section.\n",
				 function );

				return( -1 );
			}
			if( libewf_section_list_append(
			     segment_file->section_list,
			     (EWF_CHAR *) "xhash",
			     5,
			     segment_file->file_offset,
			     ( segment_file->file_offset + write_count ) ) == NULL )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to append xhash section to section list.\n",
				 function );

				return( -1 );
			}
			segment_file->file_offset += write_count;
			total_write_count         += write_count;
		}
	}
	/* Write the done or next section
	 * The segment file offset is updated by the function
	 */
	write_count = libewf_segment_file_write_last_section(
	               internal_handle,
		       segment_file,
	               last_segment_file );

	if( write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write end of segment file.\n",
		 function );

		return( -1 );
	}
	total_write_count += write_count;

	if( libewf_common_close( segment_file->file_descriptor ) != 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to close segment file.\n",
		 function );

		return( -1 );
	}
	segment_file->file_descriptor  = -1;
	segment_file->amount_of_chunks = segment_amount_of_chunks;

	return( total_write_count );
}

/* Retrieves a filename of a certain segment
 * Returns 1 if succesful, or -1 on error
 */
int libewf_segment_file_get_filename( LIBEWF_SEGMENT_FILE *segment_file, LIBEWF_FILENAME *filename, size_t length_filename )
{
	static char *function  = "libewf_segment_file_get_filename";
	size_t filename_length = 0;

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( segment_file->filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file - missing filename.\n",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid filename.\n",
		 function );

		return( -1 );
	}
	filename_length = segment_file->length_filename;

	/* Add one additional character for the end of line
	 */
	filename_length += 1;

	if( length_filename < filename_length )
	{
		LIBEWF_WARNING_PRINT( "%s: filename too small.\n",
		 function );

		return( -1 );
	}
	if( libewf_filename_copy(
	     filename,
	     segment_file->filename,
	     filename_length ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set filename.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets a filename for a specific segment file
 * Creates a duplicate of the string
 * Returns 1 if succesful, or -1 on error
 */
int libewf_segment_file_set_filename( LIBEWF_SEGMENT_FILE *segment_file, const LIBEWF_FILENAME *filename, size_t length_filename )
{
	static char *function = "libewf_segment_file_set_filename";

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid filename.\n",
		 function );

		return( -1 );
	}
	if( segment_file->filename != NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: filename already set: %" PRIs_EWF_filename ".\n",
		 function, segment_file->filename );

		return( -1 );
	}
	if( length_filename == 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid filename length is zero.\n",
		 function );

		return( -1 );
	}
	if( length_filename >= (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid filename length value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	/* One additional byte for the end of string character is needed
	 */
	segment_file->filename = (LIBEWF_FILENAME *) libewf_common_alloc( LIBEWF_FILENAME_SIZE * ( length_filename + 1 ) );

	if( segment_file->filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create filename.\n",
		 function );

		return( -1 );
	}
	if( libewf_filename_copy( segment_file->filename, filename, length_filename ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set filename.\n",
		 function );

		libewf_common_free( segment_file->filename );

		segment_file->filename = NULL;

		return( -1 );
	}
	/* Make sure the string is terminated
	 */
	segment_file->filename[ length_filename ] = '\0';
	segment_file->length_filename             = length_filename;

	return( 1 );
}

/* Initializes the segment table
 * opens EWF segment files for reading
 * and EWF delta segment files for reading and writing
 * Returns 1 if successful, 0 if not, or -1 on error
 */
int libewf_segment_file_read_open( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_FILENAME * const filenames[], uint16_t file_amount, uint8_t flags )
{
	LIBEWF_SEGMENT_FILE *segment_file = NULL;
	static char *function             = "libewf_segment_file_read_open";
	size_t filename_length            = 0;
	uint32_t iterator                 = 0;
	uint16_t segment_number           = 0;
	uint8_t segment_file_type         = 0;
	int file_descriptor               = 0;
	int result                        = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing segment table.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->segment_table->segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - invalid segment table - missing segment files.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->delta_segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing delta segment table.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->delta_segment_table->segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - invalid delta segment table - missing segment files.\n",
		 function );

		return( -1 );
	}
	if( filenames == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid filenames.\n",
		 function );

		return( -1 );
	}
	if( file_amount < 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid file amount at least 1 is required.\n",
		 function );

		return( -1 );
	}
	/* Get the basename of the first segment file and store it in
	 * the 0'th entry
	 */
	filename_length = libewf_filename_length( filenames[ 0 ] );

	if( filename_length <= 4 )
	{
		LIBEWF_WARNING_PRINT( "%s: filename is empty.\n",
		 function );

		return( -1 );
	}
	/* Set segment table basename
	 */
	segment_file = libewf_segment_file_alloc();

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create segment file.\n",
		 function );

		return( -1 );
	}
	internal_handle->segment_table->segment_file[ 0 ] = segment_file;

	if( libewf_segment_file_set_filename(
	     internal_handle->segment_table->segment_file[ 0 ],
	     filenames[ iterator ],
	     ( filename_length - 4 ) ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set filename in segment table.\n",
		 function );

		return( -1 );
	}
	/* Set delta segment table basename
	 */
	segment_file = libewf_segment_file_alloc();

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create segment file.\n",
		 function );

		return( -1 );
	}
	internal_handle->delta_segment_table->segment_file[ 0 ] = segment_file;

	if( libewf_segment_file_set_filename(
	     internal_handle->delta_segment_table->segment_file[ 0 ],
	     filenames[ iterator ],
	     ( filename_length - 4 ) ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set filename in delta segment table.\n",
		 function );

		return( -1 );
	}
	/* Open the segment files
	 */
	for( iterator = 0; iterator < file_amount; iterator++ )
	{
		LIBEWF_VERBOSE_PRINT( "%s: trying to open file: %" PRIs_EWF_filename ".\n",
		 function, filenames[ iterator ] );

		segment_file = libewf_segment_file_alloc();

		if( segment_file == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create segment file.\n",
			 function );

			return( -1 );
		}
		if( libewf_segment_file_set_filename(
		     segment_file,
		     filenames[ iterator ],
		     libewf_filename_length( filenames[ iterator ] ) ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to set filename in segment file.\n",
			 function );

			return( -1 );
		}
		if( libewf_segment_file_open(
		     segment_file,
		     LIBEWF_FLAG_READ ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to open segment file: %" PRIs_EWF_filename ".\n",
			 function, filenames[ iterator ] );

			return( -1 );
		}
		if( libewf_segment_file_read_file_header(
		     segment_file,
		     &segment_number,
		     &segment_file_type ) <= -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to read file header in: %" PRIs_EWF_filename ".\n",
			 function, filenames[ iterator ] );

			return( -1 );
		}
		if( segment_number == 0 )
		{
			LIBEWF_WARNING_PRINT( "%s: invalid segment number: 0.\n",
			 function );

			return( -1 );
		}
		if( segment_number > file_amount )
		{
			LIBEWF_WARNING_PRINT( "%s: invalid segment number, value out of range or missing segment files.\n",
			 function );

			return( -1 );
		}
		if( ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF )
		 || ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_LWF ) )
		{
			if( segment_number >= internal_handle->segment_table->amount )
			{
				if( libewf_segment_table_realloc(
				     internal_handle->segment_table,
				     ( segment_number + 1 ) ) != 1 )
				{
					LIBEWF_WARNING_PRINT( "%s: unable to reallocate the segment table.\n",
					 function );

					return( -1 );
				}
			}
			internal_handle->segment_table->segment_file[ segment_number ] = segment_file;

			LIBEWF_VERBOSE_PRINT( "%s: added segment file: %" PRIs_EWF_filename " with file descriptor: %d with segment number: %" PRIu16 ".\n",
			 function, filenames[ iterator ], file_descriptor, segment_number );
		}
		else if( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_DWF )
		{
			/* Make sure to re-open the delta segment file
			 * to allow writing if necessary
			 */
			if( ( flags & LIBEWF_FLAG_WRITE ) == LIBEWF_FLAG_WRITE )
			{
				if( libewf_segment_file_reopen(
				     segment_file,
				     ( LIBEWF_FLAG_READ & LIBEWF_FLAG_WRITE ) ) != 1 )
				{
					LIBEWF_WARNING_PRINT( "%s: unable to reopen segment file: %" PRIs_EWF_filename ".\n",
					 function, filenames[ iterator ] );

					return( -1 );
				}
			}
			if( segment_number >= internal_handle->delta_segment_table->amount )
			{
				if( libewf_segment_table_realloc(
				     internal_handle->delta_segment_table,
				     ( segment_number + 1 ) ) != 1 )
				{
					LIBEWF_WARNING_PRINT( "%s: unable to reallocate the delta segment table.\n",
					 function );

					return( -1 );
				}
			}
			internal_handle->delta_segment_table->segment_file[ segment_number ] = segment_file;

			LIBEWF_VERBOSE_PRINT( "%s: added delta segment file: %" PRIs_EWF_filename " with file descriptor: %d with segment number: %" PRIu16 ".\n",
			 function, filenames[ iterator ], file_descriptor, segment_number );
		}
		else
		{
			LIBEWF_WARNING_PRINT( "%s: unsupported segment file type.\n",
			 function );

			return( -1 );
		}
	}
	if( libewf_segment_file_build_segment_table( internal_handle ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to build segment table.\n",
		 function );

		return( -1 );
	}
	if( ( internal_handle->delta_segment_table->amount > 1 )
	 && ( libewf_segment_file_build_delta_segment_table( internal_handle ) != 1 ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to build delta segment table.\n",
		 function );

		return( -1 );
	}
	/* Only compare the primary and secondary offset table
	 * if the secondary table was created
	 */
	if( internal_handle->secondary_offset_table != NULL )
	{
		result = libewf_offset_table_compare(
			  internal_handle->offset_table,
			  internal_handle->secondary_offset_table );

		if( result == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to compare primary and secondary offset table.\n",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			LIBEWF_WARNING_PRINT( "%s: primary and secondary offset table differ.\n",
			 function );

			if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
			{
				return( -1 );
			}
			/* TODO Try to correct the table
			 */
		}
	}
	return( 1 );
}

/* Opens EWF segment file(s) for writing
 * Returns 1 if successful, 0 if not, or -1 on error
 */
int libewf_segment_file_write_open( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_FILENAME * const filenames[], uint16_t file_amount )
{
	LIBEWF_SEGMENT_FILE *segment_file = NULL;
	static char *function             = "libewf_segment_file_write_open";
	size_t filename_length            = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing segment table.\n",
		 function );

		return( -1 );
	}
	if( filenames == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid filenames.\n",
		 function );

		return( -1 );
	}
	if( file_amount < 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid file amount at least 1 is required.\n",
		 function );

		return( -1 );
	}
	filename_length = libewf_filename_length( filenames[ 0 ] );

	if( filename_length == 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: filename is empty.\n",
		 function );

		return( -1 );
	}
	/* Set segment table basename
	 */
	segment_file = libewf_segment_file_alloc();

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create segment file.\n",
		 function );

		return( -1 );
	}
	internal_handle->segment_table->segment_file[ 0 ] = segment_file;

	if( libewf_segment_file_set_filename(
	     internal_handle->segment_table->segment_file[ 0 ],
	     filenames[ 0 ],
	     filename_length ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set filename in segment table.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Seeks a certain chunk offset within the EWF segment file(s)
 * Returns the segment file offset if seek is successful, or -1 on error
 */
off64_t libewf_segment_file_seek_chunk_offset( LIBEWF_INTERNAL_HANDLE *internal_handle, uint32_t chunk )
{
	static char *function = "libewf_segment_file_seek_chunk_offset";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing segment table.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->segment_table->segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - invalid segment table - missing segment files.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->delta_segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing delta segment table.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->delta_segment_table->segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - invalid delta segment table - missing segment files.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing offset table.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->offset_table->chunk_offset == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - invalid offset table - missing chunk offsets.\n",
		 function );

		return( -1 );
	}
	if( chunk >= internal_handle->offset_table->amount )
	{
		LIBEWF_WARNING_PRINT( "%s: chunk: %" PRIu32 " not in offset table.\n",
		 function, chunk );

		return( -1 );
	}
	if( libewf_segment_file_seek_offset(
	     internal_handle->offset_table->chunk_offset[ chunk ].segment_file,
	     internal_handle->offset_table->chunk_offset[ chunk ].file_offset ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: cannot find chunk offset: %jd.\n",
		 function, internal_handle->offset_table->chunk_offset[ chunk ].file_offset );

		return( -1 );
	}
	internal_handle->current_chunk = chunk;

	return( internal_handle->offset_table->chunk_offset[ chunk ].file_offset );
}

/* Closes all the EWF segment file(s)
 * Returns 1 if successful, 0 if not, or -1 on error
 */
int libewf_segment_file_close_all( LIBEWF_INTERNAL_HANDLE *internal_handle )
{
	static char *function = "libewf_segment_file_close_all";
	uint16_t iterator     = 0;
	int result            = 1;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing segment table.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->segment_table->segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - invalid segment table - missing segment files.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->delta_segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing delta segment table.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->delta_segment_table->segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - invalid delta segment table - missing segment files.\n",
		 function );

		return( -1 );
	}
	for( iterator = 1; iterator < internal_handle->segment_table->amount; iterator++ )
	{
		if( internal_handle->segment_table->segment_file[ iterator ]->file_descriptor > 0 )
		{
			if( libewf_common_close( internal_handle->segment_table->segment_file[ iterator ]->file_descriptor ) != 0 )
			{
				if( internal_handle->segment_table->segment_file[ iterator ]->filename == NULL )
				{
					LIBEWF_WARNING_PRINT( "%s: unable to close segment file: %" PRIu16 ".\n",
					 function, iterator );
				}
				else
				{
					LIBEWF_WARNING_PRINT( "%s: unable to close segment file: %" PRIu16 " (%" PRIs_EWF_filename ").\n",
					 function, iterator, internal_handle->segment_table->segment_file[ iterator ]->filename );
				}
				result = 0;
			}
		}
	}
	for( iterator = 1; iterator < internal_handle->delta_segment_table->amount; iterator++ )
	{
		if( internal_handle->delta_segment_table->segment_file[ iterator ]->file_descriptor > 0 )
		{
			if( libewf_common_close( internal_handle->delta_segment_table->segment_file[ iterator ]->file_descriptor ) != 0 )
			{
				if( internal_handle->delta_segment_table->segment_file[ iterator ]->filename == NULL )
				{
					LIBEWF_WARNING_PRINT( "%s: unable to close delta segment file: %" PRIu16 ".\n",
					 function, iterator );
				}
				else
				{
					LIBEWF_WARNING_PRINT( "%s: unable to close delta segment file: %" PRIu16 " (%" PRIs_EWF_filename ").\n",
					 function, iterator, internal_handle->delta_segment_table->segment_file[ iterator ]->filename );
				}
				result = 0;
			}
		}
	}
	return( result );
}

