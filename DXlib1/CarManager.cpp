#include "CarManager.h"
#include "Collision.h"
#include "DxLib.h"
#include "EaseClass.h"
#include <algorithm>
#include "SoundManager.h"
#include "Input.h"

using namespace std;

const float CarManager::sCarWidthPos = 13.0f;
CarInitializeDesc CarManager::sNormalCar;
CarInitializeDesc CarManager::sTrackCar;
float CarManager::sGameSpeed = 1.0f;
const int CarManager::sDeadAnimationTimerMax = 60;

int CarManager::up, CarManager::right, CarManager::down, CarManager::left, CarManager::stop;
int CarManager::sNextModel, CarManager::sStraight, CarManager::sStop, CarManager::sRight;
int CarManager::sActFrameModel, CarManager::sGuideModel;

vector<int> CarManager::sArrowModelHandle;
int CarManager::sCautionHandle;
int CarManager::sTimeBar;
CarManager::CarManager()
{
	playerCars_.reserve(30);
	playerCars_.resize(30);
	enemyCars_.reserve(30);
	enemyCars_.resize(30);
	for (auto& e : playerCars_)
	{
		e = make_shared<Car>();
	}
	for (auto& e : enemyCars_)
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

	nextFrameObject_ = Vector3(-27.0f, 34.0f, -120.0f);
	actObject = actFrameObject_ + +Vector3(3, 0.0f, 0.0f);


	actFrameObject_ = actFrameObject_ + Vector3(0.0f, -20.0f, -10.0f);
	nextObject_ = nextFrameObject_ + Vector3(3, 0.0f, 0.0f);
	nextnextObject_ = nextObject_ + Vector3(-7, 4.0f, 0.0f);




	guideObject_ = Vector3(20.0f, 35.0f, -160.0f);

	TitleParticles.resize(60);
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

	alivePlayerCars_.remove_if([](std::weak_ptr<Car>& x)
		{
			return (!x.lock()->GetIsAlive() || x.lock()->GetIsPass());
		});




	aliveEnemyCars_.remove_if([](std::weak_ptr<Car>& x)
		{
			return (!x.lock()->GetIsAlive() || x.lock()->GetIsPass());
		});

	for (auto& e : TitleParticles)
	{
		e.Update();
	}
	playerBlast.Update();
	enemyBlast.Update();
	ArrowBlast.Update();
	inpuAnimation += 0.1f;
	inpuAnimation = clamp(inpuAnimation, 0.0f, 1.0f);
}

void CarManager::Finalize()
{
}

void CarManager::Draw()
{
	for (auto& e : playerCars_)
	{
		if (e->GetIsAlive())
		{
			e->Draw();
		}
	}
	for (auto& e : enemyCars_)
	{
		if (e->GetIsAlive())
		{
			e->Draw();
		}
	}

	for (auto& e : TitleParticles)
	{
		e.Draw();
	}
	playerBlast.Draw();
	enemyBlast.Draw();
	ArrowBlast.Draw();
}

void CarManager::SetSignal()
{

	MoveType oldSignal = inputSignal;

	if (!isDeadAnimation_)
	{
		if (CheckHitKey(KEY_INPUT_W) || CheckHitKey(KEY_INPUT_UP) || Input::isPad(XINPUT_BUTTON_DPAD_UP) || Input::isPad(XINPUT_BUTTON_Y) || Input::isPadThumb(XINPUT_THUMB_LEFTVERT) >= 0.5f)
		{
			inputSignal = MoveType::STRAIGHT;
		}
		else if (CheckHitKey(KEY_INPUT_D) || CheckHitKey(KEY_INPUT_RIGHT) || Input::isPad(XINPUT_BUTTON_DPAD_RIGHT) || Input::isPad(XINPUT_BUTTON_B) || Input::isPadThumb(XINPUT_THUMB_LEFTSIDE) >= 0.5f)
		{
			inputSignal = MoveType::RIGHTTURN;
		}
		else
		{
			inputSignal = MoveType::STOP;
		}
	}
	//if (CheckHitKey(KEY_INPUT_S))
	//{
	//	inputSignal = MoveType::STOP;
	//}

	isChangeSignal = (oldSignal != inputSignal);
	if (isChangeSignal)
	{
		inpuAnimation = 0.0f;
		sounds_->Action();
	}
	Car::SetSignal(inputSignal);
}

void CarManager::Collision()
{
	for (auto& pCar : playerCars_)
	{
		if (pCar->GetIsAlive())
		{
			for (auto& eCar : enemyCars_)
			{
				if (eCar->GetIsAlive())
				{
					bool isHit = Collision::testCapsuleCapsule(*pCar->GetCapsule(), *eCar->GetCapsule());
					if (isHit)
					{
						deadPlayerCar_ = pCar;
						deadEnemyCar_ = eCar;
						deadAnimationTimer_ = 0;
						if (isIngame_)
						{
							isDeadAnimation_ = true;
							SetGameSpeed(0.01f);
							sounds_->BGMStop();
							sounds_->Slow();
							inputSignal = MoveType::STOP;
						}
						break;
					}
				}
			}
		}
	}
}

std::vector<Vector3> CarManager::GetPassCars()
{
	int passCarCount = 0;

	vector<Vector3> retPos;
	for (auto& e : playerCars_)
	{
		if (e->GetIsAlive())
		{
			if (e->GetIsPass())
			{
				retPos.emplace_back(e->GetFrontPos());
				e->Count();
				ArrowBlast.Init(ModelType::NORMAL, e->GetFrontPos() + Vector3(0.0f, 2.0f, 0.0f), 0, true);
				passCarCount++;
			}
		}
	}
	for (auto& e : enemyCars_)
	{
		if (e->GetIsAlive())
		{

			if (e->GetIsPass())
			{
				retPos.emplace_back(e->GetFrontPos());
				e->Count();
				passCarCount++;
			}
		}
	}
	return retPos;
}

bool CarManager::GetAnyCarStop()
{
	bool isStop = false;
	for (auto& e : playerCars_)
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

		Vector3 yVec(0.0f, 5.0f, 0.0f);
		float upEase = Easing::easeOutExpo(inpuAnimation);
		float downEase = Easing::easeOutSine(inpuAnimation);
		worldMat *= translate((yVec * upEase));
		worldMat *= translate((-yVec * downEase));

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


#pragma region nextDraw
	bool isNotFirst = false;
	int carsCount = 0;
	if (!alivePlayerCars_.empty())
	{
		//画面の一番下にあたる部分
		float underFrameLine = -145.0f;

		for (auto& e : alivePlayerCars_)
		{
			//画面より下にいたら
			if (e.lock()->GetFrontPos().z < underFrameLine)
			{

				int drawHandle = -1;
				if (e.lock()->GetMoveType() == MoveType::STRAIGHT)
				{
					drawHandle = sArrowModelHandle[0];
				}
				else
				{
					drawHandle = sArrowModelHandle[1];
				}

#pragma region next
				Matrix4 worldMat;
				Matrix4 cautionMat;

				float frameScale = 0.005f;
				float cautionScale = 20.0f;
				worldMat = scale(Vector3(frameScale, frameScale, frameScale));
				cautionMat = scale(Vector3(frameScale * cautionScale, frameScale * 0.1f, frameScale * cautionScale));
				if (e.lock()->GetMoveType() == MoveType::STRAIGHT)
				{
					worldMat *= rotationY(-3.14f / 2.0f);
				}

				worldMat *= camMat_;

				worldMat *= rotationX(-3.14f / 2.0f);

				Vector3 easePos = nextObject_;

				Vector3 moveVec(18.0f, -25.0f, -20.0f);

				moveVec = transform(moveVec, camMat_);

				float easeRate = Easing::easeOutBack(nextRate);
				easePos += (moveVec);
				worldMat *= translate(easePos);
				MV1SetDifColorScale(drawHandle, GetColorF(1.0f, 1.0f, 1.0f, 0.5f));
				MV1SetMatrix(drawHandle, worldMat);
				MV1DrawModel(drawHandle);

				cautionMat *= rotationX(-3.14f / 2.0f);
				cautionMat *= rotationZ(-3.14f / 4.0f);
				cautionMat *= camMat_;
				cautionMat *= translate(easePos);
				//cautionMat ;
				MV1SetDifColorScale(sCautionHandle, GetColorF(1.0f, 1.0f, 1.0f, 0.5f));
				MV1SetMatrix(sCautionHandle, cautionMat);
				MV1DrawModel(sCautionHandle);
#pragma endregion
				break;
			}
		}
	}
#pragma endregion


#pragma region GaugeDraw
	if(gaugeRate_ > 0.0f && gaugeRate_ < 1.0f)
	{
		Matrix4 worldMat;
		Matrix4 cautionMat;


		float gaugeScale = 0.02f;
		float lengthRate = (1.0f - gaugeRate_);
		worldMat = scale(Vector3(gaugeScale * lengthRate, gaugeScale, gaugeScale));

		worldMat *= camMat_;
		Vector3 moveVec(-20.0f, 20.0f, -70.0f);

		worldMat *= translate(moveVec);

		float colorRate = Easing::easeInQuad(gaugeRate_);
		MV1SetDifColorScale(sTimeBar, GetColorF(colorRate, (1.0f - colorRate), 0.0f, 1.0f));
		MV1SetMatrix(sTimeBar, worldMat);
		MV1DrawModel(sTimeBar);
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

	//縦標識
	sGuideModel = MV1LoadModel("Resources/operate_ui/operate_ui.mv1");

	//action枠
	sActFrameModel = MV1LoadModel("Resources/action/action.mv1");

	sStop = MV1LoadModel("Resources/UI/stop.mv1");
	sStraight = MV1LoadModel("Resources/UI/straight.mv1");
	sRight = MV1LoadModel("Resources/UI/turn_right.mv1");


	sArrowModelHandle.resize(2);
	sArrowModelHandle[0] = MV1LoadModel("Resources/arrow/arrow.mv1");
	sArrowModelHandle[1] = MV1LoadModel("Resources/arrow_right/arrow_right.mv1");

	sCautionHandle = MV1LoadModel("Resources/caution/caution.mv1");
	sTimeBar = MV1LoadModel("Resources/time_bar/time_bar.mv1");

	CarBlastParticle::InitializeColor();

	Car::LoadModel(sArrowModelHandle[0], sArrowModelHandle[1]);
}

bool CarManager::GetDeadAnimation()
{
	return isDeadAnimation_;
}

bool CarManager::GetIsAllCarDead()
{
	bool isAllDead = true;

	for (auto& e : playerCars_)
	{
		if (e->GetIsAlive())
		{
			isAllDead = false;
			break;
		}
	}
	if (isAllDead)
	{
		for (auto& e : enemyCars_)
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

bool CarManager::GetIsNotTrackMove()
{
	bool isNotMove = false;

	if (inputSignal == MoveType::STOP)
	{
		return false;
	}

	if (alivePlayerCars_.size() > 0)
	{
		//isNotMove = (alivePlayerCars_.begin()->lock()->GetModelType() == ModelType::TRACK);
			isNotMove = alivePlayerCars_.begin()->lock()->GetIsSignalStop();


		if (isNotMove)
		{
			isNotMove = (isChangeSignal && (inputSignal != alivePlayerCars_.begin()->lock()->GetMoveType()));
		}
	}

	return isNotMove;
}

MoveType CarManager::GetInputSignal()
{
	return inputSignal;
}

void CarManager::SetCamMat(Matrix4 mat)
{
	camMat_ = mat;
}

void CarManager::SetSoundManager(SoundManager* sounds)
{
	sounds_ = sounds;
	Car::SetSoundManager(sounds);
}

void CarManager::SetIsResult(bool isResult)
{
	isResult_ = isResult;
}

void CarManager::AllDead()
{
	int count = 0;

	float underFrameLine = -145.0f;
	bool isBroken = false;
	for (auto& e : playerCars_)
	{
		if (e->GetIsAlive())
		{
			e->Dead();

			if (e->GetFrontPos().z > underFrameLine)
			{
				TitleParticles[count].Init(e->GetModelType(), e->GetFrontPos(), e->GetCarColor());
				isBroken = true;
			}
		}
		count++;
	}

	for (auto& e : enemyCars_)
	{
		if (e->GetIsAlive())
		{
			e->Dead();
			if (e->GetTailPos().z > underFrameLine)
			{
				TitleParticles[count].Init(e->GetModelType(), e->GetFrontPos(), e->GetCarColor());
				isBroken = true;
			}
		}
		count++;
	}
	if (isBroken)
	{
		sounds_->TitleExplosion();
	}
}

void CarManager::SetGaugeRate(float rate)
{
	gaugeRate_ = rate;
}

void CarManager::SetLevel(int level)
{
	level_ = level;
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
			sounds_->ContinueBGM();
			sounds_->Explosion();
			sounds_->Broken();

			playerBlast.Init(deadPlayerCar_.lock()->GetModelType(), deadPlayerCar_.lock()->GetFrontPos(), deadPlayerCar_.lock()->GetCarColor());
			enemyBlast.Init(deadEnemyCar_.lock()->GetModelType(), deadEnemyCar_.lock()->GetFrontPos(), deadEnemyCar_.lock()->GetCarColor());
		}
	}
	else
	{
		Collision();

		for (auto& e : playerCars_)
		{
			if (e->GetIsAlive())
			{
				e->Update();
			}
		}
		for (auto& e : enemyCars_)
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
	//Car::SetSignal();
	nextAnimationRate_ -= 0.02f;




	if (deadAnimationTimer_ <= 0)
	{
		if (!deadPlayerCar_.expired())
		{
			deadPlayerCar_.lock()->Update();
			deadPlayerCar_.lock()->Dead();
			playerBlast.Init(deadPlayerCar_.lock()->GetModelType(), deadPlayerCar_.lock()->GetFrontPos(), deadPlayerCar_.lock()->GetCarColor());

			sounds_->Explosion();
			sounds_->Broken();
		}
		if (!deadEnemyCar_.expired() && deadEnemyCar_.lock()->GetIsAlive())
		{
			deadEnemyCar_.lock()->Update();
			deadEnemyCar_.lock()->Dead();
			enemyBlast.Init(deadEnemyCar_.lock()->GetModelType(), deadEnemyCar_.lock()->GetFrontPos(), deadEnemyCar_.lock()->GetCarColor());
		}
		deadAnimationTimer_++;
	}

	if (!isResult_)
	{
		Collision();
	}

	for (auto& e : playerCars_)
	{
		if (e->GetIsAlive())
		{
			e->Update();
		}
	}
	for (auto& e : enemyCars_)
	{
		if (e->GetIsAlive())
		{
			e->Update();
		}
	}
}

bool CarManager::AddEnemyCar(bool isTitle)
{
	bool isNothing = (aliveEnemyCars_.size() <= 0);
	bool isAreaSafe = false;
	if (!isNothing)
	{
		isAreaSafe = (aliveEnemyCars_.back().lock()->GetTailPos().z < 470.0f);
	}
	bool isSpawn = (isNothing || isAreaSafe);

	if (isSpawn)
	{
		int carType = rand() % 100;

		//トラックになる確率
		int trackProbability = (level_ * 5);

		if (isTitle)
		{
			trackProbability = 50;
		}
		//トラックになるか確認
		bool isTrack = (carType <= trackProbability);


		CarInitializeDesc desc = sNormalCar;
		desc.type = MoveType::STRAIGHT;
		desc.color = Color::BLUE;
		if (isTrack)
		{
			desc = sTrackCar;
			desc.type = MoveType::RIGHTTURN;
			desc.color = Color::PINK;
			desc.length -= 3.0f;
		}
		desc.angle = Vector3(0, 0, -1);
		desc.startPos = Vector3(sCarWidthPos, 0.0f, 500.0f);
		desc.isPlayer = false;

		desc.speed = (1.0f + (level_ * 0.05f));


		for (auto& e : enemyCars_)
		{
			if (!e->GetIsAlive())
			{
				//初期化
				e->Init(desc);
				isSpawn = true;

				//ひとつ前の車両が存在するなら
				if (!enemyEndCar.expired())
				{
					e->SetFrontCar(enemyEndCar);
				}
				//自分が最後尾車両になる
				enemyEndCar = e;

				aliveEnemyCars_.emplace_back(e);
				break;
			}
		}
	}
	return isSpawn;
}

bool CarManager::AddPlayerCar()
{
	bool isSpawn = (alivePlayerCars_.size() < 5);

	if (isSpawn)
	{
		int carType = rand() % 100;

		//トラックになる確率
		int trackProbability = 30;

		//トラックになるか確認
		bool isTrack = (carType <= trackProbability);		CarInitializeDesc desc = sNormalCar;
		desc.type = MoveType::STRAIGHT;
		if (isTrack)
		{
			desc = sTrackCar;
			desc.type = MoveType::RIGHTTURN;
			desc.length -= 3.0f;
		}
		desc.angle = Vector3(0, 0, 1);
		desc.startPos = Vector3(-sCarWidthPos, 0.0f, -300.0f);
		desc.speed = 3.0f;
		desc.isPlayer = true;


		for (auto& e : playerCars_)
		{
			if (!e->GetIsAlive())
			{
				e->Init(desc);

				//ひとつ前の車両が存在するなら
				if (!playerEndCar.expired())
				{
					e->SetFrontCar(playerEndCar);
				}
				//自分が最後尾車両になる
				playerEndCar = e;
				alivePlayerCars_.emplace_back(e);
				break;
			}
		}
	}
	return isSpawn;
}
