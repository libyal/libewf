/*
 * libewf notification
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

#include <common.h>
#include <notify.h>
#include <types.h>

#if defined( HAVE_STDLIB_H )
#include <stdlib.h>
#endif

#include <libewf/definitions.h>

#include "libewf_common.h"
#include "libewf_notify.h"

/* Prints a dump of data
 */
void libewf_dump_data(
      uint8_t *data,
      size_t size )
{
	size_t iterator = 0;

	while( iterator < size )
	{
		if( iterator % 16 == 0 )
		{
			notify_verbose_printf( "%.8" PRIzx ": ",
			 iterator );
		}
		notify_verbose_printf( "%.2" PRIx8 "",
		 data[ iterator++ ] );

		if( iterator % 16 == 0 )
		{
			notify_verbose_printf( "\n" );
		}
		else if( iterator % 8 == 0 )
		{
			notify_verbose_printf( "  " );
		}
	}
	if( iterator % 16 != 0 )
	{
		notify_verbose_printf( "\n" );
	}
	notify_verbose_printf( "\n" );

	iterator = 0;

	while( iterator < size )
	{
		if( iterator % 32 == 0 )
		{
			notify_verbose_printf( "%.8" PRIzx ": ",
			 iterator );
		}
		notify_verbose_printf( "%c ",
		 (char) data[ iterator++ ] );

		if( iterator % 32 == 0 )
		{
			notify_verbose_printf( "\n" );
		}
		else if( iterator % 8 == 0 )
		{
			notify_verbose_printf( "  " );
		}
	}
	if( iterator % 32 != 0 )
	{
		notify_verbose_printf( "\n" );
	}
	notify_verbose_printf( "\n" );
}

