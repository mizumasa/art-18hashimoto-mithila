/*
 *
 */
#pragma once

#ifndef __ofxObject__
#define __ofxObject__

#include "ofMain.h"
#include "ofxBox2d.h"

class CustomParticle : public ofxBox2dCircle {
    
private:
    vector<ofVec2f> vvf_Vel;
    
public:
    void setupTheCustomData();
    void update();
    void draw();
};

#endif
