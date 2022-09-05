#pragma once
#include "DxLib.h"
#include <vector>
class Vector3:public VECTOR
{
public:
	//float x;
	//float y;
	//float z;

public:
	//コンストラクタ
	Vector3();
	Vector3(float x, float y, float z);

	//メンバ関数
	float length() const;
	Vector3 &normalaize();
	float dot(const Vector3 &v)const;
	Vector3 cross(const Vector3 &v) const;

	//単項演算子
	Vector3 operator+() const;
	Vector3 operator-() const;

	//代入演算子
	Vector3 &operator+=(const Vector3 &v);
	Vector3 &operator-=(const Vector3 &v);
	Vector3 &operator*=(float s);
	Vector3 &operator/=(float s);

};

//2項演算子
const Vector3 operator+(const Vector3 &v1, const Vector3 &v2);
const Vector3 operator-(const Vector3 &v1, const Vector3 &v2);
const Vector3 operator*(const Vector3 &v, float s);
const Vector3 operator*(float s, const Vector3 &v);
const Vector3 operator/(const Vector3 &v, float s);

const Vector3 lerp(const Vector3 &start, const Vector3 &end, const float t);

const Vector3 easeIn(const Vector3 &start, const Vector3 &end, const float t);

const Vector3 easeOut(const Vector3 &start, const Vector3 &end, const float t);

const Vector3 easeInOut(const Vector3 &start, const Vector3 &end, const float t);

const Vector3 Bezier2D(const Vector3 &start, const Vector3 &a, const Vector3 &end, const float t);

const Vector3 Bezier3D(const Vector3 & start, const Vector3 & a, const Vector3 & b, const Vector3 & end, const float t);

const Vector3 SplinePosition(const std::vector<Vector3> &points, size_t startIndex, float t);