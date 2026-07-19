#pragma once

class Vector2D {
public:
	Vector2D() : x(0), y(0) {}

	Vector2D(float x, float y) : x(x), y(y) {}

	Vector2D(const Vector2D& vector) : x(vector.x), y(vector.y) {}

	static Vector2D zeros() {
		return Vector2D(0, 0);
	}

	static Vector2D ones() {
		return Vector2D(1, 1);
	}

	Vector2D operator +(const Vector2D& vector) const {
		return Vector2D(x + vector.x, y + vector.y);
	}

	Vector2D operator -(const Vector2D& vector) const {
		return Vector2D(x - vector.x, y - vector.y);
	}

	Vector2D operator *(float scalar) const {
		return Vector2D(x * scalar, y * scalar);
	}

	~Vector2D() {}

public:
	float x, y;
};