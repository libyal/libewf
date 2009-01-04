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

#ifndef _LIBEWF_SEGMENT_FILE_H
#define _LIBEWF_SEGMENT_FILE_H

#include "libewf_includes.h"

#include <libewf/libewf_handle.h>

#include "libewf_internal_handle.h"
#include "libewf_section_list.h"

#include "ewf_crc.h"

#ifdef __cplusplus
extern "C" {
#endif

int libewf_segment_file_check_file_signature( int file_descriptor );

ssize_t libewf_segment_file_read_file_header( int file_descriptor, uint16_t *segment_number );

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
int libewf_segment_file_read_wide_open( LIBEWF_INTERNAL_HANDLE *internal_handle, wchar_t * const filenames[], uint16_t file_amount, uint8_t flags );
#else
int libewf_segment_file_read_open( LIBEWF_INTERNAL_HANDLE *internal_handle, char * const filenames[], uint16_t file_amount, uint8_t flags );
#endif

/*
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
int libewf_segment_file_write_wide_open( LIBEWF_INTERNAL_HANDLE *internal_handle, wchar_t * const filenames[], uint16_t file_amount );
#else
int libewf_segment_file_write_open( LIBEWF_INTERNAL_HANDLE *internal_handle, char * const filenames[], uint16_t file_amount );
#endif
*/

int libewf_segment_file_read_segment_table( LIBEWF_INTERNAL_HANDLE *internal_handle );

int libewf_segment_file_read_sections( LIBEWF_INTERNAL_HANDLE *internal_handle, uint16_t segment_number, int file_descriptor, LIBEWF_SECTION_LIST *section_list, int *last_segment_file );

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
int libewf_segment_file_create_wide_extension( LIBEWF_INTERNAL_HANDLE *internal_handle, uint16_t segment_number, int16_t maximum_amount_of_segments, wchar_t* extension );
#else
int libewf_segment_file_create_extension( LIBEWF_INTERNAL_HANDLE *internal_handle, uint16_t segment_number, int16_t maximum_amount_of_segments, char* extension );
#endif

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
wchar_t *libewf_segment_file_create_wide_filename( LIBEWF_INTERNAL_HANDLE *internal_handle, uint16_t segment_number, int16_t maximum_amount_of_segments, wchar_t* basename );
#else
char *libewf_segment_file_create_filename( LIBEWF_INTERNAL_HANDLE *internal_handle, uint16_t segment_number, int16_t maximum_amount_of_segments, char* basename );
#endif

int libewf_segment_file_create_file_entry( LIBEWF_INTERNAL_HANDLE *internal_handle, uint16_t segment_number, int16_t maximum_amount_of_segments );

ssize_t libewf_segment_file_write_headers( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, LIBEWF_SECTION_LIST *section_list, off_t start_offset );

ssize_t libewf_segment_file_write_start( LIBEWF_INTERNAL_HANDLE *internal_handle, uint16_t segment_number, int file_descriptor, LIBEWF_SECTION_LIST *section_list );
ssize_t libewf_segment_file_write_end( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, LIBEWF_SECTION_LIST *section_list, off_t start_offset, int last_segment_file );

ssize_t libewf_segment_file_write_chunks_section_start( LIBEWF_INTERNAL_HANDLE *internal_handle, uint16_t segment_number, size32_t chunk_size, uint32_t total_chunk_amount, uint32_t segment_chunk_amount );

ssize_t libewf_segment_file_write_chunks_data( LIBEWF_INTERNAL_HANDLE *internal_handle, uint16_t segment_number, uint32_t chunk, EWF_CHUNK *chunk_data, size_t size, int8_t is_compressed, EWF_CRC *chunk_crc, int8_t write_crc, uint32_t amount_of_chunks );

ssize_t libewf_segment_file_write_chunks_correction( LIBEWF_INTERNAL_HANDLE *internal_handle, uint16_t segment_number, off_t chunks_section_offset, size_t chunks_section_size, uint32_t amount_of_chunks, uint32_t section_amount_of_chunks );

ssize_t libewf_segment_file_write_open( LIBEWF_INTERNAL_HANDLE *internal_handle, uint16_t segment_number, int16_t maximum_amount_of_segments );
ssize_t libewf_segment_file_write_close( LIBEWF_INTERNAL_HANDLE *internal_handle, uint16_t segment_number, uint32_t segment_amount_of_chunks, int last_segment_file );

#ifdef __cplusplus
}
#endif

#endif

