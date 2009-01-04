/*
 * crypographic hash for ewftools
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
#include <types.h>

#include "ewfdigest_hash.h"

#include "../libewf/libewf_common.h"
#include "../libewf/libewf_notify.h"

/* Converts the EWF digest hash to a printable string
 * Returns 1 if successful, 0 if hash was not set, or -1 on error
 */
int ewfdigest_copy_to_string(
     ewfdigest_hash_t *digest_hash,
     size_t size_digest_hash,
     libewf_char_t *string,
     size_t size_string )
{
	static char *function       = "ewfdigest_copy_to_string";
	size_t string_iterator      = 0;
	size_t digest_hash_iterator = 0;
	uint8_t digest_digit        = 0;

	if( digest_hash == NULL )
	{
		LIBEWF_VERBOSE_PRINT( "%s: invalid digest hash.\n",
		 function );

		return( 0 );
	}
	if( string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid string.\n",
		 function );

		return( -1 );
	}
	if( ( size_string > (size_t) SSIZE_MAX )
	 || ( size_digest_hash > (size_t) SSIZE_MAX ) )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	/* The string requires space for 2 characters per digest hash digit and a end of string
	 */
	if( size_string < ( ( 2 * size_digest_hash ) + 1 ) )
	{
		LIBEWF_WARNING_PRINT( "%s: string too small.\n",
		 function );

		return( -1 );
	}
	for( digest_hash_iterator = 0; digest_hash_iterator < size_digest_hash; digest_hash_iterator++ )
	{
		digest_digit = digest_hash[ digest_hash_iterator ] / 16;

		if( digest_digit <= 9 )
		{
			string[ string_iterator++ ] = (libewf_char_t) ( (uint8_t) '0' + digest_digit );
		}
		else
		{
			string[ string_iterator++ ] = (libewf_char_t) ( (uint8_t) 'a' + ( digest_digit - 10 ) );
		}
		digest_digit = digest_hash[ digest_hash_iterator ] % 16;

		if( digest_digit <= 9 )
		{
			string[ string_iterator++ ] = (libewf_char_t) ( (uint8_t) '0' + digest_digit );
		}
		else
		{
			string[ string_iterator++ ] = (libewf_char_t) ( (uint8_t) 'a' + ( digest_digit - 10 ) );
		}
	}
	string[ string_iterator ] = (libewf_char_t) '\0';

	return( 1 );
}

