#include "SFML.h"
#include "CameraObject.h"
#include "Session.h"
#include "Transform.h"
#include "ObjectManger.h"

CoordStruct Abs_CameraObject::GetCoord() const {
	return Session::GetInstance().CameraLocation;
}

void Abs_CameraObject::Render() {
	auto loc = GetViewPos(GetCoord());
	sf::CircleShape shape(1);
	shape.setFillColor(sf::Color(0, 0, 0));
	shape.setPosition(sf::Vector2f(loc.x, loc.y));
	TestManger::GetInstance().window->draw(shape);
}