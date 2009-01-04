/*
 * Signal handling functions
 *
 * Copyright (c) 2006-2008, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of the creator, related organisations, nor the names of
 *   its contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER, COMPANY AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "../libewf/libewf_includes.h"
#include "../libewf/libewf_common.h"
#include "../libewf/libewf_notify.h"

#include "ewfsignal.h"

#if defined( HAVE_SIGNAL_H )

#if defined( HAVE_SYS_TYPES_H )
#include <sys/types.h>
#endif

#include <signal.h>

/* Attaches a signal handler for SIGINT
 * Returns 1 if successful or -1 on error
 */
int ewfsignal_attach(
     void (*signal_handler)( ewfsignal_t ) )
{
	static char *function = "ewfsignal_attach";

	if( signal_handler == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid signal handler.\n",
		 function );

		return( -1 );
	}
	if( signal(
	     SIGINT,
	     signal_handler ) == SIG_ERR )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to attach signal handler.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Detaches a signal handler for SIGINT
 * Returns 1 if successful or -1 on error
 */
int ewfsignal_detach(
     void )
{
	static char *function = "ewfsignal_detach";

	if( signal(
	     SIGINT,
	     SIG_DFL ) == SIG_ERR )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to detach signal handler.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* TODO remove function */
void ewfsignal_initialize( void )
{
}

#elif defined(HAVE_WINDOWS_API)

#include <windows.h>
#include <winnt.h>
#include <crtdbg.h>

/* You can pass this variable to your api if you
 * want to support cancellation.  The control handler
 * does nothing more than set the value of this variable to 1L.
 * On *nix you have to use signals to achieve the same result.
 */
volatile int32_t cancelled;

/* Signal handler for Ctrl+C or Ctrl+Break signals
 */
int WINAPI ewfsignal_signal_handler( unsigned long signal )
{
	static char *function = "ewfsignal_signal_handler";

	switch( signal )
	{
		/* use Ctrl+C or Ctrl+Break to simulate SERVICE_CONTROL_STOP in debug mode
		 */
		case CTRL_BREAK_EVENT:
		case CTRL_C_EVENT:
			LIBEWF_WARNING_PRINT( "%s: stopping at break signal.\n",
			 function );

			InterlockedExchange( (long *) &cancelled, 1 );

			return( 1 );

		default:
			break;
	}
	return( 0 );
}

/* Initialize memory usage and leakage debugging
 */
void ewfsignal_initialize_memory_debug( void )
{
	int flag = 0;

	/* Get the current state of the flag and store it in a temporary variable
	 */
	flag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );

	/* Turn on client block identifiers and automatic leak detection
	 */
	flag |= ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	/* Set the new state for the flag
	 */
	_CrtSetDbgFlag( flag );
}

/* Set up signal handling
 */
void ewfsignal_initialize( void )
{
	SetConsoleCtrlHandler( ewfsignal_signal_handler, 1 );

	SetConsoleCtrlHandler( NULL, FALSE );

	ewfsignal_initialize_memory_debug();

	SetErrorMode( SEM_FAILCRITICALERRORS );

#if defined( LOCALE_SUPPORT )
	/* Allow subsequent threads to have their own locale.
	 * The current application is single threaded so this
	 * call has no practical effect here.
	 */
	_configthreadlocale( _ENABLE_PER_THREAD_LOCALE );

	/* Set the current thread locale to the user default
	 * ANSI code page.
	 */
	setlocale( LC_ALL, "" );

	/* Set the the code page used by multibyte functions
	 * to use the same code page as the previous call to setlocale.
	 */
	_setmbcp( _MB_CP_LOCALE );
#endif
}

#endif

