Name: libewf
Version: 20080305
Release: 1
Summary: Library to support the Expert Witness Compression Format
Group: System Environment/Libraries
License: BSD
Source: %{name}-%{version}.tar.gz
URL: https://www.uitwisselplatform.nl/projects/libewf
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
Requires: zlib
BuildRequires: zlib-devel

%description
libewf is library for support of the Expert Witness Compression Format (EWF).
libewf allows you to read media information of EWF files in the SMART (EWF-S01)
format and the EnCase (EWF-E01) format. libewf allows to read files created by
EnCase 1 to 6, linen and FTK Imager.

%package devel
Summary: Header files and libraries for developing applications for libewf
Group: Development/Libraries
Requires: libewf = %{version}-%{release}

%description devel
Header files and libraries for developing applications for libewf.

%package tools
Summary: Several tools for reading and writing EWF files
Group: Applications/System
Requires: openssl libewf = %{version}-%{release}
BuildRequires: zlib-devel openssl-devel

%description tools
Several tools for reading and writing EWF files.

%prep
%setup -q

%build
%configure --prefix=/usr --libdir=%{_libdir} --mandir=%{_mandir}
make %{?_smp_mflags}

%install
rm -rf ${RPM_BUILD_ROOT}
make DESTDIR=${RPM_BUILD_ROOT} install

%clean
rm -rf ${RPM_BUILD_ROOT}

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(644,root,root,755)
%doc AUTHORS COPYING NEWS README
%attr(755,root,root) %{_libdir}/*.so.*

%files devel
%defattr(644,root,root,755)
%doc AUTHORS COPYING NEWS README ChangeLog
%{_libdir}/*.a
%{_libdir}/*.la
%{_libdir}/*.so
%{_libdir}/pkgconfig/libewf.pc
%{_includedir}/*
%{_mandir}/man3/*

%files tools
%defattr(644,root,root,755)
%doc AUTHORS COPYING NEWS README
%attr(755,root,root) %{_bindir}/ewfacquire
%attr(755,root,root) %{_bindir}/ewfacquirestream
%attr(755,root,root) %{_bindir}/ewfexport
%attr(755,root,root) %{_bindir}/ewfinfo
%attr(755,root,root) %{_bindir}/ewfverify
%{_mandir}/man1/*

### Exclude expirimental files ###
%exclude %{_bindir}/ewfalter

%changelog
* Sat Dec 29 2007 Joachim Metz <forensics@hoffmannbv.nl> 20071229-1
- Updated URL

* Sun Dec 9 2007 Joachim Metz <forensics@hoffmannbv.nl> 20071209-1
- Moved pkgconfig file from excluded to development
- Adjustments to Requires and BuildRequires
- Adjusted description of library package removed tools
- Corrected groups

* Sat Sep 15 2007 Joachim Metz <forensics@hoffmannbv.nl> 20070915-1
- Adjustment to text
- library and tools are now stored in seperate packages

* Mon Jan 15 2007 Joachim Metz <forensics@hoffmannbv.nl> 20070115-1
- Added ewfacquirestream to package

* Fri Dec 29 2006 Joachim Metz <forensics@hoffmannbv.nl> 20061229-1
- Added exclusion of new expirimental addtitions

* Tue Dec 26 2006 Christophe Grenier <grenier@cgsecurity.org> 20061223-2
- Made small correction to the spec file, removed abundant Requires line

* Sat Dec 23 2006 Joachim Metz <forensics@hoffmannbv.nl> 20061223-1
- Made small corrections to the spec file input by Christophe Grenier
- Added --libdir to ./configure to correct for /usr/lib64

* Sat Dec 19 2006 Joachim Metz <forensics@hoffmannbv.nl> 20061219-1
- Made small corrections to the spec file input by Christophe Grenier
- The library source package no longer contains a release number

* Sat Dec 16 2006 Christophe Grenier <grenier@cgsecurity.org> 20061213-2
- Fixed the spec file

* Sat Dec 9 2006 Joachim Metz <forensics@hoffmannbv.nl> 20061213-1
- Initial version

