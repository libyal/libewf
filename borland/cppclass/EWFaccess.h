/**
 ******************************************************************************
 *
 *   Copyright (C) 2009, .... All Rights Reserved.
 *
 ******************************************************************************
 *
 * @file EWFaccess.h
 *
 * @brief Definition class for access to EWF files (images).
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

#if !defined( _EWF_ACCESS_H )
#define _EWF_ACCESS_H

#include <common.h>
#include <libewf/codepage.h>
#include <libewf/definitions.h>
#include <libewf/error.h>
#include <libewf/extern.h>
#include <libewf/features.h>
#include <libewf/types.h>
#include <libewf/handle.h>

#include <stdio.h>

#if defined( __BORLANDC__ )
 class _export EWFaccess;
#endif


class EWFaccess
{
public:
  /**
   * @brief Constructor.
   */
  EWFaccess( void );

  /**
   * @brief Destructor.
   */
  ~EWFaccess();

  /**
   * @brief Additional constructor.
   */
  EWFaccess( const EWFaccess& );

  /**
   * @brief Copy operator.
   */
  EWFaccess& operator = (const EWFaccess& );

  /********************************************************************************/

  /**
   * @brief Returns the library version.
   */
  const char *get_version( void );

  /**
   * @brief Returns the flags for reading.
   */
  uint8_t get_flags_read( void );

  /**
   * @brief Returns the flags for reading and writing.
   */
  uint8_t get_flags_read_write( void );

  /**
   * @brief Returns the flags for writing.
   */
  uint8_t get_flags_write( void );

  /**
   * @brief Detects if a file is an EWF file (check for the EWF file signature).
   * @return 1 if true, 0 if not or -1 on error
   */
  int check_file_signature( const char *filename );
  int check_file_signature_wide( const wchar_t *filename );
#if defined( HAVE_V2_API )
  int check_file_signature( const char *filename,
							libewf_error_t **error );
  int check_file_signature_wide( const wchar_t *filename,
								 libewf_error_t **error );
#endif
  /**
   * @brief Globs the segment files according to the EWF naming schema.
   * if the format is known the filename should contain the base of the filename
   * otherwise the function will try to determine the format based on the extension
   * @return the amount of filenames if successful or -1 on error
   */
  int glob( const char *filename,
			size_t length,
			uint8_t format,
			char **filenames[] );
  int glob_wide( const wchar_t *filename,
				 size_t filename_length,
				 uint8_t format,
				 wchar_t **filenames[] );
#if defined( HAVE_V2_API )
  int glob( const char *filename,
			size_t length,
			uint8_t format,
			char **filenames[],
			int *amount_of_filenames,
			libewf_error_t **error );
  int glob_wide( const wchar_t *filename,
				 size_t filename_length,
				 uint8_t format,
				 wchar_t **filenames[],
				 int *amount_of_filenames,
				 libewf_error_t **error );
#endif

  /**
   * @brief Signals the libewf handle to abort its current activity.
   * @return 1 if successful or -1 on error
   */
  int signal_abort( void );

  /**
   * @brief Opens a set of EWF file(s).
   * For reading files should contain all filenames that make up an EWF image
   * For writing files should contain the base of the filename, extentions like .e01 will be automatically added
   * @return 1 if successful or -1 on error
   */
  int open( char * const filenames[],
            uint16_t amount_of_files,
            uint8_t flags );
  int open_wide( wchar_t * const filenames[],
				 int amount_of_filenames,
				 uint8_t flags );

  /**
   * @brief Seeks a certain offset of the media data within the EWF file(s).
   * It will set the related file offset to the specific chunk offset
   * @return the offset if seek is successful or -1 on error
   */
  off64_t seek_offset( off64_t offset );
  /**
  * @brief Retrieves the current offset of the media data within the EWF file(s)
  * @return the offset if successful or -1 on error
  */
  off64_t get_offset( void );

  /**
   * @brief Prepares a buffer with chunk data after reading it according to the handle settings.
   * intended for raw read
   * The buffer size cannot be larger than the chunk size
   * @return the resulting chunk size or -1 on error
   */
  ssize_t raw_read_prepare_buffer( void *buffer,
                                   size_t buffer_size,
                                   void *uncompressed_buffer,
                                   size_t *uncompressed_buffer_size,
                                   int8_t is_compressed,
                                   uint32_t chunk_crc,
                                   int8_t read_crc );

  /**
   * @brief Reads 'raw' data from the curent offset into a buffer.
   * size contains the size of the buffer
   * The function sets the chunk crc, is compressed and read crc values
   * @return the amount of bytes read or -1 on error
   */
  ssize_t raw_read_buffer( void *buffer,
                           size_t size,
                           int8_t *is_compressed,
                           uint32_t *chunk_crc,
                           int8_t *read_crc );

  /**
   * @brief Reads data from the curent offset into a buffer.
   * @return the amount of bytes read or -1 on error
   */
  ssize_t read_buffer( void *buffer,
                       size_t size );

  /**
   * @brief Reads media data from an offset into a buffer.
   * @return the amount of bytes read or -1 on error
   */
  ssize_t read_random( void *buffer,
                       size_t size,
                       off64_t offset );

  /**
   * @brief Prepares a buffer with chunk data before writing according to the handle settings.
   * intended for raw write
   * The buffer size cannot be larger than the chunk size
   * The function sets the chunk crc, is compressed and write crc values
   * @return the resulting chunk size or -1 on error
   */
  ssize_t raw_write_prepare_buffer( void *buffer,
                                    size_t buffer_size,
                                    void *compressed_buffer,
                                    size_t *compressed_buffer_size,
                                    int8_t *is_compressed,
                                    uint32_t *chunk_crc,
                                    int8_t *write_crc );

  /**
   * @brief Writes 'raw' data in EWF format from a buffer at the current offset.
   * the necessary settings of the write values must have been made
   * size contains the size of the data within the buffer while
   * data size contains the size of the actual input data
   * Will initialize write if necessary
   * @return the amount of input bytes written, 0 when no longer bytes can be written or -1 on error
   */
  ssize_t raw_write_buffer( void *buffer,
                            size_t size,
                            size_t data_size,
                            int8_t is_compressed,
                            uint32_t chunk_crc,
                            int8_t write_crc );

  /**
   * @brief Writes data in EWF format from a buffer at the current offset.
   * the necessary settings of the write values must have been made
   * Will initialize write if necessary
   * @return the amount of input bytes written, 0 when no longer bytes can be written or -1 on error
   */
  ssize_t write_buffer( void *buffer,
                        size_t size );

  /**
   * @brief Writes data in EWF format from a buffer at an specific offset,.
   * the necessary settings of the write values must have been made
   * Will initialize write if necessary
   * @return the amount of input bytes written, 0 when no longer bytes can be written or -1 on error
   */
  ssize_t write_random( void *buffer,
                        size_t size,
                        off64_t offset );

  /**
   * @brief Finalizes the write by correcting the EWF the meta data in the segment files.
   * This function is required after writing from stream
   * @return the amount of input bytes written or -1 on error
   */
  ssize_t write_finalize( void );

  /**
   * @brief Closes the EWF handle and frees memory used within the handle.
   * @return 0 if successful or -1 on error
   */
  int close(void );

  /**
   * @brief Retrieves the amount of sectors per chunk from the media information.
   * @return 1 if successful or -1 on error
   */
  int get_sectors_per_chunk( uint32_t *sectors_per_chunk );

  /**
   * @brief Retrieves the amount of bytes per sector from the media information.
   * @return 1 if successful or -1 on error
   */
  int get_bytes_per_sector( uint32_t *bytes_per_sector );

  /**
   * @brief Retrieves the amount of sectors from the media information.
   * @return 1 if successful or -1 on error
   */
  int get_amount_of_sectors( uint32_t *amount_of_sectors );

  /**
   * @brief Retrieves the chunk size from the media information.
   * @return 1 if successful or -1 on error
   */
  int get_chunk_size( size32_t *chunk_size );

  /**
   * @brief Retrieves the error granularity from the media information.
   * @return 1 if successful or -1 on error
   */
  int get_error_granularity( uint32_t *error_granularity );

  /**
   * @brief Retrieves the compression values.
   * @return 1 if successful or -1 on error
   */
  int get_compression_values( int8_t *compression_level,
                              uint8_t *compress_empty_block );

  /**
   * @brief Retrieves the size of the contained media data.
   * @return 1 if successful or -1 on error
   */
  int get_media_size( size64_t *media_size );

  /**
   * @brief Retrieves the media type value.
   * @return 1 if successful or -1 on error
   */
  int get_media_type( int8_t *media_type );

  /**
   * @brief Retrieves the media flags.
   * @return 1 if successful or -1 on error
   */
  int get_media_flags( int8_t *media_flags );

  /**
   * @brief Retrieves the volume type value.
   * @return 1 if successful or -1 on error
   */
  int get_volume_type( int8_t *volume_type );

  /**
   * @brief Retrieves the format type value.
   * @return 1 if successful or -1 on error
   */
  int get_format( uint8_t *format );

  /**
   * @brief Retrieves the GUID.
   * @return 1 if successful or -1 on error
   */
  int get_guid( uint8_t *guid, size_t size );

  /**
   * @brief Retrieves the MD5 hash.
   * @return 1 if successful, 0 if value not present or -1 on error
   */
  int get_md5_hash( uint8_t *md5_hash, size_t size );

  /**
   * @brief Retrieves the segment filename.
   * @return 1 if successful, 0 if value not present or -1 on error
   */
  int get_segment_filename( char *filename, size_t length );
  int get_segment_filename_wide( wchar_t *filename, size_t filename_size );

  /**
   * @brief Retrieves the delta segment filename.
   * @return 1 if successful, 0 if value not present or -1 on error
   */
  int get_delta_segment_filename( char *filename, size_t length );
  int get_delta_segment_filename_wide( wchar_t *filename, size_t filename_size );

  /**
   * @brief Retrieves the amount of acquiry errors.
   * @return 1 if successful, 0 if no acquiry errors are present or -1 on error
   */
  int get_amount_of_acquiry_errors( uint32_t *amount_of_errors );

  /**
   * @brief Retrieves the information of an acquiry error.
   * @return 1 if successful, 0 if no acquiry error could be found or -1 on error
   */
  int get_acquiry_error( uint32_t index,
                         off64_t *first_sector,
                         uint32_t *amount_of_sectors );

  /**
   * @brief Retrieves the amount of CRC errors.
   * @return 1 if successful, 0 if no CRC errors are present or -1 on error
   */
  int get_amount_of_crc_errors( uint32_t *amount_of_errors );

  /**
   * @brief Retrieves the information of a CRC error.
   * @return 1 if successful, 0 if no CRC error could be found or -1 on error
   */
  int get_crc_error( uint32_t index,
                     off64_t *first_sector,
                     uint32_t *amount_of_sectors );

  /**
   * @brief Retrieves the amount of sessions.
   * @return 1 if successful, 0 if no sessions are present or -1 on error
   */
  int get_amount_of_sessions( uint32_t *amount_of_sessions );

  /**
   * @brief Retrieves the information of a session.
   * @return 1 if successful, 0 if no sessions could be found or -1 on error
   */
  int get_session( uint32_t index,
                   off64_t *first_sector,
                   uint32_t *amount_of_sectors );

  /**
   * @brief Retrieves the amount of chunks written.
   * @return 1 if successful or -1 on error
   */
  int get_write_amount_of_chunks( uint32_t *amount_of_chunks );

  /**
   * @brief Retrieves the amount of header values.
   * @return 1 if successful, 0 if no header values are present or -1 on error
   */
  int get_amount_of_header_values( uint32_t *amount_of_values );

  /**
   * @brief Retrieves the header value identifier specified by its index.
   * @return 1 if successful, 0 if value not present or -1 on error
   */
  int get_header_value_identifier( uint32_t index,
                                   char *value,
                                   size_t length );

  /**
   * @brief Retrieves the header value specified by the identifier.
   * @return 1 if successful, 0 if value not present or -1 on error
   */
  int get_header_value( char *identifier,
                        char *value,
                        size_t length );

  /**
   * @brief Retrieves the header value case number.
   * @return 1 if successful, 0 if value not present or -1 on error
   */
  int get_header_value_case_number( char *value, size_t length );

  /**
   * @brief Retrieves the header value description.
   * @return 1 if successful, 0 if value not present or -1 on error
   */
  int get_header_value_description( char *value, size_t length );

  /**
   * @brief Retrieves the header value examiner name.
   * @return 1 if successful, 0 if value not present or -1 on error
   */
  int get_header_value_examiner_name( char *value, size_t length );

  /**
   * @brief Retrieves the header value evidence number.
   * @return 1 if successful, 0 if value not present or -1 on error
   */
  int get_header_value_evidence_number( char *value, size_t length );
  /**
   * @brief Retrieves the header value notes.
   * @return 1 if successful, 0 if value not present or -1 on error
   */
  int get_header_value_notes( char *value, size_t length );

  /**
   * @brief Retrieves the header value acquiry date.
   * @return 1 if successful, 0 if value not present or -1 on error
   */
  int get_header_value_acquiry_date( char *value, size_t length );

  /**
   * @brief Retrieves the header value system date.
   * @return 1 if successful, 0 if value not present or -1 on error
   */
  int get_header_value_system_date( char *value, size_t length );

  /**
   * @brief Retrieves the header value acquiry operating system.
   * @return 1 if successful, 0 if value not present or -1 on error
   */
  int get_header_value_acquiry_operating_system( char *value,
                                                 size_t length );

  /**
   * @brief Retrieves the header value acquiry software version.
   * @return 1 if successful, 0 if value not present or -1 on error
   */
  int get_header_value_acquiry_software_version( char *value,
                                                 size_t length );

  /**
   * @brief Retrieves the header value password.
   * @return 1 if successful, 0 if value not present or -1 on error
   */
  int get_header_value_password( char *value,
                                 size_t length );

  /**
   * @brief Retrieves the header value compression type.
   * @return 1 if successful, 0 if value not present or -1 on error
   */
  int get_header_value_compression_type( char *value,
                                         size_t length );

  /**
   * @brief Retrieves the header value model.
   * @return 1 if successful, 0 if value not present or -1 on error
   */
  int get_header_value_model( char *value,
                              size_t length );

  /**
   * @brief Retrieves the header value serial number.
   * @return 1 if successful, 0 if value not present or -1 on error
   */
  int get_header_value_serial_number( char *value,
                                      size_t length );

  /**
   * @brief Retrieves the amount of hash values.
   * @return 1 if successful, 0 if no hash values are present or -1 on error
   */
  int get_amount_of_hash_values( uint32_t *amount_of_values );

  /**
   * @brief Retrieves the hash value identifier specified by its index.
   * @return 1 if successful, 0 if value not present or -1 on error
   */
  int get_hash_value_identifier( uint32_t index,
                                 char *value,
                                 size_t length );

  /**
   * @brief Retrieves the hash value specified by the identifier.
   * @return 1 if successful, 0 if value not present or -1 on error
   */
  int get_hash_value( char *identifier,
                      char *value,
                      size_t length );

  /**
   * @brief Retrieves the hash value MD5.
   * @return 1 if successful, 0 if value not present or -1 on error
   */
  int get_hash_value_md5( char *value,
                          size_t length );

  /**
   * @brief Retrieves the hash value SHA1.
   * @return 1 if successful, 0 if value not present or -1 on error
   */
  int get_hash_value_sha1( char *value,
                           size_t length );

  /**
   * @brief Sets the amount of sectors per chunk in the media information.
   * @return 1 if successful or -1 on error
   */
  int set_sectors_per_chunk( uint32_t sectors_per_chunk );

  /**
   * @brief Sets the amount of bytes per sector in the media information.
   * @return 1 if successful or -1 on error
   */
  int set_bytes_per_sector( uint32_t bytes_per_sector );

  /**
   * @brief Sets the error granularity.
   * @return 1 if successful or -1 on error
   */
  int set_error_granularity( uint32_t error_granularity );

  /**
   * @brief Sets the compression values.
   * @return 1 if successful or -1 on error
   */
  int set_compression_values( int8_t compression_level,
                              uint8_t compress_empty_block );

  /**
   * @brief Sets the media size.
   * @return 1 if successful or -1 on error
   */
  int set_media_size( size64_t media_size );

  /**
   * @brief Sets the segment file size.
   * @return 1 if successful or -1 on error
   */
  int set_segment_file_size( size64_t segment_file_size );

  /**
   * @brief Sets the delta segment file size.
   * @return 1 if successful or -1 on error
   */
  int set_delta_segment_file_size( size64_t delta_segment_file_size );

  /**
   * @brief Sets the media type.
   * @return 1 if successful or -1 on error
   */
  int set_media_type( uint8_t media_type );

  /**
   * @brief Sets the volume type.
   * @return 1 if successful or -1 on error
   */
  int set_volume_type( uint8_t volume_type );

  /**
   * @brief Sets the output format.
   * @return 1 if successful or -1 on error
   */
  int set_format( uint8_t format );

  /**
   * @brief Sets the GUID.
   * @return 1 if successful or -1 on error
   */
  int set_guid( uint8_t *guid,
                size_t size );

  /**
   * @brief Sets the MD5 hash.
   * @return 1 if successful, 0 if value not present or -1 on error
   */
  int set_md5_hash( uint8_t *md5_hash,
                    size_t size );

  /**
   * @brief Sets the segment filename.
   * @return 1 if successful or -1 on error
   */
  int set_segment_filename( char *filename,
							size_t length );
  int set_segment_filename_wide( const wchar_t *filename,
                  			     size_t filename_length );

  /**
   * @brief Sets the delta segment filename.
   * @return 1 if successful or -1 on error
   */
  int set_delta_segment_filename( char *filename,
                                  size_t length );

  /**
   * @brief Sets the read wipe chunk on error.
   * The chunk is not wiped if read raw is used
   * @return 1 if successful or -1 on error
   */
  int set_read_wipe_chunk_on_error( uint8_t wipe_on_error );

  /**
   * @brief Sets the header value specified by the identifier.
   * @return 1 if successful or -1 on error
   */
  int set_header_value( char *identifier,
                        char *value,
                        size_t length );

  /**
   * @brief Sets the header value case number.
   * @return 1 if successful or -1 on error
   */
  int set_header_value_case_number( char *value, size_t length );

  /**
   * @brief Sets the header value description.
   * @return 1 if successful or -1 on error
   */
  int set_header_value_description( char *value, size_t length );

  /**
   * @brief Sets the header value examiner name.
   * @return 1 if successful or -1 on error
   */
  int set_header_value_examiner_name( char *value, size_t length );

  /**
   * @brief Sets the header value evidence number.
   * @return 1 if successful or -1 on error
   */
  int set_header_value_evidence_number( char *value, size_t length );

  /**
   * @brief Sets the header value notes.
   * @return 1 if successful or -1 on error
   */
  int set_header_value_notes( char *value, size_t length );

  /**
   * @brief Sets the header value acquiry date.
   * @return 1 if successful or -1 on error
   */
  int set_header_value_acquiry_date( char *value, size_t length );

  /**
   * @brief Sets the header value system date.
   * @return 1 if successful or -1 on error
   */
  int set_header_value_system_date( char *value, size_t length );

  /**
   * @brief Sets the header value acquiry operating system.
   * @return 1 if successful or -1 on error
   */
  int set_header_value_acquiry_operating_system( char *value, size_t length );

  /**
   * @brief Sets the header value acquiry software version.
   * @return 1 if successful or -1 on error
   */
  int set_header_value_acquiry_software_version( char *value, size_t length );

  /**
   * @brief Sets the header value password.
   * @return 1 if successful or -1 on error
   */
  int set_header_value_password( char *value, size_t length );

  /**
   * @brief Sets the header value compression type.
   * @return 1 if successful or -1 on error
   */
  int set_header_value_compression_type( char *value, size_t length );

  /**
   * @brief Sets the header value model.
   * @return 1 if successful or -1 on error
   */
  int set_header_value_model( char *value, size_t length );

  /**
   * @brief Sets the header value serial number.
   * @return 1 if successful or -1 on error
   */
  int set_header_value_serial_number( char *value, size_t length );

  /**
   * @brief Sets the hash value specified by the identifier.
   * @return 1 if successful or -1 on error
   */
  int set_hash_value( char *identifier,
                      char *value,
                      size_t length );

  /**
   * @brief Sets the hash value MD5.
   * @return 1 if successful or -1 on error
   */
  int set_hash_value_md5( char *value,
                          size_t length );

  /**
   * @brief Sets the hash value SHA1.
   * @return 1 if successful or -1 on error
   */
  int set_hash_value_sha1( char *value,
                           size_t length );

  /**
   * @brief Parses the header values from the xheader, header2 or header section.
   * Will parse the first available header in order mentioned above
   * @return 1 if successful, 0 if already parsed or -1 on error
   */
  int parse_header_values( uint8_t date_format );

  /**
   * @brief Parses the hash values from the xhash section.
   * @return 1 if successful, 0 if already parsed or -1 on error
   */
  int parse_hash_values(void );

  /**
   * @brief Add an acquiry error.
   * @return 1 if successful or -1 on error
   */
  int add_acquiry_error( off64_t first_sector,
                         uint32_t amount_of_sectors );

  /**
   * @brief Add a CRC error.
   * @return 1 if successful or -1 on error
   */
  int add_crc_error( off64_t first_sector,
                     uint32_t amount_of_sectors );

  /**
   * @brief Add a session.
   * @return 1 if successful or -1 on error
   */
  int add_session( off64_t first_sector,
                   uint32_t amount_of_sectors );

  /**
   * @brief Copies the header values from sample.
   * @return 1 if successful or -1 on error
   */
  int copy_header_values( const EWFaccess&);

  /**
   * @brief Copies the media values from the sample.
   * @return 1 if successful or -1 on error
   */
  int copy_media_values( const EWFaccess&);

  /**
   * @brief Set the notify values.
   */
  void set_notify_values( FILE *stream,
                          uint8_t verbose );

  /********************* API V2 ********************************************/
#if defined ( HAVE_V2_API )
  /**
   * @brief  Frees the error including elements
   */
  void error_free( libewf_error_t **error );

  /**
   * @brief  Prints a descriptive string of the error to the stream
   */
  void error_fprint( libewf_error_t *error, FILE * stream );
  void error_fprint( libewf_error_t *error, char *value, size_t length );

  /**
   * @brief  Prints a backtrace of the error to the stream
   */
  void error_backtrace_fprint( libewf_error_t *error,
							   FILE *stream );

  /**
   * @brief Initialize the handle.
   * Returns 1 if successful or -1 on error
   */
  int handle_initialize( libewf_error_t **error );

  /**
   * @brief Frees the handle including elements.
   * Returns 1 if successful or -1 on error
   */
  int handle_free( libewf_error_t **error );

  /**
   * @brief Signals the libewf handle to abort its current activity.
   * Returns 1 if successful or -1 on error
   */
  int handle_signal_abort( libewf_error_t **error );

  /**
   * @brief Opens a set of EWF file(s).
   * For reading files should contain all filenames that make up an EWF image
   * For writing files should contain the base of the filename, extentions like .e01 will be automatically added
   * Returns a pointer to the new instance of handle, NULL on error
   */
  int handle_open( char * const filenames[],
                   int amount_of_filenames,
                   uint8_t flags,
				   libewf_error_t **error );

  /**
   * @brief Opens a set of EWF file(s).
   * For reading files should contain all filenames that make up an EWF image
   * For writing files should contain the base of the filename, extentions like .e01 will be automatically added
   * Returns a pointer to the new instance of handle, NULL on error
   */
  int handle_open_wide( wchar_t * const filenames[],
                        int amount_of_filenames,
                        uint8_t flags,
						libewf_error_t **error );

  /**
   * @brief Closes the EWF handle and frees memory used within the handle.
   * Returns 0 if successful or -1 on error
   */
  int handle_close( libewf_error_t **error );

  /**
   * @brief Seeks a certain offset of the media data within the EWF file(s).
   * It will set the related file offset to the specific chunk offset
   * Returns the offset if seek is successful or -1 on error
   */
  off64_t handle_seek_offset( off64_t offset,
                              int whence,
                              libewf_error_t **error );

  /**
   * @brief Retrieves the current offset of the media data within the EWF file(s).
   * Returns the offset if successful or -1 on error
   */
  int handle_get_offset( off64_t *offset,
                         libewf_error_t **error );

  /**
   * @brief Prepares chunk data after reading it according to the handle settings.
   * This function should be used after handle_read_chunk
   * The chunk buffer size should contain the actual chunk size
   * Returns the resulting chunk size or -1 on error
   */
  ssize_t handle_prepare_read_chunk( void *chunk_buffer,
                                     size_t chunk_buffer_size,
                                     void *uncompressed_buffer,
                                     size_t *uncompressed_buffer_size,
                                     int8_t is_compressed,
                                     uint32_t chunk_crc,
                                     int8_t read_crc,
                                     libewf_error_t **error );

  /**
   * @brief Reads a chunk from the curent offset into a buffer.
   * size contains the size of the chunk buffer
   * The function sets the chunk crc, is compressed and read crc values
   * Returns the amount of bytes read or -1 on error
   */
  ssize_t handle_read_chunk( void *chunk_buffer,
                             size_t chunk_buffer_size,
                             int8_t *is_compressed,
                             void *crc_buffer,
                             uint32_t *chunk_crc,
                             int8_t *read_crc,
                             libewf_error_t **error );

  /**
   * @brief Reads data from the curent offset into a buffer.
   * Returns the amount of bytes read or -1 on error
   */
  ssize_t handle_read_buffer( void *buffer,
                              size_t buffer_size,
                              libewf_error_t **error );

  /**
   * @brief Reads media data from an offset into a buffer.
   * Returns the amount of bytes read or -1 on error
   */
  ssize_t handle_read_random( void *buffer,
                              size_t buffer_size,
                              off64_t offset,
                              libewf_error_t **error );

  /**
   * @brief Prepares a buffer with chunk data before writing according to the handle settings.
   * This function should be used before handle_write_chunk
   * The chunk buffer size should contain the actual chunk size
   * The function sets the chunk crc, is compressed and write crc values
   * Returns the resulting chunk size or -1 on error
   */
  ssize_t handle_prepare_write_chunk( void *buffer,
                                      size_t buffer_size,
                                      void *compressed_buffer,
                                      size_t *compressed_buffer_size,
                                      int8_t *is_compressed,
                                      uint32_t *chunk_crc,
                                      int8_t *write_crc,
                                      libewf_error_t **error );

  /**
   * @brief Writes a chunk in EWF format from a buffer at the current offset.
   * the necessary settings of the write values must have been made
   * size contains the size of the data within the buffer while
   * data size contains the size of the actual input data
   * Will initialize write if necessary
   * Returns the amount of input bytes written, 0 when no longer bytes can be written or -1 on error
   */
  ssize_t handle_write_chunk( void *chunk_buffer,
                              size_t chunk_buffer_size,
                              size_t data_size,
                              int8_t is_compressed,
                              void *crc_buffer,
                              uint32_t chunk_crc,
                              int8_t write_crc,
                              libewf_error_t **error );

  /**
   * @brief Writes data in EWF format from a buffer at the current offset.
   * the necessary settings of the write values must have been made
   * Will initialize write if necessary
   * Returns the amount of input bytes written, 0 when no longer bytes can be written or -1 on error
   */
  ssize_t handle_write_buffer( void *buffer,
                               size_t buffer_size,
                               libewf_error_t **error );

  /**
   * @brief Writes data in EWF format from a buffer at an specific offset,.
   * the necessary settings of the write values must have been made
   * Will initialize write if necessary
   * Returns the amount of input bytes written, 0 when no longer bytes can be written or -1 on error
   */
  ssize_t handle_write_random( void *buffer,
                               size_t buffer_size,
                               off64_t offset,
                               libewf_error_t **error );

  /**
   * @brief Finalizes the write by correcting the EWF the meta data in the segment files.
   * This function is required after writing from stream
   * Returns the amount of input bytes written or -1 on error
   */
  ssize_t handle_write_finalize( libewf_error_t **error );

  /**
   * @brief Retrieves the segment filename size.
   * The filename size includes the end of string character
   * Returns 1 if successful, 0 if value not present or -1 on error
   */
  int handle_get_segment_filename_size( size_t *filename_size,
                                        libewf_error_t **error );

  /**
   * @brief Retrieves the segment filename.
   * The filename size should include the end of string character
   * Returns 1 if successful, 0 if value not present or -1 on error
   */
  int handle_get_segment_filename( char *filename,
                                   size_t filename_size,
                                   libewf_error_t **error );

  /**
   * @brief Sets the segment filename.
   * Returns 1 if successful or -1 on error
   */
  int handle_set_segment_filename( const char *filename,
                                   size_t filename_length,
                                   libewf_error_t **error );

  /**
   * @brief Retrieves the segment filename size.
   * The filename size includes the end of string character
   * Returns 1 if successful, 0 if value not present or -1 on error
   */
  int handle_get_segment_filename_size_wide( size_t *filename_size,
                                             libewf_error_t **error );

  /**
   * @brief Retrieves the segment filename.
   * The filename size should include the end of string character
   * Returns 1 if successful, 0 if value not present or -1 on error
   */
  int handle_get_segment_filename_wide( wchar_t *filename,
                                        size_t filename_size,
                                        libewf_error_t **error );

  /**
   * @brief Sets the segment filename.
   * Returns 1 if successful or -1 on error
   */
  int handle_set_segment_filename_wide( const wchar_t *filename,
                                        size_t filename_length,
                                        libewf_error_t **error );

  /**
   * @brief Retrieves the segment file size.
   * Returns 1 if successful or -1 on error
   */
  int handle_get_segment_file_size( size64_t *segment_file_size,
                                    libewf_error_t **error );

  /**
   * @brief Sets the segment file size.
   * Returns 1 if successful or -1 on error
   */
  int handle_set_segment_file_size( size64_t segment_file_size,
                                    libewf_error_t **error );

  /**
   * @brief Retrieves the delta segment filename size.
   * The filename size includes the end of string character
   * Returns 1 if successful, 0 if value not present or -1 on error
   */
  int handle_get_delta_segment_filename_size( size_t *filename_size,
                                              libewf_error_t **error );

  /**
   * @brief Retrieves the delta segment filename.
   * The filename size should include the end of string character
   * Returns 1 if successful, 0 if value not present or -1 on error
   */
  int handle_get_delta_segment_filename( char *filename,
                                         size_t filename_size,
                                         libewf_error_t **error );

  /**
   * @brief Sets the delta segment filename.
   * Returns 1 if successful or -1 on error
   */
  int handle_set_delta_segment_filename( const char *filename,
                                         size_t filename_length,
                                         libewf_error_t **error );

  /**
   * @brief Retrieves the delta segment filename size.
   * The filename size includes the end of string character
   * Returns 1 if successful, 0 if value not present or -1 on error
   */
  int handle_get_delta_segment_filename_size_wide( size_t *filename_size,
                                                   libewf_error_t **error );

  /**
   * @brief Retrieves the delta segment filename.
   * The filename size should include the end of string character
   * Returns 1 if successful, 0 if value not present or -1 on error
   */
  int handle_get_delta_segment_filename_wide( wchar_t *filename,
                                              size_t filename_size,
                                              libewf_error_t **error );

  /**
   * @brief Sets the delta segment filename.
   * Returns 1 if successful or -1 on error
   */
  int handle_set_delta_segment_filename_wide( const wchar_t *filename,
                                              size_t filename_length,
                                              libewf_error_t **error );

  /**
   * @brief Retrieves the delta segment file size.
   * Returns 1 if successful or -1 on error
   */
  int handle_get_delta_segment_file_size( size64_t *delta_segment_file_size,
                                          libewf_error_t **error );

  /**
   * @brief Sets the delta segment file size.
   * Returns 1 if successful or -1 on error
   */
  int handle_set_delta_segment_file_size( size64_t delta_segment_file_size,
                                          libewf_error_t **error );

  /**
   * @brief Retrieves the filename size of the (delta) segment file of the current chunk.
   * The filename size includes the end of string character
   * Returns 1 if successful or -1 on error
   */
  int handle_get_filename_size( size_t *filename_size,
                                libewf_error_t **error );

  /**
   * @brief Retrieves the filename of the (delta) segment file of the current chunk.
   * The filename size should include the end of string character
   * Returns 1 if successful or -1 on error
   */
  int handle_get_filename( char *filename,
                           size_t filename_size,
                           libewf_error_t **error );

  /**
   * @brief Retrieves the filename size of the (delta) segment file of the current chunk.
   * The filename size includes the end of string character
   * Returns 1 if successful or -1 on error
   */
  int handle_get_filename_size_wide( size_t *filename_size,
                                     libewf_error_t **error );

  /**
   * @brief Retrieves the filename of the (delta) segment file of the current chunk.
   * The filename size should include the end of string character
   * Returns 1 if successful or -1 on error
   */
  int handle_get_filename_wide( wchar_t *filename,
                                size_t filename_size,
                                libewf_error_t **error );
#endif

protected:
  void* handler;

};
#endif // _EWF_ACCESS_H
