/*
 * libewf file writing
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
 * - All advertising materials mentioning features or use of this software
 *   must acknowledge the contribution by people stated in the acknowledgements.
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

#ifndef _LIBEWF_WRITE_H
#define _LIBEWF_WRITE_H

#include "libewf_includes.h"

#include <libewf/libewf_extern.h>
#include <libewf/libewf_handle.h>

#include "libewf_char.h"
#include "libewf_internal_handle.h"
#include "libewf_section_list.h"

#ifdef __cplusplus
extern "C" {
#endif

uint32_t libewf_write_calculate_chunks_per_segment( LIBEWF_INTERNAL_HANDLE *internal_handle, uint16_t segment_number );
uint32_t libewf_write_calculate_chunks_per_chunks_section( LIBEWF_INTERNAL_HANDLE *internal_handle );
int8_t libewf_write_test_segment_file_full( LIBEWF_INTERNAL_HANDLE *internal_handle, off_t segment_file_offset );
int8_t libewf_write_test_chunks_section_full( LIBEWF_INTERNAL_HANDLE *internal_handle, off_t segment_file_offset );

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
int8_t libewf_write_determine_wide_segment_file_extension( LIBEWF_INTERNAL_HANDLE *internal_handle, wchar_t* extension, uint16_t segment );
#else
int8_t libewf_write_determine_segment_file_extension( LIBEWF_INTERNAL_HANDLE *internal_handle, char* extension, uint16_t segment );
#endif

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
wchar_t *libewf_write_create_wide_segment_filename( LIBEWF_INTERNAL_HANDLE *internal_handle, wchar_t* basename, uint16_t segment );
#else
char *libewf_write_create_segment_filename( LIBEWF_INTERNAL_HANDLE *internal_handle, char* basename, uint16_t segment );
#endif

int8_t libewf_write_create_segment_file_entry( LIBEWF_INTERNAL_HANDLE *internal_handle, uint16_t segment );

ssize_t libewf_write_headers( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, off_t start_offset, LIBEWF_SECTION_LIST *section_list );

ssize_t libewf_write_segment_file_start( LIBEWF_INTERNAL_HANDLE *internal_handle, uint16_t segment_number, int file_descriptor, LIBEWF_SECTION_LIST *section_list );
ssize_t libewf_write_segment_file_chunks_section_start( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, off_t start_offset, uint32_t total_chunk_amount, uint32_t segment_chunk_amount );
ssize_t libewf_write_segment_file_chunk( LIBEWF_INTERNAL_HANDLE *internal_handle, uint16_t segment_number, int file_descriptor, EWF_CHUNK *chunk_data, off_t start_offset, size_t size, uint32_t chunk );
ssize_t libewf_write_segment_file_chunks_section_correction( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, off_t start_offset, LIBEWF_SECTION_LIST *section_list, off_t chunks_section_offset, size_t chunks_section_size );
ssize_t libewf_write_segment_file_end( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_SECTION_LIST *section_list, int file_descriptor, off_t start_offset, uint8_t last_segment_file );

ssize_t libewf_write_segment_file_open( LIBEWF_INTERNAL_HANDLE *internal_handle, uint16_t segment_number );
ssize_t libewf_write_segment_file_close( LIBEWF_INTERNAL_HANDLE *internal_handle, uint16_t segment_number, uint8_t last_segment_file );

ssize_t libewf_write_chunk( LIBEWF_INTERNAL_HANDLE *internal_handle, uint32_t chunk, uint32_t chunk_offset, void *buffer, size_t size, uint8_t force_write );

LIBEWF_EXTERN ssize_t libewf_write_buffer( LIBEWF_HANDLE *handle, void *buffer, size_t size );
LIBEWF_EXTERN ssize_t libewf_write_random( LIBEWF_HANDLE *handle, void *buffer, size_t size, off_t offset );

LIBEWF_EXTERN ssize_t libewf_write_finalize( LIBEWF_HANDLE *handle );

#ifdef __cplusplus
}
#endif

#endif

