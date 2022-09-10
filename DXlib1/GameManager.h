#pragma once

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

	int maxNormaCount = 5;

	int normaCars = 0;
	int TimeLimit = 6500;
	int gameTimer_ = 0;

	bool isClear_ = false;
	bool isFailed_ = false;

	bool isInput_ = false;
	GameStatus status_ = GameStatus::TITLE;
};

