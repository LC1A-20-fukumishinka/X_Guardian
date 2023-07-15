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
	AddTime_ = LoadSoundMem("Resources/sound/add_time.wav");
	Crap_ = LoadSoundMem("Resources/sound/crap.mp3");
	Buzzer_ = LoadSoundMem("Resources/sound/buzzer.mp3");

	jingle_ = LoadSoundMem("Resources/sound/result.mp3");
	Siren_ = LoadSoundMem("Resources/sound/siren.wav");

	int TitleExplosion1_ = DuplicateSoundMem(explosion1_);
	int TitleExplosion2_ = DuplicateSoundMem(explosion2_);
	int TitleExplosion3_ = DuplicateSoundMem(explosion3_);
	int TitleBroken = DuplicateSoundMem(broken_);;

	ChangeInitializeVolume();
}

void SoundManager::ChangeInitializeVolume()
{
	int volume = 100;

	ChangeVolumeSoundMem(volume + upperVolume, action_);
	ChangeVolumeSoundMem(volume + upperVolume, BGM_);
	ChangeVolumeSoundMem(volume + upperVolume, brake_);
	ChangeVolumeSoundMem(volume + upperVolume, enter_);
	ChangeVolumeSoundMem(volume + upperVolume, explosion1_);
	ChangeVolumeSoundMem(volume + upperVolume, explosion2_);
	ChangeVolumeSoundMem(volume + upperVolume, explosion3_);
	ChangeVolumeSoundMem(volume + upperVolume, horn1_);
	ChangeVolumeSoundMem(volume + upperVolume, horn2_);
	ChangeVolumeSoundMem(volume + upperVolume, horn3_);
	ChangeVolumeSoundMem(volume + upperVolume, leaveCar_);
	ChangeVolumeSoundMem(volume + upperVolume, comingCar_);

	ChangeVolumeSoundMem(volume + upperVolume, softBrake_);
	ChangeVolumeSoundMem(volume + upperVolume, normalCar_);
	ChangeVolumeSoundMem(volume + upperVolume, broken_);
	ChangeVolumeSoundMem(volume + upperVolume, combo_);
	ChangeVolumeSoundMem(volume + upperVolume, Timeup_);
	ChangeVolumeSoundMem(volume + upperVolume + 50, AddTime_);
	ChangeVolumeSoundMem(200 + upperVolume + 20, Crap_);
	ChangeVolumeSoundMem(130 + upperVolume + 20, jingle_);
	ChangeVolumeSoundMem(150, Siren_);

	int titleEexplosionVolume = 75;
	ChangeVolumeSoundMem(titleEexplosionVolume + upperVolume, TitleExplosion1_);
	ChangeVolumeSoundMem(titleEexplosionVolume + upperVolume, TitleExplosion2_);
	ChangeVolumeSoundMem(titleEexplosionVolume + upperVolume, TitleExplosion3_);
	ChangeVolumeSoundMem((titleEexplosionVolume * (0.666f)) + upperVolume, TitleBroken_);
}

void SoundManager::TitleVolume()
{
	int volume = 200;

	ChangeVolumeSoundMem(60 + upperVolume, action_);
	ChangeVolumeSoundMem(50 + upperVolume, Timeup_);

	ChangeVolumeSoundMem(50 + upperVolume, brake_);
	ChangeVolumeSoundMem(40 + upperVolume, softBrake_);


	ChangeVolumeSoundMem(40 + upperVolume, horn1_);
	ChangeVolumeSoundMem(40 + upperVolume, horn2_);
	ChangeVolumeSoundMem(40 + upperVolume, horn3_);
	ChangeVolumeSoundMem(60 + upperVolume, combo_);


	ChangeVolumeSoundMem(60 + upperVolume, leaveCar_);
	ChangeVolumeSoundMem(60 + upperVolume, comingCar_);
	ChangeVolumeSoundMem(100 + upperVolume, normalCar_);


	//ChangeVolumeSoundMem(volume, explosion1_);
	//ChangeVolumeSoundMem(volume, explosion2_);
	//ChangeVolumeSoundMem(volume, explosion3_);
	ChangeVolumeSoundMem(100 + upperVolume, broken_);


}

void SoundManager::IngameVolume()
{
	int volume = 100;

	int explosionVolume = 150;
	ChangeVolumeSoundMem(volume + upperVolume , action_);
	ChangeVolumeSoundMem(volume + upperVolume + 50, BGM_);
	ChangeVolumeSoundMem(volume + upperVolume, brake_);
	ChangeVolumeSoundMem(volume + upperVolume, enter_);
	ChangeVolumeSoundMem(explosionVolume + upperVolume, explosion1_);
	ChangeVolumeSoundMem(explosionVolume + upperVolume, explosion2_);
	ChangeVolumeSoundMem(explosionVolume + upperVolume, explosion3_);
	ChangeVolumeSoundMem(volume + upperVolume, horn1_);
	ChangeVolumeSoundMem(volume + upperVolume, horn2_);
	ChangeVolumeSoundMem(volume + upperVolume, horn3_);
	ChangeVolumeSoundMem(volume + upperVolume, leaveCar_);
	ChangeVolumeSoundMem(volume + upperVolume, comingCar_);

	ChangeVolumeSoundMem(volume + upperVolume, softBrake_);
	ChangeVolumeSoundMem(volume + upperVolume, normalCar_);
	ChangeVolumeSoundMem(volume + upperVolume, broken_);
	ChangeVolumeSoundMem(volume + upperVolume + 50, combo_);
	ChangeVolumeSoundMem(volume + upperVolume, Timeup_);
	ChangeVolumeSoundMem(190 + upperVolume, Buzzer_);
}

void SoundManager::ResultVolume()
{
	ChangeVolumeSoundMem(60 + upperVolume, BGM_);
}

void SoundManager::Action()
{
	PlaySoundMem(action_, DX_PLAYTYPE_BACK);
}

void SoundManager::Whistle()
{
	int playNum = (rand() % 2);

	if (playNum == 0)
	{
		PlaySoundMem(action_, DX_PLAYTYPE_BACK);
	}
	else if (playNum == 1)
	{
		PlaySoundMem(Timeup_, DX_PLAYTYPE_BACK);
	}
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
		playNum = (rand() % 2);
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
		playNum = (rand() % 3);
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
	if (num >= 0 && num <= 3)
	{
		playNum = num;
	}
	else
	{
		playNum = (rand() % 4);
	}

	if (playNum == 0)
	{
		PlaySoundMem(horn1_, DX_PLAYTYPE_BACK);
	}
	else if (playNum == 1)
	{
		PlaySoundMem(horn2_, DX_PLAYTYPE_BACK);
	}
	else if (playNum == 2)
	{
		PlaySoundMem(horn3_, DX_PLAYTYPE_BACK);
	}
	else
	{
		PlaySoundMem(combo_, DX_PLAYTYPE_BACK);
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
		playNum = (rand() % 3);
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

void SoundManager::Crap()
{
	PlaySoundMem(Crap_, DX_PLAYTYPE_BACK);
}

void SoundManager::AddTime()
{
	PlaySoundMem(AddTime_, DX_PLAYTYPE_BACK);
}

void SoundManager::Buzzer()
{
	StopSoundMem(Buzzer_);
	PlaySoundMem(Buzzer_, DX_PLAYTYPE_BACK);
}

void SoundManager::Jingle()
{
	PlaySoundMem(jingle_, DX_PLAYTYPE_BACK);
}

void SoundManager::Siren()
{
	PlaySoundMem(Siren_, DX_PLAYTYPE_BACK);
}

void SoundManager::SirenStop()
{
	StopSoundMem(Siren_);
}

void SoundManager::StopJingle()
{
	StopSoundMem(jingle_);
}

void SoundManager::TitleExplosion()
{
	int playNum;

		playNum = (rand() % 3);

	if (playNum == 0)
	{
		PlaySoundMem(TitleExplosion1_, DX_PLAYTYPE_BACK);
	}
	else if (playNum == 1)
	{
		PlaySoundMem(TitleExplosion1_, DX_PLAYTYPE_BACK);
	}
	else
	{
		PlaySoundMem(TitleExplosion1_, DX_PLAYTYPE_BACK);
	}

	PlaySoundMem(TitleBroken_, DX_PLAYTYPE_BACK);
}
