#include "ofMain.h"
#include "ofAppGLFWWindow.h"

#include "ofApp.h"


//========================================================================
int main(int argc, char *argv[]){
    std::vector<std::string> options;
    if(argc > 1){
        for(int i = 0; i < argc; i++){
            options.push_back(argv[i]);
        }
    }

    ofGLFWWindowSettings settings;
    settings.setGLVersion(2, 1);
    settings.setSize(1280, 720);
    settings.setPosition(ofVec2f(0,0));
    settings.resizable = true;
    settings.decorated = true;

    // main window
    std::shared_ptr<ofAppGLFWWindow> mainWindow = dynamic_pointer_cast<ofAppGLFWWindow>(ofCreateWindow(settings));
    std::shared_ptr<ofApp> mainApp(new ofApp);

    mainApp->arguments = options;

    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();

    // done
    return EXIT_SUCCESS;

}
