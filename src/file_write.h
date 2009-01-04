/*
 * libewf file writing
 *
 * Copyright (c) 2006, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
 *
 * This code is derrived from information and software contributed by
 * - Expert Witness Compression Format specification by Andrew Rosen
 *   (http://www.arsdata.com/SMART/whitepaper.html)
 * - libevf from PyFlag by Michael Cohen
 *   (http://pyflag.sourceforge.net/)
 * - Open SSL for the implementation of the MD5 hash algorithm
 * - Wietse Venema for error handling code
 *
 * Additional credits go to
 * - Robert Jan Mora for testing and other contribution
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
 *   must acknowledge the contribution by people stated above.
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

#ifndef _LIBEWF_FILE_WRITE_H
#define _LIBEWF_FILE_WRITE_H

#include "definitions.h"

#include "ewf_header.h"
#include "handle.h"
#include "header_values.h"

#ifdef __cplusplus
extern "C" {
#endif

int test_empty_block( uint8_t *block_buffer, uint64_t block_size );

int64_t libewf_section_write( LIBEWF_HANDLE *handle, int file_descriptor, char *section_type, uint64_t section_size, uint64_t start_offset );
int64_t libewf_last_section_write( LIBEWF_HANDLE *handle, int file_descriptor, char *section_type, uint64_t start_offset );
int64_t libewf_section_header_write( LIBEWF_HANDLE *handle, int file_descriptor, uint64_t start_offset, EWF_HEADER *header, uint32_t size, int8_t compression_level );
int64_t libewf_section_header2_write( LIBEWF_HANDLE *handle, int file_descriptor, uint64_t start_offset, EWF_HEADER *header, uint32_t size, int8_t compression_level );
int64_t libewf_section_volume_write( LIBEWF_HANDLE *handle, int file_descriptor, uint64_t start_offset );
int64_t libewf_section_volume_smart_write( LIBEWF_HANDLE *handle, int file_descriptor, uint64_t start_offset );
int64_t libewf_section_table_write( LIBEWF_HANDLE *handle, int file_descriptor, uint64_t start_offset, EWF_TABLE_OFFSET *offsets, uint32_t offsets_amount, char *section_header, uint64_t additional_size );
int64_t libewf_section_data_write( LIBEWF_HANDLE *handle, int file_descriptor, uint64_t start_offset );
int64_t libewf_section_hash_write( LIBEWF_HANDLE *handle, int file_descriptor, uint64_t start_offset, EWF_MD5HASH *md5hash );

EWF_HEADER *libewf_header_string_encase3( char **header_values );
EWF_HEADER *libewf_header_string_encase4( char **header_values );
EWF_HEADER *libewf_header2_string_encase5( char **header_values );
EWF_HEADER *libewf_header_string_ftk( char **header_values );
void libewf_headers_create( LIBEWF_HANDLE *handle, LIBEWF_HEADER_VALUES *header_values );
int64_t libewf_headers_write( LIBEWF_HANDLE *handle, int file_descriptor, uint64_t start_offset );

LIBEWF_HANDLE *libewf_set_write_parameters( LIBEWF_HANDLE *handle, uint64_t input_file_size, uint32_t sectors_per_chunk, uint32_t bytes_per_sector, uint32_t error_granularity_sectors, uint64_t ewf_file_size, int8_t compression_level, uint8_t format, LIBEWF_HEADER_VALUES *header_values, uint8_t read_error_retry, uint8_t wipe_block_on_read_error, uint8_t compress_empty_block );

int64_t libewf_read_chunk_data( int input_file_descriptor, uint8_t *buffer, uint32_t bytes_to_read );
int64_t libewf_write_from_file_descriptor( LIBEWF_HANDLE *handle, int input_file_descriptor, void (*callback)( uint64_t bytes_read, uint64_t bytes_total ) );

#ifdef __cplusplus
}
#endif

#endif

