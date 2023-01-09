%define debug_package %{nil}

Name:           demokritos-programs
Version:        1.4
Release:        0
Group:          Productivity/Science/Physics
License:        GPL-3.0
Summary:        Programs made at Demokritos lab
Url:            https://github.com/psaxioti/demokritos-programs

BuildRequires:  gcc-c++, gsl-devel

%description
This package has useful programs made and used at the demokritos Lab

%prep
%setup -q -n %{_sourcedir}/%{name}-%{version} -T -D

%build
g++ -o myrates src/myrates.cpp
g++ -o ratio src/ratio.cpp
g++ -o rutherford_cross_sections src/rutherford_cross_sections.cpp
g++ -o chi_calc src/chi_calc.cpp -lgsl -lgslcblas -lm

%install
install -Dm755 myrates   %{buildroot}%{_bindir}/myrates
install -Dm755 ratio   %{buildroot}%{_bindir}/ratio
install -Dm755 rutherford_cross_sections   %{buildroot}%{_bindir}/rutherford_cross_sections
install -Dm755 chi_calc   %{buildroot}%{_bindir}/chi_calc

%files
%{_bindir}/*