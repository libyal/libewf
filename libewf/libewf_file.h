/*
 * libewf file handling
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

#ifndef _LIBEWF_FILE_H
#define _LIBEWF_FILE_H

#include "libewf_includes.h"

#include <stdio.h>

#include <libewf/libewf_extern.h>
#include <libewf/libewf_handle.h>

#include "libewf_char.h"
#include "libewf_internal_handle.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBEWF_EXTERN const LIBEWF_CHAR *libewf_get_version( void );

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
LIBEWF_EXTERN int8_t libewf_check_file_signature( const wchar_t *filename );
#else
LIBEWF_EXTERN int8_t libewf_check_file_signature( const char *filename );
#endif

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
LIBEWF_EXTERN LIBEWF_HANDLE *libewf_open( wchar_t * const filenames[], uint16_t file_amount, uint8_t flags );
#else
LIBEWF_EXTERN LIBEWF_HANDLE *libewf_open( char * const filenames[], uint16_t file_amount, uint8_t flags );
#endif

LIBEWF_EXTERN int8_t libewf_close( LIBEWF_HANDLE *handle );

off_t libewf_seek_chunk( LIBEWF_INTERNAL_HANDLE *internal_handle, uint32_t chunk );

LIBEWF_EXTERN off_t libewf_seek_offset( LIBEWF_HANDLE *handle, off_t offset );

LIBEWF_EXTERN int32_t libewf_get_bytes_per_sector( LIBEWF_HANDLE *handle );
LIBEWF_EXTERN int32_t libewf_get_amount_of_sectors( LIBEWF_HANDLE *handle );
LIBEWF_EXTERN int32_t libewf_get_chunk_size( LIBEWF_HANDLE *handle );
LIBEWF_EXTERN int32_t libewf_get_error_granularity( LIBEWF_HANDLE *handle );
LIBEWF_EXTERN int8_t libewf_get_compression_level( LIBEWF_HANDLE *handle );
LIBEWF_EXTERN int64_t libewf_get_media_size( LIBEWF_HANDLE *handle );
LIBEWF_EXTERN int8_t libewf_get_media_type( LIBEWF_HANDLE *handle );
LIBEWF_EXTERN int8_t libewf_get_media_flags( LIBEWF_HANDLE *handle );
LIBEWF_EXTERN int8_t libewf_get_volume_type( LIBEWF_HANDLE *handle );
LIBEWF_EXTERN int8_t libewf_get_format( LIBEWF_HANDLE *handle );

LIBEWF_EXTERN int8_t libewf_get_guid( LIBEWF_HANDLE *handle, uint8_t *guid, size_t size );

LIBEWF_EXTERN int64_t libewf_get_write_amount_of_chunks( LIBEWF_HANDLE *handle );

LIBEWF_EXTERN int8_t libewf_get_header_value( LIBEWF_HANDLE *handle, LIBEWF_CHAR *identifier, LIBEWF_CHAR *value, size_t length );
LIBEWF_EXTERN int8_t libewf_get_hash_value( LIBEWF_HANDLE *handle, LIBEWF_CHAR *identifier, LIBEWF_CHAR *value, size_t length );

LIBEWF_EXTERN int8_t libewf_set_media_values( LIBEWF_HANDLE *handle, uint32_t sectors_per_chunk, uint32_t bytes_per_sector );

LIBEWF_EXTERN int8_t libewf_set_guid( LIBEWF_HANDLE *handle, uint8_t *guid, size_t size );

LIBEWF_EXTERN int8_t libewf_set_write_segment_file_size( LIBEWF_HANDLE *handle, uint32_t segment_file_size );
LIBEWF_EXTERN int8_t libewf_set_write_error_granularity( LIBEWF_HANDLE *handle, uint32_t error_granularity );
LIBEWF_EXTERN int8_t libewf_set_write_compression_values( LIBEWF_HANDLE *handle, int8_t compression_level, uint8_t compress_empty_block );
LIBEWF_EXTERN int8_t libewf_set_write_media_type( LIBEWF_HANDLE *handle, uint8_t media_type, uint8_t volume_type );
LIBEWF_EXTERN int8_t libewf_set_write_format( LIBEWF_HANDLE *handle, uint8_t format );
LIBEWF_EXTERN int8_t libewf_set_write_input_size( LIBEWF_HANDLE *handle, uint64_t input_write_size );

LIBEWF_EXTERN int8_t libewf_set_header_value( LIBEWF_HANDLE *handle, LIBEWF_CHAR *identifier, LIBEWF_CHAR *value, size_t length );
LIBEWF_EXTERN int8_t libewf_set_hash_value( LIBEWF_HANDLE *handle, LIBEWF_CHAR *identifier, LIBEWF_CHAR *value, size_t length );

LIBEWF_EXTERN int8_t libewf_set_swap_byte_pairs( LIBEWF_HANDLE *handle, uint8_t swap_byte_pairs );

LIBEWF_EXTERN int8_t libewf_calculate_md5_hash( LIBEWF_HANDLE *handle, LIBEWF_CHAR *string, size_t length );
LIBEWF_EXTERN int8_t libewf_get_stored_md5_hash( LIBEWF_HANDLE *handle, LIBEWF_CHAR *string, size_t length );
LIBEWF_EXTERN int8_t libewf_get_calculated_md5_hash( LIBEWF_HANDLE *handle, LIBEWF_CHAR *string, size_t length );

LIBEWF_EXTERN int8_t libewf_parse_header_values( LIBEWF_HANDLE *handle, uint8_t date_format );
LIBEWF_EXTERN int8_t libewf_parse_hash_values( LIBEWF_HANDLE *handle );

LIBEWF_EXTERN int8_t libewf_add_acquiry_error( LIBEWF_HANDLE *handle, uint64_t sector, uint32_t amount_of_sectors );

LIBEWF_EXTERN void libewf_set_notify_values( FILE *stream, uint8_t verbose );

#ifdef __cplusplus
}
#endif

#endif

