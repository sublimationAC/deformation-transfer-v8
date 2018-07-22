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

};

void init(std:: string &name, Mesh_my &mesh, double scale);
void cal_norm(Mesh_my &mesh);
void itplt(Mesh_my *pre_mesh, Mesh_my *nex_mesh, Mesh_my &mesh, int num, int per_frame);
void draw_mesh(Mesh_my &mesh);

