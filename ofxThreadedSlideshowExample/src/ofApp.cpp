#include "ofApp.h"
#include "ofxThreadedSlideshow.h"

/*
 * ofxThreadedSlideshow Example
*
 * A threaded slideshow capable of playing images, video, and 3d models from
 * a folder or via local or remote XML file. Handles local or remote content, and
 * streaming video as well as IP video
 *
 * Creative Commons Attribution-ShareAlike 4.0 International License
 * Originally developed by Aaron Bocanegra @ Nontrivial Studio
 * 2016
 */

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
    windowW = ofGetWidth();
    windowH = ofGetHeight();
    //Setup Slideshow and start thread
    slideshow.mode = 1;
    slideshow.folder = "images/slideshow";
    slideshow.width = windowW;
    slideshow.height = windowH;
    slideshow.duration = imageDuration;
    slideshow.showInfo = true;
    slideshow.center = true;
    slideshow.scale = true;
    slideshow.start();

    //Allocate Slide
    slideImg.allocate(slideshow.width,slideshow.height,OF_IMAGE_COLOR_ALPHA);
    slideMov.setPixelFormat(OF_PIXELS_RGBA);
    //Model
    ofDisableArbTex(); // we need GL_TEXTURE_2D for our models coords.

}

//--------------------------------------------------------------
void ofApp::update(){
    std::stringstream strm;
    if(slideshow.isInitiated()){
        strm << ofToString( roundf(ofGetFrameRate()))+ "fps | " + slideshow.getCurrentContentName() ;
    }else{
        strm << "Slideshow Not Loaded";
    }
    ofSetWindowTitle(strm.str());

    if(slideshow.isThreadRunning() && slideshow.isNewFrame){
        slideshow.nextContent();
        loadContent();
    }
    if(slideshow.getContentType() == "video"){
        slideMov.update();
    }


    updateSlideshow();
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
    drawSlideshow();
}

//--------------------------------------------------------------
void ofApp::loadContent(){
    contentType = slideshow.getContentType();
    clearSlides();

    //------------------------- Image
    if(contentType == "image"){

        if(slideshow.mode == 0){
            slideshow.duration = imageDuration;
        }else if(slideshow.mode ==1){
            slideshow.setXmlDuration();
        }

        //If connected to internet
        if(!slideshow.netAccess && ofIsStringInString(slideshow.currentContent(), "http")){
            slideshow.nextContent();
            loadContent();
            return;
        }

        int i = 0;
        int sleepyTime = 100;
        //LOAD CONTENT
        slideImg.load(slideshow.currentContent());
        while(!slideImg.isAllocated() && slideImg.getWidth()<=0){
            ofSleepMillis(sleepyTime);
            slideshow.restartTimer();
            if(i >= sleepyTime){
                slideshow.internetAccess();
                slideshow.nextContent();
                loadContent();
                return;
            }
            i++;
        }
        //------------------------------Video
    }else if(contentType == "video"){
        slideMov.setLoopState(OF_LOOP_NONE);
        //If connected to internet
        if(!slideshow.netAccess && (ofIsStringInString(slideshow.currentContent(), "http") || ofIsStringInString(slideshow.currentContent(), "rtsp"))){
            slideshow.nextContent();
            loadContent();
            return;
        }
        int i = 0;
        int sleepyTime = 100;
        //LOAD CONTENT
        slideMov.load(slideshow.currentContent());
        while(!slideMov.isLoaded()){
            ofSleepMillis(sleepyTime);
            slideshow.restartTimer();
            if(i >= sleepyTime *5 ){
                slideshow.internetAccess();
                slideshow.nextContent();
                loadContent();
                return;
            }
            i++;
        }
        int j= 0;
        int sleepyTimej= 100;
        while(slideMov.getWidth() <=0 &&
              slideMov.getCurrentFrame() <0 &&
              !slideMov.isFrameNew()
              ){
            ofSleepMillis(sleepyTimej);
            slideshow.restartTimer();
            if(j >= sleepyTimej){
                slideshow.nextContent();
                loadContent();
                return;
            }
            j++;
        }

        if(slideshow.mode == 0 ){
            if(slideMov.getDuration() > 0){
                slideshow.duration = slideMov.getDuration();
            }else{
                slideshow.duration = 60;
            }
        }

        if(slideshow.mode == 1){
            if(slideshow.getXmlDuration() == 0 &&
                    slideshow.getVideoMode() == "file"){
                int i = 0;
                int sleepyTime = 100;
                while(slideMov.getDuration() <= 0){
                    ofSleepMillis(sleepyTime);
                    slideshow.duration = slideMov.getDuration();
                    slideshow.restartTimer();
                    slideshow.setPosSize(slideMov.getWidth(), slideMov.getHeight());
                    if(i >= sleepyTime){
                        slideshow.internetAccess();
                        slideshow.nextContent();
                        loadContent();
                        return;
                    }
                    i++;
                }
            }else{
                slideshow.setXmlDuration();
                slideshow.restartTimer();
            }
        }

        if(slideMov.getDuration() >0){
            slideshow.duration = slideMov.getDuration();
        }

        int k = 0;
        int sleepyTimek = 100;
        while(slideMov.getWidth()<=0){
            slideshow.restartTimer();
            if(k>= sleepyTimek){
                slideshow.nextContent();
                loadContent();
                return;
            }
            k++;
        }
        slideshow.setPosSize(slideMov.getWidth(), slideMov.getHeight());
        slideMov.play();
        slideshow.restartTimer();
        while(!slideMov.isPlaying()){
            slideshow.restartTimer();
        }
        //-------------------------------3D Model
    }else if(contentType == "3dmodel"){
        rotateIter = 0.0f;
        if(slideshow.mode == 0){
            slideshow.duration = modelDuration;
        }else if(slideshow.mode==1){
            slideshow.setXmlDuration();
        }

        //If connected to internet
        if(!slideshow.netAccess && ofIsStringInString(slideshow.currentContent(), "http")){
            slideshow.nextContent();
            loadContent();
            return;
        }

        int i = 0;
        int sleepyTime = 100;
        //Load Model
        slideModel.loadModel(slideshow.currentContent(), true);
        while(!slideModel.hasMeshes() && slideModel.getMeshCount() <=0){
            ofSleepMillis(sleepyTime);
            slideshow.restartTimer();
            if(i >= sleepyTime){
                slideshow.nextContent();
                loadContent();
                return;
            }
            i++;
        }

        //Scale Model
        slideModel.setLoopStateForAllAnimations(OF_LOOP_NORMAL);
        if(slideModel.getAnimationCount() > 0 ){
            slideModel.playAllAnimations();
        }else{
            slideModel.setPausedForAllAnimations(true);
        }
    }else if(contentType == "unkown format"){
        slideshow.nextContent();
        loadContent();
        return;
    }

    scalePosContent();
    slideshow.restartTimer();
    cout << "End Load Content: " << slideshow.currentContent() << " : " << slideImg.isAllocated() << " : " << slideModel.hasMeshes() << " : " << slideMov.isLoaded() << endl;
}

//--------------------------------------------------------------
>>>>>>> first commit
void ofApp::scalePosContent(){
    if(contentType == "image"){
        slideshow.setPosSize(slideImg.getWidth(), slideImg.getHeight());
    }else if(contentType == "video"){
        slideshow.setPosSize(slideMov.getWidth(), slideMov.getHeight());
    }else if(contentType == "3dmodel"){
<<<<<<< HEAD

    }
=======
        float yPos;
        if(windowW != ofGetWidth() || windowH != ofGetHeight()){
            slideModel.setScaleNormalization(false);
            float w = ofGetWidth();
            float h = ofGetHeight();
            if(w >= h){
                rat = h/windowH;
                yPos = 0.9f;
            }else{
                rat =w/windowW;
                yPos = 0.65f;
            }
            if(slideModel.getScale().x == 1){
                slideModel.calculateDimensions();
                slideModel.setScaleNormalization(true);
                float modScale = slideModel.getScale().length() * 0.51f;
                slideModel.setScale(modScale, modScale,modScale );
                slideModel.setPosition(w/2.0f, h  * yPos , 0.0f);
            }else{
                slideModel.setScale(rat,rat,rat );
                slideModel.setPosition(w/2.0f, h * yPos , 0.0f);
            }
        }else{
            slideModel.calculateDimensions();
            if(slideshow.width >= slideshow.height){
                yPos =0.9f;
            }else{
                yPos = 0.65f;
            }
            slideModel.setScaleNormalization(true);
            slideModel.setScale(1,1,1);
            slideModel.setPosition(windowW/2.0f, windowH  * yPos , 0.0f);
        }
    }
}

//--------------------------------------------------------------
void ofApp::drawSlideshow(){
    ofSetColor(255,slideshow.fadeAlpha);
    if(slideshow.isInitiated()){
        if(contentType == "image"){
            slideImg.draw(slideshow.getPos(), slideshow.scaledWidth, slideshow.scaledHeight);
        }else if(contentType == "video"){
            slideMov.draw(slideshow.getPos(), slideshow.scaledWidth, slideshow.scaledHeight);
        }else if(contentType == "3dmodel"){
            ofEnableBlendMode(OF_BLENDMODE_ALPHA);
            ofEnableDepthTest();
#ifndef TARGET_PROGRAMMABLE_GL
            glShadeModel(GL_SMOOTH); //some model / light stuff
#endif
            light.enable();
            light.lookAt(ofVec3f(slideModel.getPosition().x,slideModel.getPosition().y,slideModel.getPosition().z));
            light.setDiffuseColor(ofColor (slideshow.fadeAlpha));
            light.setAmbientColor(ofColor(slideshow.fadeAlpha));
            ofEnableSeparateSpecularLight();

            ofPushMatrix();
            ofTranslate(slideModel.getPosition().x, slideModel.getPosition().y, 0);
            slideModel.setRotation(0, rotateIter++, 0,1, 0);
            ofTranslate(-slideModel.getPosition().x, -slideModel.getPosition().y, 0);
            if(slideshow.getMeshMode() == 0){
                slideModel.draw(OF_MESH_POINTS);
            }else if(slideshow.getMeshMode() == 1){
                slideModel.draw(OF_MESH_WIREFRAME);
            }else if(slideshow.getMeshMode() == 2){
                slideModel.draw(OF_MESH_FILL);
            }
            ofPopMatrix();
#ifndef TARGET_PROGRAMMABLE_GL
            glEnable(GL_NORMALIZE);
#endif
            ofDisableDepthTest();
            light.disable();
            ofDisableLighting();
            ofDisableSeparateSpecularLight();
        }
    }
    ofSetColor(255);
    slideshow.draw();
}

//--------------------------------------------------------------
void ofApp::updateSlideshow(){
    if( slideshow.isThreadRunning() && slideshow.isFrameNew()  && slideshow.isInitiated()){
        slideshow.nextContent();
        loadContent();
    }
    if(slideshow.isThreadRunning() && slideshow.getContentType() == "video"){
        if(!slideMov.getPosition()){
            slideshow.setPosSize(slideMov.getWidth(), slideMov.getHeight());
            slideshow.restartTimer();
        }else{
            slideMov.update();
        }
    }
    if(slideshow.isThreadRunning() && slideshow.getContentType() == "3dmodel"){
        slideModel.update();
    }
}

//--------------------------------------------------------------
void ofApp::clearSlides(){
    slideImg.clear();

    slideModel.createEmptyModel();
    slideModel.resetAllAnimations();
    slideModel.stopAllAnimations();
    slideModel.clear();

    if(slideMov.isPlaying()){
        slideMov.stop();
    }
    slideMov.unbind();
    slideMov.closeMovie();
    cout << "Clear Content: " << " | Img: " << slideImg.isAllocated() << " | Model: " << slideModel.hasMeshes() << " | Mov: " << slideMov.isLoaded() << endl;
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
        if(slideshow.isInitiated()){
            slideshow.fadeAlpha = 255;
            slideshow.nextContent();
            loadContent();
        }
    }

    if(key == OF_KEY_LEFT){
        if(slideshow.isInitiated()){
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

    if(slideshow.isInitiated()){
        scalePosContent();
    }
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
