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


public:
	const int baseScore = 100;
private:

	int score = 0;
	int combo = 0;

	GameStatus status_ = GameStatus::TITLE;
};

