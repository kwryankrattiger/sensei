FROM fedora:33

# Start installing stuff
RUN dnf update -y --setopt=install_weak_deps=False

COPY install_spack.sh /root/
RUN SPACK_ROOT=/opt/spack /root/install_spack.sh

COPY install_sensei_deps.sh /root/
COPY sensei-ci-packages.yaml /opt/spack/etc/spack/packages.yaml
RUN /root/install_sensei_deps.sh

# Clean up installing stuff
RUN dnf clean all
