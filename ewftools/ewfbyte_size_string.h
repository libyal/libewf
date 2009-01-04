/*
 * ewfbyte_size_string
 * Byte size string functions for the libewf tools
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

#if !defined( _EWFBYTE_SIZE_STRING_H )
#define _EWFBYTE_SIZE_STRING_H

#include <common.h>
#include <character_string.h>
#include <system_string.h>

#include <libewf/types.h>

#if defined( __cplusplus )
extern "C" {
#endif

#define EWFBYTE_SIZE_STRING_UNIT_MEGABYTE		1000
#define EWFBYTE_SIZE_STRING_UNIT_MEBIBYTE		1024

const character_t *ewfbyte_size_string_get_factor_string(
                    int8_t factor );

int8_t ewfbyte_size_string_get_factor(
        character_t factor );

int ewfbyte_size_string_create(
     character_t *byte_size_string,
     size_t byte_size_string_length,
     uint64_t size,
     int units );

int ewfbyte_size_string_convert(
     character_t *byte_size_string,
     size_t byte_size_string_length,
     uint64_t *size );

int8_t ewfbyte_size_string_get_factor_char_t(
        system_character_t factor );

int ewfbyte_size_string_convert_char_t(
     system_character_t *byte_size_string,
     size_t byte_size_string_length,
     uint64_t *size );

#if defined( __cplusplus )
}
#endif

#endif

