#pragma once
#include "Vector3.h"
#include <vector>

struct angryParticleObject
{
	Vector3 pos = Vector3();
	float objectScale = 0.0f;
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
	static const int SpawnTimerMax;
public:
	void Init();
	void Update();
	void Finalize();
	void Draw();


	void Spawn(const Vector3& pos);
private:

	int SpawnTimer = 0;
	std::vector<angryParticleObject> particles_;
};

