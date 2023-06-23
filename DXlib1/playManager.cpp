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
	frames[0] = LoadGraph("Resources/Texture/frame_1p.png");
	frames[1] = LoadGraph("Resources/Texture/frame_2p.png");
}

void playManager::Update()
{
	if (a.GetGameStatus() == GameStatus::TITLE && GameInput::ChangeMode())
	{
		PlayModeChange();
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
		DrawGraph(0,0,frames[0],TRUE);
		DrawGraph(640, 0, frames[1], TRUE);

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
