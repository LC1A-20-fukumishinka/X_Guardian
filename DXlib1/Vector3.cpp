#include "Vector3.h"
#include <cmath>
#include "DxLib.h"

const Vector3 operator+(const Vector3 &v1, const Vector3 &v2)
{
	Vector3 tmp(v1);
	return tmp += v2;
}

const Vector3 operator-(const Vector3 &v1, const Vector3 &v2)
{
	Vector3 temp(v1);
	return temp -= v2;
}

const Vector3 operator*(const Vector3 &v, float s)
{
	Vector3 temp(v);
	return temp *= s;
}

const Vector3 operator*(float s, const Vector3 &v)
{
	return v * s;
}

const Vector3 operator/(const Vector3 &v, float s)
{
	Vector3 temp(v);
	return v * (1 / s);
}

Vector3::Vector3() :
	Vector3(0, 0, 0)
{
}

Vector3::Vector3(float x, float y, float z) :
	VECTOR{ x,y,z }
{
}

float Vector3::length() const
{
	return sqrtf(x * x + y * y + z * z);
}

Vector3 &Vector3::normalaize()
{
	float l = length();
	return *this /= l;
}

float Vector3::dot(const Vector3 &v) const
{
	return x * v.x + y * v.y + z * v.z;
}

Vector3 Vector3::cross(const Vector3 &v) const
{
	return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

Vector3 Vector3::operator+() const
{
	return *this;
}

Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

Vector3 &Vector3::operator+=(const Vector3 &v)
{
	x = x + v.x;
	y = y + v.y;
	z = z + v.z;
	return *this;
}

Vector3 &Vector3::operator-=(const Vector3 &v)
{
	x = x - v.x;
	y = y - v.y;
	z = z - v.z;
	return *this;
}

Vector3 &Vector3::operator*=(float s)
{
	x = x * s;
	y = y * s;
	z = z * s;
	return *this;
}

Vector3 &Vector3::operator/=(float s)
{
	x = x / s;
	y = y / s;
	z = z / s;
	return *this;
}

const Vector3 lerp(const Vector3 &start, const Vector3 &end, const float t)
{
	Vector3 a = start, b = end, c;

	c = start * (1.0f - t) + end * t;
	return start * (1.0f - t) + end * t;
}

const Vector3 easeIn(const Vector3 &start, const Vector3 &end, const float t)
{
	float y = t * t;
	return lerp(start, end, y);
}

const Vector3 easeOut(const Vector3 &start, const Vector3 &end, const float t)
{
	float y = t * (2-t);
	return lerp(start, end, y);
}

const Vector3 easeInOut(const Vector3 &start, const Vector3 &end, const float t)
{
	float y = t * t * (3-(2 * t));
	return lerp(start, end, y);
}

const Vector3 Bezier2D(const Vector3 &start, const Vector3 &a, const Vector3 &end, const float t)
{
	Vector3 p1, p2;

	p1 = lerp(start, a, t);
	p2 = lerp(a, end, t);

	return lerp(p1, p2, t);
}

const Vector3 Bezier3D(const Vector3 & start, const Vector3 & a, const Vector3 & b, const Vector3 & end, const float t)
{
	Vector3 p1, p2, p3;

	p1 = lerp(start, a, t);
	p2 = lerp(a, b, t);
	p3 = lerp(b, end, t);

	p1 = lerp(p1, p2, t);
	p3 = lerp(p2, p3, t);

	return lerp(p1, p3, t);
}

const Vector3 SplinePosition(const std::vector<Vector3> &points, size_t startIndex, float t)
{
	size_t n = points.size() - 2;

	if (startIndex > n) return points[n];	//Pn‚Ì’l‚ð•Ô‚·
	if (startIndex < 1) return points[1];	//P1‚Ì’l‚ð•Ô‚·

	//p~p3 ‚Ì§Œä“X‚ðŽæ“¾‚·‚é ¦ p1~p2 ‚ð•âŠÔ‚·‚éB
	Vector3 p0 = points[startIndex-1];
	Vector3 p1 = points[startIndex];
	Vector3 p2 = points[startIndex + 1];
	Vector3 p3 = points[startIndex + 2];

	// Catmull-Rom‚ÌŽ®‚É‚æ‚é•âŠÔ
	Vector3 position = 0.5 * (
		2 * p1 +
		(-p0 + p2) * t +
		(2 * p0 - 5 * p1 + 4 * p2 - p3) * (t * t) +
		(-p0 + 3 * p1 - 3 * p2 + p3) * (t * t * t)
		);

	return position;
}
