/*
 * libewf hash values
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

#ifndef _LIBEWF_HASHVALUES_H
#define _LIBEWF_HASHVALUES_H

#include "libewf_includes.h"
#include "libewf_char.h"

#include "ewf_char.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LIBEWF_HASH_VALUES_DEFAULT_AMOUNT	1

#define LIBEWF_HASH_VALUES libewf_hash_values_t
#define LIBEWF_HASH_VALUES_SIZE sizeof( LIBEWF_HASH_VALUES )

typedef struct libewf_hash_values libewf_hash_values_t;

struct libewf_hash_values
{
	/* The amount of hash values
	 */
	uint32_t amount;

	/* The hash value identifiers
	 */
	LIBEWF_CHAR **identifiers;

	/* The hash values
	 */
	LIBEWF_CHAR **values;
};

LIBEWF_HASH_VALUES *libewf_hash_values_alloc( void );
LIBEWF_HASH_VALUES *libewf_hash_values_realloc( LIBEWF_HASH_VALUES *hash_values, uint32_t previous_amount, uint32_t new_amount );
void libewf_hash_values_free( LIBEWF_HASH_VALUES *hash_values );

int32_t libewf_hash_values_get_index( LIBEWF_HASH_VALUES *hash_values, LIBEWF_CHAR *identifier );
int8_t libewf_hash_values_get_value( LIBEWF_HASH_VALUES *hash_values, LIBEWF_CHAR *identifier, LIBEWF_CHAR *value, size_t length );
int8_t libewf_hash_values_set_value( LIBEWF_HASH_VALUES *hash_values, LIBEWF_CHAR *identifier, LIBEWF_CHAR *value, size_t length );

LIBEWF_HASH_VALUES *libewf_hash_values_parse_hash_string_xml( LIBEWF_CHAR *hash_string_xml, size_t length );
LIBEWF_HASH_VALUES *libewf_hash_values_parse_xhash( EWF_CHAR *xhash, size_t size );

EWF_CHAR *libewf_hash_values_convert_hash_string_to_hash( LIBEWF_CHAR *hash_string, size_t string_length, size_t *hash_length );
LIBEWF_CHAR *libewf_hash_values_generate_hash_string_xml( LIBEWF_HASH_VALUES *hash_values, size_t *string_length );
EWF_CHAR *libewf_hash_values_generate_xhash_string_ewfx( LIBEWF_HASH_VALUES *hash_values, size_t *hash_length );

#ifdef __cplusplus
}
#endif

#endif

