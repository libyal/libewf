/*
 * Globbing functions
 *
 * Copyright (c) 2006-2009, Joachim Metz <forensics@hoffmannbv.nl>,
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
#include <types.h>

#include <errno.h>

#if defined( HAVE_STDLIB_H )
#include <stdlib.h>
#endif

#if defined( HAVE_IO_H )
#include <io.h>
#endif

#include "glob.h"
#include "notify.h"
#include "system_string.h"

#if !defined( HAVE_GLOB_H )

/* Initializes a new glob
 * Returns 1 if successful or -1 on error
 */
int glob_initialize(
     glob_t **glob )
{
	static char *function = "glob_initialize";

	if( glob == NULL )
	{
		notify_warning_printf( "%s: invalid glob.\n",
		 function );

		return( -1 );
	}
	if( *glob == NULL )
	{
		*glob = (glob_t *) memory_allocate(
		                       sizeof( glob_t ) );

		if( *glob == NULL )
		{
			notify_warning_printf( "%s: unable to allocate glob.\n",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *glob,
		     0,
		     sizeof( glob_t ) ) == NULL )
		{
			notify_warning_printf( "%s: unable to clear glob.\n",
			 function );

			memory_free(
			 *glob );

			*glob = NULL;

			return( -1 );
		}
		( *glob )->amount_of_results = 0;
		( *glob )->result            = NULL;
	}
	return( 1 );
}

/* Frees memory of a glob
 * Returns 1 if successful or -1 on error
 */
int glob_free(
     glob_t **glob )
{
	static char *function = "glob_free";
	int result_iterator   = 0;

	if( glob == NULL )
	{
		notify_warning_printf( "%s: invalid glob.\n",
		 function );

		return( -1 );
	}
	if( *glob != NULL )
	{
		if( ( *glob )->result != NULL )
		{
			for( result_iterator = 0; result_iterator < ( *glob )->amount_of_results; result_iterator++ )
			{
				if( ( *glob )->result[ result_iterator ] != NULL )
				{
					memory_free(
					 ( *glob )->result[ result_iterator ] );
				}
			}
			memory_free(
			 ( *glob )->result );
		}
		memory_free(
		 *glob );

		*glob = NULL;
	}
	return( 1 );
}

/* Resizes the glob
 * Returns 1 if successful or -1 on error
 */
int glob_resize(
     glob_t *glob,
     int new_amount_of_results )
{
	void *reallocation    = NULL;
	static char *function = "glob_resize";
	size_t previous_size  = 0;
	size_t new_size       = 0;

	if( glob == NULL )
	{
		notify_warning_printf( "%s: invalid glob.\n",
		 function );

		return( -1 );
	}
	if( glob->amount_of_results >= new_amount_of_results )
	{
		notify_warning_printf( "%s: new amount less equal than current amount.\n",
		 function );

		return( -1 );
	}
	previous_size = sizeof( system_character_t * ) * glob->amount_of_results;
	new_size      = sizeof( system_character_t * ) * new_amount_of_results;

	if( ( previous_size > (size_t) SSIZE_MAX )
	 || ( new_size > (size_t) SSIZE_MAX ) )
	{
		notify_warning_printf( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	reallocation = memory_reallocate(
	                glob->result,
	                new_size );

	if( reallocation == NULL )
	{
		notify_warning_printf( "%s: unable to reallocate glob results.\n",
		 function );

		return( -1 );
	}
	glob->result = (system_character_t **) reallocation;

	if( memory_set(
	     &( glob->result[ glob->amount_of_results ] ),
	     0,
	     ( new_size - previous_size ) ) == NULL )
	{
		notify_warning_printf( "%s: unable to clear glob.\n",
		 function );

		return( -1 );
	}
	glob->amount_of_results = new_amount_of_results;

	return( 1 );
}

/* Resolves filenames with wildcards (globs)
 * Returns the amount of results if successful or -1 on error
 */
int glob_resolve(
     glob_t *glob,
     system_character_t * const patterns[],
     int amount_of_patterns )
{
#if defined( WINAPI )
	struct glob_finddata_t find_data;

	system_character_t find_path[ _MAX_PATH ];
	system_character_t find_drive[ _MAX_DRIVE ];
	system_character_t find_directory[ _MAX_DIR ];
	system_character_t find_name[ _MAX_FNAME ];
	system_character_t find_extension[ _MAX_EXT ];

	intptr_t find_handle    = 0;
#endif
	static char *function   = "glob_resolve";
	size_t find_path_length = 0;
	int globs_found         = 0;
	int iterator            = 0;

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
#if defined( WINAPI )
		if( glob_splitpath(
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
		find_handle = glob_findfirst(
		               patterns[ iterator ],
		               &find_data );

		if( find_handle != -1 )
		{
			do
			{
				if( glob_resize(
				     glob,
				     ( glob->amount_of_results + 1 ) ) != 1 )
				{
					notify_warning_printf( "%s: unable to resize glob.\n",
					 function );

					return( -1 );
				}
				if( glob_makepath(
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
				find_path_length = system_string_length(
				                    find_path );

				glob->result[ glob->amount_of_results - 1 ] = (system_character_t *) memory_allocate(
				                                                                      sizeof( system_character_t ) * ( find_path_length + 1 ) );

				if( glob->result[ glob->amount_of_results - 1 ] == NULL )
				{
					notify_warning_printf( "%s: unable to create glob result.\n",
					 function );

					return( -1 );
				}
				if( system_string_copy(
				     glob->result[ glob->amount_of_results - 1 ],
				     find_path,
				     find_path_length ) == NULL )
				{
					notify_warning_printf( "%s: unable to set glob result.\n",
					 function );

					memory_free(
					 glob->result[ glob->amount_of_results - 1 ] );

					glob->result[ glob->amount_of_results - 1 ] = NULL;

					return( -1 );
				}
				( glob->result[ glob->amount_of_results - 1 ] )[ find_path_length ] = 0;

				globs_found++;

				if( globs_found > (int32_t) UINT16_MAX )
				{
					notify_warning_printf( "%s: too many globs found.\n",
					 function );

					return( -1 );
				}
			}
			while( glob_findnext(
			        find_handle,
			        &find_data ) == 0 );

			if( errno != ENOENT )
			{
				notify_warning_printf( "%s: error finding next file entry.\n",
				 function );

				return( -1 );
			}
			if( glob_findclose(
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

