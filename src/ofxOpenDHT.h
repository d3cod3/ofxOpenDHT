/*==============================================================================

    ofxOpenDHT: Openframeworks addon for wrapping and include the openDHT
    (https://github.com/savoirfairelinux/opendht) library,
    a C++14 distributed hash table implementation

    ----------------------------------------------------------------------------
    opendht License:

    Copyright (C) 2014-2020 Savoir-faire Linux Inc.

    OpenDHT is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License as published by the Free
    Software Foundation; either version 3 of the License, or (at your option)
    any later version.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
    for more details.

    See COPYING or https://www.gnu.org/licenses/gpl-3.0.en.html for the full
    GPLv3 license.
    ----------------------------------------------------------------------------

    See https://github.com/d3cod3/ofxOpenDHT for documentation

==============================================================================*/

#pragma once

#include "ofMain.h"

#include "opendht_includes.h"

struct dht_params {
    bool generate_identity {false};
    bool daemonize {false};
    bool service {false};
    bool peer_discovery {false};
    bool log {false};
    bool syslog {false};
    std::string logfile {};
    std::string bootstrap {};
    dht::NetId network {0};
    in_port_t port {0};
    in_port_t proxyserver {0};
    in_port_t proxyserverssl {0};
    std::string proxyclient {};
    std::string pushserver {};
    std::string devicekey {};
    std::string persist_path {};
    dht::crypto::Identity id {};
    dht::crypto::Identity proxy_id {};
    std::string privkey_pwd {};
    std::string proxy_privkey_pwd {};
    std::string save_identity {};
    bool no_rate_limit {false};
    bool public_stable {false};
};

struct snode_compare {
    bool operator() (const std::shared_ptr<dht::Node>& lhs, const std::shared_ptr<dht::Node>& rhs) const{
        return (lhs->id < rhs->id) || (lhs->id == rhs->id && lhs->getFamily() == AF_INET && rhs->getFamily() == AF_INET6);
    }
};

static std::mt19937_64 rd {dht::crypto::random_device{}()};
static std::uniform_int_distribution<dht::Value::Id> rand_id;

using NodeSet = std::set<std::shared_ptr<dht::Node>, snode_compare>;

class ofxOpenDHT {

public:

    ofxOpenDHT();
    ~ofxOpenDHT();

    void        setupDHTNode(std::string network="ofxOpenDHT",size_t port=4222,std::string bootstrapNode="bootstrap.jami.net");
    void        stopDHTNode();
    void        scanNetworkNodes();

    void        print_node_info(const dht::NodeInfo& info);
    std::string printTime(const std::time_t& now);

    dht::DhtRunner  dhtNode;
    dht_params      nodeParams;

private:

    void step(dht::DhtRunner& dht, std::atomic_uint& done, std::shared_ptr<NodeSet> all_nodes, dht::InfoHash cur_h, unsigned cur_depth);
    std::pair<dht::DhtRunner::Config, dht::DhtRunner::Context> getDhtConfig(dht_params& params);

    std::condition_variable cv;

};
