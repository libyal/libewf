/*
 * Globbing functions for the ewftools
 *
 * Copyright (c) 2006-2008, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <memory.h>
#include <notify.h>
#include <system_string.h>
#include <types.h>

#include <errno.h>

#if defined( HAVE_STDLIB_H )
#include <stdlib.h>
#endif

#if !defined( HAVE_GLOB_H ) && defined( HAVE_IO_H )
#include <io.h>
#endif

#include <libewf.h>

#include "ewfglob.h"

#if !defined( HAVE_GLOB_H )

#if defined( HAVE_WIDE_SYSTEM_CHARACTER_T )

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
ewfglob_t *ewfglob_alloc(
            void )
{
	ewfglob_t *glob       = NULL;
	static char *function = "ewfglob_alloc";

	glob = (ewfglob_t *) memory_allocate(
	                      sizeof( ewfglob_t ) );

	if( glob == NULL )
	{
		notify_warning_printf( "%s: unable to allocate glob.\n",
		 function );

		return( NULL );
	}
	if( memory_set(
	     glob,
	     0,
	     sizeof( ewfglob_t ) ) == NULL )
	{
		notify_warning_printf( "%s: unable to clear glob.\n",
		 function );

		memory_free(
		 glob );

		return( NULL );
	}
	glob->results = NULL;
	glob->amount  = 0;

	return( glob );
}

/* Reallocates memory for a glob
 * Returns a pointer to the instance, NULL on error
 */
ewfglob_t *ewfglob_realloc(
            ewfglob_t *glob,
            uint16_t new_amount )
{
	void *reallocation    = NULL;
	static char *function = "ewfglob_realloc";
	size_t previous_size  = 0;
	size_t new_size       = 0;

	if( glob == NULL )
	{
		notify_warning_printf( "%s: invalid glob.\n",
		 function );

		return( NULL );
	}
	if( glob->amount >= new_amount )
	{
		notify_warning_printf( "%s: new amount less equal than current amount.\n",
		 function );

		return( NULL );
	}
	previous_size = sizeof( system_character_t * ) * glob->amount;
	new_size      = sizeof( system_character_t * ) * new_amount;

	if( ( previous_size > (size_t) SSIZE_MAX )
	 || ( new_size > (size_t) SSIZE_MAX ) )
	{
		notify_warning_printf( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( NULL );
	}
	if( glob->amount == 0 )
	{
		reallocation = memory_allocate(
		                new_size );
	}
	else
	{
		reallocation = memory_reallocate(
		                glob->results,
		                new_size );
	}
	if( reallocation == NULL )
	{
		notify_warning_printf( "%s: unable to reallocate glob results.\n",
		 function );

		return( NULL );
	}
	glob->results = (system_character_t **) reallocation;

	if( memory_set(
	     &( glob->results[ glob->amount ] ),
	     0,
	     ( new_size - previous_size ) ) == NULL )
	{
		notify_warning_printf( "%s: unable to clear glob.\n",
		 function );

		return( NULL );
	}
	glob->amount  = new_amount;

	return( glob );
}

/* Frees memory of a glob
 */
void ewfglob_free(
      ewfglob_t *glob )
{
	static char *function = "ewfglob_free";
	uint16_t iterator     = 0;

	if( glob == NULL )
	{
		notify_warning_printf( "%s: invalid glob.\n",
		 function );

		return;
	}
	if( glob->results != NULL )
	{
		for( iterator = 0; iterator < glob->amount; iterator++ )
		{
			if( glob->results[ iterator ] != NULL )
			{
				memory_free(
				 glob->results[ iterator ] );
			}
		}
		memory_free(
		 glob->results );
	}
	memory_free(
	 glob );
}

/* Resolves filenames with wildcards (globs)
 * Returns the amount of result if successful, -1 on error
 */
int32_t ewfglob_resolve(
         ewfglob_t *glob,
         system_character_t * const patterns[],
         uint32_t amount_of_patterns )
{
#if defined( HAVE_WINDOWS_API )
	struct ewfglob_finddata_t find_data;

	system_character_t find_path[ _MAX_PATH ];
	system_character_t find_drive[ _MAX_DRIVE ];
	system_character_t find_directory[ _MAX_DIR ];
	system_character_t find_name[ _MAX_FNAME ];
	system_character_t find_extension[ _MAX_EXT ];

	intptr_t find_handle  = 0;
#endif
	ewfglob_t *reallocation = NULL;
	static char *function   = "ewfglob_resolve";
	int32_t globs_found     = 0;
	uint32_t iterator       = 0;

	if( glob == NULL )
	{
		notify_warning_printf( "%s: invalid glob.\n",
		 function );

		return( -1 );
	}
	for( iterator = 0; iterator < amount_of_patterns; iterator++ )
	{
		if( patterns[ iterator ] == NULL )
		{
			notify_warning_printf( "%s: invalid pattern.\n",
			 function );

			return( -1 );
		}
#if defined( HAVE_WINDOWS_API )
		if( ewfglob_splitpath(
		     patterns[ iterator ],
		     find_drive,
		     _MAX_DRIVE,
		     find_directory,
		     _MAX_DIR,
		     find_name,
		     _MAX_FNAME,
		     find_extension,
		     _MAX_EXT ) != 0 )
		{
			notify_warning_printf( "%s: unable to split path.\n",
			 function );

			return( -1 );
		}
		find_handle = ewfglob_findfirst(
		               patterns[ iterator ],
		               &find_data );

		if( find_handle != -1 )
		{
			do
			{
				reallocation = ewfglob_realloc(
				                glob,
				                ( glob->amount + 1 ) );

				if( reallocation == NULL )
				{
					notify_warning_printf( "%s: unable to reallocate glob.\n",
					 function );

					return( -1 );
				}
				if( ewfglob_makepath(
				     find_path,
				     _MAX_PATH,
				     find_drive,
				     find_directory,
				     find_data.name,
				     _SYSTEM_CHARACTER_T_STRING( "" )  ) != 0 )
				{
					notify_warning_printf( "%s: unable to make path.\n",
					 function );

					return( -1 );
				}
				glob->results[ glob->amount - 1 ] = system_string_duplicate(
				                                     find_path,
				                                     system_string_length(
				                                      find_path ) );

				globs_found++;

				if( globs_found > (int32_t) UINT16_MAX )
				{
					notify_warning_printf( "%s: too many globs found.\n",
					 function );

					return( -1 );
				}
			}
			while( ewfglob_findnext(
			        find_handle,
			        &find_data ) == 0 );

			if( errno != ENOENT )
			{
				notify_warning_printf( "%s: error finding next file entry.\n",
				 function );

				return( -1 );
			}
			if( ewfglob_findclose(
			     find_handle ) != 0 )
			{
				notify_warning_printf( "%s: error closing find handle.\n",
				 function );

				return( -1 );
			}
		}
		else if( errno != ENOENT )
		{
			notify_warning_printf( "%s: error finding file entry.\n",
			 function );

			return( -1 );
		}
#endif
	}
	return( globs_found );
}

#endif

