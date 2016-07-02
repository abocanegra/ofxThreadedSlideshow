#include "ofApp.h"
#include "ofxThreadedSlideshow.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(false);
    ofSetFrameRate(30);
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofBackground(ofColor(0,0,0,255));

    //Setup Slideshow and start thread
    slideshow.folder = "images/slideshow";
    slideshow.width = ofGetWindowWidth();
    slideshow.height = ofGetWindowHeight();
    slideshow.duration = 10;
    slideshow.fade = true;
    slideshow.showInfo = true;
    slideshow.center = true;
    slideshow.scale = true;
    slideshow.start();

    //Allocate Slide
    slide.allocate(slideshow.width,slideshow.height,OF_IMAGE_COLOR_ALPHA);

}

//--------------------------------------------------------------
void ofApp::update(){
    std::stringstream strm;
    strm << ofToString( roundf(ofGetFrameRate()))+ "fps | " + slideshow.getCurrentImageName() ;
    ofSetWindowTitle(strm.str());

    if(slideshow.isThreadRunning() && slideshow.isNewFrame){
       slide.load(slideshow.currentImage());
       slideshow.setPosSize(slide.getWidth(), slide.getHeight());
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255,slideshow.fadeAlpha);
    slide.draw(slideshow.getPos(), slideshow.scaledWidth, slideshow.scaledHeight);
    ofSetColor(255);
    slideshow.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'h'){
        slideshow.showInfo ^= true;
    }

    //Pause Slideshow
    if(key == ' ' ){
        if(slideshow.isThreadRunning()){
            slideshow.stop();
        }else{
            slideshow.start();
        }
    }

    if(key == 's'){
        slideshow.stretch ^= true;
        if(slideshow.slideshowInit){
           slide.load(slideshow.currentImage());
           slideshow.setPosSize(slide.getWidth(), slide.getHeight());
        }
    }

    if(key == '+'){
        slideshow.scale ^= true;
        if(slideshow.slideshowInit){
           slide.load(slideshow.currentImage());
           slideshow.setPosSize(slide.getWidth(), slide.getHeight());
        }
    }

    if(key == 'c'){
        slideshow.center ^= true;
        if(slideshow.slideshowInit){
           slide.load(slideshow.currentImage());
           slideshow.setPosSize(slide.getWidth(), slide.getHeight());
        }
    }
    if(key == OF_KEY_RIGHT){
        if(slideshow.slideshowInit){
            slideshow.fadeAlpha = 255;
           slide.load(slideshow.currentImage());
           slideshow.setPosSize(slide.getWidth(), slide.getHeight());
        }
        slideshow.stop();
    }

    if(key == OF_KEY_LEFT){
        if(slideshow.slideshowInit){
            slideshow.fadeAlpha = 255;
           slide.load(slideshow.previousImage());
           slideshow.setPosSize(slide.getWidth(), slide.getHeight());
        }
        slideshow.stop();
    }
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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    slideshow.width = w;
    slideshow.height = h;
    slideshow.fontY = ofGetWindowHeight() - 20;

    if(slideshow.slideshowInit){
       slide.load(slideshow.currentImage());
       slideshow.setPosSize(slide.getWidth(), slide.getHeight());
    }
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
