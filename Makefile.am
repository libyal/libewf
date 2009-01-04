SUBDIRS = \
	libewf \
	ewftools \
	manuals

EXTRA_DIST = \
	debian/changelog \
	debian/changelog.in \
	debian/compat \
	debian/control \
	debian/copyright \
	debian/cron.d.ex \
	debian/emacsen-install.ex \
	debian/emacsen-startup.ex \
	debian/emacsen-remove.ex \
	debian/init.d.ex \
	debian/libewf.install \
	debian/libewf.dirs \
	debian/libewf.docs \
	debian/libewf.postinst.debhelper \
	debian/libewf.postrm.debhelper \
	debian/libewf-default.ex \
	debian/libewf-dev.dirs \
	debian/libewf-dev.docs \
	debian/libewf-dev.install \
	debian/libewf-tools.dirs \
	debian/libewf-tools.docs \
	debian/libewf-tools.install \
	debian/libewf.doc-base.EX \
	debian/manpage.1.ex \
	debian/manpage.sgml.ex \
	debian/manpage.xml.ex \
	debian/menu.ex \
	debian/rules \
	debian/postinst.ex \
	debian/postrm.ex \
	debian/preinst.ex \
	debian/prerm.ex \
	debian/shlibs.local.ex \
	debian/shlibs.local.ex.in \
	debian/watch.ex \
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
	msvscpp/ewfacquire \
	msvscpp/ewfacquire/ewfacquire.vcproj \
	msvscpp/ewfacquirestream \
	msvscpp/ewfacquirestream/ewfacquirestream.vcproj \
	msvscpp/ewfexport \
	msvscpp/ewfexport/ewfexport.vcproj \
	msvscpp/ewfinfo \
	msvscpp/ewfinfo/ewfinfo.vcproj \
	msvscpp/ewfverify \
	msvscpp/ewfverify/ewfverify.vcproj \
	msvscpp/libewf_dll \
	msvscpp/libewf_dll/libewf_dll.vcproj \
	msvscpp/libewf.sln \
	libewf.pc \
	libewf.pc.in \
	libewf.spec \
	libewf.spec.in \
	TESTS

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

distclean: clean
	/bin/rm -f Makefile
	/bin/rm -f config.status config.cache config.log
	/bin/rm -f libewf.pc
	/bin/rm -f libewf.spec
	/bin/rm -f debian/changelog
	/bin/rm -f debian/shlibs.local.ex
	/bin/rm -f include/libewf.h
	/bin/rm -f include/libewf/libewf_definitions.h
	/bin/rm -f include/libewf/libewf_types.h
	/bin/rm -f libewf/libewf_config.h
	@for dir in ${subdirs}; do \
		(cd $$dir && $(MAKE) distclean) \
		|| case "$(MFLAGS)" in *k*) fail=yes;; *) exit 1;; esac; \
	done && test -z "$$fail"

