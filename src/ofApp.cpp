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
	ofxLibwebsockets::ServerOptions options = ofxLibwebsockets::defaultServerOptions();
	options.port = 9092;
	options.bUseSSL = false; // you'll have to manually accept this self-signed cert if 'true'!
	bSetup = server.setup(options);

	// this adds your app as a listener for the server
	server.addListener(this);


	ofSetLogLevel(OF_LOG_ERROR);

	// midi
	// print input ports to console
	midiIn.listPorts(); // via instance
	//ofxMidiIn::listPorts(); // via static as well
	if (midiIn.getNumPorts()) {
		for (int i = 0; i < midiIn.getNumPorts(); i++) {
			string pname = midiIn.getPortName(i);
			if (ofStringTimesInString(pname, "Launchpad")) {
				printf("Launchpad on %d\n", i);
				midiInLaunchpad.openPort(i);
				midiInLaunchpad.addListener(this);
			}
			if (ofStringTimesInString(pname, "nano")) {
				printf("nanoKontrol on %d\n", i);
				midiInNanoKontrol.openPort(i);
				midiInNanoKontrol.addListener(this);
			}
		}
	}
	// open port by number (you may need to change this)
	if (midiIn.openPort(3))
	{
		//midiIn
		//midiIn.openVirtualPort("ofxMidiIn Input"); // open a virtual port

		// don't ignore sysex, timing, & active sense messages,
		// these are ignored by default
		midiIn.ignoreTypes(false, false, false);

		// add ofApp as a listener
		midiIn.addListener(this);

		// print received messages to the console
		midiIn.setVerbose(true);
	}

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
	if (bSetup){
		ofDrawBitmapString("WebSocket server setup at " + ofToString(server.getPort()) + (server.usingSSL() ? " with SSL" : " without SSL"), 10, 560);

	}
	else {
		ofDrawBitmapString("WebSocket setup failed :(", 20, 260);
	}
}
void ofApp::newMidiMessage(ofxMidiMessage& msg) {

	// make a copy of the latest message
	midiMessage = msg;
}
//--------------------------------------------------------------
void ofApp::onConnect(ofxLibwebsockets::Event& args){
	cout << "on connected" << endl;
}
//--------------------------------------------------------------
void ofApp::onIdle(ofxLibwebsockets::Event& args){
	cout << "on idle" << endl;
}

//--------------------------------------------------------------
void ofApp::onBroadcast(ofxLibwebsockets::Event& args){
	cout << "got broadcast " << args.message << endl;
}
//--------------------------------------------------------------
void ofApp::onOpen(ofxLibwebsockets::Event& args){
	cout << "new connection open" << endl;
	messages.push_back("New connection from " + args.conn.getClientIP() + ", " + args.conn.getClientName());
}

//--------------------------------------------------------------
void ofApp::onClose(ofxLibwebsockets::Event& args){
	cout << "on close" << endl;
	messages.push_back("Connection closed");
}
//--------------------------------------------------------------
void ofApp::onMessage(ofxLibwebsockets::Event& args){
	cout << "got message " << args.message << endl;

	// trace out string messages or JSON messages!
	if (!args.json.isNull()){
		messages.push_back("New message: " + args.json.toStyledString() + " from " + args.conn.getClientName());
	}
	else {
		messages.push_back("New message: " + args.message + " from " + args.conn.getClientName());
	}

	// echo server = send message right back!
	//args.conn.send(args.message);
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	string url = "http";
	switch (key){
	case 'b':
		if (server.usingSSL()){
			url += "s";
		}
		url += "://localhost:" + ofToString(server.getPort());
		ofLaunchBrowser(url);
		break;
	case OF_KEY_ESC:
		// quit
		exit();
		break;
	case OF_KEY_RETURN:
		// send to all clients
		server.send(toSend);
		messages.push_back("Sent: '" + toSend + "' to " + ofToString(server.getConnections().size()) + " websockets");
		toSend = "";
		break;
	case OF_KEY_BACKSPACE:
		if (toSend.length() > 0){
			toSend.erase(toSend.end() - 1);
		}
		break;
	default:
		toSend += key;
		break;
	}


}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

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
void ofApp::exit() {

	// clean up
	midiIn.closePort();
	midiIn.removeListener(this);
}