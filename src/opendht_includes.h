#pragma once

#ifdef TARGET_OSX
#define MSGPACK_DISABLE_LEGACY_NIL
#endif

#include <opendht.h>
#include <opendht/log.h>
#include <opendht/crypto.h>
#include <opendht/network_utils.h>
#include <opendht/node.h>
#include <opendht/rng.h>

extern "C" {
#include <gnutls/gnutls.h>
}

#ifdef OPENDHT_INDEXATION
#include <opendht/indexation/pht.h>
#endif
#ifdef OPENDHT_PROXY_SERVER
#include <opendht/dht_proxy_server.h>
#endif

#include <fmt/ranges.h>

#ifndef _MSC_VER
#include <getopt.h>
#include <readline/readline.h>
#include <readline/history.h>
#else
#define SIGHUP 0
#include "wingetopt.h"
#include <io.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>

#include <string>
#include <vector>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>
