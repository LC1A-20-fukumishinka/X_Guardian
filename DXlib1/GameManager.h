#pragma once
#include "Vector3.h"
#include <array>
#include "Matrix4.h"

class SoundManager;
enum class GameStatus
{
	TITLE,
	SELECT,
	INGAME,
	RESULT,
	PAUSE,
};

struct UpperComboObject
{
	Vector3 pos = Vector3();
	int timer = 0;
	int comboCount = 0;
};

class GameManager
{
public:
	GameManager();
	~GameManager();
	void Init();
	void Update();
	void TitleObjectUpdate();
	void scoreObjectUpdate();
	void PressAnyKeyUpdate();
	void Finalize();
	void Draw();

	void PassCar(Vector3 pos);

	void StopCar();

	GameStatus GetStatus();

	void NormaCount();

	void SetCameraPos(Vector3 camPos, Vector3 targetPos);

	void SetIsDeadAnimation(bool isDeadAnimation);

	void Load();

	void TitleDraw();

	void scoreDraw();

	void ResultDraw();

	void PressAnyKeyDraw();

	void CountDownDraw();

	void CheckCarAllDead(bool isAllDead);

	Matrix4 GetCamMat();

	int GetCombo();

	void SetSoundManager(SoundManager *sound);


	void ComboObjectUpdate();

	void ComboObjectDraw();
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
	int TimeLimit = 1800;
	int gameTimer_ = 0;
	int level_ = 0.0f;

	std::array<int, 10> numberObjects_;

	int scoreTextObject_;
	Vector3 scoreObjectPos_;
	Vector3 scoreNumberObjectPos_;
	float scoreObjectAnimationRate_ = 0.0f;

	Vector3 scoreResultObjectPos_;
	Vector3 scoreResultNumberObjectPos_;
	float scoreResultObjectAnimationRate_ = 0.0f;

	int comboTextObject_;
	Vector3 comboObjectPos_;
	Vector3 comboNumberObjectPos_;
	float comboObjectAnimationRate_ = 0.0f;

	int timeTextObjectHandle_;
	Vector3 timeObjectPos_;
	Vector3 timeNumberObjectPos_;
	float timeObjectAnimationRate_ = 0.0f;

	int TitleObject_;
	Vector3 titleObjectPos_;
	float titleObjectAnimationRate_ = 0.0f;

	int pressAnyKeyHandle_;
	Vector3 pressAnyKeyObjectPos_;
	float pressAnyKeyObjectAnimationRate_ = 0.0f;

	bool isClear_ = false;
	bool isFailed_ = false;

	bool isInput_ = false;
	GameStatus status_ = GameStatus::TITLE;

	bool isDeadAnimation_ = false;

	bool isNotAnimationEnd_ = false;

	bool isCarAllDead_ = true;
	float rotation = 0.0f;
	float animationRate = 0.0f;
	Vector3 cameraBasePos_;
	Vector3 cameraBaseTargetPos_;

	Vector3 cameraDeadAnimationPos_;

	SoundManager *sounds_ = nullptr;

	bool isOlddeadAnimation = false;

	int AddSec = 5;
	int SubSec = 3;

	std::vector<UpperComboObject> comboPos;

};

