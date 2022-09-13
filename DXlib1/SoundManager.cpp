#include "SoundManager.h"
#include "DxLib.h"

SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
}

void SoundManager::Load()
{
	action_ = LoadSoundMem("Resources/sound/action.wav");
	BGM_ = LoadSoundMem("Resources/sound/BGM.mp3");
	brake_ = LoadSoundMem("Resources/sound/brake.wav");
	enter_ = LoadSoundMem("Resources/sound/enter.mp3");
	explosion1_ = LoadSoundMem("Resources/sound/explosion_01.wav");
	explosion2_ = LoadSoundMem("Resources/sound/explosion_02.wav");
	explosion3_ = LoadSoundMem("Resources/sound/explosion_03.wav");
	horn1_ = LoadSoundMem("Resources/sound/horn_01.wav");
	horn2_ = LoadSoundMem("Resources/sound/horn_02.wav");
	horn3_ = LoadSoundMem("Resources/sound/horn_03.wav");
	leaveCar_ = LoadSoundMem("Resources/sound/move_car_01.wav");
	comingCar_ = LoadSoundMem("Resources/sound/move_car_02.wav");
	slow_ = LoadSoundMem("Resources/sound/slow.wav");
}

void SoundManager::Action()
{
	PlaySoundMem(action_, DX_PLAYTYPE_BACK);
}

void SoundManager::BGM()
{
	if (CheckSoundMem(BGM_))
	{
		StopSoundMem(BGM_);
	}
	else
	{
		PlaySoundMem(BGM_, DX_PLAYTYPE_LOOP);
	}
}

void SoundManager::Brake()
{
	PlaySoundMem(brake_, DX_PLAYTYPE_BACK);
}

void SoundManager::Enter()
{
	PlaySoundMem(enter_, DX_PLAYTYPE_BACK);
}

void SoundManager::Explosion(int num)
{
	int playNum;
	if (num >= 0 && num <= 2)
	{
		playNum =num;
	}
	else
	{
		playNum = rand() % 3;
	}

	if (playNum == 0)
	{
		PlaySoundMem(explosion1_, DX_PLAYTYPE_BACK);
	}
	else if (playNum == 1)
	{
		PlaySoundMem(explosion2_, DX_PLAYTYPE_BACK);
	}
	else
	{
		PlaySoundMem(explosion3_, DX_PLAYTYPE_BACK);
	}
}

void SoundManager::Horn(int num)
{
	int playNum;
	if (num >= 0 && num <= 2)
	{
		playNum = num;
	}
	else
	{
		playNum = rand() % 3;
	}

	if (playNum == 0)
	{
		PlaySoundMem(horn1_, DX_PLAYTYPE_BACK);
	}
	else if (playNum == 1)
	{
		PlaySoundMem(horn2_, DX_PLAYTYPE_BACK);
	}
	else
	{
		PlaySoundMem(horn3_, DX_PLAYTYPE_BACK);
	}
}

void SoundManager::Engine(int num)
{
	int playNum;
	if (num >= 0 && num <= 1)
	{
		playNum = num;
	}
	else
	{
		playNum = rand() % 2;
	}

	if (playNum == 0)
	{
		PlaySoundMem(leaveCar_, DX_PLAYTYPE_BACK);
	}
	else
	{
		PlaySoundMem(comingCar_, DX_PLAYTYPE_BACK);
	}
}

void SoundManager::Slow()
{
	PlaySoundMem(slow_, DX_PLAYTYPE_BACK);
}
