if HAVE_PYTHON
AM_CFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	-I$(top_srcdir)/libcstring \
	-I$(top_srcdir)/liberror \
	@MEMWATCH_CPPFLAGS@

# The libewf python module (pyewf)
pyexec_LTLIBRARIES = pyewf.la

pyewf_la_SOURCES = \
	pyewf.c pyewf.h \
	pyewf_handle.c pyewf_handle.h \
	pyewf_metadata.c pyewf_metadata.h

pyewf_la_LIBADD = \
	../libcstring/libcstring.la \
	../liberror/liberror.la \
	../libewf/libewf.la

pyewf_la_CPPFLAGS = $(PYTHON_CPPFLAGS)
pyewf_la_LDFLAGS  = -module $(PYTHON_LDFLAGS)

endif

distclean: clean
	/bin/rm -f Makefile

