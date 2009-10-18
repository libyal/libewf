/*
 * Empty block test functions
 *
 * Copyright (c) 2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations.
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
#include <types.h>

#include <liberror.h>

#include "libewf_empty_block.h"

/* The largest primary (or scalar) available
 * supported by a single load and store instruction
 */
typedef unsigned long int libewf_aligned_t;

/* Check for empty block
 * An emtpy block is a block that contains the same value for every byte
 * Returns 1 if block is empty, 0 if not or -1 on error
 */
int libewf_empty_block_test(
     const uint8_t *block_buffer,
     size_t block_size,
     liberror_error_t **error )
{
	libewf_aligned_t *aligned_block_iterator = NULL;
	libewf_aligned_t *aligned_block_start    = NULL;
	uint8_t *block_iterator                  = NULL;
	uint8_t *block_start                     = NULL;
	static char *function                    = "libewf_empty_block_test";

	if( block_buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block buffer.",
		 function );

		return( -1 );
	}
	if( block_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid block size value exceeds maximum.",
		 function );

		return( -1 );
	}
	block_start    = (uint8_t *) block_buffer;
	block_iterator = (uint8_t *) block_buffer + 1;
	block_size    -= 1;

	/* Only optimize for blocks larger than the alignment
	 */
	if( block_size > ( 2 * sizeof( libewf_aligned_t ) ) )
	{
		/* Align the block start
		 */
		while( ( (intptr_t) block_start % sizeof( libewf_aligned_t ) ) != 0 )
		{
			if( *block_start != *block_iterator )
			{
				return( 0 );
			}
			block_start    += 1;
			block_iterator += 1;
			block_size     -= 1;
		}
		/* Align the block iterator
		 */
		while( ( (intptr_t) block_iterator % sizeof( libewf_aligned_t ) ) != 0 )
		{
			if( *block_start != *block_iterator )
			{
				return( 0 );
			}
			block_iterator += 1;
			block_size     -= 1;
		}
		aligned_block_start    = (libewf_aligned_t *) block_start;
		aligned_block_iterator = (libewf_aligned_t *) block_iterator;

		while( block_size > sizeof( libewf_aligned_t ) )
		{
			if( *aligned_block_start != *aligned_block_iterator )
			{
				return( 0 );
			}
			aligned_block_iterator += 1;
			block_size             -= sizeof( libewf_aligned_t );
		}
		block_iterator = (uint8_t *) aligned_block_iterator;
	}
	while( block_size != 0 )
	{
		if( *block_start != *block_iterator )
		{
			return( 0 );
		}
		block_iterator += 1;
		block_size     -= 1;
	}
	return( 1 );
}

