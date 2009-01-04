/*
 * libewf file handling
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

#include <common.h>
#include <character_string.h>
#include <file_io.h>
#include <memory.h>
#include <notify.h>
#include <types.h>

#include <libewf/definitions.h>

#include "libewf_file.h"
#include "libewf_filename.h"
#include "libewf_offset_table.h"
#include "libewf_read.h"
#include "libewf_section_list.h"
#include "libewf_segment_file.h"
#include "libewf_segment_file_handle.h"
#include "libewf_segment_table.h"
#include "libewf_write.h"

#include "ewf_definitions.h"

/* Detects if a file is an EWF file (check for the EWF file signature)
 * Returns 1 if true, 0 if not or -1 on error
 */
int libewf_check_file_signature(
     const system_character_t *filename )
{
	static char *function = "libewf_check_file_signature";
	int file_descriptor   = 0;
	int result            = 0;

	if( filename == NULL )
	{
		notify_warning_printf( "%s: invalid filename.\n",
		 function );

		return( -1 );
	}
	file_descriptor = libewf_filename_open(
	                   filename,
	                   LIBEWF_OPEN_READ );

	if( file_descriptor == -1 )
	{
		notify_warning_printf( "%s: unable to open file: %" PRIs_SYSTEM ".\n",
		 function, filename );

		return( -1 );
	}
	result = libewf_segment_file_check_file_signature(
	          file_descriptor );

	if( file_io_close(
	     file_descriptor ) != 0 )
	{
		notify_warning_printf( "%s: unable to close file: %" PRIs_SYSTEM ".\n",
		 function, filename );

		return( -1 );
	}
	if( result <= -1 )
	{
		notify_warning_printf( "%s: unable to read signature from file: %" PRIs_SYSTEM ".\n",
		 function, filename );

		return( -1 );
	}
	return( result );
}

/* Globs the segment files according to the EWF naming schema
 * if format is known the filename should contain the base of the filename
 * otherwise the function will try to determine the format based on the extension
 * Returns the amount of filenames if successful or -1 on error
 */
int libewf_glob(
     const system_character_t *filename,
     size_t length,
     uint8_t format,
     system_character_t **filenames[] )
{
	system_character_t *segment_filename = NULL;
	void *reallocation                  = NULL;
	static char *function               = "libewf_glob";
	size_t additional_length            = 4;
	int amount_of_files                 = 0;
	int file_descriptor                 = 0;
	uint8_t segment_file_type           = 0;
	uint8_t ewf_format                  = 0;

	if( filename == NULL )
	{
		notify_warning_printf( "%s: invalid filename.\n",
		 function );

		return( -1 );
	}
	if( ( length == 0 )
	 || ( length > (size_t) SSIZE_MAX ) )
	{
		notify_warning_printf( "%s: invalid filename length.\n",
		 function );

		return( -1 );
	}
	if( ( format != LIBEWF_FORMAT_ENCASE1 )
	 && ( format != LIBEWF_FORMAT_ENCASE2 )
	 && ( format != LIBEWF_FORMAT_ENCASE3 )
	 && ( format != LIBEWF_FORMAT_ENCASE4 )
	 && ( format != LIBEWF_FORMAT_ENCASE5 )
	 && ( format != LIBEWF_FORMAT_ENCASE6 )
	 && ( format != LIBEWF_FORMAT_LINEN5 )
	 && ( format != LIBEWF_FORMAT_LINEN6 )
	 && ( format != LIBEWF_FORMAT_SMART )
	 && ( format != LIBEWF_FORMAT_FTK )
	 && ( format != LIBEWF_FORMAT_LVF )
	 && ( format != LIBEWF_FORMAT_EWF )
	 && ( format != LIBEWF_FORMAT_EWFX ) )
	{
		format = LIBEWF_FORMAT_UNKNOWN;
	}
	if( filenames == NULL )
	{
		notify_warning_printf( "%s: invalid filenames.\n",
		 function );

		return( -1 );
	}
	if( format == LIBEWF_FORMAT_UNKNOWN )
	{
		if( filename[ length - 4 ] != (system_character_t) '.' )
		{
			notify_warning_printf( "%s: invalid filename - missing extension.\n",
			 function );

			return( -1 );
		}
		additional_length = 0;

		if( filename[ length - 3 ] != (system_character_t) 'E' )
		{
			format = LIBEWF_FORMAT_ENCASE5;
		}
		else if( filename[ length - 3 ] != (system_character_t) 'e' )
		{
			format = LIBEWF_FORMAT_EWF;
		}
		else if( filename[ length - 3 ] != (system_character_t) 'L' )
		{
			format = LIBEWF_FORMAT_LVF;
		}
		else if( filename[ length - 3 ] != (system_character_t) 's' )
		{
			format = LIBEWF_FORMAT_SMART;
		}
		else
		{
			notify_warning_printf( "%s: invalid filename - unsupported extension: .\n",
			 function, &( filename[ length - 4 ] ) );

			return( -1 );
		}
	}
	if( format == LIBEWF_FORMAT_LVF )
	{
		segment_file_type = LIBEWF_SEGMENT_FILE_TYPE_LWF;
		ewf_format        = EWF_FORMAT_L01;
	}
	else if( format == LIBEWF_FORMAT_SMART )
	{
		segment_file_type = LIBEWF_SEGMENT_FILE_TYPE_EWF;
		ewf_format        = EWF_FORMAT_S01;
	}
	else
	{
		segment_file_type = LIBEWF_SEGMENT_FILE_TYPE_EWF;
		ewf_format        = EWF_FORMAT_E01;
	}
	while( amount_of_files < (int) UINT16_MAX )
	{
		segment_filename = memory_allocate(
			            sizeof( system_character_t ) * ( length + additional_length + 1 ) );

		if( segment_filename == NULL )
		{
			notify_warning_printf( "%s: unable to create segment filename.\n",
			 function );

			return( -1 );
		}
		if( memory_copy(
		     segment_filename,
		     filename,
		     length ) == NULL )
		{
			notify_warning_printf( "%s: unable to copy filename.\n",
			 function );

			memory_free(
			 segment_filename );

			return( -1 );
		}
		if( additional_length > 0 )
		{
			segment_filename[ length ] = (character_t) '.';
		}
		if( libewf_filename_set_extension(
		     &( segment_filename[ length + additional_length - 3 ] ),
		     (uint16_t) ( amount_of_files + 1 ),
		     UINT16_MAX,
		     segment_file_type,
		     format,
		     ewf_format ) != 1 )
		{
			notify_warning_printf( "%s: unable to set extension.\n",
			 function );

			memory_free(
			 segment_filename );

			return( -1 );
		}
		segment_filename[ length + additional_length ] = (character_t) '\0';

		file_descriptor = libewf_filename_open(
		                   filename,
		                   LIBEWF_OPEN_READ );

		if( file_descriptor == -1 )
		{
			memory_free(
			 segment_filename );

			break;
		}
		amount_of_files++;

		file_io_close(
		 file_descriptor );

		reallocation = memory_reallocate(
		                *filenames,
		                sizeof( system_character_t * ) * amount_of_files );

		if( reallocation == NULL )
		{
			notify_warning_printf( "%s: unable to reallocate filenames.\n",
			 function );

			memory_free(
			 segment_filename );

			return( -1 );
		}
		*filenames = (system_character_t **) reallocation;

		( *filenames )[ amount_of_files - 1 ] = segment_filename;
	}
	return( amount_of_files );
}

/* Opens a set of EWF file(s)
 * For reading files should contain all filenames that make up an EWF image
 * For writing files should contain the base of the filename, extentions like .e01 will be automatically added
 * Returns a pointer to the new instance of handle, NULL on error
 */
libewf_handle_t *libewf_open(
                system_character_t * const filenames[],
                uint16_t amount_of_files,
                uint8_t flags )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_open";
	size64_t *segment_file_size               = NULL;

	if( filenames == NULL )
	{
		notify_warning_printf( "%s: invalid filenames.\n",
		 function );

		return( NULL );
	}
	if( amount_of_files < 1 )
	{
		notify_warning_printf( "%s: invalid file amount at least 1 is required.\n",
		 function );

		return( NULL );
	}
	if( ( ( flags & LIBEWF_FLAG_READ ) != LIBEWF_FLAG_READ )
	 && ( ( flags & LIBEWF_FLAG_WRITE ) != LIBEWF_FLAG_WRITE ) )
	{
		notify_warning_printf( "%s: unsupported flags.\n",
		 function );

		return( NULL );
	}
	internal_handle = libewf_internal_handle_alloc(
	                   flags );

	if( internal_handle == NULL )
	{
		notify_warning_printf( "%s: unable to create handle.\n",
		 function );

		return( NULL );
	}
	if( ( flags & LIBEWF_FLAG_READ ) == LIBEWF_FLAG_READ )
	{
		if( internal_handle->write != NULL )
		{
			segment_file_size = &( internal_handle->write->segment_file_size );
		}
		if( libewf_segment_table_read_open(
		     internal_handle->segment_table, 
		     internal_handle->delta_segment_table, 
		     filenames, 
		     amount_of_files,
		     flags,
		     internal_handle->header_sections,
		     internal_handle->hash_sections,
		     internal_handle->media_values,
		     internal_handle->offset_table,
		     internal_handle->secondary_offset_table,
		     internal_handle->sessions,
		     internal_handle->acquiry_errors,
		     &( internal_handle->compression_level ),
		     &( internal_handle->format ),
		     &( internal_handle->ewf_format ),
		     segment_file_size,
		     internal_handle->error_tollerance,
		     &( internal_handle->abort ) ) != 1 )
		{
			notify_warning_printf( "%s: unable to open segment file(s).\n",
			 function );

			libewf_internal_handle_free( internal_handle );

			return( NULL );
		}
		/* Determine the EWF format
		 */
		if( libewf_header_sections_determine_format(
		     internal_handle->header_sections,
		     internal_handle->ewf_format,
		     &( internal_handle->format ) ) != 1 )
		{
			notify_warning_printf( "%s: unable to determine format.\n",
			 function );
		}
		/* Calculate the media size
		 */
		internal_handle->media_values->media_size = (size64_t) internal_handle->media_values->amount_of_sectors
		                                          * (size64_t) internal_handle->media_values->bytes_per_sector;
	}
	else if( ( flags & LIBEWF_FLAG_WRITE ) == LIBEWF_FLAG_WRITE )
	{
		if( libewf_segment_table_write_open(
		     internal_handle->segment_table, 
		     filenames, 
		     amount_of_files ) != 1 )
		{
			notify_warning_printf( "%s: unable to open segment file(s).\n",
			 function );

			libewf_internal_handle_free( internal_handle );

			return( NULL );
		}
	}
	/* Make sure format specific values are set
	 */
	if( libewf_internal_handle_initialize_format(
	     internal_handle ) != 1 )
	{
		notify_warning_printf( "%s: unable to initialize format specific values.\n",
		 function );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	notify_verbose_printf( "%s: open successful.\n",
	 function );
#endif

	return( (libewf_handle_t *) internal_handle );
}

/* Closes the EWF handle and frees memory used within the handle
 * Returns 0 if successful or -1 on error
 */
int libewf_close(
     libewf_handle_t *handle )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_close";

	if( handle == NULL )
	{
		notify_warning_printf( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( ( internal_handle->write != NULL )
	 && ( internal_handle->write->write_finalized == 0 ) )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		notify_verbose_printf( "%s: finalizing write.\n",
		 function );
#endif

		libewf_write_finalize(
		 handle );
	}
	if( libewf_segment_table_close_all(
	     internal_handle->segment_table ) != 1 )
	{
		notify_warning_printf( "%s: unable to close all segment files in segment table.\n",
		 function );

		return( -1 );
	}
	if( libewf_segment_table_close_all(
	     internal_handle->delta_segment_table ) != 1 )
	{
		notify_warning_printf( "%s: unable to close all segment files in delta segment table.\n",
		 function );

		return( -1 );
	}
	libewf_internal_handle_free(
	 internal_handle );

	return( 0 );
}

/* Seeks a certain offset of the media data within the EWF file(s)
 * It will set the related file offset to the specific chunk offset
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libewf_seek_offset(
     libewf_handle_t *handle,
     off64_t offset )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_seek_offset";
	uint64_t chunk                            = 0;
	uint64_t chunk_offset                     = 0;

	if( handle == NULL )
	{
		notify_warning_printf( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		notify_warning_printf( "%s: invalid handle - missing media values.\n",
		 function );

		return( -1 );
	}
	if( offset <= -1 )
	{
		notify_warning_printf( "%s: invalid offset value cannot be negative.\n",
		 function );

		return( -1 );
	}
	if( offset >= (off64_t) internal_handle->media_values->media_size )
	{
		notify_warning_printf( "%s: attempting to read past the end of the file.\n",
		 function );

		return( -1 );
	}
	/* Determine the chunk that is requested
	 */
	chunk = offset / internal_handle->media_values->chunk_size;

	if( chunk >= (uint64_t) INT32_MAX )
	{
		notify_warning_printf( "%s: invalid chunk value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( libewf_offset_table_seek_chunk_offset(
	     internal_handle->offset_table,
	     (uint32_t) chunk ) == -1 )
	{
		notify_warning_printf( "%s: unable to seek chunk offset.\n",
		 function );

		return( -1 );
	}
	internal_handle->current_chunk = (uint32_t) chunk;

	/* Determine the offset within the decompressed chunk that is requested
	 */
	chunk_offset = offset % internal_handle->media_values->chunk_size;

	if( chunk_offset >= (uint64_t) INT32_MAX )
	{
		notify_warning_printf( "%s: invalid chunk offset value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	internal_handle->current_chunk_offset = (uint32_t) chunk_offset;

	return( offset );
}

