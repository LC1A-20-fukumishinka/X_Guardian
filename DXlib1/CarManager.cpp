#include "CarManager.h"
#include "Collision.h"
#include "DxLib.h"
using namespace std;

const float CarManager::sCarWidthPos = 10.0f;
CarInitializeDesc CarManager::normalCar;
float CarManager::sGameSpeed = 1.0f;
const int CarManager::sDeadAnimationTimerMax = 150;

int CarManager::up, CarManager::right, CarManager::down, CarManager::left;

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
	isIngame_ = true;
	SetGameSpeed(1.0f);
}

void CarManager::Update()
{
	Car::SetGameSpeed(sGameSpeed);

	if (isIngame_)
	{
		IngameUpdate();
	}
	else
	{
		OutGameUpdate();
	}

	alivePlayerCars_.remove_if([](std::weak_ptr<Car> &x)
		{
			bool test = (!x.lock()->GetIsAlive() || x.lock()->GetIsPass());

			if (test)
			{
				return true;
			}
			else
			{
				return false;
			}

		});
	aliveEnemyCars_.remove_if([](std::weak_ptr<Car> &x)
		{
			return (!x.lock()->GetIsAlive() || x.lock()->GetIsPass());
		});
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
						deadPlayerCar_ = pCar;
						deadEnemyCar_ = eCar;
						isDeadAnimation_ = true;
						deadAnimationTimer_ = 0;
						SetGameSpeed(0.01f);
						Car::SetSignal(MoveType::STOP);
						break;
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

	SetGameSpeed(3.0f);

	isIngame_ = false;
}

void CarManager::SetGameSpeed(float speed)
{
	sGameSpeed = speed;
}

void CarManager::DrwaHud()
{
	if (!alivePlayerCars_.empty())
	{
		bool isNotFirst = false;
		int carsCount = 0;
		for (auto &e : alivePlayerCars_)
		{
			int drawHandle = -1;
			if (e.lock()->GetMoveType() == MoveType::STRAIGHT)
			{
				drawHandle = up;
			}
			else
			{
				drawHandle = right;
			}
			int x = 200, y = 300;
			
			if (isNotFirst)
			{
				DrawExtendGraph(x-23, y + (20 * (carsCount - 1)), x - 3, y + (20 * (carsCount)), drawHandle, TRUE);

			}
			else
			{
				DrawExtendGraph(x, y, x + 100, y + 100, drawHandle, TRUE);
				isNotFirst = true;
			}
			carsCount++;
		}
	}

	if (!aliveEnemyCars_.empty())
	{
		bool isNotFirst = false;
		int carsCount = 0;
		for (auto &e : aliveEnemyCars_)
		{
			int drawHandle = -1;
			if (e.lock()->GetMoveType() == MoveType::STRAIGHT)
			{
				drawHandle = down;
			}
			else
			{
				drawHandle = left;
			}

			int x = 700, y = 100;
			if (isNotFirst)
			{
				DrawExtendGraph(x + 3 + 100, y + (20 * (carsCount - 1)), x  + 23 + 100, y + (20 * (carsCount)), drawHandle, TRUE);

			}
			else
			{
				DrawExtendGraph(x, y, x + 100, y + 100, drawHandle, TRUE);
				isNotFirst = true;
			}
			carsCount++;
		}
	}

	int inputDrawHandle = -1;
	if (inputSignal == MoveType::STRAIGHT)
	{
		inputDrawHandle = up;
	}
	else if(inputSignal == MoveType::RIGHTTURN)
	{
		inputDrawHandle = right;
	}
	else
	{
		inputDrawHandle = down;
	}

	int x = 700, y = 300;
	DrawExtendGraph(x, y, x + 100, y + 100, inputDrawHandle, TRUE);

}

void CarManager::LoadGraphics()
{
	up = LoadGraph("Resources/Texture/upArrow.png");
	right = LoadGraph("Resources/Texture/rightArrow.png");
	down = LoadGraph("Resources/Texture/dawnArrow.png");
	left = LoadGraph("Resources/Texture/leftArrow.png");
}

bool CarManager::GetDeadAnimation()
{
return isDeadAnimation_;
}

void CarManager::IngameUpdate()
{

	if (isDeadAnimation_)
	{
		deadAnimationTimer_++;
		deadPlayerCar_.lock()->Update();
		deadEnemyCar_.lock()->Update();

		if (deadAnimationTimer_ >= sDeadAnimationTimerMax)
		{
			isDeadAnimation_ = false;
			deadPlayerCar_.lock()->Dead();
			deadEnemyCar_.lock()->Dead();
			SetGameSpeed(1.0f);
		}
	}
	else
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

}

void CarManager::OutGameUpdate()
{
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

bool CarManager::AddEnemyCar()
{
	CarInitializeDesc desc = normalCar;
	bool isSpawn = false;
	desc.angle = Vector3(0, 0, -1);
	desc.startPos = Vector3(sCarWidthPos, 0.0f, 500.0f);
	desc.isPlayer = false;

	desc.speed = 1.8f;
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
			//èâä˙âª
			e->Init(desc);
			isSpawn = true;

			//Ç–Ç∆Ç¬ëOÇÃé‘óºÇ™ë∂ç›Ç∑ÇÈÇ»ÇÁ
			if (!enemyEndCar.expired())
			{
				e->SetFrontCar(enemyEndCar);
			}
			//é©ï™Ç™ç≈å„îˆé‘óºÇ…Ç»ÇÈ
			enemyEndCar = e;

			aliveEnemyCars_.emplace_back(e);
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
	desc.speed = 3.0f;
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

			//Ç–Ç∆Ç¬ëOÇÃé‘óºÇ™ë∂ç›Ç∑ÇÈÇ»ÇÁ
			if (!playerEndCar.expired())
			{
				e->SetFrontCar(playerEndCar);
			}
			//é©ï™Ç™ç≈å„îˆé‘óºÇ…Ç»ÇÈ
			playerEndCar = e;
			alivePlayerCars_.emplace_back(e);

			break;
		}
	}

	return isSpawn;
}
