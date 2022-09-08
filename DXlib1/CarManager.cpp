#include "CarManager.h"

using namespace std;
CarManager::CarManager()
{
	playerCars_.reserve(30);
	playerCars_.resize(30);
	enemyCars_.reserve(30);
	enemyCars_.resize(30);
	for (auto &e : playerCars_)
	{
		e = make_shared<Car>();
	}
	for (auto &e : enemyCars_)
	{
		e = make_shared<Car>();
	}
}

CarManager::~CarManager()
{
}

void CarManager::Init()
{
}

void CarManager::Update()
{
	SetSignal();
	for (auto &e : playerCars_)
	{
		if (e->GetIsAlive())
		{
			e->Update();
		}
	}
	for (auto &e : enemyCars_)
	{
		if (e->GetIsAlive())
		{
			e->Update();
		}
	}
}

void CarManager::Finalize()
{
}

void CarManager::Draw()
{
	for (auto &e : playerCars_)
	{
		if (e->GetIsAlive())
		{
			e->Draw();
		}
	}
	for (auto &e : enemyCars_)
	{
		if (e->GetIsAlive())
		{
			e->Draw();
		}
	}
}

bool CarManager::AddEnemyCar()
{
	CarInitializeDesc desc;
	bool isSpawn = false;
	desc.angle = Vector3(0, 0, -1);
	desc.startPos = Vector3(20.0f, 0.0f, 100.0f);
	desc.isPlayer = false;

	desc.speed = 0.3f;

	for (auto &e : enemyCars_)
	{
		if (!e->GetIsAlive())
		{
			//‰Šú‰»
			e->Init(desc);
			isSpawn = true;

			//‚Ğ‚Æ‚Â‘O‚ÌÔ—¼‚ª‘¶İ‚·‚é‚È‚ç
			if (!enemyEndCar.expired())
			{
				e->SetFrontCar(enemyEndCar);
			}
			//©•ª‚ªÅŒã”öÔ—¼‚É‚È‚é
			enemyEndCar = e;
			break;
		}
	}

	return isSpawn;
}

void CarManager::SetSignal()
{
	if (CheckHitKey(KEY_INPUT_W))
	{
		inputSignal = MoveType::STRAIGHT;
	}

	if (CheckHitKey(KEY_INPUT_D))
	{
		inputSignal = MoveType::RIGHTTURN;
	}

	if (CheckHitKey(KEY_INPUT_S))
	{
		inputSignal = MoveType::STOP;
	}

	for (auto &e : playerCars_)
	{
		bool isStop = (e->GetMoveType() != inputSignal);
		e->SetSignal(isStop);
	}
}

bool CarManager::AddPlayerCar()
{
	CarInitializeDesc desc;
	bool isSpawn = false;
	desc.angle = Vector3(0, 0, 1);
	desc.startPos = Vector3(0.0f, 0.0f, -100.0f);
	desc.speed = 0.3f;
	desc.isPlayer = true;
	desc.type = MoveType::RIGHTTURN;
	for (auto &e : playerCars_)
	{
		if (!e->GetIsAlive())
		{
			e->Init(desc);
			isSpawn = true;

			//‚Ğ‚Æ‚Â‘O‚ÌÔ—¼‚ª‘¶İ‚·‚é‚È‚ç
			if (!playerEndCar.expired())
			{
				e->SetFrontCar(playerEndCar);
			}
			//©•ª‚ªÅŒã”öÔ—¼‚É‚È‚é
			playerEndCar = e;

			break;
		}
	}

	return isSpawn;
}
