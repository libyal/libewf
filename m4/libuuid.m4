dnl Functions for libuuid
dnl
dnl Version: 20120730

dnl Function to detect if libuuid is available
AC_DEFUN([AX_LIBUUID_CHECK_LIB],
 [dnl Check if parameters were provided
 AS_IF(
  [test "x$ac_cv_with_libuuid" != x && test "x$ac_cv_with_libuuid" != xno && test "x$ac_cv_with_libuuid" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_libuuid"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_libuuid}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_libuuid}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_libuuid])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_with_libuuid" = xno],
  [ac_cv_libuuid=no],
  [AS_IF(
   [test "x$ac_cv_enable_winapi" = xyes],
   [ac_cv_libuuid=librpcrt4
   ac_cv_libuuid_LIBADD=-lrpcrt4],
   [ac_cv_libuuid=no])
 
  AS_IF(
   [test "x$ac_cv_libuuid" = xno],
   [dnl Check for a pkg-config file
   AS_IF(
    [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
    [PKG_CHECK_MODULES(
     [uuid],
     [uuid >= 2.20],
     [ac_cv_libuuid=libuuid],
     [ac_cv_libuuid=no])
    ])

   AS_IF(
    [test "x$ac_cv_libuuid" = xuuid],
    [ac_cv_libuuid_CPPFLAGS="$pkg_cv_uuid_CFLAGS"
    ac_cv_libuuid_LIBADD="$pkg_cv_uuid_LIBS"],
    [dnl Check for headers
    AC_CHECK_HEADERS([uuid/uuid.h])
  
    AS_IF(
     [test "x$ac_cv_header_uuid_uuid_h" != xno],
     [AC_CHECK_FUNC(
      [uuid_generate],
      [ac_cv_libuuid=native],
      [AC_CHECK_LIB(
       [uuid],
       [uuid_generate],
       [ac_cv_libuuid=libuuid
       ac_cv_libuuid_LIBADD="-luuid"],
       [ac_cv_libuuid=no])
      ])
     ])
    ])
  
   AS_IF(
    [test "x$ac_cv_libuuid" != xno],
    [AC_DEFINE(
     [HAVE_GUID_SUPPORT],
     [1],
     [Define to 1 if GUID/UUID support is available.])
    ])
  
   AS_IF(
    [test "x$ac_cv_libuuid" = xnative],
    [AC_CHECK_FUNC(
     [uuid_generate_random],
     [AC_DEFINE(
      [HAVE_UUID_GENERATE_RANDOM],
      [1],
      [Define to 1 if uuid_generate_random funtion is available.])
     ])
    AC_CHECK_FUNC(
     [uuid_generate_time],
     [AC_DEFINE(
      [HAVE_UUID_GENERATE_TIME],
      [1],
      [Define to 1 if uuid_generate_time funtion is available.])
     ])
    ])
   
   AS_IF(
    [test "x$ac_cv_libuuid" = xlibuuid],
    [AC_CHECK_LIB(
     [uuid],
     [uuid_generate_random],
     [AC_DEFINE(
      [HAVE_UUID_GENERATE_RANDOM],
      [1],
      [Define to 1 if uuid_generate_random funtion is available in libuuid.])
     ])
    AC_CHECK_LIB(
     [uuid],
     [uuid_generate_time],
     [AC_DEFINE(
      [HAVE_UUID_GENERATE_TIME],
      [1],
      [Define to 1 if uuid_generate_time funtion is available in libuuid.])
     ])
    ])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_libuuid" = xlibrpcrt4],
  [AC_DEFINE(
   [HAVE_LIBUUID],
   [1],
   [Define to 1 if you have the 'librpcrt4' library (-lrpcrt4).])
  ])
 AS_IF(
  [test "x$ac_cv_libuuid" = xlibuuid],
  [AC_DEFINE(
   [HAVE_LIBUUID],
   [1],
   [Define to 1 if you have the 'libuuid' library (-luuid).])
  ])

 AS_IF(
  [test "x$ac_cv_libuuid" != xno],
  [AC_SUBST(
   [HAVE_LIBUUID],
   [1]) ],
  [AC_SUBST(
   [HAVE_LIBUUID],
   [0])
  ])
 ])

dnl Function to detect how to enable libuuid
AC_DEFUN([AX_LIBUUID_CHECK_ENABLE],
 [AX_COMMON_ARG_WITH(
  [libuuid],
  [libuuid],
  [search for libuuid in includedir and libdir or in the specified DIR, or no if not to use libuuid],
  [auto-detect],
  [DIR])

 dnl Check for a shared library version
 AX_LIBUUID_CHECK_LIB

 AS_IF(
  [test "x$ac_cv_libuuid_CPPFLAGS" != "x"],
  [AC_SUBST(
   [LIBUUID_CPPFLAGS],
   [$ac_cv_libuuid_CPPFLAGS])
  ])
 AS_IF(
  [test "x$ac_cv_libuuid_LIBADD" != "x"],
  [AC_SUBST(
   [LIBUUID_LIBADD],
   [$ac_cv_libuuid_LIBADD])
  ])

 AS_IF(
  [test "x$ac_cv_libuuid" = xlibuuid],
  [AC_SUBST(
   [ax_libuuid_pc_libs_private],
   [-luuid])
  ])

 AS_IF(
  [test "x$ac_cv_libuuid" = xlibuuid],
  [AC_SUBST(
   [ax_libuuid_spec_requires],
   [libuuid])
  AC_SUBST(
   [ax_libuuid_spec_build_requires],
   [libuuid-devel])
  ])
 ])

