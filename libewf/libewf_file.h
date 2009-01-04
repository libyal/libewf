/*
 * libewf file handling
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

#if !defined( _LIBEWF_FILE_H )
#define _LIBEWF_FILE_H

#include "libewf_includes.h"

#include <stdio.h>

/* If libtool DLL support is enabled set LIBEWF_DLL_EXPORT
 * before including libewf_extern.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBEWF_DLL_EXPORT
#endif

#include <libewf/libewf_extern.h>
#include <libewf/libewf_handle.h>

#include "libewf_char.h"
#include "libewf_filename.h"
#include "libewf_internal_handle.h"

#if defined( __cplusplus )
extern "C" {
#endif

LIBEWF_EXTERN const LIBEWF_CHAR *libewf_get_version( void );

LIBEWF_EXTERN uint8_t libewf_get_flags_read( void );
LIBEWF_EXTERN uint8_t libewf_get_flags_read_write( void );
LIBEWF_EXTERN uint8_t libewf_get_flags_write( void );

LIBEWF_EXTERN int libewf_check_file_signature( const LIBEWF_FILENAME *filename );

LIBEWF_EXTERN LIBEWF_HANDLE *libewf_open( LIBEWF_FILENAME * const filenames[], uint16_t file_amount, uint8_t flags );

LIBEWF_EXTERN int libewf_close( LIBEWF_HANDLE *handle );

LIBEWF_EXTERN off64_t libewf_seek_offset( LIBEWF_HANDLE *handle, off64_t offset );

LIBEWF_EXTERN int libewf_get_sectors_per_chunk( LIBEWF_HANDLE *handle, uint32_t *sectors_per_chunk );
LIBEWF_EXTERN int libewf_get_bytes_per_sector( LIBEWF_HANDLE *handle, uint32_t *bytes_per_sector );
LIBEWF_EXTERN int libewf_get_amount_of_sectors( LIBEWF_HANDLE *handle, uint32_t *amount_of_sectors );
LIBEWF_EXTERN int libewf_get_chunk_size( LIBEWF_HANDLE *handle, size32_t *chunk_size );
LIBEWF_EXTERN int libewf_get_error_granularity( LIBEWF_HANDLE *handle, uint32_t *error_granularity );
LIBEWF_EXTERN int libewf_get_compression_values( LIBEWF_HANDLE *handle, int8_t *compression_level, uint8_t *compress_empty_block );
LIBEWF_EXTERN int libewf_get_media_size( LIBEWF_HANDLE *handle, size64_t *media_size );

LIBEWF_EXTERN int libewf_get_media_type( LIBEWF_HANDLE *handle, int8_t *media_type );
LIBEWF_EXTERN int libewf_get_media_flags( LIBEWF_HANDLE *handle, int8_t *media_flags );
LIBEWF_EXTERN int libewf_get_volume_type( LIBEWF_HANDLE *handle, int8_t *volume_type );
LIBEWF_EXTERN int libewf_get_format( LIBEWF_HANDLE *handle, int8_t *format );

LIBEWF_EXTERN int libewf_get_guid( LIBEWF_HANDLE *handle, uint8_t *guid, size_t size );
LIBEWF_EXTERN int libewf_get_md5_hash( LIBEWF_HANDLE *handle, uint8_t *md5_hash, size_t size );

LIBEWF_EXTERN int libewf_get_delta_segment_filename( LIBEWF_HANDLE *handle, LIBEWF_FILENAME *filename, size_t length );

LIBEWF_EXTERN int libewf_get_amount_of_acquiry_errors( LIBEWF_HANDLE *handle, uint32_t *amount_of_errors );
LIBEWF_EXTERN int libewf_get_acquiry_error( LIBEWF_HANDLE *handle, uint32_t index, off64_t *sector, uint32_t *amount_of_sectors  );
LIBEWF_EXTERN int libewf_get_amount_of_crc_errors( LIBEWF_HANDLE *handle, uint32_t *amount_of_errors );
LIBEWF_EXTERN int libewf_get_crc_error( LIBEWF_HANDLE *handle, uint32_t index, off64_t *sector, uint32_t *amount_of_sectors );

LIBEWF_EXTERN int libewf_get_write_amount_of_chunks( LIBEWF_HANDLE *handle, uint32_t *amount_of_chunks );

LIBEWF_EXTERN int libewf_get_amount_of_header_values( LIBEWF_HANDLE *handle, uint32_t *amount_of_values );
LIBEWF_EXTERN int libewf_get_header_value_identifier( LIBEWF_HANDLE *handle, uint32_t index, LIBEWF_CHAR *value, size_t length );
LIBEWF_EXTERN int libewf_get_header_value( LIBEWF_HANDLE *handle, LIBEWF_CHAR *identifier, LIBEWF_CHAR *value, size_t length );

LIBEWF_EXTERN int libewf_get_amount_of_hash_values( LIBEWF_HANDLE *handle, uint32_t *amount_of_values );
LIBEWF_EXTERN int libewf_get_hash_value_identifier( LIBEWF_HANDLE *handle, uint32_t index, LIBEWF_CHAR *value, size_t length );
LIBEWF_EXTERN int libewf_get_hash_value( LIBEWF_HANDLE *handle, LIBEWF_CHAR *identifier, LIBEWF_CHAR *value, size_t length );

LIBEWF_EXTERN int libewf_set_sectors_per_chunk( LIBEWF_HANDLE *handle, uint32_t sectors_per_chunk );
LIBEWF_EXTERN int libewf_set_bytes_per_sector( LIBEWF_HANDLE *handle, uint32_t bytes_per_sector );

LIBEWF_EXTERN int libewf_set_error_granularity( LIBEWF_HANDLE *handle, uint32_t error_granularity );
LIBEWF_EXTERN int libewf_set_compression_values( LIBEWF_HANDLE *handle, int8_t compression_level, uint8_t compress_empty_block );
LIBEWF_EXTERN int libewf_set_media_size( LIBEWF_HANDLE *handle, size64_t media_size );
LIBEWF_EXTERN int libewf_set_segment_file_size( LIBEWF_HANDLE *handle, size64_t segment_file_size );

LIBEWF_EXTERN int libewf_set_media_type( LIBEWF_HANDLE *handle, uint8_t media_type );
LIBEWF_EXTERN int libewf_set_volume_type( LIBEWF_HANDLE *handle, uint8_t volume_type );
LIBEWF_EXTERN int libewf_set_format( LIBEWF_HANDLE *handle, uint8_t format );

LIBEWF_EXTERN int libewf_set_guid( LIBEWF_HANDLE *handle, uint8_t *guid, size_t size );
LIBEWF_EXTERN int libewf_set_md5_hash( LIBEWF_HANDLE *handle, uint8_t *md5_hash, size_t size );

LIBEWF_EXTERN int libewf_set_delta_segment_filename( LIBEWF_HANDLE *handle, LIBEWF_FILENAME *filename, size_t length );

LIBEWF_EXTERN int libewf_set_read_wipe_chunk_on_error( LIBEWF_HANDLE *handle, uint8_t wipe_on_error );

LIBEWF_EXTERN int libewf_set_header_value( LIBEWF_HANDLE *handle, LIBEWF_CHAR *identifier, LIBEWF_CHAR *value, size_t length );
LIBEWF_EXTERN int libewf_set_hash_value( LIBEWF_HANDLE *handle, LIBEWF_CHAR *identifier, LIBEWF_CHAR *value, size_t length );

LIBEWF_EXTERN int libewf_parse_header_values( LIBEWF_HANDLE *handle, uint8_t date_format );
LIBEWF_EXTERN int libewf_parse_hash_values( LIBEWF_HANDLE *handle );

LIBEWF_EXTERN int libewf_add_acquiry_error( LIBEWF_HANDLE *handle, off64_t sector, uint32_t amount_of_sectors );
LIBEWF_EXTERN int libewf_add_crc_error( LIBEWF_HANDLE *handle, off64_t sector, uint32_t amount_of_sectors );

LIBEWF_EXTERN int libewf_copy_header_values( LIBEWF_HANDLE *destination_handle, LIBEWF_HANDLE *source_handle );

LIBEWF_EXTERN void libewf_set_notify_values( FILE *stream, uint8_t verbose );

#if defined( __cplusplus )
}
#endif

#endif

