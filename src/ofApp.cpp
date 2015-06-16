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
	// websockets
	ofSetLogLevel(OF_LOG_VERBOSE);
	// basic connection:
	// client.connect("echo.websocket.org");
	// OR optionally use SSL
	// client.connect("echo.websocket.org", true);

	// advanced: set keep-alive timeouts for events like
	// loss of internet

	// 1 - get default options
	ofxLibwebsockets::ClientOptions options = ofxLibwebsockets::defaultClientOptions();

	// 2 - set basic params
	options.host = "localhost";

	// 3 - set keep alive params
	// BIG GOTCHA: on BSD systems, e.g. Mac OS X, these time params are system-wide
	// ...so ka_time just says "check if alive when you want" instead of "check if
	// alive after X seconds"
	options.ka_time = 1;
	//    options.ka_probes   = 1;
	//    options.ka_interval = 1;

	// 4 - connect
	//client.connect(options);
	client.connect("192.168.0.18", 9092);
	ofSetLogLevel(OF_LOG_ERROR);

	client.addListener(this);
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
	position.y += 40;
	ofDrawBitmapString("Type anywhere to send 'hello' to your server\nCheck the console for output!", 10, position.y);
	position.y += 30;
	ofDrawBitmapString(client.isConnected() ? "Client is connected" : "Client disconnected :(", 10, position.y);

}
//--------------------------------------------------------------
void ofApp::onConnect(ofxLibwebsockets::Event& args){
	cout << "on connected" << endl;
}

//--------------------------------------------------------------
void ofApp::onOpen(ofxLibwebsockets::Event& args){
	cout << "on open" << endl;
}

//--------------------------------------------------------------
void ofApp::onClose(ofxLibwebsockets::Event& args){
	cout << "on close" << endl;
}

//--------------------------------------------------------------
void ofApp::onIdle(ofxLibwebsockets::Event& args){
	cout << "on idle" << endl;
}

//--------------------------------------------------------------
void ofApp::onMessage(ofxLibwebsockets::Event& args){
	cout << "got message " << args.message << endl;
}

//--------------------------------------------------------------
void ofApp::onBroadcast(ofxLibwebsockets::Event& args){
	cout << "got broadcast " << args.message << endl;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	client.send("Hello");
	cout << "sending hello" << endl;
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
