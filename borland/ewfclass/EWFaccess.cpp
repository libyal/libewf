/**
 ******************************************************************************
 *
 *   Copyright (C) 2009, .... All Rights Reserved.
 *
 ******************************************************************************
 *
 * @file EWFaccess.h
 *
 * @brief Implementation of class for access to EWF files (images).
 * @author Sergey Kalkamanov
 *
 * <b>Modification History</b>
 * @code
 *   Date        Name        Description
 *  16/06/09     sergek      Created.
 * @endcode
 * $Id: EWFaccess.h,v 1.1 2009-06-16 10:01:59 skalkamanov Exp $
 *
 ******************************************************************************
 * @n @b encoding:   US-ASCII
 * @n @b tabsize:   8 (not used)
 * @n @b indentation:2
 *
 */

#include "EWFaccess.h"
#include <libewf.h>



namespace
{
	class SmartHandler
	{
		public:
#if defined( HAVE_V2_API )
			static void* makeEmpty( )
			{
			   SmartHandler* instance = new SmartHandler();

			   instance->_byInit = 1;
			   return instance;
			}
#endif
			static void* open( libewf_handle_t * h )
			{
			   if( NULL == h )
				   return NULL;
			   SmartHandler* instance = new SmartHandler(h);

			   instance->_byInit = 0;
			   return instance;
			}

			static int close( void * ref )
			{
				SmartHandler* instance = (SmartHandler*) ref;
				if( NULL == ref )
					return NULL;
				if( 1 == instance->_counter )
				{
				   return libewf_close( instance->_handle );
				}
				return -1;
			}

#if defined( HAVE_V2_API )
			static int close( void * ref, libewf_error_t **error )
			{
				if( NULL == ref )
					return NULL;
				SmartHandler* instance = (SmartHandler*) ref;
				if( 1 == instance->_counter )
				{
				   return libewf_handle_close( instance->_handle, error );
				}
				return -1;
			}
#endif

			static void* addRef( void* ref )
			{
				SmartHandler* instance = (SmartHandler*) ref;
				if( NULL == ref )
					return NULL;
			   instance->_counter++;
			   return ref;
			}

			static void* delRef( void* ref )
			{
				SmartHandler* instance = (SmartHandler*) ref;
				int retCode = 1;
				if( NULL == ref )
					return ref;
				instance->_counter--;
				if( 0 == instance->_counter )
				{
					delete instance;
					return NULL;
				}
				return ref;
			}

			static libewf_handle_t * getHandler( void* ref )
			{
				return  ((SmartHandler*)ref)->_handle;
			}

#if defined( HAVE_V2_API )
			static libewf_handle_t ** getRefHandler( void* ref )
			{
                                static libewf_handle_t * nullPTR = NULL;
				SmartHandler* instance = (SmartHandler*) ref;
				if( NULL == ref )
					return &nullPTR;
				if( 1 == instance->_byInit )
					return  &(instance->_handle);
				else
					return NULL;
			}

			static int isFreeHandle( void* ref )
			{
				if( NULL == ref )
					return 0;
				return  ((SmartHandler*)ref)->_byInit;
			}
#endif
		protected:
			SmartHandler( ) : _handle(NULL), _counter(1) {};
			SmartHandler( libewf_handle_t * h ) : _handle(h), _counter(1) {};
			~SmartHandler() { libewf_close( getHandler( this ) ); };
			libewf_handle_t * _handle;
			int               _counter;
			int               _byInit;
	};
}

#define HANDLER SmartHandler::getHandler( handler )

EWFaccess::EWFaccess( void )
{
  handler = NULL;
}

EWFaccess::EWFaccess( const EWFaccess& parent )
{
  handler = SmartHandler::addRef( parent.handler );
}

EWFaccess& EWFaccess::operator = (const EWFaccess& parent )
{
	handler = SmartHandler::addRef( parent.handler );
	return *this;
}

EWFaccess::~EWFaccess()
{
	handler = SmartHandler::delRef( handler );
}

/***************** API *************************************/

const char * EWFaccess::get_version( void )
{
	return libewf_get_version();
}

uint8_t EWFaccess::get_flags_read( void )
{
	return libewf_get_flags_read();
}

uint8_t EWFaccess::get_flags_read_write( void )
{
  return libewf_get_flags_read_write();
}

uint8_t EWFaccess::get_flags_write( void )
{
  return libewf_get_flags_write();
}

int EWFaccess::check_file_signature( const char *filename )
{
#if defined( HAVE_V2_API )
  libewf_error_t * error;
  return libewf_check_file_signature( filename, &error );
#else
  return libewf_check_file_signature( filename );
#endif
}

int EWFaccess::check_file_signature_wide( const wchar_t *filename )
{
#if defined( HAVE_V2_API )
  libewf_error_t * error;
  return libewf_check_file_signature_wide( filename, &error );
#else
  return libewf_check_file_signature_wide( filename );
#endif
}
#if defined( HAVE_V2_API )
int EWFaccess::check_file_signature( const char *filename,
									 libewf_error_t **error  )
{
  return libewf_check_file_signature( filename, error );
}

int EWFaccess::check_file_signature_wide( const wchar_t *filename,
										  libewf_error_t **error  )
{
  return libewf_check_file_signature_wide( filename, error );
}
#endif
int EWFaccess::glob( const char *filename,
					 size_t length,
					 uint8_t format, char **filenames[] )
{
#if defined( HAVE_V2_API )
  libewf_error_t * error;
  int amount_of_filenames;

  return libewf_glob( filename,
					  length,
					  format,
					  filenames,
					  &amount_of_filenames,
					  &error );
#else
  return libewf_glob( filename, length, format, filenames );
#endif
}

int EWFaccess::glob_wide( const wchar_t *filename,
						  size_t filename_length,
							uint8_t format,
							wchar_t **filenames[] )
{
#if defined( HAVE_V2_API )
  libewf_error_t * error;
  int amount_of_filenames;
  return libewf_glob_wide( filename,
						   filename_length,
						   format,
						   filenames,
						   &amount_of_filenames,
						   &error );
#else
  return libewf_glob_wide( filename, filename_length, format, filenames );
#endif
}

#if defined( HAVE_V2_API )
int EWFaccess::glob( const char *filename,
					 size_t length,
					 uint8_t format,
					 char **filenames[],
					 int *amount_of_filenames,
					 libewf_error_t **error )
{
  return libewf_glob( filename,
					  length,
					  format,
					  filenames,
					  amount_of_filenames,
					  error );
}

int EWFaccess::glob_wide( const wchar_t *filename,
						  size_t filename_length,
						  uint8_t format,
						  wchar_t **filenames[],
						  int *amount_of_filenames,
						  libewf_error_t **error )
{
  return libewf_glob_wide( filename,
						   filename_length,
						   format,
						   filenames,
						   amount_of_filenames,
						   error );
}
#endif

int EWFaccess::signal_abort( void )
{
  return libewf_signal_abort( HANDLER );
}

int EWFaccess::open( char * const filenames[],
					 uint16_t amount_of_files,
					 uint8_t flags )
{
  if( NULL != handler )
	SmartHandler::delRef( handler );

  handler =  SmartHandler::open( libewf_open( filenames,
											  amount_of_files,
											  flags ) );
  if(  NULL == handler )
	 return -1;
  else
	 return 1;
}

int EWFaccess::open_wide( wchar_t * const filenames[],
								int amount_of_filenames,
								uint8_t flags )
{
  if( NULL != handler )
	SmartHandler::delRef( handler );

  handler =  SmartHandler::open( libewf_open_wide( filenames,
												   amount_of_filenames,
												   flags ) );
  if(  NULL == handler )
	 return -1;
  else
	 return 1;
}

off64_t EWFaccess::seek_offset( off64_t offset )
{
  return libewf_seek_offset( HANDLER, offset );
}

off64_t EWFaccess::get_offset( void )
{
  return libewf_get_offset( HANDLER );
}

ssize_t EWFaccess::raw_read_prepare_buffer( void *buffer,
                                            size_t buffer_size,
                                            void *uncompressed_buffer,
                                            size_t *uncompressed_buffer_size,
                                            int8_t is_compressed,
                                            uint32_t chunk_crc,
                                            int8_t read_crc )
{
  return libewf_raw_read_prepare_buffer( HANDLER, buffer, buffer_size, uncompressed_buffer,
                                         uncompressed_buffer_size, is_compressed, chunk_crc, read_crc);
}

ssize_t EWFaccess::raw_read_buffer( void *buffer,
                                    size_t size,
                                    int8_t *is_compressed,
                                    uint32_t *chunk_crc,
                                    int8_t *read_crc )
{
  return libewf_raw_read_buffer( HANDLER,
								 buffer,
								 size,
								 is_compressed,
								 chunk_crc,
								 read_crc );
}

ssize_t EWFaccess::read_buffer( void *buffer,
                                size_t size )
{
  return libewf_read_buffer( HANDLER, buffer, size );
}

ssize_t EWFaccess::read_random( void *buffer,
                                size_t size,
                                off64_t offset )
{
  return libewf_read_random( HANDLER, buffer, size, offset );
}

ssize_t EWFaccess::raw_write_prepare_buffer( void *buffer,
                                             size_t buffer_size,
                                             void *compressed_buffer,
                                             size_t *compressed_buffer_size,
                                             int8_t *is_compressed,
                                             uint32_t *chunk_crc,
                                             int8_t *write_crc )
{
  return libewf_raw_write_prepare_buffer( HANDLER, buffer, buffer_size, 
                                           compressed_buffer, compressed_buffer_size,
                                           is_compressed, chunk_crc, write_crc );
}

ssize_t EWFaccess::raw_write_buffer( void *buffer,
                                     size_t size,
                                     size_t data_size,
                                     int8_t is_compressed,
                                     uint32_t chunk_crc,
                                     int8_t write_crc )
{
  return libewf_raw_write_buffer( HANDLER,
								  buffer,
								  size,
								  data_size,
								  is_compressed,
								  chunk_crc,
								  write_crc );
}

ssize_t EWFaccess::write_buffer( void *buffer,
                                 size_t size )
{
  return libewf_write_buffer( HANDLER, buffer, size );
}

ssize_t EWFaccess::write_random( void *buffer,
								 size_t size,
                                 off64_t offset )
{
  return libewf_write_random( HANDLER, buffer, size, offset );
}

ssize_t EWFaccess::write_finalize( void )
{
  return libewf_write_finalize( HANDLER );
}

int EWFaccess::close(void )
{
  int retCode = SmartHandler::close( handler );
  if( retCode > 0 )
	handler = SmartHandler::delRef( handler );
  return retCode;
}

int EWFaccess::get_sectors_per_chunk( uint32_t *sectors_per_chunk )
{
  return libewf_get_sectors_per_chunk( HANDLER, sectors_per_chunk );
}

int EWFaccess::get_bytes_per_sector( uint32_t *bytes_per_sector )
{
  return libewf_get_bytes_per_sector( HANDLER, bytes_per_sector );
}

int EWFaccess::get_amount_of_sectors( uint32_t *amount_of_sectors )
{
  return libewf_get_amount_of_sectors( HANDLER, amount_of_sectors );
}

int EWFaccess::get_chunk_size( size32_t *chunk_size )
{
  return libewf_get_chunk_size( HANDLER, chunk_size );
}

int EWFaccess::get_error_granularity( uint32_t *error_granularity )
{
  return libewf_get_error_granularity( HANDLER, error_granularity );
}

int EWFaccess::get_compression_values( int8_t *compression_level,
                                       uint8_t *compress_empty_block )
{
  return libewf_get_compression_values( HANDLER, compression_level, compress_empty_block );
}

int EWFaccess::get_media_size( size64_t *media_size )
{
  return libewf_get_media_size( HANDLER, media_size);
}

int EWFaccess::get_media_type( int8_t *media_type )
{
  return libewf_get_media_type( HANDLER, media_type );
}

int EWFaccess::get_media_flags( int8_t *media_flags )
{
  return libewf_get_media_flags( HANDLER, media_flags );
}

int EWFaccess::get_volume_type( int8_t *volume_type )
{
  return libewf_get_volume_type( HANDLER, volume_type );
}

int EWFaccess::get_format( uint8_t *format )
{
  return libewf_get_format( HANDLER, format );
}

int EWFaccess::get_guid( uint8_t *guid, size_t size )
{
  return libewf_get_guid( HANDLER, guid, size );
}

int EWFaccess::get_md5_hash( uint8_t *md5_hash, size_t size )
{
  return libewf_get_md5_hash( HANDLER, md5_hash, size );
}

int EWFaccess::get_segment_filename( char *filename, size_t length )
{
  return libewf_get_segment_filename( HANDLER, filename, length );
}

int EWFaccess::get_segment_filename_wide( wchar_t *filename, size_t filename_size )
{
  return libewf_get_segment_filename_wide( HANDLER, filename, filename_size );
}

int EWFaccess::get_delta_segment_filename( char *filename, size_t length )
{
  return libewf_get_delta_segment_filename( HANDLER, filename, length );
}

int EWFaccess::get_delta_segment_filename_wide( wchar_t *filename, size_t filename_size )
{
  return libewf_get_delta_segment_filename_wide( HANDLER, filename, filename_size );
}

int EWFaccess::get_amount_of_acquiry_errors( uint32_t *amount_of_errors )
{
  return libewf_get_amount_of_acquiry_errors( HANDLER, amount_of_errors );
}

int EWFaccess::get_acquiry_error( uint32_t index,
                                  off64_t *first_sector,
                                  uint32_t *amount_of_sectors )
{
  return libewf_get_acquiry_error( HANDLER, index, first_sector, amount_of_sectors );
}

int EWFaccess::get_amount_of_crc_errors( uint32_t *amount_of_errors )
{
  return libewf_get_amount_of_crc_errors( HANDLER, amount_of_errors );
}

int EWFaccess::get_crc_error( uint32_t index,
                              off64_t *first_sector,
                              uint32_t *amount_of_sectors )
{
  return libewf_get_crc_error( HANDLER, index, first_sector, amount_of_sectors );
}

int EWFaccess::get_amount_of_sessions( uint32_t *amount_of_sessions )
{
  return libewf_get_amount_of_sessions( HANDLER, amount_of_sessions );
}

int EWFaccess::get_session( uint32_t index,
                            off64_t *first_sector,
                            uint32_t *amount_of_sectors )
{
  return libewf_get_session( HANDLER, index, first_sector, amount_of_sectors );
}

int EWFaccess::get_write_amount_of_chunks( uint32_t *amount_of_chunks )
{
  return libewf_get_write_amount_of_chunks( HANDLER, amount_of_chunks );
}

int EWFaccess::get_amount_of_header_values( uint32_t *amount_of_values )
{
  return libewf_get_amount_of_header_values( HANDLER, amount_of_values );
}

int EWFaccess::get_header_value_identifier( uint32_t index,
                                            char *value,
                                            size_t length )
{
  return libewf_get_header_value_identifier( HANDLER, index, value, length );
}

int EWFaccess::get_header_value( char *identifier,
                                 char *value,
                                 size_t length )
{
  return libewf_get_header_value( HANDLER, identifier, value, length );
}

int EWFaccess::get_header_value_case_number( char *value, size_t length )
{
  return libewf_get_header_value( HANDLER, "case_number", value, length );
}

int EWFaccess::get_header_value_description( char *value, size_t length )
{
  return libewf_get_header_value( HANDLER, "description", value, length );
}

int EWFaccess::get_header_value_examiner_name( char *value, size_t length )
{
  return libewf_get_header_value( HANDLER, "examiner_name", value, length );
}

int EWFaccess::get_header_value_evidence_number( char *value, size_t length )
{
  return libewf_get_header_value( HANDLER, "evidence_number", value, length );
}

int EWFaccess::get_header_value_notes( char *value, size_t length )
{
  return libewf_get_header_value( HANDLER, "notes", value, length );
}

int EWFaccess::get_header_value_acquiry_date( char *value, size_t length )
{
  return libewf_get_header_value( HANDLER, "acquiry_date", value, length );
}

int EWFaccess::get_header_value_system_date( char *value, size_t length )
{
  return libewf_get_header_value( HANDLER, "system_date", value, length );
}

int EWFaccess::get_header_value_acquiry_operating_system( char *value,
                                                          size_t length )
{
  return libewf_get_header_value( HANDLER, "acquiry_operating_system", value, length );
}

int EWFaccess::get_header_value_acquiry_software_version( char *value,
                                                          size_t length )
{
  return libewf_get_header_value( HANDLER, "acquiry_software_version", value, length );
}

int EWFaccess::get_header_value_password( char *value,
                                          size_t length )
{
  return libewf_get_header_value( HANDLER, "password", value, length );
}

int EWFaccess::get_header_value_compression_type( char *value,
                                                  size_t length )
{
  return libewf_get_header_value( HANDLER, "compression_type", value, length );
}

int EWFaccess::get_header_value_model( char *value,
                                       size_t length )
{
  return libewf_get_header_value( HANDLER, "model", value, length );
}

int EWFaccess::get_header_value_serial_number( char *value,
                                               size_t length )
{
  return libewf_get_header_value( HANDLER, "serial_number", value, length );
}

int EWFaccess::get_amount_of_hash_values( uint32_t *amount_of_values )
{
  return libewf_get_amount_of_hash_values( HANDLER, amount_of_values );
}

int EWFaccess::get_hash_value_identifier( uint32_t index,
                                          char *value,
                                          size_t length )
{
  return libewf_get_hash_value_identifier( HANDLER, index, value, length );
}

int EWFaccess::get_hash_value( char *identifier,
                               char *value,
                               size_t length )
{
  return libewf_get_hash_value( HANDLER, identifier, value, length );
}

int EWFaccess::get_hash_value_md5( char *value,
                                   size_t length )
{
  return libewf_get_hash_value( HANDLER, "MD5", value, length );
}

int EWFaccess::get_hash_value_sha1( char *value,
                                    size_t length )
{
  return libewf_get_hash_value( HANDLER, "SHA1", value, length );
}

int EWFaccess::set_sectors_per_chunk( uint32_t sectors_per_chunk )
{
  return libewf_set_sectors_per_chunk( HANDLER, sectors_per_chunk );
}

int EWFaccess::set_bytes_per_sector( uint32_t bytes_per_sector )
{
  return libewf_set_bytes_per_sector( HANDLER, bytes_per_sector );
}

int EWFaccess::set_error_granularity( uint32_t error_granularity )
{
  return libewf_set_error_granularity( HANDLER, error_granularity );
}

int EWFaccess::set_compression_values( int8_t compression_level,
                                       uint8_t compress_empty_block )
{
  return libewf_set_compression_values( HANDLER, compression_level, compress_empty_block );
}

int EWFaccess::set_media_size( size64_t media_size )
{
  return libewf_set_media_size( HANDLER, media_size );
}

int EWFaccess::set_segment_file_size( size64_t segment_file_size )
{
  return libewf_set_segment_file_size( HANDLER, segment_file_size );
}

int EWFaccess::set_delta_segment_file_size( size64_t delta_segment_file_size )
{
  return libewf_set_delta_segment_file_size( HANDLER, delta_segment_file_size );
}

int EWFaccess::set_media_type( uint8_t media_type )
{
  return libewf_set_media_type( HANDLER, media_type );
}

int EWFaccess::set_volume_type( uint8_t volume_type )
{
  return libewf_set_volume_type( HANDLER, volume_type );
}

int EWFaccess::set_format( uint8_t format )
{
  return libewf_set_format( HANDLER, format );
}

int EWFaccess::set_guid( uint8_t *guid,
                         size_t size )
{
  return libewf_set_guid( HANDLER, guid, size );
}

int EWFaccess::set_md5_hash( uint8_t *md5_hash,
                             size_t size )
{
  return libewf_set_md5_hash( HANDLER, md5_hash, size );
}

int EWFaccess::set_segment_filename( char *filename,
 			                               size_t length )
{
  return libewf_set_segment_filename( HANDLER, filename, length );
}

int EWFaccess::set_segment_filename_wide( const wchar_t *filename,
                  	                      size_t filename_length )
{
  return libewf_set_segment_filename_wide( HANDLER, filename, filename_length );
}

int EWFaccess::set_delta_segment_filename( char *filename,
										   size_t length )
{
  return libewf_set_delta_segment_filename( HANDLER, filename, length );
}

int EWFaccess::set_read_wipe_chunk_on_error( uint8_t wipe_on_error )
{
  return libewf_set_read_wipe_chunk_on_error( HANDLER, wipe_on_error );
}

int EWFaccess::set_header_value( char *identifier,
                                 char *value,
                                 size_t length )
{
  return libewf_set_header_value( HANDLER, identifier, value, length );
}

int EWFaccess::set_header_value_case_number( char *value, size_t length )
{
  return libewf_set_header_value( HANDLER, "case_number", value, length );
}

int EWFaccess::set_header_value_description( char *value, size_t length )
{
  return libewf_set_header_value( HANDLER, "description", value, length );
}

int EWFaccess::set_header_value_examiner_name( char *value, size_t length )
{
  return libewf_set_header_value( HANDLER, "examiner_name", value, length );
}

int EWFaccess::set_header_value_evidence_number( char *value, size_t length )
{
  return libewf_set_header_value( HANDLER, "evidence_number", value, length );
}

int EWFaccess::set_header_value_notes( char *value, size_t length )
{
  return libewf_set_header_value( HANDLER, "notes", value, length );
}

int EWFaccess::set_header_value_acquiry_date( char *value, size_t length )
{
  return libewf_set_header_value( HANDLER, "acquiry_date", value, length );
}

int EWFaccess::set_header_value_system_date( char *value, size_t length )
{
  return libewf_set_header_value( HANDLER, "system_date", value, length );
}

int EWFaccess::set_header_value_acquiry_operating_system( char *value,
														  size_t length )
{
  return libewf_set_header_value( HANDLER,
								  "acquiry_operating_system",
								  value,
								  length );
}

int EWFaccess::set_header_value_acquiry_software_version( char *value,
														  size_t length )
{
  return libewf_set_header_value( HANDLER,
								  "acquiry_software_version",
								  value,
								  length );
}

int EWFaccess::set_header_value_password( char *value, size_t length )
{
  return libewf_set_header_value( HANDLER, "password", value, length );
}

int EWFaccess::set_header_value_compression_type( char *value, size_t length )
{
  return libewf_set_header_value( HANDLER, "compression_type", value, length );
}

int EWFaccess::set_header_value_model( char *value, size_t length )
{
  return libewf_set_header_value( HANDLER, "model", value, length );
}

int EWFaccess::set_header_value_serial_number( char *value, size_t length )
{
  return libewf_set_header_value( HANDLER, "serial_number", value, length );
}

int EWFaccess::set_hash_value( char *identifier,
							   char *value,
							   size_t length )
{
  return libewf_set_hash_value( HANDLER, identifier, value, length );
}

int EWFaccess::set_hash_value_md5( char *value,
                                   size_t length )
{
  return libewf_set_hash_value( HANDLER, "MD5", value, length );
}

int EWFaccess::set_hash_value_sha1( char *value,
                                    size_t length )
{
  return libewf_set_hash_value( HANDLER, "SHA1", value, length );
}

int EWFaccess::parse_header_values( uint8_t date_format )
{
  return libewf_parse_header_values( HANDLER, date_format );
}

int EWFaccess::parse_hash_values( void )
{
  return libewf_parse_hash_values( HANDLER );
}

int EWFaccess::add_acquiry_error( off64_t first_sector,
                                  uint32_t amount_of_sectors )
{
  return libewf_add_acquiry_error( HANDLER, first_sector, amount_of_sectors );
}

int EWFaccess::add_crc_error( off64_t first_sector,
                              uint32_t amount_of_sectors )
{
  return libewf_add_crc_error( HANDLER, first_sector, amount_of_sectors );
}

int EWFaccess::add_session( off64_t first_sector,
                            uint32_t amount_of_sectors )
{
  return libewf_add_session( HANDLER, first_sector, amount_of_sectors );
}

int EWFaccess::copy_header_values( const EWFaccess& org )
{
  return libewf_copy_header_values( HANDLER,
									SmartHandler::getHandler( org.handler ) );
}

int EWFaccess::copy_media_values( const EWFaccess& org )
{
  return libewf_copy_media_values( HANDLER,
                                   SmartHandler::getHandler( org.handler ) );
}

void EWFaccess::set_notify_values( FILE *stream,
                                   uint8_t verbose )
{
  return libewf_set_notify_values( stream, verbose );
}

#if defined ( HAVE_V2_API )
void EWFaccess::error_free( libewf_error_t **error )
{
	libewf_error_free( error );
}

void EWFaccess::error_fprint( libewf_error_t *error, char *value, size_t length )
{
libewf_error_sprint( error, value, length );
}

void EWFaccess::error_fprint( libewf_error_t *error, FILE* stream )
{
  libewf_error_fprint( error, stream );
}

void EWFaccess::error_backtrace_fprint( libewf_error_t *error,
										FILE *stream )
{
  libewf_error_backtrace_fprint( error, stream );
}

int EWFaccess::handle_initialize( libewf_error_t **error )
{
  if( NULL == handler )
	handler = SmartHandler::makeEmpty();
  return libewf_handle_initialize( SmartHandler::getRefHandler( handler ), error );
}

int EWFaccess::handle_free( libewf_error_t **error )
{
  if( NULL == handler )
	handler = SmartHandler::makeEmpty();
  return libewf_handle_free( SmartHandler::getRefHandler( handler ), error );
}

int EWFaccess::handle_signal_abort( libewf_error_t **error )
{
  return libewf_handle_signal_abort( HANDLER, error );
}

int EWFaccess::handle_open( char * const filenames[],
									int amount_of_filenames,
									uint8_t flags,
									libewf_error_t **error )
{
  if( NULL == handler )
	handler = SmartHandler::makeEmpty();
  else if( 0 == SmartHandler::isFreeHandle( handler ) )
	SmartHandler::delRef( handler );

  return libewf_handle_open( SmartHandler::getHandler( handler ),
							 filenames,
							 amount_of_filenames,
							 flags,
							 error );
}

int EWFaccess::handle_open_wide( wchar_t * const filenames[],
										 int amount_of_filenames,
										 uint8_t flags,
										 libewf_error_t **error )
{
  if( NULL == handler )
	handler = SmartHandler::makeEmpty();
  else if( 0 == SmartHandler::isFreeHandle( handler ) )
	SmartHandler::delRef( handler );

  return libewf_handle_open_wide( SmartHandler::getHandler( handler ),
								  filenames,
								  amount_of_filenames,
								  flags,
							      error );
}

int EWFaccess::handle_close( libewf_error_t **error )
{
  return SmartHandler::close( handler, error );
}

off64_t EWFaccess::handle_seek_offset( off64_t offset,
									   int whence,
									   libewf_error_t **error )
{
  return libewf_handle_seek_offset( HANDLER, offset, whence, error );
}

int EWFaccess::handle_get_offset( off64_t *offset,
				                          libewf_error_t **error )
{
  return libewf_handle_get_offset( HANDLER, offset, error );
}

ssize_t EWFaccess::handle_prepare_read_chunk( void *chunk_buffer,
											  size_t chunk_buffer_size,
											  void *uncompressed_buffer,
											  size_t *uncompressed_buffer_size,
											  int8_t is_compressed,
											  uint32_t chunk_crc,
											  int8_t read_crc,
					                          libewf_error_t **error )
{
  return libewf_handle_prepare_read_chunk(HANDLER, 
                                          chunk_buffer, 
                                          chunk_buffer_size, 
                                          uncompressed_buffer, 
                                          uncompressed_buffer_size, 
                                          is_compressed, 
                                          chunk_crc, 
                                          read_crc, 
                                          error );
}

ssize_t EWFaccess::handle_read_chunk( void *chunk_buffer,
					                            size_t chunk_buffer_size,
					                            int8_t *is_compressed,
					                            void *crc_buffer,
					                            uint32_t *chunk_crc,
					                            int8_t *read_crc,
					                            libewf_error_t **error )
{
  return libewf_handle_read_chunk( HANDLER, 
                                   chunk_buffer, 
                                   chunk_buffer_size, 
                                   is_compressed, 
                                   crc_buffer, 
                                   chunk_crc, 
                                   read_crc, 
                                   error );
}

ssize_t EWFaccess::handle_read_buffer( void *buffer,
					                             size_t buffer_size,
					                             libewf_error_t **error )
{
  return libewf_handle_read_buffer( HANDLER, 
                                    buffer, 
                                    buffer_size, 
                                    error );
}

ssize_t EWFaccess::handle_read_random( void *buffer,
					                             size_t buffer_size,
					                             off64_t offset,
					                             libewf_error_t **error )
{
  return libewf_handle_read_random( HANDLER,
                                    buffer, 
                                    buffer_size, 
                                    offset, 
                                    error );
}

ssize_t EWFaccess::handle_prepare_write_chunk( void *buffer,
					                                     size_t buffer_size,
					                                     void *compressed_buffer,
					                                     size_t *compressed_buffer_size,
					                                     int8_t *is_compressed,
					                                     uint32_t *chunk_crc,
					                                     int8_t *write_crc,
					                                     libewf_error_t **error )
{
  return libewf_handle_prepare_write_chunk( HANDLER,
					                                  buffer, 
                                            buffer_size, 
                                            compressed_buffer, 
                                            compressed_buffer_size, 
                                            is_compressed, 
                                            chunk_crc, 
                                            write_crc, 
                                            error );
}

ssize_t EWFaccess::handle_write_chunk( void *chunk_buffer,
					                             size_t chunk_buffer_size,
					                             size_t data_size,
					                             int8_t is_compressed,
					                             void *crc_buffer,
					                             uint32_t chunk_crc,
					                             int8_t write_crc,
					                             libewf_error_t **error )
{
  return libewf_handle_write_chunk( HANDLER,
					                          chunk_buffer,
					                          chunk_buffer_size,
					                          data_size,
					                          is_compressed,
					                          crc_buffer,
					                          chunk_crc,
					                          write_crc,
					                          error );
}

 ssize_t EWFaccess::handle_write_buffer( void *buffer,
					                               size_t buffer_size,
					                               libewf_error_t **error )
 {
   return libewf_handle_write_buffer( HANDLER,
					                            buffer,
					                            buffer_size,
					                            error );
 }

 ssize_t EWFaccess::handle_write_random( void *buffer,
					                               size_t buffer_size,
					                               off64_t offset,
					                               libewf_error_t **error )
 {
   return libewf_handle_write_random( HANDLER,
					                            buffer,
					                            buffer_size,
					                            offset,
					                            error );
 }

 ssize_t EWFaccess::handle_write_finalize( libewf_error_t **error )
 {
   return libewf_handle_write_finalize( HANDLER, error );
 }

 int EWFaccess::handle_get_segment_filename_size( size_t *filename_size,
				                                          libewf_error_t **error )
 {
   return libewf_handle_get_segment_filename_size( HANDLER,
				                                           filename_size,
				                                           error );
 }

 int EWFaccess::handle_get_segment_filename( char *filename,
				                                     size_t filename_size,
				                                     libewf_error_t **error )
 {
   return libewf_handle_get_segment_filename( HANDLER,
				                                      filename,
				                                      filename_size,
				                                      error );
 }

 int EWFaccess::handle_set_segment_filename( const char *filename,
				                                     size_t filename_length,
				                                     libewf_error_t **error )
 {
   return libewf_handle_set_segment_filename( HANDLER,
				                                      filename,
				                                      filename_length,
				                                      error );
 }

 int EWFaccess::handle_get_segment_filename_size_wide( size_t *filename_size,
				                                               libewf_error_t **error )
 {
   return libewf_handle_get_segment_filename_size_wide( HANDLER,
				                                                filename_size,
				                                                error );
 }

 int EWFaccess::handle_get_segment_filename_wide( wchar_t *filename,
				                                          size_t filename_size,
				                                          libewf_error_t **error )
 {
   return libewf_handle_get_segment_filename_wide( HANDLER,
				                                           filename,
				                                           filename_size,
				                                           error );
 }

 int EWFaccess::handle_set_segment_filename_wide( const wchar_t *filename,
				                                          size_t filename_length,
				                                          libewf_error_t **error )
 {
   return libewf_handle_set_segment_filename_wide( HANDLER,
				                                           filename,
				                                           filename_length,
				                                           error );
 }

 int EWFaccess::handle_get_segment_file_size( size64_t *segment_file_size,
				                                      libewf_error_t **error )
 {
   return libewf_handle_get_segment_file_size( HANDLER,
				                                       segment_file_size,
				                                       error );
 }

 int EWFaccess::handle_set_segment_file_size( size64_t segment_file_size,
				                                      libewf_error_t **error )
 {
   return libewf_handle_set_segment_file_size( HANDLER,
				                                       segment_file_size,
				                                       error );
 }

 int EWFaccess::handle_get_delta_segment_filename_size( size_t *filename_size,
				                                                libewf_error_t **error )
 {
   return libewf_handle_get_delta_segment_filename_size( HANDLER,
				                                                 filename_size,
				                                                 error );
 }

 int EWFaccess::handle_get_delta_segment_filename( char *filename,
				                                           size_t filename_size,
				                                           libewf_error_t **error )
 {
   return libewf_handle_get_delta_segment_filename( HANDLER,
				                                            filename,
				                                            filename_size,
				                                            error );
 }

 int EWFaccess::handle_set_delta_segment_filename( const char *filename,
				                                           size_t filename_length,
				                                           libewf_error_t **error )
 {
   return libewf_handle_set_delta_segment_filename( HANDLER,
				                                            filename,
				                                            filename_length,
				                                            error );
 }

 int EWFaccess::handle_get_delta_segment_filename_size_wide( size_t *filename_size,
				                                                     libewf_error_t **error )
 {
   return libewf_handle_get_delta_segment_filename_size_wide( HANDLER,
				                                                      filename_size,
				                                                      error );
 }

 int EWFaccess::handle_get_delta_segment_filename_wide( wchar_t *filename,
				                                                size_t filename_size,
				                                                libewf_error_t **error )
 {
   return libewf_handle_get_delta_segment_filename_wide( HANDLER,
				                                                 filename,
				                                                 filename_size,
				                                                 error );
 }

 int EWFaccess::handle_set_delta_segment_filename_wide( const wchar_t *filename,
				                                                 size_t filename_length,
				                                                 libewf_error_t **error )
 {
   return libewf_handle_set_delta_segment_filename_wide( HANDLER,
				                                                 filename,
				                                                 filename_length,
				                                                 error );
 }

 int EWFaccess::handle_get_delta_segment_file_size( size64_t *delta_segment_file_size,
				                                            libewf_error_t **error )
 {
   return libewf_handle_get_delta_segment_file_size( HANDLER,
				                                             delta_segment_file_size,
				                                             error );
 }

 int EWFaccess::handle_set_delta_segment_file_size( size64_t delta_segment_file_size,
				                                            libewf_error_t **error )
 {
   return libewf_handle_set_delta_segment_file_size( HANDLER, 
                                                     delta_segment_file_size,
				                                             error );
 }

 int EWFaccess::handle_get_filename_size( size_t *filename_size,
				                                   libewf_error_t **error )
 {
   return libewf_handle_get_filename_size( HANDLER, filename_size, error );
 }

 int EWFaccess::handle_get_filename( char *filename,
				                             size_t filename_size,
				                             libewf_error_t **error )
 {
   return libewf_handle_get_filename( HANDLER, filename, filename_size, error );
 }

 int EWFaccess::handle_get_filename_size_wide( size_t *filename_size,
				                                       libewf_error_t **error )
 {
   return libewf_handle_get_filename_size_wide( HANDLER, filename_size, error );
 }

 int EWFaccess::handle_get_filename_wide( wchar_t *filename,
				                                  size_t filename_size,
				                                  libewf_error_t **error )
 {
   return libewf_handle_get_filename_wide( HANDLER, filename, filename_size, error );
 }
#endif
