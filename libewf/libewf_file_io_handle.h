/*
 * libewf file io handle
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

#if !defined( _LIBEWF_FILE_IO_HANDLE_H )
#define _LIBEWF_FILE_IO_HANDLE_H

#include "libewf_includes.h"

#if defined( __cplusplus )
extern "C" {
#endif

#define LIBEWF_FILE_IO_HANDLE		libewf_file_io_handle_t
#define LIBEWF_FILE_IO_HANDLE_SIZE	sizeof( LIBEWF_FILE_IO_HANDLE )

typedef struct libewf_file_io_handle libewf_file_io_handle_t;

struct libewf_file_io_handle
{
	/* The filename
	 */
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	wchar_t *filename;
#else
	char *filename;
#endif

	/* The file descriptor
	 */
	int file_descriptor;

	/* The current file offsets
	 */
	off64_t file_offset;

	/* The file flags
	 */
	int flags;
};

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
int libewf_file_io_handle_get_wide_filename( LIBEWF_FILE_IO_HANDLE *file_io_handle, wchar_t *filename, size_t length_filename );
#else
int libewf_file_io_handle_get_filename( LIBEWF_FILE_IO_HANDLE *file_io_handle, char *filename, size_t length_filename );
#endif

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
int libewf_file_io_handle_set_wide_filename( LIBEWF_FILE_IO_HANDLE *file_io_handle, const wchar_t *filename, size_t length_filename );
#else
int libewf_file_io_handle_set_filename( LIBEWF_FILE_IO_HANDLE *file_io_handle, const char *filename, size_t length_filename );
#endif

#if defined( __cplusplus )
}
#endif

#endif

