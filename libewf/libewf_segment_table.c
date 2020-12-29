/*
 * Segment table functions
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
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "libewf_definitions.h"
#include "libewf_libbfio.h"
#include "libewf_libcdata.h"
#include "libewf_libcerror.h"
#include "libewf_libclocale.h"
#include "libewf_libfdata.h"
#include "libewf_libuna.h"
#include "libewf_segment_file.h"
#include "libewf_segment_table.h"

/* Creates a segment table
 * Make sure the value segment_table is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_table_initialize(
     libewf_segment_table_t **segment_table,
     libewf_io_handle_t *io_handle,
     size64_t maximum_segment_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_segment_table_initialize";

	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( *segment_table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid segment table value already set.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	*segment_table = memory_allocate_structure(
	                  libewf_segment_table_t );

	if( *segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create segment table.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *segment_table,
	     0,
	     sizeof( libewf_segment_table_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear segment table.",
		 function );

		memory_free(
		 *segment_table );

		*segment_table = NULL;

		return( -1 );
	}
	if( libfdata_list_initialize(
	     &( ( *segment_table )->segment_files_list ),
	     (intptr_t *) io_handle,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_io_handle_free,
	     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libewf_io_handle_clone,
	     (int (*)(intptr_t *, intptr_t *, libfdata_list_element_t *, libfdata_cache_t *, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libewf_segment_file_read_element_data,
	     NULL,
	     LIBFDATA_DATA_HANDLE_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create segment files list.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &( ( *segment_table )->segment_files_cache ),
	     LIBEWF_MAXIMUM_CACHE_ENTRIES_SEGMENT_FILES,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create segment files cache.",
		 function );

		goto on_error;
	}
	( *segment_table )->maximum_segment_size = maximum_segment_size;

	return( 1 );

on_error:
	if( *segment_table != NULL )
	{
		if( ( *segment_table )->segment_files_list != NULL )
		{
			libfdata_list_free(
			 &( ( *segment_table )->segment_files_list ),
			 NULL );
		}
		memory_free(
		 *segment_table );

		*segment_table = NULL;
	}
	return( -1 );
}

/* Frees a segment table
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_table_free(
     libewf_segment_table_t **segment_table,
     libcerror_error_t **error )
{
	static char *function = "libewf_segment_table_free";
	int result            = 1;

	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( *segment_table != NULL )
	{
		if( libewf_segment_table_clear(
		     *segment_table,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to clear segment table.",
			 function );

			result = -1;
		}
		if( libfcache_cache_free(
		     &( ( *segment_table )->segment_files_cache ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free segment files cache.",
			 function );

			result = -1;
		}
		if( libfdata_list_free(
		     &( ( *segment_table )->segment_files_list ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free segment files list.",
			 function );

			result = -1;
		}
		memory_free(
		 *segment_table );

		*segment_table = NULL;
	}
	return( result );
}

/* Clears the segment table
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_table_clear(
     libewf_segment_table_t *segment_table,
     libcerror_error_t **error )
{
	static char *function = "libewf_segment_table_clear";

	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( segment_table->basename != NULL )
	{
		memory_free(
		 segment_table->basename );

		segment_table->basename = NULL;
	}
	if( libfdata_list_empty(
	     segment_table->segment_files_list,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty segment files list.",
		 function );

		return( -1 );
	}
	if( libfcache_cache_empty(
	     segment_table->segment_files_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty segment files cache.",
		 function );

		return( -1 );
	}
	segment_table->maximum_segment_size = 0;
	segment_table->number_of_segments   = 0;
	segment_table->current_segment_file = NULL;
	segment_table->flags                = 0;

	return( 1 );
}

/* Clones the segment table
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_table_clone(
     libewf_segment_table_t **destination_segment_table,
     libewf_segment_table_t *source_segment_table,
     libcerror_error_t **error )
{
	static char *function = "libewf_segment_table_clone";

	if( destination_segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination segment table.",
		 function );

		return( -1 );
	}
	if( *destination_segment_table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination segment table value already set.",
		 function );

		return( -1 );
	}
	if( source_segment_table == NULL )
	{
		*destination_segment_table = NULL;

		return( 1 );
	}
	*destination_segment_table = memory_allocate_structure(
	                              libewf_segment_table_t );

	if( *destination_segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination segment table.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *destination_segment_table,
	     0,
	     sizeof( libewf_segment_table_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear source to destination segment table.",
		 function );

		memory_free(
		 *destination_segment_table );

		*destination_segment_table = NULL;

		return( -1 );
	}
	if( source_segment_table->basename != NULL )
	{
		( *destination_segment_table )->basename = system_string_allocate(
					                    source_segment_table->basename_size );

		if( *destination_segment_table == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create destination segment table.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     ( *destination_segment_table )->basename,
		     source_segment_table->basename,
		     sizeof( system_character_t ) * source_segment_table->basename_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy source to destination basename.",
			 function );

			goto on_error;
		}
		( *destination_segment_table )->basename_size = source_segment_table->basename_size;
	}
	if( libfdata_list_clone(
	     &( ( *destination_segment_table )->segment_files_list ),
	     source_segment_table->segment_files_list,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination segment files list.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_clone(
	     &( ( *destination_segment_table )->segment_files_cache ),
	     source_segment_table->segment_files_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination segment files cache.",
		 function );

		goto on_error;
	}
	( *destination_segment_table )->maximum_segment_size = source_segment_table->maximum_segment_size;

	return( 1 );

on_error:
	if( *destination_segment_table != NULL )
	{
		if( ( *destination_segment_table )->segment_files_list != NULL )
		{
			libfdata_list_free(
			 &( ( *destination_segment_table )->segment_files_list ),
			 NULL );
		}
		if( ( *destination_segment_table )->basename != NULL )
		{
			memory_free(
			 ( *destination_segment_table )->basename );
		}
		memory_free(
		 *destination_segment_table );

		*destination_segment_table = NULL;
	}
	return( -1 );
}

/* Retrieves the size of the basename
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_segment_table_get_basename_size(
     libewf_segment_table_t *segment_table,
     size_t *basename_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_segment_table_get_basename_size";

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	int result            = 0;
#endif

	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( basename_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid basename size.",
		 function );

		return( -1 );
	}
	if( segment_table->basename == NULL )
	{
		return( 0 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_size_from_utf32(
		          (libuna_utf32_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          basename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_size_from_utf16(
		          (libuna_utf16_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          basename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_size_from_utf32(
		          (libuna_utf32_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          libclocale_codepage,
		          basename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_size_from_utf16(
		          (libuna_utf16_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          libclocale_codepage,
		          basename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine basename size.",
		 function );

		return( -1 );
	}
#else
	*basename_size = segment_table->basename_size;
#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

	return( 1 );
}

/* Retrieves the basename
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_segment_table_get_basename(
     libewf_segment_table_t *segment_table,
     char *basename,
     size_t basename_size,
     libcerror_error_t **error )
{
	static char *function       = "libewf_segment_table_get_basename";
	size_t narrow_basename_size = 0;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	int result                  = 0;
#endif

	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( basename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid basename.",
		 function );

		return( -1 );
	}
	if( segment_table->basename == NULL )
	{
		return( 0 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_size_from_utf32(
		          (libuna_utf32_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          &narrow_basename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_size_from_utf16(
		          (libuna_utf16_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          &narrow_basename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_size_from_utf32(
		          (libuna_utf32_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          libclocale_codepage,
		          &narrow_basename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_size_from_utf16(
		          (libuna_utf16_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          libclocale_codepage,
		          &narrow_basename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine narrow basename size.",
		 function );

		return( -1 );
	}
#else
	narrow_basename_size = segment_table->basename_size;
#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

	if( basename_size < narrow_basename_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: basename too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_copy_from_utf32(
		          (libuna_utf8_character_t *) basename,
		          basename_size,
		          (libuna_utf32_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_copy_from_utf16(
		          (libuna_utf8_character_t *) basename,
		          basename_size,
		          (libuna_utf16_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_copy_from_utf32(
		          (uint8_t *) basename,
		          basename_size,
		          libclocale_codepage,
		          (libuna_utf32_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_copy_from_utf16(
		          (uint8_t *) basename,
		          basename_size,
		          libclocale_codepage,
		          (libuna_utf16_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set basename.",
		 function );

		return( -1 );
	}
#else
	if( system_string_copy(
	     basename,
	     segment_table->basename,
	     segment_table->basename_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set basename.",
		 function );

		return( -1 );
	}
	basename[ segment_table->basename_size - 1 ] = 0;
#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

	return( 1 );
}

/* Sets the basename
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_table_set_basename(
     libewf_segment_table_t *segment_table,
     const char *basename,
     size_t basename_length,
     libcerror_error_t **error )
{
	static char *function = "libewf_segment_table_set_basename";

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	int result            = 0;
#endif

	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( basename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid basename.",
		 function );

		return( -1 );
	}
	if( segment_table->basename != NULL )
	{
		memory_free(
		 segment_table->basename );

		segment_table->basename      = NULL;
		segment_table->basename_size = 0;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_utf8(
		          (libuna_utf8_character_t *) basename,
		          basename_length + 1,
		          &( segment_table->basename_size ),
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_utf8(
		          (libuna_utf8_character_t *) basename,
		          basename_length + 1,
		          &( segment_table->basename_size ),
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_byte_stream(
		          (uint8_t *) basename,
		          basename_length + 1,
		          libclocale_codepage,
		          &( segment_table->basename_size ),
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_byte_stream(
		          (uint8_t *) basename,
		          basename_length + 1,
		          libclocale_codepage,
		          &( segment_table->basename_size ),
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine basename size.",
		 function );

		return( -1 );
	}
#else
	segment_table->basename_size = basename_length + 1;
#endif
	segment_table->basename = system_string_allocate(
	                           segment_table->basename_size );

	if( segment_table->basename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create basename.",
		 function );

		segment_table->basename_size = 0;

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_copy_from_utf8(
		          (libuna_utf32_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          (libuna_utf8_character_t *) basename,
		          basename_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_copy_from_utf8(
		          (libuna_utf16_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          (libuna_utf8_character_t *) basename,
		          basename_length + 1,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_copy_from_byte_stream(
		          (libuna_utf32_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          (uint8_t *) basename,
		          basename_length + 1,
		          libclocale_codepage,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_copy_from_byte_stream(
		          (libuna_utf16_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          (uint8_t *) basename,
		          basename_length + 1,
		          libclocale_codepage,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set basename.",
		 function );

		memory_free(
		 segment_table->basename );

		segment_table->basename      = NULL;
		segment_table->basename_size = 0;

		return( -1 );
	}
#else
	if( system_string_copy(
	     segment_table->basename,
	     basename,
	     basename_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set basename.",
		 function );

		memory_free(
		 segment_table->basename );

		segment_table->basename      = NULL;
		segment_table->basename_size = 0;

		return( -1 );
	}
	segment_table->basename[ basename_length ] = 0;
#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Retrieves the size of the basename
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_segment_table_get_basename_size_wide(
     libewf_segment_table_t *segment_table,
     size_t *basename_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_segment_table_get_basename_size_wide";

#if !defined( HAVE_WIDE_SYSTEM_CHARACTER )
	int result            = 0;
#endif

	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( basename_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid basename size.",
		 function );

		return( -1 );
	}
	if( segment_table->basename == NULL )
	{
		return( 0 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	*basename_size = segment_table->basename_size;
#else
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_utf8(
		          (libuna_utf8_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          basename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_utf8(
		          (libuna_utf8_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          basename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_byte_stream(
		          (uint8_t *) segment_table->basename,
		          segment_table->basename_size,
		          libclocale_codepage,
		          basename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_byte_stream(
		          (uint8_t *) segment_table->basename,
		          segment_table->basename_size,
		          libclocale_codepage,
		          basename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine basename size.",
		 function );

		return( -1 );
	}
#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */
	return( 1 );
}

/* Retrieves the basename
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_segment_table_get_basename_wide(
     libewf_segment_table_t *segment_table,
     wchar_t *basename,
     size_t basename_size,
     libcerror_error_t **error )
{
	static char *function     = "libewf_segment_table_get_basename_wide";
	size_t wide_basename_size = 0;

#if !defined( HAVE_WIDE_SYSTEM_CHARACTER )
	int result                = 0;
#endif

	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( basename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid basename.",
		 function );

		return( -1 );
	}
	if( segment_table->basename == NULL )
	{
		return( 0 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	wide_basename_size = segment_table->basename_size;
#else
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_utf8(
		          (libuna_utf8_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          &wide_basename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_utf8(
		          (libuna_utf8_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          &wide_basename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_byte_stream(
		          (uint8_t *) segment_table->basename,
		          segment_table->basename_size,
		          libclocale_codepage,
		          &wide_basename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_byte_stream(
		          (uint8_t *) segment_table->basename,
		          segment_table->basename_size,
		          libclocale_codepage,
		          &wide_basename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine basename size.",
		 function );

		return( -1 );
	}
#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */
	if( basename_size < wide_basename_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: basename too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( system_string_copy(
	     basename,
	     segment_table->basename,
	     segment_table->basename_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set basename.",
		 function );

		return( -1 );
	}
	basename[ segment_table->basename_size - 1 ] = 0;
#else
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_copy_from_utf8(
		          (libuna_utf32_character_t *) basename,
		          basename_size,
		          (libuna_utf8_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_copy_from_utf8(
		          (libuna_utf16_character_t *) basename,
		          basename_size,
		          (libuna_utf8_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_copy_from_byte_stream(
		          (libuna_utf32_character_t *) basename,
		          basename_size,
		          (uint8_t *) segment_table->basename,
		          segment_table->basename_size,
		          libclocale_codepage,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_copy_from_byte_stream(
		          (libuna_utf16_character_t *) basename,
		          basename_size,
		          (uint8_t *) segment_table->basename,
		          segment_table->basename_size,
		          libclocale_codepage,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set basename.",
		 function );

		return( -1 );
	}
#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */
	return( 1 );
}

/* Sets the basename
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_table_set_basename_wide(
     libewf_segment_table_t *segment_table,
     const wchar_t *basename,
     size_t basename_length,
     libcerror_error_t **error )
{
	static char *function = "libewf_segment_table_set_basename_wide";

#if !defined( HAVE_WIDE_SYSTEM_CHARACTER )
	int result            = 0;
#endif

	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( basename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid basename.",
		 function );

		return( -1 );
	}
	if( segment_table->basename != NULL )
	{
		memory_free(
		 segment_table->basename );

		segment_table->basename      = NULL;
		segment_table->basename_size = 0;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	segment_table->basename_size = basename_length + 1;
#else
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_size_from_utf32(
		          (libuna_utf32_character_t *) basename,
		          basename_length + 1,
		          &( segment_table->basename_size ),
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_size_from_utf16(
		          (libuna_utf16_character_t *) basename,
		          basename_length + 1,
		          &( segment_table->basename_size ),
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_size_from_utf32(
		          (libuna_utf32_character_t *) basename,
		          basename_length + 1,
		          libclocale_codepage,
		          &( segment_table->basename_size ),
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_size_from_utf16(
		          (libuna_utf16_character_t *) basename,
		          basename_length + 1,
		          libclocale_codepage,
		          &( segment_table->basename_size ),
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine basename size.",
		 function );

		return( -1 );
	}
#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */
	segment_table->basename = system_string_allocate(
	                           segment_table->basename_size );

	if( segment_table->basename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create basename.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( system_string_copy(
	     segment_table->basename,
	     basename,
	     basename_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set basename.",
		 function );

		memory_free(
		 segment_table->basename );

		segment_table->basename      = NULL;
		segment_table->basename_size = 0;

		return( -1 );
	}
	segment_table->basename[ basename_length ] = 0;
#else
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_copy_from_utf32(
		          (libuna_utf8_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          (libuna_utf32_character_t *) basename,
		          basename_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_copy_from_utf16(
		          (libuna_utf8_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          (libuna_utf16_character_t *) basename,
		          basename_length + 1,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_copy_from_utf32(
		          (uint8_t *) segment_table->basename,
		          segment_table->basename_size,
		          libclocale_codepage,
		          (libuna_utf32_character_t *) basename,
		          basename_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_copy_from_utf16(
		          (uint8_t *) segment_table->basename,
		          segment_table->basename_size,
		          libclocale_codepage,
		          (libuna_utf16_character_t *) basename,
		          basename_length + 1,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set basename.",
		 function );

		memory_free(
		 segment_table->basename );

		segment_table->basename      = NULL;
		segment_table->basename_size = 0;

		return( -1 );
	}
#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */
	return( 1 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Sets the maximum segment size
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_table_set_maximum_segment_size(
     libewf_segment_table_t *segment_table,
     size64_t maximum_segment_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_segment_table_set_maximum_segment_size";

	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	segment_table->maximum_segment_size = maximum_segment_size;

	return( 1 );
}

/* Retrieves the number of segments
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_table_get_number_of_segments(
     libewf_segment_table_t *segment_table,
     uint32_t *number_of_segments,
     libcerror_error_t **error )
{
	static char *function = "libewf_segment_table_get_number_of_segments";

	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( number_of_segments == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of segments.",
		 function );

		return( -1 );
	}
	*number_of_segments = segment_table->number_of_segments;

	return( 1 );
}

/* Retrieves a specific segment from the segment table
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_table_get_segment_by_index(
     libewf_segment_table_t *segment_table,
     uint32_t segment_number,
     int *file_io_pool_entry,
     size64_t *segment_file_size,
     libcerror_error_t **error )
{
	static char *function  = "libewf_segment_table_get_segment_by_index";
	off64_t element_offset = 0;
	uint32_t element_flags = 0;

	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
#if SIZEOF_INT <= 4
	if( segment_number > (uint32_t) INT_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid segment number value out of bounds.",
		 function );

		return( -1 );
	}
#endif
	if( libfdata_list_get_element_by_index(
	     segment_table->segment_files_list,
	     (int) segment_number,
	     file_io_pool_entry,
	     &element_offset,
	     segment_file_size,
	     &element_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve element: %" PRIu32 " from segment files list.",
		 function,
		 segment_number );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a segment at a specific offset from the segment table
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libewf_segment_table_get_segment_at_offset(
     libewf_segment_table_t *segment_table,
     off64_t offset,
     int *file_io_pool_entry,
     size64_t *segment_file_size,
     libcerror_error_t **error )
{
	static char *function            = "libewf_segment_table_get_segment_at_offset";
	off64_t element_offset           = 0;
	off64_t segment_file_data_offset = 0;
	uint32_t element_flags           = 0;
	int result                       = 0;
	int segment_number               = 0;

	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	result = libfdata_list_get_element_at_offset(
	          segment_table->segment_files_list,
	          offset,
	          &segment_number,
	          &segment_file_data_offset,
	          file_io_pool_entry,
	          &element_offset,
	          segment_file_size,
	          &element_flags,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve element at offset: %" PRIi64 " (0x%08" PRIx64 ") from segment files list.",
		 function,
		 offset,
		 offset );

		return( -1 );
	}
	return( result );
}

/* Retrieves the segment storage media size for a specific segment in the segment table
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_segment_table_get_segment_storage_media_size_by_index(
     libewf_segment_table_t *segment_table,
     uint32_t segment_number,
     size64_t *storage_media_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_segment_table_get_segment_storage_media_size_by_index";
	int result            = 0;

	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
#if SIZEOF_INT <= 4
	if( segment_number > (uint32_t) INT_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid segment number value out of bounds.",
		 function );

		return( -1 );
	}
#endif
	result = libfdata_list_get_mapped_size_by_index(
	          segment_table->segment_files_list,
	          (int) segment_number,
	          storage_media_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to get mapped size of element: %" PRIu32 " in segment files list.",
		 function,
		 segment_number );

		return( -1 );
	}
	return( result );
}

/* Sets the segment storage media size for a specific segment in the segment table
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_table_set_segment_storage_media_size_by_index(
     libewf_segment_table_t *segment_table,
     uint32_t segment_number,
     size64_t storage_media_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_segment_table_set_segment_storage_media_size_by_index";

	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
#if SIZEOF_INT <= 4
	if( segment_number > (uint32_t) INT_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid segment number value out of bounds.",
		 function );

		return( -1 );
	}
#endif
	if( libfdata_list_set_mapped_size_by_index(
	     segment_table->segment_files_list,
	     (int) segment_number,
	     storage_media_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set mapped size of element: %" PRIu32 " in segment files list.",
		 function,
		 segment_number );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific segment file from the segment table
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_table_get_segment_file_by_index(
     libewf_segment_table_t *segment_table,
     uint32_t segment_number,
     libbfio_pool_t *file_io_pool,
     libewf_segment_file_t **segment_file,
     libcerror_error_t **error )
{
	static char *function = "libewf_segment_table_get_segment_file_by_index";

	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
#if SIZEOF_INT <= 4
	if( segment_number > (uint32_t) INT_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid segment number value out of bounds.",
		 function );

		return( -1 );
	}
#endif
	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( libfdata_list_get_element_value_by_index(
	     segment_table->segment_files_list,
	     (intptr_t *) file_io_pool,
	     (libfdata_cache_t *) segment_table->segment_files_cache,
	     (int) segment_number,
	     (intptr_t **) &( segment_table->current_segment_file ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve element value: %" PRIu32 " from segment files list.",
		 function,
		 segment_number );

		return( -1 );
	}
	if( segment_table->current_segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing segment file.",
		 function );

		return( -1 );
	}
	if( ( (int64_t) segment_number + 1 ) != (int64_t) segment_table->current_segment_file->segment_number )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid segment number value out of bounds.",
		 function );

		return( -1 );
	}
	if( libfdata_list_get_element_mapped_range(
	     segment_table->segment_files_list,
	     segment_number,
	     &( segment_table->current_segment_file->range_start_offset ),
	     (size64_t *) &( segment_table->current_segment_file->range_end_offset ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment files list element: %" PRIu32 " mapped range.",
		 function,
		 segment_number );

		return( -1 );
	}
	segment_table->current_segment_file->range_end_offset += segment_table->current_segment_file->range_start_offset;

	*segment_file = segment_table->current_segment_file;

	return( 1 );
}

/* Retrieves a segment file at a specific offset from the segment table
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libewf_segment_table_get_segment_file_at_offset(
     libewf_segment_table_t *segment_table,
     off64_t offset,
     libbfio_pool_t *file_io_pool,
     uint32_t *segment_number,
     off64_t *segment_file_data_offset,
     libewf_segment_file_t **segment_file,
     libcerror_error_t **error )
{
	static char *function                 = "libewf_segment_table_get_segment_file_at_offset";
	off64_t safe_segment_file_data_offset = 0;
	int result                            = 0;
	int segment_files_list_index          = 0;

	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( segment_number == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment number.",
		 function );

		return( -1 );
	}
	if( segment_file_data_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file data offset.",
		 function );

		return( -1 );
	}
	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( ( segment_table->current_segment_file != NULL )
	 && ( segment_table->current_segment_file->range_end_offset > 0 ) )
	{
		if( ( offset >= segment_table->current_segment_file->range_start_offset )
		 && ( offset < segment_table->current_segment_file->range_end_offset ) )
		{
			safe_segment_file_data_offset = offset - segment_table->current_segment_file->range_start_offset;

			result = 1;
		}
	}
	if( result == 0 )
	{
		result = libfdata_list_get_element_value_at_offset(
		          segment_table->segment_files_list,
		          (intptr_t *) file_io_pool,
		          (libfdata_cache_t *) segment_table->segment_files_cache,
		          offset,
		          &segment_files_list_index,
		          &safe_segment_file_data_offset,
		          (intptr_t **) &( segment_table->current_segment_file ),
		          0,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve segment file for offset: %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 offset,
			 offset );

			return( -1 );
		}
		else if( result != 0 )
		{
			if( segment_table->current_segment_file == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing segment file.",
				 function );

				return( -1 );
			}
			if( ( (int64_t) segment_files_list_index + 1 ) != (int64_t) segment_table->current_segment_file->segment_number )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid segment number value out of bounds.",
				 function );

				return( -1 );
			}
			if( libfdata_list_get_element_mapped_range(
			     segment_table->segment_files_list,
			     segment_files_list_index,
			     &( segment_table->current_segment_file->range_start_offset ),
			     (size64_t *) &( segment_table->current_segment_file->range_end_offset ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve segment files list element: %d mapped range.",
				 function,
				 segment_files_list_index );

				return( -1 );
			}
			segment_table->current_segment_file->range_end_offset += segment_table->current_segment_file->range_start_offset;
		}
	}
	if( result != 0 )
	{
		*segment_number           = segment_table->current_segment_file->segment_number - 1;
		*segment_file_data_offset = safe_segment_file_data_offset;
		*segment_file             = segment_table->current_segment_file;
	}
	return( result );
}

/* Appends a segment to the segment table based on the segment file
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_table_append_segment_by_segment_file(
     libewf_segment_table_t *segment_table,
     libewf_segment_file_t *segment_file,
     int file_io_pool_entry,
     size64_t segment_file_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_segment_table_append_segment_by_segment_file";

	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
#if SIZEOF_INT <= 4
	if( segment_file->segment_number > (uint32_t) INT_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid segment file - segment number value out of bounds.",
		 function );

		return( -1 );
	}
#endif
	if( segment_file->segment_number > segment_table->number_of_segments )
	{
		if( libfdata_list_resize(
		     segment_table->segment_files_list,
		     (int) segment_file->segment_number,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to resize segment files list.",
			 function );

			return( -1 );
		}
		segment_table->number_of_segments = segment_file->segment_number;
	}
	if( libfdata_list_set_element_by_index(
	     segment_table->segment_files_list,
	     (int) ( segment_file->segment_number - 1 ),
	     file_io_pool_entry,
	     0,
	     segment_file_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set element: %" PRIu32 " in segment files list.",
		 function,
		 segment_file->segment_number );

		return( -1 );
	}
	return( 1 );
}

