/*
 * libewf offset table
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

#include "libewf_includes.h"

#include "libewf_common.h"
#include "libewf_notify.h"
#include "libewf_offset_table.h"

/* Allocates memory for a new offset table struct
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_OFFSET_TABLE *libewf_offset_table_alloc( uint32_t amount )
{
	LIBEWF_OFFSET_TABLE *offset_table = NULL;
	static char *function             = "libewf_offset_table_alloc";

	offset_table = (LIBEWF_OFFSET_TABLE *) libewf_common_alloc( LIBEWF_OFFSET_TABLE_SIZE );

	if( offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate offset table.\n",
		 function );

		return( NULL );
	}
	offset_table->file_descriptor = (int *) libewf_common_alloc_cleared( ( amount * LIBEWF_OFFSET_TABLE_FILE_DESCRIPTOR_SIZE ), -1 );

	if( offset_table->file_descriptor == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate file descriptors.\n",
		 function );

		libewf_common_free( offset_table );

		return( NULL );
	}
	offset_table->compressed = (uint8_t *) libewf_common_alloc_cleared( ( amount * LIBEWF_OFFSET_TABLE_COMPRESSED_SIZE ), -1 );

	if( offset_table->compressed == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate compressed flags.\n",
		 function );

		libewf_common_free( offset_table->file_descriptor );
		libewf_common_free( offset_table );

		return( NULL );
	}
	offset_table->offset = (off_t *) libewf_common_alloc_cleared( ( amount * LIBEWF_OFFSET_TABLE_OFFSET_SIZE ), 0 );

	if( offset_table->offset == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate offsets.\n",
		 function );

		libewf_common_free( offset_table->file_descriptor );
		libewf_common_free( offset_table->compressed );
		libewf_common_free( offset_table );

		return( NULL );
	}
	offset_table->size = (size_t *) libewf_common_alloc_cleared( ( amount * LIBEWF_OFFSET_TABLE_SIZE_SIZE ), 0 );

	if( offset_table->size == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate sizes.\n",
		 function );

		libewf_common_free( offset_table->file_descriptor );
		libewf_common_free( offset_table->compressed );
		libewf_common_free( offset_table->offset );
		libewf_common_free( offset_table );

		return( NULL );
	}
	offset_table->hashed = (uint8_t *) libewf_common_alloc_cleared( ( amount * LIBEWF_OFFSET_TABLE_HASHED_SIZE ), -1 );

	if( offset_table->hashed == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate hashed flags.\n",
		 function );

		libewf_common_free( offset_table->size );
		libewf_common_free( offset_table->file_descriptor );
		libewf_common_free( offset_table->compressed );
		libewf_common_free( offset_table->offset );
		libewf_common_free( offset_table );

		return( NULL );
	}
	offset_table->segment_number = (uint16_t *) libewf_common_alloc_cleared( ( amount * LIBEWF_OFFSET_TABLE_SEGMENT_NUMBER_SIZE ), 0 );

	if( offset_table->segment_number == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate segment numbers.\n",
		 function );

		libewf_common_free( offset_table->size );
		libewf_common_free( offset_table->file_descriptor );
		libewf_common_free( offset_table->compressed );
		libewf_common_free( offset_table->offset );
		libewf_common_free( offset_table->hashed );
		libewf_common_free( offset_table );

		return( NULL );
	}
	offset_table->dirty = (uint8_t *) libewf_common_alloc_cleared( ( amount * LIBEWF_OFFSET_TABLE_DIRTY_SIZE ), 0 );

	if( offset_table->dirty == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate dirty flags.\n",
		 function );

		libewf_common_free( offset_table->size );
		libewf_common_free( offset_table->file_descriptor );
		libewf_common_free( offset_table->compressed );
		libewf_common_free( offset_table->offset );
		libewf_common_free( offset_table->hashed );
		libewf_common_free( offset_table->segment_number );
		libewf_common_free( offset_table );

		return( NULL );
	}
	offset_table->amount = amount;
        offset_table->last   = 0;

	return( offset_table );
}

/* Reallocates memory for the dynamic file descriptor, offset and size array
 * Returns 1 if successful, or -1 on error
 */
int libewf_offset_table_realloc( LIBEWF_OFFSET_TABLE *offset_table, uint32_t amount )
{
	void *reallocation    = NULL;
	static char *function = "libewf_offset_table_realloc";

	if( offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid offset_table.\n",
		 function );

		return( -1 );
	}
	reallocation = libewf_common_realloc_new_cleared(
	                offset_table->file_descriptor,
	                ( offset_table->amount * LIBEWF_OFFSET_TABLE_FILE_DESCRIPTOR_SIZE ),
	                ( amount * LIBEWF_OFFSET_TABLE_FILE_DESCRIPTOR_SIZE ),
	                -1 );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to reallocate file descriptors.\n",
		 function );

		return( -1 );
	}
	offset_table->file_descriptor = (int *) reallocation;
	reallocation                  = libewf_common_realloc_new_cleared(
	                                 offset_table->compressed,
	                                 ( offset_table->amount * LIBEWF_OFFSET_TABLE_COMPRESSED_SIZE ),
	                                 ( amount * LIBEWF_OFFSET_TABLE_COMPRESSED_SIZE ),
	                                 -1 );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to reallocate compressed flags.\n",
		 function );

		return( -1 );
	}
	offset_table->compressed = (uint8_t *) reallocation;
	reallocation             = libewf_common_realloc_new_cleared(
	                            offset_table->offset,
	                            ( offset_table->amount * LIBEWF_OFFSET_TABLE_OFFSET_SIZE ),
	                            ( amount * LIBEWF_OFFSET_TABLE_OFFSET_SIZE ),
	                            0 );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to reallocate offsets.\n",
		 function );

		return( -1 );
	}
	offset_table->offset = (off_t *) reallocation;
	reallocation         = libewf_common_realloc_new_cleared(
	                        offset_table->size,
	                        ( offset_table->amount * LIBEWF_OFFSET_TABLE_SIZE_SIZE ),
	                        ( amount * LIBEWF_OFFSET_TABLE_SIZE_SIZE ),
	                        0 );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to reallocate sizes.\n",
		 function );

		return( -1 );
	}
	offset_table->size   = (size_t *) reallocation;
	reallocation         = libewf_common_realloc_new_cleared(
	                        offset_table->hashed,
	                        ( offset_table->amount * LIBEWF_OFFSET_TABLE_HASHED_SIZE ),
	                        ( amount * LIBEWF_OFFSET_TABLE_HASHED_SIZE ),
	                        -1 );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to reallocate hashed flags.\n",
		 function );

		return( -1 );
	}
	offset_table->hashed = (uint8_t *) reallocation;
	reallocation         = libewf_common_realloc_new_cleared(
	                        offset_table->segment_number,
	                        ( offset_table->amount * LIBEWF_OFFSET_TABLE_SEGMENT_NUMBER_SIZE ),
	                        ( amount * LIBEWF_OFFSET_TABLE_SEGMENT_NUMBER_SIZE ),
	                        0 );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to reallocate segment numbers.\n",
		 function );

		return( -1 );
	}
	offset_table->segment_number = (uint16_t *) reallocation;
	reallocation                 = libewf_common_realloc_new_cleared(
	                                offset_table->dirty,
	                                ( offset_table->amount * LIBEWF_OFFSET_TABLE_DIRTY_SIZE ),
	                                ( amount * LIBEWF_OFFSET_TABLE_DIRTY_SIZE ),
	                                0 );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to reallocate dirty flags.\n",
		 function );

		return( -1 );
	}
	offset_table->dirty  = (uint8_t *) reallocation;
	offset_table->amount = amount;

	return( 1 );
}

/* Frees memory of a offset table struct including elements
 */
void libewf_offset_table_free( LIBEWF_OFFSET_TABLE *offset_table )
{
	static char *function = "libewf_offset_table_free";

	if( offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid offset table.\n",
		 function );

		return;
	}
	libewf_common_free( offset_table->file_descriptor );
	libewf_common_free( offset_table->compressed );
	libewf_common_free( offset_table->offset );
	libewf_common_free( offset_table->size );
	libewf_common_free( offset_table->hashed );
	libewf_common_free( offset_table->segment_number );
	libewf_common_free( offset_table->dirty );
	libewf_common_free( offset_table );
}

/* Sets the values for a specific offset
 * Returns 1 if successful, or -1 on error
 */
int8_t libewf_offset_table_set_values( LIBEWF_OFFSET_TABLE *offset_table, uint32_t chunk, int file_descriptor, uint8_t compressed, off_t offset, size_t size, uint16_t segment_number, uint8_t dirty )
{
	static char *function = "libewf_offset_table_set_values";

	if( offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid offset table.\n",
		 function );

		return( -1 );
	}
	if( offset_table->file_descriptor == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid offset table - missing file descriptors.\n",
		 function );

		return( -1 );
	}
	if( offset_table->compressed == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid offset table - missing compressed values.\n",
		 function );

		return( -1 );
	}
	if( offset_table->offset == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid offset table - missing offsets.\n",
		 function );

		return( -1 );
	}
	if( offset_table->size == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid offset table - missing sizes.\n",
		 function );

		return( -1 );
	}
	if( offset_table->segment_number == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid offset table - missing segment numbers.\n",
		 function );

		return( -1 );
	}
	if( offset_table->dirty == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid offset table - missing dirty values.\n",
		 function );

		return( -1 );
	}
	if( chunk > offset_table->amount )
	{
		LIBEWF_WARNING_PRINT( "%s: chunk: %" PRIu32 " not in offset table.\n",
		 function, chunk );

		return( -1 );
	}
	offset_table->file_descriptor[ chunk ] = file_descriptor;
	offset_table->compressed[ chunk ]      = compressed;
	offset_table->offset[ chunk ]          = offset;
	offset_table->size[ chunk ]            = size;
	offset_table->segment_number[ chunk ]  = segment_number;
	offset_table->dirty[ chunk ]           = dirty;

	return( 1 );
}

