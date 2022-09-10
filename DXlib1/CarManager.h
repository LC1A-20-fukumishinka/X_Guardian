#pragma once
#include "Car.h"
#include <memory>
#include <vector>
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
private:
	static const float sCarWidthPos;

	static CarInitializeDesc normalCar;
private:
	std::vector<std::shared_ptr<Car>> playerCars_;
	std::vector<std::shared_ptr<Car>> enemyCars_;

	std::weak_ptr<Car>playerEndCar;
	std::weak_ptr<Car>enemyEndCar;

	MoveType inputSignal = MoveType::STRAIGHT;

	MoveType testPlayer = MoveType::STRAIGHT;
	MoveType testEnemy = MoveType::STRAIGHT;

};

