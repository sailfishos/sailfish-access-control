Name:       sailfish-access-control-qt5
Summary:    Sailfish Access Control QML plugin
Version:    0.0.1
Release:    1
License:    LGPLv2+
URL:        https://github.com/sailfishos/sailfish-access-control
Source0:    %{name}-%{version}.tar.bz2
BuildRequires: pkgconfig(sailfishaccesscontrol) >= %{version}
BuildRequires: pkgconfig(Qt5Core)
BuildRequires: pkgconfig(Qt5Qml)

%description
%{summary}.

This library should be used to check whether a user
belongs to a group or not.

%package -n sailfish-access-control-qml-plugin
Summary:    Sailfish Access Control QML plugin
Requires:   sailfish-access-control = %{version}
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description -n sailfish-access-control-qml-plugin
%{summary}.

This library should be used to check whether a user
belongs to a group or not.

%prep
%setup -q -n %{name}-%{version}/qt

%build
%qmake5

%install

%qmake5_install

%post -n sailfish-access-control-qml-plugin -p /sbin/ldconfig

%postun -n sailfish-access-control-qml-plugin -p /sbin/ldconfig

%files -n sailfish-access-control-qml-plugin
%{_libdir}/qt5/qml/Sailfish/AccessControl
