#include "ofApp.h"
// from https://vimeo.com/59653952
//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);
	timeline.setup();
	timeline.addBangs("bangs");
	timeline.addCurves("curves", ofRange(0, ofGetWidth()));
	timeline.addSwitches("switches");
	timeline.addFlags("flags");
	timeline.addLFO("lfo", ofRange(0, 200));
	timeline.addColors("colors");
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	timeline.draw();
	ofPoint position;
	position.x = timeline.getValue("curves");
	position.y = timeline.getValue("lfo");
	position.y += timeline.getHeight();
	ofSetColor(timeline.getColor("colors"));
	if (timeline.isSwitchOn("switches"))
	{
		ofRect(position, 20, 20);
	}
	else
	{
		ofCircle(position, 20);
	}
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
