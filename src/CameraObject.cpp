#include "SFML.h"
#include "CameraObject.h"
#include "Session.h"
#include "Transform.h"
#include "ObjectManger.h"
#include "Generic.h"

CoordStruct Abs_CameraObject::GetCoord() const {
	return Generic::Session()->CameraLocation;
}

void Abs_CameraObject::Render() {
	auto loc = obsTransform::GetViewPos(GetCoord());
	sf::CircleShape shape(1);
	shape.setFillColor(sf::Color(0, 0, 0));
	shape.setPosition(sf::Vector2f(loc.x, loc.y));
	TestManger::GetInstance().window->draw(shape);
}