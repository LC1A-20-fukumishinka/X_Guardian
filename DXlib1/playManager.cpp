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
	if (a.GetGameStatus() == GameStatus::TITLE && GameInput::ChangeMode())
	{
		PlayModeChange();
	}

	if (isFinish)
	{
		a.ToResult();
		b.ToResult();
		BLose = false;
		ALose = false;
		isFinish = false;
	}
	if (isSolo)
	{
		a.SetGameNum(GameNum::SOLO);
	}
	else
	{
		a.SetGameNum(GameNum::PLAYER1);
	}
	GameInput::Update();
	a.Update();
	a.IngameDraw();
	if (!isSolo)
	{
		b.Update();
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
