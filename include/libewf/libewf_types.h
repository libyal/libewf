/*
 * Type definitions for libewf
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

#ifndef _LIBEWF_TYPES_H
#define _LIBEWF_TYPES_H

#if 1 || defined( HAVE_SYS_TYPES_H )
#include <sys/types.h>

#else
#error Missing system type definitions (sys/types.h)
#endif

#if defined( HAVE_WINDOWS_API ) && !defined( HAVE_INTTYPES_H ) && !defined( HAVE_STDINT_H )

/* Define LIBEWF_DEFINITION_INTEGER_TYPES to avoid conflict if types are already defined
 */
#if !defined( LIBEWF_DEFINITION_INTEGER_TYPES )

#define LIBEWF_DEFINITION_INTEGER_TYPES

#ifdef __cplusplus
extern "C" {
#endif

/* Windows does not have <inttypes.h> or <stdint.h> and therefore does not support the
 * (u)int#_t type definitions it has __int# defintions instead
 */
typedef __int8			int8_t;
typedef unsigned __int8		uint8_t;
typedef __int16			int16_t;
typedef unsigned __int16	uint16_t;
typedef __int32			int32_t;
typedef unsigned __int32	uint32_t;
typedef __int64			int64_t;
typedef unsigned __int64	uint64_t;
typedef int			ssize_t;

#ifdef __cplusplus
}
#endif

#endif

#elif 1 || defined( HAVE_INTTYPES_H )
#include <inttypes.h>

#elif 1 || defined( HAVE_STDINT_H )
#include <stdint.h>

#else
#error Missing integer type definitions (inttypes.h, stdint.h)
#endif

#if 0 || defined( HAVE_WIDE_CHARACTER_TYPE )

#if 0 || defined( HAVE_WCHAR_H )

/* __USE_ISOC99 is required to add swprintf definition
 */
#if !defined( __USE_ISOC99 )
#define __USE_ISOC99
#define LIBEWF_DEFINITION_ISOC99
#endif

#include <wchar.h>

#if defined( LIBEWF_DEFINITION_ISOC99 )
#undef __USE_ISOC99
#undef LIBEWF_DEFINITION_ISOC99
#endif

#else
#error Missing wide character type definition (wchar.h)
#endif

#endif

#endif

