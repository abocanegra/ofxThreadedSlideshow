#pragma once

#include "ofMain.h"
#include "ofxThreadedSlideshow.h"
<<<<<<< HEAD
#include "Poco/XML/XML.h"
//#include "ofxXmlPoco.h"
=======
#include "ofxXmlPoco.h"
>>>>>>> b289fc3948989d49eec67cbe3d8e042d36929171

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    void loadContent();
    void scalePosContent();
    void updateSlideshow();

    int windowW,windowH;
protected:
    ofImage slideImg;
    ofVideoPlayer slideMov;
    ofAs
    ofxThreadedSlideshow slideshow;
    string contentType;
    int imageDuration;
    int modelDuration;
};
