#
# Borland C++ IDE generated makefile
#
.AUTODEPEND

#
# Borland C++ tools
#
IMPLIB  = Implib
BCC32   = Bcc32 +BccW32.cfg 
BCC32I  = Bcc32i +BccW32.cfg 
TLINK32 = TLink32
ILINK32 = Ilink32
TLIB    = TLib
BRC32   = Brc32
TASM32  = Tasm32

#
# IDE macros
#

#
# Options
#
IDE_LinkFLAGS32 =  -LC:\Borland\BCC55\LIB
CompLocalOptsAtC32_liberrordlib =  -weff -i128 -w-aus -wcln -wsig -wucp -wbbf -wpin -wnak -wdef -wnod -wamb -wuse -wstv -wasm -wamp -wobs -wpch -H-
LinkerLocalOptsAtC32_liberrordlib =  -LC:\Borland\BCC55\LIB -Tpd -aa -V4.0 -c
ResLocalOptsAtC32_liberrordlib = 
BLocalOptsAtC32_liberrordlib =  /P32
CompOptsAt_liberrordlib = $(CompLocalOptsAtC32_liberrordlib)
CompInheritOptsAt_liberrordlib = -I..\..\INCLUDE;..\..\COMMON;..\..\LIBERROR;C:\Borland\BCC55\INCLUDE -D_RTLDLL;
LinkerInheritOptsAt_liberrordlib = -x
LinkerOptsAt_liberrordlib = $(LinkerLocalOptsAtC32_liberrordlib)
ResOptsAt_liberrordlib = $(ResLocalOptsAtC32_liberrordlib)
BOptsAt_liberrordlib = $(BLocalOptsAtC32_liberrordlib)
CompLocalOptsAtC32_libnotifydlib =  -H-
LinkerLocalOptsAtC32_libnotifydlib =  -Tpd -aa -V4.0 -c
ResLocalOptsAtC32_libnotifydlib = 
BLocalOptsAtC32_libnotifydlib = 
CompOptsAt_libnotifydlib = $(CompLocalOptsAtC32_libnotifydlib)
CompInheritOptsAt_libnotifydlib = -I..\..\INCLUDE;..\..\COMMON;..\..\LIBERROR;..\..\LIBNOTIFY;C:\Borland\BCC55\INCLUDE -D_RTLDLL;
LinkerInheritOptsAt_libnotifydlib = -x
LinkerOptsAt_libnotifydlib = $(LinkerLocalOptsAtC32_libnotifydlib)
ResOptsAt_libnotifydlib = $(ResLocalOptsAtC32_libnotifydlib)
BOptsAt_libnotifydlib = $(BLocalOptsAtC32_libnotifydlib)
CompLocalOptsAtC32_libunadlib = 
LinkerLocalOptsAtC32_libunadlib =  -Tpd -aa -V4.0 -c
ResLocalOptsAtC32_libunadlib = 
BLocalOptsAtC32_libunadlib = 
CompOptsAt_libunadlib = $(CompLocalOptsAtC32_libunadlib)
CompInheritOptsAt_libunadlib = -I..\..\INCLUDE;..\..\COMMON;..\..\LIBERROR;..\..\LIBUNA;C:\Borland\BCC55\INCLUDE -D_RTLDLL;HAVE_LOCAL_LIBUNA
LinkerInheritOptsAt_libunadlib = -x
LinkerOptsAt_libunadlib = $(LinkerLocalOptsAtC32_libunadlib)
ResOptsAt_libunadlib = $(ResLocalOptsAtC32_libunadlib)
BOptsAt_libunadlib = $(BLocalOptsAtC32_libunadlib)
CompLocalOptsAtC32_libbfiodlib = 
LinkerLocalOptsAtC32_libbfiodlib =  -Tpd -aa -V4.0 -c
ResLocalOptsAtC32_libbfiodlib = 
BLocalOptsAtC32_libbfiodlib = 
CompOptsAt_libbfiodlib = $(CompLocalOptsAtC32_libbfiodlib)
CompInheritOptsAt_libbfiodlib = -I..\..\INCLUDE;..\..\COMMON;..\..\LIBERROR;..\..\LIBNOTIFY;..\..\LIBUNA;..\..\LIBBFIO;C:\Borland\BCC55\INCLUDE -D_RTLDLL;HAVE_LOCAL_LIBUNA;HAVE_LOCAL_LIBBFIO
LinkerInheritOptsAt_libbfiodlib = -x
LinkerOptsAt_libbfiodlib = $(LinkerLocalOptsAtC32_libbfiodlib)
ResOptsAt_libbfiodlib = $(ResLocalOptsAtC32_libbfiodlib)
BOptsAt_libbfiodlib = $(BLocalOptsAtC32_libbfiodlib)
LinkerLocalOptsAtC32_zlibdlib =  -Tpd -ap -c
ResLocalOptsAtC32_zlibdlib = 
BLocalOptsAtC32_zlibdlib = 
CompOptsAt_zlibdlib = $(CompLocalOptsAtC32_zlibdlib)
CompInheritOptsAt_zlibdlib = -I..\ZLIB-1.2.3;C:\Borland\BCC55\INCLUDE -D_RTLDLL;ZLIB_DLL;ZLIB_WINAPI
LinkerInheritOptsAt_zlibdlib = -x
LinkerOptsAt_zlibdlib = $(LinkerLocalOptsAtC32_zlibdlib)
ResOptsAt_zlibdlib = $(ResLocalOptsAtC32_zlibdlib)
BOptsAt_zlibdlib = $(BLocalOptsAtC32_zlibdlib)
LinkerLocalOptsAtC32_libewfdlib =  -Tpd -ap -c
ResLocalOptsAtC32_libewfdlib = 
BLocalOptsAtC32_libewfdlib = 
CompOptsAt_libewfdlib = $(CompLocalOptsAtC32_libewfdlib)
CompInheritOptsAt_libewfdlib = -I..\..\INCLUDE;..\..\COMMON;..\..\LIBERROR;..\..\LIBNOTIFY;..\..\LIBUNA;..\..\LIBBFIO;..\ZLIB-1.2.3;..\..\LIBEWF;C:\Borland\BCC55\INCLUDE -D_RTLDLL;HAVE_LOCAL_LIBUNA;HAVE_LOCAL_LIBBFIO;ZLIB_DLL;ZLIB_WINAPI;LIBEWF_DLL_EXPORT
LinkerInheritOptsAt_libewfdlib = -x
LinkerOptsAt_libewfdlib = $(LinkerLocalOptsAtC32_libewfdlib)
ResOptsAt_libewfdlib = $(ResLocalOptsAtC32_libewfdlib)
BOptsAt_libewfdlib = $(BLocalOptsAtC32_libewfdlib)
CompLocalOptsAtC32_libsystemdlib =  -WC
LinkerLocalOptsAtC32_libsystemdlib =  -Tpe -ap -c
ResLocalOptsAtC32_libsystemdlib = 
BLocalOptsAtC32_libsystemdlib = 
CompOptsAt_libsystemdlib = $(CompLocalOptsAtC32_libsystemdlib)
CompInheritOptsAt_libsystemdlib = -I..\..\INCLUDE;..\..\COMMON;..\..\LIBERROR;..\..\LIBUNA;..\..\LIBSYSTEM;C:\Borland\BCC55\INCLUDE -D_RTLDLL;HAVE_LOCAL_LIBUNA
LinkerInheritOptsAt_libsystemdlib = -x
LinkerOptsAt_libsystemdlib = $(LinkerLocalOptsAtC32_libsystemdlib)
ResOptsAt_libsystemdlib = $(ResLocalOptsAtC32_libsystemdlib)
BOptsAt_libsystemdlib = $(BLocalOptsAtC32_libsystemdlib)
CompLocalOptsAtC32_ewfinfodexe =  -WC
LinkerLocalOptsAtC32_ewfinfodexe =  -Tpe -ap -c
ResLocalOptsAtC32_ewfinfodexe = 
BLocalOptsAtC32_ewfinfodexe = 
CompOptsAt_ewfinfodexe = $(CompLocalOptsAtC32_ewfinfodexe)
CompInheritOptsAt_ewfinfodexe = -I..\..\INCLUDE;..\..\COMMON;..\..\LIBERROR;..\..\LIBUNA;..\..\LIBBFIO;..\..\LIBSYSTEM;..\..\LIBEWF;..\..\EWFTOOLS;C:\Borland\BCC55\INCLUDE -D_RTLDLL;HAVE_LOCAL_LIBUNA;HAVE_LOCAL_LIBBFIO;LIBEWF_DLL_IMPORT
LinkerInheritOptsAt_ewfinfodexe = -x
LinkerOptsAt_ewfinfodexe = $(LinkerLocalOptsAtC32_ewfinfodexe)
ResOptsAt_ewfinfodexe = $(ResLocalOptsAtC32_ewfinfodexe)
BOptsAt_ewfinfodexe = $(BLocalOptsAtC32_ewfinfodexe)

#
# Dependency List
#
Dep_libewf = \
   liberror.lib\
   libnotify.lib\
   libuna.lib\
   libbfio.lib\
   zlib.lib\
   libewf.lib\
   libsystem.lib\
   ewfinfo.exe

libewf : BccW32.cfg $(Dep_libewf)
  echo MakeNode

Dep_liberrordlib = \
   liberror_error.obj

liberror.lib : $(Dep_liberrordlib)
  $(TLIB) $< $(IDE_BFLAGS) $(BOptsAt_liberrordlib) @&&|
 -+liberror_error.obj
|

liberror_error.obj :  ..\..\liberror\liberror_error.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_liberrordlib) $(CompInheritOptsAt_liberrordlib) -o$@ ..\..\liberror\liberror_error.c
|

Dep_libnotifydlib = \
   libnotify.obj

libnotify.lib : $(Dep_libnotifydlib)
  $(TLIB) $< $(IDE_BFLAGS) $(BOptsAt_libnotifydlib) @&&|
 -+libnotify.obj
|

libnotify.obj :  ..\..\libnotify\libnotify.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libnotifydlib) $(CompInheritOptsAt_libnotifydlib) -o$@ ..\..\libnotify\libnotify.c
|

Dep_libunadlib = \
   libuna_codepage_windows_1252.obj\
   libuna_codepage_windows_1253.obj\
   libuna_codepage_windows_1254.obj\
   libuna_codepage_windows_1255.obj\
   libuna_codepage_windows_1256.obj\
   libuna_codepage_windows_1257.obj\
   libuna_codepage_windows_1258.obj\
   libuna_support.obj\
   libuna_unicode_character.obj\
   libuna_url_stream.obj\
   libuna_utf7_stream.obj\
   libuna_utf8_stream.obj\
   libuna_utf8_string.obj\
   libuna_utf16_stream.obj\
   libuna_utf16_string.obj\
   libuna_utf32_stream.obj\
   libuna_utf32_string.obj\
   libuna_base16_stream.obj\
   libuna_base64_stream.obj\
   libuna_base64url_stream.obj\
   libuna_byte_stream.obj\
   libuna_codepage_iso_8859_2.obj\
   libuna_codepage_iso_8859_3.obj\
   libuna_codepage_iso_8859_4.obj\
   libuna_codepage_iso_8859_5.obj\
   libuna_codepage_iso_8859_6.obj\
   libuna_codepage_iso_8859_7.obj\
   libuna_codepage_iso_8859_8.obj\
   libuna_codepage_iso_8859_9.obj\
   libuna_codepage_iso_8859_10.obj\
   libuna_codepage_iso_8859_13.obj\
   libuna_codepage_iso_8859_14.obj\
   libuna_codepage_iso_8859_15.obj\
   libuna_codepage_iso_8859_16.obj\
   libuna_codepage_koi8_r.obj\
   libuna_codepage_koi8_u.obj\
   libuna_codepage_windows_874.obj\
   libuna_codepage_windows_1250.obj\
   libuna_codepage_windows_1251.obj

libuna.lib : $(Dep_libunadlib)
  $(TLIB) $< $(IDE_BFLAGS) $(BOptsAt_libunadlib) @&&|
 -+libuna_codepage_windows_1252.obj &
-+libuna_codepage_windows_1253.obj &
-+libuna_codepage_windows_1254.obj &
-+libuna_codepage_windows_1255.obj &
-+libuna_codepage_windows_1256.obj &
-+libuna_codepage_windows_1257.obj &
-+libuna_codepage_windows_1258.obj &
-+libuna_support.obj &
-+libuna_unicode_character.obj &
-+libuna_url_stream.obj &
-+libuna_utf7_stream.obj &
-+libuna_utf8_stream.obj &
-+libuna_utf8_string.obj &
-+libuna_utf16_stream.obj &
-+libuna_utf16_string.obj &
-+libuna_utf32_stream.obj &
-+libuna_utf32_string.obj &
-+libuna_base16_stream.obj &
-+libuna_base64_stream.obj &
-+libuna_base64url_stream.obj &
-+libuna_byte_stream.obj &
-+libuna_codepage_iso_8859_2.obj &
-+libuna_codepage_iso_8859_3.obj &
-+libuna_codepage_iso_8859_4.obj &
-+libuna_codepage_iso_8859_5.obj &
-+libuna_codepage_iso_8859_6.obj &
-+libuna_codepage_iso_8859_7.obj &
-+libuna_codepage_iso_8859_8.obj &
-+libuna_codepage_iso_8859_9.obj &
-+libuna_codepage_iso_8859_10.obj &
-+libuna_codepage_iso_8859_13.obj &
-+libuna_codepage_iso_8859_14.obj &
-+libuna_codepage_iso_8859_15.obj &
-+libuna_codepage_iso_8859_16.obj &
-+libuna_codepage_koi8_r.obj &
-+libuna_codepage_koi8_u.obj &
-+libuna_codepage_windows_874.obj &
-+libuna_codepage_windows_1250.obj &
-+libuna_codepage_windows_1251.obj
|

libuna_codepage_windows_1252.obj :  ..\..\libuna\libuna_codepage_windows_1252.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_codepage_windows_1252.c
|

libuna_codepage_windows_1253.obj :  ..\..\libuna\libuna_codepage_windows_1253.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_codepage_windows_1253.c
|

libuna_codepage_windows_1254.obj :  ..\..\libuna\libuna_codepage_windows_1254.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_codepage_windows_1254.c
|

libuna_codepage_windows_1255.obj :  ..\..\libuna\libuna_codepage_windows_1255.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_codepage_windows_1255.c
|

libuna_codepage_windows_1256.obj :  ..\..\libuna\libuna_codepage_windows_1256.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_codepage_windows_1256.c
|

libuna_codepage_windows_1257.obj :  ..\..\libuna\libuna_codepage_windows_1257.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_codepage_windows_1257.c
|

libuna_codepage_windows_1258.obj :  ..\..\libuna\libuna_codepage_windows_1258.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_codepage_windows_1258.c
|

libuna_support.obj :  ..\..\libuna\libuna_support.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_support.c
|

libuna_unicode_character.obj :  ..\..\libuna\libuna_unicode_character.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_unicode_character.c
|

libuna_url_stream.obj :  ..\..\libuna\libuna_url_stream.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_url_stream.c
|

libuna_utf7_stream.obj :  ..\..\libuna\libuna_utf7_stream.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_utf7_stream.c
|

libuna_utf8_stream.obj :  ..\..\libuna\libuna_utf8_stream.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_utf8_stream.c
|

libuna_utf8_string.obj :  ..\..\libuna\libuna_utf8_string.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_utf8_string.c
|

libuna_utf16_stream.obj :  ..\..\libuna\libuna_utf16_stream.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_utf16_stream.c
|

libuna_utf16_string.obj :  ..\..\libuna\libuna_utf16_string.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_utf16_string.c
|

libuna_utf32_stream.obj :  ..\..\libuna\libuna_utf32_stream.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_utf32_stream.c
|

libuna_utf32_string.obj :  ..\..\libuna\libuna_utf32_string.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_utf32_string.c
|

libuna_base16_stream.obj :  ..\..\libuna\libuna_base16_stream.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_base16_stream.c
|

libuna_base64_stream.obj :  ..\..\libuna\libuna_base64_stream.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_base64_stream.c
|

libuna_base64url_stream.obj :  ..\..\libuna\libuna_base64url_stream.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_base64url_stream.c
|

libuna_byte_stream.obj :  ..\..\libuna\libuna_byte_stream.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_byte_stream.c
|

libuna_codepage_iso_8859_2.obj :  ..\..\libuna\libuna_codepage_iso_8859_2.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_codepage_iso_8859_2.c
|

libuna_codepage_iso_8859_3.obj :  ..\..\libuna\libuna_codepage_iso_8859_3.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_codepage_iso_8859_3.c
|

libuna_codepage_iso_8859_4.obj :  ..\..\libuna\libuna_codepage_iso_8859_4.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_codepage_iso_8859_4.c
|

libuna_codepage_iso_8859_5.obj :  ..\..\libuna\libuna_codepage_iso_8859_5.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_codepage_iso_8859_5.c
|

libuna_codepage_iso_8859_6.obj :  ..\..\libuna\libuna_codepage_iso_8859_6.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_codepage_iso_8859_6.c
|

libuna_codepage_iso_8859_7.obj :  ..\..\libuna\libuna_codepage_iso_8859_7.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_codepage_iso_8859_7.c
|

libuna_codepage_iso_8859_8.obj :  ..\..\libuna\libuna_codepage_iso_8859_8.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_codepage_iso_8859_8.c
|

libuna_codepage_iso_8859_9.obj :  ..\..\libuna\libuna_codepage_iso_8859_9.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_codepage_iso_8859_9.c
|

libuna_codepage_iso_8859_10.obj :  ..\..\libuna\libuna_codepage_iso_8859_10.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_codepage_iso_8859_10.c
|

libuna_codepage_iso_8859_13.obj :  ..\..\libuna\libuna_codepage_iso_8859_13.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_codepage_iso_8859_13.c
|

libuna_codepage_iso_8859_14.obj :  ..\..\libuna\libuna_codepage_iso_8859_14.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_codepage_iso_8859_14.c
|

libuna_codepage_iso_8859_15.obj :  ..\..\libuna\libuna_codepage_iso_8859_15.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_codepage_iso_8859_15.c
|

libuna_codepage_iso_8859_16.obj :  ..\..\libuna\libuna_codepage_iso_8859_16.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_codepage_iso_8859_16.c
|

libuna_codepage_koi8_r.obj :  ..\..\libuna\libuna_codepage_koi8_r.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_codepage_koi8_r.c
|

libuna_codepage_koi8_u.obj :  ..\..\libuna\libuna_codepage_koi8_u.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_codepage_koi8_u.c
|

libuna_codepage_windows_874.obj :  ..\..\libuna\libuna_codepage_windows_874.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_codepage_windows_874.c
|

libuna_codepage_windows_1250.obj :  ..\..\libuna\libuna_codepage_windows_1250.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_codepage_windows_1250.c
|

libuna_codepage_windows_1251.obj :  ..\..\libuna\libuna_codepage_windows_1251.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libunadlib) $(CompInheritOptsAt_libunadlib) -o$@ ..\..\libuna\libuna_codepage_windows_1251.c
|

Dep_libbfiodlib = \
   libbfio_system_string.obj\
   libbfio_handle.obj\
   libbfio_list_type.obj\
   libbfio_memory_range.obj\
   libbfio_offset_list.obj\
   libbfio_pool.obj\
   libbfio_support.obj\
   libbfio_error_string.obj\
   libbfio_file.obj

libbfio.lib : $(Dep_libbfiodlib)
  $(TLIB) $< $(IDE_BFLAGS) $(BOptsAt_libbfiodlib) @&&|
 -+libbfio_system_string.obj &
-+libbfio_handle.obj &
-+libbfio_list_type.obj &
-+libbfio_memory_range.obj &
-+libbfio_offset_list.obj &
-+libbfio_pool.obj &
-+libbfio_support.obj &
-+libbfio_error_string.obj &
-+libbfio_file.obj
|

libbfio_system_string.obj :  ..\..\libbfio\libbfio_system_string.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libbfiodlib) $(CompInheritOptsAt_libbfiodlib) -o$@ ..\..\libbfio\libbfio_system_string.c
|

libbfio_handle.obj :  ..\..\libbfio\libbfio_handle.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libbfiodlib) $(CompInheritOptsAt_libbfiodlib) -o$@ ..\..\libbfio\libbfio_handle.c
|

libbfio_list_type.obj :  ..\..\libbfio\libbfio_list_type.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libbfiodlib) $(CompInheritOptsAt_libbfiodlib) -o$@ ..\..\libbfio\libbfio_list_type.c
|

libbfio_memory_range.obj :  ..\..\libbfio\libbfio_memory_range.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libbfiodlib) $(CompInheritOptsAt_libbfiodlib) -o$@ ..\..\libbfio\libbfio_memory_range.c
|

libbfio_offset_list.obj :  ..\..\libbfio\libbfio_offset_list.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libbfiodlib) $(CompInheritOptsAt_libbfiodlib) -o$@ ..\..\libbfio\libbfio_offset_list.c
|

libbfio_pool.obj :  ..\..\libbfio\libbfio_pool.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libbfiodlib) $(CompInheritOptsAt_libbfiodlib) -o$@ ..\..\libbfio\libbfio_pool.c
|

libbfio_support.obj :  ..\..\libbfio\libbfio_support.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libbfiodlib) $(CompInheritOptsAt_libbfiodlib) -o$@ ..\..\libbfio\libbfio_support.c
|

libbfio_error_string.obj :  ..\..\libbfio\libbfio_error_string.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libbfiodlib) $(CompInheritOptsAt_libbfiodlib) -o$@ ..\..\libbfio\libbfio_error_string.c
|

libbfio_file.obj :  ..\..\libbfio\libbfio_file.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libbfiodlib) $(CompInheritOptsAt_libbfiodlib) -o$@ ..\..\libbfio\libbfio_file.c
|

zlib.lib : zlib.dll
  $(IMPLIB) $@ zlib.dll


Dep_zlibddll = \
   adler32.obj\
   compress.obj\
   crc32.obj\
   deflate.obj\
   gzio.obj\
   infback.obj\
   inffast.obj\
   inflate.obj\
   inftrees.obj\
   trees.obj\
   uncompr.obj\
   zutil.obj

zlib.dll : $(Dep_zlibddll)
  $(ILINK32) @&&|
 /v $(IDE_LinkFLAGS32) $(LinkerOptsAt_zlibdlib) $(LinkerInheritOptsAt_zlibdlib) +
C:\Borland\BCC55\LIB\c0d32.obj+
adler32.obj+
compress.obj+
crc32.obj+
deflate.obj+
gzio.obj+
infback.obj+
inffast.obj+
inflate.obj+
inftrees.obj+
trees.obj+
uncompr.obj+
zutil.obj
$<,$*
C:\Borland\BCC55\LIB\import32.lib+
C:\Borland\BCC55\LIB\cw32i.lib



|
adler32.obj :  ..\zlib-1.2.3\adler32.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_zlibdlib) $(CompInheritOptsAt_zlibdlib) -o$@ ..\zlib-1.2.3\adler32.c
|

compress.obj :  ..\zlib-1.2.3\compress.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_zlibdlib) $(CompInheritOptsAt_zlibdlib) -o$@ ..\zlib-1.2.3\compress.c
|

crc32.obj :  ..\zlib-1.2.3\crc32.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_zlibdlib) $(CompInheritOptsAt_zlibdlib) -o$@ ..\zlib-1.2.3\crc32.c
|

deflate.obj :  ..\zlib-1.2.3\deflate.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_zlibdlib) $(CompInheritOptsAt_zlibdlib) -o$@ ..\zlib-1.2.3\deflate.c
|

gzio.obj :  ..\zlib-1.2.3\gzio.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_zlibdlib) $(CompInheritOptsAt_zlibdlib) -o$@ ..\zlib-1.2.3\gzio.c
|

infback.obj :  ..\zlib-1.2.3\infback.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_zlibdlib) $(CompInheritOptsAt_zlibdlib) -o$@ ..\zlib-1.2.3\infback.c
|

inffast.obj :  ..\zlib-1.2.3\inffast.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_zlibdlib) $(CompInheritOptsAt_zlibdlib) -o$@ ..\zlib-1.2.3\inffast.c
|

inflate.obj :  ..\zlib-1.2.3\inflate.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_zlibdlib) $(CompInheritOptsAt_zlibdlib) -o$@ ..\zlib-1.2.3\inflate.c
|

inftrees.obj :  ..\zlib-1.2.3\inftrees.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_zlibdlib) $(CompInheritOptsAt_zlibdlib) -o$@ ..\zlib-1.2.3\inftrees.c
|

trees.obj :  ..\zlib-1.2.3\trees.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_zlibdlib) $(CompInheritOptsAt_zlibdlib) -o$@ ..\zlib-1.2.3\trees.c
|

uncompr.obj :  ..\zlib-1.2.3\uncompr.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_zlibdlib) $(CompInheritOptsAt_zlibdlib) -o$@ ..\zlib-1.2.3\uncompr.c
|

zutil.obj :  ..\zlib-1.2.3\zutil.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_zlibdlib) $(CompInheritOptsAt_zlibdlib) -o$@ ..\zlib-1.2.3\zutil.c
|

libewf.lib : libewf.dll
  $(IMPLIB) $@ libewf.dll


Dep_libewfddll = \
   libbfio.lib\
   liberror.lib\
   libnotify.lib\
   libuna.lib\
   zlib.lib\
   libewf_write_io_handle.obj\
   libewf.obj\
   libewf_chunk_cache.obj\
   libewf_compression.obj\
   libewf_date_time.obj\
   libewf_date_time_values.obj\
   libewf_debug.obj\
   libewf_empty_block.obj\
   libewf_error.obj\
   libewf_file_entry.obj\
   libewf_filename.obj\
   libewf_handle.obj\
   libewf_hash_sections.obj\
   libewf_hash_values.obj\
   libewf_header_sections.obj\
   libewf_header_values.obj\
   libewf_io_handle.obj\
   libewf_legacy.obj\
   libewf_list_type.obj\
   libewf_media_values.obj\
   libewf_metadata.obj\
   libewf_notify.obj\
   libewf_offset_table.obj\
   libewf_read_io_handle.obj\
   libewf_section.obj\
   libewf_section_list.obj\
   libewf_sector_table.obj\
   libewf_segment_file.obj\
   libewf_segment_file_handle.obj\
   libewf_segment_table.obj\
   libewf_single_file_entry.obj\
   libewf_single_files.obj\
   libewf_split_values.obj\
   libewf_string.obj\
   libewf_support.obj\
   libewf_tree_type.obj\
   libewf_values_table.obj

libewf.dll : $(Dep_libewfddll)
  $(ILINK32) @&&|
 /v $(IDE_LinkFLAGS32) $(LinkerOptsAt_libewfdlib) $(LinkerInheritOptsAt_libewfdlib) +
C:\Borland\BCC55\LIB\c0d32.obj+
libewf_write_io_handle.obj+
libewf.obj+
libewf_chunk_cache.obj+
libewf_compression.obj+
libewf_date_time.obj+
libewf_date_time_values.obj+
libewf_debug.obj+
libewf_empty_block.obj+
libewf_error.obj+
libewf_file_entry.obj+
libewf_filename.obj+
libewf_handle.obj+
libewf_hash_sections.obj+
libewf_hash_values.obj+
libewf_header_sections.obj+
libewf_header_values.obj+
libewf_io_handle.obj+
libewf_legacy.obj+
libewf_list_type.obj+
libewf_media_values.obj+
libewf_metadata.obj+
libewf_notify.obj+
libewf_offset_table.obj+
libewf_read_io_handle.obj+
libewf_section.obj+
libewf_section_list.obj+
libewf_sector_table.obj+
libewf_segment_file.obj+
libewf_segment_file_handle.obj+
libewf_segment_table.obj+
libewf_single_file_entry.obj+
libewf_single_files.obj+
libewf_split_values.obj+
libewf_string.obj+
libewf_support.obj+
libewf_tree_type.obj+
libewf_values_table.obj
$<,$*
libbfio.lib+
liberror.lib+
libnotify.lib+
libuna.lib+
zlib.lib+
C:\Borland\BCC55\LIB\import32.lib+
C:\Borland\BCC55\LIB\cw32i.lib



|
libewf_write_io_handle.obj :  ..\..\libewf\libewf_write_io_handle.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf_write_io_handle.c
|

libewf.obj :  ..\..\libewf\libewf.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf.c
|

libewf_chunk_cache.obj :  ..\..\libewf\libewf_chunk_cache.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf_chunk_cache.c
|

libewf_compression.obj :  ..\..\libewf\libewf_compression.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf_compression.c
|

libewf_date_time.obj :  ..\..\libewf\libewf_date_time.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf_date_time.c
|

libewf_date_time_values.obj :  ..\..\libewf\libewf_date_time_values.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf_date_time_values.c
|

libewf_debug.obj :  ..\..\libewf\libewf_debug.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf_debug.c
|

libewf_empty_block.obj :  ..\..\libewf\libewf_empty_block.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf_empty_block.c
|

libewf_error.obj :  ..\..\libewf\libewf_error.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf_error.c
|

libewf_file_entry.obj :  ..\..\libewf\libewf_file_entry.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf_file_entry.c
|

libewf_filename.obj :  ..\..\libewf\libewf_filename.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf_filename.c
|

libewf_handle.obj :  ..\..\libewf\libewf_handle.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf_handle.c
|

libewf_hash_sections.obj :  ..\..\libewf\libewf_hash_sections.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf_hash_sections.c
|

libewf_hash_values.obj :  ..\..\libewf\libewf_hash_values.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf_hash_values.c
|

libewf_header_sections.obj :  ..\..\libewf\libewf_header_sections.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf_header_sections.c
|

libewf_header_values.obj :  ..\..\libewf\libewf_header_values.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf_header_values.c
|

libewf_io_handle.obj :  ..\..\libewf\libewf_io_handle.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf_io_handle.c
|

libewf_legacy.obj :  ..\..\libewf\libewf_legacy.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf_legacy.c
|

libewf_list_type.obj :  ..\..\libewf\libewf_list_type.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf_list_type.c
|

libewf_media_values.obj :  ..\..\libewf\libewf_media_values.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf_media_values.c
|

libewf_metadata.obj :  ..\..\libewf\libewf_metadata.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf_metadata.c
|

libewf_notify.obj :  ..\..\libewf\libewf_notify.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf_notify.c
|

libewf_offset_table.obj :  ..\..\libewf\libewf_offset_table.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf_offset_table.c
|

libewf_read_io_handle.obj :  ..\..\libewf\libewf_read_io_handle.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf_read_io_handle.c
|

libewf_section.obj :  ..\..\libewf\libewf_section.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf_section.c
|

libewf_section_list.obj :  ..\..\libewf\libewf_section_list.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf_section_list.c
|

libewf_sector_table.obj :  ..\..\libewf\libewf_sector_table.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf_sector_table.c
|

libewf_segment_file.obj :  ..\..\libewf\libewf_segment_file.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf_segment_file.c
|

libewf_segment_file_handle.obj :  ..\..\libewf\libewf_segment_file_handle.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf_segment_file_handle.c
|

libewf_segment_table.obj :  ..\..\libewf\libewf_segment_table.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf_segment_table.c
|

libewf_single_file_entry.obj :  ..\..\libewf\libewf_single_file_entry.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf_single_file_entry.c
|

libewf_single_files.obj :  ..\..\libewf\libewf_single_files.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf_single_files.c
|

libewf_split_values.obj :  ..\..\libewf\libewf_split_values.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf_split_values.c
|

libewf_string.obj :  ..\..\libewf\libewf_string.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf_string.c
|

libewf_support.obj :  ..\..\libewf\libewf_support.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf_support.c
|

libewf_tree_type.obj :  ..\..\libewf\libewf_tree_type.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf_tree_type.c
|

libewf_values_table.obj :  ..\..\libewf\libewf_values_table.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libewfdlib) $(CompInheritOptsAt_libewfdlib) -o$@ ..\..\libewf\libewf_values_table.c
|

Dep_libsystemdlib = \
   libsystem_date_time.obj\
   libsystem_error.obj\
   libsystem_file.obj\
   libsystem_getopt.obj\
   libsystem_glob.obj\
   libsystem_notify.obj\
   libsystem_signal.obj\
   libsystem_split_values.obj\
   libsystem_string.obj\
   libsystem_support.obj

libsystem.lib : $(Dep_libsystemdlib)
  $(TLIB) $< $(IDE_BFLAGS) $(BOptsAt_libsystemdlib) @&&|
 -+libsystem_date_time.obj &
-+libsystem_error.obj &
-+libsystem_file.obj &
-+libsystem_getopt.obj &
-+libsystem_glob.obj &
-+libsystem_notify.obj &
-+libsystem_signal.obj &
-+libsystem_split_values.obj &
-+libsystem_string.obj &
-+libsystem_support.obj
|

libsystem_date_time.obj :  ..\..\libsystem\libsystem_date_time.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libsystemdlib) $(CompInheritOptsAt_libsystemdlib) -o$@ ..\..\libsystem\libsystem_date_time.c
|

libsystem_error.obj :  ..\..\libsystem\libsystem_error.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libsystemdlib) $(CompInheritOptsAt_libsystemdlib) -o$@ ..\..\libsystem\libsystem_error.c
|

libsystem_file.obj :  ..\..\libsystem\libsystem_file.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libsystemdlib) $(CompInheritOptsAt_libsystemdlib) -o$@ ..\..\libsystem\libsystem_file.c
|

libsystem_getopt.obj :  ..\..\libsystem\libsystem_getopt.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libsystemdlib) $(CompInheritOptsAt_libsystemdlib) -o$@ ..\..\libsystem\libsystem_getopt.c
|

libsystem_glob.obj :  ..\..\libsystem\libsystem_glob.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libsystemdlib) $(CompInheritOptsAt_libsystemdlib) -o$@ ..\..\libsystem\libsystem_glob.c
|

libsystem_notify.obj :  ..\..\libsystem\libsystem_notify.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libsystemdlib) $(CompInheritOptsAt_libsystemdlib) -o$@ ..\..\libsystem\libsystem_notify.c
|

libsystem_signal.obj :  ..\..\libsystem\libsystem_signal.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libsystemdlib) $(CompInheritOptsAt_libsystemdlib) -o$@ ..\..\libsystem\libsystem_signal.c
|

libsystem_split_values.obj :  ..\..\libsystem\libsystem_split_values.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libsystemdlib) $(CompInheritOptsAt_libsystemdlib) -o$@ ..\..\libsystem\libsystem_split_values.c
|

libsystem_string.obj :  ..\..\libsystem\libsystem_string.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libsystemdlib) $(CompInheritOptsAt_libsystemdlib) -o$@ ..\..\libsystem\libsystem_string.c
|

libsystem_support.obj :  ..\..\libsystem\libsystem_support.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_libsystemdlib) $(CompInheritOptsAt_libsystemdlib) -o$@ ..\..\libsystem\libsystem_support.c
|

Dep_ewfinfodexe = \
   libuna.lib\
   liberror.lib\
   libewf.lib\
   libsystem.lib\
   byte_size_string.obj\
   digest_hash.obj\
   ewfinfo.obj\
   ewfinput.obj\
   ewfoutput.obj\
   guid.obj\
   info_handle.obj

ewfinfo.exe : $(Dep_ewfinfodexe)
  $(ILINK32) @&&|
 /v $(IDE_LinkFLAGS32) $(LinkerOptsAt_ewfinfodexe) $(LinkerInheritOptsAt_ewfinfodexe) +
C:\Borland\BCC55\LIB\c0x32.obj+
byte_size_string.obj+
digest_hash.obj+
ewfinfo.obj+
ewfinput.obj+
ewfoutput.obj+
guid.obj+
info_handle.obj
$<,$*
libuna.lib+
liberror.lib+
libewf.lib+
libsystem.lib+
C:\Borland\BCC55\LIB\import32.lib+
C:\Borland\BCC55\LIB\cw32i.lib



|
byte_size_string.obj :  ..\..\ewftools\byte_size_string.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_ewfinfodexe) $(CompInheritOptsAt_ewfinfodexe) -o$@ ..\..\ewftools\byte_size_string.c
|

digest_hash.obj :  ..\..\ewftools\digest_hash.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_ewfinfodexe) $(CompInheritOptsAt_ewfinfodexe) -o$@ ..\..\ewftools\digest_hash.c
|

ewfinfo.obj :  ..\..\ewftools\ewfinfo.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_ewfinfodexe) $(CompInheritOptsAt_ewfinfodexe) -o$@ ..\..\ewftools\ewfinfo.c
|

ewfinput.obj :  ..\..\ewftools\ewfinput.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_ewfinfodexe) $(CompInheritOptsAt_ewfinfodexe) -o$@ ..\..\ewftools\ewfinput.c
|

ewfoutput.obj :  ..\..\ewftools\ewfoutput.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_ewfinfodexe) $(CompInheritOptsAt_ewfinfodexe) -o$@ ..\..\ewftools\ewfoutput.c
|

guid.obj :  ..\..\ewftools\guid.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_ewfinfodexe) $(CompInheritOptsAt_ewfinfodexe) -o$@ ..\..\ewftools\guid.c
|

info_handle.obj :  ..\..\ewftools\info_handle.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_ewfinfodexe) $(CompInheritOptsAt_ewfinfodexe) -o$@ ..\..\ewftools\info_handle.c
|

# Compiler configuration file
BccW32.cfg : 
   Copy &&|
-w
-R
-v
-WM-
-vi
-H
-H=liberror.csm
-wcln
-wsig
-wucp
-wbbf
-wpin
-wnak
-wdef
-wnod
-wamb
-w-aus
-wuse
-wstv
-wasm
-wamp
-wobs
-wpch
-i128
-H-
| $@


