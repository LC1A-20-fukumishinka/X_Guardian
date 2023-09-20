#pragma once
#include "Game.h"
#include "SoundManager.h"
class playManager
{
public:
	void Init();
	void Update();
	void Finalize();
	void Draw();

	bool GameEnd();
private:
	Game a, b;
	std::unique_ptr<SoundManager> sounds;
	void PlayModeChange();

	bool isSolo = true;

	std::array<int, 2> frames;

	bool ALose = false;
	bool BLose = false;
	bool isFinish = false;

	int aObstaclesCount = 0;
	int bObstaclesCount = 0;

	bool isSoloModeSelect = true;
};

