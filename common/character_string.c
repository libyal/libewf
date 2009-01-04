/*
 * Character string functions
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

#include "common.h"
#include "character_string.h"
#include "endian.h"
#include "memory.h"
#include "notify.h"
#include "types.h"

/* Duplicates a string
 * Returns the pointer to the duplicate string, or NULL on error
 */
character_t *libewf_string_duplicate(
              character_t *string,
              size_t length )
{
	character_t *duplicate = NULL;
	static char *function  = "libewf_string_duplicate";

	if( string == NULL )
	{
		return( NULL );
	}
	if( length == 0 )
	{
		return( NULL );
	}
	if( length > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid length value exceeds maximum.\n",
		 function );

		return( NULL );
	}
	/* Add an additional character for the end of string
	 */
	length += 1;

	duplicate = (character_t *) memory_allocate(
	                             sizeof( character_t ) * length );

	if( duplicate == NULL )
	{
		notify_warning_printf( "%s: unable to create duplicate string.\n",
		 function );

		return( NULL );
	}
	if( string_copy(
	     duplicate,
	     string,
	     length ) == NULL )
	{
		notify_warning_printf( "%s: unable to set duplicate string.\n",
		 function );

		memory_free(
		 duplicate );

		return( NULL );
	}
	duplicate[ length - 1 ] = (character_t) '\0';

	return( duplicate );
}

/* Returns the value represented by a string, returns 0 error
 */
int64_t libewf_string_to_int64(
         const character_t *string,
         size_t length )
{
	character_t *end_of_string = NULL;
	static char *function      = "libewf_string_to_int64";
	int64_t value              = 0;

	if( string == NULL )
	{
		notify_warning_printf( "%s: invalid string.\n",
		 function );

		return( 0 );
	}
	if( length == 0 )
	{
		notify_warning_printf( "%s: string is emtpy.\n",
		 function );

		return( 0 );
	}
	if( length > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid length value exceeds maximum.\n",
		 function );

		return( 0 );
	}
	end_of_string = (character_t *) &string[ length - 1 ];

	value = string_to_signed_long_long(
	         string,
	         &end_of_string,
	         0 );

	if( value == (int64_t) LONG_MAX )
	{
		notify_warning_printf( "%s: unable to convert string.\n",
		 function );

		return( 0 );
	}
	return( value );
}

/* Returns the value represented by a string, returns 0 on error
 */
uint64_t libewf_string_to_uint64(
          const character_t *string,
          size_t length )
{
	character_t *end_of_string = NULL;
	static char *function      = "libewf_string_to_uint64";
	uint64_t value             = 0;

	if( string == NULL )
	{
		notify_warning_printf( "%s: invalid string.\n",
		 function );

		return( 0 );
	}
	if( length == 0 )
	{
		notify_warning_printf( "%s: string is emtpy.\n",
		 function );

		return( 0 );
	}
	if( length > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid length value exceeds maximum.\n",
		 function );

		return( 0 );
	}
	end_of_string = (character_t *) &string[ length - 1 ];

	value = string_to_unsigned_long_long(
	         string,
	         &end_of_string,
	         0 );

	if( value == (uint64_t) LONG_MAX )
	{
		notify_warning_printf( "%s: unable to convert string.\n",
		 function );

		return( 0 );
	}
	return( value );
}

/* Copies a multi byte UTF-16 stream to a character string
 * Returns 1 if successful, on -1 on error
 */
int libewf_string_copy_from_utf16_stream(
     character_t *string,
     size_t length_string,
     uint8_t *utf16_stream,
     size_t size_utf16_stream,
     uint8_t byte_order )
{
#if defined( HAVE_WIDE_CHARACTER_TYPE )
	mbstate_t conversion_state;
#endif
	static char *function   = "libewf_string_copy_from_utf16_stream";
	size_t utf16_iterator   = 0;
	size_t string_iterator  = 0;
	size_t zero_byte        = 0;
	uint8_t read_byte_order = 0;

	if( string == NULL )
	{
		notify_warning_printf( "%s: invalid string.\n",
		 function );

		return( -1 );
	}
	if( length_string > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid length value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( utf16_stream == NULL )
	{
		notify_warning_printf( "%s: invalid UTF-16 stream.\n",
		 function );

		return( -1 );
	}
	if( size_utf16_stream > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	/* Check if UTF-16 stream is in big or little endian
	 */
	if( ( (uint8_t) utf16_stream[ 0 ] == 0xff )
	 && ( (uint8_t) utf16_stream[ 1 ] == 0xfe ) )
	{
		read_byte_order = LIBEWF_ENDIAN_LITTLE;
		utf16_iterator  = 2;
	}
	else if( ( (uint8_t) utf16_stream[ 0 ] == 0xfe )
	      && ( (uint8_t) utf16_stream[ 1 ] == 0xff ) )
	{
		read_byte_order = LIBEWF_ENDIAN_BIG;
		utf16_iterator  = 2;
	}
	else
	{
		if( ( utf16_stream[ 0 ] == (character_t) '\0' )
		 && ( utf16_stream[ 1 ] != (character_t) '\0' ) )
		{
			read_byte_order = LIBEWF_ENDIAN_BIG;
		}
		else if( ( utf16_stream[ 0 ] != (character_t) '\0' )
		 && ( utf16_stream[ 1 ] == (character_t) '\0' ) )
		{
			read_byte_order = LIBEWF_ENDIAN_LITTLE;
		}
		else
		{
			if( ( byte_order != LIBEWF_ENDIAN_BIG )
			 && ( byte_order != LIBEWF_ENDIAN_LITTLE ) )
			{
				notify_warning_printf( "%s: unsupported byte order.\n",
				 function );

				return( -1 );
			}
			read_byte_order = byte_order;
		}
		utf16_iterator = 0;
	}
	/* Check if the UTF-16 stream is terminated with zero bytes
	 */
	if( ( utf16_stream[ size_utf16_stream - 2 ] != 0 )
	 || ( utf16_stream[ size_utf16_stream - 1 ] != 0 ) )
	{
		zero_byte = 1;
	}
	/* The UTF-16 stream contains twice as much bytes needed for the string
	 * it could contain two additional bytes representing byte order
	 */
	if( length_string < ( ( ( size_utf16_stream - utf16_iterator ) / 2 ) + zero_byte ) )
	{
		notify_warning_printf( "%s: string too small.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE )
	if( memory_set(
	     &conversion_state,
	     0,
	     sizeof( mbstate_t ) ) == NULL )
	{
		notify_warning_printf( "%s: unable to clear converion state.\n",
		 function );

		return( -1 );
	}
	if( mbsinit(
	     &conversion_state ) == 0 )
	{
		notify_warning_printf( "%s: unable to initialize converion state.\n",
		 function );

		return( -1 );
	}
#endif
	/* Convert stream
	 */
	while( utf16_iterator < size_utf16_stream )
	{
#if defined( HAVE_WIDE_CHARACTER_TYPE )
		if( mbrtowc(
		     &( string[ string_iterator ] ),
		     (const char *) &( utf16_stream[ utf16_iterator ] ),
		     2,
		     &conversion_state ) >= (size_t) -2 )
		{
			notify_warning_printf( "%s: unable to convert UTF-16 character: %02x %02x.\n",
			 function, utf16_stream[ utf16_iterator ], utf16_stream[ utf16_iterator + 1 ] );
		}
#else
		if( read_byte_order == LIBEWF_ENDIAN_BIG )
		{
			if( utf16_stream[ utf16_iterator ] == (uint8_t) '\0' )
			{
				string[ string_iterator ] = utf16_stream[ utf16_iterator + 1 ];
			}
			else
			{
				/* Add a place holder character
				 */
				string[ string_iterator ] = '_';
			}
		}
		else if( read_byte_order == LIBEWF_ENDIAN_LITTLE )
		{
			if( utf16_stream[ utf16_iterator + 1 ] == (uint8_t) '\0' )
			{
				string[ string_iterator ] = utf16_stream[ utf16_iterator ];
			}
			else
			{
				/* Add a place holder character
				 */
				string[ string_iterator ] = '_';
			}
		}
#endif
		utf16_iterator  += 2;
		string_iterator += 1;
	}
	string[ string_iterator ] = (character_t) '\0';

	return( 1 );
}

/* Copies a character string to a multi byte UTF-16 stream
 * Returns 1 if successful, on -1 on error
 */
int libewf_string_copy_to_utf16_stream(
     character_t *string,
     size_t length_string,
     uint8_t *utf16_stream,
     size_t size_utf16_stream,
     uint8_t byte_order )
{
#if defined( HAVE_WIDE_CHARACTER_TYPE )
	mbstate_t conversion_state;
#endif
	static char *function  = "libewf_string_copy_to_utf16_stream";
	size_t string_iterator = 0;
	size_t utf16_iterator  = 2;

	if( string == NULL )
	{
		notify_warning_printf( "%s: invalid string.\n",
		 function );

		return( -1 );
	}
	if( length_string > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid length value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( utf16_stream == NULL )
	{
		notify_warning_printf( "%s: invalid UTF-16 stream.\n",
		 function );

		return( -1 );
	}
	if( size_utf16_stream > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( ( byte_order != LIBEWF_ENDIAN_BIG )
	 && ( byte_order != LIBEWF_ENDIAN_LITTLE ) )
	{
		notify_warning_printf( "%s: unsupported byte order.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE )
	if( memory_set(
	     &conversion_state,
	     0,
	     sizeof( mbstate_t ) ) == NULL )
	{
		notify_warning_printf( "%s: unable to clear converion state.\n",
		 function );

		return( -1 );
	}
	if( mbsinit(
	     &conversion_state ) == 0 )
	{
		notify_warning_printf( "%s: unable to initialize converion state.\n",
		 function );

		return( -1 );
	}
#endif
	/* Two additional bytes required for the byte order indicator
	 */
	if( size_utf16_stream < ( ( length_string * 2 ) + 2 ) )
	{
		notify_warning_printf( "%s: UTF-16 stream too small.\n",
		 function );

		return( -1 );
	}
	/* Add the endian byte order
	 */
	if( byte_order == LIBEWF_ENDIAN_LITTLE )
	{
		utf16_stream[ 0 ] = 0xff;
		utf16_stream[ 1 ] = 0xfe;
	}
	else if( byte_order == LIBEWF_ENDIAN_BIG )
	{
		utf16_stream[ 0 ] = 0xfe;
		utf16_stream[ 1 ] = 0xff;
	}
	/* Convert the string
	 */
	while( string_iterator < length_string )
	{
#if defined( HAVE_WIDE_CHARACTER_TYPE )
		if( wcrtomb(
		     (char *) &( utf16_stream[ utf16_iterator ] ),
		     string[ string_iterator ],
		     &conversion_state ) >= (size_t) -1 )
		{
			notify_warning_printf( "%s: unable to convert character: %04x.\n",
			 function, string[ string_iterator ] );
		}
#else
		if( byte_order == LIBEWF_ENDIAN_LITTLE )
		{
			utf16_stream[ utf16_iterator     ] = (uint8_t) string[ string_iterator ];
			utf16_stream[ utf16_iterator + 1 ] = (uint8_t) '\0';
		}
		else if( byte_order == LIBEWF_ENDIAN_BIG )
		{
			utf16_stream[ utf16_iterator     ] = (uint8_t) '\0';
			utf16_stream[ utf16_iterator + 1 ] = (uint8_t) string[ string_iterator ];
		}
#endif
		string_iterator += 1;
		utf16_iterator  += 2;
	}
	utf16_stream[ size_utf16_stream - 2 ] = (uint8_t) '\0';
	utf16_stream[ size_utf16_stream - 1 ] = (uint8_t) '\0';

	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && !defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )

/* Sets ctime in the string
 * The string must be at least 32 characters of length
 * Returns the pointer to the string if successful or NULL on error
 */
character_t *libewf_string_ctime(
              const time_t *timestamp,
              character_t *string,
              size_t length )
{
	char ctime_string[ 32 ];

	static char *function = "libewf_string_ctime";

	if( timestamp == NULL )
	{
		notify_warning_printf( "%s: invalid timestamp.\n",
		 function );

		return( NULL );
	}
	if( string == NULL )
	{
		notify_warning_printf( "%s: invalid string.\n",
		 function );

		return( NULL );
	}
	if( length > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid length.\n",
		 function );

		return( NULL );
	}
	if( length < 32 )
	{
		notify_warning_printf( "%s: string too small.\n",
		 function );

		return( NULL );
	}
	if( date_time_ctime(
	     timestamp,
	     ctime_string,
	     32 ) == NULL )
	{
		notify_warning_printf( "%s: unable to set ctime string.\n",
		 function );

		return( NULL );
	}
	if( string_copy_char_to_wchar(
	     string,
	     ctime_string,
	     32 ) != 1 )
	{
		notify_warning_printf( "%s: unable to set string.\n",
		 function );

		return( NULL );
	}
	return( string );
}

#endif

