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
    ofImage    *    texturePtr;
    
public:
    bool b_Debug;
    void setupTheCustomData();
    void update();
    void setTexture(ofImage * texture);
    void draw();
    void deletePos(ofVec2f pos);
};

#endif
