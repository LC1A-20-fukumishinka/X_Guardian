#pragma once
#include "Vector3.h"

struct Sphere
{
	Vector3 position;
	float radius;

	unsigned color;
	Sphere(const Vector3 &position, float radius, unsigned color);

	void draw();
	void drawInfo(int x, int y, unsigned color);
};

struct Box
{
	//xyzの最小値と最大値をデータとして持たせる方法
	Vector3 minPosition;
	Vector3 maxPosition;

	//中心座標とサイズとして持たせる方法もある
	Vector3 posittion;
	float halfSize[3];

	unsigned color;

	Box(const Vector3 &minPosition, const Vector3 &maxPosition, unsigned color);

	void draw();
	void drawInfo(int x, int y, unsigned color);
};

struct Capsule
{
	Vector3 startPosition;
	Vector3 endPosition;
	float radius;
	unsigned color;

	Capsule(const Vector3 &startPosition, const Vector3 &endPosition, float radius, unsigned color);

	void draw();
	void drawInfo(int x , int y , unsigned color);
};