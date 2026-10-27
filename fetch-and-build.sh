#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 <libyang-version> <sysrepo-version>"
    exit 1
fi

pushd /root

wget https://github.com/CESNET/libyang/archive/refs/tags/v$1.tar.gz
tar -xf v$1.tar.gz
cd libyang-$1
mkdir build; cd build
cmake ..
make
make install
cd ../..

wget https://github.com/sysrepo/sysrepo/archive/refs/tags/v$2.tar.gz
tar -xf v$2.tar.gz
cd sysrepo-$2
mkdir build; cd build
cmake ..
make
make install

popd

ldconfig
