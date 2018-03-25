#include "ofApp.h"

//--------------------------------------------------------------
void testApp::setup() {
    system("/usr/local/bin/python /Users/sapugc/programming/of_v0.9.8_osx/apps/Art2018/MithilaPainting/bin/main.py &");
	//ofBackgroundHex(0xfdefc2);
    ofBackgroundHex(0xffffff);
    ofSetLogLevel(OF_LOG_NOTICE);
	ofSetVerticalSync(true);
    ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL);
    
    ofToggleFullscreen();
    fullWidth = ofGetWidth();
    fullHeight = ofGetHeight();
    ofToggleFullscreen();
    ofSetWindowShape( fullWidth/2 ,fullHeight );
	// Box2d
	box2d.init();
	box2d.setGravity(0, 0);
	box2d.createBounds();
	box2d.setFPS(60.0);
    ofToggleFullscreen();
    
    //ofSetWindowShape(ofGetWidth()*2, ofGetHeight());

    
    ofDirectory dir;
    int n = dir.listDir("textures");
    for (int i=0; i<n; i++) {
        textures.push_back(ofImage(dir.getPath(i)));
    }

    for (int i=0; i<n; i++) {
        vi_TextureSize.push_back(ofRandom(20, 60));
    }
    
    ofDirectory dir2;
    int n2 = dir2.listDir("texturesPoly");
    for (int i=0; i<n2; i++) {
        texturesPoly.push_back(ofImage(dir2.getPath(i)));
    }

    for (int i=0; i<n2; i++) {
        vi_TexturePolySize.push_back(ofRandom(20, 60));
    }
    
    printf("%i Textures Loaded\n", (int)textures.size());
    printf("%i TexturesPoly Loaded\n", (int)texturesPoly.size());

#ifdef _USE_LIVE_VIDEO
#ifdef _USE_BLACKMAGIC
    cam.setup(BLACKMAGIC_W, BLACKMAGIC_H, BLACKMAGIC_FPS);
#else
    vidGrabber.setDeviceID(0);
    vidGrabber.initGrabber(WEB_CAM_W,WEB_CAM_H);
#endif
#else
    movie.loadMovie("MAH00013.MP4");
    movie.setVolume(0.0);
    movie.play();
#endif
    
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
    b_GrabScreen = false;
    b_Captured = false;
    
    setupSequence();

    avoidImage.load("sampleShadow.png");
    
    receiver.setup(OSC_PORT_PY2OF);
    sender.setup(OSC_IP, OSC_PORT_OF2PY);
    osc_message = "";
}

//--------------------------------------------------------------
void testApp::update() {
    sequence.update();
    updateSequence();
    updateCam();
    updateOSC();
    
	box2d.update();
    
    if(b_Auto){
        for(int i=0;i<10;i++){
            int textureIdx = (int)ofRandom(textures.size());
            shared_ptr<CustomParticle> p = shared_ptr<CustomParticle>(new CustomParticle);
            p.get()->setPhysics(1.0, 0, 0);
            p.get()->setup(box2d.getWorld(),  fullWidth/2*ofRandomf(), fullHeight*ofRandomf(), vi_TextureSize[textureIdx]);
            p.get()->setVelocity(ofRandom(-3, 3), ofRandom(-3, 3));
            p.get()->setupTheCustomData();
            p.get()->setTexture(&textures[textureIdx]);
            particles.push_back(p);
        }
        
        for(int i=0;i<2;i++){
            int textureIdx = (int)ofRandom(texturesPoly.size());

            shared_ptr<TextureShape> p = shared_ptr<TextureShape>(new TextureShape);
            p.get()->setTexture(&texturesPoly[textureIdx]);
            p.get()->setup(box2d,textureIdx, (ofGetWidth()/4)+ofRandom(-20, 20), 300, vi_TexturePolySize[textureIdx]);
            particlesPoly.push_back(p);
            
        }
    }
    
    if(b_Captured){
        b_Captured = false;
        ofxOscMessage m;
        m.setAddress( "/image/saved" );
        m.addStringArg(filenameCapture);
        sender.sendMessage( m );
    }
    
    //particles[0]->update();
    
    /*for(int i=0;i<particles.size();i++){
        particles[i].get()->setRadius(MIN(100, particles[i].get()->getRadius()+1));
    }*/
}




//--------------------------------------------------------------
void testApp::draw() {
    
    ofSetHexColor(0xffffff);
    
    //inputCam.draw(0, 0, 320, 180);
    //cropCam.draw(0, 400);
    drawW = ofGetWidth() / 2 * 0.8;
    drawH = (int)(drawW * CANVAS_H / CANVAS_W);
    drawX = ofGetWidth()/2 * 0.1;
    drawY = (ofGetHeight() - drawH)/2;
    inputCam.draw(drawX, drawY, drawW, drawH);
    avoidImage.draw(drawX, drawY, drawW, drawH);
    
    contourFinder.draw(drawX,drawY);

    ofSetHexColor(0xffffff);
	for(int i=0; i<particles.size(); i++) {
		particles[i].get()->draw();
	}
    
    cout << "poly"<<particlesPoly.size()<<endl;
    for (int i=0; i<particlesPoly.size(); i++) {
        particlesPoly[i].get()->draw();
    }
    
    ofSetHexColor(0x444342);
    ofNoFill();
    for (int i=0; i<lines.size(); i++) {
        lines[i].draw();
    }
    for (int i=0; i<edges.size(); i++) {
        edges[i].get()->draw();
    }
	
    ofPushMatrix();ofPushStyle();
    {
        ofFill();
        ofSetColor(255, 255, 255);
        ofDrawRectangle(0, 0, drawX, ofGetHeight());
        ofDrawRectangle(0, 0, ofGetWidth() / 2, drawY);
        ofDrawRectangle(drawX+drawW, 0, drawX, ofGetHeight());
        ofDrawRectangle(0, drawY+drawH, ofGetWidth() / 2, drawY);
    }
    ofPopStyle();ofPopMatrix();

    for (int i=0; i<texturesPoly.size(); i++) {
        texturesPoly[i].draw(i*100,0,100,100);
    }
    

    
    if(b_GrabScreen){
        b_GrabScreen = false;
        imgGrab.grabScreen(drawX, drawY , drawW, drawH);
        int mn = ofGetMonth();
        int d = ofGetDay();
        int s = ofGetSeconds();
        int m = ofGetMinutes();
        int h = ofGetHours();
        string filename = "capture/screenshot_" + ofToString(mn, 0) + "_" + ofToString(d, 0) + "_" + ofToString(h, 0) + "_" + ofToString(m, 0) + "_" + ofToString(s, 0) +".png";
        imgGrab.save(filename);
        filenameCapture = filename;
        b_Captured = true;
    }
    
    if(b_Debug){
        string info = "FPS: "+ofToString(ofGetFrameRate(), 1);
        info += "\n";
        info += osc_message;
        ofSetHexColor(0x444342);
        ofDrawBitmapString(info, 20, 20);
    }
}


//--------------------------------------------------------------
void testApp::keyPressed(int key) {
    ofxOscMessage m;
    switch(key){
        case 'o':
            system("/usr/local/bin/python /Users/sapugc/programming/of_v0.9.8_osx/apps/Art2018/MithilaPainting/bin/main.py &");
            break;
        case 't':
            m.setAddress( "/image/saved" );
            m.addStringArg("test/test.png");
            sender.sendMessage( m );
            break;
        case 'k':
            m.setAddress( "/kill" );
            sender.sendMessage( m );
            break;
        case 'a':
            avoidRemove();
            break;
        case 'c':
            lines.clear();
            edges.clear();
            avoidEdges.clear();
            break;
        case 'f':
            ofToggleFullscreen();
            break;
        case 's':
            b_GrabScreen = true;
            break;
        case ' ':
            b_Auto = !b_Auto;
            break;
        case 'd':
            b_Debug = !b_Debug;
            for(int i=0; i<particles.size(); i++) {
                particles[i]->b_Debug = b_Debug;
            }
            for(int i=0; i<particlesPoly.size(); i++) {
                particlesPoly[i]->b_Debug = b_Debug;
            }
            break;
        case 'r':
            avoidImage.resize(drawW, drawH);
            colorImg.allocate(drawW, drawH);
            grayImage.allocate(drawW, drawH);
            colorImg.setFromPixels(avoidImage.getPixels().getData(),drawW,drawH);
            grayImage = colorImg;
            grayImage.threshold(100);
            contourFinder.findContours(grayImage, 20,
                                       (drawW * drawH)/3, 10, true);
            
            ofPoint pointBuf;
            pointBuf.set(drawX, drawY);
            
            for(int i = 0; i< contourFinder.nBlobs; i++){
                shared_ptr <ofxBox2dEdge> edge = shared_ptr<ofxBox2dEdge>(new ofxBox2dEdge);
                for(int j = 0; j<contourFinder.blobs[i].pts.size(); j++){
                    edge.get()->addVertex( pointBuf + contourFinder.blobs[i].pts[j]);
                }
                edge.get()->addVertex( pointBuf + contourFinder.blobs[i].pts[0]);
                edge.get()->create(box2d.getWorld());
                avoidEdges.push_back(edge);
            }
            
            break;
    }
}

void testApp::avoidRemove(){
    ofPixels pixels;
    pixels = grayImage.getPixels();
    unsigned char *pixelBuf;
    pixelBuf = pixels.getData();
    int width,height;
    width = pixels.getWidth();
    height = pixels.getHeight();
    int buf;
    ofVec2f pos;
    
    cout << width <<":"<<height<<":"<< int(pixelBuf[0])<<":"  <<  pixels.getNumChannels() << endl;
    for(int i=0; i<(width*height); i+=3){
        buf = int(pixelBuf[i]);
        if(buf > 100){
            pos = ofVec2f(drawX + int(i%width) ,drawY + int(i/width));
            for(int i=0;i<particles.size();i++){
                particles[i].get()->deletePos(pos);
            }
        }
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

void testApp::setupSequence(){
    /*
     AID_INIT = 0,
     AID_WAITING,
     AID_COUNT_3,
     AID_COUNT_2,
     AID_COUNT_1,
     AID_SHOOT,//5
     AID_EDIT,
     AID_RESULT_SHOW,
     AID_GOODBYE,
     */
    sequence.pushData(ofxFragment(1,AID_INIT,2));
    sequence.pushData(ofxFragment(2,AID_WAITING,3));
    sequence.pushData(ofxFragment(3,AID_COUNT_3,1));
    sequence.pushData(ofxFragment(4,AID_COUNT_2,1));
    sequence.pushData(ofxFragment(5,AID_COUNT_1,1));
    sequence.pushData(ofxFragment(6,AID_SHOOT,1,2));
    sequence.pushData(ofxFragment(7,AID_EDIT,1,2));
    sequence.pushData(ofxFragment(8,AID_RESULT_SHOW,3));
    sequence.pushData(ofxFragment(9,2,AID_GOODBYE,3));
    
    
    sequence.setup();
    sequence.play();
}

void testApp::updateSequence(){
    if(sequence.getChanged()){
        int param;
        param = sequence.getParamNow();
        cout<<"[Sequence] "<<sequence.getIdNow()<<" :param "<<param<<endl;
        switch(sequence.getIdNow()){
            case AID_INIT:
                break;
            case AID_WAITING:
                break;
            case AID_COUNT_3:
                break;
            case AID_COUNT_2:
                break;
            case AID_COUNT_1:
                break;
            case AID_SHOOT:
                break;
            case AID_EDIT:
                break;
            case AID_RESULT_SHOW:
                break;
            case AID_GOODBYE:
                break;
        }
    }
}

void testApp::updateOSC(){
    //現在順番待ちのOSCメッセージがあるか確認
    while( receiver.hasWaitingMessages() )
    {
        ofxOscMessage m;
        receiver.getNextMessage( &m );
        
        if ( m.getAddress() == "/image/uploaded" ){
            string str;
            str = m.getArgAsString( 0 );
            cout <<"[OSC] got QR code path" << str <<endl;
            osc_message = "[OSC] got QR code path" + str;
        }
        if ( m.getAddress() == "/status" ){
            string str;
            str = m.getArgAsString( 0 );
            cout <<"[OSC]" << str <<endl;
            osc_message = "[OSC] status" + str;
        }
    }
}

void testApp::updateCam(){
#ifdef _USE_LIVE_VIDEO
#ifdef _USE_BLACKMAGIC
    if(cam.update()){
        timer.tick();
        //b_CamStart=true;
        camPixels=cam.getColorPixels();
        camPixels.resize(CAPTURE_W, CAPTURE_H);
        camPixels.setImageType(OF_IMAGE_COLOR);

        //perspective.setPixels(camPixels);
        //perspective.update();
        //colorImg.setFromPixels(camPixels);
        /*grayImage = colorImg;
        if(b_DrawImage){
            //detect.setPixels(grayImage.getPixels());
            detect.setColorPixels(colorImg.getPixels());
        }else{
            detect.setColorPixels(perspective.getPixels());
        }*/
        //camImg.setFromPixels(camPixels.getData(), BLACKMAGIC_W, BLACKMAGIC_H, OF_IMAGE_COLOR_ALPHA);
    }
    
#else
    
    vidGrabber.update();
    
    if (vidGrabber.isFrameNew()){
        camPixels = vidGrabber.getPixels();
        camPixels.resize(CAPTURE_W, CAPTURE_H);
        
        inputCam.setFromPixels(camPixels);
        inputCam.crop((CAPTURE_W - CANVAS_W)/2 , 0, CANVAS_W, CANVAS_H);
        cropCam = inputCam;
        //perspective.setPixels(vidGrabber.getPixels());
        //perspective.update();
        
        //colorImg.setFromPixels(vidGrabber.getPixels());
        /*grayImage = colorImg;
        if(b_DrawImage){
            //detect.setPixels(grayImage.getPixels());
            detect.setColorPixels(colorImg.getPixels());
        }else{
            detect.setColorPixels(perspective.getPixels());
        }*/
    }
    
#endif
    
#else
    movie.update();
    if(movie.isFrameNew()) {
        ofPixels movieBuf;
        movieBuf = movie.getPixels();
        movieBuf.resize(CAPTURE_W,CAPTURE_H);
        
        //perspective.setPixels(movieBuf);
        //perspective.update();
        
        //colorImg.setFromPixels(movieBuf);
        /*grayImage = colorImg;
        if(b_DrawImage){
            //detect.setPixels(grayImage.getPixels());
            detect.setColorPixels(colorImg.getPixels());
        }else{
            detect.setColorPixels(perspective.getPixels());
        }*/
    }
#endif
}

void testApp::exit(){
    cout << "finish app" << endl;
    ofxOscMessage m;
    m.setAddress( "/kill" );
    sender.sendMessage( m );
}
