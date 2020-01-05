/*
 * Mounts an Expert Witness Compression Format (EWF) image file
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
#include <file_stream.h>
#include <memory.h>
#include <system_string.h>
#include <types.h>

#include <stdio.h>

#if defined( HAVE_IO_H ) || defined( WINAPI )
#include <io.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( HAVE_SYS_RESOURCE_H )
#include <sys/resource.h>
#endif

#include "ewftools_getopt.h"
#include "ewftools_glob.h"
#include "ewftools_i18n.h"
#include "ewftools_libcerror.h"
#include "ewftools_libclocale.h"
#include "ewftools_libcnotify.h"
#include "ewftools_libewf.h"
#include "ewftools_output.h"
#include "ewftools_signal.h"
#include "ewftools_unused.h"
#include "mount_dokan.h"
#include "mount_fuse.h"
#include "mount_handle.h"

mount_handle_t *ewfmount_mount_handle = NULL;
int ewfmount_abort                    = 0;

/* Prints usage information
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use ewfmount to mount an Expert Witness Compression Format (EWF) image file\n\n" );

	fprintf( stream, "Usage: ewfmount [ -f format ] [ -X extended_options ] [ -hvV ] image mount_point\n\n" );

	fprintf( stream, "\timage:       an Expert Witness Compression Format (EWF) image file\n\n" );
	fprintf( stream, "\tmount_point: the directory to serve as mount point\n\n" );

	fprintf( stream, "\t-f:          specify the input format, options: raw (default), files (restricted to\n"
	                 "\t             logical volume files)\n" );
	fprintf( stream, "\t-h:          shows this help\n" );
	fprintf( stream, "\t-v:          verbose output to stderr, while ewfmount will remain running in the\n"
	                 "\t             foreground\n" );
	fprintf( stream, "\t-V:          print version\n" );
	fprintf( stream, "\t-X:          extended options to pass to sub system\n" );
}

/* Signal handler for ewfmount
 */
void ewfmount_signal_handler(
      ewftools_signal_t signal EWFTOOLS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "ewfmount_signal_handler";

	EWFTOOLS_UNREFERENCED_PARAMETER( signal )

	ewfmount_abort = 1;

	if( ewfmount_mount_handle != NULL )
	{
		if( mount_handle_signal_abort(
		     ewfmount_mount_handle,
		     &error ) != 1 )
		{
			libcnotify_printf(
			 "%s: unable to signal mount handle to abort.\n",
			 function );

			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );
		}
	}
	/* Force stdin to close otherwise any function reading it will remain blocked
	 */
#if defined( WINAPI ) && !defined( __CYGWIN__ )
	if( _close(
	     0 ) != 0 )
#else
	if( close(
	     0 ) != 0 )
#endif
	{
		libcnotify_printf(
		 "%s: unable to close stdin.\n",
		 function );
	}
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
#if defined( HAVE_GETRLIMIT )
	struct rlimit limit_data;
#endif

	system_character_t * const *sources         = NULL;
	libewf_error_t *error                       = NULL;
	system_character_t *mount_point             = NULL;
	system_character_t *option_extended_options = NULL;
	system_character_t *option_format           = NULL;
	const system_character_t *path_prefix       = NULL;
	char *program                               = _SYSTEM_STRING( "ewfmount" );
	system_integer_t option                     = 0;
	size_t path_prefix_size                     = 0;
	int number_of_sources                       = 0;
	int result                                  = 0;
	int verbose                                 = 0;

#if !defined( HAVE_GLOB_H )
	ewftools_glob_t *glob                       = NULL;
#endif

#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE )
	struct fuse_operations ewfmount_fuse_operations;

	struct fuse_args ewfmount_fuse_arguments    = FUSE_ARGS_INIT(0, NULL);
	struct fuse_chan *ewfmount_fuse_channel     = NULL;
	struct fuse *ewfmount_fuse_handle           = NULL;

#elif defined( HAVE_LIBDOKAN )
	DOKAN_OPERATIONS ewfmount_dokan_operations;
	DOKAN_OPTIONS ewfmount_dokan_options;
#endif

	libcnotify_stream_set(
	 stderr,
	 NULL );
	libcnotify_verbose_set(
	 1 );

	if( libclocale_initialize(
	     "ewftools",
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize locale values.\n" );

		goto on_error;
	}
	if( ewftools_output_initialize(
	     _IONBF,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize output settings.\n" );

		goto on_error;
	}
	ewftools_output_version_fprint(
	 stdout,
	 program );

	while( ( option = ewftools_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "f:hvVX:" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM "\n",
				 argv[ optind - 1 ] );

				usage_fprint(
				 stdout );

				return( EXIT_FAILURE );

			case (system_integer_t) 'f':
				option_format = optarg;

				break;

			case (system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'v':
				verbose = 1;

				break;

			case (system_integer_t) 'V':
				ewftools_output_copyright_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'X':
				option_extended_options = optarg;

				break;
		}
	}
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing source image(s).\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	if( ( optind + 1 ) == argc )
	{
		fprintf(
		 stderr,
		 "Missing mount point.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	mount_point = argv[ argc - 1 ];

	libcnotify_verbose_set(
	 verbose );
	libewf_notify_set_stream(
	 stderr,
	 NULL );
	libewf_notify_set_verbose(
	 verbose );

#if !defined( HAVE_GLOB_H )
	if( ewftools_glob_initialize(
	     &glob,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize glob.\n" );

		goto on_error;
	}
	if( ewftools_glob_resolve(
	     glob,
	     &( argv[ optind ] ),
	     argc - optind - 1,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to resolve glob.\n" );

		goto on_error;
	}
	if( ewftools_glob_get_results(
	     glob,
	     &number_of_sources,
	     (system_character_t ***) &sources,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve glob results.\n" );

		goto on_error;
	}
#else
	sources           = &( argv[ optind ] );
	number_of_sources = argc - optind - 1;
#endif

	if( mount_handle_initialize(
	     &ewfmount_mount_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize mount handle.\n" );

		goto on_error;
	}
	if( option_format != NULL )
	{
		result = mount_handle_set_format(
			  ewfmount_mount_handle,
			  option_format,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set format.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			fprintf(
			 stderr,
			 "Unsupported input format defaulting to: raw.\n" );
		}
	}
#if defined( HAVE_GETRLIMIT )
	if( getrlimit(
	     RLIMIT_NOFILE,
	     &limit_data ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to determine limit: number of open file descriptors.\n" );
	}
	if( limit_data.rlim_max > (rlim_t) INT_MAX )
	{
		limit_data.rlim_max = (rlim_t) INT_MAX;
	}
	if( limit_data.rlim_max > 0 )
	{
		limit_data.rlim_max /= 2;
	}
	if( mount_handle_set_maximum_number_of_open_handles(
	     ewfmount_mount_handle,
	     (int) limit_data.rlim_max,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to set maximum number of open file handles.\n" );

		goto on_error;
	}
#endif /* defined( HAVE_GETRLIMIT ) */

#if defined( WINAPI )
	path_prefix = _SYSTEM_STRING( "\\EWF" );
#else
	path_prefix = _SYSTEM_STRING( "/ewf" );
#endif
	path_prefix_size = 1 + system_string_length(
	                        path_prefix );

	if( mount_handle_set_path_prefix(
	     ewfmount_mount_handle,
	     path_prefix,
	     path_prefix_size,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to set path prefix.\n" );

		goto on_error;
	}
	if( mount_handle_open(
	     ewfmount_mount_handle,
	     sources,
	     number_of_sources,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open source image(s)\n" );

		goto on_error;
	}
#if !defined( HAVE_GLOB_H )
	if( ewftools_glob_free(
	     &glob,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free glob.\n" );

		goto on_error;
	}
#endif
#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE )
	if( option_extended_options != NULL )
	{
		/* This argument is required but ignored
		 */
		if( fuse_opt_add_arg(
		     &ewfmount_fuse_arguments,
		     "" ) != 0 )
		{
			fprintf(
			 stderr,
			 "Unable add fuse arguments.\n" );

			goto on_error;
		}
		if( fuse_opt_add_arg(
		     &ewfmount_fuse_arguments,
		     "-o" ) != 0 )
		{
			fprintf(
			 stderr,
			 "Unable add fuse arguments.\n" );

			goto on_error;
		}
		if( fuse_opt_add_arg(
		     &ewfmount_fuse_arguments,
		     option_extended_options ) != 0 )
		{
			fprintf(
			 stderr,
			 "Unable add fuse arguments.\n" );

			goto on_error;
		}
	}
	if( memory_set(
	     &ewfmount_fuse_operations,
	     0,
	     sizeof( struct fuse_operations ) ) == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to clear fuse operations.\n" );

		goto on_error;
	}
	ewfmount_fuse_operations.open       = &mount_fuse_open;
	ewfmount_fuse_operations.read       = &mount_fuse_read;
	ewfmount_fuse_operations.release    = &mount_fuse_release;
	ewfmount_fuse_operations.opendir    = &mount_fuse_opendir;
	ewfmount_fuse_operations.readdir    = &mount_fuse_readdir;
	ewfmount_fuse_operations.releasedir = &mount_fuse_releasedir;
	ewfmount_fuse_operations.getattr    = &mount_fuse_getattr;
	ewfmount_fuse_operations.destroy    = &mount_fuse_destroy;

	ewfmount_fuse_channel = fuse_mount(
	                         mount_point,
	                         &ewfmount_fuse_arguments );

	if( ewfmount_fuse_channel == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to create fuse channel.\n" );

		goto on_error;
	}
	ewfmount_fuse_handle = fuse_new(
	                        ewfmount_fuse_channel,
	                        &ewfmount_fuse_arguments,
	                        &ewfmount_fuse_operations,
	                        sizeof( struct fuse_operations ),
	                        ewfmount_mount_handle );

	if( ewfmount_fuse_handle == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to create fuse handle.\n" );

		goto on_error;
	}
	if( verbose == 0 )
	{
		if( fuse_daemonize(
		     0 ) != 0 )
		{
			fprintf(
			 stderr,
			 "Unable to daemonize fuse.\n" );

			goto on_error;
		}
	}
	result = fuse_loop(
	          ewfmount_fuse_handle );

	if( result != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to run fuse loop.\n" );

		goto on_error;
	}
	fuse_destroy(
	 ewfmount_fuse_handle );

	fuse_opt_free_args(
	 &ewfmount_fuse_arguments );

	return( EXIT_SUCCESS );

#elif defined( HAVE_LIBDOKAN )
	if( memory_set(
	     &ewfmount_dokan_operations,
	     0,
	     sizeof( DOKAN_OPERATIONS ) ) == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to clear dokan operations.\n" );

		goto on_error;
	}
	if( memory_set(
	     &ewfmount_dokan_options,
	     0,
	     sizeof( DOKAN_OPTIONS ) ) == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to clear dokan options.\n" );

		goto on_error;
	}
	ewfmount_dokan_options.Version     = DOKAN_VERSION;
	ewfmount_dokan_options.ThreadCount = 0;
	ewfmount_dokan_options.MountPoint  = mount_point;

	if( verbose != 0 )
	{
		ewfmount_dokan_options.Options |= DOKAN_OPTION_STDERR;
#if defined( HAVE_DEBUG_OUTPUT )
		ewfmount_dokan_options.Options |= DOKAN_OPTION_DEBUG;
#endif
	}
/* This will only affect the drive properties
	ewfmount_dokan_options.Options |= DOKAN_OPTION_REMOVABLE;
*/

#if ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 )
	ewfmount_dokan_options.Options |= DOKAN_OPTION_KEEP_ALIVE;

	ewfmount_dokan_operations.CreateFile           = &mount_dokan_CreateFile;
	ewfmount_dokan_operations.OpenDirectory        = &mount_dokan_OpenDirectory;
	ewfmount_dokan_operations.CreateDirectory      = NULL;
	ewfmount_dokan_operations.Cleanup              = NULL;
	ewfmount_dokan_operations.CloseFile            = &mount_dokan_CloseFile;
	ewfmount_dokan_operations.ReadFile             = &mount_dokan_ReadFile;
	ewfmount_dokan_operations.WriteFile            = NULL;
	ewfmount_dokan_operations.FlushFileBuffers     = NULL;
	ewfmount_dokan_operations.GetFileInformation   = &mount_dokan_GetFileInformation;
	ewfmount_dokan_operations.FindFiles            = &mount_dokan_FindFiles;
	ewfmount_dokan_operations.FindFilesWithPattern = NULL;
	ewfmount_dokan_operations.SetFileAttributes    = NULL;
	ewfmount_dokan_operations.SetFileTime          = NULL;
	ewfmount_dokan_operations.DeleteFile           = NULL;
	ewfmount_dokan_operations.DeleteDirectory      = NULL;
	ewfmount_dokan_operations.MoveFile             = NULL;
	ewfmount_dokan_operations.SetEndOfFile         = NULL;
	ewfmount_dokan_operations.SetAllocationSize    = NULL;
	ewfmount_dokan_operations.LockFile             = NULL;
	ewfmount_dokan_operations.UnlockFile           = NULL;
	ewfmount_dokan_operations.GetFileSecurity      = NULL;
	ewfmount_dokan_operations.SetFileSecurity      = NULL;
	ewfmount_dokan_operations.GetDiskFreeSpace     = NULL;
	ewfmount_dokan_operations.GetVolumeInformation = &mount_dokan_GetVolumeInformation;
	ewfmount_dokan_operations.Unmount              = &mount_dokan_Unmount;

#else
	ewfmount_dokan_operations.ZwCreateFile         = &mount_dokan_ZwCreateFile;
	ewfmount_dokan_operations.Cleanup              = NULL;
	ewfmount_dokan_operations.CloseFile            = &mount_dokan_CloseFile;
	ewfmount_dokan_operations.ReadFile             = &mount_dokan_ReadFile;
	ewfmount_dokan_operations.WriteFile            = NULL;
	ewfmount_dokan_operations.FlushFileBuffers     = NULL;
	ewfmount_dokan_operations.GetFileInformation   = &mount_dokan_GetFileInformation;
	ewfmount_dokan_operations.FindFiles            = &mount_dokan_FindFiles;
	ewfmount_dokan_operations.FindFilesWithPattern = NULL;
	ewfmount_dokan_operations.SetFileAttributes    = NULL;
	ewfmount_dokan_operations.SetFileTime          = NULL;
	ewfmount_dokan_operations.DeleteFile           = NULL;
	ewfmount_dokan_operations.DeleteDirectory      = NULL;
	ewfmount_dokan_operations.MoveFile             = NULL;
	ewfmount_dokan_operations.SetEndOfFile         = NULL;
	ewfmount_dokan_operations.SetAllocationSize    = NULL;
	ewfmount_dokan_operations.LockFile             = NULL;
	ewfmount_dokan_operations.UnlockFile           = NULL;
	ewfmount_dokan_operations.GetFileSecurity      = NULL;
	ewfmount_dokan_operations.SetFileSecurity      = NULL;
	ewfmount_dokan_operations.GetDiskFreeSpace     = NULL;
	ewfmount_dokan_operations.GetVolumeInformation = &mount_dokan_GetVolumeInformation;
	ewfmount_dokan_operations.Unmounted            = NULL;
	ewfmount_dokan_operations.FindStreams          = NULL;
	ewfmount_dokan_operations.Mounted              = NULL;

#endif /* ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 ) */

	result = DokanMain(
	          &ewfmount_dokan_options,
	          &ewfmount_dokan_operations );

	switch( result )
	{
		case DOKAN_SUCCESS:
			break;

		case DOKAN_ERROR:
			fprintf(
			 stderr,
			 "Unable to run dokan main: generic error\n" );
			break;

		case DOKAN_DRIVE_LETTER_ERROR:
			fprintf(
			 stderr,
			 "Unable to run dokan main: bad drive letter\n" );
			break;

		case DOKAN_DRIVER_INSTALL_ERROR:
			fprintf(
			 stderr,
			 "Unable to run dokan main: unable to load driver\n" );
			break;

		case DOKAN_START_ERROR:
			fprintf(
			 stderr,
			 "Unable to run dokan main: driver error\n" );
			break;

		case DOKAN_MOUNT_ERROR:
			fprintf(
			 stderr,
			 "Unable to run dokan main: unable to assign drive letter\n" );
			break;

		case DOKAN_MOUNT_POINT_ERROR:
			fprintf(
			 stderr,
			 "Unable to run dokan main: mount point error\n" );
			break;

		default:
			fprintf(
			 stderr,
			 "Unable to run dokan main: unknown error: %d\n",
			 result );
			break;
	}
	return( EXIT_SUCCESS );

#else
	fprintf(
	 stderr,
	 "No sub system to mount EWF format.\n" );

	return( EXIT_FAILURE );

#endif /* defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE ) */

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE )
	if( ewfmount_fuse_handle != NULL )
	{
		fuse_destroy(
		 ewfmount_fuse_handle );
	}
	fuse_opt_free_args(
	 &ewfmount_fuse_arguments );
#endif
	if( ewfmount_mount_handle != NULL )
	{
		mount_handle_free(
		 &ewfmount_mount_handle,
		 NULL );
	}
#if !defined( HAVE_GLOB_H )
	if( glob != NULL )
	{
		ewftools_glob_free(
		 &glob,
		 NULL );
	}
#endif
	return( EXIT_FAILURE );
}

