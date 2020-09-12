/*
 * Platform functions
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
#include <narrow_string.h>
#include <types.h>

#if defined( HAVE_SYS_UTSNAME_H )
#include <sys/utsname.h>
#endif

#if defined( WINAPI ) && ( WINVER >= 0x0a00 )
#include <VersionHelpers.h>
#endif

#include "ewftools_libcerror.h"
#include "ewftools_libclocale.h"
#include "ewftools_libuna.h"
#include "platform.h"

#if !defined( LIBEWF_OPERATING_SYSTEM )
#define LIBEWF_OPERATING_SYSTEM		"Unknown"
#endif

/* Determines the operating system string
 * Returns 1 if successful or -1 on error
 */
int platform_get_operating_system(
     system_character_t *operating_system_string,
     size_t operating_system_string_size,
     libcerror_error_t **error )
{
#if defined( HAVE_UNAME ) && !defined( WINAPI )
	struct utsname utsname_buffer;
#endif

	char *operating_system       = NULL;
	static char *function        = "platform_get_operating_system";
	size_t operating_system_size = 0;

#if defined( WINAPI )
	DWORD windows_version        = 0;
	DWORD windows_major_version  = 0;
	DWORD windows_minor_version  = 0;
#endif

	if( operating_system_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid operating system string.",
		 function );

		return( -1 );
	}
#if defined( WINAPI ) && ( WINVER >= 0x0a00 )

/* TODO consider detecting Windows server version with IsWindowsServer() */

	if( IsWindowsXPOrGreater() == FALSE )
	{
		operating_system = "Windows 2000";
	}
	else if( IsWindowsXPSP1OrGreater() == FALSE )
	{
		operating_system = "Windows XP";
	}
	else if( IsWindowsXPSP2OrGreater() == FALSE )
	{
		operating_system = "Windows XP SP1";
	}
	else if( IsWindowsXPSP3OrGreater() == FALSE )
	{
		operating_system = "Windows XP SP2";
	}
	else if( IsWindowsVistaOrGreater() == FALSE )
	{
		operating_system = "Windows XP SP3";
	}
	else if( IsWindowsVistaSP1OrGreater() == FALSE )
	{
		operating_system = "Windows Vista";
	}
	else if( IsWindowsVistaSP2OrGreater() == FALSE )
	{
		operating_system = "Windows Vista SP1";
	}
	else if( IsWindows7OrGreater() == FALSE )
	{
		operating_system = "Windows Vista SP2";
	}
	else if( IsWindows7SP1OrGreater() == FALSE )
	{
		operating_system = "Windows 7";
	}
	else if( IsWindows8OrGreater() == FALSE )
	{
		operating_system = "Windows 7 SP1";
	}
	else if( Windows8Point1OrGreater() == FALSE )
	{
		operating_system = "Windows 8.0";
	}
	else if( IsWindows10OrGreater() == FALSE )
	{
		operating_system = "Windows 8.1";
	}
	else
	{
		operating_system = "Windows 10";
	}

#elif defined( WINAPI )
	operating_system = "Windows";

	windows_version = GetVersion();

	windows_major_version = (DWORD) ( LOBYTE( LOWORD( windows_version ) ) );
	windows_minor_version = (DWORD) ( HIBYTE( LOWORD( windows_version ) ) );

	if( windows_major_version == 3 )
	{
		if( windows_version < 0x80000000 )
		{
			if( windows_minor_version == 51 )
			{
				operating_system = "Windows NT 3.51";
			}
		}
	}
	else if( windows_major_version == 4 )
	{
		if( windows_version < 0x80000000 )
		{
			if( windows_minor_version == 0 )
			{
				operating_system = "Windows NT 4";
			}
		}
		else
		{
			if( windows_minor_version == 0 )
			{
				operating_system = "Windows 95";
			}
			else if( windows_minor_version == 10 )
			{
				operating_system = "Windows 98";
			}
			else if( windows_minor_version == 90 )
			{
				operating_system = "Windows ME";
			}
		}
	}
	else if( windows_major_version == 5 )
	{
		if( windows_version < 0x80000000 )
		{
			if( windows_minor_version == 0 )
			{
				operating_system = "Windows 2000";
			}
			else if( windows_minor_version == 1 )
			{
				operating_system = "Windows XP";
			}
			else if( windows_minor_version == 2 )
			{
				operating_system = "Windows 2003";
			}
		}
	}
	else if( windows_major_version == 6 )
	{
		if( windows_version < 0x80000000 )
		{
			if( windows_minor_version == 0 )
			{
				operating_system = "Windows Vista/2008";
			}
			else if( windows_minor_version == 1 )
			{
				operating_system = "Windows 7";
			}
			else if( windows_minor_version == 2 )
			{
				operating_system = "Windows 8.0";
			}
			else if( windows_minor_version == 3 )
			{
				operating_system = "Windows 8.1";
			}
		}
	}

#elif defined( HAVE_UNAME )
	/* Determine the operating system
	 */
	if( uname(
	     &utsname_buffer ) == 0 )
	{
		operating_system = utsname_buffer.sysname;
	}
	else
	{
		operating_system = "Undetermined";
	}
#else
	/* Have configure determine the operating system
	 */
	operating_system = LIBEWF_OPERATING_SYSTEM;
#endif

	operating_system_size = 1 + narrow_string_length(
	                             operating_system );

	if( operating_system_string_size < operating_system_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: operating system string too small.",
		 function );

		return( -1 );
	}	
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
#if SIZEOF_WCHAR_T == 4
	if( libuna_utf32_string_copy_from_utf8(
	     (libuna_utf32_character_t *) operating_system_string,
	     operating_system_string_size,
	     (libuna_utf8_character_t *) operating_system,
	     operating_system_size,
	     error ) != 1 )
#elif SIZEOF_WCHAR_T == 2
	if( libuna_utf16_string_copy_from_utf8(
	     (libuna_utf16_character_t *) operating_system_string,
	     operating_system_string_size,
	     (libuna_utf8_character_t *) operating_system,
	     operating_system_size,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set string.",
		 function );

		return( -1 );
	}
#else
	if( libclocale_codepage == 0 )
	{
		if( operating_system_string_size < operating_system_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: string too small.",
			 function );

			return( -1 );
		}
		if( memory_copy(
		     operating_system_string,
		     operating_system,
		     operating_system_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set string.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( libuna_byte_stream_copy_from_utf8(
		     (uint8_t *) operating_system_string,
		     operating_system_string_size,
		     libclocale_codepage,
		     (libuna_utf8_character_t *) operating_system,
		     operating_system_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set string.",
			 function );

			return( -1 );
		}
	}
#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

	return( 1 );
}

