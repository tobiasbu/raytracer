
#include "entity/primitive.h"

/*void Primitive::setName(const std::string & name)
{
	_name = name;
}*/



void Primitive::setMaterial(Material * material) {

	_material = material;

}

Primitive::PrimitiveType Primitive::getType() const {

	return _type;

}

const Bounds & Primitive::getBounds() const {

	return _bounds;

}

Material * Primitive::getMaterial() const {

	return _material;

}

/*const std::string & Primitive::getName() const
{
	return _name;
}*/
