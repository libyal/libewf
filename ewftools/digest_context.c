/*
 * Crypographic digest context
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
#include <types.h>

#include <liberror.h>

#include "digest_context.h"

/* Initializes the digest context
 * Returns 1 if successful, 0 on failure or -1 on error
 */
int digest_context_initialize(
     digest_context_t *digest_context,
     uint8_t type,
     liberror_error_t **error )
{
#if defined( WINAPI )
	DWORD digest_type         = 0;
#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
	const EVP_MD *digest_type = NULL;
#endif
	static char *function     = "digest_context_initialize";

	if( digest_context == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid digest context.",
		 function );

		return( -1 );
	}
	if( ( type != DIGEST_CONTEXT_TYPE_MD5 )
	 && ( type != DIGEST_CONTEXT_TYPE_SHA1 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported digest context type.",
		 function );

		return( -1 );
	}
#if defined( WINAPI )
	digest_context->crypt_provider = 0;
	digest_context->hash           = 0;

	/* Request the AES crypt provider, fail back to the RSA crypt provider
	*/
 	if( CryptAcquireContext(
	     &digest_context->crypt_provider,
	     NULL,
	     NULL,
	     PROV_RSA_AES,
	     CRYPT_VERIFYCONTEXT ) == 0 )
	{
		if( CryptAcquireContext(
		     &digest_context->crypt_provider,
		     NULL,
		     NULL,
		     PROV_RSA_FULL,
		     CRYPT_VERIFYCONTEXT ) == 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create AES or RSA crypt provider.",
			 function );

			return( 0 );
		}
	}
	if( digest_context->crypt_provider == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: unable to create crypt provider.",
		 function );

		return( 0 );
	}
	if( type == DIGEST_CONTEXT_TYPE_MD5 )
	{
		digest_type = CALG_MD5;
	}
	else if( type == DIGEST_CONTEXT_TYPE_SHA1 )
	{
		digest_type = CALG_SHA1;
	}
	if( CryptCreateHash(
	     digest_context->crypt_provider,
	     digest_type,
	     0,
	     0,
	     &digest_context->hash ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create hash object.",
		 function );

		CryptReleaseContext(
		 digest_context->crypt_provider,
		 0 );

		return( 0 );
	}

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
	EVP_MD_CTX_init( digest_context );

	if( type == DIGEST_CONTEXT_TYPE_MD5 )
	{
		digest_type = EVP_md5();
	}
	else if( type == DIGEST_CONTEXT_TYPE_SHA1 )
	{
		digest_type = EVP_sha1();
	}
	if( EVP_DigestInit_ex(
	     digest_context,
	     digest_type,
	     NULL ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize context.",
		 function );

		return( 0 );
	}
#endif
	return( 1 );
}

/* Updates the digest context
 * Returns 1 if successful, 0 on failure or -1 on error
 */
int digest_context_update(
     digest_context_t *digest_context,
     uint8_t *buffer,
     size_t size,
     liberror_error_t **error )
{
	static char *function = "digest_context_update";

	if( digest_context == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid digest context.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
#if defined( WINAPI )
	if( digest_context->hash == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid digest context - missing hash.",
		 function );

		return( -1 );
	}
	if( CryptHashData(
	     digest_context->hash,
	     (BYTE *) buffer,
	     (DWORD) size,
	     0 ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to update digest context hash.",
		 function );

		return( 0 );
	}

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
	if( EVP_DigestUpdate(
	     digest_context,
	     (const void *) buffer,
	     (unsigned long) size ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to update digest hash.",
		 function );

		return( 0 );
	}
#endif
	return( 1 );
}

/* Finalizes the digest context
 * Returns 1 if successful, 0 on failure or -1 on error
 */
int digest_context_finalize(
     digest_context_t *digest_context,
     digest_hash_t *digest_hash,
     size_t *size,
     liberror_error_t **error )
{
	static char *function = "digest_context_finalize";

	if( digest_context == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid digest context.",
		 function );

		return( -1 );
	}
	if( digest_hash == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid digest hash.",
		 function );

		return( -1 );
	}
	if( *size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
#if defined( WINAPI )
	if( digest_context->hash == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid digest context - missing hash.",
		 function );

		return( -1 );
	}
	if( CryptGetHashParam(
	     digest_context->hash,
	     HP_HASHVAL,
	     (BYTE *) digest_hash,
	     (DWORD *) size,
	     0 ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to finalize digest hash.",
		 function );

		return( 0 );
	}
	if( digest_context->crypt_provider != 0 )
	{
		CryptReleaseContext(
		 digest_context->crypt_provider,
		 0 );
	}
	if( digest_context->hash != 0 )
	{
		CryptDestroyHash(
		 digest_context->hash );
	}

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
	if( EVP_DigestFinal_ex(
	     digest_context,
	     (unsigned char *) digest_hash,
	     (unsigned int *) size ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to finalize digest hash.",
		 function );

		return( 0 );
	}
	if( EVP_MD_CTX_cleanup(
	     digest_context ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to clean up digest context.",
		 function );
	}
#endif
	return( 1 );
}

