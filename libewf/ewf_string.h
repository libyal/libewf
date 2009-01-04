/*
 * EWF character type string functions
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

#ifndef _EWF_STRING_H
#define _EWF_STRING_H

#include "libewf_includes.h"

#if defined( HAVE_STRING_H )
#include <string.h>
#endif

#include "ewf_char.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined( HAVE_STRLEN )
#define ewf_string_length( string ) \
	strlen( (char *) string )
#else
#error Missing string length function (strlen)
#endif

#if defined( HAVE_MEMCMP )
#define ewf_string_compare( string1, string2, size ) \
	memcmp( (void *) string1, (void *) string2, size )
#elif defined( HAVE_STRNCMP )
#define ewf_string_compare( string1, string2, size ) \
	strncmp( string1, string2, size )
#elif defined( HAVE_STRCMP )
#define ewf_string_compare( string1, string2, size ) \
	strcmp( string1, string2 )
#else
#error Missing string compare function (memcmp, strncmp and strcmp)
#endif

#if defined( HAVE_MEMCPY )
#define ewf_string_copy( destination, source, size ) \
	(LIBEWF_CHAR *) memcpy( (void *) destination, (void *) source, size )
#elif defined( HAVE_STRNCPY )
#define ewf_string_copy( destination, source, size ) \
	strncpy( destination, source, size )
#elif defined( HAVE_STRCPY )
#define ewf_string_copy( destination, source, size ) \
	strcpy( destination, source )
#else
#error Missing string copy function (memcpy, strncpy and strcpy)
#endif

EWF_CHAR *ewf_string_uncompress( EWF_CHAR *compressed_string, size_t *size );
EWF_CHAR *ewf_string_compress( EWF_CHAR *uncompressed_string, size_t *size, int8_t compression_level );

ssize_t ewf_string_read_to_buffer( EWF_CHAR *string, int file_descriptor, size_t size );
ssize_t ewf_string_write_from_buffer( EWF_CHAR *string, int file_descriptor, size_t size );

EWF_CHAR *ewf_string_read( int file_descriptor, size_t size );
#define ewf_string_write( string, file_descriptor, size ) \
	ewf_string_write_from_buffer( string, file_descriptor, size )

EWF_CHAR *ewf_string_read_compressed( int file_descriptor, size_t *size );
ssize_t ewf_string_write_compressed( EWF_CHAR *string, int file_descriptor, size_t *size, int8_t compression_level );

#ifdef __cplusplus
}
#endif

#endif

