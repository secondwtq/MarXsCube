#ifndef CUBE_E_TRANSFORM_H
#define CUBE_E_TRANSFORM_H

#include "Common.h"
#include "SFML.h"
#include <armadillo>

namespace obsTransform {
	extern arma::mat Vm;
	extern sf::Vector2f x;
	
	void UpdateVm(int ox = 0, int oy = 0);
	
	inline void GetViewPos_Opted(const arma::vec& src) {
		arma::vec::fixed<4> ret = Vm * src * TransformScaleFactor;
		x.x = ret[0], x.y = ret[1];
	}
	
	sf::Vector2f GetViewPos(const CoordStruct &coord);
	CoordStruct GetWorldPos(const CubePoint &pt);
	
	template <typename T> void printVector(T src);
	template<> void printVector(sf::Vector2f src);
	template <> void printVector(const sf::Vector2f& src);
	template<> void printVector(sf::Vector2i src);
	template<> void printVector(sf::Vector3i src);
}

#endif