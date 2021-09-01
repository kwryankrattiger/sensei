source /root/.bashrc

# Install dependencies from the system
dnf update -y --setopt=install_weak_deps=False

# These are required for running sensei tests
dnf install -y --setopt=install_weak_deps=False \
  vim less bc

dnf clean all

# Install main dependencies with spack
source ${SPACK_ROOT}/share/spack/setup-env.sh

spack env create ci

spack -e ci install -v -j$(grep -c '^processor' /proc/cpuinfo) --only dependencies \
  sensei

spack clean -a

echo "echo \"Activating CI Environment\"" >> /root/.bashrc
echo "spack env activate ci" >> /root/.bashrc
