/*
 * libewf character type string functions
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

#if !defined( _LIBEWF_STRING_H )
#define _LIBEWF_STRING_H

#include <common.h>
#include <character_string.h>

#include <stdio.h>

#include <libewf/types.h>

#include "ewf_char.h"
#include "ewf_digest_hash.h"

#if defined( __cplusplus )
extern "C" {
#endif

#define LIBEWF_STRING_LITTLE_ENDIAN	(uint8_t) 'l'
#define LIBEWF_STRING_BIG_ENDIAN	(uint8_t) 'b'

character_t **libewf_string_split(
               character_t *string,
               size_t size,
               character_t delimiter,
               uint32_t *amount );

void libewf_string_split_values_free(
      character_t **split_values,
      uint32_t amount );

int libewf_string_copy_from_utf16(
     character_t *string,
     size_t size_string,
     ewf_char_t *utf16_string,
     size_t size_utf16 );

int libewf_string_copy_to_utf16(
     character_t *string,
     size_t size,
     ewf_char_t *utf16_string,
     size_t size_utf16,
     uint8_t byte_order );

int libewf_string_copy_from_ewf_char(
     character_t *string,
     size_t size_string,
     ewf_char_t *ewf_char_string,
     size_t size_ewf_char_string );

int libewf_string_copy_to_ewf_char(
     character_t *string,
     size_t size_string,
     ewf_char_t *ewf_char_string,
     size_t size_ewf_char_string );

#define libewf_string_copy_from_header( string, size_string, header, size_header ) \
	libewf_string_copy_from_ewf_char( string, size_string, header, size_header )
#define libewf_string_copy_to_header( string, size_string, header, size_header ) \
	libewf_string_copy_to_ewf_char( string, size_string, header, size_header )

int libewf_string_copy_from_header2(
     character_t *string,
     size_t size_string,
     ewf_char_t *header2,
     size_t size_header2 );

int libewf_string_copy_to_header2(
     character_t *string,
     size_t size_string,
     ewf_char_t *header2,
     size_t size_header2 );

int libewf_string_ctime(
     const time_t *timestamp,
     character_t **ctime_string,
     size_t *ctime_string_length );

#if defined( __cplusplus )
}
#endif

#endif

