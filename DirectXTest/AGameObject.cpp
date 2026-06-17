#include "AGameObject.h"

AGameObject::AGameObject(std::string name)
{
	m_name = name;
	m_position = Vector3D::zeros();
	m_scale = Vector3D::ones();
	m_rotation = Vector3D::zeros();
}

AGameObject::~AGameObject() {}

void AGameObject::setPosition(float x, float y, float z) 
{ 
	m_position = Vector3D(x, y, z); 
}

void AGameObject::setPosition(Vector3D pos) 
{ 
	m_position = pos; 
}

Vector3D AGameObject::getLocalPosition() 
{ 
	return m_position; 
}

void AGameObject::setScale(float x, float y, float z) 
{ 
	m_scale = Vector3D(x, y, z); 
}
void AGameObject::setScale(Vector3D scale) 
{ 
	m_scale = scale; 
}

Vector3D AGameObject::getLocalScale() 
{ 
	return m_scale; 
}

void AGameObject::setRotation(float x, float y, float z) 
{ 
	m_rotation = Vector3D(x, y, z); 
}

void AGameObject::setRotation(Vector3D rot) 
{ 
	m_rotation = rot; 
}

Vector3D AGameObject::getLocalRotation() 
{ 
	return m_rotation; 
}