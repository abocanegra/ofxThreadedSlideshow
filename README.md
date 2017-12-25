# ofxThreadedSlideshow

 This is an addon for openFrameworks - tested on version 0.9.3 Linux x64
 Written by Aaron Bocanegra of Nontrivial Studio
 Creative Commons Attribution-ShareAlike 4.0 International License
 June 2016

 ##Current Functionality:
 Threaded slideshow of images or video.
 See below for user specific settings.
 Handles auto scaling, fit to window, no scaling, stretch, centering
 Loads all images and videos from a selected folder.
 Handles images: jpg, png, bmp, gif (non animated), tif, psd
 Handles video: ogv, mxf, mp4, mpeg
 Automatically changes duration to fit video length, then returns to pre-selected image duration.
 
##Updates will include:
 The ability to load images/video/3dmodels via URL using an xml document.
 Streaming video via rtsp, http stream (manifest), m3u8
 3d model display

 
##Initialization Options 
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
