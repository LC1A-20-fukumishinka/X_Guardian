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
#include "SoundManager.h"
#include <memory>
#include "XGuardian.h"
#include "RoadSignManager.h"
using namespace DxLib;

enum class TimerName
{
	WHISTLE,
	BRAKE,
	HORN,
	ENGINE,
	BROKEN,
};

enum class MenuTextNumbers
{
	BACK,
	RETRY,
	TITLE,
	END,
};

struct TimerRange
{
int min = 100;
int max = 1000;

void Set(int min,int max);
};
class Game
{
public:
	Game();
	~Game();
	void Init();
	void Finalize();
	void Draw();
	void Update();

	bool GameEnd();
private:
	void TitleUpdate();
	void IngameUpdate();

	void MenuUpdate();
private:
	void BaseInitialize();

	void DrawAxis3D(const float length);

	void DrawFloorLine();

	void SceneChange();

	void SoundUpdate();

	int SetRandTimer(TimerRange range);

	Matrix4 ZSkew(float angle);

public:
	int model;
	int ground;
	int skyModel;
	int info;
	int spawnTimer = 60;
	int playerSpawnTimer = 60;
	float camZPos = -50;
	float NotTrackMoveAnimationRate_ = 1.0f;
	Matrix4 matWorld;
	Matrix4 skyMat;
	Matrix4 boardMat;

	std::vector<int> soundTimers_;
	std::vector<TimerRange> soundTimersMax_;
	int whistleTimer_;
	int brakeTimer_;
	int hornTimer_;
	int engineTimer_;
	int brokenTimer_;

	float cityAnimationRate = 1.0f;
	int leftLightHandle;
	int rightLightHandle;

	int comboTimer = 0;

	std::array<bool, 3> isComboEffects;
	bool isTurn = false;
	CarManager carManager;
	GameManager gameManager;
	XGuardian xGuardian;
	RoadSignManager roadSignManager;
	Vector3 cameraPosition;
	Vector3 cameraTarget;

	Vector3 cameraUp;

	GameStatus OldScene;
	std::unique_ptr<SoundManager> sounds;

	bool GameEndFlag = false;

	bool isMenu = false;

	int menuTextNumber = 0;

	bool menuDone = false;
};

