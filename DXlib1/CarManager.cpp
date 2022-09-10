#include "CarManager.h"
#include "Collision.h"
using namespace std;

const float CarManager::sCarWidthPos = 10.0f;
CarInitializeDesc CarManager::normalCar;
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

	normalCar.length = 11.0f;
	normalCar.radius = 4.0f;
}

CarManager::~CarManager()
{
}

void CarManager::Init()
{
	Car::SetSignal(MoveType::STRAIGHT);

}

void CarManager::Update()
{
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

int CarManager::GetPassCars()
{
	int passCarCount = 0;
	for (auto &e : playerCars_)
	{
		if (e->GetIsAlive())
		{
			if (e->GetIsPass())
			{
				e->Count();
				passCarCount++;
			}
		}
	}
	for (auto &e : enemyCars_)
	{
		if (e->GetIsAlive())
		{

			if (e->GetIsPass())
			{
				e->Count();
				passCarCount++;
			}
		}
	}
	return passCarCount;
}

bool CarManager::GetAnyCarStop()
{
	bool isStop = false;
	for (auto &e : playerCars_)
	{
		if (e->GetIsAlive())
		{
			if (e->GetIsSignalStop())
			{
			isStop = true;
			break;
			}
		}
	}
	return isStop;
}

void CarManager::EndGame()
{
	Car::SetSignal(MoveType::ALLOK);
}

bool CarManager::AddEnemyCar()
{
	CarInitializeDesc desc = normalCar;
	bool isSpawn = false;
	desc.angle = Vector3(0, 0, -1);
	desc.startPos = Vector3(sCarWidthPos, 0.0f, 500.0f);
	desc.isPlayer = false;

	desc.speed = 0.6f;
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
	CarInitializeDesc desc = normalCar;
	bool isSpawn = false;
	desc.angle = Vector3(0, 0, 1);
	desc.startPos = Vector3(-sCarWidthPos, 0.0f, -300.0f);
	desc.speed = 1.0f;
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
