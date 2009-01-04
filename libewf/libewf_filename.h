/*
 * libewf filename
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

#if !defined( _LIBEWF_FILENAME_H )
#define _LIBEWF_FILENAME_H

#include "libewf_includes.h"

#include "libewf_common.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
#define LIBEWF_FILENAME		wchar_t
#define LIBEWF_FILENAME_SIZE	sizeof( wchar_t )
#define PRIs_EWF_filename	"ls"

#define libewf_filename_length( filename ) \
	libewf_common_wide_string_length( filename )

#define libewf_filename_copy( destination, source, length ) \
	libewf_common_wide_memcpy( destination, source, length )

#define libewf_filename_open( filename, flags ) \
	libewf_common_wide_open( filename, flags )

#else
#define LIBEWF_FILENAME		char
#define LIBEWF_FILENAME_SIZE	sizeof( char )
#define PRIs_EWF_filename	"s"

#define libewf_filename_length( filename ) \
	libewf_common_string_length( filename )

#define libewf_filename_copy( destination, source, length ) \
	libewf_common_memcpy( destination, source, length )

#define libewf_filename_open( filename, flags ) \
	libewf_common_open( filename, flags )

#endif

#ifdef REFACTOR
int libewf_filename_get( LIBEWF_FILENAME *internal_filename, LIBEWF_FILENAME *external_filename, size_t length_external_filename );

int libewf_filename_set( LIBEWF_FILENAME **internal_filename, const LIBEWF_FILENAME *external_filename, size_t length_external_filename );
#endif

int libewf_filename_set_extension( LIBEWF_FILENAME *extension, uint16_t segment_number, int16_t maximum_amount_of_segments, uint8_t segment_file_type, uint8_t format, uint8_t ewf_format );

int libewf_filename_create( LIBEWF_FILENAME **filename, size_t *length_filename, LIBEWF_FILENAME *basename, size_t length_basename, uint16_t segment_number, int16_t maximum_amount_of_segments, uint8_t segment_file_type, uint8_t format, uint8_t ewf_format );


#if defined( __cplusplus )
}
#endif

#endif

