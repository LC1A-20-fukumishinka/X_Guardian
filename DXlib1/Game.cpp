#include "Game.h"

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


	model = MV1LoadModel("cross_road/cross_road.mv1");
	//int model = MV1LoadModel("city/city.mv1");

	Vector3 BasePos(0, 0, 225);
	Sphere sphere(BasePos, 5, GetColor(0, 0, 255));

	Matrix4 matScale = scale(Vector3(0.08f, 0.08f, 0.08f));
	Matrix4 matRot = rotate(quaternion(Vector3(0, 1, 0), 3.14));
	Matrix4 matTrans = translate(BasePos);


	Quaternion qLocal = quaternion(Vector3(0.0f, 1.0f, 0.0f), 0);


	Quaternion tmpQ = quaternion(Vector3(0, 1, 0), qLocal);
	Vector3 vUpAxis = getAxis(tmpQ);
	tmpQ = quaternion(Vector3(0, 0, 1), qLocal);
	Vector3 vForwordAxis = getAxis(tmpQ);
	Quaternion rot = quaternion(Vector3(0, 1, 0), 0.0f);
	qLocal = rot * qLocal;

	matWorld = matScale;
	matWorld *= rotate(qLocal);
	BasePos += vForwordAxis *= (-1.0f * 0.0f);
	matWorld *= translate(BasePos);
	cameraUpAngle = 0.0f;
	cameraRightAngle = 0.0f;



	gameManager.Init();

	OldScene = gameManager.GetStatus();
}

Game::~Game()
{
}

void Game::Init()
{
	Car::LoadModel();
}

void Game::Finalize()
{
}

void Game::Draw()
{
	ClearDrawScreen();
	DrawAxis3D(200.0f);
	DrawFloorLine();

	MV1SetMatrix(model, matWorld);
	MV1DrawModel(model);

	carManager.Draw();

	gameManager.Draw();
}

void Game::Update()
{
	//更新
	switch (gameManager.GetStatus())
	{
	case GameStatus::TITLE:
		carManager.Update();
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
	gameManager.Update();


	if (OldScene != gameManager.GetStatus())
	{
		SceneChange();
	}
	OldScene = gameManager.GetStatus();

}

void Game::TitleUpdate()
{
	carManager.Update();
}

void Game::IngameUpdate()
{
	spawnTimer--;
	if (spawnTimer <= 0)
	{
		spawnTimer = 180;
		carManager.AddPlayerCar();
		carManager.AddEnemyCar();
	}


	carManager.SetSignal();
	carManager.Update();
	int passCarCount = carManager.GetPassCars();
	for (int i = 0; i < passCarCount; i++)
	{
		gameManager.PassCar();
	}

	if (carManager.GetAnyCarStop())
	{
		gameManager.StopCar();
	}
}


void Game::BaseInitialize()
{
	const int WindowWidth = 1280;
	const int WindowHeight = 720;

	ChangeWindowMode(true);
	SetGraphMode(WindowWidth, WindowHeight, 32);
	SetBackgroundColor(0, 0, 64);
	SetWindowText(_T("十字路の守護神"));
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
		break;
	case GameStatus::SELECT:
		break;
	case GameStatus::INGAME:
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

