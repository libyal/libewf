/*
 * libewf values table
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

#if !defined( _LIBEWF_VALUES_TABLE_H )
#define _LIBEWF_VALUES_TABLE_H

#include "libewf_includes.h"
#include "libewf_char.h"

#include "ewf_char.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_values_table libewf_values_table_t;

struct libewf_values_table
{
	/* The amount of values
	 */
	uint32_t amount;

	/* The value identifiers
	 */
	libewf_char_t **identifiers;

	/* The values
	 */
	libewf_char_t **values;
};

libewf_values_table_t *libewf_values_table_alloc( uint32_t amount );
int libewf_values_table_realloc( libewf_values_table_t *values_table, uint32_t previous_amount, uint32_t new_amount );
void libewf_values_table_free( libewf_values_table_t *values_table );

int32_t libewf_values_table_get_index( libewf_values_table_t *values_table, libewf_char_t *identifier );
int libewf_values_table_get_identifier( libewf_values_table_t *values_table, uint32_t index, libewf_char_t *identifier, size_t length );
int libewf_values_table_get_value( libewf_values_table_t *values_table, libewf_char_t *identifier, libewf_char_t *value, size_t length );
int libewf_values_table_set_value( libewf_values_table_t *values_table, libewf_char_t *identifier, libewf_char_t *value, size_t length );

#if defined( __cplusplus )
}
#endif

#endif

