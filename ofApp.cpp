#include "ofApp.h"

ofEasyCam cam;
std::vector<ofLight> lights;
const int N = 10;

string undeformed_source_name =  "./data_horse_clps/un_src.obj" ;
string undeformed_target_name = "./data_horse_clps/un_tgt.obj";
string deformed_source_name = "./data_horse_clps/de_src_";
string deformed_target_name = "./data_horse_clps/de_tgt_";
//string undeformed_source_name = "./data/un_src.obj";
//string undeformed_target_name = "./data/un_tgt.obj";
//string deformed_source_name = "./data/de_src_";
//string deformed_target_name = "./data/de_tgt_";
Mesh_my undeformed_source_mesh, deformed_source_mesh;
Mesh_my undeformed_target_mesh, deformed_target_mesh, std_deformed_target_mesh;
//--------------------------------------------------------------
void ofApp::setup(){
	init(undeformed_source_name, undeformed_source_mesh);
	cal_norm(undeformed_source_mesh);
	init(undeformed_target_name, undeformed_target_mesh);
	cal_norm(undeformed_target_mesh);
	int v_n = undeformed_source_mesh.num_vtx, t_n = undeformed_source_mesh.num_tri;
	Eigen::SparseMatrix <double> A(t_n * 3, v_n);
	A.reserve(VectorXi::Constant(t_n * 3, 3));
	cal_A(undeformed_target_mesh,A);
	int tot_num=1;
	//scanf("%d",tot_num);
	for (int i = 0; i < tot_num; i++) {
		printf("calculating %d now...\n",i);
		string deformed_source_name_i = deformed_source_name + to_string(i) + ".obj";
		string deformed_target_name_i = deformed_target_name + to_string(i) + ".obj";
		string deformed_target_name_my_i = deformed_target_name + to_string(i) + "_my.obj";
		init(deformed_source_name_i, deformed_source_mesh);
		cal_norm(deformed_source_mesh);
		init(deformed_target_name_i, deformed_target_mesh);
		EigenMatrixXs F(t_n * 3, 3);
		cal_dfmt(undeformed_source_mesh,deformed_source_mesh,F);
		tsf(A,F,deformed_target_mesh);
		cal_norm(deformed_target_mesh);
		tsf_tslt(undeformed_target_mesh, deformed_target_mesh);
		save_mesh(undeformed_target_name,deformed_target_name_my_i,deformed_target_mesh);
		free(deformed_source_mesh.v);
		free(deformed_source_mesh.v_inv);
		free(deformed_target_mesh.v);
		free(deformed_target_mesh.v_inv);
		puts("finished!");
	}
	freopen("my.txt", "w", stdout);
	for (int i = 0; i < 5; i++,puts(""))
		for (int j = 0; j < 3; j++)
			printf("%.10f ", deformed_target_mesh.vtx(i, j));
	puts("");
	puts("-----------------------");
	for (int i = 0; i < 5; i++, puts(""))
		for (int j = 0; j < 3; j++)
			printf("%d ", deformed_target_mesh.tri(i, j));
	puts("");
	puts("-----------------------");
	for (int i = 0; i < 5; i++, puts(""))
		for (int j = 0; j < 3; j++)
			printf("%.10f ", deformed_target_mesh.norm_vtx(i, j));
	puts("");
	puts("-----------------------");
	fclose(stdout);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
//	ofBackgroundGradient(ofColor(100), ofColor(25));
//
//	glEnable(GL_DEPTH_TEST);//?????
//
//	ofEnableLighting();
//	for (int i = 0; i < lights.size(); ++i) { lights[i].enable(); }
//
//	cam.begin(); //?????????????
//
//	ofPushMatrix();
//	ofTranslate(-200, 200);
//	ofSetColor(ofColor(133, 180, 250));
//	draw_mesh(undeformed_source_mesh);
//	ofPopMatrix();
///*
//	ofPushMatrix();
//	ofTranslate(200, 200);
//	ofSetColor(ofColor(133, 180, 250));
//	draw_mesh(deformed_source_mesh);
//	ofPopMatrix();
//
//	ofPushMatrix();
//	ofTranslate(-200, -200);
//	ofSetColor(ofColor(133, 180, 250));
//	draw_mesh(undeformed_target_mesh);
//	ofPopMatrix();
//
//	ofPushMatrix();
//	ofTranslate(200, -200);
//	ofSetColor(ofColor(133, 180, 250));
//	draw_mesh(deformed_target_mesh);
//	ofPopMatrix();*/
//
//	cam.end();
//
//	for (int i = 0; i < lights.size(); i++) { lights[i].disable(); }
//	ofDisableLighting();
//	glDisable(GL_DEPTH_TEST);
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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
