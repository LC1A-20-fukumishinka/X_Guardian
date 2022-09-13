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
	softBrake_ = LoadSoundMem("Resources/sound/new_brake.wav");
	enter_ = LoadSoundMem("Resources/sound/enter.mp3");
	explosion1_ = LoadSoundMem("Resources/sound/explosion_01.wav");
	explosion2_ = LoadSoundMem("Resources/sound/explosion_02.wav");
	explosion3_ = LoadSoundMem("Resources/sound/explosion_03.wav");
	horn1_ = LoadSoundMem("Resources/sound/horn_01.wav");
	horn2_ = LoadSoundMem("Resources/sound/horn_02.wav");
	horn3_ = LoadSoundMem("Resources/sound/horn_03.wav");
	leaveCar_ = LoadSoundMem("Resources/sound/move_car_01.wav");
	comingCar_ = LoadSoundMem("Resources/sound/move_car_02.wav");
	normalCar_ = LoadSoundMem("Resources/sound/move_car_03.wav");
	slow_ = LoadSoundMem("Resources/sound/slow.wav");
	broken_ = LoadSoundMem("Resources/sound/break_car.wav");
	combo_ = LoadSoundMem("Resources/sound/combo.mp3");
	Timeup_ = LoadSoundMem("Resources/sound/game_end.mp3");
}

void SoundManager::ChangeVolume()
{
	ChangeVolumeSoundMem(150, action_);
	ChangeVolumeSoundMem(150, BGM_);
	ChangeVolumeSoundMem(150, brake_);
	ChangeVolumeSoundMem(150, enter_);
	ChangeVolumeSoundMem(150, explosion1_);
	ChangeVolumeSoundMem(150, explosion2_);
	ChangeVolumeSoundMem(150, explosion3_);
	ChangeVolumeSoundMem(150, horn1_);
	ChangeVolumeSoundMem(150, horn2_);
	ChangeVolumeSoundMem(150, horn3_);
	ChangeVolumeSoundMem(150, leaveCar_);
	ChangeVolumeSoundMem(150, comingCar_);
	ChangeVolumeSoundMem(150, slow_);
}

void SoundManager::Action()
{
	PlaySoundMem(action_, DX_PLAYTYPE_BACK);
}

void SoundManager::BGM()
{
	PlaySoundMem(BGM_, DX_PLAYTYPE_LOOP);
}

void SoundManager::BGMStop()
{
	if (CheckSoundMem(BGM_))
	{
		StopSoundMem(BGM_);
	}
}

void SoundManager::ContinueBGM()
{
	PlaySoundMem(BGM_, DX_PLAYTYPE_LOOP, FALSE);
}

void SoundManager::Broken()
{
	PlaySoundMem(broken_, DX_PLAYTYPE_BACK);
}

void SoundManager::Brake(int num)
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
		PlaySoundMem(brake_, DX_PLAYTYPE_BACK);
	}
	else if (playNum == 1)
	{
		PlaySoundMem(softBrake_, DX_PLAYTYPE_BACK);
	}
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
		playNum = num;
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
		PlaySoundMem(leaveCar_, DX_PLAYTYPE_BACK);
	}

	else if (playNum == 1)
	{
		PlaySoundMem(comingCar_, DX_PLAYTYPE_BACK);
	}
	else
	{
		PlaySoundMem(normalCar_, DX_PLAYTYPE_BACK);
	}
}

void SoundManager::EngineStop(int num)
{
	int playNum;
	if (num >= 0 && num <= 2)
	{
		playNum = num;
	}
	else
	{
		playNum = 2;
	}

	if (playNum == 0)
	{
		StopSoundMem(leaveCar_);
	}

	else if (playNum == 1)
	{
		StopSoundMem(comingCar_);
	}
	else
	{
		StopSoundMem(normalCar_);
	}
}

bool SoundManager::EngineCheck(int num)
{
	int playNum;
	if (num >= 0 && num <= 2)
	{
		playNum = num;
	}
	else
	{
		playNum = 2;
	}

	bool isSound = false;
	if (playNum == 0)
	{
		isSound = CheckSoundMem(leaveCar_);
	}
	else if (playNum == 1)
	{
		isSound = CheckSoundMem(comingCar_);
	}
	else
	{
		isSound = CheckSoundMem(normalCar_);
	}
	return isSound;
}

void SoundManager::Slow()
{
	PlaySoundMem(slow_, DX_PLAYTYPE_BACK);
}

void SoundManager::Combo()
{
	PlaySoundMem(combo_, DX_PLAYTYPE_BACK);
}

void SoundManager::TimeUp()
{
	PlaySoundMem(Timeup_, DX_PLAYTYPE_BACK);
}
