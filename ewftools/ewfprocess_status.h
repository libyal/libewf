/*
 * Process status functions for the ewftools
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

#if !defined( _EWFPROCESS_STATUS_H )
#define _EWFPROCESS_STATUS_H

#include <common.h>
#include <character_string.h>
#include <date_time.h>
#include <types.h>

#include <stdio.h>

#if defined( __cplusplus )
extern "C" {
#endif

#define EWFPROCESS_STATUS_ABORTED	(int) 'a'
#define EWFPROCESS_STATUS_COMPLETED	(int) 'c'
#define EWFPROCESS_STATUS_FAILED	(int) 'f'

#if defined( HAVE_WIDE_SYSTEM_CHARACTER_T )
#define ewfprocess_status_ctime( timestamp, string, length ) \
        date_time_wctime( timestamp, string, length )

#else
#define ewfprocess_status_ctime( timestamp, string, length ) \
        date_time_ctime( timestamp, string, length )

#endif

typedef struct ewfprocess_status ewfprocess_status_t;

struct ewfprocess_status
{
	/* The status process string
	 */
	const character_t *status_process_string;

	/* The status update string
	 */
	const character_t *status_update_string;

	/* The status summary string
	 */
	const character_t *status_summary_string;

	/* The ouput stream
	 */
	FILE *output_stream;

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

extern ewfprocess_status_t *process_status;

int ewfprocess_status_initialize(
     ewfprocess_status_t **process_status,
     const character_t *status_process_string,
     const character_t *status_update_string,
     const character_t *status_summary_string,
     FILE *output_stream );

int ewfprocess_status_free(
     ewfprocess_status_t **process_status );

int ewfprocess_status_start(
     ewfprocess_status_t *process_status );

int ewfprocess_status_update(
     ewfprocess_status_t *process_status,
     size64_t bytes_read,
     size64_t bytes_total );

int ewfprocess_status_update_unknown_total(
     ewfprocess_status_t *process_status,
     size64_t bytes_read,
     size64_t bytes_total );

int ewfprocess_status_stop(
     ewfprocess_status_t *process_status,
     size64_t bytes_total,
     int status );

#if defined( __cplusplus )
}
#endif

#endif

