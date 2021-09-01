FROM fedora:33

COPY *.sh /root/

RUN SPACK_ROOT=/opt/spack /root/install_spack.sh

COPY sensei-ci-packages.yaml $SPACK_ROOT/etc/spack/packages.yaml
RUN /root/install_sensei_deps.sh

ENTRYPOINT source /root/.bashrc
CMD source /root/.bashrc
