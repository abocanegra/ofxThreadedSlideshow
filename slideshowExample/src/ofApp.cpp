#include "ofApp.h"
#include "ofxThreadedSlideshow.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(false);
    ofSetFrameRate(30);
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofBackground(ofColor(0,0,0,255));

    imageDuration = 10;
    modelDuration = 60;

    //Setup Slideshow and start thread
    slideshow.folder = "images/slideshow";
    slideshow.width = ofGetWindowWidth();
    slideshow.height = ofGetWindowHeight();
    slideshow.duration = imageDuration;
    slideshow.fade = true;
    slideshow.showInfo = true;
    slideshow.center = true;
    slideshow.scale = true;
    slideshow.start();

    //Allocate Slide
    slideImg.allocate(slideshow.width,slideshow.height,OF_IMAGE_COLOR_ALPHA);
    slideMov.setPixelFormat(OF_PIXELS_RGBA);

}

//--------------------------------------------------------------
void ofApp::update(){
    std::stringstream strm;
    strm << ofToString( roundf(ofGetFrameRate()))+ "fps | " + slideshow.getCurrentContentName() ;
    ofSetWindowTitle(strm.str());

    if(slideshow.isThreadRunning() && slideshow.isNewFrame){
        slideshow.nextContent();
        loadContent();
    }
    if(slideshow.getContentType() == "video"){
        slideMov.update();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255,slideshow.fadeAlpha);
    if(contentType == "image"){
        slideImg.draw(slideshow.getPos(), slideshow.scaledWidth, slideshow.scaledHeight);
    }else if(contentType == "video"){
        slideMov.draw(slideshow.getPos(), slideshow.scaledWidth, slideshow.scaledHeight);
    }else if(contentType == "3dmodel"){

    }
    ofSetColor(255);
    slideshow.draw();
}

void ofApp::loadContent(){
    contentType = slideshow.getContentType();
    slideMov.closeMovie();
    if(contentType == "image"){
        slideshow.duration = imageDuration;
        slideImg.load(slideshow.currentContent());
        slideshow.setPosSize(slideImg.getWidth(), slideImg.getHeight());
    }else if(contentType == "video"){
        slideMov.load(slideshow.currentContent());
        slideshow.setPosSize(slideMov.getWidth(), slideMov.getHeight());
        slideshow.duration = slideMov.getDuration();
        slideMov.setLoopState(OF_LOOP_NONE);
        slideMov.play();
    }else if(contentType == "3dmodel"){
        slideshow.duration = modelDuration;

    }else if(contentType == "unkown format"){

    }
}

void ofApp::scalePosContent(){
    if(contentType == "image"){
        slideshow.setPosSize(slideImg.getWidth(), slideImg.getHeight());
    }else if(contentType == "video"){
        slideshow.setPosSize(slideMov.getWidth(), slideMov.getHeight());
    }else if(contentType == "3dmodel"){

    }
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
        if(slideshow.slideshowInit && slideshow.stretch){
            slideshow.setPosSize(slideImg.getWidth(), slideImg.getHeight());
            slideshow.setPosSize(slideImg.getWidth(), slideImg.getHeight());
        }else if (slideshow.slideshowInit && ! slideshow.stretch){
            scalePosContent();
        }
    }

    if(key == '+'){
        slideshow.scale ^= true;
        if(slideshow.slideshowInit){
            scalePosContent();
        }
    }

    if(key == 'c'){
        slideshow.center ^= true;
        if(slideshow.slideshowInit){
            scalePosContent();
        }
    }
    if(key == OF_KEY_RIGHT){
        if(slideshow.slideshowInit){
            slideshow.fadeAlpha = 255;
            slideshow.nextContent();
            loadContent();
        }
    }

    if(key == OF_KEY_LEFT){
        if(slideshow.slideshowInit){
            slideshow.fadeAlpha = 255;
            slideshow.previousContent();
            loadContent();
        }
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
        scalePosContent();
    }
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
