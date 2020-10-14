#pragma once

#include "ofMain.h"

#include "ofxImGui.h"
#include "imgui_stdlib.h"

#include "ofxOpenDHT.h"


#define DHT_NETWORK                         "ofxOpenDHT"
#define DHT_PORT                            4222
#define DHT_BOOTSTRAP_NODE                  "bootstrap.jami.net"

#define RETINA_MIN_WIDTH                    2560
#define RETINA_MIN_HEIGHT                   1600
#define DEBUG_APP                           true

class ofApp : public ofBaseApp{

public:

    void setup();
    void update();
    void draw();
    void exit();

    void keyPressed(int key);
    void keyReleased(int key);

    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);

    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);

    void setupDHTNode();


    std::vector<std::string> arguments;

    ofxImGui::Gui           gui;
    bool                    isRetina;
    float                   scaleFactor;

    ofxOpenDHT              dht;

    dht::InfoHash           room;
    std::future<size_t>     token;
    dht::InfoHash           myid;

    std::map<std::string,std::string>   openChats;
    std::map<std::string,std::string>   participants;

    std::string             userID;
    std::string             aka;

    std::string             participantsList;
    std::string             message;

    bool                    init;
    bool                    initModal;

private:



};
