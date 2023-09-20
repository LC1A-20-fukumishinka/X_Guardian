#pragma once
#include "Vector3.h"
#include <array>
#include "Matrix4.h"
#include "GameNum.h"

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
	void Update(bool isSoloMode);
	void TitleObjectUpdate();
	void ModeSelectObjectUpdate();
	void scoreObjectUpdate();
	void PressAnyKeyUpdate();
	void Finalize();
	void Draw();

	void FrameDraw();
	void PassCar(Vector3 pos, bool SoloHardMode);

	void StopCar();

	GameStatus GetStatus();

	void NormaCount();

	void SetCameraPos(Vector3 camPos, Vector3 targetPos);

	void SetIsDeadAnimation(bool isDeadAnimation);

	void SetDeadAnimationPos(Vector3 carPos);
	void Load();

	void ModeSelectDraw();

	void ModeSelectDraw2D();

	void TitleDraw();

	void scoreDraw();

	void ResultDraw();

	void PressAnyKeyDraw();

	void CountDownDraw();

	void ConcentLineDraw();

	void MenuDraw();

	void CheckCarAllDead(bool isAllDead);

	Matrix4 GetCamMat();

	int GetCombo();

	void SetSoundManager(SoundManager *sound);


	void ComboObjectUpdate();

	void ComboObjectDraw();

	void AddTimeUpdate();

	int GetGameLevel();

	void SetMenuDatas(bool isMenu, int menuNumber);

	void Retry();
	void ToTitle();

	void ToResult();
	void SetMenuDone(bool menuDone);

	void SetGameNum(GameNum num);

	bool GetisGameOver();

	int SendObstacles();

	/// <summary>
	/// ã~ã}é‘ÇèoåªÇ≥ÇπÇÈÇÊÇ§Ç…óvêøÇ∑ÇÈ
	/// </summary>
	/// <returns></returns>
	int RequestToPopAmbulance();

	void SetIsSoloMode(bool isSoloMode);

	bool GetToIngameFlag();
private:
	//sceneà⁄ìÆä÷êî
	void ToIngame();

	void ToSelect();
public:
	const int baseScore = 100;
private:

	int score = 0;
	int combo = 0;
	bool isSendObstacles = false;

	int maxNormaCount = 25;

	int normaCars = 0;
	int TimeLimit = 3600;
	int gameTimer_ = 0;
	int level_ = 0.0f;

	std::array<int, 10> numberObjects_;

	int scoreTextObject_;
	Vector3 scoreObjectPos_;
	Vector3 scoreNumberObjectPos_;
	float scoreObjectAnimationRate_ = 0.0f;
	float addScoreAnimationRate_ = 0.0f;


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
	float countDownAnimationRate_ = 0.0f;
	int oldCount = 0;

	int TitleObject_;
	Vector3 titleObjectPos_;
	float titleObjectAnimationRate_ = 0.0f;

	int pressAnyKeyHandle_;
	Vector3 pressAnyKeyObjectPos_;
	float pressAnyKeyObjectAnimationRate_ = 0.0f;

	int AddHandle_;
	int SubHandle_;

	Vector3 AddSecObjectPos_;
	float AddSecObjectAnimationRate_ = 1.0f;
	float SubSecObjectAnimationRate_ = 1.0f;
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
	Vector3 CrashAnimationTargetPos_;

	Vector3 cameraDeadAnimationPos_;

	SoundManager *sounds_ = nullptr;

	bool isOlddeadAnimation = false;

	float AddSec = 2.5f;
	int SubSec = 3;

	int winHandle_ = -1;
	int loseHandle_ = -1;

	std::vector<UpperComboObject> comboPos;

	int gameLevel_ = 0;
	bool isLevelUp = false;
	int elapsedTime_ = 0;

	std::array<int, 2> concentLineHandles_;
	int drawConcentLineNum = 0;
	bool drawConcentTurnFlag = false;

	int BackText;

	std::array<int, 4> menuTexts;
	std::array<float, 4> textScales;
	int EndText;
	int MenuText;
	int RetryText;
	int TitleText;

	bool isMenu = false;
	int menuNumber = 0;

	float menuScale = 0.0f;
	bool menuScaleRoundTripFlag = true;
	bool menuDone = false;
	GameNum gameNumber = GameNum::SOLO;

	bool isFlipConcentLine = false;
	bool isSolo = true;
	int life = 3;

	int P1frameHandle = -1;
	int P2frameHandle = -1;
	int P1UIframeHandle = -1;
	int P2UIframeHandle = -1;

	int frameHandle = -1;
	int lifeHandle = -1;
	int lifeEmptyHandle = -1;

	/// <summary>
	/// ëäéËÇ…èoåªÇ≥ÇπÇƒÇ‡ÇÁÇ§ã~ã}é‘ÇÃêî
	/// </summary>
	int requestToPopAmbulanceCount = 0;
	bool isWin = true;

	int VSModeHandle_ = -1;
	int ScoreModeHandle_ = -1;

	int VSModeTextureHandle_ = -1;
	int ScoreModeTextureHandle_ = -1;

	float leftModeScaleRate = 0.0f;
	float baseScaleRate = 0.0f;

	bool modeSelected = false;

	static const float pickupModeScale;
	static const float unPickupModeScale;

	Vector3 SelectedModeObjectPos_;
	Vector3 NotSelectedModeObjectPos_;
	float ModeAnimationRate_ = 0.0f;

	bool isSoloMode = true;

	bool ToIngameFlag = false;
public:
	static const int gameLevelMax;
};

