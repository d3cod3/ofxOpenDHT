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
        context.statusChangedCallback = [logger = *context.logger](dht::NodeStatus status4, dht::NodeStatus status6) {
            logger.WARN("Connectivity changed: IPv4: %s, IPv6: %s", dht::statusToStr(status4), dht::statusToStr(status6));
        };
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
