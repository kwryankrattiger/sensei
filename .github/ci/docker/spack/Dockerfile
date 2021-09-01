FROM fedora:33

COPY install_spack.sh /root/
RUN SPACK_ROOT=/opt/spack /root/install_spack.sh

COPY install_sensei_deps.sh /root/
COPY sensei-ci-packages.yaml /opt/spack/etc/spack/packages.yaml
RUN /root/install_sensei_deps.sh
