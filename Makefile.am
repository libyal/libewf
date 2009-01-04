SUBDIRS = \
	libewf \
	ewftools \
	manuals

EXTRA_DIST = \
	include/libewf.h.in \
	include/libewf/libewf_definitions.h.in \
	include/libewf/libewf_types.h.in \
	libewf/libewf_config.h \
	libewf/libewf_config_windows.h \
	manuals/ewfacquire.1 \
	manuals/ewfacquirestream.1 \
	manuals/ewfexport.1 \
	manuals/ewfinfo.1 \
	manuals/ewfverify.1 \
	manuals/libewf.3 \
	TESTS \
	libewf.spec.in \
	libewf.spec \
	libewf.pc.in

pkgconfigdir = $(libdir)/pkgconfig

pkgconfig_DATA = \
	libewf.pc

include_HEADERS = \
	include/libewf.h

pkginclude_HEADERS = \
	include/libewf/libewf_definitions.h \
	include/libewf/libewf_extern.h \
	include/libewf/libewf_handle.h \
	include/libewf/libewf_types.h

libtool: @LIBTOOL_DEPS@
	cd $(srcdir) && $(SHELL) ./config.status --recheck

lib: library

library:
	cd $(srcdir)/libewf && $(MAKE) $(AM_MAKEFLAGS)

