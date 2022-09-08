#include "CarManager.h"
#include "Collision.h"
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

	Collision();

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

	Car::SetSignal(inputSignal);
}

void CarManager::Collision()
{
	for (auto &pCar : playerCars_)
	{
		if (pCar->GetIsAlive())
		{
			for (auto &eCar : enemyCars_)
			{
				if (eCar->GetIsAlive())
				{
					bool isHit = Collision::testCapsuleCapsule(*pCar->GetCapsule(), *eCar->GetCapsule());
					if (isHit)
					{
					pCar->Dead();
					eCar->Dead();
					}
				}
			}
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
	desc.type = testEnemy;

	if (testEnemy == MoveType::STRAIGHT)
	{
		testEnemy = MoveType::RIGHTTURN;
	}
	else
	{
		testEnemy = MoveType::STRAIGHT;
	}

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

bool CarManager::AddPlayerCar()
{
	CarInitializeDesc desc;
	bool isSpawn = false;
	desc.angle = Vector3(0, 0, 1);
	desc.startPos = Vector3(0.0f, 0.0f, -100.0f);
	desc.speed = 0.3f;
	desc.isPlayer = true;
	desc.type = testPlayer;
	if (testPlayer == MoveType::STRAIGHT)
	{
		testPlayer = MoveType::RIGHTTURN;
	}
	else
	{
		testPlayer = MoveType::STRAIGHT;
	}

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
