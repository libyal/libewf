/*
 * EWF character type string functions
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

#if !defined( _EWF_STRING_H )
#define _EWF_STRING_H

#include <common.h>

#include <libewf/types.h>

#if defined( HAVE_STRING_H )
#include <string.h>
#endif

#include "ewf_char.h"

#if defined( __cplusplus )
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
	(ewf_char_t *) memcpy( (void *) destination, (void *) source, size )
#elif defined( HAVE_STRNCPY )
#define ewf_string_copy( destination, source, size ) \
	strncpy( destination, source, size )
#elif defined( HAVE_STRCPY )
#define ewf_string_copy( destination, source, size ) \
	strcpy( destination, source )
#else
#error Missing string copy function (memcpy, strncpy and strcpy)
#endif

#if defined( __cplusplus )
}
#endif

#endif

