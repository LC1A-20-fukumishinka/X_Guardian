#pragma once
#include "Car.h"
#include "CarBlastParticle.h"
#include <memory>
#include <vector>
#include <list>
#include "Matrix4.h"
class SoundManager;
class CarManager
{
public:
	CarManager();
	~CarManager();

	void Init();
	void Update();
	void Finalize();
	void Draw();

	bool AddPlayerCar();
	bool AddEnemyCar(bool isTitle);

	void SetSignal();

	void Collision();

	std::vector<Vector3> GetPassCars();

	bool GetAnyCarStop();

	void EndGame();

	void SetGameSpeed(float speed);

	void DrwaHud();

	void LoadGraphics();

	bool GetDeadAnimation();

	bool GetIsAllCarDead();

	bool GetIsNotTrackMove();
	MoveType GetInputSignal();

	void SetCamMat(Matrix4 mat);

	void SetSoundManager(SoundManager *sounds);

	void SetIsResult(bool isResult);
	
	void AllDead();

	void SetGaugeRate(float rate);

	void SetLevel(int level);
private:
	void IngameUpdate();

	void OutGameUpdate();
private:
	static const float sCarWidthPos;

	static CarInitializeDesc sNormalCar;

	static CarInitializeDesc sTrackCar;
	static float sGameSpeed;

	static int up, right, down, left, stop;
	static int sNextModel, sStraight, sStop,sRight;

	static int sActFrameModel, sGuideModel;
	static const int sDeadAnimationTimerMax;

	static std::vector<int> sArrowModelHandle;
	static int sCautionHandle;
	static int sTimeBar;

private:
	std::vector<std::shared_ptr<Car>> playerCars_;
	std::vector<std::shared_ptr<Car>> enemyCars_;

	std::weak_ptr<Car>playerEndCar;
	std::weak_ptr<Car>enemyEndCar;

	std::weak_ptr<Car>deadPlayerCar_;
	std::weak_ptr<Car>deadEnemyCar_;

	std::list<std::weak_ptr<Car>>alivePlayerCars_;
	std::list<std::weak_ptr<Car>>aliveEnemyCars_;

	MoveType inputSignal = MoveType::STRAIGHT;

	Matrix4 camMat_;

	Vector3 nextFrameObject_;
	Vector3 nextObject_;
	Vector3 nextnextObject_;
	float nextAnimationRate_ = 0.0f;


	Vector3 actFrameObject_;
	Vector3 actObject;

	Vector3 guideObject_;

	bool isDeadAnimation_ = false;

	bool isResult_ = false;
	bool isIngame_ = false;

	bool isChangeSignal = false;
	float inpuAnimation = 0.0f;

	float gaugeRate_ = 0.0f;
	int deadAnimationTimer_ = 0;

	bool isNextDraw_ = false;
	int nextDrawHandle = -1;
	float nextColorRate_ = 0.0f;
	bool nextColorAnimation = false;
	int level_ = 0;
	CarBlastParticle playerBlast;
	CarBlastParticle enemyBlast;
	CarBlastParticle ArrowBlast;
	std::vector<CarBlastParticle> TitleParticles;
	SoundManager *sounds_ = nullptr;
};

