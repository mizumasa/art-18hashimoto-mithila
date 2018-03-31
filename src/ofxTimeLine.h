/*
 *
*/
#pragma once

#ifndef __ofxTimeLine__
#define __ofxTimeLine__

#include "ofMain.h"

#define TIMELINE_SPEED 1
#define STEP_NUM 29
#define ACT_ID_NUM 12

enum ACT_ID{
    AID_INIT = 0,
    AID_WAITING,
    AID_COUNT_3,
    AID_COUNT_2,
    AID_COUNT_1,
    AID_SHOOT,//5
    AID_EDIT,
    AID_RESULT_CAPTURE,
    AID_RESULT_SHOW,
    AID_RESULT_SHOW_WAIT,
    AID_GOODBYE,
    AID_IDLE,
};

class ofxFragment{
public:
    ofxFragment();
    ofxFragment(int _i_FragId,ACT_ID _actId,float _f_Length);
    ofxFragment(int _i_FragId,ACT_ID _actId,float _f_Length,int _i_Param);
    ofxFragment(int _i_FragId,int _i_nextFragId,ACT_ID _actId,float _f_Length);
    ofxFragment(int _i_FragId,int _i_nextFragId,ACT_ID _actId,float _f_Length,int _i_Param);
    int i_FragId;
    int i_nextFragId;
    ACT_ID actId;
    float f_Length;
    int i_Param;
    bool b_ActiveOnce;
private:
};

class ofxSequence : public ofBaseApp{
public:
    ofxSequence();
    void setup();
    void pushData(ofxFragment _fragment);
    void update();
    ofxFragment getNextFrag(int _i_nextFragId);
    void draw();
    bool getChanged();//when ActiveOnceUsed
    ACT_ID getIdNow();
    int getFragIdNow();
    int getParamNow();
    bool isPlaying();
    void play();
    void stop();
    void togglePlay();
    void interrupt(int _i_InterruptNextId);
private:
    vector<ofxFragment> vf_Sequence;
    ofxFragment nowFragment;
    int i_TimeUnitMsec;//1000
    uint64_t t_FragmentStartTime;
    bool b_Playing;
    bool b_Changed;
    bool b_Interrupt;
    int i_InterruptNextId;
};

#endif
