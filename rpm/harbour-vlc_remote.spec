# 
# Do NOT Edit the Auto-generated Part!
# Generated by: spectacle version 0.27
# 

Name:       harbour-vlc_remote

# >> macros
# << macros

%{!?qtc_qmake:%define qtc_qmake %qmake}
%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}
%{!?qtc_make:%define qtc_make make}
%{?qtc_builddir:%define _builddir %qtc_builddir}
Summary:    This VLC remote allows user to control basic features of the VLC player using your mobile phone
Version:    1.3.2
Release:    0
Group:      Qt/Qt
License:    LICENSE
URL:        http://example.org/
Source0:    %{name}-%{version}.tar.bz2
Source100:  harbour-vlc_remote.yaml
Requires:   qt5-qtdeclarative-import-xmllistmodel
Requires:   sailfishsilica-qt5 >= 0.10.9
Requires:   mpris-qt5
BuildRequires:  pkgconfig(sailfishapp) >= 0.0.10
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(Qt5Xml)
BuildRequires:  pkgconfig(mpris-qt5)
BuildRequires:  desktop-file-utils

%description
Short description of my SailfishOS Application


%prep
%setup -q -n %{name}-%{version}

# >> setup
# << setup

%build
# >> build pre
# << build pre

%qtc_qmake5 

%qtc_make %{?_smp_mflags}

# >> build post
# << build post

%install
rm -rf %{buildroot}
# >> install pre
# << install pre
%qmake5_install

# >> install post
# << install post

desktop-file-install --delete-original       \
  --dir %{buildroot}%{_datadir}/applications             \
   %{buildroot}%{_datadir}/applications/*.desktop

%files
%defattr(-,root,root,-)
/usr/share/icons/hicolor/86x86/apps
/usr/share/applications
/usr/share/harbour-vlc_remote
/usr/bin
%{_datadir}/icons/hicolor/86x86/apps/%{name}.png
%{_datadir}/applications/%{name}.desktop
%{_datadir}/%{name}/qml
%{_bindir}
# >> files
# << files
