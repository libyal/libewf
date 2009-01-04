/*
 * libewf hash values
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

#if !defined( _LIBEWF_HASH_VALUES_H )
#define _LIBEWF_HASH_VALUES_H

#include "libewf_includes.h"
#include "libewf_char.h"
#include "libewf_values_table.h"

#include "ewf_char.h"

#if defined( __cplusplus )
extern "C" {
#endif

#define LIBEWF_HASH_VALUES_DEFAULT_AMOUNT	1

int libewf_hash_values_initialize( libewf_values_table_t *hash_values );

libewf_values_table_t *libewf_hash_values_parse_hash_string_xml( libewf_char_t *hash_string_xml, size_t length );
libewf_values_table_t *libewf_hash_values_parse_xhash( ewf_char_t *xhash, size_t size );

ewf_char_t *libewf_hash_values_convert_hash_string_to_hash( libewf_char_t *hash_string, size_t string_length, size_t *hash_length );
libewf_char_t *libewf_hash_values_generate_hash_string_xml( libewf_values_table_t *hash_values, size_t *string_length );
ewf_char_t *libewf_hash_values_generate_xhash_string_ewfx( libewf_values_table_t *hash_values, size_t *hash_length );

#if defined( __cplusplus )
}
#endif

#endif

