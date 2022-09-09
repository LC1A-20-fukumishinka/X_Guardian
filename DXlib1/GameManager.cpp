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
}

void GameManager::Finalize()
{
}

void GameManager::Draw()
{
	DrawFormatString(0, 0, GetColor(255, 255, 255), "SCORE : %d", score);

	DrawFormatString(0, 20, GetColor(255, 255, 255), "COMBO : %d", combo);
}

void GameManager::PassCar()
{
combo++;
float comboRate = 0.5;

score += (baseScore * (combo * comboRate));
}

void GameManager::StopCar()
{

combo = 0;
}
