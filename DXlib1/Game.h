#pragma once
#include "DxLib.h"
#include <cstdlib>
#include "Vector3.h"
#include "Matrix4.h"
#include "Quaternion.h"
#include "Primitive.h"
#include "Collision.h"
#include <cstdlib>
#include "CarManager.h"
#include "GameManager.h"
using namespace DxLib;

class Game
{
public:
	Game();
	~Game();
	void Init();
	void Update();
	void Finalize();
	void Draw();
private:
	void DrawAxis3D(const float length);

	void DrawFloorLine();

public:
	int model;
	int spawnTimer = 60;

	Matrix4 matWorld;

	CarManager carManager;
	GameManager gameManager;
};

