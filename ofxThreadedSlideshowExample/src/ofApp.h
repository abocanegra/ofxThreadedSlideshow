#pragma once
#ifndef _EXAMPLE_OFXTHREADEDSLIDESHOW
#define _EXAMPLE_OFXTHREADEDSLIDESHOW

#include "ofMain.h"
#include "ofxThreadedSlideshow.h"
#include "ofVboMesh.h"
#include "ofxAssimpModelLoader.h"
#include "ofxXmlPoco.h"


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
    void drawSlideshow();
    void clearSlides();

    ofImage texture;
    int windowW,windowH;
protected:
    ofImage                 slideImg;
    ofVideoPlayer		    slideMov;
    ofxAssimpModelLoader	slideModel;
    ofxThreadedSlideshow	slideshow;
    ofLight			        light;
    string			        contentType;
    int imageDuration, modelDuration;
    float rotateIter, rat;
};
#endif
