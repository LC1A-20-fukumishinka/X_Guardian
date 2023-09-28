#include "GameManager.h"
#include "DxLib.h"
#include "EaseClass.h"
#include <algorithm>
#include "Matrix4.h"
#include <string>
#include "SoundManager.h"
#include "Input.h"
#include "GameInput.h"
using namespace std;

const int GameManager::gameLevelMax = 20;
const float GameManager::pickupModeScale = 0.25f;
const float GameManager::unPickupModeScale = 0.1f;
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


	scoreObjectPos_ = Vector3(45.0f, 9.5f, -130.0f);
	scoreNumberObjectPos_ = scoreObjectPos_ + Vector3(4.0f, -7.0f, 0.0f);

	Vector3 comboObjectMovePos = Vector3(-25.0f, 0.3f, 0.0f);
	comboObjectPos_ = scoreObjectPos_ + comboObjectMovePos;
	comboNumberObjectPos_ = comboObjectPos_ + Vector3(6.5f, -7.0f, 0.0f);
	comboObjectAnimationRate_ = 0.0f;


#pragma region result
	scoreResultObjectPos_ = Vector3(0.0f, 60.0f, -70.0f);
	scoreResultNumberObjectPos_ = scoreResultObjectPos_ + Vector3(-8.5f, -40.0f, -30.0f);
	scoreObjectAnimationRate_ = 0.0f;
#pragma endregion


	titleObjectPos_ = Vector3(4, 40.0f, -120.0f);
	titleObjectAnimationRate_ = 1.0f;

	pressAnyKeyObjectPos_ = Vector3(0.0f, 10.0f, -120.0f);
	pressAnyKeyObjectAnimationRate_ = 1.0f;

	timeObjectPos_ = Vector3(0.0f, 70.0f, -70.0f);
	timeNumberObjectPos_ = timeObjectPos_ + Vector3(6.3f, -10.0f, 0.0f);
	timeObjectAnimationRate_ = 0.0f;

	const int comboObjectMaxCount = 30;
	comboPos.reserve(comboObjectMaxCount);
	comboPos.resize(comboObjectMaxCount);
	CrashAnimationTargetPos_ = { 0.0f,0.0f ,0.0f };
	textScales = { 1.0f, 1.25f, 1.25f, 0.75f };

}

void GameManager::Update(bool isSoloMode)
{

	bool isGameOver = false;
	float easeRate = Easing::easeOutQuad(animationRate);

	int sec = 60;

	Vector3 angle;
	Vector3 animationEndPos;
	Vector3 distance;
	Vector3 easePos;
	Vector3 easeTargetPos;



	switch (status_)
	{
	case GameStatus::TITLE:
		if (GameInput::Done(GameNum::SOLO) && !isInput_ && !Input::isKey(KEY_INPUT_ESCAPE))
		{
			status_ = GameStatus::SELECT;
			sounds_->Enter();
		}
		animationRate -= 0.1f;

		animationRate = std::clamp(animationRate, 0.0f, 1.0f);
		isSolo = isSoloMode;

		if (gameNumber == GameNum::PLAYER2 && !isSoloMode)
		{
			ToIngame();
		}
		break;
	case GameStatus::SELECT:

		if (GameInput::Done(GameNum::SOLO) && !isInput_ && !Input::isKey(KEY_INPUT_ESCAPE) && (baseScaleRate >= 1.0f))
		{
			modeSelected = true;
			sounds_->Enter();
		}

		if (modeSelected && !isInput_ && !Input::isKey(KEY_INPUT_ESCAPE) && (baseScaleRate <= 0.0f))
		{
			ToIngame();
			modeSelected = false;
			ToIngameFlag = true;
		}
		break;
	case GameStatus::INGAME:

		gameLevel_ = (elapsedTime_ / (5 * sec));

		gameLevel_ = std::clamp(gameLevel_, 0, gameLevelMax);
		if (!isDeadAnimation_)
		{
			gameTimer_++;
		}
		//isClear_ = normaCars >= maxNormaCount;
		isFailed_ = ((isSolo && gameTimer_ >= TimeLimit));

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
				life--;
				gameTimer_ += 60 * SubSec;
				SubSecObjectAnimationRate_ = 0.0f;
			}
			animationRate -= 0.1f;
			rotation = 0.0f;
		}
		isOlddeadAnimation = isDeadAnimation_;
		animationRate = std::clamp(animationRate, 0.0f, 1.0f);

		angle = transform(cameraDeadAnimationPos_, rotationY(rotation));
		addScoreAnimationRate_ += (1.0f / 30.0f);
		addScoreAnimationRate_ = std::clamp(addScoreAnimationRate_, 0.0f, 1.0f);

		countDownAnimationRate_ += (1.0f / 10.0f);
		countDownAnimationRate_ = std::clamp(countDownAnimationRate_, 0.0f, 1.0f);
		elapsedTime_++;

		break;
	case GameStatus::RESULT:
		gameLevel_ = 0;
		if (GameInput::Done(GameNum::SOLO) && !isInput_ && isCarAllDead_)
		{
			ToTitle();
		}
		break;
	case GameStatus::PAUSE:
		break;
	default:
		break;
	}

	//モード選択のスケール設定
	if (this->isSoloMode)
	{
		leftModeScaleRate += 0.2f;
	}
	else
	{
		leftModeScaleRate -= 0.2f;
	}
	if (status_ == GameStatus::SELECT && !modeSelected && titleObjectAnimationRate_ <= 0.0f)
	{
		baseScaleRate += 0.15f;
	}
	else
	{
		baseScaleRate -= 0.15f;
	}
	leftModeScaleRate = std::clamp(leftModeScaleRate, 0.0f, 1.0f);
	baseScaleRate = std::clamp(baseScaleRate, 0.0f, 1.0f);

	//					通常時のターゲット位置							衝突演出時のターゲット位置
	easeTargetPos = (cameraBaseTargetPos_ * (1 - animationRate)) + (CrashAnimationTargetPos_ * (animationRate));
	animationEndPos = angle + easeTargetPos;
	distance = animationEndPos - cameraBasePos_;


	easePos = cameraBasePos_ + (distance * animationRate);
	SetCameraPositionAndTargetAndUpVec(easePos, easeTargetPos, Vector3(0.0f, 1.0f, 0.0f));

	//見下ろしカメラ（デバッグ用）
	//SetCameraPositionAndTargetAndUpVec(Vector3(0,500, -120), Vector3(0, 0, -100), Vector3(0.0f, 1.0f, 0.0f));

	isNotAnimationEnd_ = false;
	TitleObjectUpdate();
	ModeSelectObjectUpdate();
	scoreObjectUpdate();
	PressAnyKeyUpdate();
	AddTimeUpdate();
	ComboObjectUpdate();

	isInput_ = (isNotAnimationEnd_);
}

void GameManager::TitleObjectUpdate()
{
	if (gameNumber == GameNum::PLAYER2)
	{
		titleObjectAnimationRate_ = 0.0f;
	}

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

void GameManager::ModeSelectObjectUpdate()
{
	if (gameNumber == GameNum::PLAYER2)
	{
		ModeAnimationRate_ = 0.0f;
	}

	const float moveRate = 0.03f;
	if (status_ == GameStatus::SELECT)
	{
		ModeAnimationRate_ += moveRate;
	}
	else
	{
		ModeAnimationRate_ -= moveRate;
	}

	ModeAnimationRate_ = clamp(ModeAnimationRate_, 0.0f, 1.0f);
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
	if (gameNumber == GameNum::PLAYER2)
	{
		pressAnyKeyObjectAnimationRate_ = 0.0f;
	}

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
	ModeSelectDraw();
	if (!isMenu)
	{
		TitleDraw();
		PressAnyKeyDraw();
	}
	ComboObjectDraw();

	if (isDeadAnimation_)
	{
		ConcentLineDraw();
	}

}

void GameManager::FrameDraw()
{
	int lifeGraphWidth = 31;

	int lifeHeight = 300;

	int sukima = 95;

	int drawHandle = lifeHandle;
	if (gameNumber == GameNum::PLAYER1)
	{
		DrawGraph(0, 0, frameHandle, TRUE);
		DrawGraph(600, 0, P1UIframeHandle, TRUE);

		for (int i = 0; i < 3; i++)
		{
			if (3 - i <= life)
			{
				drawHandle = lifeHandle;
			}
			else
			{
				drawHandle = lifeEmptyHandle;
			}
			DrawGraph(640 - lifeGraphWidth - 6, 53 + (i * sukima), drawHandle, TRUE);
		}
	}
	else if (gameNumber == GameNum::PLAYER2)
	{
		DrawGraph(680, 0, frameHandle, TRUE);
		DrawGraph(600, 350, P2UIframeHandle, TRUE);


		for (int i = 0; i < 3; i++)
		{
			if (i < life)
			{
				drawHandle = lifeHandle;
			}
			else
			{
				drawHandle = lifeEmptyHandle;
			}
			DrawGraph(640 - lifeGraphWidth + 4, 362 + (i * sukima), drawHandle, TRUE);
		}
	}
}

void GameManager::PassCar(Vector3 pos, bool SoloHardMode)
{
	combo++;
	if (combo % 50 == 0)
	{
		requestToPopAmbulanceCount += 5;
	}
	else if (combo % 10 == 0)
	{
		requestToPopAmbulanceCount++;
	}

	score++;
	addScoreAnimationRate_ = 0.0f;
	int tmpTimer = comboPos.begin()->comboCount;

	//コンボ対象の選択
	std::vector<UpperComboObject>::iterator itr = comboPos.begin();
	for (auto tmpItr = comboPos.begin(); tmpItr != comboPos.end(); tmpItr++)
	{
		if (tmpItr->timer < itr->timer)
		{
			itr = tmpItr;
		}
		if (itr->timer <= 0)
		{
			break;
		}
	}

	const int timerMax = 60;
	itr->timer = timerMax;
	itr->comboCount = combo;
	itr->pos = pos;
	itr->pos += Vector3(0.0f, 10.0f, 0.0f);
	if (gameNumber == GameNum::SOLO && combo % 5 == 0)
	{
		if (SoloHardMode)
		{
			AddSec = 1.5f;
		}
		else
		{
			AddSec = 2.5f;
		}
		gameTimer_ -= static_cast<int>(60 * AddSec);
		sounds_->AddTime();
		sounds_->Crap();
		AddSecObjectAnimationRate_ = 0.0f;
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

void GameManager::SetDeadAnimationPos(Vector3 carPos)
{
	CrashAnimationTargetPos_ = carPos;
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
	pressAnyKeyHandle_ = MV1LoadModel("Resources/press_to_space/press_to_space.mv1");
	timeTextObjectHandle_ = MV1LoadModel("Resources/time_ui/time_ui.mv1");
	AddHandle_ = MV1LoadModel("Resources/add_sub/add.mv1");
	SubHandle_ = MV1LoadModel("Resources/add_sub/sub.mv1");

	ScoreModeHandle_ = MV1LoadModel("Resources/mode_select/score_mode.mv1");
	VSModeHandle_ = MV1LoadModel("Resources/mode_select/vs_mode.mv1");

	VSModeTextureHandle_ = LoadGraph("Resources/Texture/VS_mode.png");
	ScoreModeTextureHandle_ = LoadGraph("Resources/Texture/SCORE_mode.png");

	concentLineHandles_[0] = LoadGraph("Resources/Texture/concent_line_01.png");
	concentLineHandles_[1] = LoadGraph("Resources/Texture/concent_line_02.png");

	MenuText = LoadGraph("Resources/Texture/Menu_txt.png");

	menuTexts[0] = LoadGraph("Resources/Texture/Back_txt.png");
	menuTexts[1] = LoadGraph("Resources/Texture/Retry_txt.png");
	menuTexts[2] = LoadGraph("Resources/Texture/Title_txt.png");
	menuTexts[3] = LoadGraph("Resources/Texture/End_txt.png");

	P1frameHandle = LoadGraph("Resources/Texture/frame_1p.png");
	P2frameHandle = LoadGraph("Resources/Texture/frame_2p.png");
	P1UIframeHandle = LoadGraph("Resources/Texture/frame_1p_UI.png");
	P2UIframeHandle = LoadGraph("Resources/Texture/frame_2p_UI.png");

	lifeHandle = LoadGraph("Resources/Texture/HP_UI_01.png");
	lifeEmptyHandle = LoadGraph("Resources/Texture/HP_UI_02.png");
	frameHandle = P1frameHandle;

	winHandle_ = MV1LoadModel("Resources/win/win.mv1");
	loseHandle_ = MV1LoadModel("Resources/lose/lose.mv1");
}

void GameManager::ModeSelectDraw()
{
	Matrix4 cameraPosture = Posture((cameraBaseTargetPos_ - cameraBasePos_), Vector3(0.0f, 1.0f, 0.0f));
	Matrix4 worldMat;

	if (gameNumber == GameNum::SOLO)
	{
		worldMat = scale(Vector3(0.1f, 0.1f, 0.1f));
	}
	else
	{
		worldMat = scale(Vector3(0.06f, 0.06f, 0.06f));
	}

	worldMat *= cameraPosture;

	SelectedModeObjectPos_.y = sin(level_ * (DX_PI_F / 180.0f)) * 2 + 40;
	NotSelectedModeObjectPos_.y = 30;
	SelectedModeObjectPos_.z = -100.0f;
	NotSelectedModeObjectPos_.z = -80.0f;

	Vector3 SelectedPos = SelectedModeObjectPos_;
	Vector3 NotSelectedPos = NotSelectedModeObjectPos_;

	Vector3 moveVec(0.0f, 100.0f, 0.0f);

	moveVec = transform(moveVec, cameraPosture);
	float easeRate = Easing::easeOutBack(ModeAnimationRate_);
	SelectedPos += (moveVec * (1 - easeRate));
	NotSelectedPos += (moveVec * (1 - easeRate));


	float widthRate = 30.0f;
	Vector3 ScoreModeObjectPos;
	Vector3 VSModeObjectPos;
	if (isSoloMode)
	{
		ScoreModeObjectPos = SelectedPos + Vector3(-widthRate, 0.0f, 0.0f);
		VSModeObjectPos = NotSelectedPos + Vector3(widthRate, 0.0f, 0.0f);
	}
	else
	{
		ScoreModeObjectPos = NotSelectedPos + Vector3(-widthRate, 0.0f, 0.0f);
		VSModeObjectPos = SelectedPos + Vector3(widthRate, 0.0f, 0.0f);
	}

	MV1SetMatrix(VSModeHandle_, worldMat * translate(VSModeObjectPos));
	MV1SetMatrix(ScoreModeHandle_, worldMat * translate(ScoreModeObjectPos));

	//MV1DrawModel(VSModeHandle_);
	//MV1DrawModel(ScoreModeHandle_);

}

void GameManager::ModeSelectDraw2D()
{

	Vector3 scoreCenterPos = { 400.0f, 360.0f, 0.0f };
	Vector3 vsCenterPos = { 820.0f, 360.0f, 0.0f };

	Vector3 modeScale = { 1024.0f, 1118.0f , 0 };

	float easePickupModeRate = Easing::EaseCalc(Easing::EaseMove::InOut, Easing::Type::Cubic, leftModeScaleRate);
	float leftModeRate = (unPickupModeScale * (1.0f - easePickupModeRate)) + (pickupModeScale * easePickupModeRate);
	float rightModeRate = (unPickupModeScale * easePickupModeRate) + (pickupModeScale * (1.0f - easePickupModeRate));

	float easeBaseModeRate = Easing::EaseCalc(Easing::EaseMove::Out, Easing::Type::Back, baseScaleRate);

	leftModeRate *= easeBaseModeRate;
	rightModeRate *= easeBaseModeRate;
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 100);

	{
		Vector3 LT = scoreCenterPos - (modeScale * leftModeRate);
		Vector3 BR = scoreCenterPos + (modeScale * leftModeRate);
		DrawExtendGraph(LT.x, LT.y, BR.x, BR.y, ScoreModeTextureHandle_, TRUE);
	}

	{

		Vector3 LT = vsCenterPos - (modeScale * rightModeRate);
		Vector3 BR = vsCenterPos + (modeScale * rightModeRate);
		DrawExtendGraph(LT.x, LT.y, BR.x, BR.y, VSModeTextureHandle_, TRUE);
	}
}

void GameManager::TitleDraw()
{
	Matrix4 cameraPosture = Posture((cameraBaseTargetPos_ - cameraBasePos_), Vector3(0.0f, 1.0f, 0.0f));
	Matrix4 worldMat;

	if (gameNumber == GameNum::SOLO)
	{
		worldMat = scale(Vector3(0.1f, 0.1f, 0.1f));
	}
	else
	{
		worldMat = scale(Vector3(0.06f, 0.06f, 0.06f));
	}

	worldMat *= cameraPosture;


	titleObjectPos_.y = sin(level_ * (DX_PI_F / 180.0f)) * 2 + 50;

	level_ += 2.0f;

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
	if (gameNumber == GameNum::SOLO)
	{
#pragma region scoreNumber
		//scoreNumber表示
		{
			int deget = 0;

			std::vector<int> scoreNums;
			scoreNums.resize(5);
			int tmpScore = score;
			for (int i = 1; i <= scoreNums.size(); i++)
			{
				int tmpDegit = tmpScore % 10;
				scoreNums[scoreNums.size() - i] = tmpDegit;

				tmpScore /= 10;
			}

			//拡縮アニメーション用変数
			float addScoreEaseRate = Easing::EaseCalc(Easing::Out, Easing::Type::Cubic, addScoreAnimationRate_);
			addScoreEaseRate = 1.0f + (0.2f * (1.0f - addScoreEaseRate));
			float drawRate = scoreScale * addScoreEaseRate;
			for (auto &e : scoreNums)
			{

				Matrix4 worldMat;


				worldMat = scale(Vector3(drawRate, drawRate, drawRate));

				worldMat *= cameraPosture;


				Vector3 easePos = scoreNumberObjectPos_ + Vector3(-10.0f + ((125.0f * drawRate) * deget), 0.0f, 0.0f);

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
	}
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

		//拡縮アニメーション用変数
		float addComboEaseRate = Easing::EaseCalc(Easing::Out, Easing::Type::Cubic, addScoreAnimationRate_);
		addComboEaseRate = 1.0f + (0.2f * (1.0f - addComboEaseRate));
		float drawRate = scoreScale * addComboEaseRate;

		for (auto &e : comboNums)
		{

			Matrix4 worldMat;
			worldMat = scale(Vector3(drawRate, drawRate, drawRate));

			worldMat *= cameraPosture;

			Vector3 easePos = comboNumberObjectPos_ + Vector3(-10.0f + ((125.0f * drawRate) * (deget)), 0.0f, 0.0f);
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
	if (gameNumber == GameNum::SOLO)
	{
#pragma region timeNumber
		{
			int deget = 0;
			std::vector<int> timeNums;
			timeNums.resize(2);
			int tmpTime = TimeLimit - gameTimer_;
			bool isNotTimeup = (tmpTime > 0);
			tmpTime /= 60;

			if (tmpTime != oldCount)
			{
				countDownAnimationRate_ = 0.0f;
			}
			oldCount = tmpTime;

			tmpTime = std::clamp(tmpTime, 0, 99);
			if (isNotTimeup)
			{
				tmpTime++;
			}
			for (int i = 1; i <= timeNums.size(); i++)
			{
				int tmpDegit = tmpTime % 10;
				timeNums[timeNums.size() - i] = tmpDegit;

				tmpTime /= 10;
			}

			//拡縮アニメーション用変数
			float countdownEaseRate = Easing::EaseCalc(Easing::Out, Easing::Type::Cubic, countDownAnimationRate_);
			countdownEaseRate = 1.0f + (0.2f * (1.0f - countdownEaseRate));
			float drawRate = timeScale * countdownEaseRate * 2;
			for (auto &e : timeNums)
			{

				Matrix4 worldMat;
				worldMat = scale(Vector3(drawRate, drawRate, drawRate));

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


		Vector3 iconMovePos = Vector3(15.0f, 0.0f, 0.0f);
		Vector3 iconBasePos = Vector3(-23.0f, 0.0f, 0.0f);

#pragma region Addtime
		if (AddSecObjectAnimationRate_ < 1.0f)
		{
			Matrix4 worldMat;
			Matrix4 iconMat;

			float scaleRate = (1 - Easing::easeInExpo(AddSecObjectAnimationRate_));
			float baseScale = timeScale * 2 * scaleRate;
			worldMat = scale(Vector3(baseScale, timeScale * 2, timeScale * 2));

			worldMat *= cameraPosture;
			iconMat = worldMat;

			Vector3 easePos = timeNumberObjectPos_ - iconMovePos + iconBasePos;
			Vector3 iconEasePos = timeNumberObjectPos_ - iconMovePos + iconBasePos;

			float easeRate = Easing::easeOutExpo(AddSecObjectAnimationRate_);
			easePos += ((iconMovePos * easeRate) + Vector3(+3.5f * scaleRate, 0.0f, 0.0f));
			iconEasePos += ((iconMovePos * easeRate) + Vector3(-3.5f * scaleRate, 0.0f, 0.0f));

			worldMat *= translate(easePos);
			iconMat *= translate(iconEasePos);
			MV1SetMatrix(numberObjects_[AddSec], worldMat);

			MV1DrawModel(numberObjects_[AddSec]);

			MV1SetMatrix(AddHandle_, iconMat);

			MV1DrawModel(AddHandle_);

		}
#pragma endregion

#pragma region Subtime
		if (SubSecObjectAnimationRate_ < 1.0f)
		{
			float scaleRate = (1 - Easing::easeInExpo(SubSecObjectAnimationRate_));
			float baseScale = timeScale * 2 * scaleRate;
			Matrix4 worldMat;
			Matrix4 iconMat;
			worldMat = scale(Vector3(baseScale, timeScale * 2, timeScale * 2));

			worldMat *= cameraPosture;
			iconMat = worldMat;

			Vector3 easePos = timeNumberObjectPos_ + iconMovePos + iconBasePos;
			Vector3 iconEasePos = timeNumberObjectPos_ + iconMovePos + iconBasePos;

			float easeRate = Easing::easeOutExpo(SubSecObjectAnimationRate_);
			easePos += (-iconMovePos * easeRate) + Vector3(+3.5f * scaleRate, 0.0f, 0.0f);
			iconEasePos += (-iconMovePos * easeRate) + Vector3(-3.5f * scaleRate, 0.0f, 0.0f);

			worldMat *= translate(easePos);
			iconMat *= translate(iconEasePos);
			MV1SetMatrix(numberObjects_[3], worldMat);

			MV1DrawModel(numberObjects_[3]);

			MV1SetMatrix(SubHandle_, iconMat);

			MV1DrawModel(SubHandle_);

		}
#pragma endregion

	}
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
	//scoreResultNumber表示
	{
		if (gameNumber == GameNum::SOLO)
		{
			int i = 0;

			std::vector<int> scoreNums;
			scoreNums.resize(5);
			int tmpScore = score;
			for (int i = 1; i <= scoreNums.size(); i++)
			{
				int degit = tmpScore % 10;
				scoreNums[scoreNums.size() - i] = degit;

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
		else
		{

			Matrix4 worldMat;

			float animationRate = sin(level_ * (DX_PI_F / 180.0f));
			float scaleRate = animationRate * 0.01;
			if (life <= 0)
			{
				worldMat = scale(Vector3(0.1f + scaleRate, 0.1f + scaleRate, 0.1f + scaleRate));
			}
			else
			{
				scaleRate *= 2.0f;
				worldMat = scale(Vector3(0.2f + scaleRate, 0.2f + scaleRate, 0.1f + scaleRate));
			}

			worldMat *= cameraPosture;


			Vector3 easePos = scoreResultNumberObjectPos_;
			easePos.y = 40.0f + sin(level_ * (DX_PI_F / 180.0f)) + 1.0f;

			float easeRate = Easing::easeOutBack(rate[0]);


			easePos += (moveVec * (1 - easeRate));

			easePos.x = 0.0f;

			worldMat *= translate(easePos);

			int drawHandle = winHandle_;
			if (life <= 0)
			{
				drawHandle = loseHandle_;
			}
			MV1SetMatrix(drawHandle, worldMat);

			MV1DrawModel(drawHandle);
		}
	}


	if (gameNumber == GameNum::SOLO)
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
	if (gameNumber == GameNum::SOLO)
	{
		worldMat = scale(Vector3(0.05f, 0.05f, 0.05f));
	}
	else
	{
		worldMat = scale(Vector3(0.04f, 0.04f, 0.04f));
	}
	worldMat *= cameraPosture;

	pressAnyKeyObjectPos_.y = sin(level_ * (DX_PI_F / 180.0f)) * 1 + 10.0f;



	Vector3 easePos = pressAnyKeyObjectPos_;

	if (gameNumber != GameNum::SOLO)
	{
		easePos.y += 5.0f;
	}
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

void GameManager::ConcentLineDraw()
{
	isFlipConcentLine = !isFlipConcentLine;

	if (drawConcentTurnFlag)
	{
		DrawTurnGraph(0, 0, concentLineHandles_[drawConcentLineNum], TRUE);
	}
	else
	{
		DrawGraph(0, 0, concentLineHandles_[drawConcentLineNum], TRUE);
	}
	if (isFlipConcentLine)
	{
		drawConcentTurnFlag = !drawConcentTurnFlag;
		if (drawConcentTurnFlag)
		{
			if (drawConcentLineNum == 0)
			{
				drawConcentLineNum = 1;
			}
			else
			{
				drawConcentLineNum = 0;
			}
		}
	}
}

void GameManager::MenuDraw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 100);

	float xc = 640.0f;
	float yc = 80.0f;
	float textWidth = 100.0f;

	float height = textWidth / 3.0f;

	DrawExtendGraphF(xc - textWidth, yc - height,
		xc + textWidth, yc + height, MenuText, TRUE);

	yc += 200.0f;

	float menuEaseScale = Easing::EaseCalc(Easing::Out, Easing::Type::Cubic, menuScale);

	float moveRate = (1.0f / 23.25f);
	menuScale += moveRate;
	menuScale = std::clamp(menuScale, 0.0f, 1.0f);
	for (int i = 0; i < menuTexts.size(); i++)
	{
		float drawWidth = textWidth * textScales[i];
		float animationHeightScale = (1.0f * menuEaseScale) * height;

		if (menuNumber == i)
		{
			DrawExtendGraphF(xc - drawWidth, yc - animationHeightScale,
				xc + drawWidth, yc + height, menuTexts[i], TRUE);
		}
		else
		{
			DrawExtendGraphF(xc - drawWidth, yc - height,
				xc + drawWidth, yc + height, menuTexts[i], TRUE);
		}
		yc += 100.0f;
	}


	if (menuScale >= 1.0f)
	{
		menuScale = 0.0f;
	}
}

void GameManager::CheckCarAllDead(bool isAllDead)
{
	isCarAllDead_ = isAllDead;
}

Matrix4 GameManager::GetCamMat()
{
	return  Posture((cameraBaseTargetPos_ - cameraBasePos_), Vector3(0.0f, 1.0f, 0.0f));
}

int GameManager::GetCombo()
{
	return combo;
}

void GameManager::SetSoundManager(SoundManager *sound)
{
	sounds_ = sound;
}

void GameManager::ComboObjectUpdate()
{
	for (auto &e : comboPos)
	{
		if (e.timer > 0)
		{
			e.timer--;
		}
	}
}

void GameManager::ComboObjectDraw()
{

	Vector3 moveVec = Vector3(0.0f, 20.0f, 0.0f);

	float comboScale = 0.05f;
	for (auto &e : comboPos)
	{
		if (e.timer > 0)
		{
			float rate = (1 - (e.timer / 60.0f));


			int degetMoveCount;
#pragma region comboNum
			{
				int deget = 0;

				std::vector<int> comboNums;
				comboNums;
				int tmpCombo = e.comboCount;
				for (int i = 0; 10 <= tmpCombo; i++)
				{

					int tmpDegit = tmpCombo % 10;
					comboNums.emplace_back(tmpDegit);

					tmpCombo /= 10;
				}
				comboNums.emplace_back(tmpCombo);
				int digitsCount = static_cast<int>(comboNums.size());

				degetMoveCount = digitsCount;
				degetMoveCount--;
				for (auto &num : comboNums)
				{

					Matrix4 worldMat;

					float scaleRate = Easing::easeInExpo(rate);
					scaleRate = (1.0f - scaleRate);
					worldMat = scale(Vector3(comboScale, comboScale * scaleRate, comboScale));

					//								   一の位					  そこからずらす量
					Vector3 easePos = e.pos + Vector3((2.5f * degetMoveCount) - ((125.0f * comboScale) * (deget)), -15.0f * scaleRate, 0.0f);

					easePos = easePos;

					float easeRate = Easing::easeOutCubic(rate);
					easePos += ((moveVec + Vector3(0.0f, 0.0f, 0.0f)) * easeRate);

					worldMat *= translate(easePos);

					deget++;
					MV1SetMatrix(numberObjects_[num], worldMat);

					MV1DrawModel(numberObjects_[num]);
				}
			}

#pragma endregion

#pragma region comboText
			{
				Matrix4 worldMat;

				float scaleRate = Easing::easeInExpo(rate);
				scaleRate = (1.0f - scaleRate);

				float textScale = comboScale / 1.0f;
				textScale *= scaleRate;
				worldMat = scale(Vector3(textScale, textScale, textScale));

				Vector3 easePos = e.pos + Vector3(/*(5.0f * degetMoveCount)*/0.0f, -5.0f * scaleRate, 0.0f);

				easePos += Vector3();
				float easeRate = Easing::easeOutCubic(rate);
				easePos += (moveVec * easeRate);


				worldMat *= translate(easePos);
				MV1SetMatrix(comboTextObject_, worldMat);

				MV1DrawModel(comboTextObject_);
			}
#pragma endregion
		}
	}
}

void GameManager::AddTimeUpdate()
{
	AddSecObjectAnimationRate_ += (1.0f / 60.0f);
	SubSecObjectAnimationRate_ += (1.0f / 60.0f);
	AddSecObjectAnimationRate_ = clamp(AddSecObjectAnimationRate_, 0.0f, 1.0f);
	SubSecObjectAnimationRate_ = clamp(SubSecObjectAnimationRate_, 0.0f, 1.0f);
}

int GameManager::GetGameLevel()
{
	return gameLevel_;
}

void GameManager::SetMenuDatas(bool isMenu, int menuNumber)
{
	if (this->menuNumber != menuNumber)
	{
		menuScale = 0.0f;
	}

	this->isMenu = isMenu;
	this->menuNumber = menuNumber;
}

void GameManager::Retry()
{
	if (gameNumber != GameNum::PLAYER2)
	{
		sounds_->BGMStop();
	}
	ToIngame();

}

int GameManager::RequestToPopAmbulance()
{
	int tmpRequestToPopAmbulanceCount = requestToPopAmbulanceCount;
	requestToPopAmbulanceCount = 0;
	return tmpRequestToPopAmbulanceCount;
}

void GameManager::SetIsSoloMode(bool isSoloMode)
{
	this->isSoloMode = isSoloMode;
}

bool GameManager::GetToIngameFlag()
{
	if (!ToIngameFlag) return false;
	ToIngameFlag = false;
	return 	true;
}

bool GameManager::GetSelectMode()
{
	return leftModeScaleRate;
}

void GameManager::ToIngame()
{
	score = 0;
	combo = 0;
	life = 3;
	normaCars = 0;
	gameTimer_ = 0;
	elapsedTime_ = 0;
	gameLevel_ = 0;
	sounds_->IngameVolume();
	requestToPopAmbulanceCount = 0;
	if (gameNumber != GameNum::PLAYER2)
	{
		sounds_->BGM();
	}
	status_ = GameStatus::INGAME;
}


void GameManager::ToResult()
{
	if (gameNumber != GameNum::PLAYER2)
	{
		sounds_->Jingle();
		sounds_->ResultVolume();
	}
	status_ = GameStatus::RESULT;
	isFailed_ = false;
	level_ = 0;
}


void GameManager::ToTitle()
{
	//タイトルオブジェクトのアニメーションが始まるのでフラグをオンにする
	isInput_ = true;
	isDeadAnimation_ = false;
	isOlddeadAnimation = false;
	life = 3;


	modeSelected = false;
	ToIngameFlag = true;

	if (gameNumber != GameNum::PLAYER2)
	{
		sounds_->StopJingle();
		sounds_->TitleVolume();
		sounds_->BGMStop();
		sounds_->Enter();
	}

	status_ = GameStatus::TITLE;
}

void GameManager::SetMenuDone(bool menuDone)
{
	this->menuDone = menuDone;
}

void GameManager::SetGameNum(GameNum num)
{
	this->gameNumber = num;

	if (gameNumber == GameNum::PLAYER1)
	{
		frameHandle = P1frameHandle;

	}
	else if (gameNumber == GameNum::PLAYER2)
	{
		frameHandle = P2frameHandle;
	}
	isSolo = (gameNumber == GameNum::SOLO);
}

bool GameManager::GetisGameOver()
{
	return (status_ == GameStatus::INGAME && !isSolo && life <= 0 && animationRate <= 0.0f);
}

int GameManager::SendObstacles()
{
	if (!isSendObstacles)return 0;

	isSendObstacles = false;
	int sendObstaclesCount = combo;
	combo = 0;
	return sendObstaclesCount;
}


void GameManager::ToSelect()
{
	status_ = GameStatus::SELECT;
}