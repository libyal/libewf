/*
 * Process status functions
 *
 * Copyright (c) 2006-2012, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _PROCESS_STATUS_H )
#define _PROCESS_STATUS_H

#include <common.h>
#include <types.h>

#include "ewftools_libcerror.h"
#include "ewftools_libcstring.h"
#include "ewftools_libcsystem.h"

#if defined( __cplusplus )
extern "C" {
#endif

enum PROCESS_STATUS
{
	PROCESS_STATUS_ABORTED		= (int) 'a',
	PROCESS_STATUS_COMPLETED	= (int) 'c',
	PROCESS_STATUS_FAILED		= (int) 'f'
};

typedef struct process_status process_status_t;

struct process_status
{
	/* The status process string
	 */
	const libcstring_system_character_t *status_process_string;

	/* The status update string
	 */
	const libcstring_system_character_t *status_update_string;

	/* The status summary string
	 */
	const libcstring_system_character_t *status_summary_string;

	/* The ouput stream
	 */
	FILE *output_stream;

	/* Value to indicate if the status information should be printed to the output stream
	 */
	uint8_t print_status_information;

	/* The start timestamp
	 */
	time_t start_timestamp;

	/* The last timestamp
	 */
	time_t last_timestamp;

	/* The last bytes total
	 */
	size64_t last_bytes_total;

	/* The last percentage
	 */
	int8_t last_percentage;
};

#if defined( HAVE_CTIME ) || defined( HAVE_CTIME_R ) || defined( WINAPI )
int process_status_get_ctime_string(
     const time_t *timestamp,
     libcstring_system_character_t *string,
     size_t string_size,
     libcerror_error_t **error );
#endif

#if defined( HAVE_GMTIME ) || defined( HAVE_GMTIME_R ) || defined( WINAPI )
int process_status_get_time_elements_in_utc(
     const time_t *timestamp,
     struct tm *time_elements,
     libcerror_error_t **error );
#endif

int process_status_initialize(
     process_status_t **process_status,
     const libcstring_system_character_t *status_process_string,
     const libcstring_system_character_t *status_update_string,
     const libcstring_system_character_t *status_summary_string,
     FILE *output_stream,
     uint8_t print_status_information,
     libcerror_error_t **error );

int process_status_free(
     process_status_t **process_status,
     libcerror_error_t **error );

int process_status_start(
     process_status_t *process_status,
     libcerror_error_t **error );

int process_status_update(
     process_status_t *process_status,
     size64_t bytes_read,
     size64_t bytes_total,
     libcerror_error_t **error );

int process_status_update_unknown_total(
     process_status_t *process_status,
     size64_t bytes_read,
     libcerror_error_t **error );

int process_status_stop(
     process_status_t *process_status,
     size64_t bytes_total,
     int status,
     libcerror_error_t **error );

void process_status_timestamp_fprint(
      FILE *stream,
      time_t timestamp );

void process_status_bytes_per_second_fprint(
      FILE *stream,
      size64_t bytes,
      time_t seconds );

void process_status_bytes_fprint(
      FILE *stream,
      size64_t bytes );

#if defined( __cplusplus )
}
#endif

#endif

