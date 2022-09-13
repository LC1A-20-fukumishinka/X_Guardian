#pragma once
#include "Vector3.h"
#include <vector>

enum class ModelType;

struct particleObject
{
	Vector3 pos = Vector3();
	float scale = 0.0f;
	Vector3 speed = Vector3();

	Vector3 color = Vector3();

	bool isAlive = false;

	int boundCount = 0;
	void Init(Vector3 pos, Vector3 angle , float power, Vector3 color);

	void Update();

	void Draw();
};
class CarBlastParticle
{
public:
	CarBlastParticle();
	~CarBlastParticle();

	void Init(ModelType type, Vector3 pos, int color);
	void Update();
	void Finalize();
	void Draw();

	static void InitializeColor();
private:
	static 	std::vector<Vector3>nColors;

	std::vector<particleObject> particles_;
	float scale = 1.0f;
};

Vector3 RandBlastVector3(bool isNarrow);