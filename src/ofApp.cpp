#include "ofApp.h"

//--------------------------------------------------------------
void testApp::setup() {
    
	//ofBackgroundHex(0xfdefc2);
    ofBackgroundHex(0xffffff);
    ofSetLogLevel(OF_LOG_NOTICE);
	ofSetVerticalSync(true);
    ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL);
    
    ofToggleFullscreen();
	// Box2d
	box2d.init();
	box2d.setGravity(0, 0);
	box2d.createBounds();
	box2d.setFPS(60.0);
	
    
    ofDirectory dir;
    int n = dir.listDir("textures");
    for (int i=0; i<n; i++) {
        textures.push_back(ofImage(dir.getPath(i)));
    }
    printf("%i Textures Loaded\n", (int)textures.size());

    for (int i=0; i<n; i++) {
        vi_TextureSize.push_back(ofRandom(20, 60));
    }
    
    // load the lines we saved...
    if(0){
        ifstream f;
        f.open(ofToDataPath("lines.txt").c_str());
        vector <string> strLines;
        while (!f.eof()) {
            string ptStr;
            getline(f, ptStr);
            strLines.push_back(ptStr);
        }
        f.close();
        
        for (int i=0; i<strLines.size(); i++) {
            vector <string> pts = ofSplitString(strLines[i], ",");
            if(pts.size() > 0) {
                shared_ptr <ofxBox2dEdge> edge = shared_ptr<ofxBox2dEdge>(new ofxBox2dEdge);
                for (int j=0; j<pts.size(); j+=2) {
                    if(pts[j].size() > 0) {
                        float x = ofToFloat(pts[j]);
                        float y = ofToFloat(pts[j+1]);
                        edge.get()->addVertex(x, y);
                    }
                }
                edge.get()->create(box2d.getWorld());
                edges.push_back(edge);
            }
        }
    }
    
    
    b_Auto = true;
    b_Debug = false;
}

//--------------------------------------------------------------
void testApp::update() {
	box2d.update();
    
    if(b_Auto){
        for(int i=0;i<10;i++){
            int textureIdx = (int)ofRandom(textures.size());
            shared_ptr<CustomParticle> p = shared_ptr<CustomParticle>(new CustomParticle);
            p.get()->setPhysics(1.0, 0, 0);
            p.get()->setup(box2d.getWorld(), ofGetWidth()*ofRandomf(), ofGetHeight()*ofRandomf(), vi_TextureSize[textureIdx]);
            p.get()->setVelocity(ofRandom(-3, 3), ofRandom(-3, 3));
            p.get()->setupTheCustomData();
            p.get()->setTexture(&textures[textureIdx]);
            particles.push_back(p);
        }
    }
    particles[0]->update();
    
    /*for(int i=0;i<particles.size();i++){
        particles[i].get()->setRadius(MIN(100, particles[i].get()->getRadius()+1));
    }*/
}

//--------------------------------------------------------------
void testApp::draw() {
	
	for(int i=0; i<particles.size(); i++) {
		particles[i].get()->draw();
	}
    
    ofSetHexColor(0x444342);
    ofNoFill();
    for (int i=0; i<lines.size(); i++) {
        lines[i].draw();
    }
    for (int i=0; i<edges.size(); i++) {
        edges[i].get()->draw();
    }
	
	string info = "FPS: "+ofToString(ofGetFrameRate(), 1);
	info += "\nClick to add particles\nThe letters and color are from the custom data.";
	ofSetHexColor(0x444342);
	ofDrawBitmapString(info, 20, 20);
}


//--------------------------------------------------------------
void testApp::keyPressed(int key) {
    switch(key){
        case 'c':
            lines.clear();
            edges.clear();
            break;
        case 'f':
            ofToggleFullscreen();
            break;
        case ' ':
            b_Auto = !b_Auto;
            break;
        case 'd':
            b_Debug = !b_Debug;
            for(int i=0; i<particles.size(); i++) {
                particles[i]->b_Debug = b_Debug;
            }
            break;
    }
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
	
    /*shared_ptr<CustomParticle> p = shared_ptr<CustomParticle>(new CustomParticle);
	p.get()->setPhysics(1.0, 0.5, 0.3);
    p.get()->setFriction(0);
	p.get()->setup(box2d.getWorld(), x, y, ofRandom(20, 60));
    p.get()->setVelocity(ofRandom(-3, 3), ofRandom(-3, 3));
	p.get()->setupTheCustomData();
	particles.push_back(p);
*/
    ofVec2f pos = ofVec2f(x,y);
    for(int i=0;i<particles.size();i++){
        particles[i].get()->deletePos(pos);
    }

    
    lines.push_back(ofPolyline());
    lines.back().addVertex(x, y);

}


//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
    lines.back().addVertex(x, y);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
    
    shared_ptr <ofxBox2dEdge> edge = shared_ptr<ofxBox2dEdge>(new ofxBox2dEdge);
    lines.back().simplify();
    
    for (int i=0; i<lines.back().size(); i++) {
        edge.get()->addVertex(lines.back()[i]);
    }
    
    //poly.setPhysics(1, .2, 1);  // uncomment this to see it fall!
    edge.get()->create(box2d.getWorld());
    edges.push_back(edge);
    
    //lines.clear();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h) {
}

