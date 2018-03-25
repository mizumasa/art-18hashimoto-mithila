#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"
#include "ofxObject.h"

#include "ofxOsc.h"
#include "ofxOpenCv.h"
#include "ofxGUI.h"

#include "ofxTimeLine.h"
#include "ofxBlackMagic.h"

//#define _USE_BLACKMAGIC
#define _USE_LIVE_VIDEO

#define BLACKMAGIC_W 1920
#define BLACKMAGIC_H 1080
#define BLACKMAGIC_FPS 30
#define WEB_CAM_W 1920
#define WEB_CAM_H 1080
//#define WEB_CAM_W 1280
//#define WEB_CAM_H 720

#define TEST_VIDEO_W 1920
#define TEST_VIDEO_H 1080
//#define CAPTURE_W 1280
//#define CAPTURE_H 720
#define CAPTURE_W 1920
#define CAPTURE_H 1080

#define CANVAS_W 810
#define CANVAS_H 1080

#define OSC_IP "localhost"
#define OSC_PORT_OF2PY 7110
#define OSC_PORT_PY2OF 7111

#define EDIT_SIZE 100

class RateTimer {
protected:
    float lastTick, averagePeriod, smoothing;
    bool secondTick;
public:
    RateTimer() :
    smoothing(.9) {
        reset();
    }
    void reset() {
        lastTick = 0, averagePeriod = 0, secondTick = false;
    }
    void setSmoothing(float smoothing) {
        this->smoothing = smoothing;
    }
    float getFramerate() {
        return averagePeriod == 0 ? 0 : 1 / averagePeriod;
    }
    void tick() {
        float curTick = ofGetElapsedTimef();
        if(lastTick == 0) {
            secondTick = true;
        } else {
            float curDiff = curTick - lastTick;;
            if(secondTick) {
                averagePeriod = curDiff;
                secondTick = false;
            } else {
                averagePeriod = ofLerp(curDiff, averagePeriod, smoothing);
            }
        }
        lastTick = curTick;
    }
};

// ------------------------------------------------- App
class testApp : public ofBaseApp {
	
public:
	
	void setup();
    void setupSequence();

	void update();
    void updateCam();
    void updateSequence();
    void updateOSC();
	void draw();
    void exit();
    
    void avoidRemove();
	
	void keyPressed  (int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	
	ofxBox2d                            box2d;
	vector <shared_ptr<CustomParticle> >		particles;
    
    vector <ofImage> texturesPoly;
    vector <shared_ptr<TextureShape> > particlesPoly;

    
    
    vector <ofPolyline>                 lines;
    vector <shared_ptr<ofxBox2dEdge> >  edges;
    bool                                b_Auto;
    bool                                b_Debug;
    bool                                b_Edit;
    vector<ofVec2f> vvf_EditLog;


    vector <shared_ptr<ofxBox2dEdge> >  avoidEdges;

    vector <ofImage> textures;
    vector <int> vi_TextureSize;
    vector <int> vi_TexturePolySize;
    
    int fullWidth;
    int fullHeight;
    
    ofxSequence sequence;

    
#ifdef _USE_LIVE_VIDEO
#ifdef _USE_BLACKMAGIC
    ofxBlackMagic cam;
    RateTimer timer;
    ofPixels camPixels;
    ofImage camImg;
    bool b_CamStart;
#else
    ofVideoGrabber         vidGrabber;
    ofPixels camPixels;
#endif
#else
    ofVideoPlayer         movie;
#endif
    
    ofImage inputCam;
    ofImage cropCam;
    ofImage imgGrab;
    bool b_GrabScreen;
    int drawW,drawH,drawX,drawY;
    
    ofImage avoidImage;
    
    ofxCvColorImage colorImg;
    ofxCvGrayscaleImage grayImage;
    //ofxCvGrayscaleImage grayBg;
    //ofxCvGrayscaleImage grayDiff;
    ofxCvContourFinder contourFinder;
    
    bool b_Captured;
    
    ofxOscSender sender;
    ofxOscReceiver receiver;
    string osc_message;
    
    string filenameCapture;
    string filenameQR;
};
