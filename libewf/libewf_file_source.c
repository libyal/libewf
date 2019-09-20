/*
 * File source functions
 */

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libewf_definitions.h"
#include "libewf_file_source.h"
#include "libewf_handle.h"
#include "libewf_libcdata.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libcthreads.h"
#include "libewf_single_file_source.h"
#include "libewf_single_file_source_tree.h"
#include "libewf_types.h"

/* Creates a file source
 * Make sure the value file_source is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_initialize(
     libewf_file_source_t **file_source,
     libewf_internal_handle_t *internal_handle,
     libcdata_tree_node_t *file_source_tree_node,
     libcerror_error_t **error )
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	static char *function                               = "libewf_file_source_initialize";

	if( file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file source.",
		 function );

		return( -1 );
	}
	if( *file_source != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file source value already set.",
		 function );

		return( -1 );
	}
	internal_file_source = memory_allocate_structure(
	                       libewf_internal_file_source_t );

	if( internal_file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file source.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_file_source,
	     0,
	     sizeof( libewf_internal_file_source_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file source.",
		 function );

		memory_free(
		 internal_file_source );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_file_source->read_write_lock ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize read/write lock.",
		 function );

		goto on_error;
	}
#endif
	internal_file_source->internal_handle      = internal_handle;
	internal_file_source->file_source_tree_node = file_source_tree_node;

	*file_source = (libewf_file_source_t *) internal_file_source;

	return( 1 );

on_error:
	if( internal_file_source != NULL )
	{
		memory_free(
		 internal_file_source );
	}
	return( -1 );
}

/* Frees a file source
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_free(
     libewf_file_source_t **file_source,
     libcerror_error_t **error )
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	static char *function                               = "libewf_file_source_free";
	int result                                          = 1;

	if( file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file source.",
		 function );

		return( -1 );
	}
	if( *file_source != NULL )
	{
		internal_file_source = (libewf_internal_file_source_t *) *file_source;
		*file_source         = NULL;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( internal_file_source->read_write_lock ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free read/write lock.",
			 function );

			result = -1;
		}
#endif
		/* The internal_handle and file_source_tree_node references are freed elsewhere
		 */
		memory_free(
		 internal_file_source );
	}
	return( result );
}

/* Retrieves the type
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_type(
     libewf_file_source_t *file_source,
     uint8_t *type,
     libcerror_error_t **error )
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source     = NULL;
	static char *function                               = "libewf_file_source_get_type";

	if( file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file source.",
		 function );

		return( -1 );
	}
	internal_file_source = (libewf_internal_file_source_t *) file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_source->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libcdata_tree_node_get_value(
	     internal_file_source->file_source_tree_node,
	     (intptr_t **) &single_file_source,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file source tree node.",
		 function );

		goto on_error;
	}
	if( libewf_single_file_source_get_type(
	     single_file_source,
	     type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve type.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_source->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_file_source->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the drive_type
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_drive_type(
	libewf_file_source_t *file_source,
	uint8_t *drive_type,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source     = NULL;
	static char *function                               = "libewf_file_source_get_drive_type";

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_source_get_drive_type(
		single_file_source,
		drive_type,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve drive_type.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the physical offset
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_physical_offset(
     libewf_file_source_t *file_source,
     off64_t *physical_offset,
     libcerror_error_t **error )
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source     = NULL;
	static char *function                               = "libewf_file_source_get_physical_offset";

	if( file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file source.",
		 function );

		return( -1 );
	}
	internal_file_source = (libewf_internal_file_source_t *) file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_source->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libcdata_tree_node_get_value(
	     internal_file_source->file_source_tree_node,
	     (intptr_t **) &single_file_source,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file source tree node.",
		 function );

		goto on_error;
	}
	if( libewf_single_file_source_get_physical_offset(
	     single_file_source,
	     physical_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve physical offset.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_source->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_file_source->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the logical offset
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_logical_offset(
	libewf_file_source_t *file_source,
	off64_t *logical_offset,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source     = NULL;
	static char *function                               = "libewf_file_source_get_logical_offset";

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_source_get_logical_offset(
		single_file_source,
		logical_offset,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve logical offset.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the total size
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_total_size(
     libewf_file_source_t *file_source,
     size64_t *total_size,
     libcerror_error_t **error )
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source     = NULL;
	static char *function                               = "libewf_file_source_get_total_size";

	if( file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file source.",
		 function );

		return( -1 );
	}
	internal_file_source = (libewf_internal_file_source_t *) file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_source->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libcdata_tree_node_get_value(
	     internal_file_source->file_source_tree_node,
	     (intptr_t **) &single_file_source,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file source tree node.",
		 function );

		goto on_error;
	}
	if( libewf_single_file_source_get_total_size(
	     single_file_source,
	     total_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve total size.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_source->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_file_source->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_utf8_name_size(
     libewf_file_source_t *file_source,
     size_t *utf8_name_size,
     libcerror_error_t **error )
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source     = NULL;
	static char *function                               = "libewf_file_source_get_utf8_name_size";

	if( file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file source.",
		 function );

		return( -1 );
	}
	internal_file_source = (libewf_internal_file_source_t *) file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_source->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libcdata_tree_node_get_value(
	     internal_file_source->file_source_tree_node,
	     (intptr_t **) &single_file_source,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file source tree node.",
		 function );

		goto on_error;
	}
	if( libewf_single_file_source_get_utf8_name_size(
	     single_file_source,
	     utf8_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 name size.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_source->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_file_source->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the UTF-8 encoded name value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_utf8_name(
     libewf_file_source_t *file_source,
     uint8_t *utf8_name,
     size_t utf8_name_size,
     libcerror_error_t **error )
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source     = NULL;
	static char *function                               = "libewf_file_source_get_utf8_name";

	if( file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file source.",
		 function );

		return( -1 );
	}
	internal_file_source = (libewf_internal_file_source_t *) file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_source->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libcdata_tree_node_get_value(
	     internal_file_source->file_source_tree_node,
	     (intptr_t **) &single_file_source,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file source tree node.",
		 function );

		goto on_error;
	}
	if( libewf_single_file_source_get_utf8_name(
	     single_file_source,
	     utf8_name,
	     utf8_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 name.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_source->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_file_source->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_utf16_name_size(
     libewf_file_source_t *file_source,
     size_t *utf16_name_size,
     libcerror_error_t **error )
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source     = NULL;
	static char *function                               = "libewf_file_source_get_utf16_name_size";

	if( file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file source.",
		 function );

		return( -1 );
	}
	internal_file_source = (libewf_internal_file_source_t *) file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_source->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libcdata_tree_node_get_value(
	     internal_file_source->file_source_tree_node,
	     (intptr_t **) &single_file_source,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file source tree node.",
		 function );

		goto on_error;
	}
	if( libewf_single_file_source_get_utf16_name_size(
	     single_file_source,
	     utf16_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 name size.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_source->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_file_source->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the UTF-16 encoded name value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_utf16_name(
     libewf_file_source_t *file_source,
     uint16_t *utf16_name,
     size_t utf16_name_size,
     libcerror_error_t **error )
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source     = NULL;
	static char *function                               = "libewf_file_source_get_utf16_name";

	if( file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file source.",
		 function );

		return( -1 );
	}
	internal_file_source = (libewf_internal_file_source_t *) file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_source->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libcdata_tree_node_get_value(
	     internal_file_source->file_source_tree_node,
	     (intptr_t **) &single_file_source,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file source tree node.",
		 function );

		goto on_error;
	}
	if( libewf_single_file_source_get_utf16_name(
	     single_file_source,
	     utf16_name,
	     utf16_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 name.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_source->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_file_source->read_write_lock,
	 NULL );
#endif
	return( -1 );
}


/* Retrieves the size of the UTF-8 encoded serial_number
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_utf8_serial_number_size(
	libewf_file_source_t *file_source,
	size_t *utf8_serial_number_size,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source     = NULL;
	static char *function                               = "libewf_file_source_get_utf8_serial_number_size";

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_source_get_utf8_serial_number_size(
		single_file_source,
		utf8_serial_number_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-8 serial_number size.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the UTF-8 encoded serial_number value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_utf8_serial_number(
	libewf_file_source_t *file_source,
	uint8_t *utf8_serial_number,
	size_t utf8_serial_number_size,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source     = NULL;
	static char *function                               = "libewf_file_source_get_utf8_serial_number";

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_source_get_utf8_serial_number(
		single_file_source,
		utf8_serial_number,
		utf8_serial_number_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-8 serial_number.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the size of the UTF-16 encoded serial_number
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_utf16_serial_number_size(
	libewf_file_source_t *file_source,
	size_t *utf16_serial_number_size,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source     = NULL;
	static char *function                               = "libewf_file_source_get_utf16_serial_number_size";

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_source_get_utf16_serial_number_size(
		single_file_source,
		utf16_serial_number_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-16 serial_number size.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the UTF-16 encoded serial_number value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_utf16_serial_number(
	libewf_file_source_t *file_source,
	uint16_t *utf16_serial_number,
	size_t utf16_serial_number_size,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source     = NULL;
	static char *function                               = "libewf_file_source_get_utf16_serial_number";

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_source_get_utf16_serial_number(
		single_file_source,
		utf16_serial_number,
		utf16_serial_number_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-8 serial_number.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}


/* Retrieves the size of the UTF-8 encoded model
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_utf8_model_size(
	libewf_file_source_t *file_source,
	size_t *utf8_model_size,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source     = NULL;
	static char *function                               = "libewf_file_source_get_utf8_model_size";

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_source_get_utf8_model_size(
		single_file_source,
		utf8_model_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-8 model size.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the UTF-8 encoded model value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_utf8_model(
	libewf_file_source_t *file_source,
	uint8_t *utf8_model,
	size_t utf8_model_size,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source     = NULL;
	static char *function                               = "libewf_file_source_get_utf8_model";

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_source_get_utf8_model(
		single_file_source,
		utf8_model,
		utf8_model_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-8 model.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the size of the UTF-16 encoded model
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_utf16_model_size(
	libewf_file_source_t *file_source,
	size_t *utf16_model_size,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source     = NULL;
	static char *function                               = "libewf_file_source_get_utf16_model_size";

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_source_get_utf16_model_size(
		single_file_source,
		utf16_model_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-16 model size.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the UTF-16 encoded model value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_utf16_model(
	libewf_file_source_t *file_source,
	uint16_t *utf16_model,
	size_t utf16_model_size,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source     = NULL;
	static char *function                               = "libewf_file_source_get_utf16_model";

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_source_get_utf16_model(
		single_file_source,
		utf16_model,
		utf16_model_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-8 model.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}


/* Retrieves the size of the UTF-8 encoded manufacturer
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_utf8_manufacturer_size(
	libewf_file_source_t *file_source,
	size_t *utf8_manufacturer_size,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source     = NULL;
	static char *function                               = "libewf_file_source_get_utf8_manufacturer_size";

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_source_get_utf8_manufacturer_size(
		single_file_source,
		utf8_manufacturer_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-8 manufacturer size.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the UTF-8 encoded manufacturer value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_utf8_manufacturer(
	libewf_file_source_t *file_source,
	uint8_t *utf8_manufacturer,
	size_t utf8_manufacturer_size,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source     = NULL;
	static char *function                               = "libewf_file_source_get_utf8_manufacturer";

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_source_get_utf8_manufacturer(
		single_file_source,
		utf8_manufacturer,
		utf8_manufacturer_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-8 manufacturer.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the size of the UTF-16 encoded manufacturer
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_utf16_manufacturer_size(
	libewf_file_source_t *file_source,
	size_t *utf16_manufacturer_size,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source     = NULL;
	static char *function                               = "libewf_file_source_get_utf16_manufacturer_size";

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_source_get_utf16_manufacturer_size(
		single_file_source,
		utf16_manufacturer_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-16 manufacturer size.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the UTF-16 encoded manufacturer value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_utf16_manufacturer(
	libewf_file_source_t *file_source,
	uint16_t *utf16_manufacturer,
	size_t utf16_manufacturer_size,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source     = NULL;
	static char *function                               = "libewf_file_source_get_utf16_manufacturer";

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_source_get_utf16_manufacturer(
		single_file_source,
		utf16_manufacturer,
		utf16_manufacturer_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-8 manufacturer.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the size of the UTF-8 encoded evidence_number
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_utf8_evidence_number_size(
	libewf_file_source_t *file_source,
	size_t *utf8_evidence_number_size,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	static char *function = "libewf_file_source_get_utf8_evidence_number_size";

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_source_get_utf8_evidence_number_size(
		single_file_source,
		utf8_evidence_number_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-8 evidence_number size.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the UTF-8 encoded evidence_number value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_utf8_evidence_number(
	libewf_file_source_t *file_source,
	uint8_t *utf8_evidence_number,
	size_t utf8_evidence_number_size,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	static char *function = "libewf_file_source_get_utf8_evidence_number";

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_source_get_utf8_evidence_number(
		single_file_source,
		utf8_evidence_number,
		utf8_evidence_number_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-8 evidence_number.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the size of the UTF-16 encoded evidence_number
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_utf16_evidence_number_size(
	libewf_file_source_t *file_source,
	size_t *utf16_evidence_number_size,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	static char *function = "libewf_file_source_get_utf16_evidence_number_size";

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_source_get_utf16_evidence_number_size(
		single_file_source,
		utf16_evidence_number_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-16 evidence_number size.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the UTF-16 encoded evidence_number value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_utf16_evidence_number(
	libewf_file_source_t *file_source,
	uint16_t *utf16_evidence_number,
	size_t utf16_evidence_number_size,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	static char *function = "libewf_file_source_get_utf16_evidence_number";

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_source_get_utf16_evidence_number(
		single_file_source,
		utf16_evidence_number,
		utf16_evidence_number_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-8 evidence_number.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

int libewf_file_source_get_acquisition_time(
	libewf_file_source_t *file_source,
	uint32_t *acquisition_time,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	static char *function = "libewf_file_source_get_acquisition_time";

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file_source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_source_get_acquisition_time(
		single_file_source,
		acquisition_time,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve acquisition time.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the UTF-8 encoded MD5 hash value
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_file_source_get_utf8_hash_value_md5(
     libewf_file_source_t *file_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source     = NULL;
	static char *function                               = "libewf_file_source_get_utf8_hash_value_md5";
	int result                                          = 0;

	if( file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file source.",
		 function );

		return( -1 );
	}
	internal_file_source = (libewf_internal_file_source_t *) file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_source->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libcdata_tree_node_get_value(
	     internal_file_source->file_source_tree_node,
	     (intptr_t **) &single_file_source,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file source tree node.",
		 function );

		goto on_error;
	}
	result = libewf_single_file_source_get_utf8_hash_value_md5(
		  single_file_source,
		  utf8_string,
		  utf8_string_size,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve hash value: MD5.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_source->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_file_source->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the UTF-16 encoded MD5 hash value
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_file_source_get_utf16_hash_value_md5(
     libewf_file_source_t *file_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source     = NULL;
	static char *function                               = "libewf_file_source_get_utf16_hash_value_md5";
	int result                                          = 0;

	if( file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file source.",
		 function );

		return( -1 );
	}
	internal_file_source = (libewf_internal_file_source_t *) file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_source->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libcdata_tree_node_get_value(
	     internal_file_source->file_source_tree_node,
	     (intptr_t **) &single_file_source,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file source tree node.",
		 function );

		goto on_error;
	}
	result = libewf_single_file_source_get_utf16_hash_value_md5(
		  single_file_source,
		  utf16_string,
		  utf16_string_size,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve hash value: MD5.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_source->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_file_source->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the UTF-8 encoded SHA1 hash value
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_file_source_get_utf8_hash_value_sha1(
     libewf_file_source_t *file_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source     = NULL;
	static char *function                               = "libewf_file_source_get_utf8_hash_value_sha1";
	int result                                          = 0;

	if( file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file source.",
		 function );

		return( -1 );
	}
	internal_file_source = (libewf_internal_file_source_t *) file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_source->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libcdata_tree_node_get_value(
	     internal_file_source->file_source_tree_node,
	     (intptr_t **) &single_file_source,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file source tree node.",
		 function );

		goto on_error;
	}
	result = libewf_single_file_source_get_utf8_hash_value_sha1(
		  single_file_source,
		  utf8_string,
		  utf8_string_size,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve hash value: SHA1.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_source->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_file_source->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the UTF-16 encoded SHA1 hash value
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_file_source_get_utf16_hash_value_sha1(
     libewf_file_source_t *file_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source     = NULL;
	static char *function                               = "libewf_file_source_get_utf16_hash_value_sha1";
	int result                                          = 0;

	if( file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file source.",
		 function );

		return( -1 );
	}
	internal_file_source = (libewf_internal_file_source_t *) file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_source->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libcdata_tree_node_get_value(
	     internal_file_source->file_source_tree_node,
	     (intptr_t **) &single_file_source,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file source tree node.",
		 function );

		goto on_error;
	}
	result = libewf_single_file_source_get_utf16_hash_value_sha1(
		  single_file_source,
		  utf16_string,
		  utf16_string_size,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve hash value: SHA1.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_source->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_file_source->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the UTF-8 encoded GUID
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_file_source_get_utf8_guid(
	libewf_file_source_t *file_source,
	uint8_t *utf8_string,
	size_t utf8_string_size,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source     = NULL;
	static char *function                               = "libewf_file_source_get_utf8_guid";
	int result = 0;

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	result = libewf_single_file_source_get_utf8_guid(
		single_file_source,
		utf8_string,
		utf8_string_size,
		error);

	if (result == -1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve hash value: SHA1.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(result);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the UTF-16 encoded GUID
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_file_source_get_utf16_guid(
	libewf_file_source_t *file_source,
	uint16_t *utf16_string,
	size_t utf16_string_size,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source     = NULL;
	static char *function                               = "libewf_file_source_get_utf16_guid";
	int result = 0;

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	result = libewf_single_file_source_get_utf16_guid(
		single_file_source,
		utf16_string,
		utf16_string_size,
		error);

	if (result == -1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve hash value: SHA1.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(result);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the UTF-8 encoded primary GUID
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_file_source_get_utf8_primary_guid(
	libewf_file_source_t *file_source,
	uint8_t *utf8_string,
	size_t utf8_string_size,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source     = NULL;
	static char *function                               = "libewf_file_source_get_utf8_primary_guid";
	int result = 0;

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	result = libewf_single_file_source_get_utf8_primary_guid(
		single_file_source,
		utf8_string,
		utf8_string_size,
		error);

	if (result == -1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve hash value: SHA1.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(result);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the UTF-16 encoded primary GUID
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_file_source_get_utf16_primary_guid(
	libewf_file_source_t *file_source,
	uint16_t *utf16_string,
	size_t utf16_string_size,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source     = NULL;
	static char *function                               = "libewf_file_source_get_utf16_primary_guid";
	int result = 0;

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	result = libewf_single_file_source_get_utf16_primary_guid(
		single_file_source,
		utf16_string,
		utf16_string_size,
		error);

	if (result == -1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve hash value: SHA1.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(result);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the size of the UTF-8 encoded location
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_utf8_location_size(
	libewf_file_source_t *file_source,
	size_t *utf8_location_size,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	static char *function = "libewf_file_source_get_utf8_location_size";

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_source_get_utf8_location_size(
		single_file_source,
		utf8_location_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-8 location size.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the UTF-8 encoded location value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_utf8_location(
	libewf_file_source_t *file_source,
	uint8_t *utf8_location,
	size_t utf8_location_size,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	static char *function = "libewf_file_source_get_utf8_location";

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_source_get_utf8_location(
		single_file_source,
		utf8_location,
		utf8_location_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-8 location.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the size of the UTF-16 encoded location
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_utf16_location_size(
	libewf_file_source_t *file_source,
	size_t *utf16_location_size,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	static char *function = "libewf_file_source_get_utf16_location_size";

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_source_get_utf16_location_size(
		single_file_source,
		utf16_location_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-16 location size.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the UTF-16 encoded location value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_utf16_location(
	libewf_file_source_t *file_source,
	uint16_t *utf16_location,
	size_t utf16_location_size,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	static char *function = "libewf_file_source_get_utf16_location";

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_source_get_utf16_location(
		single_file_source,
		utf16_location,
		utf16_location_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-8 location.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the size of the UTF-8 encoded domain
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_utf8_domain_size(
	libewf_file_source_t *file_source,
	size_t *utf8_domain_size,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	static char *function = "libewf_file_source_get_utf8_domain_size";

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_source_get_utf8_domain_size(
		single_file_source,
		utf8_domain_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-8 domain size.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the UTF-8 encoded domain value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_utf8_domain(
	libewf_file_source_t *file_source,
	uint8_t *utf8_domain,
	size_t utf8_domain_size,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	static char *function = "libewf_file_source_get_utf8_domain";

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_source_get_utf8_domain(
		single_file_source,
		utf8_domain,
		utf8_domain_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-8 domain.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the size of the UTF-16 encoded domain
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_utf16_domain_size(
	libewf_file_source_t *file_source,
	size_t *utf16_domain_size,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	static char *function = "libewf_file_source_get_utf16_domain_size";

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_source_get_utf16_domain_size(
		single_file_source,
		utf16_domain_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-16 domain size.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the UTF-16 encoded domain value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_utf16_domain(
	libewf_file_source_t *file_source,
	uint16_t *utf16_domain,
	size_t utf16_domain_size,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	static char *function = "libewf_file_source_get_utf16_domain";

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_source_get_utf16_domain(
		single_file_source,
		utf16_domain,
		utf16_domain_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-8 domain.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the size of the UTF-8 encoded ip_address
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_utf8_ip_address_size(
	libewf_file_source_t *file_source,
	size_t *utf8_ip_address_size,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	static char *function = "libewf_file_source_get_utf8_ip_address_size";

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_source_get_utf8_ip_address_size(
		single_file_source,
		utf8_ip_address_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-8 ip_address size.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the UTF-8 encoded ip_address value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_utf8_ip_address(
	libewf_file_source_t *file_source,
	uint8_t *utf8_ip_address,
	size_t utf8_ip_address_size,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	static char *function = "libewf_file_source_get_utf8_ip_address";

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_source_get_utf8_ip_address(
		single_file_source,
		utf8_ip_address,
		utf8_ip_address_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-8 ip_address.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the size of the UTF-16 encoded ip_address
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_utf16_ip_address_size(
	libewf_file_source_t *file_source,
	size_t *utf16_ip_address_size,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	static char *function = "libewf_file_source_get_utf16_ip_address_size";

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_source_get_utf16_ip_address_size(
		single_file_source,
		utf16_ip_address_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-16 ip_address size.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the UTF-16 encoded ip_address value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_utf16_ip_address(
	libewf_file_source_t *file_source,
	uint16_t *utf16_ip_address,
	size_t utf16_ip_address_size,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	static char *function = "libewf_file_source_get_utf16_ip_address";

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_source_get_utf16_ip_address(
		single_file_source,
		utf16_ip_address,
		utf16_ip_address_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-8 ip_address.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the static_ip value
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_static_ip(
	libewf_file_source_t *file_source,
	uint8_t *static_ip,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	static char *function = "libewf_file_source_get_static_ip";

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_source_get_static_ip(
		single_file_source,
		static_ip,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve static_ip.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the size of the UTF-8 encoded mac_address
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_utf8_mac_address_size(
	libewf_file_source_t *file_source,
	size_t *utf8_mac_address_size,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	static char *function = "libewf_file_source_get_utf8_mac_address_size";

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_source_get_utf8_mac_address_size(
		single_file_source,
		utf8_mac_address_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-8 mac_address size.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the UTF-8 encoded mac_address value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_utf8_mac_address(
	libewf_file_source_t *file_source,
	uint8_t *utf8_mac_address,
	size_t utf8_mac_address_size,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	static char *function = "libewf_file_source_get_utf8_mac_address";

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_source_get_utf8_mac_address(
		single_file_source,
		utf8_mac_address,
		utf8_mac_address_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-8 mac_address.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the size of the UTF-16 encoded mac_address
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_utf16_mac_address_size(
	libewf_file_source_t *file_source,
	size_t *utf16_mac_address_size,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	static char *function = "libewf_file_source_get_utf16_mac_address_size";

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_source_get_utf16_mac_address_size(
		single_file_source,
		utf16_mac_address_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-16 mac_address size.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the UTF-16 encoded mac_address value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_utf16_mac_address(
	libewf_file_source_t *file_source,
	uint16_t *utf16_mac_address,
	size_t utf16_mac_address_size,
	libcerror_error_t **error)
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *single_file_source = NULL;
	static char *function = "libewf_file_source_get_utf16_mac_address";

	if (file_source == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file source.",
			function);

		return(-1);
	}
	internal_file_source = (libewf_internal_file_source_t *)file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_source->file_source_tree_node,
		(intptr_t **)&single_file_source,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file source tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_source_get_utf16_mac_address(
		single_file_source,
		utf16_mac_address,
		utf16_mac_address_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-8 mac_address.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_source->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the number of sub file sources
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_number_of_sub_file_sources(
     libewf_file_source_t *file_source,
     int *number_of_sub_file_sources,
     libcerror_error_t **error )
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	static char *function                               = "libewf_file_source_get_number_of_sub_file_sources";
	int result                                          = 0;

	if( file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file source.",
		 function );

		return( -1 );
	}
	internal_file_source = (libewf_internal_file_source_t *) file_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_source->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libcdata_tree_node_get_number_of_sub_nodes(
	          internal_file_source->file_source_tree_node,
	          number_of_sub_file_sources,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub file sources.",
		 function );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_source->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the sub file source for the specific index
 * Returns 1 if successful or -1 on error
 */
int libewf_file_source_get_sub_file_source(
     libewf_file_source_t *file_source,
     int sub_file_source_index,
     libewf_file_source_t **sub_file_source,
     libcerror_error_t **error )
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libcdata_tree_node_t *sub_node                      = NULL;
	static char *function                               = "libewf_file_source_get_sub_file_source";

	if( file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file source.",
		 function );

		return( -1 );
	}
	internal_file_source = (libewf_internal_file_source_t *) file_source;

	if( internal_file_source->file_source_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file source - missing file source tree node.",
		 function );

		return( -1 );
	}
	if( sub_file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub file source.",
		 function );

		return( -1 );
	}
	if( *sub_file_source != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub file source already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_source->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libcdata_tree_node_get_sub_node_by_index(
	     internal_file_source->file_source_tree_node,
             sub_file_source_index,
             &sub_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub file source tree node.",
		 function );

		goto on_error;
	}
	if( sub_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid sub node.",
		 function );

		goto on_error;
	}
	if( libewf_file_source_initialize(
	     sub_file_source,
	     internal_file_source->internal_handle,
	     sub_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize sub file source.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_source->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_file_source->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the sub file source for the specific UTF-8 encoded name
 * Returns 1 if successful, 0 if no such sub file source or -1 on error
 */
int libewf_file_source_get_sub_file_source_by_id(
     libewf_file_source_t *file_source,
	 uint32_t id,
     libewf_file_source_t **sub_file_source,
     libcerror_error_t **error )
{
	libewf_internal_file_source_t *internal_file_source = NULL;
	libewf_single_file_source_t *sub_single_file_source = NULL;
	libcdata_tree_node_t *sub_node                      = NULL;
	static char *function                               = "libewf_file_source_get_sub_file_source_by_id";
	int result                                          = 0;

	if( file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file source.",
		 function );

		return( -1 );
	}
	internal_file_source = (libewf_internal_file_source_t *) file_source;

	if( internal_file_source->file_source_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file source - missing file source tree node.",
		 function );

		return( -1 );
	}
	if( sub_file_source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub file source.",
		 function );

		return( -1 );
	}
	if( *sub_file_source != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub file source already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_source->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libewf_single_file_source_tree_get_sub_node_by_id(
	          internal_file_source->file_source_tree_node,
	          id,
	          &sub_node,
	          &sub_single_file_source,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub file source by id.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libewf_file_source_initialize(
		     sub_file_source,
		     internal_file_source->internal_handle,
		     sub_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize sub file source.",
			 function );

			goto on_error;
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_source->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_file_source->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

