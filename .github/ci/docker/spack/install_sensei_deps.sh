set -e
set -x

source /root/.bashrc

# These are required for running sensei tests
dnf install -y --setopt=install_weak_deps=False \
  vim less bc

# Install main dependencies with spack
source ${SPACK_ROOT}/share/spack/setup-env.sh

spack env create ci

spack -e ci install -v -j$(grep -c '^processor' /proc/cpuinfo) --only dependencies \
  sensei

spack clean -a

echo "echo \"Activating CI Environment\"" >> /root/.bashrc
echo "spack env activate ci" >> /root/.bashrc
