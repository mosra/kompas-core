Name: kompas-core
URL: http://mosra.cz/blog/kompas.php
Version: 0.1.1
Release: 1
License: LGPLv3
Group: System/Libraries
Source: https://github.com/mosra/%{name}/tarball/v%{version}/%{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-buildroot
BuildRequires: cmake >= 2.6.0

Summary: Core library for Kompas navigation software

%description
Core interfaces and plugin management for Kompas.

%package devel
Group: Development/Libraries/C and C++
Summary: Kompas Core development files
Requires: %{name} = %{version}

%description devel
Core headers and tools needed for developing for Kompas.

%prep
%setup -q -n mosra-kompas-core-a23e2f7

%build
mkdir build && cd build
cmake .. \
    -DCMAKE_INSTALL_PREFIX=%{_prefix} \
    -DCMAKE_BUILD_TYPE=Release
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
cd build
make DESTDIR=$RPM_BUILD_ROOT install

%clean

%files
%defattr(-,root,root,-)
%{_prefix}/lib*

%files devel
%defattr(-,root,root,-)
%{_prefix}/bin/kompas-rc
%{_prefix}/include/Kompas
%{_prefix}/share/*/Modules
