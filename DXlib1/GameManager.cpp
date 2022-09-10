#include "GameManager.h"
#include "DxLib.h"
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
}

void GameManager::Update()
{

	bool isGameOver = false;

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
