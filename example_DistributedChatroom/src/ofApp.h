#pragma once

#include "ofMain.h"

#include "ofxImGui.h"
#include "imgui_stdlib.h"

#include "ofxOpenDHT.h"

#define RETINA_MIN_WIDTH                    2560
#define RETINA_MIN_HEIGHT                   1600

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


    ofxImGui::Gui           gui;
    bool                    isRetina;
    float                   scaleFactor;


    ofxOpenDHT              dht;

    dht::InfoHash           room;
    std::future<size_t>     token;

    dht::InfoHash           myid;

    std::string             chatContent;
    std::string             message;

    bool                    init;

};
