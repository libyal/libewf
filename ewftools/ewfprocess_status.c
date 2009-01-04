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

#include <common.h>
#include <character_string.h>
#include <date_time.h>
#include <memory.h>
#include <notify.h>
#include <system_string.h>
#include <types.h>

#include "ewfprocess_status.h"
#include "ewfoutput.h"

ewfprocess_status_t *process_status = NULL;

/* Initializes the process status information
 * Returns 1 if successful or -1 on error
 */
int ewfprocess_status_initialize(
     ewfprocess_status_t **process_status,
     const character_t *status_process_string,
     const character_t *status_update_string,
     const character_t *status_summary_string,
     FILE *output_stream )
{
	static char *function = "ewfprocess_status_initialize";

	if( process_status == NULL )
	{
		notify_warning_printf( "%s: invalid process status.\n",
		 function );

		return( -1 );
	}
	if( *process_status == NULL )
	{
		*process_status = (ewfprocess_status_t *) memory_allocate(
		                                           sizeof( ewfprocess_status_t ) );

		if( *process_status == NULL )
		{
			notify_warning_printf( "%s: unable to create process status.\n",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *process_status,
		     0,
		     sizeof( ewfprocess_status_t ) ) == NULL )
		{
			notify_warning_printf( "%s: unable to clear process status.\n",
			 function );

			return( -1 );
		}
		( *process_status )->status_process_string = status_process_string;
		( *process_status )->status_update_string  = status_update_string;
		( *process_status )->status_summary_string = status_summary_string;
		( *process_status )->output_stream         = output_stream;
	}
	return( 1 );
}

/* Frees the process status information
 * Returns 1 if successful or -1 on error
 */
int ewfprocess_status_free(
     ewfprocess_status_t **process_status )
{
	static char *function = "ewfprocess_status_free";

	if( process_status == NULL )
	{
		notify_warning_printf( "%s: invalid process status.\n",
		 function );

		return( -1 );
	}
	if( *process_status != NULL )
	{
		memory_free(
		 *process_status );

		*process_status = NULL;
	}
	return( 1 );
}

/* Starts the process status information
 * Returns 1 if successful or -1 on error
 */
int ewfprocess_status_start(
     ewfprocess_status_t *process_status )
{
	system_character_t time_string[ 32 ];

	static char *function = "ewfprocess_status_start";

	if( process_status == NULL )
	{
		notify_warning_printf( "%s: invalid process status.\n",
		 function );

		return( -1 );
	}
	process_status->last_percentage = -1;
	process_status->start_timestamp = date_time_time(
	                                   NULL );

	if( ( process_status->output_stream != NULL )
	 && ( process_status->status_process_string != NULL ) )
	{
		if( ewfprocess_status_ctime(
		     &( process_status->start_timestamp ),
		     time_string,
		     32 ) == NULL )
		{
			fprintf( stderr, "%" PRIs " started.\n",
			 process_status->status_process_string );
		}
		else
		{
			fprintf( stderr, "%" PRIs " started at: %" PRIs_SYSTEM "\n",
			 process_status->status_process_string, time_string );
		}
		fprintf( stderr, "This could take a while.\n\n" );
	}
	return( 1 );
}

/* Updates the process status information
 * Returns 1 if successful or -1 on error
 */
int ewfprocess_status_update(
     ewfprocess_status_t *process_status,
     size64_t bytes_read,
     size64_t bytes_total )
{
	static char *function    = "ewfprocess_status_update";
	time_t seconds_current   = 0;
	time_t seconds_total     = 0;
	time_t seconds_remaining = 0;
	time_t timestamp_current = 0;
	int8_t new_percentage    = 0;

	if( process_status == NULL )
	{
		notify_warning_printf( "%s: invalid process status.\n",
		 function );

		return( -1 );
	}
	if( ( process_status->output_stream != NULL )
	 && ( process_status->status_update_string != NULL ) )
	{
		if( ( bytes_total > 0 )
		 && ( bytes_read > 0 ) )
		{
			new_percentage = (int8_t) ( ( bytes_read * 100 ) / bytes_total );
		}
		/* Estimate the remaining time
		 */
		timestamp_current = time( NULL );

		if( ( new_percentage > process_status->last_percentage )
		 && ( timestamp_current > process_status->last_timestamp ) )
		{
			process_status->last_percentage = new_percentage;

			fprintf(
			 process_status->output_stream,
			 "Status: at %" PRIu8 "%%.\n",
			 new_percentage );

			fprintf(
			 process_status->output_stream,
			 "        %" PRIs "",
			 process_status->status_update_string );

			ewfoutput_bytes_fprint(
			 process_status->output_stream,
			 bytes_read );

			fprintf(
			 process_status->output_stream,
			 " of total" );

			ewfoutput_bytes_fprint(
			 process_status->output_stream,
			 bytes_total );

			fprintf(
			 process_status->output_stream,
			 ".\n" );

			if( ( timestamp_current > process_status->start_timestamp )
			 && ( new_percentage > 0 ) )
			{
				process_status->last_timestamp = timestamp_current;

				seconds_current   = timestamp_current - process_status->start_timestamp;
				seconds_total     = ( ( seconds_current * 100 ) / new_percentage );
				seconds_remaining = seconds_total - seconds_current;

				/* Negative time means nearly finished
				 */
				if( seconds_remaining < 0 )
				{
					seconds_remaining = 0;
				}
				fprintf(
				 process_status->output_stream,
				 "        completion" );

				ewfoutput_timestamp_fprint(
				 process_status->output_stream,
				 seconds_remaining );

				ewfoutput_bytes_per_second_fprint(
				 process_status->output_stream,
				 bytes_total,
				 seconds_total );

				fprintf(
				 process_status->output_stream,
				 ".\n" );
			}
			fprintf(
			 process_status->output_stream,
			 "\n" );
		}
	}
	return( 1 );
}

/* Updates the process status information when the total amount of bytes is unknown
 * Returns 1 if successful or -1 on error
 */
int ewfprocess_status_update_unknown_total(
     ewfprocess_status_t *process_status,
     size64_t bytes_read,
     size64_t bytes_total )
{
	static char *function    = "ewfprocess_status_update_unknown_total";
	time_t seconds_current   = 0;
	time_t timestamp_current = 0;

	if( process_status == NULL )
	{
		notify_warning_printf( "%s: invalid process status.\n",
		 function );

		return( -1 );
	}
	if( ( process_status->output_stream != NULL )
	 && ( process_status->status_update_string != NULL ) )
	{
		timestamp_current = time( NULL );

		if( timestamp_current > process_status->last_timestamp )
		{
			/* Update state
			 * - if no status was printed before
			 * - or input has grown > 10 MiB
			 * - or the last update was 30 seconds ago
			 */
			if( ( process_status->last_bytes_total == 0 )
			 || ( bytes_read > ( process_status->last_bytes_total + ( 10 * 1024 * 1024 ) ) )
			 || ( ( timestamp_current - process_status->last_timestamp ) > 30 ) )
			{
				process_status->last_timestamp   = timestamp_current;
				process_status->last_bytes_total = bytes_read;

				fprintf(
				 process_status->output_stream,
				 "Status: %" PRIs "",
				 process_status->status_update_string );

				ewfoutput_bytes_fprint(
				 process_status->output_stream,
				 bytes_read );

				fprintf(
				 process_status->output_stream,
				 "\n" );

				seconds_current = timestamp_current - process_status->start_timestamp;

				fprintf(
				 process_status->output_stream,
				 "       " );

				ewfoutput_timestamp_fprint(
				 process_status->output_stream,
				 seconds_current );

				ewfoutput_bytes_per_second_fprint(
				 process_status->output_stream,
				 bytes_read,
				 seconds_current );

				fprintf(
				 process_status->output_stream,
				 ".\n\n" );
			}
		}
	}
	return( 1 );
}

/* Stops the process status information
 * Returns 1 if successful or -1 on error
 */
int ewfprocess_status_stop(
     ewfprocess_status_t *process_status,
     size64_t bytes_total,
     int status )
{
	system_character_t time_string[ 32 ];

	static char *function            = "ewfprocess_status_start";
	const character_t *status_string = NULL;
	time_t seconds_total             = 0;

	if( process_status == NULL )
	{
		notify_warning_printf( "%s: invalid process status.\n",
		 function );

		return( -1 );
	}
	if( ( status != EWFPROCESS_STATUS_ABORTED )
	 && ( status != EWFPROCESS_STATUS_COMPLETED )
	 && ( status != EWFPROCESS_STATUS_FAILED ) )
	{
		notify_warning_printf( "%s: unsupported status.\n",
		 function );

		return( -1 );
	}
	process_status->last_timestamp = date_time_time(
	                                  NULL );

	if( ( process_status->output_stream != NULL )
	 && ( process_status->status_process_string != NULL ) )
	{
		if( status == EWFPROCESS_STATUS_ABORTED )
		{
			status_string = _CHARACTER_T_STRING( "aborted" );
		}
		else if( status == EWFPROCESS_STATUS_COMPLETED )
		{
			status_string = _CHARACTER_T_STRING( "completed" );
		}
		else if( status == EWFPROCESS_STATUS_FAILED )
		{
			status_string = _CHARACTER_T_STRING( "failed" );
		}
		fprintf(
		 process_status->output_stream,
		 "%" PRIs " %" PRIs "",
		 process_status->status_process_string,
		 status_string );

		if( ewfprocess_status_ctime(
		     &( process_status->last_timestamp ),
		     time_string,
		     32 ) != NULL )
		{
			fprintf(
			 process_status->output_stream,
			 " at: %" PRIs_SYSTEM "\n",
			 time_string );
		}
		else
		{
			fprintf(
			 process_status->output_stream,
			 ".\n" );
		}
		if( ( status == EWFPROCESS_STATUS_COMPLETED )
	 	 && ( process_status->status_summary_string != NULL ) )
		{
			seconds_total = process_status->last_timestamp - process_status->start_timestamp;

			fprintf(
			process_status->output_stream,
			"%" PRIs ":",
			process_status->status_summary_string );

			ewfoutput_bytes_fprint(
			process_status->output_stream,
			bytes_total );

			ewfoutput_timestamp_fprint(
			process_status->output_stream,
			seconds_total );

			ewfoutput_bytes_per_second_fprint(
			process_status->output_stream,
			bytes_total,
			seconds_total );

			fprintf(
			process_status->output_stream,
			".\n" );
		}
	}
	return( 1 );
}

