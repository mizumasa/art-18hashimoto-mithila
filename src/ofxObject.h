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
        float scale = r / (float)texturePtr->getWidth();
        for (int i=0; i<nPts; i++) {
            float n = ofMap(i, 0, nPts-1, 0.0, TWO_PI);
            float x = cos(n);
            float y = sin(n);
            float d = ofRandom(-r/2, r/2);
            polyShape.addVertex(ofPoint(cx + (x * r + d), cy + (y * r + d)));
            mesh.addTexCoord(ofPoint(0, 0));
            mesh.addTexCoord(ofPoint(x * scale, y * scale));
        }
        
        polyShape.setPhysics(0.3, 0.5, 0.1);
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
            cout << "pts"<<pts.size() <<endl;
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
    
};


#endif


