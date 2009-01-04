/*
 * libewf segment file handle
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

#if !defined( _LIBEWF_SEGMENT_FILE_HANDLE_H )
#define _LIBEWF_SEGMENT_FILE_HANDLE_H

#include "libewf_includes.h"

#include "libewf_filename.h"
#include "libewf_section_list.h"

#if defined( __cplusplus )
extern "C" {
#endif

#define LIBEWF_SEGMENT_FILE_HANDLE		libewf_segment_file_handle_t
#define LIBEWF_SEGMENT_FILE_HANDLE_SIZE		sizeof( LIBEWF_SEGMENT_FILE_HANDLE )

typedef struct libewf_segment_file_handle libewf_segment_file_handle_t;

struct libewf_segment_file_handle
{
	/* The filename
	 */
	LIBEWF_FILENAME *filename;

	/* The filename length
	 */
	size_t length_filename;

	/* The file descriptor
	 */
	int file_descriptor;

	/* The file offset
	 */
	off64_t file_offset;

	/* The amount of chunks
	 */
	uint32_t amount_of_chunks;

        /* The list of all the sections
         */
        LIBEWF_SECTION_LIST *section_list;
};

LIBEWF_SEGMENT_FILE_HANDLE *libewf_segment_file_handle_alloc( void );
void libewf_segment_file_handle_free( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle );

int libewf_segment_file_handle_get_filename( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, LIBEWF_FILENAME *filename, size_t length_filename );
int libewf_segment_file_handle_set_filename( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, const LIBEWF_FILENAME *filename, size_t length_filename );

int libewf_segment_file_handle_open( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, uint8_t flags );
int libewf_segment_file_handle_reopen( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, uint8_t flags );
ssize_t libewf_segment_file_handle_read( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, void *buffer, size_t size );
ssize_t libewf_segment_file_handle_write( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, void *buffer, size_t size );
off64_t libewf_segment_file_handle_seek_offset( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, off64_t offset );
int libewf_segment_file_handle_close( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle );

#if defined( __cplusplus )
}
#endif

#endif

