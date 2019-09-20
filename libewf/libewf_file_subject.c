/*
 * File subject functions
 */

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libewf_definitions.h"
#include "libewf_file_subject.h"
#include "libewf_handle.h"
#include "libewf_libcdata.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libcthreads.h"
#include "libewf_single_file_subject.h"
#include "libewf_single_file_subject_tree.h"
#include "libewf_types.h"

/* Creates a file subject
 * Make sure the value file_subject is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_file_subject_initialize(
     libewf_file_subject_t **file_subject,
     libewf_internal_handle_t *internal_handle,
     libcdata_tree_node_t *file_subject_tree_node,
     libcerror_error_t **error )
{
	libewf_internal_file_subject_t *internal_file_subject = NULL;
	static char *function                                 = "libewf_file_subject_initialize";

	if( file_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file subject.",
		 function );

		return( -1 );
	}
	if( *file_subject != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file subject value already set.",
		 function );

		return( -1 );
	}
	internal_file_subject = memory_allocate_structure(
	                       libewf_internal_file_subject_t );

	if( internal_file_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file subject.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_file_subject,
	     0,
	     sizeof( libewf_internal_file_subject_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file subject.",
		 function );

		memory_free(
		 internal_file_subject );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_file_subject->read_write_lock ),
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
	internal_file_subject->internal_handle        = internal_handle;
	internal_file_subject->file_subject_tree_node = file_subject_tree_node;

	*file_subject = (libewf_file_subject_t *) internal_file_subject;

	return( 1 );

on_error:
	if( internal_file_subject != NULL )
	{
		memory_free(
		 internal_file_subject );
	}
	return( -1 );
}

/* Frees a file subject
 * Returns 1 if successful or -1 on error
 */
int libewf_file_subject_free(
     libewf_file_subject_t **file_subject,
     libcerror_error_t **error )
{
	libewf_internal_file_subject_t *internal_file_subject = NULL;
	static char *function                                 = "libewf_file_subject_free";
	int result                                            = 1;

	if( file_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file subject.",
		 function );

		return( -1 );
	}
	if( *file_subject != NULL )
	{
		internal_file_subject = (libewf_internal_file_subject_t *) *file_subject;
		*file_subject         = NULL;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( internal_file_subject->read_write_lock ),
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
		/* The internal_handle and file_subject_tree_node references are freed elsewhere
		 */
		memory_free(
		 internal_file_subject );
	}
	return( result );
}

/* Retrieves the type
 * Returns 1 if successful or -1 on error
 */
int libewf_file_subject_get_type(
     libewf_file_subject_t *file_subject,
     uint8_t *type,
     libcerror_error_t **error )
{
	libewf_internal_file_subject_t *internal_file_subject = NULL;
	libewf_single_file_subject_t *single_file_subject     = NULL;
	static char *function                                 = "libewf_file_subject_get_type";

	if( file_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file subject.",
		 function );

		return( -1 );
	}
	internal_file_subject = (libewf_internal_file_subject_t *) file_subject;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_subject->read_write_lock,
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
	     internal_file_subject->file_subject_tree_node,
	     (intptr_t **) &single_file_subject,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file subject tree node.",
		 function );

		goto on_error;
	}
	if( libewf_single_file_subject_get_type(
	     single_file_subject,
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
	     internal_file_subject->read_write_lock,
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
	 internal_file_subject->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_subject_get_utf8_name_size(
     libewf_file_subject_t *file_subject,
     size_t *utf8_name_size,
     libcerror_error_t **error )
{
	libewf_internal_file_subject_t *internal_file_subject = NULL;
	libewf_single_file_subject_t *single_file_subject     = NULL;
	static char *function                                 = "libewf_file_subject_get_utf8_name_size";

	if( file_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file subject.",
		 function );

		return( -1 );
	}
	internal_file_subject = (libewf_internal_file_subject_t *) file_subject;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_subject->read_write_lock,
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
	     internal_file_subject->file_subject_tree_node,
	     (intptr_t **) &single_file_subject,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file subject tree node.",
		 function );

		goto on_error;
	}
	if( libewf_single_file_subject_get_utf8_name_size(
	     single_file_subject,
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
	     internal_file_subject->read_write_lock,
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
	 internal_file_subject->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the UTF-8 encoded name value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_subject_get_utf8_name(
     libewf_file_subject_t *file_subject,
     uint8_t *utf8_name,
     size_t utf8_name_size,
     libcerror_error_t **error )
{
	libewf_internal_file_subject_t *internal_file_subject = NULL;
	libewf_single_file_subject_t *single_file_subject     = NULL;
	static char *function                                 = "libewf_file_subject_get_utf8_name";

	if( file_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file subject.",
		 function );

		return( -1 );
	}
	internal_file_subject = (libewf_internal_file_subject_t *) file_subject;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_subject->read_write_lock,
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
	     internal_file_subject->file_subject_tree_node,
	     (intptr_t **) &single_file_subject,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file subject tree node.",
		 function );

		goto on_error;
	}
	if( libewf_single_file_subject_get_utf8_name(
	     single_file_subject,
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
	     internal_file_subject->read_write_lock,
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
	 internal_file_subject->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_subject_get_utf16_name_size(
     libewf_file_subject_t *file_subject,
     size_t *utf16_name_size,
     libcerror_error_t **error )
{
	libewf_internal_file_subject_t *internal_file_subject = NULL;
	libewf_single_file_subject_t *single_file_subject     = NULL;
	static char *function                                 = "libewf_file_subject_get_utf16_name_size";

	if( file_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file subject.",
		 function );

		return( -1 );
	}
	internal_file_subject = (libewf_internal_file_subject_t *) file_subject;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_subject->read_write_lock,
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
	     internal_file_subject->file_subject_tree_node,
	     (intptr_t **) &single_file_subject,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file subject tree node.",
		 function );

		goto on_error;
	}
	if( libewf_single_file_subject_get_utf16_name_size(
	     single_file_subject,
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
	     internal_file_subject->read_write_lock,
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
	 internal_file_subject->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the UTF-16 encoded name value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_subject_get_utf16_name(
     libewf_file_subject_t *file_subject,
     uint16_t *utf16_name,
     size_t utf16_name_size,
     libcerror_error_t **error )
{
	libewf_internal_file_subject_t *internal_file_subject = NULL;
	libewf_single_file_subject_t *single_file_subject     = NULL;
	static char *function                                 = "libewf_file_subject_get_utf16_name";

	if( file_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file subject.",
		 function );

		return( -1 );
	}
	internal_file_subject = (libewf_internal_file_subject_t *) file_subject;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_subject->read_write_lock,
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
	     internal_file_subject->file_subject_tree_node,
	     (intptr_t **) &single_file_subject,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file subject tree node.",
		 function );

		goto on_error;
	}
	if( libewf_single_file_subject_get_utf16_name(
	     single_file_subject,
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
	     internal_file_subject->read_write_lock,
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
	 internal_file_subject->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the size of the UTF-8 encoded number
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_subject_get_utf8_number_size(
	libewf_file_subject_t *file_subject,
	size_t *utf8_number_size,
	libcerror_error_t **error)
{
	libewf_internal_file_subject_t *internal_file_subject = NULL;
	libewf_single_file_subject_t *single_file_subject     = NULL;
	static char *function                                 = "libewf_file_subject_get_utf8_number_size";

	if (file_subject == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file subject.",
			function);

		return(-1);
	}
	internal_file_subject = (libewf_internal_file_subject_t *)file_subject;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_subject->read_write_lock,
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
		internal_file_subject->file_subject_tree_node,
		(intptr_t **)&single_file_subject,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file subject tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_subject_get_utf8_number_size(
		single_file_subject,
		utf8_number_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-8 number size.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_subject->read_write_lock,
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
		internal_file_subject->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the UTF-8 encoded number value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_subject_get_utf8_number(
	libewf_file_subject_t *file_subject,
	uint8_t *utf8_number,
	size_t utf8_number_size,
	libcerror_error_t **error)
{
	libewf_internal_file_subject_t *internal_file_subject = NULL;
	libewf_single_file_subject_t *single_file_subject     = NULL;
	static char *function                                 = "libewf_file_subject_get_utf8_number";

	if (file_subject == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file subject.",
			function);

		return(-1);
	}
	internal_file_subject = (libewf_internal_file_subject_t *)file_subject;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_subject->read_write_lock,
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
		internal_file_subject->file_subject_tree_node,
		(intptr_t **)&single_file_subject,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file subject tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_subject_get_utf8_number(
		single_file_subject,
		utf8_number,
		utf8_number_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-8 number.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_subject->read_write_lock,
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
		internal_file_subject->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the size of the UTF-16 encoded number
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_subject_get_utf16_number_size(
	libewf_file_subject_t *file_subject,
	size_t *utf16_number_size,
	libcerror_error_t **error)
{
	libewf_internal_file_subject_t *internal_file_subject = NULL;
	libewf_single_file_subject_t *single_file_subject     = NULL;
	static char *function                                 = "libewf_file_subject_get_utf16_number_size";

	if (file_subject == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file subject.",
			function);

		return(-1);
	}
	internal_file_subject = (libewf_internal_file_subject_t *)file_subject;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_subject->read_write_lock,
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
		internal_file_subject->file_subject_tree_node,
		(intptr_t **)&single_file_subject,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file subject tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_subject_get_utf16_number_size(
		single_file_subject,
		utf16_number_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-16 number size.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_subject->read_write_lock,
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
		internal_file_subject->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the UTF-16 encoded number value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_subject_get_utf16_number(
	libewf_file_subject_t *file_subject,
	uint16_t *utf16_number,
	size_t utf16_number_size,
	libcerror_error_t **error)
{
	libewf_internal_file_subject_t *internal_file_subject = NULL;
	libewf_single_file_subject_t *single_file_subject     = NULL;
	static char *function                                 = "libewf_file_subject_get_utf16_number";

	if (file_subject == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file subject.",
			function);

		return(-1);
	}
	internal_file_subject = (libewf_internal_file_subject_t *)file_subject;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_subject->read_write_lock,
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
		internal_file_subject->file_subject_tree_node,
		(intptr_t **)&single_file_subject,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file subject tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_subject_get_utf16_number(
		single_file_subject,
		utf16_number,
		utf16_number_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-8 number.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_subject->read_write_lock,
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
		internal_file_subject->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the size of the UTF-8 encoded comment
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_subject_get_utf8_comment_size(
	libewf_file_subject_t *file_subject,
	size_t *utf8_comment_size,
	libcerror_error_t **error)
{
	libewf_internal_file_subject_t *internal_file_subject = NULL;
	libewf_single_file_subject_t *single_file_subject     = NULL;
	static char *function                                 = "libewf_file_subject_get_utf8_comment_size";

	if (file_subject == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file subject.",
			function);

		return(-1);
	}
	internal_file_subject = (libewf_internal_file_subject_t *)file_subject;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_subject->read_write_lock,
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
		internal_file_subject->file_subject_tree_node,
		(intptr_t **)&single_file_subject,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file subject tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_subject_get_utf8_comment_size(
		single_file_subject,
		utf8_comment_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-8 comment size.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_subject->read_write_lock,
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
		internal_file_subject->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the UTF-8 encoded comment value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_subject_get_utf8_comment(
	libewf_file_subject_t *file_subject,
	uint8_t *utf8_comment,
	size_t utf8_comment_size,
	libcerror_error_t **error)
{
	libewf_internal_file_subject_t *internal_file_subject = NULL;
	libewf_single_file_subject_t *single_file_subject     = NULL;
	static char *function                                 = "libewf_file_subject_get_utf8_comment";

	if (file_subject == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file subject.",
			function);

		return(-1);
	}
	internal_file_subject = (libewf_internal_file_subject_t *)file_subject;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_subject->read_write_lock,
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
		internal_file_subject->file_subject_tree_node,
		(intptr_t **)&single_file_subject,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file subject tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_subject_get_utf8_comment(
		single_file_subject,
		utf8_comment,
		utf8_comment_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-8 comment.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_subject->read_write_lock,
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
		internal_file_subject->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the size of the UTF-16 encoded comment
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_subject_get_utf16_comment_size(
	libewf_file_subject_t *file_subject,
	size_t *utf16_comment_size,
	libcerror_error_t **error)
{
	libewf_internal_file_subject_t *internal_file_subject = NULL;
	libewf_single_file_subject_t *single_file_subject     = NULL;
	static char *function                                 = "libewf_file_subject_get_utf16_comment_size";

	if (file_subject == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file subject.",
			function);

		return(-1);
	}
	internal_file_subject = (libewf_internal_file_subject_t *)file_subject;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_subject->read_write_lock,
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
		internal_file_subject->file_subject_tree_node,
		(intptr_t **)&single_file_subject,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file subject tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_subject_get_utf16_comment_size(
		single_file_subject,
		utf16_comment_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-16 comment size.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_subject->read_write_lock,
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
		internal_file_subject->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the UTF-16 encoded comment value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_subject_get_utf16_comment(
	libewf_file_subject_t *file_subject,
	uint16_t *utf16_comment,
	size_t utf16_comment_size,
	libcerror_error_t **error)
{
	libewf_internal_file_subject_t *internal_file_subject = NULL;
	libewf_single_file_subject_t *single_file_subject     = NULL;
	static char *function                                 = "libewf_file_subject_get_utf16_comment";

	if (file_subject == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file subject.",
			function);

		return(-1);
	}
	internal_file_subject = (libewf_internal_file_subject_t *)file_subject;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_subject->read_write_lock,
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
		internal_file_subject->file_subject_tree_node,
		(intptr_t **)&single_file_subject,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file subject tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_subject_get_utf16_comment(
		single_file_subject,
		utf16_comment,
		utf16_comment_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-8 comment.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_subject->read_write_lock,
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
		internal_file_subject->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the UTF-8 encoded GUID
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_file_subject_get_utf8_guid(
	libewf_file_subject_t *file_subject,
	uint8_t *utf8_string,
	size_t utf8_string_size,
	libcerror_error_t **error)
{
	libewf_internal_file_subject_t *internal_file_subject = NULL;
	libewf_single_file_subject_t *single_file_subject     = NULL;
	static char *function                                 = "libewf_file_subject_get_utf8_guid";
	int result = 0;

	if (file_subject == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file subject.",
			function);

		return(-1);
	}
	internal_file_subject = (libewf_internal_file_subject_t *)file_subject;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_subject->read_write_lock,
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
		internal_file_subject->file_subject_tree_node,
		(intptr_t **)&single_file_subject,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file subject tree node.",
			function);

		goto on_error;
	}
	result = libewf_single_file_subject_get_utf8_guid(
		single_file_subject,
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
		internal_file_subject->read_write_lock,
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
		internal_file_subject->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the UTF-16 encoded GUID
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_file_subject_get_utf16_guid(
	libewf_file_subject_t *file_subject,
	uint16_t *utf16_string,
	size_t utf16_string_size,
	libcerror_error_t **error)
{
	libewf_internal_file_subject_t *internal_file_subject = NULL;
	libewf_single_file_subject_t *single_file_subject     = NULL;
	static char *function                                 = "libewf_file_subject_get_utf16_guid";
	int result = 0;

	if (file_subject == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file subject.",
			function);

		return(-1);
	}
	internal_file_subject = (libewf_internal_file_subject_t *)file_subject;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_subject->read_write_lock,
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
		internal_file_subject->file_subject_tree_node,
		(intptr_t **)&single_file_subject,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file subject tree node.",
			function);

		goto on_error;
	}
	result = libewf_single_file_subject_get_utf16_guid(
		single_file_subject,
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
		internal_file_subject->read_write_lock,
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
		internal_file_subject->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the number of sub file subjects
 * Returns 1 if successful or -1 on error
 */
int libewf_file_subject_get_number_of_sub_file_subjects(
     libewf_file_subject_t *file_subject,
     int *number_of_sub_file_subjects,
     libcerror_error_t **error )
{
	libewf_internal_file_subject_t *internal_file_subject = NULL;
	static char *function                                 = "libewf_file_subject_get_number_of_sub_file_subjects";
	int result                                            = 0;

	if( file_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file subject.",
		 function );

		return( -1 );
	}
	internal_file_subject = (libewf_internal_file_subject_t *) file_subject;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_subject->read_write_lock,
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
	          internal_file_subject->file_subject_tree_node,
	          number_of_sub_file_subjects,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub file subjects.",
		 function );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_subject->read_write_lock,
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

/* Retrieves the sub file subject for the specific index
 * Returns 1 if successful or -1 on error
 */
int libewf_file_subject_get_sub_file_subject(
     libewf_file_subject_t *file_subject,
     int sub_file_subject_index,
     libewf_file_subject_t **sub_file_subject,
     libcerror_error_t **error )
{
	libewf_internal_file_subject_t *internal_file_subject = NULL;
	libcdata_tree_node_t *sub_node                        = NULL;
	static char *function                                 = "libewf_file_subject_get_sub_file_subject";

	if( file_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file subject.",
		 function );

		return( -1 );
	}
	internal_file_subject = (libewf_internal_file_subject_t *) file_subject;

	if( internal_file_subject->file_subject_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file subject - missing file subject tree node.",
		 function );

		return( -1 );
	}
	if( sub_file_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub file subject.",
		 function );

		return( -1 );
	}
	if( *sub_file_subject != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub file subject already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_subject->read_write_lock,
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
	     internal_file_subject->file_subject_tree_node,
             sub_file_subject_index,
             &sub_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub file subject tree node.",
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
	if( libewf_file_subject_initialize(
	     sub_file_subject,
	     internal_file_subject->internal_handle,
	     sub_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize sub file subject.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_subject->read_write_lock,
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
	 internal_file_subject->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the sub file subject for the specific UTF-8 encoded name
 * Returns 1 if successful, 0 if no such sub file subject or -1 on error
 */
int libewf_file_subject_get_sub_file_subject_by_id(
     libewf_file_subject_t *file_subject,
	 uint32_t id,
     libewf_file_subject_t **sub_file_subject,
     libcerror_error_t **error )
{
	libewf_internal_file_subject_t *internal_file_subject = NULL;
	libewf_single_file_subject_t *sub_single_file_subject = NULL;
	libcdata_tree_node_t *sub_node                        = NULL;
	static char *function                                 = "libewf_file_subject_get_sub_file_subject_by_id";
	int result                                            = 0;

	if( file_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file subject.",
		 function );

		return( -1 );
	}
	internal_file_subject = (libewf_internal_file_subject_t *) file_subject;

	if( internal_file_subject->file_subject_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file subject - missing file subject tree node.",
		 function );

		return( -1 );
	}
	if( sub_file_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub file subject.",
		 function );

		return( -1 );
	}
	if( *sub_file_subject != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub file subject already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_subject->read_write_lock,
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
	result = libewf_single_file_subject_tree_get_sub_node_by_id(
	          internal_file_subject->file_subject_tree_node,
	          id,
	          &sub_node,
	          &sub_single_file_subject,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub file subject by id.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libewf_file_subject_initialize(
		     sub_file_subject,
		     internal_file_subject->internal_handle,
		     sub_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize sub file subject.",
			 function );

			goto on_error;
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_subject->read_write_lock,
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
	 internal_file_subject->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

