#include "CarManager.h"
#include "Collision.h"
#include "DxLib.h"
#include "EaseClass.h"
#include <algorithm>
#include "SoundManager.h"

using namespace std;

const float CarManager::sCarWidthPos = 13.0f;
CarInitializeDesc CarManager::sNormalCar;
CarInitializeDesc CarManager::sTrackCar;
float CarManager::sGameSpeed = 1.0f;
const int CarManager::sDeadAnimationTimerMax = 90;

int CarManager::up, CarManager::right, CarManager::down, CarManager::left, CarManager::stop;
int CarManager::sNextModel, CarManager::sStraight, CarManager::sStop, CarManager::sRight;
int CarManager::sActFrameModel, CarManager::sGuideModel;
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

	sNormalCar.length = 11.0f;
	sNormalCar.radius = 4.0f;
	sNormalCar.model = ModelType::NORMAL;

	sTrackCar.length = 13.0f;
	sTrackCar.radius = 4.0f;
	sTrackCar.model = ModelType::TRACK;

	camMat_ = identity();

	actFrameObject_ = Vector3(-30.0f, 34.0f, -120.0f);
	actObject = actFrameObject_ + +Vector3(3, 0.0f, 0.0f);


	nextFrameObject_ = actFrameObject_ + Vector3(0.0f, -20.0f, -10.0f);
	nextObject_ = nextFrameObject_ + Vector3(3, 0.0f, 0.0f);
	nextnextObject_ = nextObject_ + Vector3(-7, 4.0f, 0.0f);




	guideObject_ = Vector3(20.0f, 35.0f, -160.0f);
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
	nextAnimationRate_ = clamp(nextAnimationRate_, 0.0f, 1.0f);

	alivePlayerCars_.remove_if([](std::weak_ptr<Car> &x)
		{
			return (!x.lock()->GetIsAlive() || x.lock()->GetIsPass());
		});




	aliveEnemyCars_.remove_if([](std::weak_ptr<Car> &x)
		{
			return (!x.lock()->GetIsAlive() || x.lock()->GetIsPass());
		});

	playerBlast.Update();
	enemyBlast.Update();
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
	playerBlast.Draw();
	enemyBlast.Draw();
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
						inputSignal = MoveType::STOP;
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

	SetGameSpeed(10.0f);

	isIngame_ = false;
}

void CarManager::SetGameSpeed(float speed)
{
	sGameSpeed = speed;
}

void CarManager::DrwaHud()
{

#pragma region enemyNextDraw
	//if (!aliveEnemyCars_.empty())
	//{
	//	bool isNotFirst = false;
	//	int carsCount = 0;
	//	for (auto &e : aliveEnemyCars_)
	//	{
	//		int drawHandle = -1;
	//		if (e.lock()->GetMoveType() == MoveType::STRAIGHT)
	//		{
	//			drawHandle = down;
	//		}
	//		else
	//		{
	//			drawHandle = left;
	//		}

	//		int x = 700, y = 100;
	//		if (isNotFirst)
	//		{
	//			DrawExtendGraph(x + 3 + 100, y + (20 * (carsCount - 1)), x + 23 + 100, y + (20 * (carsCount)), drawHandle, TRUE);

	//		}
	//		else
	//		{
	//			DrawExtendGraph(x, y, x + 100, y + 100, drawHandle, TRUE);
	//			isNotFirst = true;
	//		}
	//		carsCount++;
	//	}
	//}
#pragma endregion


	float nextRate, inputRate, allRate, lag;
	lag = 0.1f;
	allRate = nextAnimationRate_ * (1.0f + lag);
	nextRate = std::clamp(allRate, 0.0f, 1.0f);
	inputRate = std::clamp((allRate - lag), 0.0f, 1.0f);


	int inputDrawHandle = -1;
	if (inputSignal == MoveType::STRAIGHT)
	{
		inputDrawHandle = sStraight;
	}
	else if (inputSignal == MoveType::RIGHTTURN)
	{
		inputDrawHandle = sRight;
	}
	else
	{
		inputDrawHandle = sStop;
	}

	int x = 700, y = 300;


#pragma region guideDraw
	{
		Matrix4 worldMat;

		float guideScale = 0.03f;
		worldMat = scale(Vector3(guideScale, guideScale, guideScale));

		worldMat *= rotationY(0.3f);
		worldMat *= camMat_;

		Vector3 easePos = guideObject_;

		Vector3 moveVec(-10.0f, -100.0f, 0.0f);

		moveVec = transform(moveVec, camMat_);
		float easeRate = Easing::easeOutBack(inputRate);
		easePos += (moveVec * (1 - easeRate));
		worldMat *= translate(easePos);
		MV1SetMatrix(sGuideModel, worldMat);
		MV1DrawModel(sGuideModel);
	}
#pragma endregion


#pragma region input

	{
		Matrix4 worldMat;

		float frameScale = 0.023f;
		worldMat = scale(Vector3(frameScale, frameScale, frameScale));

		worldMat *= camMat_;

		Vector3 easePos = actFrameObject_;

		Vector3 moveVec(0.0f, -100.0f, 0.0f);

		moveVec = transform(moveVec, camMat_);
		float easeRate = Easing::easeOutBack(inputRate);
		easePos += (moveVec * (1 - easeRate));
		worldMat *= translate(easePos);
		MV1SetMatrix(sActFrameModel, worldMat);
		MV1DrawModel(sActFrameModel);
	}

	{
		Matrix4 worldMat;

		float frameScale = 0.020f;
		worldMat = scale(Vector3(frameScale, frameScale, frameScale));

		worldMat *= camMat_;

		Vector3 easePos = actObject;

		Vector3 moveVec(0.0f, -100.0f, 0.0f);

		moveVec = transform(moveVec, camMat_);
		float easeRate = Easing::easeOutBack(inputRate);
		easePos += (moveVec * (1 - easeRate));
		worldMat *= translate(easePos);
		MV1SetMatrix(inputDrawHandle, worldMat);
		MV1DrawModel(inputDrawHandle);
		DrawExtendGraph(x, y, x + 100, y + 100, inputDrawHandle, TRUE);
	}
#pragma endregion


#pragma region frameDraw

	Matrix4 worldMat;

	float frameScale = 0.023f;
	worldMat = scale(Vector3(frameScale, frameScale, frameScale));

	worldMat *= camMat_;

	Vector3 easePos = nextFrameObject_;

	Vector3 moveVec(0.0f, -100.0f, 0.0f);

	moveVec = transform(moveVec, camMat_);
	float easeRate = Easing::easeOutBack(nextRate);
	easePos += (moveVec * (1 - easeRate));
	worldMat *= translate(easePos);
	MV1SetMatrix(sNextModel, worldMat);
	MV1DrawModel(sNextModel);
#pragma endregion

#pragma region nextDraw
	bool isNotFirst = false;
	int carsCount = 0;
	if (!alivePlayerCars_.empty())
	{
		for (auto &e : alivePlayerCars_)
		{
			int drawHandle = -1;
			if (e.lock()->GetMoveType() == MoveType::STRAIGHT)
			{
				drawHandle = sStraight;
			}
			else
			{
				drawHandle = sRight;
			}

			if (isNotFirst)
			{
#pragma region nextnext
				Matrix4 worldMat;

				float frameScale = 0.006f;
				worldMat = scale(Vector3(frameScale, frameScale, frameScale));

				worldMat *= camMat_;

				Vector3 easePos = nextnextObject_ + (Vector3(0.0f, -3.4f, -1.7f) * (carsCount - 1));

				Vector3 moveVec(0.0f, -100.0f, 0.0f);

				moveVec = transform(moveVec, camMat_);
				float easeRate = Easing::easeOutBack(nextRate);
				easePos += (moveVec * (1 - easeRate));
				worldMat *= translate(easePos);
				MV1SetMatrix(drawHandle, worldMat);
				MV1DrawModel(drawHandle);
#pragma endregion
			}
			else
			{
#pragma region next
				Matrix4 worldMat;

				float frameScale = 0.020f;
				worldMat = scale(Vector3(frameScale, frameScale, frameScale));

				worldMat *= camMat_;

				Vector3 easePos = nextObject_;

				Vector3 moveVec(0.0f, -100.0f, 0.0f);

				moveVec = transform(moveVec, camMat_);
				float easeRate = Easing::easeOutBack(nextRate);
				easePos += (moveVec * (1 - easeRate));
				worldMat *= translate(easePos);
				MV1SetMatrix(drawHandle, worldMat);
				MV1DrawModel(drawHandle);
#pragma endregion
				isNotFirst = true;
			}
			carsCount++;

			if (carsCount >= 4)
			{
				break;
			}
		}
	}
#pragma endregion

}

void CarManager::LoadGraphics()
{
	//up = LoadGraph("Resources/Texture/upArrow.png");
	//right = LoadGraph("Resources/Texture/rightArrow.png");
	//down = LoadGraph("Resources/Texture/downArrow.png");
	//left = LoadGraph("Resources/Texture/leftArrow.png");
	up = LoadGraph("Resources/Texture/UP.png");
	right = LoadGraph("Resources/Texture/RIGHT.png");
	down = LoadGraph("Resources/Texture/DOWN.png");
	left = LoadGraph("Resources/Texture/LEFT.png");
	stop = LoadGraph("Resources/Texture/STOP.png");

	sNextModel = MV1LoadModel("Resources/UI/next.mv1");

	//ècïWéØ
	sGuideModel = MV1LoadModel("Resources/operate_ui/operate_ui.mv1");

	//actionòg
	sActFrameModel = MV1LoadModel("Resources/action/action.mv1");

	sStop = MV1LoadModel("Resources/UI/stop.mv1");
	sStraight = MV1LoadModel("Resources/UI/straight.mv1");
	sRight = MV1LoadModel("Resources/UI/turn_right.mv1");

	CarBlastParticle::InitializeColor();
}

bool CarManager::GetDeadAnimation()
{
	return isDeadAnimation_;
}

bool CarManager::GetIsAllCarDead()
{
	bool isAllDead = true;

	for (auto &e : playerCars_)
	{
		if (e->GetIsAlive())
		{
			isAllDead = false;
			break;
		}
	}
	if (isAllDead)
	{
		for (auto &e : enemyCars_)
		{
			if (e->GetIsAlive())
			{
				isAllDead = false;
				break;
			}
		}
	}
	return isAllDead;
}

void CarManager::SetCamMat(Matrix4 mat)
{
	camMat_ = mat;
}

void CarManager::SetSoundManager(SoundManager *sounds)
{
	sounds_ = sounds;
	Car::SetSoundManager(sounds);
}

void CarManager::IngameUpdate()
{
	nextAnimationRate_ += 0.02f;
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

			playerBlast.Init(deadPlayerCar_.lock()->GetModelType(), deadPlayerCar_.lock()->GetFrontPos(), deadPlayerCar_.lock()->GetCarColor());
			enemyBlast.Init(deadEnemyCar_.lock()->GetModelType(), deadEnemyCar_.lock()->GetFrontPos(), deadEnemyCar_.lock()->GetCarColor());
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
	nextAnimationRate_ -= 0.02f;

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
	int carType = rand() % 2;
	CarInitializeDesc desc = sNormalCar;
	desc.type = MoveType::STRAIGHT;
	if (carType == 1)
	{
		desc = sTrackCar;
		desc.type = MoveType::RIGHTTURN;
	}
	bool isSpawn = false;
	desc.angle = Vector3(0, 0, -1);
	desc.startPos = Vector3(sCarWidthPos, 0.0f, 500.0f);
	desc.isPlayer = false;

	desc.speed = 1.0f;


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
	bool isSpawn = (alivePlayerCars_.size() < 6);

	if (isSpawn)
	{
		int carType = rand() % 2;
		CarInitializeDesc desc = sNormalCar;
		desc.type = MoveType::STRAIGHT;
		if (carType == 1)
		{
			desc = sTrackCar;
			desc.type = MoveType::RIGHTTURN;
		}
		desc.angle = Vector3(0, 0, 1);
		desc.startPos = Vector3(-sCarWidthPos, 0.0f, -300.0f);
		desc.speed = 3.0f;
		desc.isPlayer = true;


		for (auto &e : playerCars_)
		{
			if (!e->GetIsAlive())
			{
				e->Init(desc);

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
	}
	return isSpawn;
}
