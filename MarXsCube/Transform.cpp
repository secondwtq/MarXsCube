#include "Transform.h"
#include "Common.h"

#include <armadillo>
using namespace sf;
using namespace arma;

namespace obsTransform {

mat Vm;
vec cam_pos = { 384, 384, 320 };
vec up_vector = {0, 0, 1};
vec look_at = {0, 0, 0};
sf::Vector2f x(0, 0);
	
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

sf::Vector2f GetViewPos(const CoordStruct &coord) {
	vec ret = { double(-coord.x), double(-coord.y), double(-coord.z), 1 };
	ret = Vm * ret * TransformScaleFactor;
	return sf::Vector2f(ret[0], ret[1]);
}

CoordStruct GetWorldPos(const CubePoint &pt) {LOGFUNC;
	rowvec x = { (double)(pt.x), (double)(pt.y), 0, TransformScaleFactor };
	x = x * Vm.t().i();
	x[0] -= x[2] * (6/5.0), x[1] -= x[2] * (6/5.0);
	x[2] = 0;
	x /= 0.6484375;
	return CoordStruct(-x[0], -x[1], -x[2]);
}

template <typename T> void printVector(T src) { }

template<> void printVector(Vector2i src) { printf("%d %d\n", src.x, src.y); }
template<> void printVector(Vector2f src) { printf("%.3f %.3f\n", src.x, src.y); }
template<> void printVector(Vector3i src) { printf("%d %d %d\n", src.x, src.y, src.z); }
template <> void printVector(const Vector2f& src) { printf("%.3f %.3f\n", src.x, src.y); }

}