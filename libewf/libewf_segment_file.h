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

#if !defined( _LIBEWF_SEGMENT_FILE_H )
#define _LIBEWF_SEGMENT_FILE_H

#include "libewf_includes.h"

#include <libewf/libewf_handle.h>

#include "libewf_internal_handle.h"
#include "libewf_section_list.h"

#include "ewf_crc.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libewf_segment_file_check_file_signature( int file_descriptor );

LIBEWF_SEGMENT_FILE *libewf_segment_file_alloc( void );
int libewf_segment_file_open( LIBEWF_SEGMENT_FILE *segment_file, uint8_t flags );
int libewf_segment_file_reopen( LIBEWF_SEGMENT_FILE *segment_file, uint8_t flags );
ssize_t libewf_segment_file_read( LIBEWF_SEGMENT_FILE *segment_file, void *buffer, size_t size );
ssize_t libewf_segment_file_write( LIBEWF_SEGMENT_FILE *segment_file, void *buffer, size_t size );
off64_t libewf_segment_file_seek_offset( LIBEWF_SEGMENT_FILE *segment_file, off64_t offset );

ssize_t libewf_segment_file_read_file_header( LIBEWF_SEGMENT_FILE *segment_file, uint16_t *segment_number, uint8_t *segment_file_type );

int libewf_segment_file_create( LIBEWF_SEGMENT_TABLE *segment_table, uint16_t segment_number, int16_t maximum_amount_of_segments, uint8_t segment_file_type, uint8_t ewf_format, uint8_t format );

int libewf_segment_file_build_segment_table( LIBEWF_INTERNAL_HANDLE *internal_handle );
int libewf_segment_file_build_delta_segment_table( LIBEWF_INTERNAL_HANDLE *internal_handle );

int libewf_segment_file_read_sections( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_SEGMENT_FILE *segment_file, int *last_segment_file );

ssize_t libewf_segment_file_write_headers( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_SEGMENT_FILE *segment_file,EWF_CHAR *header, size_t header_size, EWF_CHAR *header2, size_t header2_size, EWF_CHAR *xheader, size_t xheader_size, int8_t compression_level, uint8_t format );
ssize_t libewf_segment_file_write_last_section( LIBEWF_SEGMENT_FILE *segment_file, int last_segment_file, uint8_t format, uint8_t ewf_format );

ssize_t libewf_segment_file_write_start( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_SEGMENT_FILE *segment_file, uint16_t segment_number, uint8_t segment_file_type, LIBEWF_MEDIA_VALUES *media_values, int8_t compression_level, uint8_t format, uint8_t ewf_format, EWF_DATA **cached_data_section );

ssize_t libewf_segment_file_write_chunks_section_start( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_SEGMENT_FILE *segment_file, size32_t chunk_size, uint32_t total_chunk_amount, uint32_t segment_chunk_amount );

ssize_t libewf_segment_file_write_chunks_data( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_SEGMENT_FILE *segment_file, uint32_t chunk, EWF_CHAR *chunk_data, size_t size, int8_t is_compressed, EWF_CRC *chunk_crc, int8_t write_crc );

ssize_t libewf_segment_file_write_chunks_correction( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_SEGMENT_FILE *segment_file, off64_t chunks_section_offset, size64_t chunks_section_size, uint32_t amount_of_chunks, uint32_t section_amount_of_chunks );

ssize_t libewf_segment_file_write_delta_chunk( LIBEWF_SEGMENT_FILE *segment_file, uint32_t chunk, EWF_CHAR *chunk_data, size_t chunk_size, EWF_CRC *chunk_crc, uint8_t write_crc );

ssize_t libewf_segment_file_write_close( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_SEGMENT_FILE *segment_file, uint16_t segment_number, uint32_t segment_amount_of_chunks, int last_segment_file );

int libewf_segment_file_get_filename( LIBEWF_SEGMENT_FILE *segment_file, LIBEWF_FILENAME *filename, size_t length_filename );
int libewf_segment_file_set_filename( LIBEWF_SEGMENT_FILE *segment_file, const LIBEWF_FILENAME *filename, size_t length_filename );

int libewf_segment_file_read_open( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_FILENAME * const filenames[], uint16_t file_amount, uint8_t flags );
int libewf_segment_file_write_open( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_FILENAME * const filenames[], uint16_t file_amount );

off64_t libewf_segment_file_seek_chunk_offset( LIBEWF_INTERNAL_HANDLE *internal_handle, uint32_t chunk );

int libewf_segment_file_close_all( LIBEWF_INTERNAL_HANDLE *internal_handle );

#if defined( __cplusplus )
}
#endif

#endif

