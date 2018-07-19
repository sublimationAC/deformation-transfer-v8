#include "deformation_transfer.h"
const int tot = 4000;
/*

1984
3247

*/
void init(std:: string &name,Mesh_my &mesh) {
	printf("Initiating mesh...\n");
	igl::readOBJ(name, mesh.vtx, mesh.tri);	
	mesh.num_tri = mesh.tri.rows();
	mesh.num_vtx = mesh.vtx.rows();
	//puts("asd");
	int m_scaling = 180;
	mesh.vtx *= m_scaling;
	mesh.v = (Matrix3d *)malloc(sizeof(Matrix3d)*mesh.num_tri);
	mesh.v_inv = (Matrix3d *) malloc(sizeof(Matrix3d)*mesh.num_tri);
	//puts("asd");
	
	for (int i = 0; i < mesh.num_tri; i++) {
		//printf("%d %d %d %d--%d\n", i,mesh.tri(i, 0), mesh.tri(i, 1), mesh.tri(i, 2), mesh.num_tri);
		Vector3d point[3];
		for (int k = 0; k < 3; k++)
			for (int j = 0; j < 3; j++) 
				point[k](j) = mesh.vtx(mesh.tri(i, k), j);///////////////////////test 0/1
		//puts("qwe");
		Vector3d v[3];
		v[0] = point[1] - point[0];
		v[1] = point[2] - point[0];
		v[2] = v[0].cross(v[1]);
		v[2] = v[2] + Vector3d(1*v[1](0),2 * v[1](1),1.5 * v[1](2));
		v[2] = v[2].array() / (double)(v[2].norm());
		//puts("qwe");
		for (int j = 0; j < 3; j++)
			mesh.v[i].col(j) = v[j];
		//puts("3qwe");
		mesh.v_inv[i] = mesh.v[i].inverse();
		//puts("q4we");
		//for (int j=0;j<3;j++) mesh.vtx(idx_v,j) = point[0](j) + v[3](j);
		//idx_v++;
	}
}
void cal_A(Mesh_my &mesh, Eigen::SparseMatrix <double> &A) {
	printf("calculate Matrix A...\n");
	
	for (int i = 0; i < mesh.num_tri; i++) {
		for (int j = 0; j < 3; j++) {
			//puts("aaa");
			//A.insert(3 * i + j, mesh.num_vtx + i) = mesh.v_inv[i](2, j);
			//puts("aaab");
			//A.coeffRef(3 * i + j, mesh.tri(i, 0)) += -mesh.v_inv[i](2, j);
			for (int k = 1; k < 3; k++) {
				//printf("%d %d %d %.10f\n", i,j,k, mesh.v_inv[i](k, j));
				//printf("%d %d-\n", 3 * i + j, mesh.tri(i, k));
				A.insert(3 * i + j, mesh.tri(i, k)) = mesh.v_inv[i](k - 1, j);
				//A.coeffRef(3 * i + j, mesh.tri(i, 0)) += -mesh.v_inv[i](k - 1, j);
				//printf("%d %d %d\n", i, j, k);
			}
			A.insert(3 * i + j, mesh.tri(i, 0)) += -mesh.v_inv[i](0, j) - mesh.v_inv[i](1, j);
		}
	}
	/*for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++, puts(""))
			for (int k = 0; k < 3; k++)
				printf("%.10f ", mesh.v_inv[i](j, k));
		puts("-----------------------------------");
		std::cout << mesh.tri(i, 0) << ' ' << mesh.tri(i, 1) << ' ' << mesh.tri(i, 2) << '\n';
		for (int j = 0; j < 3; j++,puts("")) {
			for (int k = 0; k < 3; k++) {
				printf("%.10f ", A.coeffRef(i*3+j,mesh.tri(i,k)));
			}
			printf("%.10f ", A.coeffRef(i * 3 + j, i + mesh.num_vtx));
		}
	}*/
}
/*
Vector3d F(t_n * 3, 3);
cal_dfmt(undeformed_source_mesh,deformed_source_mesh,F);
tsf(A,F,deformed_target_mesh);
*/

void cal_dfmt(Mesh_my &undeformed_source_mesh,Mesh_my &deformed_source_mesh, EigenMatrixXs &F) {
	printf("calculate deformation Matrix...\n");
	for (int i = 0, num = undeformed_source_mesh.num_tri; i < num; i++) {
		deformed_source_mesh.v[i] = deformed_source_mesh.v[i] * undeformed_source_mesh.v_inv[i];
		for (int j = 0; j < 3; j++) F.row(i * 3 + j) = deformed_source_mesh.v[i].col(j);
		/*if (i < 10)
			printf("%d %.10f %.10f %.10f \n", i, F(i, 0), F(i, 1), F(i, 2));*/
	}
}

void tsf(Eigen::SparseMatrix <double> &A, EigenMatrixXs &F, Mesh_my &mesh) {
	printf("transfering ...\n");
	SparseLU<Eigen::SparseMatrix<double>> lu(A.transpose()*A);
	for (int j = 0; j < 3; j++) {
		VectorXd temp(A.cols());
		temp = lu.solve(A.transpose()*F.col(j));
		for (int k = 0; k < mesh.num_vtx; k++)
			mesh.vtx(k, j) = temp(k);

	}
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

			//printf("%.10f %.10f %.10f\n", mesh.norm_vtx(VertIndex, 0), mesh.norm_vtx(VertIndex, 1), mesh.norm_vtx(VertIndex, 2));
			GLdouble normal[3] = { mesh.norm_vtx(VertIndex, 0), mesh.norm_vtx(VertIndex, 1), mesh.norm_vtx(VertIndex, 2) };
			glNormal3dv(normal);

			glVertex3f(mesh.vtx(VertIndex, 0), mesh.vtx(VertIndex, 1), mesh.vtx(VertIndex, 2));
		}
		glEnd();
	}
}

void tsf_tslt(Mesh_my &undeformed_target_mesh, Mesh_my &deformed_target_mesh) {
	Vector3d temp = undeformed_target_mesh.vtx.row(0) - deformed_target_mesh.vtx.row(0);
	for (int i = 0; i < undeformed_target_mesh.num_vtx; i++)
		deformed_target_mesh.vtx.row(i) += temp;
}

void save_mesh(std::string ref_name,std::string name,Mesh_my &mesh) {
	std :: ifstream fin;
	std::ofstream fout;
	fin.open(ref_name);
	fout.open(name);
	std:: string s;
	int idx_v = 0, idx_vn = 0;
	while (getline(fin,s)) {
		//std :: cout << s << " + " << idx_v << " - " << idx_vn << '\n';
		if (s[0] == 'v' && s[1] == 'n') {
			fout << "vn " << mesh.norm_vtx(idx_vn, 0) << ' ' << mesh.norm_vtx(idx_vn, 1) << ' '
			 	<< mesh.norm_vtx(idx_vn, 2) << '\n';
			idx_vn++;
			continue;
		}
		if (s[0] == 'v' && s[1] == ' ') {
			fout << "v " << mesh.vtx(idx_v, 0) << ' ' << mesh.vtx(idx_v, 1) << ' '
				<< mesh.vtx(idx_v, 2) << '\n';
			idx_v++;
			continue;
		}
		fout << s << '\n';
	}
	fin.close();
	fout.close();
}