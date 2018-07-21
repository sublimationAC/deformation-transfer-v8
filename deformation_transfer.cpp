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
		//v[2] = v[2] + Vector3d(1*v[1](0),2 * v[1](1),1.5 * v[1](2));
		v[2] = v[2].array() / (double)(v[2].norm());
		//puts("qwe");
		for (int j = 0; j < 3; j++)
			mesh.v[i].col(j) = v[j];
		//puts("3qwe");
		mesh.v_inv[i] = mesh.v[i].inverse();
		//puts("q4we");
		//for (int j=0;j<3;j++) mesh.vtx(idx_v,j) = point[0](j) + v[3](j);
		//idx_v++;
		//if (i < 5) {
		//	printf("%d:\n\n", i);
		//	for (int q = 0; q < 3; q++, puts(""))
		//		for (int p = 0; p < 3; p++)
		//			printf("%.10f ", mesh.v[i](q, p));
		//	puts("--------------------------------");
		//	for (int q = 0; q < 3; q++, puts(""))
		//		for (int p = 0; p < 3; p++)
		//			printf("%.10f ", mesh.v_inv[i](q, p));
		//	//printf("%d %.10f %.10f %.10f %.10f %.10f\n", i, mesh.v_inv[i](0, 0), mesh.v_inv[i](2, 1), mesh.v[i](0, 0), mesh.v[i](1, 0), mesh.v[i](1, 1));
		//}
	}
}

#include<algorithm>

int cmp(std::pair <int, double > x, std::pair <int, double > y) {
	if (x.second < y.second) return 1;
	if (x.second > y.second) return 0;
	return x.first < y.first;
}
int init_cor(char *name, EigenMatrixXi &X,Mesh_my &mesh,int max_cor) {
	puts("initiating correspondence...");
	FILE *fin = fopen(name, "r");
	int num = 0,j=0;
	fscanf(fin, "%d", &num);
	X.resize(mesh.num_tri, max_cor+1);
	X.setZero();
	int tot = 0;
	std::vector<std :: pair <int,double > > temp;
	temp.clear();
	int last_y = -1;
	for (int i = 0; i<num; i++) {
		//printf("%d %dini\n", i,num);
		int x, y;
		char c;
		double f;
		fscanf(fin, "%d,%d,%lf", &x, &y, &f);
		//printf("%d %d %.10f\n",x,y,f);
		if (y == last_y || last_y==-1) temp.push_back(std::make_pair(x,f) );
		else {
			std::sort(temp.begin(),temp.end(),cmp);
			X(last_y, 0) = std::min(max_cor, (int)temp.size());
			//if (y < 10) printf("%d %d %d %d %.10f\n", i, x, y, temp.size() , f);
			for (int j = 1; j <= X(last_y, 0); j++) X(last_y, j) = temp[j-1].first;
			temp.clear();
			temp.push_back(std::make_pair(x, f));
		}
		last_y = y;
	}
	fclose(fin);
	for (int i = 0; i < mesh.num_tri; i++)
		tot += (X(i, 0) > 0) ? X(i, 0) : 1;
	tot *= 3;
	return tot;
}
void cal_A(Mesh_my &mesh, Eigen::SparseMatrix <double> &A,EigenMatrixXi &Cor) {
	printf("calculate Matrix A...\n");
	typedef Triplet<double> Tri;
	std::vector<Tri> triplets;
	int i_row = 0;
	for (int i = 0; i < mesh.num_tri; i++) {
		//printf("%d\n",i);
		if (Cor(i, 0) == 0) {
			for (int j = 0; j < 3; j++) {
				triplets.push_back(Tri(i_row, mesh.tri(i, 1), mesh.v_inv[i](0, j)));
				triplets.push_back(Tri(i_row, mesh.tri(i, 2), mesh.v_inv[i](1, j)));
				//triplets.push_back(Tri(i_row, mesh.num_vtx+i, mesh.v_inv[i](2, j)));
				triplets.push_back(Tri(i_row, mesh.tri(i, 0),-mesh.v_inv[i](0, j) -mesh.v_inv[i](1, j)));
				i_row++;
			}
		}
		else {
			for (int i_cor = 0; i_cor < Cor(i, 0); i_cor++)
				for (int j = 0; j < 3; j++) {
					triplets.push_back(Tri(i_row, mesh.tri(i, 1), mesh.v_inv[i](0, j)));
					triplets.push_back(Tri(i_row, mesh.tri(i, 2), mesh.v_inv[i](1, j)));
					//triplets.push_back(Tri(i_row, mesh.num_vtx + i, mesh.v_inv[i](2, j)));
					triplets.push_back(Tri(i_row, mesh.tri(i, 0),-mesh.v_inv[i](0, j) -mesh.v_inv[i](1, j)));
					i_row++;
				}
		}	
	}
	/*printf("%d %d\n",i_row,A.rows());
	puts("medium");*/
	A.setFromTriplets(triplets.begin(), triplets.end());
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

void cal_dfmt(Mesh_my &undeformed_source_mesh,Mesh_my &deformed_source_mesh, EigenMatrixXs &F,EigenMatrixXi &X) {
	printf("calculate deformation Matrix...\n");
	for (int i = 0, num = undeformed_source_mesh.num_tri; i < num; i++) {
		deformed_source_mesh.v[i] = deformed_source_mesh.v[i] * undeformed_source_mesh.v_inv[i];
		//for (int j = 0; j < 3; j++) F.row(i * 3 + j) = deformed_source_mesh.v[i].col(j);
		/*if (i < 10)
			printf("%d %.10f %.10f %.10f \n", i, F(i, 0), F(i, 1), F(i, 2));*/
	}
	int i_row = 0;
	for (int i = 0, num = X.rows(); i < num; i++) {
		if (X(i, 0) == 0) {
			F(i_row, 0) = F(i_row + 1, 1) = F(i_row + 2, 2) = 1;
			i_row += 3;
		}
		else {
			for (int cor = 1; cor <= X(i, 0); cor++) {
				for (int j = 0; j < 3; j++) F.row(i_row++) = deformed_source_mesh.v[X(i,cor)].col(j);
			}
		}
		/*if (i < 10)
			printf("%d %.10f %.10f %.10f \n", i, F(i, 0), F(i, 1), F(i, 2));*/
	}
}

void tsf(Eigen::SparseMatrix <double> &A, EigenMatrixXs &F, Mesh_my &mesh) {
	printf("transfering ...\n");
	mesh.num_vtx = A.cols();
	mesh.vtx.resize(mesh.num_vtx, 3);
	SparseLU<Eigen::SparseMatrix<double>> lu(A.transpose()*A);
	for (int j = 0; j < 3; j++) {
		VectorXd temp(A.cols());
		temp = lu.solve(A.transpose()*F.col(j));
		for (int k = 0; k < mesh.num_vtx; k++)
			mesh.vtx(k, j) = temp(k);
	}
}

void cal_norm(Mesh_my &mesh,Mesh_my &mesh_ref) {
	puts("calculating norm ...");
	mesh.norm_vtx.resize(mesh.num_vtx,3);
	mesh.norm_vtx.setZero();
	//printf("%d %d\n", mesh.norm_vtx.rows(), mesh.norm_vtx.cols());
	for (int i = 0; i < mesh.num_tri; i++) {
		
		Vector3d point[3];
		for (int k = 0; k < 3; k++)
			for (int j = 0; j < 3; j++)
				point[k](j) = mesh.vtx(mesh_ref.tri(i, k), j);
		Vector3d v[3];
		v[0] = point[1] - point[0];
		v[1] = point[2] - point[0];
		v[2] = v[0].cross(v[1]);
		//v[2] = v[2].array() / (double)(v[2].norm());
		v[2].normalize();
		//if (i < 10) printf("-%d %.10f %.10f %.10f\n",i,v[2](0), v[2](1), v[2](2));
		for (int j = 0; j < 3; j++)
			mesh.norm_vtx.row(mesh_ref.tri(i, j)) += v[2].transpose();
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
	/*printf("%.10f %.10f %.10f \n", undeformed_target_mesh.vtx(0, 0), undeformed_target_mesh.vtx(0, 1), undeformed_target_mesh.vtx(0, 2));
	printf("%.10f %.10f %.10f \n", deformed_target_mesh.vtx(0, 0), deformed_target_mesh.vtx(0, 1), deformed_target_mesh.vtx(0, 2));
	printf("%.10f %.10f %.10f \n", temp(0),temp(1),temp(2));*/
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