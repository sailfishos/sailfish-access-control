Name:       sailfish-access-control
Summary:    Sailfish Access Control library
Version:    0.0.1
Release:    1
License:    LGPLv2+
URL:        https://github.com/sailfishos/sailfish-access-control
Source0:    %{name}-%{version}.tar.bz2
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
Requires: glibc
BuildRequires: glibc-devel
BuildRequires: pkgconfig(glib-2.0)

%description
Sailfish Access Control library is a thin wrapper on top
of pwd.h and grp.h of glibc.

This library should be used to check whether a user
belongs to a group or not.

%package devel
Summary:    Development files for Sailfish Access Control
Requires:   %{name} = %{version}-%{release}

%description devel
C language headers of the Sailfish Access Control library.

%prep
%setup -q -n %{name}-%{version}/glib

%build
make LIBDIR=%{_libdir} ROOT=%{buildroot} %{_smp_mflags} VERSION=%{version}
make LIBDIR=%{_libdir} ROOT=%{buildroot} VERSION=%{version} sailfishaccesscontrol.pc

%install
make LIBDIR=%{_libdir} ROOT=%{buildroot} install-libsailfishaccesscontrol
make LIBDIR=%{_libdir} ROOT=%{buildroot} install-libsailfishaccesscontrol-dev

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%license COPYING
%{_libdir}/libsailfishaccesscontrol.so.*

%files devel
%{_includedir}/sailfishaccesscontrol/*
%{_libdir}/libsailfishaccesscontrol.so
%{_libdir}/pkgconfig/sailfishaccesscontrol.pc
