#pragma once
#include "Vector3.h"
enum class GameStatus
{
	TITLE,
	SELECT,
	INGAME,
	RESULT,
	PAUSE,
};
class GameManager
{
public:
	GameManager();
	~GameManager();
	void Init();
	void Update();
	void Finalize();
	void Draw();

	void PassCar();

	void StopCar();

	GameStatus GetStatus();

	void NormaCount();

	void SetCameraPos(Vector3 camPos, Vector3 targetPos);

	void SetIsDeadAnimation(bool isDeadAnimation);
private:
	//sceneà⁄ìÆä÷êî
	void ToIngame();
	void ToResult();
	void ToTitle();
	void ToSelect();
public:
	const int baseScore = 100;
private:

	int score = 0;
	int combo = 0;

	int maxNormaCount = 25;

	int normaCars = 0;
	int TimeLimit = 3000;
	int gameTimer_ = 0;

	bool isClear_ = false;
	bool isFailed_ = false;

	bool isInput_ = false;
	GameStatus status_ = GameStatus::TITLE;

	bool isDeadAnimation_ = false;

	float rotation = 0.0f;
	float animationRate = 0.0f;
	Vector3 cameraBasePos_;
	Vector3 cameraBaseTargetPos_;

	Vector3 cameraDeadAnimationPos_;
};

