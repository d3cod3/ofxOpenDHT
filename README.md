# ofxOpenDHT

Openframeworks addon for wrapping and include the [openDHT](https://github.com/savoirfairelinux/opendht) library, a C++17 distributed hash table implementation

## Compile

Before using this addon, you'll need to install opendht dependencies and build the library. Just follow [this build instructions](https://github.com/savoirfairelinux/opendht/wiki/Build-the-library), when finished you'll be ready to use the ofxOpenDHT addon.

### macOS ISSUES

In some macOS systems, the install of opendht library will fail due to a security feature that block coping files from user space to system folders.
In order to solve the issue just clone the opendht repo in a folder outside the user space and make/make install from there.

To check if you are working from user space, just try this in a terminal, from the folder where you cloned opendht for compiling ( thanks [@daandelange](https://github.com/Daandelange) ):

```bash
echo `pwd` | grep -Eq "^`realpath ~/../`" && echo "You are running from user space, newer versions of macos (±10.15+) will probably fail to install (below). Please copy the compilation files out of your user folder."
```

## OpenDHT License

Copyright (C) 2014-2024 Savoir-faire Linux Inc.

OpenDHT is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

See COPYING or [https://www.gnu.org/licenses/gpl-3.0.en.html](https://www.gnu.org/licenses/gpl-3.0.en.html) for the full GPLv3 license.
