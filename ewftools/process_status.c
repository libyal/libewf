/*
 * Process status functions
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
#include <system_string.h>
#include <types.h>

#if defined( TIME_WITH_SYS_TIME )
#include <sys/time.h>
#include <time.h>
#elif defined( HAVE_SYS_TIME_H )
#include <sys/time.h>
#else
#include <time.h>
#endif

#include "byte_size_string.h"
#include "ewftools_libcdatetime.h"
#include "ewftools_libcerror.h"
#include "ewftools_libclocale.h"
#include "ewftools_libcnotify.h"
#include "process_status.h"

/* Creates process status information
 * Make sure the value process_status is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int process_status_initialize(
     process_status_t **process_status,
     const system_character_t *status_process_string,
     const system_character_t *status_update_string,
     const system_character_t *status_summary_string,
     FILE *output_stream,
     uint8_t print_status_information,
     libcerror_error_t **error )
{
	static char *function = "process_status_initialize";

	if( process_status == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid process status.",
		 function );

		return( -1 );
	}
	if( *process_status != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid process status value already set.",
		 function );

		return( -1 );
	}
	*process_status = memory_allocate_structure(
	                   process_status_t );

	if( *process_status == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create process status.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *process_status,
	     0,
	     sizeof( process_status_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear process status.",
		 function );

		memory_free(
		 *process_status );

		*process_status = NULL;

		return( -1 );
	}
	if( libcdatetime_elements_initialize(
	     &( ( *process_status )->start_time_elements ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create start time elements.",
		 function );

		goto on_error;
	}
	if( libcdatetime_elements_initialize(
	     &( ( *process_status )->current_time_elements ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create current time elements.",
		 function );

		goto on_error;
	}
	if( libcdatetime_elements_initialize(
	     &( ( *process_status )->last_time_elements ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create last time elements.",
		 function );

		goto on_error;
	}
	if( libclocale_locale_get_decimal_point(
	     &( ( *process_status )->decimal_point ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve locale decimal point.",
		 function );

		goto on_error;
	}
	( *process_status )->status_process_string    = status_process_string;
	( *process_status )->status_update_string     = status_update_string;
	( *process_status )->status_summary_string    = status_summary_string;
	( *process_status )->output_stream            = output_stream;
	( *process_status )->print_status_information = print_status_information;

	return( 1 );

on_error:
	if( *process_status != NULL )
	{
		memory_free(
		 *process_status );

		*process_status = NULL;
	}
	return( -1 );
}

/* Frees process status information
 * Returns 1 if successful or -1 on error
 */
int process_status_free(
     process_status_t **process_status,
     libcerror_error_t **error )
{
	static char *function = "process_status_free";
	int result            = 1;

	if( process_status == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid process status.",
		 function );

		return( -1 );
	}
	if( *process_status != NULL )
	{
		if( libcdatetime_elements_free(
		     &( ( *process_status )->start_time_elements ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free start time elements.",
			 function );

			result = -1;
		}
		if( libcdatetime_elements_free(
		     &( ( *process_status )->current_time_elements ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free current time elements.",
			 function );

			result = -1;
		}
		if( libcdatetime_elements_free(
		     &( ( *process_status )->last_time_elements ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free last time elements.",
			 function );

			result = -1;
		}
		memory_free(
		 *process_status );

		*process_status = NULL;
	}
	return( result );
}

/* Starts the process status information
 * Returns 1 if successful or -1 on error
 */
int process_status_start(
     process_status_t *process_status,
     libcerror_error_t **error )
{
	system_character_t time_string[ 32 ];

	static char *function = "process_status_start";

	if( process_status == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid process status.",
		 function );

		return( -1 );
	}
	process_status->last_parts_per_million = -1;

	if( libcdatetime_elements_set_current_time_localtime(
	     process_status->start_time_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set start time elements to current time.",
		 function );

		return( -1 );
	}
	if( libcdatetime_elements_copy(
	     process_status->current_time_elements,
	     process_status->start_time_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy start time elements to current.",
		 function );

		return( -1 );
	}
	if( libcdatetime_elements_copy(
	     process_status->last_time_elements,
	     process_status->start_time_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy start time elements to last.",
		 function );

		return( -1 );
	}
	if( ( process_status->output_stream != NULL )
	 && ( process_status->print_status_information != 0 )
	 && ( process_status->status_process_string != NULL ) )
	{
		if( libcdatetime_elements_copy_to_string(
		     process_status->start_time_elements,
		     (uint8_t *) time_string,
		     32,
		     LIBCDATETIME_STRING_FORMAT_TYPE_CTIME | LIBCDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
		     NULL ) == 1 )
		{
			fprintf(
			 process_status->output_stream,
			 "%" PRIs_SYSTEM " started at: %" PRIs_SYSTEM "\n",
			 process_status->status_process_string,
			 time_string );
		}
		else
		{
			fprintf(
			 process_status->output_stream,
			 "%" PRIs_SYSTEM " started.\n",
			 process_status->status_process_string );
		}
		fprintf(
		 process_status->output_stream,
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
     libcerror_error_t **error )
{
	static char *function               = "process_status_update";
	int64_t new_parts_per_million       = 0;
	int64_t number_of_seconds           = 0;
	int64_t remaining_number_of_seconds = 0;
	int64_t total_number_of_seconds     = 0;

	if( process_status == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
			new_parts_per_million = (int64_t) ( ( bytes_read * 1000000 ) / bytes_total );
		}
		if( libcdatetime_elements_set_current_time_localtime(
		     process_status->current_time_elements,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set current time elements to current time.",
			 function );

			return( -1 );
		}
		if( libcdatetime_elements_get_delta_in_seconds(
		     process_status->current_time_elements,
		     process_status->last_time_elements,
		     &number_of_seconds,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine delta between last and current time.",
			 function );

			return( -1 );
		}
		/* Estimate the remaining time
		 */
		if( number_of_seconds > 3 )
		{
			if( libcdatetime_elements_copy(
			     process_status->last_time_elements,
			     process_status->current_time_elements,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy current time elements to last.",
				 function );

				return( -1 );
			}
			if( libcdatetime_elements_get_delta_in_seconds(
			     process_status->last_time_elements,
			     process_status->start_time_elements,
			     &number_of_seconds,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine delta between last and start time.",
				 function );

				return( -1 );
			}
			process_status->last_parts_per_million = new_parts_per_million;

			fprintf(
			 process_status->output_stream,
			 "Status: at %" PRIi64 "%c%" PRIi64 "%%\n",
			 new_parts_per_million / 10000,
			 (char) process_status->decimal_point,
			 ( new_parts_per_million % 10000 ) / 1000 );

			fprintf(
			 process_status->output_stream,
			 "        %" PRIs_SYSTEM "",
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
			 "\n" );

			if( new_parts_per_million > 0 )
			{
				total_number_of_seconds     = ( number_of_seconds * 1000000 ) / new_parts_per_million;
				remaining_number_of_seconds = total_number_of_seconds - number_of_seconds;

				/* Negative time means nearly finished
				 */
				if( remaining_number_of_seconds < 0 )
				{
					remaining_number_of_seconds = 0;
				}
				fprintf(
				 process_status->output_stream,
				 "        completion" );

				process_status_timestamp_fprint(
				 process_status->output_stream,
				 remaining_number_of_seconds );

				process_status_bytes_per_second_fprint(
				 process_status->output_stream,
				 bytes_total,
				 total_number_of_seconds );

				fprintf(
				 process_status->output_stream,
				 "\n" );
			}
			fprintf(
			 process_status->output_stream,
			 "\n" );
		}
	}
	return( 1 );
}

/* Updates the process status information when the total number of bytes is unknown
 * Returns 1 if successful or -1 on error
 */
int process_status_update_unknown_total(
     process_status_t *process_status,
     size64_t bytes_read,
     libcerror_error_t **error )
{
	static char *function     = "process_status_update_unknown_total";
	int64_t number_of_seconds = 0;

	if( process_status == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid process status.",
		 function );

		return( -1 );
	}
	if( ( process_status->output_stream != NULL )
	 && ( process_status->print_status_information != 0 )
	 && ( process_status->status_update_string != NULL ) )
	{
		if( libcdatetime_elements_set_current_time_localtime(
		     process_status->current_time_elements,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set current time elements to current time.",
			 function );

			return( -1 );
		}
		if( libcdatetime_elements_get_delta_in_seconds(
		     process_status->current_time_elements,
		     process_status->last_time_elements,
		     &number_of_seconds,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine delta between last and current time.",
			 function );

			return( -1 );
		}
		if( number_of_seconds > 3 )
		{
			if( libcdatetime_elements_copy(
			     process_status->last_time_elements,
			     process_status->current_time_elements,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy current time elements to last.",
				 function );

				return( -1 );
			}
			if( libcdatetime_elements_get_delta_in_seconds(
			     process_status->last_time_elements,
			     process_status->start_time_elements,
			     &number_of_seconds,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine delta between last and start time.",
				 function );

				return( -1 );
			}
			process_status->last_bytes_total = bytes_read;

			fprintf(
			 process_status->output_stream,
			 "Status: %" PRIs_SYSTEM "",
			 process_status->status_update_string );

			process_status_bytes_fprint(
			 process_status->output_stream,
			 bytes_read );

			fprintf(
			 process_status->output_stream,
			 "\n" );

			fprintf(
			 process_status->output_stream,
			 "       " );

			process_status_timestamp_fprint(
			 process_status->output_stream,
			 number_of_seconds );

			process_status_bytes_per_second_fprint(
			 process_status->output_stream,
			 bytes_read,
			 number_of_seconds );

			fprintf(
			 process_status->output_stream,
			 "\n\n" );
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
     libcerror_error_t **error )
{
	system_character_t time_string[ 32 ];

	const system_character_t *status_string = NULL;
	static char *function                   = "process_status_start";
	int64_t total_number_of_seconds         = 0;

	if( process_status == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid process status.",
		 function );

		return( -1 );
	}
	if( ( status != PROCESS_STATUS_ABORTED )
	 && ( status != PROCESS_STATUS_COMPLETED )
	 && ( status != PROCESS_STATUS_FAILED ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported status.",
		 function );

		return( -1 );
	}
	if( libcdatetime_elements_set_current_time_localtime(
	     process_status->last_time_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set last time elements to current time.",
		 function );

		return( -1 );
	}
	if( ( process_status->output_stream != NULL )
	 && ( process_status->print_status_information != 0 )
	 && ( process_status->status_process_string != NULL ) )
	{
		if( status == PROCESS_STATUS_ABORTED )
		{
			status_string = _SYSTEM_STRING( "aborted" );
		}
		else if( status == PROCESS_STATUS_COMPLETED )
		{
			status_string = _SYSTEM_STRING( "completed" );
		}
		else if( status == PROCESS_STATUS_FAILED )
		{
			status_string = _SYSTEM_STRING( "failed" );
		}
		fprintf(
		 process_status->output_stream,
		 "%" PRIs_SYSTEM " %" PRIs_SYSTEM "",
		 process_status->status_process_string,
		 status_string );

		if( libcdatetime_elements_copy_to_string(
		     process_status->last_time_elements,
		     (uint8_t *) time_string,
		     32,
		     LIBCDATETIME_STRING_FORMAT_TYPE_CTIME | LIBCDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
		     NULL ) == 1 )
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
			 "\n" );
		}
		if( ( status == PROCESS_STATUS_COMPLETED )
	 	 && ( process_status->status_summary_string != NULL )
		 && ( bytes_total > 0 ) )
		{
			fprintf(
			 process_status->output_stream,
			 "\n" );

			if( libcdatetime_elements_get_delta_in_seconds(
			     process_status->last_time_elements,
			     process_status->start_time_elements,
			     &total_number_of_seconds,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine delta between last and start time.",
				 function );

				return( -1 );
			}
			fprintf(
			 process_status->output_stream,
			 "%" PRIs_SYSTEM ":",
			 process_status->status_summary_string );

			process_status_bytes_fprint(
			 process_status->output_stream,
			 bytes_total );

			process_status_timestamp_fprint(
			 process_status->output_stream,
			 total_number_of_seconds );

			process_status_bytes_per_second_fprint(
			 process_status->output_stream,
			 bytes_total,
			 total_number_of_seconds );

			fprintf(
			 process_status->output_stream,
			 "\n" );
		}
	}
	return( 1 );
}

/* Prints a time stamp (with a leading space) to a stream
 */
void process_status_timestamp_fprint(
      FILE *stream,
      int64_t number_of_seconds )
{
	libcdatetime_elements_t *time_elements = NULL;
	libcerror_error_t *error               = NULL;
	uint16_t day_of_year                   = 0;
	uint8_t hours                          = 0;
	uint8_t minutes                        = 0;
	uint8_t seconds                        = 0;

	if( stream == NULL )
	{
		return;
	}
	if( libcdatetime_elements_initialize(
	     &time_elements,
	     &error ) != 1 )
	{
		goto on_error;
	}
	if( libcdatetime_elements_set_from_delta_in_seconds(
	     time_elements,
	     number_of_seconds,
	     &error ) != 1 )
	{
		goto on_error;
	}
	if( libcdatetime_elements_get_day_of_year(
	     time_elements,
	     &day_of_year,
	     &error ) != 1 )
	{
		goto on_error;
	}
	if( libcdatetime_elements_get_time_values(
	     time_elements,
	     &hours,
	     &minutes,
	     &seconds,
	     &error ) != 1 )
	{
		goto on_error;
	}
	fprintf(
	 stream,
	 " in" );

	if( day_of_year > 0 )
	{
		fprintf(
		 stream,
		 " %" PRIu16 " day(s), %" PRIu8 " hour(s), %" PRIu8 " minute(s) and",
		 day_of_year,
		 hours,
		 minutes );
	}
	else if( hours > 0 )
	{
		fprintf(
		 stream,
		 " %" PRIu8 " hour(s), %" PRIu8 " minute(s) and",
		 hours,
		 minutes );
	}
	else if( minutes > 0 )
	{
		fprintf(
		 stream,
		 " %" PRIu8 " minute(s) and",
		 minutes );
	}
	fprintf(
	 stream,
	 " %" PRIu8 " second(s)",
	 seconds );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( time_elements != NULL )
	{
		libcdatetime_elements_free(
		 &time_elements,
		 NULL );
	}
}

/* Prints the number of bytes per second (with a leading space) to a stream
 */
void process_status_bytes_per_second_fprint(
      FILE *stream,
      size64_t bytes,
      time_t seconds )
{
	system_character_t bytes_per_second_string[ 16 ];

	size64_t bytes_per_second = 0;
	int result                = 0;

	if( stream == NULL )
	{
		return;
	}
	if( seconds == 0 )
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
			 " %" PRIs_SYSTEM "/s (%" PRIu64 " bytes/second)",
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

/* Prints the number of bytes (with a leading space) to a stream
 * Creates a human readable version of the number of bytes if possible
 */
void process_status_bytes_fprint(
      FILE *stream,
      size64_t bytes )
{
	system_character_t bytes_string[ 16 ];

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
		 " %" PRIs_SYSTEM " (%" PRIi64 " bytes)",
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

