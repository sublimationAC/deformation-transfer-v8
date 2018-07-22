#include "ofApp.h"

using namespace Eigen;
//const int per_frame = 5;
const int tot_obj = 48;
Mesh_my mesh_horse[tot_obj+1],mesh_camel[tot_obj + 1];
Mesh_my cal_horse, cal_camel;
namespace {
	std::string mesh_name_horse = "./horse-gallop/horse-gallop-";
	std::string mesh_name_camel = "./camel-gallop-my/camel-gallop-";
}

//--------------------------------------------------------------
void ofApp::setup() {
	string name_horse, name_camel;
	for (int i = 1; i <= tot_obj; i++) {
		printf("Loading %d now\n", i);
		name_horse = mesh_name_horse;
		name_camel = mesh_name_camel;
		if (i < 10) {
			name_horse = mesh_name_horse + "0";
			name_camel = mesh_name_camel + "0";
		}
		name_horse = name_horse + to_string(i) +".obj";
		name_camel = name_camel + to_string(i) + "_my.obj";
		/*cout << name_horse << '\n';
		cout << name_camel << '\n';*/
		init(name_horse, mesh_horse[i], 300);
		init(name_camel, mesh_camel[i], 1.5);
	}
	/*init(name_horse, cal_horse, 300);
	init(name_camel, cal_camel, 1.5);*/
	idx = 0;// set the beginning of the obj
	// lights setting 
	lights.resize(2);
	float light_distance = 300.;
	lights[0].setPosition(2.0*light_distance, 1.0*light_distance, 0.);
	lights[1].setPosition(-2.0*light_distance, -1.0*light_distance, 1.0* light_distance);
}

//--------------------------------------------------------------

void ofApp::update() {
	idx++;
	if (idx > tot_obj) idx = 1;
	cameraOrbit += ofGetLastFrameTime() * 20.; // 20 degrees per second;
	cam.orbitDeg(cameraOrbit, 0., cam.getDistance(), { 0., 0., 0. });
	/*if (idx >= (tot_obj-1) * per_frame) idx = 0;
	int pre = idx / per_frame + 1, nex = pre + 1;
	int num = idx - (pre - 1)*per_frame;
	printf("%d %d %d %d\n",idx,pre,nex,num);
	itplt(&mesh_horse[pre],&mesh_horse[nex],cal_horse,num,per_frame);
	itplt(&mesh_camel[pre],&mesh_camel[nex], cal_camel,num, per_frame);*/
}

//--------------------------------------------------------------
//#include <Windows.h>
void ofApp::draw() {

	ofBackgroundGradient(ofColor(100), ofColor(25));

	glEnable(GL_DEPTH_TEST);//?????

	ofEnableLighting();
	for (int i = 0; i < lights.size(); ++i) { lights[i].enable(); }

	cam.begin(); //?????????????
	
	ofPushMatrix();
	ofTranslate(-200, -100 );
	ofSetColor(ofColor(133, 180, 250));
	draw_mesh(mesh_horse[idx]);
	ofPopMatrix();

	ofPushMatrix();
	ofTranslate(200, -100);
	ofSetColor(ofColor(133, 180, 250));
	draw_mesh(mesh_camel[idx]);
	ofPopMatrix();

	//sleep(10);
	//ofDrawCircle(10, 100, 10);
	cam.end();

	for (int i = 0; i < lights.size(); i++) { lights[i].disable(); }
	ofDisableLighting();
	glDisable(GL_DEPTH_TEST);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
