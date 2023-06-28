#include "Game.h"
#include "EaseClass.h"
#include <algorithm>
#include "GameInput.h"
#include "Input.h"
Game::Game()
{
	//通常
	Vector3 cameraOrgPosition(0.0f, 70.0f, -130.0f + camZPos);
	Vector3 cameraOrgUp(0.0f, 1.0f, 0.0f);

	//デバッグ
	//Vector3 cameraOrgPosition(0.0f, 200.0f, 0.0f);
	//Vector3 cameraOrgUp(0.0f, 0.0f, 1.0f);

	cameraPosition = cameraOrgPosition;
	cameraTarget = Vector3(0.0f, 0.0f, 0.0f + camZPos);

	cameraUp = cameraOrgUp;

	BaseInitialize();



	float cameraRightAngle = 0.0f;
	float cameraUpAngle = 0.0f;


	model = MV1LoadModel("Resources/city/city_object.mv1");
	ground = MV1LoadModel("Resources/city/city_ground.mv1");
	skyModel = MV1LoadModel("Resources/skydome/sky_dome.mv1");
	info = MV1LoadModel("Resources/signboard/signboard.mv1");
	Vector3 BasePos(-20, 0.1, 302);
	Sphere sphere(BasePos, 5, GetColor(0, 0, 255));

	float cityScale = 0.2f;
	Matrix4 matScale = scale(Vector3(cityScale, cityScale, cityScale));
	Matrix4 matRot = rotate(quaternion(Vector3(0, 1, 0), 0));
	Matrix4 matTrans = translate(BasePos);


	Quaternion qLocal = quaternion(Vector3(0.0f, 1.0f, 0.0f), 0);


	Quaternion tmpQ = quaternion(Vector3(0, 1, 0), qLocal);
	Vector3 vUpAxis = getAxis(tmpQ);
	tmpQ = quaternion(Vector3(0, 0, 1), qLocal);
	Vector3 vForwordAxis = getAxis(tmpQ);
	Quaternion rot = quaternion(Vector3(0, 1, 0), 0.0f);
	qLocal = rot * qLocal;

	matWorld = matScale;
	matWorld *= matRot;
	matWorld *= translate(BasePos);
	MV1SetMatrix(ground, matWorld);

	cameraUpAngle = 0.0f;
	cameraRightAngle = 0.0f;

	skyMat = scale(Vector3(10.0f, 10.0f, 10.0f));
	skyMat *= rotate(qLocal);
	BasePos += vForwordAxis;
	skyMat *= translate(BasePos);

	gameManager.Init();
	gameManager.SetCameraPos(cameraOrgPosition, cameraTarget);
	gameManager.Load();

	carManager.SetCamMat(gameManager.GetCamMat());
	OldScene = gameManager.GetStatus();
	int test = SetLightAmbColor(GetColorF(1.0f, 1.0f, 1.0f, 0.0f));

	ChangeLightTypeDir(Vector3(0.0f, -1.0f, 1.0f));


	soundTimers_.resize(5);
	soundTimersMax_.resize(5);
	soundTimersMax_[static_cast<int>(TimerName::WHISTLE)].Set(600, 1200);
	soundTimersMax_[static_cast<int>(TimerName::BRAKE)].Set(300, 480);
	soundTimersMax_[static_cast<int>(TimerName::HORN)].Set(180, 300);
	soundTimersMax_[static_cast<int>(TimerName::ENGINE)].Set(120, 240);
	soundTimersMax_[static_cast<int>(TimerName::BROKEN)].Set(600, 1200);

	int count = 0;
	for (auto& e : soundTimers_)
	{
		e = SetRandTimer(soundTimersMax_[count]);
		e -= (120 * (count % 2));
		count++;
	}

	xGuardian.Init();
	for (auto& e : isComboEffects)
	{
		e = false;
	}

}

Game::~Game()
{
}

void Game::Init(bool makeLighrFlag)
{
	RoadSign::LoadModel();
	carManager.LoadGraphics();
	xGuardian.LoadModel();
	screenNum = MakeScreen(1280, 720, TRUE);

	if (makeLighrFlag)
	{
		leftLightHandle = CreateDirLightHandle(VGet(1.0f, -0.2f, 0.0f));
		rightLightHandle = CreateDirLightHandle(VGet(-1.0f, -0.2f, 0.0f));
	}

}

void Game::Finalize()
{
}

void Game::IngameDraw()
{
	ClearDrawScreen();
	//DrawAxis3D(200.0f);
	//DrawFloorLine();

	MV1SetMatrix(model, matWorld);
	MV1DrawModel(model);

	MV1SetMatrix(info, boardMat);
	MV1DrawModel(info);
	MV1DrawModel(ground);
	MV1SetMatrix(skyModel, skyMat);

	MV1DrawModel(skyModel);

	carManager.Draw();

	gameManager.Draw();

	switch (gameManager.GetStatus())
	{
	case GameStatus::TITLE:
		break;
	case GameStatus::SELECT:
		break;
	case GameStatus::INGAME:
		if (!carManager.GetDeadAnimation())
		{
			carManager.DrwaHud();
		}
		xGuardian.Draw();
		roadSignManager.Draw();
		break;
	case GameStatus::RESULT:
		break;
	case GameStatus::PAUSE:
		break;
	default:
		break;
	}

	if (isMenu)
	{
		gameManager.MenuDraw();
	}

}

void Game::Draw()
{
	if (gameNumber != static_cast<int>(GameNum::PLAYER2))
	{
		SetDrawScreen(DX_SCREEN_BACK);
	}

	switch (static_cast<GameNum>(gameNumber))
	{
	case GameNum::SOLO:
		DrawGraph(0, 0, screenNum, TRUE);
		break;
	case GameNum::PLAYER1:
		DrawRectGraph(0, 0, 340, 0, 640, 720, screenNum, TRUE);
		break;
	case GameNum::PLAYER2:
		DrawRectGraph(640, 0, 340, 0, 640, 720, screenNum, TRUE);
		break;
	default:
		break;
	}
}

void Game::Update()
{
	carManager.SetGameNumber(static_cast<GameNum>(gameNumber));
	SetDrawScreen(screenNum);

	const int WindowWidth = 1280;
	const int WindowHeight = 720;
	SetCameraNearFar(1.0f, 10000.0f);
	SetCameraScreenCenter(WindowWidth / 2.0f, WindowHeight / 2.0f);
	SetCameraPositionAndTargetAndUpVec(cameraPosition, cameraTarget, cameraUp);
	if (GameInput::Pause(static_cast<GameNum>(gameNumber)))
	{
		isMenu = !isMenu;
		menuTextNumber = static_cast<int>(MenuTextNumbers::BACK);
	}

	if (isMenu)
	{
		MenuUpdate();
	}
	gameManager.SetMenuDatas(isMenu, menuTextNumber);

	if (isMenu) return;
	switch (gameManager.GetStatus())
	{
	case GameStatus::TITLE:
		carManager.SetGameSpeed(1.0f);
		carManager.Update();
		SoundUpdate();

		spawnTimer--;
		if (spawnTimer <= 0)
		{
			spawnTimer = 90;
			carManager.AddPlayerCar();
			carManager.AddEnemyCar(true);
		}

		break;
	case GameStatus::SELECT:
		break;
	case GameStatus::INGAME:
		IngameUpdate();
		break;
	case GameStatus::RESULT:
		carManager.Update();
		break;
	case GameStatus::PAUSE:
		break;
	default:
		break;
	}

	gameManager.CheckCarAllDead(carManager.GetIsAllCarDead());
	gameManager.Update();
	carManager.SetIsResult((gameManager.GetStatus() == GameStatus::RESULT));
	if (OldScene != gameManager.GetStatus())
	{
		SceneChange();
	}
	OldScene = gameManager.GetStatus();


	Vector3 BasePos(-20, 0.1, 302);


	float cityAnimationScale = 0.05f;

	if (cityAnimationRate >= 1.0f && (gameManager.GetStatus() == GameStatus::INGAME))
	{
		cityAnimationRate = 0.0f;
		int comboCount = gameManager.GetCombo();

		for (int i = 0; i < isComboEffects.size(); i++)
		{
			isComboEffects[i] = (comboCount >= (10 * (i + 1)));
		}
		isTurn = !isTurn;
	}
	else
	{
		float moveRate = (1.0f / 23.25f);

		if (carManager.GetDeadAnimation())
		{
			moveRate *= 0.01f;
		}
		cityAnimationRate += moveRate;
	}


	cityAnimationRate = std::clamp(cityAnimationRate, 0.0f, 1.0f);

	//町ぷにぷにON/OFF
	//cityAnimationRate = 1.0f;

	Car::SetPressAnimationRate(cityAnimationRate);
	float easeRate = Easing::easeOutCubic(cityAnimationRate);
	{
		cityAnimationScale *= (1.0f - easeRate);
		float cityScale = 0.2f;
		Matrix4 matScale = scale(Vector3(cityScale, cityScale - cityAnimationScale, cityScale));
		Matrix4 matRot = rotate(quaternion(Vector3(0, 1, 0), 0));
		Matrix4 matTrans = translate(BasePos);
		matWorld = matScale;


		float skewRate = 0.314f;
		if (isComboEffects[2])
		{
			skewRate /= 1.0f;
		}
		else if (isComboEffects[1])
		{
			skewRate /= 2.0f;
		}
		else if (isComboEffects[0])
		{
			skewRate /= 10.0f;
		}
		else
		{
			skewRate = 0.0f;
		}


		if (isTurn)
		{
			matWorld *= ZSkew((skewRate * easeRate));
		}
		else
		{
			matWorld *= ZSkew((-skewRate * easeRate));

		}
		matWorld *= matRot;
		matWorld *= translate(BasePos);
		xGuardian.ScaleAnimation(easeRate);
		roadSignManager.ScaleAnimation(easeRate);
	}


	{
		float boardScale = 0.1f;
		float boardAnimationScale = boardScale / 20.0f;

		boardAnimationScale *= (1.0f - easeRate);

		Vector3 drawPos = BasePos;

		drawPos += Vector3(-75.0f, 0.0f, -275.0f);

		Vector3 boardBaseScale = Vector3(boardScale, boardScale - boardAnimationScale, boardScale);
		Vector3 NotMoveAnimationScale = Vector3(0.0f, 0.05f, 0.05f);
		float rate = Easing::easeInBack(NotTrackMoveAnimationRate_);
		NotMoveAnimationScale *= (1 - rate);
		Matrix4 matScale = scale(boardBaseScale + NotMoveAnimationScale);
		Matrix4 matRot = rotate(quaternion(Vector3(0, 1, 0), (3.14 / 2.0f)));
		Matrix4 matTrans = translate(BasePos);


		boardMat = matScale;
		boardMat *= matRot;
		boardMat *= translate(drawPos);
	}
}

void Game::TitleUpdate()
{
	carManager.Update();
}

void Game::IngameUpdate()
{
	int spawnTimerMax = 90;
	if (carManager.GetDeadAnimation())
	{
		spawnTimer = spawnTimerMax;
		playerSpawnTimer = spawnTimerMax;
	}
	else
	{
		//スローモーション中は生成しない
		if (!carManager.GetSlowmotion())
		{
			spawnTimer--;
			playerSpawnTimer--;
		}
	}

	//湧く処理
	if (spawnTimer <= 0)
	{
		spawnTimer = spawnTimerMax - (gameManager.GetGameLevel() * 1.5f);

		carManager.AddEnemyCar(false);
	}

	if (playerSpawnTimer <= 0)
	{
		playerSpawnTimer = (spawnTimerMax - 30);
		carManager.AddPlayerCar();
	}


	carManager.SetSignal();
	carManager.SetLevel(gameManager.GetGameLevel());
	carManager.Update();
	std::vector<Vector3> passCarsPos = carManager.GetPassCars();
	for (auto& e : passCarsPos)
	{
		gameManager.PassCar(e);
	}

	if (carManager.GetAnyCarStop())
	{
		comboTimer++;

		const int comboLimit = 40;
		if (comboTimer >= comboLimit)
		{
			gameManager.StopCar();
		}
	}
	else
	{
		comboTimer = 0;
	}

	float gaugeRate = static_cast<float>(comboTimer) / 30.0f;

	gaugeRate = std::clamp(gaugeRate, 0.0f, 1.0f);
	carManager.SetGaugeRate(gaugeRate);

	gameManager.SetIsDeadAnimation(carManager.GetDeadAnimation());
	gameManager.SetDeadAnimationPos(carManager.GetDeadCarPos());
	xGuardian.Update(carManager.GetInputSignal());
	roadSignManager.Update(carManager.GetInputSignal());
	roadSignManager.conboToAlive(gameManager.GetCombo());

	if (carManager.GetIsNotTrackMove())
	{
		NotTrackMoveAnimationRate_ = 0.0f;
		sounds->Buzzer();
	}
	NotTrackMoveAnimationRate_ += 0.04f;

	NotTrackMoveAnimationRate_ = std::clamp(NotTrackMoveAnimationRate_, 0.0f, 1.0f);
}

void Game::MenuUpdate()
{
	//前の選択肢
	if (Input::isKeyTrigger(KEY_INPUT_S) || Input::isKeyTrigger(KEY_INPUT_DOWN))
	{
		menuTextNumber++;
	}

	//次の選択肢
	if (Input::isKeyTrigger(KEY_INPUT_W) || Input::isKeyTrigger(KEY_INPUT_UP))
	{
		menuTextNumber--;
	}

	menuTextNumber = std::clamp(menuTextNumber, 0, 3);
	//選択肢の実行
	if (Input::isKeyTrigger(KEY_INPUT_SPACE) || Input::isKeyTrigger(KEY_INPUT_RETURN))
	{
		switch (static_cast<MenuTextNumbers>(menuTextNumber))
		{
		case MenuTextNumbers::BACK:
			break;
		case MenuTextNumbers::RETRY:
			gameManager.Retry();
			SceneChange();
			break;
		case MenuTextNumbers::TITLE:
			gameManager.ToTitle();
			carManager.EndGame();
			break;
		case MenuTextNumbers::END:
			GameEndFlag = true;
			break;
		default:
			break;
		}

		isMenu = false;
	}
}


void Game::BaseInitialize()
{
	const int WindowWidth = 1280;
	const int WindowHeight = 720;

	ChangeWindowMode(true);
	SetGraphMode(WindowWidth, WindowHeight, 32);
	SetBackgroundColor(0, 0, 64);
	SetWindowText(_T("3008_十字路の守護神"));
	if (DxLib_Init() == -1)return;
	SetDrawScreen(DX_SCREEN_BACK);

	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	//クリップ面
	SetCameraNearFar(1.0f, 10000.0f);
	SetCameraScreenCenter(WindowWidth / 2.0f, WindowHeight / 2.0f);
	SetCameraPositionAndTargetAndUpVec(cameraPosition, cameraTarget, cameraUp);
}

void Game::DrawAxis3D(const float length)
{
	DrawLine3D(Vector3(-length, 0, 0), Vector3(+length, 0, 0), GetColor(255, 0, 0));
	DrawLine3D(Vector3(0, -length, 0), Vector3(0, +length, 0), GetColor(0, 255, 0));
	DrawLine3D(Vector3(0, 0, -length), Vector3(0, 0, +length), GetColor(0, 0, 255));
}


void Game::DrawFloorLine()
{
	float lineLength = 500.0f;
	float width = 10.0f;
	int color = GetColor(100, 100, 100);
	DrawLine3D(Vector3(0, 0, -lineLength), Vector3(0, 0, +lineLength), color);
	DrawLine3D(Vector3(-lineLength, 0, 0), Vector3(+lineLength, 0, 0), color);

	//for (int i = 1; i * width < 500; i++)
	//{
	//	float linePos = i * width;
	//	//Z軸線
	//	DrawLine3D(Vector3(linePos, 0, -lineLength), Vector3(linePos, 0, +lineLength), color);
	//	DrawLine3D(Vector3(-linePos, 0, -lineLength), Vector3(-linePos, 0, +lineLength), color);
	//	//
	//	DrawLine3D(Vector3(-lineLength, 0, linePos), Vector3(+lineLength, 0, linePos), color);
	//	DrawLine3D(Vector3(-lineLength, 0, -linePos), Vector3(+lineLength, 0, -linePos), color);

	//}

}

void Game::SceneChange()
{
	//シーン切り替え処理
	switch (gameManager.GetStatus())
	{
	case GameStatus::TITLE:
		carManager.SetGameSpeed(1.0f);

		break;
	case GameStatus::SELECT:
		break;
	case GameStatus::INGAME:
		carManager.AllDead();
		carManager.Init();
		break;
	case GameStatus::RESULT:
		carManager.EndGame();
		break;
	case GameStatus::PAUSE:
		break;
	default:
		break;
	}
}

void Game::SoundUpdate()
{
	int count = 0;
	for (auto& e : soundTimers_)
	{
		e--;

		if (e <= 0)
		{
			switch (static_cast<TimerName>(count))
			{
			case TimerName::WHISTLE:
				sounds->Whistle();
				break;
			case TimerName::BRAKE:
				sounds->Brake();
				break;
			case TimerName::HORN:
				sounds->Horn();
				break;
			case TimerName::ENGINE:
				sounds->Engine();
				break;
			case TimerName::BROKEN:
				sounds->Broken();
				break;
			default:
				break;
			}
			e = SetRandTimer(soundTimersMax_[count]);
		}

		count++;
	}
}

int Game::SetRandTimer(TimerRange range)
{
	int tmpRange = (range.max - range.min);
	if (tmpRange == 0)
	{
		tmpRange = 1;
	}
	int ret = (rand() % tmpRange);

	ret += range.min;
	return ret;
}

Matrix4 Game::ZSkew(float angle)
{
	Matrix4 a =
	{
		1.0f		,0.0f,0.0f,0.0f,
		tan(angle)	,1.0f,0.0f,0.0f,
		0.0f		,0.0f,1.0f,0.0f,
		0.0f		,0.0f,0.0f,1.0f
	};
	return a;
}

void Game::ToResult()
{
	gameManager.ToResult();
}

bool Game::isGameOver()
{
	return gameManager.GetisGameOver();
}

bool Game::GameEnd()
{
	return GameEndFlag;
}

void Game::SetGameNum(GameNum num)
{
	gameNumber = static_cast<int>(num);
	gameManager.SetGameNum(num);
}

void Game::SetSoundManager(SoundManager* sounds)
{
	this->sounds = sounds;
	sounds->TitleVolume();
	carManager.SetSoundManager(sounds);
	gameManager.SetSoundManager(sounds);

}

GameStatus Game::GetGameStatus()
{
	return gameManager.GetStatus();
}

void TimerRange::Set(int min, int max)
{
	this->min = min;
	this->max = max;
}
