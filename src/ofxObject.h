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




static int hexColors[4] = {0x31988A, 0xFDB978, 0xFF8340, 0xE8491B};

class TextureShape {
public:
    ofImage    *    texturePtr;
    ofMesh          mesh;
    ofColor         color;
    ofxBox2dPolygon polyShape;
    float radius;
    bool b_Debug;

    TextureShape() {
        texturePtr = NULL;
        b_Debug = false;
    }
    void setup(ofxBox2d &world,int textureIdx ,float cx, float cy, float r) {
        
        radius = r;
        color.setHex(hexColors[textureIdx]);
        
        mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
        int   nPts  = 8;
        
        switch (textureIdx){
            case 0:
                polyShape.addVertex(ofPoint(cx + (r * -0.96), cy + (r * -0.24)));
                polyShape.addVertex(ofPoint(cx + (r * -0.98), cy + (r * -0.68)));
                polyShape.addVertex(ofPoint(cx + (r * -0.5), cy + (r * -0.92)));
                polyShape.addVertex(ofPoint(cx + (r * 0.66), cy + (r * -0.42)));
                polyShape.addVertex(ofPoint(cx + (r * 0.96), cy + (r * 0.62)));
                polyShape.addVertex(ofPoint(cx + (r * 0.28), cy + (r * 0.92)));
                polyShape.addVertex(ofPoint(cx + (r * -0.08), cy + (r * 0.84)));
                break;
            case 1:
                polyShape.addVertex(ofPoint(cx + (r * -0.68), cy + (r * -0.04)));
                polyShape.addVertex(ofPoint(cx + (r * -0.68), cy + (r * -0.62)));
                polyShape.addVertex(ofPoint(cx + (r * 0.08), cy + (r * -0.94)));
                polyShape.addVertex(ofPoint(cx + (r * 0.68), cy + (r * -0.68)));
                polyShape.addVertex(ofPoint(cx + (r * 0.7), cy + (r * -0.04)));
                polyShape.addVertex(ofPoint(cx + (r * 0.36), cy + (r * 0.92)));
                polyShape.addVertex(ofPoint(cx + (r * -0.36), cy + (r * 0.96)));
                break;
            case 2:
                polyShape.addVertex(ofPoint(cx + (r * -0.72), cy + (r * 0.14)));
                polyShape.addVertex(ofPoint(cx + (r * -0.76), cy + (r * -0.3)));
                polyShape.addVertex(ofPoint(cx + (r * 0.08), cy + (r * -0.98)));
                polyShape.addVertex(ofPoint(cx + (r * 0.84), cy + (r * -0.36)));
                polyShape.addVertex(ofPoint(cx + (r * 0.88), cy + (r * 0.04)));
                polyShape.addVertex(ofPoint(cx + (r * 0.26), cy + (r * 1)));
                polyShape.addVertex(ofPoint(cx + (r * -0.1), cy + (r * 1.02)));
                break;
        }
        polyShape.setPhysics(1.0, 0.1, 0.1);
        polyShape.create(world.getWorld());
        
        
    }
    void setTexture(ofImage * texture) {
        texturePtr = texture;
        //texturePtr->getTextureReference().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
        //texturePtr->getTextureReference().setTextureWrap(GL_REPEAT, GL_REPEAT);
    }
    void draw() {
        
        ofPushMatrix();ofPushStyle();
        
        if(b_Debug){
            mesh.clearVertices();
            vector<ofPoint> &pts = polyShape.getPoints();
            ofPoint center       = polyShape.getCentroid2D();
            for (int i=0; i<pts.size(); i++) {
                mesh.addVertex(center);
                mesh.addVertex(pts[i]);
            }
            mesh.addVertex(center);
            mesh.addVertex(pts.front());
            
            ofSetColor(color);
            ofFill();
            
            //texturePtr->bind();
            mesh.draw();
            //texturePtr->unbind();
        }else{
            ofTranslate(polyShape.getPosition());
            ofRotateZ(polyShape.getRotation());
            ofSetColor(255,255,255);
            ofFill();
            texturePtr->draw(-radius, -radius, radius*2,radius*2);
        }
        ofPopStyle();ofPopMatrix();
        
    }
    void deletePos(ofVec2f pos){
        if( (polyShape.getPosition() - pos).length() < radius ){
            polyShape.destroy();
        }
    }
};


#endif


