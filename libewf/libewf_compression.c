/*
 * Compression handling functions
 *
 * Copyright (C) 2006-2020, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( HAVE_LIBBZ2 ) || defined( BZIP2_DLL )
#include <bzlib.h>
#endif

#if defined( HAVE_ZLIB ) || defined( ZLIB_DLL )
#include <zlib.h>
#endif

#include "libewf_compression.h"
#include "libewf_definitions.h"
#include "libewf_deflate.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"

/* Compresses data using the compression method
 * Returns 1 on success, 0 if buffer is too small or -1 on error
 */
int libewf_compress_data(
     uint8_t *compressed_data,
     size_t *compressed_data_size,
     uint16_t compression_method,
     int8_t compression_level,
     const uint8_t *uncompressed_data,
     size_t uncompressed_data_size,
     libcerror_error_t **error )
{
	static char *function                   = "libewf_compress_data";
	int result                              = 0;

#if defined( HAVE_LIBBZ2 ) || defined( BZIP2_DLL )
	unsigned int bzip2_compressed_data_size = 0;
	int bzip2_compression_level             = 0;
#endif
#if ( defined( HAVE_ZLIB ) && defined( HAVE_ZLIB_COMPRESS2 ) ) || defined( ZLIB_DLL )
	uLongf zlib_compressed_data_size        = 0;
	int zlib_compression_level              = 0;
#endif

	if( compressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data buffer.",
		 function );

		return( -1 );
	}
	if( compressed_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data size.",
		 function );

		return( -1 );
	}
	if( uncompressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed data buffer.",
		 function );

		return( -1 );
	}
	if( compressed_data == uncompressed_data )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed data buffer equals compressed data buffer.",
		 function );

		return( -1 );
	}
	if( compression_method == LIBEWF_COMPRESSION_METHOD_DEFLATE )
	{
#if ( defined( HAVE_ZLIB ) && defined( HAVE_ZLIB_COMPRESS2 ) ) || defined( ZLIB_DLL )
		if( compression_level == LIBEWF_COMPRESSION_LEVEL_DEFAULT )
		{
			zlib_compression_level = Z_DEFAULT_COMPRESSION;
		}
		else if( compression_level == LIBEWF_COMPRESSION_LEVEL_FAST )
		{
			zlib_compression_level = Z_BEST_SPEED;
		}
		else if( compression_level == LIBEWF_COMPRESSION_LEVEL_BEST )
		{
			zlib_compression_level = Z_BEST_COMPRESSION;
		}
		else if( compression_level == LIBEWF_COMPRESSION_LEVEL_NONE )
		{
			zlib_compression_level = Z_NO_COMPRESSION;
		}
		else
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported compression level.",
			 function );

			return( -1 );
		}
#if ULONG_MAX < SSIZE_MAX
		if( *compressed_data_size > (size_t) ULONG_MAX )
#else
		if( *compressed_data_size > (size_t) SSIZE_MAX )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid compressed data size value exceeds maximum.",
			 function );

			return( -1 );
		}
#if ULONG_MAX < SSIZE_MAX
		if( uncompressed_data_size > (size_t) ULONG_MAX )
#else
		if( uncompressed_data_size > (size_t) SSIZE_MAX )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid uncompressed data size value exceeds maximum.",
			 function );

			return( -1 );
		}
		zlib_compressed_data_size = (uLongf) *compressed_data_size;

		result = compress2(
			  (Bytef *) compressed_data,
			  &zlib_compressed_data_size,
			  (Bytef *) uncompressed_data,
			  (uLong) uncompressed_data_size,
			  zlib_compression_level );

		if( result == Z_OK )
		{
			*compressed_data_size = (size_t) zlib_compressed_data_size;

			result = 1;
		}
		else if( result == Z_BUF_ERROR )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to write compressed data: target buffer too small.\n",
				 function );
			}
#endif
#if defined( HAVE_COMPRESS_BOUND ) || defined( WINAPI )
			/* Use compressBound to determine the size of the uncompressed buffer
			 */
			zlib_compressed_data_size = compressBound( (uLong) uncompressed_data_size );
			*compressed_data_size     = (size_t) zlib_compressed_data_size;
#else
			/* Estimate that a factor 2 enlargement should suffice
			 */
			*compressed_data_size *= 2;
#endif
			result = 0;
		}
		else if( result == Z_MEM_ERROR )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to write compressed data: insufficient memory.",
			 function );

			*compressed_data_size = 0;

			result = -1;
		}
		else
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
			 LIBCERROR_COMPRESSION_ERROR_COMPRESS_FAILED,
			 "%s: zlib returned undefined error: %d.",
			 function,
			 result );

			*compressed_data_size = 0;

			result = -1;
		}
#else
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: missing support for deflate compression.",
		 function );

		return( -1 );
#endif /* ( defined( HAVE_ZLIB ) && defined( HAVE_ZLIB_COMPRESS2 ) ) || defined( ZLIB_DLL ) */
	}
	else if( compression_method == LIBEWF_COMPRESSION_METHOD_BZIP2 )
	{
#if defined( HAVE_LIBBZ2 ) || defined( BZIP2_DLL )
		if( ( compression_level == LIBEWF_COMPRESSION_LEVEL_DEFAULT )
		 || ( compression_level == LIBEWF_COMPRESSION_LEVEL_FAST ) )
		{
			bzip2_compression_level = 1;
		}
		else if( compression_level == LIBEWF_COMPRESSION_LEVEL_BEST )
		{
			bzip2_compression_level = 9;
		}
		else
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported compression level.",
			 function );

			return( -1 );
		}
		if( *compressed_data_size > (size_t) UINT_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid compressed data size value exceeds maximum.",
			 function );

			return( -1 );
		}
		if( uncompressed_data_size > (size_t) UINT_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid uncompressed data size value exceeds maximum.",
			 function );

			return( -1 );
		}
		bzip2_compressed_data_size = (unsigned int) *compressed_data_size;

		result = BZ2_bzBuffToBuffCompress(
			  (char *) compressed_data,
			  &bzip2_compressed_data_size,
			  (char *) uncompressed_data,
			  (unsigned int) uncompressed_data_size,
			  bzip2_compression_level,
			  0,
			  30 );

		if( result == BZ_OK )
		{
			*compressed_data_size = (size_t) bzip2_compressed_data_size;

			result = 1;
		}
		else if( result == BZ_OUTBUFF_FULL )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
			 	"%s: unable to write compressed data: target buffer too small.\n",
				 function );
			}
#endif
			/* Estimate that a factor 2 enlargement should suffice
			 */
			*compressed_data_size *= 2;

			result = 0;
		}
		else if( result == BZ_MEM_ERROR )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to write compressed data: insufficient memory.",
			 function );

			*compressed_data_size = 0;

			result = -1;
		}
		else
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
			 LIBCERROR_COMPRESSION_ERROR_COMPRESS_FAILED,
			 "%s: libbz2 returned undefined error: %d.",
			 function,
			 result );

			*compressed_data_size = 0;

			result = -1;
		}
#else
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: missing support for bzip2 compression.",
		 function );

		return( -1 );
#endif /* defined( HAVE_LIBBZ2 ) || defined( BZIP2_DLL ) */
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported compression method.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Decompresses data using the compression method
 * Returns 1 on success, 0 on failure or -1 on error
 */
int libewf_decompress_data(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     uint16_t compression_method,
     uint8_t *uncompressed_data,
     size_t *uncompressed_data_size,
     libcerror_error_t **error )
{
	static char *function                     = "libewf_decompress_data";
	int result                                = 0;

#if defined( HAVE_LIBBZ2 ) || defined( BZIP2_DLL )
	unsigned int bzip2_uncompressed_data_size = 0;
#endif
#if ( defined( HAVE_ZLIB ) && defined( HAVE_ZLIB_UNCOMPRESS ) ) || defined( ZLIB_DLL )
	uLongf zlib_uncompressed_data_size        = 0;
#endif

	if( compressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data buffer.",
		 function );

		return( -1 );
	}
	if( uncompressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed data buffer.",
		 function );

		return( -1 );
	}
	if( uncompressed_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed data size.",
		 function );

		return( -1 );
	}
	if( uncompressed_data == compressed_data )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data buffer equals uncompressed data buffer.",
		 function );

		return( -1 );
	}
	if( compression_method == LIBEWF_COMPRESSION_METHOD_DEFLATE )
	{
#if ( defined( HAVE_ZLIB ) && defined( HAVE_ZLIB_UNCOMPRESS ) ) || defined( ZLIB_DLL )
#if ULONG_MAX < SSIZE_MAX
		if( compressed_data_size > (size_t) ULONG_MAX )
#else
		if( compressed_data_size > (size_t) SSIZE_MAX )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid compressed data size value exceeds maximum.",
			 function );

			return( -1 );
		}
#if ULONG_MAX < SSIZE_MAX
		if( *uncompressed_data_size > (size_t) ULONG_MAX )
#else
		if( *uncompressed_data_size > (size_t) SSIZE_MAX )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid uncompressed data size value exceeds maximum.",
			 function );

			return( -1 );
		}
		zlib_uncompressed_data_size = (uLongf) *uncompressed_data_size;

		result = uncompress(
			  (Bytef *) uncompressed_data,
			  &zlib_uncompressed_data_size,
			  (Bytef *) compressed_data,
			  (uLong) compressed_data_size );

		if( result == Z_OK )
		{
			*uncompressed_data_size = (size_t) zlib_uncompressed_data_size;

			result = 1;
		}
		else if( result == Z_DATA_ERROR )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to read compressed data: data error.\n",
				 function );
			}
#endif
			*uncompressed_data_size = 0;

			result = -1;
		}
		else if( result == Z_BUF_ERROR )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				"%s: unable to read compressed data: target buffer too small.\n",
				 function );
			}
#endif
			/* Estimate that a factor 2 enlargement should suffice
			 */
			*uncompressed_data_size *= 2;

			result = 0;
		}
		else if( result == Z_MEM_ERROR )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to read compressed data: insufficient memory.",
			 function );

			*uncompressed_data_size = 0;

			result = -1;
		}
		else
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
			 LIBCERROR_COMPRESSION_ERROR_DECOMPRESS_FAILED,
			 "%s: zlib returned undefined error: %d.",
			 function,
			 result );

			*uncompressed_data_size = 0;

			result = -1;
		}
#else
		result = libewf_deflate_decompress_zlib(
		          compressed_data,
		          compressed_data_size,
		          uncompressed_data,
		          uncompressed_data_size,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
			 LIBCERROR_ENCRYPTION_ERROR_GENERIC,
			 "%s: unable to decompress deflate compressed data.",
			 function );

			return( -1 );
		}
#endif /* ( defined( HAVE_ZLIB ) && defined( HAVE_ZLIB_UNCOMPRESS ) ) || defined( ZLIB_DLL ) */
	}
	else if( compression_method == LIBEWF_COMPRESSION_METHOD_BZIP2 )
	{
#if defined( HAVE_LIBBZ2 ) || defined( BZIP2_DLL )
		if( compressed_data_size > (size_t) UINT_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid compressed data size value exceeds maximum.",
			 function );

			return( -1 );
		}
		if( *uncompressed_data_size > (size_t) UINT_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid uncompressed data size value exceeds maximum.",
			 function );

			return( -1 );
		}
		bzip2_uncompressed_data_size = (unsigned int) *uncompressed_data_size;

		result = BZ2_bzBuffToBuffDecompress(
			  (char *) uncompressed_data,
			  &bzip2_uncompressed_data_size,
			  (char *) compressed_data,
			  (unsigned int) compressed_data_size,
			  0,
			  0 );

		if( result == BZ_OK )
		{
			*uncompressed_data_size = (size_t) bzip2_uncompressed_data_size;

			result = 1;
		}
		else if( ( result == BZ_DATA_ERROR )
		      || ( result == BZ_DATA_ERROR_MAGIC ) )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to read compressed data: data error.\n",
				 function );
			}
#endif
			*uncompressed_data_size = 0;

			result = -1;
		}
		else if( result == BZ_OUTBUFF_FULL )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				"%s: unable to read compressed data: target buffer too small.\n",
				 function );
			}
#endif
			/* Estimate that a factor 2 enlargement should suffice
			 */
			*uncompressed_data_size *= 2;

			result = 0;
		}
		else if( result == BZ_MEM_ERROR )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to read compressed data: insufficient memory.",
			 function );

			*uncompressed_data_size = 0;

			result = -1;
		}
		else
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
			 LIBCERROR_COMPRESSION_ERROR_DECOMPRESS_FAILED,
			 "%s: libbz2 returned undefined error: %d.",
			 function,
			 result );

			*uncompressed_data_size = 0;

			result = -1;
		}
#else
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: missing support for bzip2 compression.",
		 function );

		return( -1 );
#endif /* defined( HAVE_LIBBZ2 ) || defined( BZIP2_DLL ) */
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported compression method.",
		 function );

		return( -1 );
	}
	return( result );
}

