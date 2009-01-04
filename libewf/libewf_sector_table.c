/*
 * Error sector table definition for CRC and acquiry read errrors
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
#include <memory.h>

#include "libewf_includes.h"

#include "libewf_common.h"
#include "libewf_notify.h"
#include "libewf_sector_table.h"

/* Allocates memory for a sector table struct
 * Returns a pointer to the new instance or NULL on error
 */
libewf_sector_table_t *libewf_sector_table_alloc(
                        uint32_t amount )
{
	libewf_sector_table_t *sector_table = NULL;
	static char *function               = "libewf_sector_table_alloc";

	sector_table = (libewf_sector_table_t *) memory_allocate(
	                                          sizeof( libewf_sector_table_t ) );

	if( sector_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate sector table.\n",
		 function );

		return( NULL );
	}
	sector_table->sector = NULL;

	if( amount > 0 )
	{
		sector_table->sector = (libewf_sector_table_entry_t *) memory_allocate(
		                                                        sizeof( libewf_sector_table_entry_t ) * amount );

		if( sector_table->sector == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to allocate dynamic sector array.\n",
			 function );

			libewf_common_free( sector_table );

			return( NULL );
		}
		if( libewf_common_memset(
		     sector_table->sector,
		     0, 
		     ( sizeof( libewf_sector_table_entry_t ) * amount ) ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to clear dynamic sector array.\n",
			 function );

			libewf_common_free(
			 sector_table->sector );
			libewf_common_free(
			 sector_table );

			return( NULL );
		} 
	}
	sector_table->amount = amount;

	return( sector_table );
}

/* Reallocates memory for the sector table values
 * Returns 1 if successful or -1 on error
 */
int libewf_sector_table_realloc(
     libewf_sector_table_t *sector_table,
     uint32_t amount )
{
	void *reallocation    = NULL;
	static char *function = "libewf_sector_table_realloc";

	if( sector_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid sector table.\n",
		 function );

		return( -1 );
	}
	if( sector_table->amount >= amount )
	{
		LIBEWF_WARNING_PRINT( "%s: new amount must be greater than previous amount.\n",
		 function );

		return( -1 );
	}
	reallocation = libewf_common_realloc(
	                sector_table->sector,
	                ( sizeof( libewf_sector_table_entry_t ) * amount ) );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to reallocate dynamic sector array.\n",
		 function );

		return( -1 );
	}
	sector_table->sector = (libewf_sector_table_entry_t *) reallocation;

	if( libewf_common_memset(
	     &( sector_table->sector[ sector_table->amount ] ),
	     0, 
	     ( sizeof( libewf_sector_table_entry_t ) * ( amount - sector_table->amount ) ) ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to clear dynamic sector array.\n",
		 function );

		return( 1 );
	} 
	sector_table->amount = amount;

	return( 1 );
}

/* Frees memory of a sector table struct including elements
 */
void libewf_sector_table_free(
      libewf_sector_table_t *sector_table )
{
	static char *function = "libewf_sector_table_free";

	if( sector_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid sector table.\n",
		 function );

		return;
	}
	if( sector_table->sector != NULL )
	{
		libewf_common_free(
		 sector_table->sector );
	}
	libewf_common_free(
	 sector_table );
}

/* Retrieves the information of a sector
 * Returns 1 if successful, 0 if no sector could be found or -1 on error
 */
int libewf_sector_table_get_sector(
     libewf_sector_table_t *sector_table,
     uint32_t index,
     off64_t *first_sector,
     uint32_t *amount_of_sectors )
{
	static char *function = "libewf_sector_table_get_sector";

	if( sector_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid sector table.\n",
		 function );

		return( -1 );
	}
	if( sector_table->amount == 0 )
	{
		return( 0 );
	}
	if( sector_table->sector == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid sector table - missing sectors.\n",
		 function );

		return( -1 );
	}
	if( first_sector == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid first sector.\n",
		 function );

		return( -1 );
	}
	if( amount_of_sectors == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid amount of sectors.\n",
		 function );

		return( -1 );
	}
	if( index >= sector_table->amount )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid index out of range.\n",
		 function );

		return( -1 );
	}
	*first_sector      = sector_table->sector[ index ].first_sector;
	*amount_of_sectors = sector_table->sector[ index ].amount_of_sectors;

	return( 1 );
}

/* Add a sector
 * Returns 1 if successful, or -1 on error
 */
int libewf_sector_table_add_sector(
     libewf_sector_table_t *sector_table,
     off64_t first_sector,
     uint32_t amount_of_sectors,
     int merge_continious_entries )
{
	libewf_sector_table_entry_t *reallocation = NULL;
	static char *function                     = "libewf_sector_table_add_sector";
	off64_t last_sector                       = 0;
	off64_t last_range_sector                 = 0;
	uint32_t iterator                         = 0;

	if( sector_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid sector table.\n",
		 function );

		return( -1 );
	}
	if( first_sector < 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid first sector value is below zero.\n",
		 function );

		return( -1 );
	}
	if( ( merge_continious_entries != 0 )
	 && ( sector_table->sector != NULL ) )
	{
		/* Check if the sector is already in the table
		 */
		for( iterator = 0; iterator < sector_table->amount; iterator++ )
		{
			last_range_sector = sector_table->sector[ iterator ].first_sector
			                  + sector_table->sector[ iterator ].amount_of_sectors;

			if( ( first_sector >= sector_table->sector[ iterator ].first_sector )
			 && ( first_sector <= last_range_sector ) )
			{
				/* Merge current sector with existing
				 */
				last_sector = first_sector + amount_of_sectors;

				if( last_sector > last_range_sector )
				{
					sector_table->sector[ iterator ].amount_of_sectors += (uint32_t) ( last_sector - last_range_sector );
				}
				return( 1 );
			}
		}
	}
	/* Create a new sector
	 */
	reallocation = (libewf_sector_table_entry_t *) libewf_common_realloc(
	                                                sector_table->sector,
	                                                ( sizeof( libewf_sector_table_entry_t ) * ( sector_table->amount + 1 ) ) );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create sectors.\n",
		 function );

		return( -1 );
	}
	sector_table->sector = reallocation;

	sector_table->sector[ sector_table->amount ].first_sector      = first_sector;
	sector_table->sector[ sector_table->amount ].amount_of_sectors = amount_of_sectors;

	sector_table->amount += 1;

	return( 1 );
}

