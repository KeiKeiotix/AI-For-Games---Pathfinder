#include <cmath>
#pragma once
struct Vector2
{
	float x;
	float y;

	Vector2() {}
	Vector2(float xVal, float yVal);
	void Set(float xVal, float yVal);
	void Add(float xVal, float yVal);
	void Add(Vector2 v2);
	void Multiply(float val);
	void Divide(float val);

	Vector2 Vector2::GetNormalised() const {
		Vector2 result(x, y);
		float magnitude = result.GetMagnitude();

		if (magnitude != 0) {
			result /= magnitude;
		}

		return result;
	}

	float Vector2::GetMagnitude() const {
		return sqrtf(x * x + y * y);
	}

	Vector2 operator+(const Vector2& other) const {
		return Vector2(x + other.x, y + other.y);
	}

	void operator+=(const Vector2& other) {
		x += other.x;
		y += other.y;
	}

	Vector2 operator-(const Vector2& other) const {
		return Vector2(x - other.x, y - other.y);
	}

	void operator-=(const Vector2& other) {
		x -= other.x;
		y -= other.y;
	}

	Vector2 operator*(const Vector2& other) const {
		return Vector2(x * other.x, y * other.y);
	}

	Vector2 operator*(float value) const {
		return Vector2(x * value, y * value);
	}

	void operator*=(const Vector2& other) {
		x *= other.x;
		y *= other.y;
	}

	void operator*=(float value) {
		x *= value;
		y *= value;
	}

	Vector2 operator/(const Vector2& other) const {
		Vector2 result;
		result.x = x / other.x;
		result.y = y / other.y;
		return result;
	}

	Vector2 operator/(float value) const {
		Vector2 result;
		result.x = x / value;
		result.y = y / value;
		return result;
	}

	void operator/=(const Vector2& other) {
		x /= other.x;
		y /= other.y;
	}

	void operator/=(float value) {
		x /= value;
		y /= value;
	}

	Vector2 operator-()
	{
		return Vector2(-x, -y);
	}

	bool operator==(const Vector2& other) const {
		return x == other.x && y == other.y;
	}

	bool operator!=(const Vector2& other) const {
		return !(*this == other);
	}

};

