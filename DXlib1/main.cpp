#include "DxLib.h"
#include <cstdlib>
#include "Vector3.h"
#include "Matrix4.h"
#include "Quaternion.h"
#include "Primitive.h"
#include "Collision.h"
#include <cstdlib>
#include "CarManager.h"
#include "GameManager.h"
//test
using namespace DxLib;
void DrawAxis3D(const float length);

void DrawFloorLine();


enum class Mode
{
	Setting,
	Moving,
};
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	const int WindowWidth = 1280;
	const int WindowHeight = 720;

	ChangeWindowMode(true);
	SetGraphMode(WindowWidth, WindowHeight, 32);
	SetBackgroundColor(0, 0, 64);
	SetWindowText(_T("十字路の守護神"));
	if (DxLib_Init() == -1)return -1;
	SetDrawScreen(DX_SCREEN_BACK);

	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	//通常

	float camZPos = -50;
	Vector3 cameraOrgPosition(0.0f, 70.0f, -130.0f + camZPos);
	Vector3 cameraOrgUp(0.0f, 1.0f, 0.0f);

	//デバッグ
	//Vector3 cameraOrgPosition(0.0f, 200.0f, 0.0f);
	//Vector3 cameraOrgUp(0.0f, 0.0f, 1.0f);

	Vector3 cameraPosition = cameraOrgPosition;
	Vector3 cameraTarget(0.0f, 0.0f, 0.0f + camZPos);

	Vector3 cameraUp = cameraOrgUp;

	float cameraRightAngle = 0.0f;
	float cameraUpAngle = 0.0f;
	//クリップ面
	SetCameraNearFar(1.0f, 10000.0f);
	SetCameraScreenCenter(WindowWidth / 2.0f, WindowHeight / 2.0f);
	SetCameraPositionAndTargetAndUpVec(cameraPosition, cameraTarget, cameraUp);

	int model = MV1LoadModel("cross_road/cross_road.mv1");
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

	Matrix4 matWorld = matScale;
	matWorld *= rotate(qLocal);
	BasePos += vForwordAxis *= (-1.0f * 0.0f);
	matWorld *= translate(BasePos);
	cameraUpAngle = 0.0f;
	cameraRightAngle = 0.0f;


	Car::LoadModel();

	CarManager carManager;
	GameManager gameManager;
	gameManager.Init();
	int spawnTimer = 60;

	SetGlobalAmbientLight(GetColorF(1.0f, 0.3f, 0.3f, 0.5f));
	//ゲームループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		int passCarCount =0;
		//更新
		switch (gameManager.GetStatus())
		{
		case GameStatus::TITLE:
			break;
		case GameStatus::SELECT:
			break;
		case GameStatus::INGAME:

			spawnTimer--;
			if (spawnTimer <= 0)
			{
				spawnTimer = 180;
				carManager.AddPlayerCar();
				carManager.AddEnemyCar();
			}

			carManager.Update();
			passCarCount = carManager.GetPassCars();
			for (int i = 0; i < passCarCount; i++)
			{
				gameManager.PassCar();
			}

			if (carManager.GetAnyCarStop())
			{
				gameManager.StopCar();
			}
			break;
		case GameStatus::RESULT:
			break;
		case GameStatus::PAUSE:
			break;
		default:
			break;
		}
		gameManager.Update();
		//描画
		ClearDrawScreen();
		DrawAxis3D(200.0f);
		DrawFloorLine();

		MV1SetMatrix(model, matWorld);
		MV1DrawModel(model);

		carManager.Draw();

		gameManager.Draw();
		//フリップする
		ScreenFlip();
	}
	DxLib_End();
	return 0;
}

void DrawAxis3D(const float length)
{
	DrawLine3D(Vector3(-length, 0, 0), Vector3(+length, 0, 0), GetColor(255, 0, 0));
	DrawLine3D(Vector3(0, -length, 0), Vector3(0, +length, 0), GetColor(0, 255, 0));
	DrawLine3D(Vector3(0, 0, -length), Vector3(0, 0, +length), GetColor(0, 0, 255));
}


void DrawFloorLine()
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
