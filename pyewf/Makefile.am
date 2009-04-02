if HAVE_PYTHON

AM_CFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/common

# This is for the libewf python module (pyewf)
pyexec_LTLIBRARIES = pyewf.la

pyewf_la_SOURCES = \
	pyewf.c pyewf.h \
	pyewf_file.c pyewf_file.h

pyewf_la_LIBADD   = ../libewf/libewf.la
pyewf_la_CPPFLAGS = $(PYTHON_CPPFLAGS)
pyewf_la_LDFLAGS  = -module $(PYTHON_LDFLAGS)

endif

