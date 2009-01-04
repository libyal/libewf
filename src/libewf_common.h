/*
 * Common code for libewf - wraps external function calls
 *
 * Copyright (c) 2006, Joachim Metz <forensics@hoffmannbv.nl>,
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

#ifndef _LIBEWF_COMMON_H
#define _LIBEWF_COMMON_H

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

void *libewf_alloc( uint32_t size );
void *libewf_realloc( void *buffer, uint32_t size );
void libewf_free( void *buffer );
void *libewf_memset( void *buffer, uint8_t value, uint32_t count );
void *libewf_memcpy( void *destination, const void *source, uint32_t size );
int32_t libewf_memcmp( const void *buffer1, const void *buffer2, uint32_t size );
char *libewf_strdup( const char *string );
uint32_t libewf_strlen( const char *string );
char *libewf_strchr( const char *string, uint8_t character );
uint8_t libewf_strncmp( const char *string1, const char *string2, uint32_t length );
int32_t libewf_read( int file_descriptor, void *buffer, uint32_t count );
int32_t libewf_write( int file_descriptor, const void *buffer, uint32_t count );

void *libewf_alloc_cleared( uint32_t size, int8_t clear_value );
void *libewf_realloc_full_cleared( void *buffer, uint32_t previous_size, uint32_t new_size, int8_t clear_value );
void *libewf_realloc_new_cleared( void *buffer, uint32_t previous_size, uint32_t new_size, int8_t clear_value );

#ifdef __cplusplus
}
#endif

#endif

