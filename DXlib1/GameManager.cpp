#include "GameManager.h"
#include "DxLib.h"
#include "EaseClass.h"
#include <algorithm>
#include "Matrix4.h"
#include <string>
#include "SoundManager.h"
using namespace std;
GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}

void GameManager::Init()
{
	score = 0;
	combo = 0;
	cameraDeadAnimationPos_ = Vector3(0.0f, 30.0f, -20.0f);


	scoreObjectPos_ = Vector3(50.0f, 9.5f, -130.0f);
	scoreNumberObjectPos_ = scoreObjectPos_ + Vector3(-5.0f, -7.0f, 0.0f);

	Vector3 comboObjectMovePos = Vector3(-30.0f, 0.3f, 0.0f);
	comboObjectPos_ = scoreObjectPos_ + comboObjectMovePos;
	comboNumberObjectPos_ = comboObjectPos_ + Vector3(6.5f, -7.0f, 0.0f);
	comboObjectAnimationRate_ = 0.0f;


#pragma region result
	scoreResultObjectPos_ = Vector3(0.0f, 60.0f, -70.0f);
	scoreResultNumberObjectPos_ = scoreResultObjectPos_ + Vector3(-35.0f, -40.0f, -30.0f);
	scoreObjectAnimationRate_ = 0.0f;
#pragma endregion


	titleObjectPos_ = Vector3(5, 50.0f, -120.0f);
	titleObjectAnimationRate_ = 1.0f;

	pressAnyKeyObjectPos_ = Vector3(0.0f, 10.0f, -120.0f);
	pressAnyKeyObjectAnimationRate_ = 1.0f;

	timeObjectPos_ = Vector3(0.0f, 70.0f, -70.0f);
	timeNumberObjectPos_ = timeObjectPos_ + Vector3(6.3f, -10.0f, 0.0f);
	timeObjectAnimationRate_ = 0.0f;
}

void GameManager::Update()
{

	bool isGameOver = false;
	float easeRate = Easing::easeOutQuad(animationRate);


	Vector3 angle;
	Vector3 animationEndPos;
	Vector3 distance;
	Vector3 easePos;
	Vector3 easeTargetPos;
	switch (status_)
	{
	case GameStatus::TITLE:
		if (CheckHitKeyAll() && !isInput_)
		{
			ToIngame();
		}
		break;
	case GameStatus::SELECT:
		break;
	case GameStatus::INGAME:

		if (!isDeadAnimation_)
		{
			gameTimer_++;
		}
		//isClear_ = normaCars >= maxNormaCount;
		isFailed_ = gameTimer_ >= TimeLimit;

		isGameOver = (isClear_ || isFailed_);

		if (isGameOver && animationRate <= 0.0f)
		{
			sounds_->TimeUp();
			ToResult();
		}


		if (isDeadAnimation_)
		{
			animationRate += 0.1f;
			rotation += 0.002f;
			combo = 0;
		}
		else
		{
			if (isOlddeadAnimation)
			{
				gameTimer_ += 60 * SubSec;
			}
			animationRate -= 0.1f;
			rotation = 0.0f;
		}
		isOlddeadAnimation = isDeadAnimation_;
		animationRate = std::clamp(animationRate, 0.0f, 1.0f);

		angle = transform(cameraDeadAnimationPos_, rotationY(rotation));

		easeTargetPos = (cameraBaseTargetPos_ * (1 - animationRate));
		animationEndPos = angle + easeTargetPos;
		distance = animationEndPos - cameraBasePos_;


		easePos = cameraBasePos_ + (distance * animationRate);
		SetCameraPositionAndTargetAndUpVec(easePos, easeTargetPos, Vector3(0.0f, 1.0f, 0.0f));

		break;
	case GameStatus::RESULT:
		if (CheckHitKeyAll() && !isInput_ && isCarAllDead_)
		{
			ToTitle();
		}
		break;
	case GameStatus::PAUSE:
		break;
	default:
		break;
	}

	isNotAnimationEnd_ = false;
	TitleObjectUpdate();
	scoreObjectUpdate();
	PressAnyKeyUpdate();

	if (CheckHitKeyAll() && !isInput_)
	{
		int a = 2;
		//if (CheckHitKey(KEY_INPUT_1))
		//{
		//	AddSec--;
		//}
		//if (CheckHitKey(KEY_INPUT_2))
		//{
		//	AddSec++;
		//}
		//if (CheckHitKey(KEY_INPUT_3))
		//{
		//	SubSec--;
		//}
		//if (CheckHitKey(KEY_INPUT_4))
		//{
		//	SubSec--;
		//}
		//if (CheckHitKey(KEY_INPUT_5))
		//{
		//	sounds_->Explosion(a);
		//}
		//if (CheckHitKey(KEY_INPUT_6))
		//{
		//	sounds_->Horn(a);
		//}


		if (CheckHitKey(KEY_INPUT_9))
		{
			sounds_->Enter();
		}
		if (CheckHitKey(KEY_INPUT_0))
		{
			sounds_->BGM();
		}
	}

	isInput_ = (CheckHitKeyAll() || isNotAnimationEnd_);
}

void GameManager::TitleObjectUpdate()
{

	const float moveRate = 0.03f;
	if (status_ == GameStatus::TITLE)
	{
		titleObjectAnimationRate_ += moveRate;
	}
	else
	{
		titleObjectAnimationRate_ -= moveRate;
	}
	if (titleObjectAnimationRate_ < 1.0f && titleObjectAnimationRate_ >= 0.0f)
	{
		isNotAnimationEnd_ = true;
	}
	titleObjectAnimationRate_ = clamp(titleObjectAnimationRate_, 0.0f, 1.0f);
}

void GameManager::scoreObjectUpdate()
{
	const float moveRate = 0.03f;

	if (status_ == GameStatus::INGAME)
	{
		scoreObjectAnimationRate_ += moveRate;
	}
	else
	{
		scoreObjectAnimationRate_ -= moveRate;
	}

	if (status_ == GameStatus::RESULT)
	{
		scoreResultObjectAnimationRate_ += moveRate;
	}
	else
	{
		scoreResultObjectAnimationRate_ -= moveRate;
	}

	if (scoreObjectAnimationRate_ < 1.0f && scoreObjectAnimationRate_ >= 0.0f)
	{
		isNotAnimationEnd_ = true;
	}
	if (scoreResultObjectAnimationRate_ < 1.0f && scoreResultObjectAnimationRate_ >= 0.0f)
	{
		isNotAnimationEnd_ = true;
	}
	scoreObjectAnimationRate_ = clamp(scoreObjectAnimationRate_, 0.0f, 1.0f);
	scoreResultObjectAnimationRate_ = clamp(scoreResultObjectAnimationRate_, 0.0f, 1.0f);
}

void GameManager::PressAnyKeyUpdate()
{
	const float moveRate = 0.03f;
	if ((status_ == GameStatus::TITLE) || (status_ == GameStatus::RESULT))
	{
		pressAnyKeyObjectAnimationRate_ += moveRate;
	}
	else
	{
		pressAnyKeyObjectAnimationRate_ -= moveRate;
	}

	pressAnyKeyObjectAnimationRate_ = clamp(pressAnyKeyObjectAnimationRate_, 0.0f, 1.0f);
}

void GameManager::Finalize()
{
}

void GameManager::Draw()
{
	int heightPos = 600;

	switch (status_)
	{
	case GameStatus::TITLE:
		//DrawFormatString(0, 0, GetColor(255, 255, 255), "SCENE : TITLE");

		//DrawFormatString(0, 20, GetColor(255, 255, 255), "PRESS ANY KEY : INGAME");


		break;
	case GameStatus::SELECT:
		//DrawFormatString(0, 0, GetColor(255, 255, 255), "SCENE : SELECT");

		break;
	case GameStatus::INGAME:
		//DrawFormatString(0, heightPos + 0, GetColor(255, 255, 255), "SCENE : INGAME");

		//DrawFormatString(0, heightPos + 20, GetColor(255, 255, 255), "TIME : %d / %d", gameTimer_, TimeLimit);

		//DrawFormatString(0, heightPos + 40, GetColor(255, 255, 255), "NORMA : %d / %d", normaCars, maxNormaCount);

		//DrawFormatString(0, heightPos + 60, GetColor(255, 255, 255), "SCORE : %d", score);

		//DrawFormatString(0, heightPos + 80, GetColor(255, 255, 255), "COMBO : %d", combo);
		break;
	case GameStatus::RESULT:
		//DrawFormatString(0, 0, GetColor(255, 255, 255), "SCENE : RESULT");

		//DrawFormatString(0, 20, GetColor(255, 255, 255), "PRESS ANY KEY : TITLE");

		break;
	case GameStatus::PAUSE:
		//DrawFormatString(0, 0, GetColor(255, 255, 255), "SCENE : PAUSE");

		break;
	default:
		break;
	}
	scoreDraw();
	ResultDraw();
	TitleDraw();
	PressAnyKeyDraw();
}

void GameManager::PassCar()
{
	combo++;
	float comboRate = 0.5;

	score += (baseScore * (combo * comboRate));

	if (combo % 5 == 0)
	{
		gameTimer_ -= (60 * AddSec);
	}
	normaCars++;
}

void GameManager::StopCar()
{

	combo = 0;
}

GameStatus GameManager::GetStatus()
{
	return status_;
}

void GameManager::NormaCount()
{
	normaCars++;
}

void GameManager::SetCameraPos(Vector3 camPos, Vector3 targetPos)
{
	cameraBasePos_ = camPos;
	cameraBaseTargetPos_ = targetPos;

}

void GameManager::SetIsDeadAnimation(bool isDeadAnimation)
{
	isDeadAnimation_ = isDeadAnimation;
}

void GameManager::Load()
{
	string baseName = "Resources/";
	string number = "number/number_";

	int i = 0;
	for (auto &e : numberObjects_)
	{
		string path = baseName + number + to_string(i) + ".mv1";
		e = MV1LoadModel(path.c_str());
		i++;
	}

	scoreTextObject_ = MV1LoadModel("Resources/score/score.mv1");
	comboTextObject_ = MV1LoadModel("Resources/combo/combo.mv1");
	TitleObject_ = MV1LoadModel("Resources/title/title.mv1");
	pressAnyKeyHandle_ = MV1LoadModel("Resources/press_any_key/press_any_key.mv1");
	timeTextObjectHandle_ = MV1LoadModel("Resources/time_ui/time_ui.mv1");
}

void GameManager::TitleDraw()
{
	Matrix4 cameraPosture = Posture((cameraBaseTargetPos_ - cameraBasePos_), Vector3(0.0f, 1.0f, 0.0f));
	Matrix4 worldMat;
	worldMat = scale(Vector3(0.1f, 0.1f, 0.1f));

	worldMat *= cameraPosture;

	Vector3 easePos = titleObjectPos_;

	Vector3 moveVec(0.0f, 100.0f, 0.0f);

	moveVec = transform(moveVec, cameraPosture);
	float easeRate = Easing::easeOutBack(titleObjectAnimationRate_);
	easePos += (moveVec * (1 - easeRate));
	worldMat *= translate(easePos);
	MV1SetMatrix(TitleObject_, worldMat);

	MV1DrawModel(TitleObject_);
}


void GameManager::scoreDraw()
{
	array<float, 15> rate;

	Matrix4 cameraPosture = Posture((cameraBaseTargetPos_ - cameraBasePos_), Vector3(0.0f, 1.0f, 0.0f));

	{
		int i = 0;

		float Range = 0.2f;
		float allFlowRate = scoreObjectAnimationRate_ * (1.0f + Range);
		for (auto &e : rate)
		{
			float RangeLag = Range / 11.0f;
			e = allFlowRate - (i * RangeLag);
			e = clamp(e, 0.0f, 1.0f);
			i++;
		}
	}
	Vector3 moveVec(0.0f, 100.0f, 0.0f);

	moveVec = transform(moveVec, cameraPosture);


	float scoreScale = 0.03f;
	float comboScale = 0.03f;
	float timeScale = 0.03f;

#pragma region scoreNumber
	//scoreNumber•\Ž¦
	{
		int deget = 0;

		std::vector<int> scoreNums;
		scoreNums.resize(10);
		int tmpScore = score;
		for (int i = 1; i <= scoreNums.size(); i++)
		{
			int tmpDegit = tmpScore % 10;
			scoreNums[scoreNums.size() - i] = tmpDegit;

			tmpScore /= 10;
		}
		for (auto &e : scoreNums)
		{

			Matrix4 worldMat;
			worldMat = scale(Vector3(scoreScale, scoreScale, scoreScale));

			worldMat *= cameraPosture;


			Vector3 easePos = scoreNumberObjectPos_ + Vector3(-10.0f + ((125.0f * scoreScale) * deget), 0.0f, 0.0f);

			easePos = transform(easePos, cameraPosture);


			float easeRate = Easing::easeOutBack(rate[deget]);
			easePos += (moveVec * (1 - easeRate));

			worldMat *= translate(easePos);

			deget++;
			MV1SetMatrix(numberObjects_[e], worldMat);

			MV1DrawModel(numberObjects_[e]);
		}
	}
#pragma endregion

#pragma region scoreText
	{
		Matrix4 worldMat;
		worldMat = scale(Vector3(scoreScale, scoreScale, scoreScale));

		worldMat *= cameraPosture;


		Vector3 easePos = transform(scoreObjectPos_, cameraPosture);


		float easeRate = Easing::easeOutBack(rate[10]);
		easePos += (moveVec * (1 - easeRate));


		worldMat *= translate(easePos);
		MV1SetMatrix(scoreTextObject_, worldMat);

		MV1DrawModel(scoreTextObject_);
	}
#pragma endregion

#pragma region comboNum
	{
		int deget = 0;

		std::vector<int> comboNums;
		comboNums.resize(3);
		int tmpCombo = combo;
		for (int i = 1; i <= comboNums.size(); i++)
		{
			int tmpDegit = tmpCombo % 10;
			comboNums[comboNums.size() - i] = tmpDegit;

			tmpCombo /= 10;
		}
		for (auto &e : comboNums)
		{

			Matrix4 worldMat;
			worldMat = scale(Vector3(comboScale, comboScale, comboScale));

			worldMat *= cameraPosture;


			Vector3 easePos = comboNumberObjectPos_ + Vector3(-10.0f + ((125.0f * comboScale) * (deget)), 0.0f, 0.0f);

			easePos = transform(easePos, cameraPosture);

			float easeRate = Easing::easeOutBack(rate[deget + 11]);
			easePos += (moveVec * (1 - easeRate));

			worldMat *= translate(easePos);

			deget++;
			MV1SetMatrix(numberObjects_[e], worldMat);

			MV1DrawModel(numberObjects_[e]);
		}
	}

#pragma endregion

#pragma region comboText
	{
		Matrix4 worldMat;
		worldMat = scale(Vector3(comboScale, comboScale, comboScale));

		worldMat *= cameraPosture;


		Vector3 easePos = easePos = transform(comboObjectPos_, cameraPosture);


		float easeRate = Easing::easeOutBack(rate[14]);
		easePos += (moveVec * (1 - easeRate));


		worldMat *= translate(easePos);
		MV1SetMatrix(comboTextObject_, worldMat);

		MV1DrawModel(comboTextObject_);
	}
#pragma endregion

#pragma region timeNumber
	{
		int deget = 0;

		std::vector<int> timeNums;
		timeNums.resize(2);
		int tmpTime = TimeLimit - gameTimer_;
		tmpTime /= 60;
		tmpTime++;
		for (int i = 1; i <= timeNums.size(); i++)
		{
			int tmpDegit = tmpTime % 10;
			timeNums[timeNums.size() - i] = tmpDegit;

			tmpTime /= 10;
		}
		for (auto &e : timeNums)
		{

			Matrix4 worldMat;
			worldMat = scale(Vector3(timeScale * 2, timeScale * 2, timeScale * 2));

			worldMat *= cameraPosture;


			Vector3 easePos = timeNumberObjectPos_ + Vector3(-10.0f + (7.0f * (deget)), 0.0f, 0.0f);

			float easeRate = Easing::easeOutBack(rate[deget + 11]);
			easePos += (moveVec * (1 - easeRate));

			worldMat *= translate(easePos);

			deget++;
			MV1SetMatrix(numberObjects_[e], worldMat);

			MV1DrawModel(numberObjects_[e]);
		}
	}

#pragma endregion

#pragma region timeText
	{
		Matrix4 worldMat;
		worldMat = scale(Vector3(timeScale, timeScale, timeScale));

		worldMat *= cameraPosture;


		Vector3 easePos = timeObjectPos_;


		float easeRate = Easing::easeOutBack(rate[14]);
		easePos += (moveVec * (1 - easeRate));


		worldMat *= translate(easePos);
		MV1SetMatrix(timeTextObjectHandle_, worldMat);

		MV1DrawModel(timeTextObjectHandle_);
	}
#pragma endregion

}

void GameManager::ResultDraw()
{

	array<float, 11> rate;

	Matrix4 cameraPosture = Posture((cameraBaseTargetPos_ - cameraBasePos_), Vector3(0.0f, 1.0f, 0.0f));
	{
		int i = 0;

		float Range = 0.1f;
		float allFlowRate = scoreResultObjectAnimationRate_ * (1.0f + Range);
		for (auto &e : rate)
		{
			float RangeLag = Range / 11.0f;
			e = allFlowRate - (i * RangeLag);
			e = clamp(e, 0.0f, 1.0f);
			i++;
		}
	}
	Vector3 moveVec(0.0f, 100.0f, 0.0f);

	moveVec = transform(moveVec, cameraPosture);
	//scoreResultNumber•\Ž¦
	{
		int i = 0;

		std::vector<int> scoreNums;
		scoreNums.resize(10);
		int tmpScore = score;
		for (int i = 1; i <= 10; i++)
		{
			int degit = tmpScore % 10;
			scoreNums[10 - i] = degit;

			tmpScore /= 10;
		}
		for (auto &e : scoreNums)
		{

			Matrix4 worldMat;
			worldMat = scale(Vector3(0.1f, 0.1f, 0.1f));

			worldMat *= cameraPosture;


			Vector3 easePos = scoreResultNumberObjectPos_ + Vector3(-10.0f + (10.0f * i), 0.0f, 0.0f);

			float easeRate = Easing::easeOutBack(rate[i]);
			easePos += (moveVec * (1 - easeRate));

			worldMat *= translate(easePos);

			i++;
			MV1SetMatrix(numberObjects_[e], worldMat);

			MV1DrawModel(numberObjects_[e]);
		}
	}

	{
		Matrix4 worldMat;
		worldMat = scale(Vector3(0.1f, 0.1f, 0.1f));

		worldMat *= cameraPosture;


		Vector3 easePos = scoreResultObjectPos_;


		float easeRate = Easing::easeOutBack(rate[10]);
		easePos += (moveVec * (1 - easeRate));


		worldMat *= translate(easePos);
		MV1SetMatrix(scoreTextObject_, worldMat);

		MV1DrawModel(scoreTextObject_);
	}
}

void GameManager::PressAnyKeyDraw()
{
	Matrix4 cameraPosture = Posture((cameraBaseTargetPos_ - cameraBasePos_), Vector3(0.0f, 1.0f, 0.0f));
	Matrix4 worldMat;
	worldMat = scale(Vector3(0.05f, 0.05f, 0.05f));

	worldMat *= cameraPosture;

	Vector3 easePos = pressAnyKeyObjectPos_;

	Vector3 moveVec(0.0f, 100.0f, 0.0f);

	moveVec = transform(moveVec, cameraPosture);
	float easeRate = Easing::easeOutBack(pressAnyKeyObjectAnimationRate_);
	easePos += (moveVec * (1 - easeRate));
	worldMat *= translate(easePos);
	MV1SetMatrix(pressAnyKeyHandle_, worldMat);

	MV1DrawModel(pressAnyKeyHandle_);
}

void GameManager::CountDownDraw()
{
}

void GameManager::CheckCarAllDead(bool isAllDead)
{
	isCarAllDead_ = isAllDead;
}

Matrix4 GameManager::GetCamMat()
{
	return  Posture((cameraBaseTargetPos_ - cameraBasePos_), Vector3(0.0f, 1.0f, 0.0f));
}

void GameManager::SetSoundManager(SoundManager *sound)
{
	sounds_ = sound;
}

void GameManager::ToIngame()
{
	score = 0;
	combo = 0;
	normaCars = 0;
	gameTimer_ = 0;
	sounds_->IngameVolume();
	sounds_->Enter();
	sounds_->BGM();
	status_ = GameStatus::INGAME;
}


void GameManager::ToResult()
{
	sounds_->ResultVolume();
	status_ = GameStatus::RESULT;
}


void GameManager::ToTitle()
{
	sounds_->TitleVolume();
	sounds_->BGMStop();
	sounds_->Enter();
	status_ = GameStatus::TITLE;
}


void GameManager::ToSelect()
{
	status_ = GameStatus::SELECT;
}
