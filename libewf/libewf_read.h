/*
 * libewf file reading
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

#if !defined( _LIBEWF_READ_H )
#define _LIBEWF_READ_H

#include "libewf_includes.h"

/* If libtool DLL support is enabled set LIBEWF_DLL_EXPORT
 * before including libewf_extern.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBEWF_DLL_EXPORT
#endif

#include <libewf/libewf_extern.h>
#include <libewf/libewf_handle.h>

#include "libewf_internal_handle.h"

#include "ewf_char.h"
#include "ewf_crc.h"

#if defined( __cplusplus )
extern "C" {
#endif

ssize_t libewf_read_process_chunk_data( LIBEWF_INTERNAL_HANDLE *internal_handle, EWF_CHAR *chunk_data, size_t chunk_data_size, EWF_CHAR *uncompressed_chunk_data, size_t *uncompressed_chunk_data_size, int8_t is_compressed, EWF_CRC chunk_crc, int8_t read_crc );

ssize_t libewf_raw_read_chunk( LIBEWF_INTERNAL_HANDLE *internal_handle, uint32_t chunk, EWF_CHAR *chunk_buffer, size_t chunk_size, int8_t *is_compressed, EWF_CRC *chunk_crc, int8_t *read_crc );

ssize_t libewf_read_chunk_data( LIBEWF_INTERNAL_HANDLE *internal_handle, uint32_t chunk, uint32_t chunk_offset, EWF_CHAR *buffer, size_t size );

LIBEWF_EXTERN ssize_t libewf_raw_read_prepare_buffer( LIBEWF_HANDLE *handle, void *buffer, size_t buffer_size, void *uncompressed_buffer, size_t *uncompressed_buffer_size, int8_t is_compressed, uint32_t chunk_crc, int8_t read_crc );

LIBEWF_EXTERN ssize_t libewf_raw_read_buffer( LIBEWF_HANDLE *handle, void *buffer, size_t size, int8_t *is_compressed, uint32_t *chunk_crc, int8_t *read_crc );

LIBEWF_EXTERN ssize_t libewf_read_buffer( LIBEWF_HANDLE *handle, void *buffer, size_t size );
LIBEWF_EXTERN ssize_t libewf_read_random( LIBEWF_HANDLE *handle, void *buffer, size_t size, off64_t offset );

#if defined( __cplusplus )
}
#endif

#endif

