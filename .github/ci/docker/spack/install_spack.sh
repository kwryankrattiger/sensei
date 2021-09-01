set -e
set -x

# Install Spack requirements
dnf install -y --setopt=install_weak_deps=False \
  binutils \
  bzip2 \
  diffutils \
  file \
  findutils \
  git \
  gcc \
  g++ \
  gfortran \
  gzip \
  patch \
  python \
  pip \
  procps-ng \
  tar \
  unzip \
  which \
  xz \
  zstd

# Install clingo for new concretizer
pip install clingo

if [[ -z ${SPACK_ROOT} ]]; then
  if [[ ! -z $1 ]]; then
    SPACK_ROOT=$1
  else
    SPACK_ROOT=/opt/spack
  fi
fi

mkdir -p ${SPACK_ROOT}

# Use spack@develop
git clone https://github.com/spack/spack.git ${SPACK_ROOT}
cd ${SPACK_ROOT}
git checkout develop

echo "echo \"Activating Spack Environment\"" >> /root/.bashrc
echo "source ${SPACK_ROOT}/share/spack/setup-env.sh" >> /root/.bashrc
echo "export PATH=\${PATH}:/opt/spack/bin/" >> /root/.bashrc

source /root/.bashrc

rm -rf /root/.spack
spack compiler find --scope site

