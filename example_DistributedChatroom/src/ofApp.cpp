#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    setupDHTNode();

    scaleFactor     = 1.0f;

    gui.setup(nullptr, false);

    init        = false;
    initModal   = false;

}

//--------------------------------------------------------------
void ofApp::setupDHTNode(){

    dht.setupDHTNode(DHT_NETWORK,DHT_PORT,DHT_BOOTSTRAP_NODE);

    myid = dht.dhtNode.getId();

    userID = myid.toString();
    aka = "";

    chatname = "ofxOpenDHT chatroom";

    room = dht::InfoHash::get(chatname);

    welcome_message = "\n\n";
    welcome_message +=" ______________\n";
    welcome_message +="< DHT CHATROOM >\n";
    welcome_message +=" --------------\n";
    welcome_message +="        \\   ^__^\n";
    welcome_message +="         \\  (OO)\\_______\n";
    welcome_message +="            (__)\\       )\\/\\\n";
    welcome_message +="                ||----w |\n";
    welcome_message +="                ||     ||\n\n\n";

    openChats[chatname] = welcome_message;


    if(DEBUG_APP) std::cout << "Joining h(" << chatname << ") = " << room << std::endl;

    // node running thread
    token = dht.dhtNode.listen<dht::ImMessage>(room, [&](dht::ImMessage&& msg) {
            if (msg.from != myid){

                // store current session participants list
                std::map<string,string>::iterator it = participants.find(msg.from.toString());
                if (it == participants.end()){
                    unsigned first = msg.msg.find("<");
                    unsigned last = msg.msg.find_last_of(">");
                    string newAka = msg.msg.substr(first+1,last-first-1);
                    participants[msg.from.toString()] = newAka;
                }

                // debug log
                if(DEBUG_APP){
                    std::cout << msg.from.toString() << " at " << dht.printTime(msg.date)
                              << " (took " << dht::print_dt(std::chrono::system_clock::now() - std::chrono::system_clock::from_time_t(msg.date))
                              << "s) " << ": " << msg.id << " - " << msg.msg << std::endl;
                }

                if(msg.to == myid){ // encrypted ( private message )
                    openChats[msg.from.toString()] += "["+ofGetTimestampString("%H-%M-%S")+"] " + msg.msg;
                    openChats[msg.from.toString()] += "\n";
                }else{
                    // update chatroom messages
                    openChats[chatname] += "["+ofGetTimestampString("%H-%M-%S")+"] " + msg.msg;
                    openChats[chatname] += "\n";
                }

            }
            return true;
    });

}

//--------------------------------------------------------------
void ofApp::update(){

    participantsList = "";
    for(std::map<std::string,std::string>::iterator it = participants.begin(); it != participants.end(); it++ ){
        participantsList += it->second;
        participantsList += "\n";
    }

    if(!init){
        init = true;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

    // create gui
    this->gui.begin();
    {

        ImGui::SetNextWindowPos(ImVec2(0,0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(ofGetWindowWidth()*scaleFactor,ofGetWindowHeight()*scaleFactor), ImGuiCond_Always);

        if (ImGui::Begin(chatname.c_str()))
        {

            // startup modal for choosing session username
            if(!initModal){
                initModal = true;
                ImGui::OpenPopup("Set user AKA for this chatroom session");
            }

            bool open = true;
            ImGui::SetNextWindowPos(ImVec2(ofGetWindowWidth()/2 - 150*scaleFactor,ofGetWindowHeight()/2 - 50*scaleFactor), ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(300*scaleFactor,100*scaleFactor), ImGuiCond_Always);

            if (ImGui::BeginPopupModal("Set user AKA for this chatroom session", &open)){
                ImGui::InputText("###aka",&aka);
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                if (ImGui::Button("Apply")){
                    if(aka != "" && checkAKAIsValid(aka)){ // if aka is not empty and is not already taken
                       participants[userID] = aka;
                    }else{
                       participants[userID] = userID;
                       aka = userID;
                    }
                    ImGui::CloseCurrentPopup();
                }
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::EndPopup();
            }

            // main GUI
            static int selected = 0;
            int counter = 0;
            ImGui::BeginChild("participants", ImVec2(ofGetWindowWidth()/5, ofGetWindowHeight()-34*scaleFactor), true);
            for(std::map<std::string,std::string>::iterator it = participants.begin(); it != participants.end(); it++ ){
                if (ImGui::Selectable(it->second.c_str(), selected == counter)){
                    selected = counter;
                }
                if (ImGui::BeginPopupContextItem()){
                    ImGui::Text("%s",it->first.c_str());
                    ImGui::Spacing();
                    ImGui::Text("%s",it->second.c_str());
                    if(openChats.find(it->first) == openChats.end() && it->first != userID){
                        ImGui::Spacing();
                        ImGui::Separator();
                        ImGui::Spacing();
                        if(ImGui::Button("New Private Chat",ImVec2(200*scaleFactor,26*scaleFactor))){
                            openChats[it->first] = "";
                            ImGui::CloseCurrentPopup();
                        }
                    }

                    ImGui::EndPopup();
                }
                counter++;
            }
            ImGui::EndChild();

            ImGui::SameLine();

            ImGui::BeginChild("chat view", ImVec2(ofGetWindowWidth()/5*4, ofGetWindowHeight()-34*scaleFactor));
            if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_Reorderable|ImGuiTabBarFlags_AutoSelectNewTabs|ImGuiTabBarFlags_TabListPopupButton )){

                //static bool opened[4] = { true, true, true, true }; // Persistent user state

                for(std::map<std::string,std::string>::iterator it = openChats.begin(); it != openChats.end(); it++ ){
                    string tabName = "";
                    if(it->first == chatname.c_str()){
                        tabName = it->first;
                    }else{
                        tabName = participants[it->first];
                    }
                    //if (opened[n] && ImGui::BeginTabItem(names[n], &opened[n], ImGuiTabItemFlags_NoTooltip))
                    if (ImGui::BeginTabItem(tabName.c_str())){
                        string tempLabel = "###chat_"+it->first;
                        ImGui::InputTextMultiline(tempLabel.c_str(),&it->second,ImVec2(ofGetWindowWidth()/5*4, -30));

                        ImGui::Spacing();
                        ImGui::Separator();
                        ImGui::Spacing();

                        ImGui::PushItemWidth(-80);
                        ImGui::InputText("###message",&message);
                        ImGui::PopItemWidth();

                        ImGui::SameLine();

                        if(ImGui::Button("SEND")){
                            auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

                            if(it->first == chatname.c_str()){
                                it->second += "["+ofGetTimestampString("%H-%M-%S")+"] <" + aka + "> " +message;
                                it->second += "\n";

                                dht.dhtNode.putSigned(room, dht::ImMessage(rand_id(rd), std::move("<"+aka+"> "+message), now));
                            }else{
                                it->second += "["+ofGetTimestampString("%H-%M-%S")+"] " +message;
                                it->second += "\n";

                                dht.dhtNode.putEncrypted(room, dht::InfoHash(it->first), dht::ImMessage(rand_id(rd), std::move("<"+aka+"> "+message), now));
                            }


                            message = "";
                        }


                        ImGui::EndTabItem();
                    }
                }
                ImGui::EndTabBar();
            }
            ImGui::EndChild();

        }
        ImGui::End();
    }
    this->gui.end();


    // draw gui
    this->gui.draw();
}

//--------------------------------------------------------------
void ofApp::exit(){
    dht.dhtNode.cancelListen(room, std::move(token));
    dht.stopDHTNode();
}

//--------------------------------------------------------------
bool ofApp::checkAKAIsValid(std::string aka){
     for(std::map<std::string,std::string>::iterator it = participants.begin(); it != participants.end(); it++ ){
         if(it->second == aka){
             return false;
         }
     }
     return true;
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
