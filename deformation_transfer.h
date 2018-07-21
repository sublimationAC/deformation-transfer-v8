#include "math_headers.h"
#include <igl/read_triangle_mesh.h>
#include <string.h>


using namespace Eigen;

struct Mesh_my
{
	int num_vtx=0, num_tri=0;
	EigenMatrixXs vtx;
	EigenMatrixXs norm_vtx;
	EigenMatrixXi tri;

	Matrix3d *v = NULL;
	Matrix3d *v_inv = NULL;

};

void init(std:: string &name, Mesh_my &mesh);
void cal_norm(Mesh_my &mesh,Mesh_my &mesh_ref);
int cmp(std::pair <int, double > x, std::pair <int, double > y);
int init_cor(char *name, EigenMatrixXi &X, Mesh_my &mesh, int max_cor);
void tsf(Eigen::SparseMatrix <double> &A, EigenMatrixXs &F, Mesh_my &mesh);
void cal_dfmt(Mesh_my &undeformed_source_mesh, Mesh_my &deformed_source_mesh, EigenMatrixXs &F, EigenMatrixXi &X);
void cal_A(Mesh_my &mesh, Eigen::SparseMatrix <double> &A, EigenMatrixXi &Cor);
void draw_mesh(Mesh_my &mesh);
void tsf_tslt(Mesh_my &undeformed_target_mesh, Mesh_my &deformed_target_mesh);
void save_mesh(std::string ref_name, std::string name, Mesh_my &mesh);