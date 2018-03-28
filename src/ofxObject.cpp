#include "ofxObject.h"


// A simple little Data class. This is were
// you can store anything you want.
class Data {
public:
    ofColor color;
    string  name;
    int        id;
    int     iconId;
    int     size;
};

int COLORS_LIST[] = {0xcae72b, 0xe63b8f, 0x2bb0e7};

void CustomParticle::setupTheCustomData() {
    
    static int colors[] = {0xcae72b, 0xe63b8f, 0x2bb0e7};
    static string abc   = "abcdefghijklmnopqrstuvwxyz";
    
    // we are using a Data pointer because
    // box2d needs to have a pointer not
    // a referance
    setData(new Data());
    Data * theData = (Data*)getData();
    
    theData->id = ofRandom(0, 100);
    theData->name += abc[(int)ofRandom(0, abc.size())];
    theData->color.setHex(colors[0]);
    theData->iconId = ofRandom(0, 6);
    
    b_Debug = false;
}

void CustomParticle::update() {
    vvf_Vel.push_back(getVelocity());
    
    if(vvf_Vel.size()>5){
        vvf_Vel.erase(vvf_Vel.begin());
        //cout << vvf_Vel[0]  << vvf_Vel[1] <<  vvf_Vel[2]  << vvf_Vel[3] << vvf_Vel[4] << endl;
    }
}

void CustomParticle::setTexture(ofImage * texture) {
    texturePtr = texture;
}

void CustomParticle::deletePos(ofVec2f pos){
    if( (getPosition() - pos).length() < getRadius() ){
        destroy();
    }
}


void CustomParticle::draw() {
    Data* theData = (Data*)getData();
    if(theData) {
        
        float radius = getRadius();
        ofPushMatrix();
        ofTranslate(getPosition());
        ofRotateZ(getRotation());
        
        
        if(b_Debug){
            ofSetHexColor(COLORS_LIST[textureId]);
            ofFill();
            ofCircle(0, 0, radius);
        }else{
            texturePtr->draw(-radius*DRAW_SCALE, -radius*DRAW_SCALE, radius*2*DRAW_SCALE,radius*2*DRAW_SCALE);
        }
        
        /*float textSize = radius/10;
        ofPushMatrix();
        ofScale(textSize, textSize);
        ofSetColor(255);
        ofDrawBitmapString(theData->name, -textSize/2, textSize);
        ofPopMatrix();*/
        
        ofPopMatrix();
    }
}

