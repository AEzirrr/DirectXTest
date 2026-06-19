#pragma once

class Vector3D {

public:
	Vector3D() : x(0), y(0), z(0) {}

	Vector3D(float x, float y, float z) : x(x), y(y), z(z) {}

	Vector3D(const Vector3D& vector) : x(vector.x), y(vector.y), z(vector.z) {}

	static Vector3D zeros() {
		return Vector3D(0, 0, 0);
	}

	static Vector3D ones() {
		return Vector3D(1, 1, 1);
	}

	Vector3D operator +(Vector3D vector) {
		return Vector3D(x + vector.x, y + vector.y, z + vector.z);
	}

	Vector3D operator *(float scalar) {
		return Vector3D(x * scalar, y * scalar, z * scalar);
	}

	~Vector3D() {}

public:
	float x, y, z;
};