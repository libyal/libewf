dnl Function to test if a certain feature was enabled
AC_DEFUN([LIBEWF_TEST_ENABLE],
	[AC_ARG_ENABLE(
	 [$1],
	 [AS_HELP_STRING(
	  [--enable-$1],
	  [$3 (default is $4)])],
	 [ac_libewf_enable_$2=$enableval],
	 [ac_libewf_enable_$2=$4])dnl
	 AC_CACHE_CHECK(
	  [whether to enable $3],
	  [ac_libewf_enable_$2],
	  [ac_libewf_enable_$2=$4])dnl
])

dnl Function to detect whether printf conversion specifier "%jd" is available
AC_DEFUN([LIBEWF_PRINTF_JD],
	[SAVE_CFLAGS="$CFLAGS"
	CFLAGS="$CFLAGS -Wall -Werror"
	AC_LANG_PUSH(C)
	AC_MSG_CHECKING(
	 [whether printf supports the conversion specifier "%jd"])
	dnl First try to see if compilation and linkage without a parameter succeeds
	AC_LINK_IFELSE(
		[AC_LANG_PROGRAM(
		 [[#include <stdio.h>]],
		 [[printf( "%jd" ); ]] )],
		[AC_MSG_RESULT(
		 [no])],
		dnl Second try to see if compilation and linkage with a parameter succeeds
	 	[AC_LINK_IFELSE(
			[AC_LANG_PROGRAM(
			 [[#include <sys/types.h>
#include <stdio.h>]],
		 	 [[printf( "%jd", (off_t) 10 ); ]] )],
			dnl Third try to see if the program runs correctly
	 		 [AC_RUN_IFELSE(
				[AC_LANG_PROGRAM(
				 [[#include <sys/types.h>
#include <stdio.h>]],
				 [[char string[ 3 ];
if( snprintf( string, 3, "%jd", (off_t) 10 ) < 0 ) return( 1 );
if( ( string[ 0 ] != '1' ) || ( string[ 1 ] != '0' ) ) return( 1 ); ]] )],
				[AC_MSG_RESULT(
				 [yes])
				AC_DEFINE(
				 [HAVE_PRINTF_JD],
				 [1],
				 [Define to 1 whether printf supports the conversion specifier "%jd".] )],
				[AC_MSG_RESULT(
				 [no])],
				[AC_MSG_RESULT(
				 [undetermined])] )],
			[AC_MSG_RESULT(
			 [no])] )] )
	AC_LANG_POP(C)
	CFLAGS="$SAVE_CFLAGS"])

dnl Function to detect whether printf conversion specifier "%zd" is available
AC_DEFUN([LIBEWF_PRINTF_ZD],
	[SAVE_CFLAGS="$CFLAGS"
	 CFLAGS="$CFLAGS -Wall -Werror"
	 AC_LANG_PUSH(C)
	 AC_MSG_CHECKING(
	  [whether printf supports the conversion specifier "%zd"])
	dnl First try to see if compilation and linkage without a parameter succeeds
	AC_LINK_IFELSE(
		[AC_LANG_PROGRAM(
		 [[#include <stdio.h>]],
		 [[printf( "%zd" ); ]] )],
		[AC_MSG_RESULT(
		 [no])],
		dnl Second try to see if compilation and linkage with a parameter succeeds
	 	[AC_LINK_IFELSE(
			[AC_LANG_PROGRAM(
			 [[#include <sys/types.h>
#include <stdio.h>]],
		 	 [[printf( "%zd", (size_t) 10 ); ]] )],
			dnl Third try to see if the program runs correctly
	 		 [AC_RUN_IFELSE(
				[AC_LANG_PROGRAM(
				 [[#include <sys/types.h>
#include <stdio.h>]],
				 [[char string[ 3 ];
if( snprintf( string, 3, "%zd", (size_t) 10 ) < 0 ) return( 1 );
if( ( string[ 0 ] != '1' ) || ( string[ 1 ] != '0' ) ) return( 1 ); ]] )],
				[AC_MSG_RESULT(
				 [yes])
				AC_DEFINE(
				 [HAVE_PRINTF_ZD],
				 [1],
				 [Define to 1 whether printf supports the conversion specifier "%zd".] )],
				[AC_MSG_RESULT(
				 [no])],
				[AC_MSG_RESULT(
				 [undetermined])] )],
			[AC_MSG_RESULT(
			 [no])] )] )
	AC_LANG_POP(C)
	CFLAGS="$SAVE_CFLAGS"])

dnl Function to detect if ctime_r is defined according to the POSIX standard
AC_DEFUN([LIBEWF_FUNC_CTIME_R],
	[AC_CHECK_FUNC(
	 [ctime_r],
	 [AC_DEFINE(
	  [HAVE_CTIME_R],
	  [1],
	  [Define to 1 if you have the ctime_r function.] )
	 AC_LANG_PUSH(C)
	 AC_MSG_CHECKING(
	  [if ctime_r is defined according to the POSIX definition])
	 AC_LINK_IFELSE(
		AC_LANG_PROGRAM(
		 [[#include <time.h>]],
		 [[ctime_r(NULL,NULL,0)]]),
		[AC_MSG_RESULT(
		 [ctime_r with additional size argument detected])
		AC_DEFINE(
		 [HAVE_CTIME_R_SIZE],
		 [1],
		 [Define to 1 if you have the ctime_r function with a third size argument.] )],
		[AC_LINK_IFELSE(
			AC_LANG_PROGRAM(
			 [[#include <time.h>]],
			 [[ctime_r(NULL,NULL)]]),
			 [AC_MSG_RESULT(
			  [yes])],
			 [CPPFLAGS="$CPPFLAGS -D_POSIX_PTHREAD_SEMANTICS"
			 AC_LINK_IFELSE(
				AC_LANG_PROGRAM(
				 [[#include <time.h>]],
				 [[ctime_r(NULL,NULL)]] ),
				[AC_MSG_RESULT(
				 [ctime_r requires additional compile flags])],
				[AC_MSG_WARN(
				 [Unable to determine how to compile ctime_r])] )] )] )
	AC_LANG_POP(C) ])
])

dnl Function to determine the operating system
AC_DEFUN([LIBEWF_DETERMINE_OPERATING_SYSTEM],
[ac_libewf_target_string="$target";
AS_IF(
	[test "x$ac_libewf_target_string" = x],
	[ac_libewf_target_string="$build"])

AS_CASE(
	[$ac_libewf_target_string],
	[*cygwin*], [ac_libewf_operating_system="Cygwin";],
	[*darwin*], [ac_libewf_operating_system="Darwin";],
	[*freebsd*], [ac_libewf_operating_system="FreeBSD";],
	[*netbsd*], [ac_libewf_operating_system="NetBSD";],
	[*openbsd*], [ac_libewf_operating_system="OpenBSD";],
	[*linux*], [ac_libewf_operating_system="Linux";],
	[*mingw*], [ac_libewf_operating_system="MingW";],
	[*solaris*], [ac_libewf_operating_system="SunOS";],
	[*], [ac_libewf_operating_system="Unknown";])

AC_DEFINE_UNQUOTED(
 LIBEWF_OPERATING_SYSTEM,
 "$ac_libewf_operating_system",
 [Defines the fallback operating system string.])
])

dnl Function to detect if Python build environment is available
AC_DEFUN([LIBEWF_PYTHON_DEVEL],
[AC_REQUIRE([AM_PATH_PYTHON])

dnl Check for Python include path
AC_MSG_CHECKING(
 [for Python include path])

PYTHON_INCLUDE_DIR=`$PYTHON -c "import distutils.sysconfig;print distutils.sysconfig.get_python_inc() "`;
AC_MSG_RESULT(
 [$PYTHON_INCLUDE_DIR])

AC_SUBST(
 [PYTHON_CPPFLAGS],
 [-I$PYTHON_INCLUDE_DIR])

AS_IF(
	[test ! -r $PYTHON_INCLUDE_DIR/Python.h],
	[AC_MSG_ERROR(
	 [Missing Python include file])])

dnl Check for Python library path
AC_MSG_CHECKING(
 [for Python library path])

PYTHON_LDFLAGS=`$PYTHON -c "import distutils.sysconfig;print distutils.sysconfig.get_python_lib() "`;
AC_MSG_RESULT(
 [$PYTHON_LDFLAGS])

AC_SUBST(
 [PYTHON_LDFLAGS],
 ["-L$python_path -lpython$PYTHON_VERSION"])

python_site=`echo $python_path | sed "s/config/site-packages/"`;

AC_SUBST(
 [PYTHON_SITE_PKG],
 [$python_site])

dnl Determine the Python libraries which must be linked in when embedding
AC_MSG_CHECKING(
 [for Python extra libraries])

PYTHON_EXTRA_LIBS=`$PYTHON -c "import distutils.sysconfig;conf = distutils.sysconfig.get_config_var;print conf('LOCALMODLIBS')+' '+conf('LIBS')"`;
AC_MSG_RESULT(
 [$PYTHON_EXTRA_LIBS])

AC_SUBST(
 [PYTHON_EXTRA_LIBS])
])

