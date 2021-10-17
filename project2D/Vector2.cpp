#include "Vector2.h"


Vector2::Vector2(float xVal = 0, float yVal = 0) {
	x = xVal;
	y = yVal;
}
void Vector2::Set(float xVal, float yVal) {
	x = xVal;
	y = yVal;
}

void Vector2::Add(float xVal, float yVal) {
	x += xVal;
	y += yVal;
}
void Vector2::Add(Vector2 v2) {
	x += v2.x;
	y += v2.y;
}
void Vector2::Multiply(float val) {
	x *= val;
	y *= val;
}
void Vector2::Divide(float val) {
	x /= val;
	y /= val;
}