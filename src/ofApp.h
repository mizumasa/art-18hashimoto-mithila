#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"
#include "ofxObject.h"



// ------------------------------------------------- App
class testApp : public ofBaseApp {
	
public:
	
	void setup();
	void update();
	void draw();
	
	void keyPressed  (int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	
	ofxBox2d                            box2d;
	vector <shared_ptr<CustomParticle> >		particles;
	
    vector <ofPolyline>                 lines;
    vector <shared_ptr<ofxBox2dEdge> >  edges;
    bool                                b_Auto;

};
