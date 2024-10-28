#!/bin/bash

########################################################################
#
# openDHT library install & compile script
#
# OpenDHT library github:
# https://github.com/savoirfairelinux/opendht
#
########################################################################


########################################################################
# Check if running with sudo
if [ $(id -u) != 0 ]; then
    echo -e "\nthis installer script must be run with super-user privileges."
    echo -e "\nUsage:\nsudo "$0"\n"
    exit $exit_code
    exit 1
fi

########################################################################
# Install OpenDHT dependencies
sudo apt install build-essential git cmake libncurses5-dev libreadline-dev nettle-dev libgnutls28-dev libargon2-0-dev libmsgpack-dev libssl-dev libfmt-dev libjsoncpp-dev libhttp-parser-dev libasio-dev libcppunit-dev -y

########################################################################
# clone the repo
git clone https://github.com/savoirfairelinux/opendht.git

########################################################################
# build and install
cd opendht
mkdir build && cd build
cmake -DOPENDHT_PYTHON=OFF -DCMAKE_INSTALL_PREFIX=/usr ..
make -j4
sudo make install

########################################################################
# copy lib pkgconfig reference
sudo cp /usr/lib/x86_64-linux-gnu/pkgconfig/opendht.pc /usr/local/lib/pkgconfig/opendht.pc

