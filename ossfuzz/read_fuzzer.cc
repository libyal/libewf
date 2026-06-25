/*
 * OSS-Fuzz target for the libewf read-buffer (data) path
 *
 * The existing handle_fuzzer only opens and closes the image, so the chunk
 * table / section descriptor parsing and the per-chunk (zlib / bzip2)
 * decompression of the EWF/EWF2 read path are never reached. This target
 * additionally reads the media so those parsers are exercised.
 */

#include <stddef.h>
#include <stdint.h>

/* Note that some of the OSS-Fuzz engines use C++
 */
extern "C" {

#include "ossfuzz_libbfio.h"
#include "ossfuzz_libewf.h"

#if !defined( LIBEWF_HAVE_BFIO )

LIBEWF_EXTERN \
int libewf_handle_open_file_io_pool(
     libewf_handle_t *handle,
     libbfio_pool_t *file_io_pool,
     int access_flags,
     libewf_error_t **error );

#endif /* !defined( LIBEWF_HAVE_BFIO ) */

int LLVMFuzzerTestOneInput(
     const uint8_t *data,
     size_t size )
{
	libbfio_handle_t *file_io_handle = NULL;
	libbfio_pool_t *file_io_pool     = NULL;
	libewf_handle_t *handle          = NULL;
	int entry_index                  = 0;

	if( libbfio_memory_range_initialize(
	     &file_io_handle,
	     NULL ) != 1 )
	{
		return( 0 );
	}
	if( libbfio_memory_range_set(
	     file_io_handle,
	     (uint8_t *) data,
	     size,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libbfio_pool_initialize(
	     &file_io_pool,
	     0,
	     0,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libbfio_pool_append_handle(
	     file_io_pool,
	     &entry_index,
	     file_io_handle,
	     LIBBFIO_OPEN_READ,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	/* The file IO pool takes over management of the file IO handle
	 */
	file_io_handle = NULL;

	if( libewf_handle_initialize(
	     &handle,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libewf_handle_open_file_io_pool(
	     handle,
	     file_io_pool,
	     LIBEWF_OPEN_READ,
	     NULL ) != 1 )
	{
		goto on_error_libewf;
	}
	/* Read the media to drive the chunk table parsing / decompression. */
	{
		size64_t media_size = 0;

		if( libewf_handle_get_media_size(
		     handle,
		     &media_size,
		     NULL ) == 1 )
		{
			uint8_t buffer[ 4096 ];
			off64_t offset = 0;
			int iterations = 0;

			while( ( offset < (off64_t) media_size )
			    && ( iterations < 2048 ) )
			{
				ssize_t read_count = libewf_handle_read_buffer_at_offset(
				                      handle,
				                      buffer,
				                      sizeof( buffer ),
				                      offset,
				                      NULL );

				if( read_count <= 0 )
				{
					break;
				}
				offset += read_count;
				iterations++;
			}
		}
	}
	libewf_handle_close(
	 handle,
	 NULL );

on_error_libewf:
	libewf_handle_free(
	 &handle,
	 NULL );

on_error_libbfio:
	/* Note that on error the handle still has a reference to file_io_pool
	 * that will be closed. Therefore the file IO pool and handle need to
	 * be freed after closing or freeing the handle.
	 */
	if( file_io_pool != NULL )
	{
		libbfio_pool_free(
		 &file_io_pool,
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( 0 );
}

} /* extern "C" */
