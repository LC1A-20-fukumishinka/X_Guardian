#pragma once
#include "Vector3.h"
#include <vector>

struct angryParticleObject
{
	Vector3 pos = Vector3();
	float scale = 0.0f;
	Vector3 speed = Vector3();

	Vector3 color = Vector3();
	float alpha = 0.0f;
	bool isAlive = false;

	int boundCount = 0;
	void Init(Vector3 pos, Vector3 angle, float power);

	void Update();

	void Draw(int handle);
};

class angryParticle
{

	static int angryParticleModelHandle;

};

