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

void DrawKeyOperation();

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


	Quaternion qLocal = quaternion(Vector3(0.0f, 1.0f, 0.0f),0);

	Capsule targetCapsule(Vector3(-10, -20, 0), Vector3(+10, +20, 0), 10, GetColor(0, 0, 255));

	Sphere targetSphere(Vector3(-30, 0, 0), 10, GetColor(0, 0, 255));

	Box targetBox(Vector3(+10, -20, -10), Vector3(+30, +20, +10), GetColor(0, 255, 0));



	const float MOVE_UNIT = 0.1f;
	float moveX = 0.0f;
	float moveY = 0.0f;
	float moveZ = 0.0f;

	const float ROT_UNIT = 0.01f;
	float rotX = 0.0f;
	float rotY = 0.0f;
	float rotZ = 0.0f;
	Vector3 frontVec = {0, 0, 1};

	Car::LoadModel();

	CarManager carManager;
	GameManager scoreManager;
	scoreManager.Init();
	int spawnTimer = 60;

	SetGlobalAmbientLight(GetColorF(1.0f, 0.3f, 0.3f, 0.5f));
	//ゲームループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		//更新
		moveX = moveY = moveZ = 0.0f;
		rotX = rotY = rotZ = 0.0f;

		float speedRate = 0.0f;
		//if (CheckHitKey(KEY_INPUT_A)) rotY = -ROT_UNIT;
		//if (CheckHitKey(KEY_INPUT_D)) rotY = +ROT_UNIT;
		//if (CheckHitKey(KEY_INPUT_W)) speedRate = 1.0f;
		//if (CheckHitKey(KEY_INPUT_S)) speedRate = -1.0f;

		Quaternion tmpQ = quaternion(Vector3(0, 1, 0), qLocal);
		Vector3 vUpAxis = getAxis(tmpQ);
		tmpQ = quaternion(Vector3(0, 0, 1), qLocal);
		Vector3 vForwordAxis = getAxis(tmpQ);
		Quaternion rot = quaternion(Vector3(0, 1, 0), rotY);
		qLocal = rot * qLocal;
		Matrix4 matWorld = matScale;
		matWorld *= rotate(qLocal);
		BasePos += vForwordAxis *= (-1.0f * speedRate);
		matWorld *= translate(BasePos);
		cameraUpAngle = 0.0f;
		cameraRightAngle = 0.0f;

		//if (CheckHitKey(KEY_INPUT_A))cameraRightAngle = -ROT_UNIT;
		//if (CheckHitKey(KEY_INPUT_D))cameraRightAngle = +ROT_UNIT;

		//if (CheckHitKey(KEY_INPUT_UP))cameraUpAngle = -ROT_UNIT;
		//if (CheckHitKey(KEY_INPUT_DOWN))cameraUpAngle = +ROT_UNIT;

		//Quaternion q = qLocal;

		//Quaternion qCameraPosition = quaternion(cameraOrgPosition.x, cameraOrgPosition.y, cameraOrgPosition.z, 0.0f);

		//Quaternion qq = conjugate(q);

		//tmpQ = q * qCameraPosition;
		// qCameraPosition = (tmpQ * qq);

		//cameraPosition = { qCameraPosition.x, qCameraPosition.y , qCameraPosition.z };
		//cameraPosition += BasePos;
		//Quaternion qCameraUp = quaternion(cameraUp.x, cameraUp.y, cameraUp.z, 0.0f);

		//tmpQ = q * qCameraUp;
		//qCameraUp = (tmpQ * qq);

		//cameraUp = getAxis(qCameraUp);

		//SetCameraPositionAndTargetAndUpVec(cameraPosition, BasePos, cameraUp);





		sphere.position = BasePos;

		targetCapsule.color = GetColor(0, 0, 255);
		if (Collision::testSphereCapsule(sphere, targetCapsule))
		{
			targetCapsule.color = GetColor(255, 0, 0);
		}
		targetBox.color = GetColor(0, 255, 0);
		if (Collision::testSphereBox(sphere, targetBox))
		{
			targetBox.color = GetColor(255, 0, 0);
		}
		targetSphere.color = GetColor(255, 255, 0);
		if (Collision::testSphereSphere(sphere, targetSphere))
		{
			targetSphere.color = GetColor(255, 0, 0);
		}

		spawnTimer--;
		if (spawnTimer <= 0)
		{
			spawnTimer = 180;
			carManager.AddPlayerCar();
			carManager.AddEnemyCar();
		}

		carManager.Update();
		int passCarCount = carManager.GetPassCars();
		for (int i = 0; i < passCarCount; i++)
		{
			scoreManager.PassCar();
		}

		if (carManager.GetAnyCarStop())
		{
			scoreManager.StopCar();
		}
		//描画
		ClearDrawScreen();
		DrawAxis3D(200.0f);
		DrawFloorLine();

		MV1SetMatrix(model, matWorld);
		MV1DrawModel(model);
		DrawKeyOperation();

		carManager.Draw();

		scoreManager.Draw();
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

void DrawKeyOperation()
{
	const unsigned white = GetColor(255, 255, 255);

	DrawFormatString(10, 20 * 1, white, "  [W]       A / D:カプセルのY軸回転");
	DrawFormatString(10, 20 * 2, white, "[A][S][D]   W / S:カプセルの前後移動");
	//DrawFormatString(10, 20 * 3, white, "               W / S:カプセルの前後移動");
	//DrawFormatString(10, 20 * 4, white, "               E / Z:カプセルの上下移動");
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
