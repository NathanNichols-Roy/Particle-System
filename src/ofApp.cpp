#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(ofColor::black);
    ofSetFrameRate(160);

    particleSystem.setup( ofGetWidth(), ofGetHeight() );

    k_particles = 5;
    float max_velocity = 0.5;

    for(int i = 0; i < k_particles*1000; ++i)
    {
        float x = ofRandom(ofGetWidth());
        float y = ofRandom(ofGetHeight());
        float x_vel = ofRandom(-max_velocity, max_velocity);
        float y_vel = ofRandom(-max_velocity, max_velocity);

        Particle particle(x, y, x_vel, y_vel);
        particleSystem.add(particle);
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    particleSystem.applyForce(mouseX, mouseY, 50);
    particleSystem.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    glPointSize(2);
    particleSystem.quadTree->drawTree();

    ofSetColor(ofColor::white);
    particleSystem.draw();
    ofDrawBitmapString(ofToString(k_particles) + " k particles", 32, 32);
    ofDrawBitmapString(ofToString((int) ofGetFrameRate()) + " fps", 32, 42);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
