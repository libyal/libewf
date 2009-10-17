/* 
 * Info handle
 *
 * Copyright (C) 2008-2009, Joachim Metz <forensics@hoffmannbv.nl>,
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

#if !defined( _INFO_HANDLE_H )
#define _INFO_HANDLE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

/* If libtool DLL support is enabled set LIBEWF_DLL_IMPORT
 * before including libewf.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBEWF_DLL_IMPORT
#endif

#include <libewf.h>

#include <libsystem.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct info_handle info_handle_t;

struct info_handle
{
	/* The libewf input handle
	 */
	libewf_handle_t *input_handle;
};

int info_handle_initialize(
     info_handle_t **info_handle,
     liberror_error_t **error );

int info_handle_free(
     info_handle_t **info_handle,
     liberror_error_t **error );

int info_handle_signal_abort(
     info_handle_t *info_handle,
     liberror_error_t **error );

int info_handle_open_input(
     info_handle_t *info_handle,
     libsystem_character_t * const * filenames,
     int amount_of_filenames,
     liberror_error_t **error );

int info_handle_close(
     info_handle_t *info_handle,
     liberror_error_t **error );

int info_handle_get_header_value(
     info_handle_t *info_handle,
     char *header_value_identifier,
     size_t header_value_identifier_length,
     libsystem_character_t *header_value,
     size_t header_value_size,
     liberror_error_t **error );

int info_handle_get_hash_value(
     info_handle_t *info_handle,
     char *hash_value_identifier,
     size_t hash_value_identifier_length,
     libsystem_character_t *hash_value,
     size_t hash_value_size,
     liberror_error_t **error );

int info_handle_set_header_codepage(
     info_handle_t *info_handle,
     int header_codepage,
     liberror_error_t **error );

int info_handle_header_values_fprint(
     info_handle_t *info_handle,
     uint8_t date_format,
     FILE *stream,
     liberror_error_t **error );

int info_handle_header_value_extents_fprint(
     libsystem_character_t *header_value,
     size_t header_value_length,
     FILE *stream,
     liberror_error_t **error );

int info_handle_media_information_fprint(
     info_handle_t *info_handle,
     FILE *stream,
     liberror_error_t **error );

int info_handle_hash_values_fprint(
     info_handle_t *info_handle,
     FILE *stream,
     liberror_error_t **error );

int info_handle_acquiry_errors_fprint(
     info_handle_t *info_handle,
     FILE *stream,
     liberror_error_t **error );

int info_handle_sessions_fprint(
     info_handle_t *info_handle,
     FILE *stream,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

