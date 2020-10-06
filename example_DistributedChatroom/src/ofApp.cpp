#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    setupDHTNode();

    isRetina        = false;
    scaleFactor     = 1.0f;

    gui.setup();

    init = false;

}

//--------------------------------------------------------------
void ofApp::setupDHTNode(){

    dht.setupDHTNode("ofxOpenDHT",4222,"bootstrap.jami.net");

    myid = dht.dhtNode.getId();

    room = dht::InfoHash::get("ofxOpenDHT chatroom");
    std::cout << "Joining h(ofxOpenDHT chatroom) = " << room << std::endl;

    token = dht.dhtNode.listen<dht::ImMessage>(room, [&](dht::ImMessage&& msg) {
            //if (msg.from != myid)
            std::cout << msg.from.toString() << " at " << dht.printTime(msg.date)
                      << " (took " << dht::print_dt(std::chrono::system_clock::now() - std::chrono::system_clock::from_time_t(msg.date))
                      << "s) " << (msg.to == myid ? "ENCRYPTED ":"") << ": " << msg.id << " - " << msg.msg << std::endl;
            chatContent += msg.msg;
            chatContent += "\n";
            return true;
    });

}

//--------------------------------------------------------------
void ofApp::update(){

    if(!init){
        init = true;
        // RETINA FIX
        if(ofGetScreenWidth() >= RETINA_MIN_WIDTH && ofGetScreenHeight() >= RETINA_MIN_HEIGHT){
            isRetina        = true;
            scaleFactor     = 2.0f;
            ofSetWindowShape(1024*scaleFactor,768*scaleFactor);
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    this->gui.begin();
    {

        if (ImGui::Begin("ofxOpenDHT Chatroom"))
        {

            ImGui::SetWindowPos(ImVec2(0,0), ImGuiCond_Always);
            ImGui::SetWindowSize(ImVec2(1024*scaleFactor,768*scaleFactor), ImGuiCond_Always);

            ImGui::InputTextMultiline("###chatroom",&chatContent,ImVec2(-1, 500*scaleFactor));

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            ImGui::InputText("###message",&message);
            ImGui::SameLine();
            if(ImGui::Button("SEND")){
                auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                dht.dhtNode.putSigned(room, dht::ImMessage(rand_id(rd), std::move(message), now));

                message = "";
            }


        }
        ImGui::End();
    }
    this->gui.end();
}

//--------------------------------------------------------------
void ofApp::exit(){
    dht.dhtNode.cancelListen(room, std::move(token));
    dht.stopDHTNode();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
