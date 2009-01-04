/*
 * libewf segment file handle
 *
 * Copyright (c) 2006-2008, Joachim Metz <forensics@hoffmannbv.nl>,
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

#include "libewf_common.h"
#include "libewf_notify.h"
#include "libewf_segment_file_handle.h"
#include "libewf_string.h"

/* Allocates memory for a segment file handle struct
 * Returns a pointer to the new instance, NULL on error
 */
libewf_segment_file_handle_t *libewf_segment_file_handle_alloc(
                               void )
{
	libewf_segment_file_handle_t *segment_file_handle = NULL;
	static char *function                             = "libewf_segment_file_handle_alloc";

	segment_file_handle = (libewf_segment_file_handle_t *) libewf_common_alloc(
	                                                        sizeof( libewf_segment_file_handle_t ) );

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create segment file handle.\n",
		 function );

		return( NULL );
	}
	segment_file_handle->section_list = (libewf_section_list_t *) libewf_common_alloc(
	                                                               sizeof( libewf_section_list_t ) );

	if( segment_file_handle->section_list == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create section list.\n",
		 function );

		libewf_common_free( segment_file_handle );

		return( NULL );
	}
	segment_file_handle->file_descriptor     = -1;
	segment_file_handle->filename            = NULL;
	segment_file_handle->length_filename     = 0;
	segment_file_handle->file_offset         = 0;
	segment_file_handle->amount_of_chunks    = 0;
	segment_file_handle->section_list->first = NULL;
	segment_file_handle->section_list->last  = NULL;
	segment_file_handle->file_type           = 0;

	return( segment_file_handle );
}

/* Frees memory of a segment file handle struct including elements
 */
void libewf_segment_file_handle_free(
      libewf_segment_file_handle_t *segment_file_handle )
{
        libewf_section_list_entry_t *section_list_entry         = NULL;
        libewf_section_list_entry_t *current_section_list_entry = NULL;
	static char *function                                   = "libewf_segment_file_handle_free";

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file handle.\n",
		 function );

		return;
	}
	if( segment_file_handle->filename != NULL )
	{
		libewf_common_free(
		 segment_file_handle->filename );
	}
	if( segment_file_handle->section_list != NULL )
	{
		section_list_entry = segment_file_handle->section_list->first;

		while( section_list_entry != NULL )
		{
			current_section_list_entry = section_list_entry;
			section_list_entry         = section_list_entry->next;

			libewf_common_free( current_section_list_entry );
		}
		libewf_common_free( segment_file_handle->section_list );
	}
	libewf_common_free( segment_file_handle );
}

/* Retrieves a filename of a certain segment file handle
 * Returns 1 if succesful, or -1 on error
 */
int libewf_segment_file_handle_get_filename(
     libewf_segment_file_handle_t *segment_file_handle,
     libewf_filename_t *filename,
     size_t length_filename )
{
	static char *function  = "libewf_segment_file_handle_get_filename";
	size_t filename_length = 0;

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file handle.\n",
		 function );

		return( -1 );
	}
	if( segment_file_handle->filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file handle - missing filename.\n",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid filename.\n",
		 function );

		return( -1 );
	}
	filename_length = segment_file_handle->length_filename;

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
	     segment_file_handle->filename,
	     filename_length ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set filename.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets a filename for a specific segment file handle
 * Creates a duplicate of the string
 * Returns 1 if succesful, or -1 on error
 */
int libewf_segment_file_handle_set_filename(
     libewf_segment_file_handle_t *segment_file_handle,
     const libewf_filename_t *filename,
     size_t length_filename )
{
	static char *function = "libewf_segment_file_handle_set_filename";

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file handle.\n",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid filename.\n",
		 function );

		return( -1 );
	}
	if( segment_file_handle->filename != NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: filename already set: %" PRIs_EWF_filename ".\n",
		 function, segment_file_handle->filename );

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
	segment_file_handle->filename = (libewf_filename_t *) libewf_common_alloc(
	                                                       sizeof( libewf_filename_t ) * ( length_filename + 1 ) );

	if( segment_file_handle->filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create filename.\n",
		 function );

		return( -1 );
	}
	if( libewf_filename_copy(
	     segment_file_handle->filename,
	     filename,
	     length_filename ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set filename.\n",
		 function );

		libewf_common_free(
		 segment_file_handle->filename );

		segment_file_handle->filename = NULL;

		return( -1 );
	}
	/* Make sure the string is terminated
	 */
	segment_file_handle->filename[ length_filename ] = '\0';
	segment_file_handle->length_filename             = length_filename;

	return( 1 );
}

/* Opens a segment file handle
 * Sets the filename and the file descriptor in the segment file handle struct
 * Returns 1 if successful, or -1 on error
 */
int libewf_segment_file_handle_open(
     libewf_segment_file_handle_t *segment_file_handle,
     uint8_t flags )
{
	static char *function = "libewf_segment_file_handle_open";

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file handle.\n",
		 function );

		return( -1 );
	}
	if( segment_file_handle->filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file handle - missing filename.\n",
		 function );

		return( -1 );
	}
	segment_file_handle->file_descriptor = libewf_filename_open(
	                                        segment_file_handle->filename,
	                                        flags );

	if( segment_file_handle->file_descriptor == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to open segment file: %" PRIs_EWF_filename ".\n",
		 function, segment_file_handle->filename );

		return( -1 );
	}
	return( 1 );
}

/* Re-opens a segment file handle
 * Returns 1 if successful, or -1 on error
 */
int libewf_segment_file_handle_reopen(
     libewf_segment_file_handle_t *segment_file_handle,
     uint8_t flags )
{
	static char *function = "libewf_segment_file_handle_reopen";

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file handle.\n",
		 function );

		return( -1 );
	}
	if( segment_file_handle->filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file handle - missing filename.\n",
		 function );

		return( -1 );
	}
	if( libewf_common_close( segment_file_handle->file_descriptor ) != 0 )
	{
		LIBEWF_VERBOSE_PRINT( "%s: unable to close segment file: %" PRIs_EWF_filename ".\n",
		 function, segment_file_handle->filename );
	}
	segment_file_handle->file_descriptor = libewf_filename_open(
	                                        segment_file_handle->filename,
	                                        flags );

	if( segment_file_handle->file_descriptor == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to open file: %" PRIs_EWF_filename ".\n",
		 function, segment_file_handle->filename );

		return( -1 );
	}
	/* Seek the previous file offset
	 */
	if( libewf_common_lseek(
	     segment_file_handle->file_descriptor,
	     segment_file_handle->file_offset,
	     SEEK_CUR ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to seek in file: %" PRIs_EWF_filename ".\n",
		 function, segment_file_handle->filename );

		return( -1 );
	}
	return( 1 );
}

/* Reads a buffer from a segment file handle
 * Updates the file offset
 * Returns the amount of bytes read if successful, or -1 on errror
 */
ssize_t libewf_segment_file_handle_read(
         libewf_segment_file_handle_t *segment_file_handle,
         void *buffer,
         size_t size )
{
	static char *function = "libewf_segment_file_handle_read";
	ssize_t read_count    = 0;

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file handle.\n",
		 function );

		return( -1 );
	}
	if( segment_file_handle->filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file handle - missing filename.\n",
		 function );

		return( -1 );
	}
	if( segment_file_handle->file_descriptor == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file handle - invalid file descriptor.\n",
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
	              segment_file_handle->file_descriptor,
	              buffer,
	              size );

	if( read_count > 0 )
	{
		segment_file_handle->file_offset += (off64_t) read_count;
	}
	if( read_count != (ssize_t) size )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read from segment file: %" PRIs_EWF_filename ".\n",
		 function, segment_file_handle->filename );
	}
	return( read_count );
}

/* Writes a buffer to a segment file handle
 * Updates the file offset
 * Returns the amount of bytes written if successful, or -1 on errror
 */
ssize_t libewf_segment_file_handle_write(
         libewf_segment_file_handle_t *segment_file_handle,
         void *buffer,
         size_t size )
{
	static char *function = "libewf_segment_file_handle_write";
	ssize_t write_count   = 0;

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file handle.\n",
		 function );

		return( -1 );
	}
	if( segment_file_handle->filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file handle - missing filename.\n",
		 function );

		return( -1 );
	}
	if( segment_file_handle->file_descriptor == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file handle - invalid file descriptor.\n",
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
	               segment_file_handle->file_descriptor,
	               buffer,
	               size );

	if( write_count > 0 )
	{
		segment_file_handle->file_offset += (off64_t) write_count;
	}
	if( write_count != (ssize_t) size )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write to segment file: %" PRIs_EWF_filename ".\n",
		 function, segment_file_handle->filename );
	}
	return( write_count );
}

/* Seeks a certain offset within the a segment file handle
 * Returns 1 if the seek is successful, or -1 on error
 */
off64_t libewf_segment_file_handle_seek_offset(
         libewf_segment_file_handle_t *segment_file_handle,
         off64_t offset )
{
	static char *function = "libewf_segment_file_handle_seek_offset";

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file handle.\n",
		 function );

		return( -1 );
	}
	if( segment_file_handle->file_descriptor == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file handle - invalid file descriptor.\n",
		 function );

		return( -1 );
	}
	if( segment_file_handle->filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file handle - missing filename.\n",
		 function );

		return( -1 );
	}
	if( offset > (off64_t) INT64_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid offset value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( segment_file_handle->file_offset != offset )
	{
		LIBEWF_VERBOSE_PRINT( "%s: seeking offset: %" PRIjd_EWF " in segment file: %" PRIs_EWF_filename " with file descriptor: %d.\n",
		 function, offset, segment_file_handle->filename, segment_file_handle->file_descriptor );

		if( libewf_common_lseek(
		     segment_file_handle->file_descriptor,
		     offset,
		     SEEK_SET ) == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to find offset: %" PRIjd_EWF " in segment file: %" PRIs_EWF_filename ".\n",
			 function, offset, segment_file_handle->filename );

			return( -1 );
		}
		segment_file_handle->file_offset = offset;
	}
	return( offset );
}

/* Closes a segment file handle
 * Returns 0 if successful, or -1 on error
 */
int libewf_segment_file_handle_close(
     libewf_segment_file_handle_t *segment_file_handle )
{
	static char *function = "libewf_segment_file_handle_close";

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file handle.\n",
		 function );

		return( -1 );
	}
	if( segment_file_handle->file_descriptor == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file handle - invalid file descriptor.\n",
		 function );

		return( -1 );
	}
	if( libewf_common_close( segment_file_handle->file_descriptor ) != 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to close segment file handle.\n",
		 function );

		return( -1 );
	}
	segment_file_handle->file_descriptor = -1;

	return( 0 );
}

