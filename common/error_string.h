/*
 * Error string functions
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

#if !defined( _ERROR_STRING_H )
#define _ERROR_STRING_H

#include "common.h"

#include <libewf/types.h>

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )

#if defined( HAVE_WINDOWS_API )
#define error_string_wcserror_r( error_number, string, size ) \
	_wcserror_s( string, size, error_number )

#define ERROR_STRING_WCSTRERROR_R_RETURN_ERROR	0

#else
#error Missing wide character equivalent of strerror()
#endif

#endif

#if defined( HAVE_WINDOWS_API )
#define error_string_strerror_r( error_number, string, size ) \
	strerror_s( string, size, error_number )

#define ERROR_STRING_STRERROR_R_RETURN_ERROR	0

#elif defined( HAVE_STRERROR_R )
#define error_string_strerror_r( error_number, string, size ) \
	strerror_r( error_number, string, size )

#if defined( STRERROR_R_CHAR_P )
#define ERROR_STRING_STRERROR_R_RETURN_ERROR	NULL

#else
#define ERROR_STRING_STRERROR_R_RETURN_ERROR	0
#endif

#endif

char *libewf_error_string_strerror(
       int error_number );

#define error_string_strerror( error_number ) \
	libewf_error_string_strerror( error_number )

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
wchar_t *libewf_error_string_wcerror(
          int error_number );

#define error_string_wcserror( error_number ) \
	libewf_error_string_wcrerror( error_number )

#endif

#if defined( __cplusplus )
}
#endif

#endif

