/*
 *
 */
#pragma once

#ifndef __ofxObject__
#define __ofxObject__

#include "ofMain.h"
#include "ofxBox2d.h"

#define DRAW_SCALE 0.9

class CustomParticle : public ofxBox2dCircle {
    
private:
    vector<ofVec2f> vvf_Vel;
    ofImage    *    texturePtr;
    int             textureId;
    
public:
    bool b_Debug;
    void setupTheCustomData();
    void update();
    void setTexture(ofImage * texture);
    void setTextureId(int textureId_){
        textureId = textureId_;
    }
    int getTextureId(){
        return textureId;
    }

    void draw();
    void deletePos(ofVec2f pos);
};




static int hexColors[4] = {0x31988A, 0xFDB978, 0xFF8340, 0xE8491B};

class TextureShape {
private:
    int             textureId;
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
        polyShape.setPhysics(1.0 / radius , 0.1, 0.1);
        polyShape.create(world.getWorld());
        
        
    }
    void setTexture(ofImage * texture) {
        texturePtr = texture;
        //texturePtr->getTextureReference().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
        //texturePtr->getTextureReference().setTextureWrap(GL_REPEAT, GL_REPEAT);
    }
    void setTextureId(int textureId_){
        textureId = textureId_;
    }
    int getTextureId(){
        return textureId;
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
            texturePtr->draw(-radius * DRAW_SCALE, -radius * DRAW_SCALE, radius*2 * DRAW_SCALE,radius*2 * DRAW_SCALE);
        }
        ofPopStyle();ofPopMatrix();
        
    }
    void deletePos(ofVec2f pos){
        if( (polyShape.getPosition() - pos).length() < radius ){
            polyShape.destroy();
        }
    }
};

class countDown {
private:
    int i_NowId;
public:
    vector<ofImage>   v_Image;
    countDown() {
    }
    void setup(){
        ofImage buf1, buf2, buf3;
        buf1.load("1.png");
        buf2.load("2.png");
        buf3.load("3.png");
        v_Image.push_back(buf1);
        v_Image.push_back(buf2);
        v_Image.push_back(buf3);
    }
    void set(int i){
        i_NowId = i;
    }
    void draw() {
        if(i_NowId>0){
            ofPushMatrix();ofPushStyle();
            v_Image[i_NowId - 1].draw(ofGetWidth()/2, 200);
            ofPopStyle();ofPopMatrix();
        }
    }
};

#endif


