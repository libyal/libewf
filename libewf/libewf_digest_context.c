/*
 * libewf crypographic digest wrapper code
 *
 * Copyright (c) 2006-2007, Joachim Metz <forensics@hoffmannbv.nl>,
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
 * - All advertising materials mentioning features or use of this software
 *   must acknowledge the contribution by people stated in the acknowledgements.
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


#include "libewf_includes.h"
#include "libewf_common.h"
#include "libewf_digest_context.h"
#include "libewf_notify.h"

/* Initializes the digest context
 * Returns 1 if successful, 0 on failure, -1 on error
 */
int8_t libewf_digest_context_initialize( LIBEWF_DIGEST_CONTEXT* digest_context, uint8_t type )
{
#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
	const EVP_MD *digest_type = NULL;
#elif defined( HAVE_WINCPRYPT_H )
	DWORD digest_type   = 0;
#endif
	if( digest_context == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_digest_context_initialize: invalid digest context.\n" );

		return( -1 );
	}
	if( ( type != LIBEWF_DIGEST_CONTEXT_TYPE_MD5 )
	 && ( type != LIBEWF_DIGEST_CONTEXT_TYPE_SHA1 ) )
	{
		LIBEWF_WARNING_PRINT( "libewf_digest_context_alloc: unsupported digest context type.\n" );

		return( -1 );
	}
#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
	EVP_MD_CTX_init( digest_context );

	if( type == LIBEWF_DIGEST_CONTEXT_TYPE_MD5 )
	{
		digest_type = EVP_md5();
	}
	else if( type == LIBEWF_DIGEST_CONTEXT_TYPE_SHA1 )
	{
		digest_type = EVP_sha1();
	}
	if( EVP_DigestInit_ex( digest_context, digest_type, NULL ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_digest_context_initialize: unable to initialize context.\n" );

		return( 0 );
	}
#elif defined( HAVE_WINCPRYPT_H )
	digest_context->crypt_provider = 0;
	digest_context->hash           = 0;

	/* Request the AES crypt provider, fail back to the RSA crypt provider
	*/
 	if( CryptAcquireContext( &digest_context->crypt_provider, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT ) == 0 )
	{
		if( CryptAcquireContext( &digest_context->crypt_provider, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT ) == 0 )
		{
			LIBEWF_WARNING_PRINT( "libewf_digest_context_initialize: unable to create AES or RSA crypt provider.\n" );

			return( 0 );
		}
	}
	if( digest_context->crypt_provider == 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_digest_context_initialize: unable to create crypt provider.\n" );

		return( 0 );
	}
	if( type == LIBEWF_DIGEST_CONTEXT_TYPE_MD5 )
	{
		digest_type = CALG_MD5;
	}
	else if( type == LIBEWF_DIGEST_CONTEXT_TYPE_SHA1 )
	{
		digest_type = CALG_SHA1;
	}
	if( CryptCreateHash( digest_context->crypt_provider, digest_type, 0, 0, &digest_context->hash ) != 1 ) 
	{
		LIBEWF_WARNING_PRINT( "libewf_digest_context_initialize: unable to create hash object.\n" );

		CryptReleaseContext( digest_context->crypt_provider, 0 );

		return( 0 );
	}
#endif
	return( 1 );
}

/* Updates the digest context
 * Returns 1 if successful, 0 on failure, -1 on error
 */
int8_t libewf_digest_context_update( LIBEWF_DIGEST_CONTEXT* digest_context, uint8_t *buffer, size_t size )
{
	if( digest_context == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_digest_context_update: invalid digest context.\n" );

		return( -1 );
	}
	if( buffer == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_digest_context_update: invalid buffer.\n" );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "libewf_digest_context_update: invalid size value exceeds maximum.\n" );

		return( -1 );
	}
#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
	if( EVP_DigestUpdate( digest_context, (const void *) buffer, (unsigned long) size ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_digest_context_update: unable to update digest hash.\n" );

		return( 0 );
	}
#elif defined( HAVE_WINCPRYPT_H )
	if( digest_context->hash == 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_digest_context_update: invalid digest context - missing hash.\n" );

		return( -1 );
	}
	if( CryptHashData( digest_context->hash, (BYTE *) buffer, (DWORD) size, 0 ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_digest_context_update: unable to update digest context hash.\n" );

		return( 0 );
	}
#endif
	return( 1 );
}

/* Finalizes the digest context
 * Returns 1 if successful, 0 on failure, -1 on error
 */
int8_t libewf_digest_context_finalize( LIBEWF_DIGEST_CONTEXT* digest_context, EWF_DIGEST_HASH *digest_hash, size_t *size )
{
	if( digest_context == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_digest_context_finalize: invalid digest context.\n" );

		return( -1 );
	}
	if( digest_hash == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_digest_context_finalize: invalid digest hash.\n" );

		return( -1 );
	}
	if( *size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "libewf_digest_context_finalize: invalid size value exceeds maximum.\n" );

		return( -1 );
	}
#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
	if( EVP_DigestFinal_ex( digest_context, (unsigned char *) digest_hash, (unsigned int *) size  ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_digest_context_finalize: unable to finalize digest hash.\n" );

		return( 0 );
	}
	if( EVP_MD_CTX_cleanup( digest_context ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_digest_context_finalize: unable to clean up digest context.\n" );
	}
#elif defined( HAVE_WINCPRYPT_H )
	if( digest_context->hash == 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_digest_context_finalize: invalid digest context - missing hash.\n" );

		return( -1 );
	}
	if( CryptGetHashParam( digest_context->hash, HP_HASHVAL, (BYTE *) digest_hash, (DWORD *) size, 0 ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_digest_context_finalize: unable to finalize digest hash.\n" );

		return( 0 );
	}
	if( digest_context->crypt_provider != 0 )
	{
		CryptReleaseContext( digest_context->crypt_provider, 0 );
	}
	if( digest_context->hash != 0 )
	{
		CryptDestroyHash( digest_context->hash );
	}
#endif
	return( 1 );
}

