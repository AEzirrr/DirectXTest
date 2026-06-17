#pragma once

class Vector4D {

public:
	Vector4D() : x(0), y(0), z(0), w(0) {}

	Vector4D(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

	Vector4D(const Vector4D& vector) : x(vector.x), y(vector.y), z(vector.z), w(vector.w) {}

	static Vector4D zeros() {
		return Vector4D(0, 0, 0, 0);
	}

	static Vector4D ones() {
		return Vector4D(1, 1, 1, 1);
	}

	Vector4D cross(const Vector4D& v1, const Vector4D& v2, const Vector4D& v3) {
		this->x = v1.y * (v2.z * v3.w - v2.w * v3.z) - v1.z * (v2.y * v3.w - v2.w * v3.y) + v1.w * (v2.y * v3.z - v2.z * v3.y);
		this->y = -(v1.x * (v2.z * v3.w - v2.w * v3.z) - v1.z * (v2.x * v3.w - v2.w * v3.x) + v1.w * (v2.x * v3.z - v2.z * v3.x));
		this->z = v1.x * (v2.y * v3.w - v2.w * v3.y) - v1.y * (v2.x * v3.w - v2.w * v3.x) + v1.w * (v2.x * v3.y - v2.y * v3.x);
		this->w = -(v1.x * (v2.y * v3.z - v2.z * v3.y) - v1.y * (v2.x * v3.z - v2.z * v3.x) + v1.z * (v2.x * v3.y - v2.y * v3.x));
		return *this;
	}

	~Vector4D() {}

public:
	float x, y, z, w;
};