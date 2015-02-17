
#include "Common.h"
#include "Transform.h"

#include <armadillo>
using namespace arma;

namespace obsTransform {

mat Vm;
vec cam_pos = { 384, 384, 320 };
vec up_vector = {0, 0, 1};
vec look_at = {0, 0, 0};
	
mat vm_temp;

void gen_Vm() {LOGFUNC;
	vec za = normalise(cam_pos - look_at);
	vec xa = normalise(cross(up_vector, za));
	vec ya = cross(za, xa);
	rowvec posa = { -1*dot(xa, cam_pos), -1*dot(ya, cam_pos), -1*dot(za, cam_pos), 1};
	static vec z = {0, 0, 0};
	vm_temp = join_rows(xa, ya);
	vm_temp = join_rows(vm_temp, za);
	vm_temp = join_rows(vm_temp, z);
	vm_temp = join_cols(vm_temp, posa);
	Vm = vm_temp.t();
}

void UpdateVm(int ox, int oy) {LOGFUNC;
	look_at[0] = -ox, look_at[1] = -oy;
	cam_pos[0] = 384-ox, cam_pos[1] = 384-oy;
	
	gen_Vm();
}

CoordStruct GetWorldPos(const CubePoint &pt) {LOGFUNC;
	rowvec x = { (double)(pt.x), (double)(pt.y), 0, TransformScaleFactor };
	x = x * Vm.t().i();
	x[0] -= x[2] * (6/5.0), x[1] -= x[2] * (6/5.0);
	x[2] = 0;
	x /= TransformScaleFactor;
	return CoordStruct(-x[0], -x[1], -x[2]);
}

}