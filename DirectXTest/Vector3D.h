#pragma once

class Vector3D {

public:
	Vector3D() : x(0), y(0), z(0) {}

	Vector3D(float x, float y, float z) : x(x), y(y), z(z) {}

	Vector3D(const Vector3D& vector) : x(vector.x), y(vector.y), z(vector.z) {}

	static Vector3D lerp(const Vector3D& start, const Vector3D& end, float delta) {
		return Vector3D(
			start.x + (end.x - start.x) * delta,
			start.y + (end.y - start.y) * delta,
			start.z + (end.z - start.z) * delta
		);
	}

	~Vector3D() {}

public:
	float x, y, z;
};