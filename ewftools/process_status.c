/*
 * Process status functions
 *
 * Copyright (c) 2006-2009, Joachim Metz <forensics@hoffmannbv.nl>,
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
#include <memory.h>
#include <types.h>

#include <liberror.h>

#include <libsystem.h>

#include "byte_size_string.h"
#include "process_status.h"

/* Initializes the process status information
 * Returns 1 if successful or -1 on error
 */
int process_status_initialize(
     process_status_t **process_status,
     const libsystem_character_t *status_process_string,
     const libsystem_character_t *status_update_string,
     const libsystem_character_t *status_summary_string,
     FILE *output_stream,
     uint8_t print_status_information,
     liberror_error_t **error )
{
	static char *function = "process_status_initialize";

	if( process_status == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid process status.",
		 function );

		return( -1 );
	}
	if( *process_status == NULL )
	{
		*process_status = (process_status_t *) memory_allocate(
		                                        sizeof( process_status_t ) );

		if( *process_status == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create process status.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *process_status,
		     0,
		     sizeof( process_status_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear process status.",
			 function );

			memory_free(
			 *process_status );

			*process_status = NULL;

			return( -1 );
		}
		( *process_status )->status_process_string    = status_process_string;
		( *process_status )->status_update_string     = status_update_string;
		( *process_status )->status_summary_string    = status_summary_string;
		( *process_status )->output_stream            = output_stream;
		( *process_status )->print_status_information = print_status_information;
	}
	return( 1 );
}

/* Frees the process status information
 * Returns 1 if successful or -1 on error
 */
int process_status_free(
     process_status_t **process_status,
     liberror_error_t **error )
{
	static char *function = "process_status_free";

	if( process_status == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid process status.",
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
int process_status_start(
     process_status_t *process_status,
     liberror_error_t **error )
{
	libsystem_character_t time_string[ 32 ];

	static char *function = "process_status_start";

	if( process_status == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid process status.",
		 function );

		return( -1 );
	}
	process_status->last_percentage = -1;
	process_status->start_timestamp = libsystem_date_time_time(
	                                   NULL );

	if( ( process_status->output_stream != NULL )
	 && ( process_status->print_status_information != 0 )
	 && ( process_status->status_process_string != NULL ) )
	{
		if( libsystem_date_time_get_ctime_string(
		     &( process_status->start_timestamp ),
		     time_string,
		     32,
		     NULL ) == 1 )
		{
			fprintf(
			 stderr,
			 "%" PRIs_LIBSYSTEM " started at: %" PRIs_LIBSYSTEM "\n",
			 process_status->status_process_string,
			 time_string );
		}
		else
		{
			fprintf(
			 stderr,
			 "%" PRIs_LIBSYSTEM " started.\n",
			 process_status->status_process_string );
		}
		fprintf(
		 stderr,
		 "This could take a while.\n\n" );
	}
	return( 1 );
}

/* Updates the process status information
 * Returns 1 if successful or -1 on error
 */
int process_status_update(
     process_status_t *process_status,
     size64_t bytes_read,
     size64_t bytes_total,
     liberror_error_t **error )
{
	static char *function    = "process_status_update";
	time_t seconds_current   = 0;
	time_t seconds_total     = 0;
	time_t seconds_remaining = 0;
	time_t timestamp_current = 0;
	int8_t new_percentage    = 0;

	if( process_status == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid process status.",
		 function );

		return( -1 );
	}
	if( ( process_status->output_stream != NULL )
	 && ( process_status->print_status_information != 0 )
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
			 "        %" PRIs_LIBSYSTEM "",
			 process_status->status_update_string );

			process_status_bytes_fprint(
			 process_status->output_stream,
			 bytes_read );

			fprintf(
			 process_status->output_stream,
			 " of total" );

			process_status_bytes_fprint(
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

				process_status_timestamp_fprint(
				 process_status->output_stream,
				 seconds_remaining );

				process_status_bytes_per_second_fprint(
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
int process_status_update_unknown_total(
     process_status_t *process_status,
     size64_t bytes_read,
     liberror_error_t **error )
{
	static char *function    = "process_status_update_unknown_total";
	time_t seconds_current   = 0;
	time_t timestamp_current = 0;

	if( process_status == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid process status.",
		 function );

		return( -1 );
	}
	if( ( process_status->output_stream != NULL )
	 && ( process_status->print_status_information != 0 )
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
				 "Status: %" PRIs_LIBSYSTEM "",
				 process_status->status_update_string );

				process_status_bytes_fprint(
				 process_status->output_stream,
				 bytes_read );

				fprintf(
				 process_status->output_stream,
				 "\n" );

				seconds_current = timestamp_current - process_status->start_timestamp;

				fprintf(
				 process_status->output_stream,
				 "       " );

				process_status_timestamp_fprint(
				 process_status->output_stream,
				 seconds_current );

				process_status_bytes_per_second_fprint(
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
int process_status_stop(
     process_status_t *process_status,
     size64_t bytes_total,
     int status,
     liberror_error_t **error )
{
	libsystem_character_t time_string[ 32 ];

	static char *function                      = "process_status_start";
	const libsystem_character_t *status_string = NULL;
	time_t seconds_total                       = 0;

	if( process_status == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid process status.",
		 function );

		return( -1 );
	}
	if( ( status != PROCESS_STATUS_ABORTED )
	 && ( status != PROCESS_STATUS_COMPLETED )
	 && ( status != PROCESS_STATUS_FAILED ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported status.",
		 function );

		return( -1 );
	}
	process_status->last_timestamp = libsystem_date_time_time(
	                                  NULL );

	if( ( process_status->output_stream != NULL )
	 && ( process_status->print_status_information != 0 )
	 && ( process_status->status_process_string != NULL ) )
	{
		if( status == PROCESS_STATUS_ABORTED )
		{
			status_string = _LIBSYSTEM_CHARACTER_T_STRING( "aborted" );
		}
		else if( status == PROCESS_STATUS_COMPLETED )
		{
			status_string = _LIBSYSTEM_CHARACTER_T_STRING( "completed" );
		}
		else if( status == PROCESS_STATUS_FAILED )
		{
			status_string = _LIBSYSTEM_CHARACTER_T_STRING( "failed" );
		}
		fprintf(
		 process_status->output_stream,
		 "%" PRIs_LIBSYSTEM " %" PRIs_LIBSYSTEM "",
		 process_status->status_process_string,
		 status_string );

		if( libsystem_date_time_get_ctime_string(
		     &( process_status->last_timestamp ),
		     time_string,
		     32,
		     NULL ) == 1 )
		{
			fprintf(
			 process_status->output_stream,
			 " at: %" PRIs_LIBSYSTEM "\n",
			 time_string );
		}
		else
		{
			fprintf(
			 process_status->output_stream,
			 ".\n" );
		}
		if( ( status == PROCESS_STATUS_COMPLETED )
	 	 && ( process_status->status_summary_string != NULL ) )
		{
			seconds_total = process_status->last_timestamp - process_status->start_timestamp;

			fprintf(
			process_status->output_stream,
			"%" PRIs_LIBSYSTEM ":",
			process_status->status_summary_string );

			process_status_bytes_fprint(
			process_status->output_stream,
			bytes_total );

			process_status_timestamp_fprint(
			process_status->output_stream,
			seconds_total );

			process_status_bytes_per_second_fprint(
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

/* Prints a time stamp (with a leading space) to a stream
 */
void process_status_timestamp_fprint(
      FILE *stream,
      time_t timestamp )
{
	struct tm time_elements;

	if( stream == NULL )
	{
		return;
	}
	if( libsystem_date_time_get_time_elements_in_utc(
	     &timestamp,
	     &time_elements,
	     NULL ) == 1 )
	{
		fprintf(
		 stream,
		 " in" );

		if( time_elements.tm_isdst != 0 )
		{
			time_elements.tm_hour -= 1;
			time_elements.tm_isdst = 0;	
		}
		if( time_elements.tm_yday > 0 )
		{
			fprintf(
			 stream,
			 " %i day(s), %i hour(s), %i minute(s) and",
			 time_elements.tm_yday,
			 time_elements.tm_hour,
			 time_elements.tm_min );
		}
		else if( time_elements.tm_hour > 0 )
		{
			fprintf(
			 stream,
			 " %i hour(s), %i minute(s) and",
			 time_elements.tm_hour,
			 time_elements.tm_min );
		}
		else if( time_elements.tm_min > 0 )
		{
			fprintf(
			 stream,
			 " %i minute(s) and",
			 time_elements.tm_min );
		}
		fprintf(
		 stream,
		 " %i second(s)",
		 time_elements.tm_sec );
	}
}

/* Prints the amount of bytes per second (with a leading space) to a stream
 */
void process_status_bytes_per_second_fprint(
      FILE *stream,
      size64_t bytes,
      time_t seconds )
{
	libsystem_character_t bytes_per_second_string[ 16 ];

	size64_t bytes_per_second = 0;
	int result                = 0;

	if( stream == NULL )
	{
		return;
	}
	if( seconds > 0 )
	{
		bytes_per_second = bytes / seconds;

		if( bytes_per_second > 1024 )
		{
			result = byte_size_string_create(
			          bytes_per_second_string,
			          10,
			          bytes_per_second,
			          BYTE_SIZE_STRING_UNIT_MEBIBYTE,
			          NULL );
		}
		fprintf(
		 stream,
		 " with" );

		if( result == 1 )
		{
			fprintf(
			 stream,
			 " %" PRIs_LIBSYSTEM "/s (%" PRIu64 " bytes/second)",
			 bytes_per_second_string,
			 bytes_per_second );
		}
		else
		{
			fprintf(
			 stream,
			 " %" PRIu64 " bytes/second",
			 bytes_per_second );
		}
	}
}

/* Prints the amount of bytes (with a leading space) to a stream
 * Creates a human readable version of the amount of bytes if possible
 */
void process_status_bytes_fprint(
      FILE *stream,
      size64_t bytes )
{
	libsystem_character_t bytes_string[ 16 ];

	int result = 0;

	if( stream == NULL )
	{
		return;
	}
	if( bytes > 1024 )
	{
		result = byte_size_string_create(
		          bytes_string,
		          10,
		          bytes,
		          BYTE_SIZE_STRING_UNIT_MEBIBYTE,
		          NULL );
	}
	if( result == 1 )
	{
		fprintf(
		 stream,
		 " %" PRIs_LIBSYSTEM " (%" PRIi64 " bytes)",
		 bytes_string,
		 bytes );
	}
	else
	{
		fprintf(
		 stream,
		 " %" PRIi64 " bytes",
		 bytes );
	}
}

