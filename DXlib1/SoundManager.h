#pragma once
class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	void Load();
	void ChangeInitializeVolume();

	void TitleVolume();

	void IngameVolume();

	void ResultVolume();
	void Action();
	void Whistle();
	void BGM();
	void BGMStop();
	void ContinueBGM();
	void Broken();
	void Brake(int num = -1);
	void Enter();
	void Explosion(int num = -1);
	void Horn(int num = -1);
	void Engine(int num = -1);
	void EngineStop(int num = 2);
	bool EngineCheck(int num = 2);
	void Slow();
	void Combo();
	void TimeUp();
	void Crap();
	void AddTime();
	void Buzzer();
	void Jingle();

	void StopJingle();

	void TitleExplosion();
private:
	int action_;
	int BGM_;
	int brake_;
	int softBrake_;
	int broken_;
	int enter_;
	int explosion1_;
	int explosion2_;
	int explosion3_;
	int horn1_;
	int horn2_;
	int horn3_;
	int leaveCar_;
	int comingCar_;
	int normalCar_;
	int slow_;
	int combo_;
	int Timeup_;
	int AddTime_;
	int Crap_;
	int Buzzer_;
	int jingle_;

	int TitleExplosion1_;
	int TitleExplosion2_;
	int TitleExplosion3_;
	int TitleBroken_;

	int upperVolume = 50;

};

