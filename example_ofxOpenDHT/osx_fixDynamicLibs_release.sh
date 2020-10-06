#!/bin/bash

########################################################################
#
# Remember to build the OpenDHT library before running this script!!!
#
# OpenDHT build instruction:
# https://github.com/savoirfairelinux/opendht/wiki/Build-the-library
#
########################################################################

SCRIPTPATH="$( cd "$(dirname "$0")" ; pwd -P )"
APPNAME="example_ofxOpenDHT"

# copy the necessary libraries
cp /usr/local/lib/libopendht.2.dylib $SCRIPTPATH/bin/$APPNAME.app/Contents/MacOS/
cp /usr/local/Cellar/libffi/3.3/lib/libffi.7.dylib $SCRIPTPATH/bin/$APPNAME.app/Contents/MacOS/
cp /usr/local/lib/libgmp.10.dylib $SCRIPTPATH/bin/$APPNAME.app/Contents/MacOS/
cp /usr/local/lib/libgnutls.30.dylib $SCRIPTPATH/bin/$APPNAME.app/Contents/MacOS/
cp /usr/local/lib/libhogweed.6.dylib $SCRIPTPATH/bin/$APPNAME.app/Contents/MacOS/
cp /usr/local/lib/libidn2.0.dylib $SCRIPTPATH/bin/$APPNAME.app/Contents/MacOS/
cp /usr/local/lib/libintl.8.dylib $SCRIPTPATH/bin/$APPNAME.app/Contents/MacOS/
cp /usr/local/lib/libnettle.8.dylib $SCRIPTPATH/bin/$APPNAME.app/Contents/MacOS/
cp /usr/local/lib/libp11-kit.0.dylib $SCRIPTPATH/bin/$APPNAME.app/Contents/MacOS/
cp /usr/local/lib/libtasn1.6.dylib $SCRIPTPATH/bin/$APPNAME.app/Contents/MacOS/
cp /usr/local/lib/libunistring.2.dylib $SCRIPTPATH/bin/$APPNAME.app/Contents/MacOS/

# re-link libraries locally
install_name_tool -change @rpath/libopendht.2.dylib @executable_path/libopendht.2.dylib $SCRIPTPATH/bin/$APPNAME.app/Contents/MacOS/$APPNAME

install_name_tool -change /usr/local/opt/gnutls/lib/libgnutls.30.dylib @executable_path/libgnutls.30.dylib $SCRIPTPATH/bin/$APPNAME.app/Contents/MacOS/libopendht.2.dylib
install_name_tool -change /usr/local/opt/nettle/lib/libnettle.8.dylib @executable_path/libnettle.8.dylib $SCRIPTPATH/bin/$APPNAME.app/Contents/MacOS/libopendht.2.dylib

install_name_tool -change /usr/local/opt/p11-kit/lib/libp11-kit.0.dylib @executable_path/libp11-kit.0.dylib $SCRIPTPATH/bin/$APPNAME.app/Contents/MacOS/libgnutls.30.dylib
install_name_tool -change /usr/local/opt/libidn2/lib/libidn2.0.dylib @executable_path/libidn2.0.dylib $SCRIPTPATH/bin/$APPNAME.app/Contents/MacOS/libgnutls.30.dylib

install_name_tool -change /usr/local/opt/libunistring/lib/libunistring.2.dylib @executable_path/libunistring.2.dylib $SCRIPTPATH/bin/$APPNAME.app/Contents/MacOS/libgnutls.30.dylib
install_name_tool -change /usr/local/opt/libtasn1/lib/libtasn1.6.dylib @executable_path/libtasn1.6.dylib $SCRIPTPATH/bin/$APPNAME.app/Contents/MacOS/libgnutls.30.dylib
install_name_tool -change /usr/local/opt/nettle/lib/libnettle.8.dylib @executable_path/libnettle.8.dylib $SCRIPTPATH/bin/$APPNAME.app/Contents/MacOS/libgnutls.30.dylib
install_name_tool -change /usr/local/opt/nettle/lib/libhogweed.6.dylib @executable_path/libhogweed.6.dylib $SCRIPTPATH/bin/$APPNAME.app/Contents/MacOS/libgnutls.30.dylib
install_name_tool -change /usr/local/opt/gmp/lib/libgmp.10.dylib @executable_path/libgmp.10.dylib $SCRIPTPATH/bin/$APPNAME.app/Contents/MacOS/libgnutls.30.dylib
install_name_tool -change /usr/local/opt/gettext/lib/libintl.8.dylib @executable_path/libintl.8.dylib $SCRIPTPATH/bin/$APPNAME.app/Contents/MacOS/libgnutls.30.dylib

install_name_tool -change /usr/local/Cellar/nettle/3.6/lib/libnettle.8.dylib @executable_path/libnettle.8.dylib $SCRIPTPATH/bin/$APPNAME.app/Contents/MacOS/libhogweed.6.dylib
install_name_tool -change /usr/local/opt/gmp/lib/libgmp.10.dylib @executable_path/libgmp.10.dylib $SCRIPTPATH/bin/$APPNAME.app/Contents/MacOS/libhogweed.6.dylib

install_name_tool -change /usr/local/opt/gettext/lib/libintl.8.dylib @executable_path/libintl.8.dylib $SCRIPTPATH/bin/$APPNAME.app/Contents/MacOS/libidn2.0.dylib
install_name_tool -change /usr/local/opt/libunistring/lib/libunistring.2.dylib @executable_path/libunistring.2.dylib $SCRIPTPATH/bin/$APPNAME.app/Contents/MacOS/libidn2.0.dylib

install_name_tool -change /usr/local/opt/libffi/lib/libffi.7.dylib @executable_path/libffi.7.dylib $SCRIPTPATH/bin/$APPNAME.app/Contents/MacOS/libp11-kit.0.dylib
