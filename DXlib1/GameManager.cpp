#include "GameManager.h"
#include "DxLib.h"
#include "EaseClass.h"
#include <algorithm>
#include "Matrix4.h"
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
		gameTimer_++;
		isClear_ = normaCars >= maxNormaCount;
		isFailed_ = gameTimer_ >= TimeLimit;

		isGameOver = (isClear_ || isFailed_);

		if (isGameOver)
		{
			ToResult();
		}


		if (isDeadAnimation_)
		{
			animationRate += 0.1f;
			rotation += 0.002f;
		}
		else
		{
			animationRate -= 0.1f;
			rotation = 0.0f;
		}

		animationRate = std::clamp(animationRate, 0.0f, 1.0f);

		angle = transform(cameraDeadAnimationPos_, rotationY(rotation));

		easeTargetPos = (cameraBaseTargetPos_ * (1 - animationRate));
		animationEndPos = angle + easeTargetPos;
		distance = animationEndPos - cameraBasePos_;


		easePos = cameraBasePos_ + (distance * animationRate);
		SetCameraPositionAndTargetAndUpVec(easePos, easeTargetPos, Vector3(0.0f, 1.0f, 0.0f));

		break;
	case GameStatus::RESULT:
		if (CheckHitKeyAll() && !isInput_)
		{
			ToTitle();
		}
		break;
	case GameStatus::PAUSE:
		break;
	default:
		break;
	}

	isInput_ = CheckHitKeyAll();

}

void GameManager::Finalize()
{
}

void GameManager::Draw()
{
	switch (status_)
	{
	case GameStatus::TITLE:
		DrawFormatString(0, 0, GetColor(255, 255, 255), "SCENE : TITLE");

		DrawFormatString(0, 20, GetColor(255, 255, 255), "PRESS ANY KEY : INGAME");
		break;
	case GameStatus::SELECT:
		DrawFormatString(0, 0, GetColor(255, 255, 255), "SCENE : SELECT");

		break;
	case GameStatus::INGAME:
		DrawFormatString(0, 0, GetColor(255, 255, 255), "SCENE : INGAME");

		DrawFormatString(0, 20, GetColor(255, 255, 255), "TIME : %d / %d", gameTimer_, TimeLimit);

		DrawFormatString(0, 40, GetColor(255, 255, 255), "NORMA : %d / %d", normaCars, maxNormaCount);

		//DrawFormatString(0, 40, GetColor(255, 255, 255), "SCORE : %d", score);

		//DrawFormatString(0, 60, GetColor(255, 255, 255), "COMBO : %d", combo);
		break;
	case GameStatus::RESULT:
		DrawFormatString(0, 0, GetColor(255, 255, 255), "SCENE : RESULT");

		DrawFormatString(0, 20, GetColor(255, 255, 255), "PRESS ANY KEY : TITLE");

		break;
	case GameStatus::PAUSE:
		DrawFormatString(0, 0, GetColor(255, 255, 255), "SCENE : PAUSE");

		break;
	default:
		break;
	}

}

void GameManager::PassCar()
{
	combo++;
	float comboRate = 0.5;

	score += (baseScore * (combo * comboRate));

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

void GameManager::ToIngame()
{
	normaCars = 0;
	gameTimer_ = 0;
	status_ = GameStatus::INGAME;
}

void GameManager::ToResult()
{
	status_ = GameStatus::RESULT;
}

void GameManager::ToTitle()
{
	status_ = GameStatus::TITLE;
}

void GameManager::ToSelect()
{
	status_ = GameStatus::SELECT;
}
