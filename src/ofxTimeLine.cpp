/*
 *
*/

#include "ofxTimeLine.h"

#define f_TimeLineSabi 0.262
#define f_TimeLineLag 0.001
#define SCENE1_START 0
#define SCENE2_START 10



ofxFragment::ofxFragment(){
    b_ActiveOnce = true;
    i_FragId = 0;
    i_nextFragId = 1;
    actId = AID_INIT;
    f_Length = 0;
    i_Param = 0;
}
ofxFragment::ofxFragment(int _i_FragId,ACT_ID _actId,float _f_Length){
    b_ActiveOnce = true;
    i_FragId = _i_FragId;
    i_nextFragId = _i_FragId+1;
    actId = _actId;
    f_Length = _f_Length;
    i_Param = 0;
}
ofxFragment::ofxFragment(int _i_FragId,ACT_ID _actId,float _f_Length,int _i_Param){
    b_ActiveOnce = true;
    i_FragId = _i_FragId;
    i_nextFragId = _i_FragId+1;
    actId = _actId;
    f_Length = _f_Length;
    i_Param = _i_Param;
}
ofxFragment::ofxFragment(int _i_FragId,int _i_nextFragId,ACT_ID _actId,float _f_Length){
    b_ActiveOnce = true;
    i_FragId = _i_FragId;
    i_nextFragId = _i_nextFragId;
    actId = _actId;
    f_Length = _f_Length;
    i_Param = 0;
}
ofxFragment::ofxFragment(int _i_FragId,int _i_nextFragId,ACT_ID _actId,float _f_Length,int _i_Param){
    b_ActiveOnce = true;
    i_FragId = _i_FragId;
    i_nextFragId = _i_nextFragId;
    actId = _actId;
    f_Length = _f_Length;
    i_Param = _i_Param;
}

ofxSequence::ofxSequence(){
    i_TimeUnitMsec = 1000;//1000
    t_FragmentStartTime = 0;
    b_Playing = false;
    b_Changed = false;
}
void ofxSequence::setup(){
    nowFragment = ofxFragment();
}
void ofxSequence::pushData(ofxFragment _fragment){
    vf_Sequence.push_back(_fragment);
}
void ofxSequence::update(){
    if(b_Playing){
        if(b_Interrupt){
            b_Interrupt = false;
            b_Changed = true;
            int nextId = i_InterruptNextId;
            nowFragment = getNextFrag(nextId);
            t_FragmentStartTime = ofGetElapsedTimeMillis();
        }
        if(ofGetElapsedTimeMillis() > (t_FragmentStartTime+nowFragment.f_Length*i_TimeUnitMsec)){
            cout <<"[Sequence] "<< nowFragment.actId <<" finished"<< endl;
            b_Changed = true;
            int nextId = nowFragment.i_nextFragId;
            nowFragment = getNextFrag(nextId);
            t_FragmentStartTime = ofGetElapsedTimeMillis();
        }
    }
}

ofxFragment ofxSequence::getNextFrag(int _i_nextFragId){
    for(int i=0;i<vf_Sequence.size();i++){
        if(_i_nextFragId == vf_Sequence[i].i_FragId){
            return vf_Sequence[i];
        }
    }
    return ofxFragment();
}

void ofxSequence::interrupt(int _i_InterruptNextId){
    b_Interrupt = true;
    i_InterruptNextId = _i_InterruptNextId;
}

void ofxSequence::draw(){
    
}
bool ofxSequence::getChanged(){
    if(b_Changed){
        b_Changed = false;
        return true;
    }else{
        return false;
    }
}
ACT_ID ofxSequence::getIdNow(){
    return nowFragment.actId;
}

int ofxSequence::getFragIdNow(){
    return nowFragment.i_FragId;
}

int ofxSequence::getParamNow(){
    return nowFragment.i_Param;
}
bool ofxSequence::isPlaying(){
    return b_Playing;
}
void ofxSequence::play(){
    b_Playing = true;
    t_FragmentStartTime = ofGetElapsedTimeMillis();
    nowFragment = getNextFrag(1);
}
void ofxSequence::stop(){
    b_Playing = false;
}
void ofxSequence::togglePlay(){
    b_Playing = !b_Playing;
    if(!b_Playing)cout << "Sequence stop" << endl;
    if(b_Playing)cout << "Sequence restart" << endl;
}

