/*
 * OSS-Fuzz target for the libewf write path.
 *
 * The production handle_fuzzer only opens an EWF image for read. Acquisition
 * tools (ewfacquire) instead open for write, set format / compression /
 * sector geometry and forensic header values, then stream media bytes and
 * finalize. Those bytes are attacker-controlled when a tool converts or
 * re-wraps an untrusted image, and the write/finalize code is dark in the
 * production coverage report.
 *
 * Copyright (C) 2026 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <vector>

#include <fuzzer/FuzzedDataProvider.h>

/* Note that some of the OSS-Fuzz engines use C++
 */
extern "C" {

#include "ossfuzz_libewf.h"

static const uint8_t kFormats[] = {
	LIBEWF_FORMAT_ENCASE5,
	LIBEWF_FORMAT_ENCASE6,
	LIBEWF_FORMAT_ENCASE7,
	LIBEWF_FORMAT_SMART,
	LIBEWF_FORMAT_FTK_IMAGER,
	LIBEWF_FORMAT_LINEN6,
	LIBEWF_FORMAT_EWF,
	LIBEWF_FORMAT_EWFX
};

static const uint8_t kMediaTypes[] = {
	LIBEWF_MEDIA_TYPE_REMOVABLE,
	LIBEWF_MEDIA_TYPE_FIXED,
	LIBEWF_MEDIA_TYPE_OPTICAL,
	LIBEWF_MEDIA_TYPE_MEMORY
};

static const int8_t kCompressionLevels[] = {
	LIBEWF_COMPRESSION_NONE,
	LIBEWF_COMPRESSION_FAST,
	LIBEWF_COMPRESSION_BEST
};

static const uint32_t kSectorsPerChunk[] = { 1, 16, 64, 256 };
static const uint32_t kBytesPerSector[] = { 512, 1024, 2048, 4096 };

/* A failure is not fatal: some identifiers are rejected for some formats.
 */
static void set_header(
             libewf_handle_t *handle,
             const char *identifier,
             const std::string &value )
{
	if( value.empty() )
	{
		return;
	}
	libewf_handle_set_utf8_header_value(
	 handle,
	 (uint8_t *) identifier,
	 strlen( identifier ),
	 (uint8_t *) value.data(),
	 value.size(),
	 NULL );
}

int LLVMFuzzerTestOneInput(
     const uint8_t *data,
     size_t size )
{
	FuzzedDataProvider provider( data, size );

	libewf_handle_t *handle = NULL;
	char filename[ 64 ];
	char *filenames[ 1 ];

	uint8_t format = kFormats[ provider.ConsumeIntegralInRange<size_t>(
	     0, ( sizeof( kFormats ) / sizeof( kFormats[ 0 ] ) ) - 1 ) ];
	uint8_t media_type = kMediaTypes[ provider.ConsumeIntegralInRange<size_t>(
	     0, ( sizeof( kMediaTypes ) / sizeof( kMediaTypes[ 0 ] ) ) - 1 ) ];
	int8_t compression_level = kCompressionLevels[
	     provider.ConsumeIntegralInRange<size_t>(
	      0, ( sizeof( kCompressionLevels ) / sizeof( kCompressionLevels[ 0 ] ) ) - 1 ) ];
	uint32_t sectors_per_chunk = kSectorsPerChunk[
	     provider.ConsumeIntegralInRange<size_t>(
	      0, ( sizeof( kSectorsPerChunk ) / sizeof( kSectorsPerChunk[ 0 ] ) ) - 1 ) ];
	uint32_t bytes_per_sector = kBytesPerSector[
	     provider.ConsumeIntegralInRange<size_t>(
	      0, ( sizeof( kBytesPerSector ) / sizeof( kBytesPerSector[ 0 ] ) ) - 1 ) ];
	uint8_t media_flags = provider.ConsumeIntegralInRange<uint8_t>( 0, 0x0e );
	uint32_t error_granularity = provider.ConsumeIntegralInRange<uint32_t>( 1, 64 );
	int write_rounds = provider.ConsumeIntegralInRange<int>( 1, 4 );

	std::string case_number = provider.ConsumeRandomLengthString( 32 );
	std::string description = provider.ConsumeRandomLengthString( 48 );
	std::string examiner_name = provider.ConsumeRandomLengthString( 32 );
	std::string notes = provider.ConsumeRandomLengthString( 48 );

	/* libewf_handle_open(WRITE) treats the name as a basename and appends
	 * the segment extension itself. Passing ".E01" makes it look for a
	 * second extension and the open fails.
	 */
	snprintf(
	 filename, sizeof( filename ), "/tmp/ewf-%d", (int) getpid() );
	filenames[ 0 ] = filename;

	if( libewf_handle_initialize( &handle, NULL ) != 1 )
	{
		return( 0 );
	}
	if( libewf_handle_open(
	     handle, filenames, 1, LIBEWF_OPEN_WRITE, NULL ) != 1 )
	{
		goto on_error;
	}
	libewf_handle_set_format( handle, format, NULL );
	libewf_handle_set_bytes_per_sector( handle, bytes_per_sector, NULL );
	libewf_handle_set_sectors_per_chunk( handle, sectors_per_chunk, NULL );
#if defined( MEMORY_SANITIZER )
	/* System zlib is not MSan-instrumented in this image, so a deflate
	 * buffer looks uninitialized when it is written back. Keep compression
	 * off under MSan only; ASan/UBSan still sweep the compression levels.
	 */
	libewf_handle_set_compression_values(
	 handle, LIBEWF_COMPRESSION_NONE, 0, NULL );
#else
	libewf_handle_set_compression_values(
	 handle, compression_level, 0, NULL );
#endif
	libewf_handle_set_media_type( handle, media_type, NULL );
	libewf_handle_set_media_flags( handle, media_flags, NULL );
	libewf_handle_set_error_granularity( handle, error_granularity, NULL );
	/* Bound the image so write + finalize stay inside one input.
	 */
	libewf_handle_set_media_size(
	 handle,
	 (size64_t) bytes_per_sector * (size64_t) ( 1 + ( write_rounds * 4 ) ),
	 NULL );
	libewf_handle_set_header_values_date_format(
	 handle, LIBEWF_DATE_FORMAT_ISO8601, NULL );

	set_header( handle, "case_number", case_number );
	set_header( handle, "description", description );
	set_header( handle, "examiner_name", examiner_name );
	set_header( handle, "notes", notes );
	set_header( handle, "acquiry_date", std::string( "2026-01-01 00:00:00" ) );

	/* Only set the hash when the input actually supplies 16 bytes.
	 * ConsumeData zero-fills a short read, which MSan then reports as
	 * an uninitialized value inside libewf_hash_values_parse_md5_hash.
	 */
	if( provider.remaining_bytes() >= 16 )
	{
		std::vector<uint8_t> md5 = provider.ConsumeBytes<uint8_t>( 16 );

		libewf_handle_set_md5_hash( handle, md5.data(), 16, NULL );
	}
	if( provider.remaining_bytes() >= 16 )
	{
		std::vector<uint8_t> guid_bytes = provider.ConsumeBytes<uint8_t>( 16 );

		libewf_handle_set_segment_file_set_identifier(
		 handle, guid_bytes.data(), 16, NULL );
	}

	for( int i = 0; i < 2; i++ )
	{
		uint64_t start = provider.ConsumeIntegralInRange<uint64_t>( 0, 1024 );
		uint64_t count = provider.ConsumeIntegralInRange<uint64_t>( 1, 32 );

		libewf_handle_append_session( handle, start, count, NULL );
		libewf_handle_append_track( handle, start, count, NULL );
		libewf_handle_append_acquiry_error( handle, start, count, NULL );
	}
	for( int round = 0; round < write_rounds; round++ )
	{
		size_t want = provider.ConsumeIntegralInRange<size_t>( 1, 2048 );
		std::vector<uint8_t> bytes = provider.ConsumeBytes<uint8_t>( want );

		if( bytes.empty() )
		{
			break;
		}
		libewf_handle_write_buffer(
		 handle, bytes.data(), bytes.size(), NULL );
	}
	libewf_handle_write_finalize( handle, NULL );
	libewf_handle_close( handle, NULL );

on_error:
	if( handle != NULL )
	{
		libewf_handle_free( &handle, NULL );
	}
	unlink( filename );
	{
		char segment_name[ 72 ];

		snprintf(
		 segment_name, sizeof( segment_name ), "%s.E01", filename );
		unlink( segment_name );
		snprintf(
		 segment_name, sizeof( segment_name ), "%s.E02", filename );
		unlink( segment_name );
		snprintf(
		 segment_name, sizeof( segment_name ), "%s.s01", filename );
		unlink( segment_name );
	}
	return( 0 );
}

} /* extern "C" */
