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
};

