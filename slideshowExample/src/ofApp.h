#pragma once

#include "ofMain.h"
#include "ofxThreadedSlideshow.h"
#include "ofxAssimpModelLoader.h"
#include "ofVboMesh.h"

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
    void drawSlideshow();
    void updateSlideshow();
    void clearSlides();
protected:
    ofImage slideImg;
    ofVideoPlayer slideMov;
    ofxThreadedSlideshow slideshow;
    string contentType;
    float imageDuration;
    float modelDuration;

    float windowW;
    float windowH;

    //3D Model
    ofxAssimpModelLoader slideModel;
    ofLight	light;
    float rotateIter;
    float rat;
};
