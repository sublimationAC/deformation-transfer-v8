#include "deformation_transfer.h"
void init(std:: string &name,Mesh_my &mesh,double scale) {
	printf("Initiating mesh...\n");
	igl::readOBJ(name, mesh.vtx, mesh.tri);	
	mesh.num_tri = mesh.tri.rows();
	mesh.num_vtx = mesh.vtx.rows();
	mesh.vtx *= scale;
	cal_norm(mesh);
	
}

void cal_norm(Mesh_my &mesh) {
	puts("calculating norm ...");
	mesh.norm_vtx.resize(mesh.num_vtx,3);
	mesh.norm_vtx.setZero();
	//printf("%d %d\n", mesh.norm_vtx.rows(), mesh.norm_vtx.cols());
	for (int i = 0; i < mesh.num_tri; i++) {
		
		Vector3d point[3];
		for (int k = 0; k < 3; k++)
			for (int j = 0; j < 3; j++)
				point[k](j) = mesh.vtx(mesh.tri(i, k), j);
		Vector3d v[3];
		v[0] = point[1] - point[0];
		v[1] = point[2] - point[0];
		v[2] = v[0].cross(v[1]);
		//v[2] = v[2].array() / (double)(v[2].norm());
		v[2].normalize();
		//if (i < 10) printf("-%d %.10f %.10f %.10f\n",i,v[2](0), v[2](1), v[2](2));
		for (int j = 0; j < 3; j++)
			mesh.norm_vtx.row(mesh.tri(i, j)) += v[2].transpose();
		/*if (i < 10) printf("++%d %.10f %.10f %.10f\n", i, mesh.norm_vtx(mesh.tri(i, 1),0)
			, mesh.norm_vtx(mesh.tri(i, 1),1), mesh.norm_vtx(mesh.tri(i, 1), 2));*/
	}
	for (int i = 0; i < mesh.num_vtx; i++) {
		if (mesh.norm_vtx.row(i).norm() > EPSILON)
			mesh.norm_vtx.row(i).normalize();
			//mesh.norm_vtx.row(i) = mesh.norm_vtx.row(i).array() / (double)mesh.norm_vtx.row(i).norm();
	}
}

void itplt(Mesh_my *pre_mesh,Mesh_my *nex_mesh,Mesh_my &mesh,int num,int per_frame) {
	for (int i = 0, tot = pre_mesh->num_vtx; i < tot; i++) {
		Vector3d temp = nex_mesh->vtx.row(i) - pre_mesh->vtx.row(i);
		for (int j = 0; j < 3; j++) temp(j) = temp(j)*num / per_frame;
		mesh.vtx.row(i) = pre_mesh->vtx.row(i)+temp.transpose();
	}
	cal_norm(mesh);
}

#include "GL/glut.h"
void draw_mesh(Mesh_my &mesh) {
	
	glLineWidth(0.3);
	for (int i = 0; i < mesh.num_tri; ++i) {
		//printf("%d\n",i);
		glBegin(GL_TRIANGLES);

		for (int j = 0; j < 3; ++j) {
			//printf("%d %d\n", mesh.norm_vtx.rows(), mesh.norm_vtx.cols());
			//printf("%d %d %d\n", i, j, mesh.tri(i, j));
			int VertIndex = mesh.tri(i, j);
			/*if (i < 10) {
				printf("+%.10f %.10f %.10f\n", mesh.norm_vtx(VertIndex, 0), mesh.norm_vtx(VertIndex, 1), mesh.norm_vtx(VertIndex, 2));
				printf("-%.10f %.10f %.10f\n", mesh.vtx(VertIndex, 0), mesh.vtx(VertIndex, 1), mesh.vtx(VertIndex, 2));
			}*/
			GLdouble normal[3] = { mesh.norm_vtx(VertIndex, 0), mesh.norm_vtx(VertIndex, 1), mesh.norm_vtx(VertIndex, 2) };
			glNormal3dv(normal);

			glVertex3f(mesh.vtx(VertIndex, 0), mesh.vtx(VertIndex, 1), mesh.vtx(VertIndex, 2));
		}
		glEnd();
	}
}

