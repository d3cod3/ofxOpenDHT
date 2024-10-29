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

#include "ofxOpenDHT.h"

//--------------------------------------------------------------
ofxOpenDHT::ofxOpenDHT(){

}

//--------------------------------------------------------------
ofxOpenDHT::~ofxOpenDHT(){

}

//--------------------------------------------------------------
void ofxOpenDHT::setupDHTNode(std::string network,size_t port,std::string bootstrapNode) {
    nodeParams.generate_identity = true;
    nodeParams.network = strtoul(network.c_str(), nullptr, 0);
    nodeParams.port = port;
    nodeParams.bootstrap = bootstrapNode+":"+ofToString(port);
    auto dhtConf = getDhtConfig(nodeParams);

    dhtNode.run(nodeParams.port, dhtConf.first, std::move(dhtConf.second));

    if (!nodeParams.bootstrap.empty()){
        dhtNode.bootstrap(nodeParams.bootstrap);
    }

    print_node_info(dhtNode.getNodeInfo());
}

//--------------------------------------------------------------
void ofxOpenDHT::stopDHTNode(){
    if(dhtNode.isRunning()){
        // stop the node
        dhtNode.join();
    }
}

//--------------------------------------------------------------
void ofxOpenDHT::scanNetworkNodes(){
    std::cout << "Scanning network..." << std::endl;
    auto all_nodes = std::make_shared<NodeSet>();

    // Set hash to 1 because 0 is the null hash
    dht::InfoHash cur_h {};
    cur_h.setBit(8*HASH_LEN-1, 1);

    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::atomic_uint done {0};
    step(dhtNode, done, all_nodes, cur_h, 0);

    {
        std::mutex m;
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, [&](){
            return done.load() == 0;
        });
    }

    std::cout << std::endl << "Scan ended: " << all_nodes->size() << " nodes found." << std::endl;
    for (const auto& n : *all_nodes){
        std::cout << "Node " << *n << std::endl;
    }
}

//--------------------------------------------------------------

void ofxOpenDHT::step(dht::DhtRunner& dht, std::atomic_uint& done, std::shared_ptr<NodeSet> all_nodes, dht::InfoHash cur_h, unsigned cur_depth)
{
    std::cout << "step at " << cur_h << ", depth " << cur_depth << std::endl;
    done++;
    dht.get(cur_h, [all_nodes](const std::vector<std::shared_ptr<dht::Value>>& /*values*/) {
        return true;
    }, [&,all_nodes,cur_h,cur_depth](bool, const std::vector<std::shared_ptr<dht::Node>>& nodes) {
        all_nodes->insert(nodes.begin(), nodes.end());
        NodeSet sbuck {nodes.begin(), nodes.end()};
        if (not sbuck.empty()) {
            unsigned bdepth = sbuck.size()==1 ? 0u : dht::InfoHash::commonBits((*sbuck.begin())->id, (*sbuck.rbegin())->id);
            unsigned target_depth = std::min(8u, bdepth+6u);
            std::cout << cur_h << " : " << nodes.size() << " nodes; target is " << target_depth << " bits deep (cur " << cur_depth << ")" << std::endl;
            for (unsigned b = cur_depth ; b < target_depth; b++) {
                auto new_h = cur_h;
                new_h.setBit(b, 1);
                step(dht, done, all_nodes, new_h, b+1);
            }
        }
        done--;
        std::cout << done.load() << " operations left, " << all_nodes->size() << " nodes found." << std::endl;
        cv.notify_one();
    });
}

//--------------------------------------------------------------
std::pair<dht::DhtRunner::Config, dht::DhtRunner::Context> ofxOpenDHT::getDhtConfig(dht_params& params){
    if (not params.id.first and params.generate_identity) {
        auto node_ca = std::make_unique<dht::crypto::Identity>(dht::crypto::generateEcIdentity("DHT Node CA"));
        params.id = dht::crypto::generateIdentity("DHT Node", *node_ca);
        if (not params.save_identity.empty()) {
            dht::crypto::saveIdentity(*node_ca, params.save_identity + "_ca", params.privkey_pwd);
            dht::crypto::saveIdentity(params.id, params.save_identity, params.privkey_pwd);
        }
    }

    dht::DhtRunner::Config config {};
    config.dht_config.node_config.network = params.network;
    config.dht_config.node_config.maintain_storage = false;
    config.dht_config.node_config.persist_path = params.persist_path;
    config.dht_config.node_config.public_stable = params.public_stable;
    config.dht_config.id = params.id;
    config.dht_config.cert_cache_all = static_cast<bool>(params.id.first);
    config.threaded = true;
    config.proxy_server = params.proxyclient;
    config.push_node_id = "dhtnode";
    config.push_token = params.devicekey;
    config.peer_discovery = params.peer_discovery;
    config.peer_publish = params.peer_discovery;
    if (params.no_rate_limit) {
        config.dht_config.node_config.max_req_per_sec = -1;
        config.dht_config.node_config.max_peer_req_per_sec = -1;
        config.dht_config.node_config.max_searches = -1;
        config.dht_config.node_config.max_store_size = -1;
    }

    dht::DhtRunner::Context context {};
    if (params.log) {
        if (params.syslog or (params.daemonize and params.logfile.empty()))
            context.logger = dht::log::getSyslogLogger("dhtnode");
        else if (not params.logfile.empty())
            context.logger = dht::log::getFileLogger(params.logfile);
        else
            context.logger = dht::log::getStdLogger();
    }
    if (context.logger) {
        context.statusChangedCallback = [logger = context.logger](dht::NodeStatus status4, dht::NodeStatus status6) {
            logger->w("Connectivity changed: IPv4: %s, IPv6: %s", dht::statusToStr(status4), dht::statusToStr(status6));
        };
        /*context.publicAddressChangedCb = [logger = context.logger](std::vector<dht::SockAddr> addrs) {
            logger->warn("Public address changed: {}", addrs);
        };*/
    }
    return {std::move(config), std::move(context)};
}

//--------------------------------------------------------------
void ofxOpenDHT::print_node_info(const dht::NodeInfo& info) {
    std::cout << "OpenDHT node " << info.node_id << " running on ";
    if (info.bound4 == info.bound6)
        std::cout << "port " << info.bound4 << std::endl;
    else
        std::cout << "IPv4 port " << info.bound4 << ", IPv6 port " << info.bound6 << std::endl;
    if (info.id)
        std::cout << "Public key ID " << info.id << std::endl;
}

//--------------------------------------------------------------
std::string ofxOpenDHT::printTime(const std::time_t& now){
    struct tm tstruct = *localtime(&now);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    return buf;
}
