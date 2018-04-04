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
//#define _USE_LIVE_VIDEO

#define USE_PYTHON 1

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

#define DEBUG_CANVAS_W 300
#define DEBUG_CANVAS_H 400


#define OSC_IP "localhost"
#define OSC_PORT_OF2PY 7110
#define OSC_PORT_PY2OF 7111

#define EDIT_SIZE 100

#define CHECK_NUM_PER_FRAME 300 

#define CONT_RESIZE 4.0

//#define PARTICLE_MAX 3000000
//#define PARTICLE_MAX 30000
#define PARTICLE_MAX 3000000

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
    void makeAvoidImage();
    void clearAvoidImage();
	ofxBox2d                            box2d;
    vector< vector <shared_ptr<CustomParticle> >>        v_particles;

    vector <ofImage> texturesPoly;
    vector< vector <shared_ptr<TextureShape> >> v_particlesPoly;

    
    
    vector <ofPolyline>                 lines;
    vector <shared_ptr<ofxBox2dEdge> >  edges;
    bool                                b_Auto;
    bool                                b_AutoBorn;
    bool                                b_Debug;
    bool                                b_PrintDebug;
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
    
    ofxPanel gui;
    ofxFloatSlider p_frameLeft;
    ofxFloatSlider p_frameTop;
    ofxFloatSlider p_frameRight;
    ofxFloatSlider p_frameBottom;
    ofxFloatSlider p_IgnoreLeft;
    ofxFloatSlider p_IgnoreTop;
    ofxFloatSlider p_IgnoreRight;
    ofxFloatSlider p_IgnoreBottom;
    ofxIntSlider p_DepthMin;
    ofxIntSlider p_DepthMax;
    ofxIntSlider p_ReactThr;
    ofxIntSlider p_ParticleMax;
    ofxIntSlider p_ProcessTime;

    ofImage inputCam;
    ofImage cropCam;
    ofImage imgGrab;
    bool b_GrabScreen;
    int drawW,drawH,drawX,drawY;
    
    ofImage avoidImage;
    ofImage avoidImageOri;
    
    ofxCvColorImage colorImg;
    ofxCvGrayscaleImage grayImage;
    ofxCvGrayscaleImage grayImageResize;
    //ofxCvGrayscaleImage grayBg;
    //ofxCvGrayscaleImage grayDiff;
    ofxCvContourFinder contourFinder;
    
    bool b_Captured;
    bool b_CapturedOnce;

    ofxOscSender sender;
    ofxOscReceiver receiver;
    string osc_message;
    
    string filenameCapture;
    string filenameQR;
    
    int i_DestroyCheckCount;
    int i_DestroyCheckCountPoly;
    int i_DestroyCheckCountId;
    int i_DestroyCheckCountPolyId;
    int i_CheckCount;
    int i_CheckCountPoly;
    uint64_t i_ParticlesSum;
    bool b_Camera;

    bool b_WaitCamReply;
    bool b_StartPyCam;
    ofImage PyCamColor;
    ofImage PyCamColorBuffer;
    ofImage PyCamDepth;
    bool b_PyCamDraw;
    
    ofxCvColorImage depthColorCvImage;
    ofxCvGrayscaleImage depthGrayCvImage;
    ofxCvGrayscaleImage depthGrayCvImage2;

    countDown objCountDown;
    int i_WhiteFadeLevel;
    bool b_QrUploaded;
    ofImage imageTitle;
    ofImage imageQr;
    ofImage imageInstaRecommend;
    bool b_Demo;
    int i_SecCountBuf;
    int i_SecCount;
};



