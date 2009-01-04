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

/* If libtool DLL support is enabled set LIBEWF_DLL_EXPORT
 * before including libewf_extern.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBEWF_DLL_EXPORT
#endif

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
int libewf_write_test_segment_file_full( LIBEWF_INTERNAL_HANDLE *internal_handle, off_t segment_file_offset );
int libewf_write_test_chunks_section_full( LIBEWF_INTERNAL_HANDLE *internal_handle, off_t segment_file_offset );

ssize_t libewf_write_prepare_chunk_data( LIBEWF_INTERNAL_HANDLE *internal_handle, EWF_CHUNK *chunk_data, size_t chunk_data_size, EWF_CHUNK *compressed_chunk_data, size_t *compressed_chunk_data_size, int8_t *is_compressed, EWF_CRC *chunk_crc, int8_t *write_crc );
ssize_t libewf_write_process_chunk_data( LIBEWF_INTERNAL_HANDLE *internal_handle, EWF_CHUNK **chunk_data, size_t chunk_data_size, int8_t *is_compressed, EWF_CRC *chunk_crc, int8_t *write_crc );

ssize_t libewf_write_chunk( LIBEWF_INTERNAL_HANDLE *internal_handle, int8_t raw_access, uint32_t chunk, uint32_t chunk_offset, void *buffer, size_t size, size_t uncompressed_size, int8_t is_compressed, EWF_CRC chunk_crc, int8_t write_crc, int8_t force_write );
ssize_t libewf_write_chunk_data( LIBEWF_INTERNAL_HANDLE *internal_handle, int8_t raw_access, void *buffer, size_t size, size_t uncompressed_size, int8_t is_compressed, uint32_t chunk_crc, int8_t write_crc );

LIBEWF_EXTERN ssize_t libewf_raw_write_prepare_buffer( LIBEWF_HANDLE *handle, void *buffer, size_t buffer_size, void *compressed_buffer, size_t *compressed_buffer_size, int8_t *is_compressed, uint32_t *chunk_crc, int8_t *write_crc );

LIBEWF_EXTERN ssize_t libewf_raw_write_buffer( LIBEWF_HANDLE *handle, void *buffer, size_t size, size_t uncompressed_size, int8_t is_compressed, uint32_t chunk_crc, int8_t write_crc );

LIBEWF_EXTERN ssize_t libewf_write_buffer( LIBEWF_HANDLE *handle, void *buffer, size_t size );
LIBEWF_EXTERN ssize_t libewf_write_random( LIBEWF_HANDLE *handle, void *buffer, size_t size, off64_t offset );

LIBEWF_EXTERN ssize_t libewf_write_finalize( LIBEWF_HANDLE *handle );

#ifdef __cplusplus
}
#endif

#endif

