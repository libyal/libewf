/*
 * libewf file io handler
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

#if !defined( _LIBEWF_FILE_IO_HANDLER_H )
#define _LIBEWF_FILE_IO_HANDLER_H

#include "libewf_includes.h"
#include "libewf_char.h"

#include "libewf_section_list.h"

#if defined( __cplusplus )
extern "C" {
#endif

#define LIBEWF_FILE_IO_HANDLER				libewf_file_io_handler_t
#define LIBEWF_FILE_IO_HANDLER_SIZE			sizeof( LIBEWF_FILE_IO_HANDLER )

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
#define LIBEWF_FILE_IO_HANDLER_FILENAME_SIZE		sizeof( wchar_t* )
#else
#define LIBEWF_FILE_IO_HANDLER_FILENAME_SIZE		sizeof( char* )
#endif

#define LIBEWF_FILE_IO_HANDLER_DESCRIPTOR_SIZE		sizeof( int )
#define LIBEWF_FILE_IO_HANDLER_OFFSET_SIZE		sizeof( off64_t )
#define LIBEWF_FILE_IO_HANDLER_FLAGS_SIZE		sizeof( int )

typedef struct libewf_file_io_handler libewf_file_io_handler_t;

struct libewf_file_io_handler
{
	/* The amount of files in the handler
	 */
	size_t amount;

	/* The amount of open file descriptors
	 */
	size_t open_files;

	/* A dynamic array containting the filenames
	 */
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	wchar_t **filename;
#else
	char **filename;
#endif

	/* A dynamic array containing the file descriptors
	 */
	int *descriptor;

	/* A dynamic array containing the file offsets
	 */
	off64_t *offset;

	/* A dynamic array containing the file flags
	 */
	int *flags;
};

LIBEWF_FILE_IO_HANDLER *libewf_file_io_handler_alloc( size_t amount );
int libewf_file_io_handler_realloc( LIBEWF_FILE_IO_HANDLER *handler, size_t amount );
void libewf_file_io_handler_free( LIBEWF_FILE_IO_HANDLER *handler );

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
int libewf_file_io_handler_get_wide_filename( LIBEWF_FILE_IO_HANDLER *handler, size_t entry, wchar_t *filename, size_t length_filename );
#else
int libewf_file_io_handler_get_filename( LIBEWF_FILE_IO_HANDLER *handler, size_t entry, char *filename, size_t length_filename );
#endif

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
int libewf_file_io_handler_set_wide_filename( LIBEWF_FILE_IO_HANDLER *handler, size_t entry, const wchar_t *filename, size_t length_filename );
#else
int libewf_file_io_handler_set_filename( LIBEWF_FILE_IO_HANDLER *handler, size_t entry, const char *filename, size_t length_filename );
#endif

int libewf_file_io_handler_open( LIBEWF_FILE_IO_HANDLER *handler, size_t entry );
ssize_t libewf_file_io_handler_read( LIBEWF_FILE_IO_HANDLER *handler, size_t entry, uint8_t *buffer, size_t size );
ssize_t libewf_file_io_handler_write( LIBEWF_FILE_IO_HANDLER *handler, size_t entry, uint8_t *buffer, size_t size );
off64_t libewf_file_io_handler_seek( LIBEWF_FILE_IO_HANDLER *handler, size_t entry, off64_t offset, int whence );
int libewf_file_io_handler_close( LIBEWF_FILE_IO_HANDLER *handler, size_t entry );

#if defined( __cplusplus )
}
#endif

#endif

