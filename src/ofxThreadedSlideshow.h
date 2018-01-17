#pragma once

#ifndef _OFXTHREADEDSLIDESHOW_APP
#define _OFXTHREADEDSLIDESHOW_APP

#include "ofMain.h"
#include "ofThread.h"
#include <time.h>
#include "ofxXmlPoco.h"

/*
 *
 * ofxThreadedSlideshow
 *
 * A threaded slideshow capable of playing images, video, and 3d models from
 * a folder0861-3C86 or via local or remote XML file. Handles local or remote content, and
 * streaming video as well as IP video
 *
 * Creative Commons Attribution-ShareAlike 4.0 International License
 * Originally developed by Aaron Bocanegra @ Nontrivial Studio
 * 2016
 */



class ofxThreadedSlideshow : public ofThread
{
public:
    ofxThreadedSlideshow()
    {
        mode                 = 1; // 0 = Local Folder, 1 = XML
        defaultMeshMode      = 2;//0,1,2
        folder               = "images/slideshow";
        xml                  = "images/slideshow/content.xml";
        width                = 1920;
        height               = 1080;
        duration             = 10;
        font.load( "arial.ttf", 18 );
        fontX                = 10;
        fontY                = this->height - 20;
        fontColor            = ofColor(0,128,200,200);
        showInfo             = false;
        fade                 = true;
        scale                = false;
        center               = true;
        stretch              = false;
        isLoaded             = false;
        slideshowInit        = false;
        fadeUp     	         = false;
        isNewFrame	         = false;
        blur                 = 0.0f;
        trail                = 0.0f;
        currImg		         = -1;
    }

    ~ofxThreadedSlideshow(){
    }

    /// Start the thread.
    void start()
    {
        if(!this->isInitiated()){
            setup();
        }
        startThread();
    }

    /// Stop the thread.
    void stop()
    {
        stopThread();
    }

    /// Our implementation of threadedFunction.
    void threadedFunction()
    {
        while(isThreadRunning())
        {
            // Attempt to lock the mutex.  If blocking is turned on,
            if(lock())
            {
                if(this->isInitiated()){
                    slideTimer();
                }
                unlock();
                sleep(1);
            }
            else
            {
                ofLogWarning("threadedFunction()") << "ofxThreadedSlideshow: Unable to lock mutex.";
            }
        }
    }

    //Draw Thread
    void draw()
    {
        if(lock())
        {
            if(this->showInfo && this->slideshowInit){
                this->info = "Content: "
                        + ofToString( getTotalContentCount() )
                        + " | Time: "
                        + ofToString( this->duration - timeDiff() )
                        + " | Current: "
                        + getCurrentContentName();
            }
            unlock();
            if(this->showInfo && this->slideshowInit){
                ofSetColor(this->fontColor);
                ofFill();
                font.drawString(this->info, this->fontX,this->fontY);
                ofSetColor(255);
            }
        }
        else
        {
            ofLogWarning("threadedFunction()") << "ofxThreadedSlideshow: Unable to lock mutex.";
        }
    }

    //Setup Slideshow and Initialize
    void setup(){
        //Is Internet Available?
        //this->netAccess = this->internetAccess();
        //Folder Mode
        if(this->mode == 0){
            this->dir.open(this->folder + "/");
            //Images
            this->dir.allowExt("png");
            this->dir.allowExt("jpg");
            this->dir.allowExt("bmp");
            this->dir.allowExt("gif");
            this->dir.allowExt("tif");
            this->dir.allowExt("psd");
            //Videos
            this->dir.allowExt("ogv");
            this->dir.allowExt("mxf");
            this->dir.allowExt("mp4");
            this->dir.allowExt("mpeg");
            this->dir.allowExt("mov");
            //3d Models
            this->dir.allowExt("3ds");
            this->dir.allowExt("ase");
            this->dir.allowExt("dxf");
            this->dir.allowExt("hmp");
            this->dir.allowExt("nff");
            this->dir.allowExt("ply");
            this->dir.allowExt("stl");
            this->dir.allowExt("x");
            this->dir.allowExt("obj");
            this->dir.allowExt("dae");
            //Do The Work
            this->dir.listDir(this->folder);
            this->dir.getFiles();
            this->dir.sort();
            //XML Mode
        }else if(this->mode == 1){
            this->content.load(this->xml);
            this->content.setTo("SLIDESHOW");
            this->xmlContentTotal = this->content.getNumChildren();
        }
        if(this->getTotalContentCount() > 0){
            this->isNewFrame = true;
        }
        if(this->fade){
            this->fadeAlpha = 0;
            this->fadeUp = true;
        }else{
            this->fadeAlpha = 255;
        }
        this->slideshowInit = true;
        this->fontY =ofGetHeight() - 20;
    }

    //Call Next Content
    void nextContent(){
        if(this->getTotalContentCount()){
            if(this->currImg+1 < this->getTotalContentCount()){
                this->currImg++;
            }else{
                this->currImg = 0;
                ofResetElapsedTimeCounter();
            }
        }
        if(this->mode == 1){
            this->setCurrNextInfo();
            this->currXMLSlide = "//slide[" + ofToString(this->currImg) + "]";
            this->content.setTo( this->currXMLSlide );
        }
        this->isNewFrame = false;
        time(&this->startTime);
    }

    //Call Previous Content
    void previousContent(){
        if(this->getTotalContentCount()){
            if(this->currImg > 0){
                this->currImg--;
            }else{
                this->currImg = this->getTotalContentCount()-1;
            }
        }
        if(this->mode == 1){
            //Get Properties of Next Content
            this->setCurrNextInfo();
            this->currXMLSlide = "//slide[" + ofToString(this->currImg) + "]";
            this->content.setTo( this->currXMLSlide );
        }
        this->isNewFrame = false;
        time(&this->startTime);
    }

    //return blur
    float getBlur(){
        if(this->mode == 0){
            return this->blur;
        }else if(this->mode == 1){
            return this->content.getFloatValue("blur");
        }
        return 0.0f;
    }

    float getTrail(){
        if(this->mode == 0){
            return this->trail;
        }else if(this->mode == 1){
            return this->content.getFloatValue("trail");
        }
        return 0.0f;
    }

    //Return Current Content String
    string currentContent(){
        //Return Current Content
        this->isLoaded = true;
        if(this->mode == 0){
            return this->dir.getPath(this->currImg);
        }else if(this->mode == 1){
            return this->xmlCurrPath;
        }
        return "";
    }

    int getCurrentCount(){
        return this->currImg;
    }

    int getNextCount(){
        if(this->currImg +1 < this->getTotalContentCount()){
            return this->currImg +1;
        }
        return 0;
    }

    string getExtension(){
        string ext;
        if(this->mode == 0){
            ext =  (this->dir.getPath(this->currImg).substr(
                        this->dir.getPath(this->currImg).find_last_of(".") + 1));
        }else if(this->mode == 1){
            ext =  (this->content.getValue("path").substr(
                        this->content.getValue("path").find_last_of(".") + 1));
        }
        return ext;
    }

    string getContentType(){
        if(this->mode == 0){
            string ext = ofToLower( this->getExtension() );
            if(ext == "jpg" || ext == "jpeg" ||
                    ext == "png" || ext == "gif" ||
                    ext == "tif" || ext =="psd" ||
                    ext =="bmp"){
                return "image";
            }else if(ext =="mp4" || ext == "ogv" ||
                     ext == "mxf" || ext == "mpeg"
                     ){
                return "video";
            }else if(ext == "3ds" || ext == "ase" ||
                     ext == "dxf" || ext == "hmp" ||
                     ext == "nff" || ext == "ply" ||
                     ext == "stl" || ext == "x" ||
                     ext == "obj" || ext == "dae"){
                return "3dmodel";
            }else{
                return "unkown format";
            }
        }else if(this->mode == 1){
            return this->xmlCurrType;
        }
        return "";
    }

    string getNextExtension(){
        string ext =  (this->getNextContentName().substr(
                           this->getNextContentName().find_last_of(".") + 1));
        return ext;
    }

    string getNextContentType(){
        if(this->mode == 0){
            string ext = ofToLower( this->getNextExtension() );
            if(ext == "jpg" || ext == "jpeg" ||
                    ext == "png" || ext == "gif" ||
                    ext == "tif" || ext =="psd" ||
                    ext =="bmp"){
                return "image";
            }else if(ext =="mp4" || ext == "ogv" ||
                     ext == "mxf" || ext == "mpeg"
                     ){
                return "video";
            }else if(ext == "3ds" || ext == "ase" ||
                     ext == "dxf" || ext == "hmp" ||
                     ext == "nff" || ext == "ply" ||
                     ext == "stl" || ext == "x" ||
                     ext == "obj" || ext == "dae"){
                return "3dmodel";
            }else{
                return "unkown format";
            }
        }else if(this->mode == 1){
            return this->xmlNextType;
        }
        return "";
    }

    int getTotalContentCount(){
        if(this->mode == 0){
            return this->dir.size();
        }else if(this->mode == 1){
            return this->xmlContentTotal;
        }
        return 0;
    }

    string getCurrentContentName(){
        if(this->mode == 0){
            if(this->currImg >= 0){
                return this->dir.getName(this->currImg);
            }
        }else if(this->mode == 1){
            return this->xmlCurrName;
        }
        return "";
    }


    void setCurrNextInfo(){
        if(this->currImg + 1 < this->getTotalContentCount()){
            this->content.setTo("//slide[" + ofToString(this->currImg +1) +"]");
        }else{
            this->content.setTo("//slide[0]");
        }
        this->xmlNextType = this->content.getValue("type");
        this->xmlNextName =  this->content.getValue("name");
        this->currXMLSlide = "//slide[" + ofToString(this->currImg) + "]";
        this->content.setTo(this->currXMLSlide);
        this->xmlCurrName = this->content.getValue("name");
        this->xmlCurrType = this->content.getValue("type");
        this->xmlCurrPath = this->content.getValue("path");
        this->xmlMeshMode =this->content.getIntValue("meshmode");
    }

    string getNextContentName(){
        if(this->currImg+1 >= 0 && this->currImg+1 < this->getTotalContentCount()){
            if(this->mode == 0){
                return this->dir.getName(this->currImg+1);
            }else if(this->mode == 1){
                return this->xmlNextName;
            }
        }else{
            if(this->mode ==0){
                return this->dir.getName(0);
            }else if(this->mode == 1){
                return this->xmlNextName;
            }
        }
    }

    void setXmlDuration(){
        this->duration = this->content.getFloatValue("duration");
    }

    float getXmlDuration(){
        return this->content.getFloatValue("duration");
    }

    //livestream (set manual duration) - file (get duration from video)
    string getVideoMode(){
        return this->content.getValue("mode");
    }

    void setPosSize(float imgWidth, float imgHeight){
        if(this->stretch){
            this->scaledWidth = this->width;
            this->scaledHeight = this->height;
            this->scaledX = 0;
            this->scaledY = 0;
        }else{
            if(imgWidth > imgHeight){
                orientation = "landscape";
                //Image Smaller than ofImage
                if(imgWidth < this->width  &&  imgHeight < this->height){
                    if(this->scale){
                        ratio = (float)this->width / imgWidth;
                        this->scaledWidth =this->width;
                        this->scaledHeight = imgHeight * ratio;
                        if(this->scaledHeight > this->height){
                            ratio = (float)this->height / this->scaledHeight;
                            this->scaledWidth = this->scaledWidth* ratio;
                            this->scaledHeight = this->height;
                        }
                    }else{
                        this->scaledWidth = imgWidth;
                        this->scaledHeight = imgHeight;
                    }
                    //Image Larger than ofImage
                }else if(imgWidth > this->width || imgHeight > this->height ){
                    if(imgWidth > this->width){
                        ratio = (float)this->width / imgWidth;
                        this->scaledWidth = this->width;
                        this->scaledHeight = imgHeight * ratio;
                    }else if(imgHeight > this->height){
                        ratio = (float)this->height / imgHeight;
                        this->scaledWidth = imgWidth * ratio;
                        this->scaledHeight = this->height;
                    }
                    if(this->scaledHeight > this->height){
                        ratio = (float)this->height/this->scaledHeight;
                        this->scaledWidth = this->scaledWidth * ratio;
                        this->scaledHeight = this->height;
                    }
                    if(this->scaledWidth > this ->width){
                        ratio = (float)this->width/this->scaledWidth;
                        this->scaledWidth = this->width;
                        this->scaledHeight = this->height* ratio;
                    }
                    //Image is the same as ofImage
                }else{
                    this->scaledWidth = imgWidth;
                    this->scaledHeight = imgHeight;
                }
            }else if(imgHeight > imgWidth){
                orientation = "portrait";
                //Image Smaller Than ofImage
                if(imgHeight < this->height && imgWidth < this->width){
                    if(this->scale){
                        ratio = (float)this->height / imgHeight;
                        this->scaledWidth = imgWidth * ratio;
                        this->scaledHeight = this->height;
                        if(this->scaledWidth > this->width){
                            ratio = (float)this->width / this->scaledWidth;
                            this->scaledWidth = this->width;
                            this->scaledHeight = this->scaledHeight* ratio;
                        }
                    }else{
                        this->scaledWidth = imgWidth;
                        this->scaledHeight = imgHeight;
                    }
                    //Image Larger than OFImage
                }else if(imgHeight > this->height || imgWidth > this->width){
                    if(imgHeight > this->height){
                        ratio = (float)this->height / imgHeight;
                        this->scaledHeight = this->height;
                        this->scaledWidth = imgWidth * ratio;
                    }else  if(imgWidth > this->width){
                        ratio = (float)this->width / imgWidth;
                        this->scaledHeight = imgHeight * ratio;
                        this->scaledWidth = this->width;
                    }
                    if(this->scaledWidth > this->width){
                        ratio = (float)this->width/this->scaledWidth;
                        this->scaledHeight = this->scaledHeight * ratio;
                        this->scaledWidth = this->width;
                    }
                    if(this->scaledHeight > this->height){
                        ratio = (float)this->height/this->scaledHeight;
                        this->scaledHeight = this->height;
                        this->scaledWidth = this->scaledWidth* ratio;
                    }
                    //Image is the same as ofImage
                }else{
                    this->scaledWidth = imgWidth;
                    this->scaledHeight = imgHeight;
                }
            }else if(imgHeight == imgWidth){
                orientation = "square";
                //Image Smaller Than ofImage
                if(imgHeight < this->height && imgWidth < this->width){
                    if(this->scale){
                        if(this->width > this->height){
                            this->scaledWidth = this->height;
                            this->scaledHeight =this->height;
                        }else{
                            this->scaledWidth = this->width;
                            this->scaledHeight = this->width;
                        }
                    }else{
                        this->scaledWidth = imgWidth;
                        this->scaledHeight = imgHeight;
                    }
                    //Image Larger than ofImage
                }else if(imgHeight > this->height || imgWidth > this->width){
                    if(this->width > this->height){
                        this->scaledWidth = this->height;
                        this->scaledHeight = this->height;
                    }else{
                        this->scaledWidth = this->width;
                        this->scaledHeight = this->width;
                    }
                    //Image Same size as ofImage
                }else{
                    this->scaledWidth = imgWidth;
                    this->scaledHeight = imgHeight;
                }
            }
            //Positioning
            if(this->center){
                this->scaledX = (this->width /2) - (this->scaledWidth/2);
                this->scaledY = (this->height /2) - (this->scaledHeight/2);
            }else{
                this->scaledX = 0;
                this->scaledY = 0;
            }
        }
        this->currentWidth = imgWidth;
        this->currentHeight = imgHeight;
    }

    int getTimeRemaining(){
        return this->timeDiff();
    }

    void restartTimer(){
        time(&this->startTime);
        time(&this->currentTime);
    }

    bool isInitiated(){
        return this->slideshowInit;
    }

    bool isFrameNew(){
        return this->isNewFrame;
    }

    bool getIsLoaded(){
        return this->isLoaded;
    }

    ofVec2f getPos(){
        return ofVec2f(this->scaledX, this->scaledY);
    }

    string getOrientation(){
        return orientation;
    }

    int getMeshMode(){
        if(this->mode == 0){
            return this->defaultMeshMode;
        }else if(this->mode == 1){
            return this->xmlMeshMode;
        }
        return this->defaultMeshMode;
    }

    ofVec2f getImageSize(){
        return ofVec2f(this->currentWidth, this->currentHeight);
    }

    //Is Net Available
    bool internetAccess(){
        int status = system("ping -w 2 8.8.8.8");
        if (-1 != status){
            this->netAccess = true;
            return true;
        }
        this->netAccess = false;
        return false;
    }

    int mode; //0 = disk, 1 = xml
    bool isLoaded;
    string folder;
    int width;
    int height;
    float duration;
    int fadeAlpha;
    bool fade;
    bool showInfo;
    int fontX;
    int fontY;
    int scaledWidth;
    int scaledHeight;
    int scaledX;
    int scaledY;
    ofColor fontColor;
    bool slideshowInit;
    bool center;
    bool scale;
    bool stretch;
    string orientation;
    string currXMLSlide;
    int currentWidth;
    int currentHeight;
    int xmlContentTotal;
    float ratio;
    ofTrueTypeFont font;
    ofxXmlPoco content;
    string xml;
    float blur;
    float trail;
    bool netAccess;
    int currImg;

protected:
    //Timer
    void slideTimer(){
        if(this->slideshowInit && this->isNewFrame == false){
            time(&this->currentTime);
            if(timeDiff()  >= this->duration){
                //Fade Down to new frame
                if(this->fade){
                    if(!this->fadeUp){
                        this->fadeAlpha--;
                    }
                    //New Frame Set Fade up
                    if(this->fadeAlpha <= 0 ){
                        this->isNewFrame = true;
                        this->fadeUp = true;
                    }
                    //New Frame No Fade
                }else{
                    this->isNewFrame = true;
                }
            }
            //Fade Up
            if(this->fadeAlpha<255 && this->fadeUp){
                this->fadeAlpha++;
            }else if(this->fadeAlpha == 255){
                this->fadeUp = false;
            }
        }
    }

    //Return Time Left
    int timeDiff(){
        return difftime(this->currentTime,this->startTime);
    }
    int defaultMeshMode;
    int xmlMeshMode;
    string xmlNextName;
    string xmlNextType;
    string xmlCurrName;
    string xmlCurrType;
    string xmlCurrPath;
    bool fadeUp;
    bool isNewFrame;
    ofDirectory dir;
    time_t startTime;
    time_t currentTime;
    string info;
private:
    //private vars
};
#endif
