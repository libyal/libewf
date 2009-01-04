/*
 * File IO functions
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

#include "common.h"
#include "file_io.h"
#include "notify.h"

/* Function to wrap open()
 */
int libewf_file_io_open(
     const char *filename,
     int flags )
{
	static char *function = "libewf_file_io_open";
	int file_descriptor   = 0;

	if( filename == NULL )
	{
		notify_warning_printf( "%s: invalid filename.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_WINDOWS_API )
	if( _sopen_s(
	     &file_descriptor,
	     filename,
	     ( flags | _O_BINARY ),
	     _SH_DENYRW,
	     ( _S_IREAD | _S_IWRITE ) ) != 0 )
#else
	file_descriptor = open(
	                   filename,
	                   flags,
	                   0644 );

	if( file_descriptor == -1 )
#endif
	{
		notify_warning_printf( "%s: error opening file: %s.\n",
		 function, filename );

		return( -1 );
	}
	return( file_descriptor );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )

/* Function to wrap wopen() which is the wide character equivalent of open()
 */
int libewf_file_io_wopen(
     const wchar_t *filename,
     int flags )
{
	static char *function = "libewf_file_io_wopen";
	int file_descriptor   = 0;

	if( filename == NULL )
	{
		notify_warning_printf( "%s: invalid filename.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_WINDOWS_API )
	if( _wsopen_s(
	     &file_descriptor,
	     filename,
	     ( flags | _O_BINARY ),
	     _SH_DENYRW,
	     ( _S_IREAD | _S_IWRITE ) ) != 0 )
	{
		notify_warning_printf( "%s: error opening file: %s.\n",
		 function, filename );

		return( -1 );
	}
	return( file_descriptor );
#else
#error missing wopen()
#endif
}
#endif

