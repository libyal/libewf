/*
 * ewfglob
 * Globbing functions for the libewf tools
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

#include "../libewf/libewf_includes.h"

#include <errno.h>

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#if !defined(HAVE_GLOB_H) && defined(HAVE_IO_H)
#include <io.h>
#endif

#include <libewf.h>

#include "../libewf/libewf_common.h"
#include "../libewf/libewf_notify.h"
#include "../libewf/libewf_string.h"

#include "ewfglob.h"

#ifndef HAVE_GLOB_H

#ifdef HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS

#define ewfglob_finddata_t	_wfinddata_t
#define ewfglob_makepath	_wmakepath_s
#define ewfglob_findfirst	_wfindfirst
#define ewfglob_findnext	_wfindnext
#define ewfglob_splitpath	_wsplitpath_s

#else

#define ewfglob_finddata_t	_finddata_t
#define ewfglob_makepath	_makepath_s
#define ewfglob_findfirst	_findfirst
#define ewfglob_findnext	_findnext
#define ewfglob_splitpath	_splitpath_s

#endif

#define ewfglob_findclose	_findclose

/* Allocates memory for a new glob struct
 * Returns a pointer to the new instance, NULL on error
 */
EWFGLOB *ewfglob_alloc( void )
{
	EWFGLOB *glob = NULL;

	glob = (EWFGLOB *) libewf_common_alloc_cleared( EWFGLOB_SIZE, 0 );

	if( glob == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfglob_alloc: unable to allocate glob.\n" );

		return( NULL );
	}
	glob->results = NULL;
	glob->amount  = 0;

	return( glob );
}

/* Reallocates memory for a glob
 * Returns a pointer to the instance, NULL on error
 */
EWFGLOB *ewfglob_realloc( EWFGLOB *glob, uint16_t new_amount )
{
	CHAR_T **reallocation = NULL;
	size_t previous_size  = 0;
	size_t new_size       = 0;

	if( glob == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfglob_realloc: invalid glob.\n" );

		return( NULL );
	}
	if( glob->amount >= new_amount )
	{
		LIBEWF_WARNING_PRINT( "ewfglob_realloc: new amount less equal than current amount.\n" );

		return( NULL );
	}
	new_size = EWFGLOB_RESULT_SIZE * new_amount;

	if( ( previous_size > (size_t) SSIZE_MAX ) || ( new_size > (size_t) SSIZE_MAX ) )
	{
		LIBEWF_WARNING_PRINT( "ewfglob_realloc: invalid size value exceeds maximum.\n" );

		return( NULL );
	}
	if( glob->amount == 0 )
	{
		reallocation = (CHAR_T **) libewf_common_alloc_cleared( new_size, 0 );
	}
	else
	{
		previous_size = glob->amount * EWFGLOB_RESULT_SIZE;
		reallocation  = (CHAR_T **) libewf_common_realloc_new_cleared( glob->results, previous_size, new_size, 0 );
	}
	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfglob_realloc: unable to reallocate glob results.\n" );

		return( NULL );
	}
	glob->results = reallocation;
	glob->amount  = new_amount;

	return( glob );
}

/* Frees memory of a glob
 */
void ewfglob_free( EWFGLOB *glob )
{
	uint16_t iterator = 0;

	if( glob == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfglob_free: invalid glob.\n" );

		return;
	}
	if( glob->results != NULL )
	{
		for( iterator = 0; iterator < glob->amount; iterator++ )
		{
			if( glob->results[ iterator ] != NULL )
			{
				libewf_common_free( glob->results[ iterator ] );
			}
		}
		libewf_common_free( glob->results );
	}
	libewf_common_free( glob );
}

/* Resolves filenames with wildcards (globs)
 * Returns the amount of result if successful, -1 on error
 */
int32_t ewfglob_resolve( EWFGLOB *glob, CHAR_T * const patterns[], uint32_t amount_of_patterns )
{
#ifdef HAVE_WINDOWS_API
	struct ewfglob_finddata_t find_data;

	CHAR_T find_path[ _MAX_PATH ];
	CHAR_T find_drive[ _MAX_DRIVE ];
	CHAR_T find_directory[ _MAX_DIR ];
	CHAR_T find_name[ _MAX_FNAME ];
	CHAR_T find_extension[ _MAX_EXT ];

	intptr_t find_handle  = 0;
#endif
	EWFGLOB *reallocation = NULL;
	int32_t globs_found   = 0;
	uint32_t iterator     = 0;

	if( glob == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfglob_resolve: invalid glob.\n" );

		return( -1 );
	}
	for( iterator = 0; iterator < amount_of_patterns; iterator++ )
	{
		if( patterns[ iterator ] == NULL )
		{
			LIBEWF_WARNING_PRINT( "ewfglob_resolve: invalid pattern.\n" );

			return( -1 );
		}
#ifdef HAVE_WINDOWS_API
		if( ewfglob_splitpath( patterns[ iterator ], find_drive, _MAX_DRIVE, find_directory, _MAX_DIR, find_name, _MAX_FNAME, find_extension, _MAX_EXT ) != 0 )
		{
			LIBEWF_WARNING_PRINT( "ewfglob_resolve: unable to split path.\n" );

			return( -1 );
		}
		find_handle = ewfglob_findfirst( patterns[ iterator ], &find_data );

		if( find_handle != -1 )
		{
			do
			{
				reallocation = ewfglob_realloc( glob, ( glob->amount + 1 ) );

				if( reallocation == NULL )
				{
					LIBEWF_WARNING_PRINT( "ewfglob_resolve: unable to reallocate glob.\n" );

					return( -1 );
				}
				if( ewfglob_makepath( find_path, _MAX_PATH, find_drive, find_directory, find_data.name, _S_CHAR_T( "" )  ) != 0 )
				{
					LIBEWF_WARNING_PRINT( "ewfglob_resolve: unable to make path.\n" );

					return( -1 );
				}
				glob->results[ glob->amount - 1 ] = CHAR_T_DUPLICATE( find_path );

				globs_found++;

				if( globs_found > UINT16_MAX )
				{
					LIBEWF_WARNING_PRINT( "ewfglob_resolve: too many globs found.\n" );

					return( -1 );
				}
			}
			while( ewfglob_findnext( find_handle, &find_data ) == 0 );

			if( errno != ENOENT )
			{
				LIBEWF_WARNING_PRINT( "ewfglob_resolve: error finding next file entry.\n" );

				return( -1 );
			}
			if( ewfglob_findclose( find_handle ) != 0 )
			{
				LIBEWF_WARNING_PRINT( "ewfglob_resolve: error closing find handle.\n" );

				return( -1 );
			}
		}
		else if( errno != ENOENT )
		{
			LIBEWF_WARNING_PRINT( "ewfglob_resolve: error finding file entry.\n" );

			return( -1 );
		}
#endif
	}
	return( globs_found );
}

#endif

