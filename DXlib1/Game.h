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
	void Finalize();
	void Draw();
	void Update();
private:
	void TitleUpdate();
	void IngameUpdate();
private:
	void BaseInitialize();

	void DrawAxis3D(const float length);

	void DrawFloorLine();

	void SceneChange();
public:
	int model;
	int spawnTimer = 60;
	float camZPos = -50;
	Matrix4 matWorld;

	CarManager carManager;
	GameManager gameManager;
	Vector3 cameraPosition;
	Vector3 cameraTarget;

	Vector3 cameraUp;

	GameStatus OldScene;

};

