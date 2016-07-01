# ofxThreadedSlideshow

# This is an addon for openFrameworks - tested on version 0.9.3 Linux x64
# Written by Aaron Bocanegra of Nontrivial Studio
# Creative Commons Attribution-ShareAlike 4.0 International License
# June 2016
#
# This is the first version. Updates will include:
#    The ability to load images via URL rather than just locally.
#    Video player implementation, so images and videos can be played.

# Formats searched for in directory: png, jpg, bmp, gif (non-animated), tif, psd    
 
# Initialization Options 
    ofxThreadedSlideshow()
    {
        mode = 0; // 0 = Local, 1 = URL Currently only mode 0 (local)
        folder = "images/threadLoadExample"; //Folder inside /bin/data
        width = 1920;        //coincides with ofImage Width
        height = 1080;       //coincides with ofImage Height
        isNewFrame = true;   //Has the frame changed
        showInfo = false;    //Show the information (number images | timer | current image)
        duration = 10;       //Duration each image is up
        fade = true;         //Whether or not to fade out/in in between images
        fontX = 10;          //x pos for info
        fontY = ofGetWindowHeight() - 20;   //ypos for info
        fontColor = ofColor(0,128,200,200); //ofColor for info text color
        scale = false;       //Scale image up to fit ofImage size
        center = true;       //Center image position
        stretch = false;     //Stretch image to fill screen
    }
    
#Basic Usage
##########################ofApp.h
#include "ofxThreadedSlideshow.h"
    ofImage slide;
    ofxThreadedSlideshow slideshow;
##########################ofApp.cpp
//--------------------------------------------------------------
void ofApp::setup(){
    ...
    //Setup Slideshow and start thread
    slideshow.folder = "images/slideshow";
    slideshow.width = ofGetWindowWidth();
    slideshow.height = ofGetWindowHeight();
    slideshow.duration = 10;
    slideshow.fade = true;
    slideshow.showInfo = true;
    slideshow.center = true;
    slideshow.scale = false;
    slideshow.start();

    //Allocate Slide
    slide.allocate(slideshow.width,slideshow.height,OF_IMAGE_COLOR_ALPHA);
}

//--------------------------------------------------------------
void ofApp::update(){
    ...
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

