#pragma once
#include "ofThread.h"
#include "ofMain.h"
#include <time.h>

/// This is a simple threaded slideshow addon.
/// Creative Commons Attribution-ShareAlike 4.0 International License
/// Originally developed by Aaron Bocanegra @ Nontrivial Studio
/// abocanegra@nontrivialstudio.com 
/// Not Complete - Features to be built:
/// web loading remote images via url
/// Add ability to include videos
class ofxThreadedSlideshow: public ofThread
{
public:
    ofxThreadedSlideshow()
    {
        mode = 0; // 0 = Local, 1 = URL -- Currently only mode 0 (local)
        folder = "images/threadLoadExample";
        width = 1920;
        height = 1080;
        isNewFrame = true;
        showInfo = false;
        duration = 10;
        fade = true;
        font.load("arial.ttf", 18);
        fontX = 10;
        fontY = ofGetWindowHeight() - 20;
        fontColor = ofColor(0,128,200,200);
        scale = false;
        center = true;
        stretch = false;
    }

    /// Start the thread.
    void start()
    {
        // Mutex blocking is set to true by default
        // It is rare that one would want to use startThread(false).
        startThread();
        cout << "Start Slideshow Thread" << endl;
    }

    /// Signal the thread to stop.  After calling this method,
    /// isThreadRunning() will return false and the while loop will stop
    /// next time it has the chance to.
    void stop()
    {
        stopThread();
        cout << "End Slideshow Thread" << endl;
    }

    void setup(){
        this->dir.allowExt("png");
        this->dir.allowExt("jpg");
        this->dir.allowExt("bmp");
        this->dir.allowExt("gif");
        this->dir.allowExt("tif");
        this->dir.allowExt("psd");

        this->dir.listDir(this->folder + "/");
        this->dir.sort();
        this->slideshowInit = true;
        this->isNewFrame = true;
        if(this->fade){
            this->fadeAlpha = 0;
            this->fadeUp = true;
        }else{
            this->fadeAlpha = 255;
        }
    }


    /// Our implementation of threadedFunction.
    void threadedFunction()
    {
        while(isThreadRunning())
        {
            // Attempt to lock the mutex.  If blocking is turned on,
            if(lock())
            {
                if(!this->slideshowInit){
                    setup();
                }else{
                    slideTimer();
                }

                // The mutex is now locked and the "count"
                // variable is protected.  Time to modify it.

                // Here, we simply cause it to reset to zero if it gets big.

                // Unlock the mutex.  This is only
                // called if lock() returned true above.
                unlock();

                // Sleep for 1 second.
                sleep(1);


                if(shouldThrowTestException > 0)
                {
                    shouldThrowTestException = 0;

                    // Throw an exception to test the global ofBaseThreadErrorHandler.
                    // Users that require more specialized exception handling,
                    // should make sure that their threaded objects catch all
                    // exceptions. ofBaseThreadErrorHandler is only used as a
                    // way to provide better debugging / logging information in
                    // the event of an uncaught exception.
                    throw Poco::ApplicationException("We just threw a test exception!");
                }
            }
            else
            {
                // If we reach this else statement, it means that we could not
                // lock our mutex, and so we do not need to call unlock().
                // Calling unlock without locking will lead to problems.
                ofLogWarning("threadedFunction()") << "Unable to lock mutex.";
            }
        }
    }

    string currentImage(){
        if(this->currImg < (int)this->dir.size()-1){
            this->currImg++;
        }else{
            this->currImg = 0;
        }
        this->isNewFrame = false;
        time(&this->startTime);
        return this->dir.getPath(this->currImg);
    }

    string previousImage(){
        if(this->currImg > 0){
            this->currImg--;
        }else{
            this->currImg = this->dir.size()-1;
        }
        this->isNewFrame = false;
        time(&this->startTime);
        return this->dir.getPath(this->currImg);
    }

    /// This drawing function cannot be called from the thread itself because
    /// it includes OpenGL calls (ofDrawBitmapString).
    void draw()
    {

        if(lock())
        {
            // The mutex is now locked and the "count"
            // Unlock the mutex.  This is only
            // called if lock() returned true above.
            if(this->showInfo){
                this->info = "Images: "
                        + ofToString( getTotalImages() )
                        + " | Time: "
                        + ofToString( timeDiff() )
                        + " | Current: "
                        + getCurrentImageName();
            }
            unlock();

            if(this->showInfo){
                ofSetColor(this->fontColor);
                ofFill();
                font.drawString(this->info, this->fontX,this->fontY);
                ofSetColor(255);
            }
        }
        else
        {
            // If we reach this else statement, it means that we could not
            // lock our mutex, and so we do not need to call unlock().
            // Calling unlock without locking will lead to problems.
            ofLogWarning("threadedFunction()") << "Unable to lock mutex.";
        }

    }

    int getTotalImages(){
        return this->dir.size();
    }

    string getCurrentImageName(){
        if(this->currImg >= 0){
            return this->dir.getName(this->currImg);
        }
        return "";
    }

    void setPosSize(float imgWidth, float imgHeight){
        cout << "original: " << imgWidth << "x" << imgHeight << endl;
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
            cout << "Scaled: " << this->scaledWidth << "x" << this->scaledHeight  << endl;
        }

        this->currentWidth = imgWidth;
        this->currentHeight = imgHeight;
    }

    ofVec2f getPos(){
        return ofVec2f(this->scaledX, this->scaledY);
    }

    string getOrientation(){
        return orientation;
    }

    ofVec2f getImageSize(){
        return ofVec2f(this->currentWidth, this->currentHeight);
    }

    int mode; //0 = disk, 1 = url
    string folder;
    int width;
    int height;
    bool isNewFrame;
    int duration;
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
    bool slideshowInit = false;
    bool center;
    bool scale;
    bool stretch;
    string orientation;
    int currentWidth;
    int currentHeight;
    float ratio;
protected:
    void slideTimer(){
        if(this->slideshowInit && this->isNewFrame == false){
            time(&currentTime);
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

    int timeDiff(){
        return difftime(this->currentTime,this->startTime);
    }

    bool fadeUp = false;
    int currImg = -1;
    int dirLoadIndex;
    ofDirectory dir;
    time_t startTime;
    time_t  currentTime;
    string info;
    ofTrueTypeFont font;

    // A flag to check and see if we should throw a test exception.
    Poco::AtomicCounter shouldThrowTestException;
private:
};
