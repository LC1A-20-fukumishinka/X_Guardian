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
	//xyz�̍ŏ��l�ƍő�l���f�[�^�Ƃ��Ď���������@
	Vector3 minPosition;
	Vector3 maxPosition;

	//���S���W�ƃT�C�Y�Ƃ��Ď���������@������
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