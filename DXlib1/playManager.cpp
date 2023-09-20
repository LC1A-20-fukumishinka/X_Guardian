#include "playManager.h"
#include "GameInput.h"

void playManager::Init()
{
	sounds = std::make_unique<SoundManager>();
	sounds->Load();

	a.SetSoundManager(sounds.get());
	b.SetSoundManager(sounds.get());
	a.Init(true);
	b.Init(false);

	a.SetGameNum(GameNum::PLAYER1);
	b.SetGameNum(GameNum::PLAYER2);
}

void playManager::Update()
{
	if (a.GetGameStatus() == GameStatus::TITLE)
	{
		isSolo = true;
		isSoloModeSelect = true;
	}

	if (a.GetGameStatus() == GameStatus::SELECT)
	{
		if (GameInput::Left())
		{
			isSoloModeSelect = true;
		}

		if (GameInput::Right())
		{
			isSoloModeSelect = false;
		}
	}

	a.SetIsSoloMode(isSoloModeSelect);
	if (isFinish)
	{
		a.ToResult();
		b.ToResult();
		BLose = false;
		ALose = false;
		isFinish = false;
	}

	GameInput::Update();
	if (a.GetGameStatus() == GameStatus::SELECT && GameInput::Done(GameNum::SOLO))
	{
		isSolo = isSoloModeSelect;
	}
	if (isSolo)
	{
		a.SetGameNum(GameNum::SOLO);
	}
	else
	{
		a.SetGameNum(GameNum::PLAYER1);
	}

	a.Update(true);
	a.IngameDraw();
	if (!isSolo)
	{
		b.Update(isSolo);
		b.IngameDraw();
		BLose = b.isGameOver();
		ALose = a.isGameOver();
		isFinish = BLose || ALose;

		a.ReceiveObstacles(b.SendObstacles());
		b.ReceiveObstacles(a.SendObstacles());
	}


}

void playManager::Finalize()
{
	a.Finalize();
	b.Finalize();
}

void playManager::Draw()
{
	a.Draw();
	if (!isSolo)
	{
		b.Draw();

	}
}

bool playManager::GameEnd()
{
	return a.GameEnd();
}

void playManager::PlayModeChange()
{
	isSolo = !isSolo;
}
