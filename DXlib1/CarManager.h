#pragma once
#include "Car.h"
#include "CarBlastParticle.h"
#include <memory>
#include <vector>
#include <list>
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
	bool AddEnemyCar();

	void SetSignal();

	void Collision();

	int GetPassCars();

	bool GetAnyCarStop();

	void EndGame();

	void SetGameSpeed(float speed);

	void DrwaHud();

	void LoadGraphics();

	bool GetDeadAnimation();
private:
	void IngameUpdate();

	void OutGameUpdate();
private:
	static const float sCarWidthPos;

	static CarInitializeDesc sNormalCar;

	static CarInitializeDesc sTrackCar;
	static float sGameSpeed;

	static int up, right, down, left;

	static const int sDeadAnimationTimerMax;
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



	bool isDeadAnimation_ = false;
	bool isIngame_ = false;
	int deadAnimationTimer_ = 0;
	CarBlastParticle playerBlast;
	CarBlastParticle enemyBlast;
};

