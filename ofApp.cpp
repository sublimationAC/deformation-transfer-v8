#include "ofApp.h"

ofEasyCam cam;
std::vector<ofLight> lights;
const int N = 10;
const int max_cor = 3;
string undeformed_source_name =  "./horse-collapose/horse-collapose-reference.obj" ;
string undeformed_target_name = "./data_gallop/camel_ref.obj";
string deformed_source_name = "./horse-collapose/horse-collapose-";
string deformed_target_name = "./camel-collapose-my/camel-collapose-";
char  correspondence_name[30] = "./data_gallop/out.tricorrs";
//string undeformed_source_name = "./data/un_src.obj";
//string undeformed_target_name = "./data/un_tgt.obj";
//string deformed_source_name = "./data/de_src_";
//string deformed_target_name = "./data/de_tgt_";
//char  correspondence_name[50] = "./data/face2face.tricorrs";
Mesh_my undeformed_source_mesh, deformed_source_mesh;
Mesh_my undeformed_target_mesh, deformed_target_mesh, std_deformed_target_mesh;
//--------------------------------------------------------------
void ofApp::setup(){
	puts("usrc");
	init(undeformed_source_name, undeformed_source_mesh);
	cal_norm(undeformed_source_mesh, undeformed_source_mesh);
	puts("uTGT");
	init(undeformed_target_name, undeformed_target_mesh);
	cal_norm(undeformed_target_mesh, undeformed_target_mesh);
	//printf("A %.10f %.10f %.10f \n", undeformed_target_mesh.vtx(0, 0), undeformed_target_mesh.vtx(0, 1), undeformed_target_mesh.vtx(0, 2));
	EigenMatrixXi Cor;
	int tot=init_cor(correspondence_name,Cor,undeformed_target_mesh,max_cor);
	printf("number of the expent triangle: %d\n",tot);
	int v_n = undeformed_target_mesh.num_vtx;// , t_n = undeformed_target_mesh.num_tri;
	Eigen::SparseMatrix <double> A(tot,v_n);
	A.reserve(VectorXi::Constant(tot, 3));
	cal_A(undeformed_target_mesh, A, Cor);
	//printf(" %.10f %.10f %.10f \n", undeformed_target_mesh.vtx(0, 0), undeformed_target_mesh.vtx(0, 1), undeformed_target_mesh.vtx(0, 2));
	int tot_num=53;
	//scanf("%d",tot_num);
	for (int i = 0; i < tot_num; i++) {
		printf("calculating %d now...\n",i);
		string deformed_source_name_i, deformed_target_name_my_i;
		if (i < 9) {
			deformed_source_name_i = deformed_source_name +"0"+ to_string(i+1) + ".obj";
			//	string deformed_target_name_i = deformed_target_name + to_string(i) + ".obj";
			deformed_target_name_my_i = deformed_target_name+"0" + to_string(i+1) + "_my.obj";
		}
		else {
			deformed_source_name_i = deformed_source_name + to_string(i+1) + ".obj";
			//	string deformed_target_name_i = deformed_target_name + to_string(i) + ".obj";
			deformed_target_name_my_i = deformed_target_name + to_string(i+1) + "_my.obj";
		}
		puts("dsrc");
		init(deformed_source_name_i, deformed_source_mesh);
		cal_norm(deformed_source_mesh,undeformed_source_mesh);
		deformed_target_mesh.num_tri = undeformed_target_mesh.num_tri;
		deformed_target_mesh.num_vtx = undeformed_target_mesh.num_vtx;
		puts("dsrc");
		//init(deformed_target_name_i, deformed_target_mesh);
		EigenMatrixXs F(tot, 3);
		F.setZero();
		cal_dfmt(undeformed_source_mesh,deformed_source_mesh,F,Cor);
		tsf(A,F,deformed_target_mesh);
		tsf_tslt(undeformed_target_mesh, deformed_target_mesh);
		cal_norm(deformed_target_mesh,undeformed_target_mesh);
		save_mesh(undeformed_target_name,deformed_target_name_my_i,deformed_target_mesh);
		free(deformed_source_mesh.v);
		free(deformed_source_mesh.v_inv);
		puts("finished!");
	}
	/*freopen("my.txt", "w", stdout);
	for (int i = 0; i < 5; i++,puts(""))
		for (int j = 0; j < 3; j++)
			printf("%.10f ", deformed_target_mesh.vtx(i, j));
	puts("");
	puts("-----------------------");
	for (int i = 0; i < 5; i++, puts(""))
		for (int j = 0; j < 3; j++)
			printf("%.10f ", deformed_target_mesh.norm_vtx(i, j));
	puts("");
	puts("-----------------------");
	fclose(stdout);*/

	// lights setting 
	/*lights.resize(2);
	float light_distance = 300.;
	lights[0].setPosition(2.0*light_distance, 1.0*light_distance, 0.);
	lights[1].setPosition(-2.0*light_distance, -1.0*light_distance, 1.0* light_distance);*/
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
