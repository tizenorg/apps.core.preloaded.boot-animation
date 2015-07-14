#sbs-git:slp/pkgs/b/boot-animation boot-animation 0.2 d1bbca3948e4cdb6b2f9e75f176500f452fe6a33

%if "%{?tizen_profile_name}" == "wearable"
	%define micro_ani ON
%else
	%define micro_ani OFF
%endif

%if "%{?tizen_profile_name}" == "tv"
ExcludeArch: %{arm} %ix86 x86_64
%endif

Name: boot-animation
Version: 0.3.7
Release: 7
Summary: Boot animation
URL: http://slp-source.sec.samsung.net
Source: %{name}-%{version}.tar.gz
Source1: boot-animation.service
Source2: shutdown-animation.service
Source3: silent-animation.service
Source4: late-tizen-system.service
Source5: boot-animation.path
License: Apache
Group: Samsung/Application
BuildRoot: %(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)
BuildRequires: cmake
BuildRequires: edje, edje-bin, embryo, embryo-bin
BuildRequires: pkgconfig(ecore)
BuildRequires: pkgconfig(libexif)
BuildRequires: pkgconfig(elementary)
BuildRequires: pkgconfig(vconf)
BuildRequires: pkgconfig(mm-bootsound)
BuildRequires: pkgconfig(capi-appfw-preference)
BuildRequires: pkgconfig(capi-system-info)
BuildRequires: pkgconfig(capi-system-system-settings)

Requires(post): /usr/bin/vconftool

%description
Shows an animation and plays a sound when the device is booted or shutdown.

%prep
%setup -q
#%patch0 -p1

%build
%ifarch %{arm}
%define ARCH arm
%else
%define ARCH emulator
%endif

cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix} -DUSE_MICRO_ANI=%{micro_ani} -DARCH=%{ARCH} -DTIZEN_PROFILE_NAME=%{tizen_profile_name}

make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
mkdir -p %{buildroot}/usr/share/license
cp -f LICENSE %{buildroot}/usr/share/license/%{name}
%make_install

mkdir -p %{buildroot}/usr/lib/systemd/system/multi-user.target.wants
install -m 0644 %SOURCE1 %{buildroot}/usr/lib/systemd/system/boot-animation.service
install -m 0644 %SOURCE2 %{buildroot}/usr/lib/systemd/system/shutdown-animation.service
install -m 0644 %SOURCE3 %{buildroot}/usr/lib/systemd/system/silent-animation.service
ln -s ../silent-animation.service %{buildroot}/usr/lib/systemd/system/multi-user.target.wants/
install -m 0644 %SOURCE4 %{buildroot}/usr/lib/systemd/system/late-tizen-system.service
ln -s ../late-tizen-system.service %{buildroot}/usr/lib/systemd/system/multi-user.target.wants/
install -m 0644 %SOURCE5 %{buildroot}/usr/lib/systemd/system/boot-animation.path
ln -s ../boot-animation.path %{buildroot}/usr/lib/systemd/system/multi-user.target.wants/

%clean
make clean

%docs_package

%post


%files
%manifest boot-animation.manifest
/usr/share/edje/poweroff.edj
/usr/share/edje/poweron.edj
/usr/share/edje/mobile_poweroff.edj
/usr/share/edje/mobile_poweron.edj
/usr/share/edje/wearable_poweroff.edj
/usr/share/edje/wearable_poweron.edj
%if %{?ARCH} == arm
/usr/share/keysound/poweron.ogg
%else
/usr/share/keysound/poweron.wav
%endif
%if %{?ARCH} == emulator
%endif
/usr/share/license/%{name}
%{_bindir}/boot-animation
/usr/lib/systemd/system/boot-animation.service
/usr/lib/systemd/system/shutdown-animation.service
/usr/lib/systemd/system/silent-animation.service
/usr/lib/systemd/system/multi-user.target.wants/silent-animation.service
/usr/lib/systemd/system/late-tizen-system.service
/usr/lib/systemd/system/multi-user.target.wants/late-tizen-system.service
/usr/lib/systemd/system/boot-animation.path
/usr/lib/systemd/system/multi-user.target.wants/boot-animation.path
